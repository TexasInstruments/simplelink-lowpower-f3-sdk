from __future__ import annotations

import os
import queue
import time
import threading
import json
import logging
import enum

from logging.handlers import RotatingFileHandler
from dataclasses import dataclass
from construct import Struct, Int16sl, Int8ul, Int16ul, Int32ul, this, Array, Byte, Enum, Int8sl
from unpi.unpiparser import NiceBytes, ReverseBytes
from rtls import RTLSManager, RTLSNode
from ble_device.ble_device_enums import (
    CommonEventType,
    PeripheralCommands,
    ConnectionCommands,
    CentralCommands,
    HandoverCommands,
    CsCommands,
    CmCommands,
    PairingCommands,
    GATTCommands,
    CaServerCommands,
    AppSpecifier,
    CentralEventType,
    PeripheralEventType,
    HandoverEventType,
    L2CAPEventType,
    CmEventType,
    AddressMode,
    ConnectionEventType,
    CsEventType,
    PairingEventType,
    CommonCommands,
    L2CAPCommands,
    ConnectionPhysPreference,
    ConnectionPhyOpts,
    ConnectionEventNotifyType,
    ConnectionEventTaskType,
    ConnectionPhy,
    ConnectionEventStatus,
    CmConnUpdateType,
    GATTEventType,
    CaServerEventType,
    BondReadTypes
)

from ble_device.ble_device_exception import *


class EventData:
    def __init__(self, count=0, last_data=None, max_event_list_size=50):
        self.count: int = count
        self.last_data = last_data
        self.max_event_list_size = max_event_list_size
        self.event_list_data = []

    def update(self, data_from_event):
        self.count += 1
        self.last_data = data_from_event
        self.event_list_data.append(data_from_event)

        if len(self.event_list_data) > self.max_event_list_size:
            self.event_list_data.pop(0)

    def to_dict(self):
        return {
            "count": self.count,
            "last_data": self.last_data,
            "event_list_data": self.event_list_data,
        }

    def __repr__(self):
        return json.dumps(self.to_dict(), indent=4)


class EventCounter:
    def __init__(self, event_type_class, max_event_list_size):
        self.events = {}
        self.enum_events = event_type_class
        self.max_event_list_size = max_event_list_size

        for event in self.enum_events:
            self.events[event.name] = EventData(
                max_event_list_size=self.max_event_list_size
            )

    def increment_event(self, event_value, data_from_event):
        event_name = self.enum_events(event_value).name

        if event_name in self.events:
            self.events[event_name].update(data_from_event)

        else:
            raise ValueError(f"Invalid event type: {event_value}")

    def get_event(self, event_value) -> EventData:
        event_name = self.enum_events(event_value).name
        return self.events.get(event_name, None)

    def get_event_count(self, event_value) -> int:
        event_name = self.enum_events(event_value).name
        return self.events.get(event_name, None).count

    def reset_all(self):
        for event_type in self.events:
            self.events[event_type] = EventData(self.max_event_list_size)

    def to_dict(self):
        return {
            event_name: event_data.to_dict()
            for event_name, event_data in self.events.items()
        }

    def __repr__(self):
        return json.dumps(self.to_dict(), indent=4)


class EventList:
    def __init__(self, max_list_size):
        self.events_list = []
        self.max_list_size = max_list_size

    def is_event_in_list(self, event_type):
        return any([e["event_type"] == event_type for e in self.events_list])

    def add_event(self, event_type, data_from_event):
        self.events_list.append(
            dict(event_type=event_type, data_from_event=data_from_event)
        )
        # Remove the oldest event if list is full
        if len(self.events_list) > self.max_list_size:
            self.events_list.pop(0)

    def get_event_from_list(self, event_type):
        copy_events_list = self.events_list[:]
        event_data = None

        for index, e in enumerate(copy_events_list):
            if e["event_type"] == event_type:
                event_data = e["data_from_event"]
                break
        else:
            return None

        self.events_list.pop(index)
        return event_data

    def clear(self):
        self.events_list = []

    def __repr__(self):
        return json.dumps(self.events_list, indent=4)


@dataclass
class BleDeviceLoggingLevel:
    INFO = 20
    DEBUG = 10
    ALL = 0
    NONE = -1


class BleDeviceBasic:
    def __init__(self, logger, event_type_class, max_event_list_size):
        self.logger = logger
        self.event_type_class = event_type_class
        self.max_event_list_size = max_event_list_size

        self._timeout = 30
        self.timeout = self.timeout

        self.ble_device = None
        self.app_specifier = None
        self.cmd = None
        self.data_struct = None
        self.events_counter = EventCounter(
            event_type_class=self.event_type_class,
            max_event_list_size=self.max_event_list_size,
        )

    @property
    def timeout(self):
        return self._timeout

    @timeout.setter
    def timeout(self, value):
        self._timeout = value

    def add_user_log(self, msg, print_to_terminal=True):
        if self.logger:
            self.logger.info(msg)

        if print_to_terminal:
            print(msg)

    def ble_device_wait(self, true_cond_func, nodes, timeout_message):
        timeout = time.time() + self._timeout
        timeout_reached = time.time() > timeout

        while not true_cond_func(nodes) and not timeout_reached:
            time.sleep(0.1)
            timeout_reached = time.time() > timeout

        if timeout_reached:
            raise BleDeviceTimeoutException(
                f"Timeout reached while waiting for : {timeout_message}"
            )

    def _container_to_dict(self, container):
        if isinstance(container, dict):
            return {k: self._container_to_dict(v) for k, v in container.items()}
        elif isinstance(container, list):
            return [self._container_to_dict(e) for e in container]
        elif hasattr(container, "__iter__") and not isinstance(container, (str, bytes)):
            return [self._container_to_dict(e) for e in container]
        else:
            return container

    def parse_struct(self, data_struct, msg):
        return self._container_to_dict(data_struct.parse(msg.data))

    def send_nwp_cmd(self, data_to_send):
        data_bytes = (
            b"" if self.data_struct is None else self.data_struct.build(data_to_send)
        )
        data_len = len(data_bytes)
        self.ble_device.device_node.rtls.nwpAsyncCmdReq(
            self.app_specifier, self.cmd, data_len, data_bytes
        )

    def wait_for_event(self, event_type, timeout, total_event_count=1):
        if event_type not in set(
            item.value for item in self.events_counter.enum_events
        ):
            raise ValueError(
                f"Event type {event_type} is not member in {self.events_counter.enum_events}"
            )

        _timeout = time.time() + timeout
        timeout_reached = time.time() > _timeout

        while (
            self.events_counter.get_event_count(event_type) < total_event_count
            and not timeout_reached
        ):
            time.sleep(1e-12)
            timeout_reached = time.time() > _timeout

        if timeout_reached:
            self.add_user_log(
                f"Timeout reached after {timeout} sec while waiting for event type {event_type}"
            )
            return None

        event = self.events_counter.get_event(event_value=event_type)
        data = event.last_data if event else None

        return data


class BleDevice:
    def __init__(
        self,
        device_comport,
        logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        websocket_port=None,
        max_log_file_size=100,
        sync_command=True,
        max_event_list_size=50,
        default_timeout=10,
    ):
        self.logger = None
        self.logger_fh = None

        self.logging_level = logging_level
        if not (self.logging_level == BleDeviceLoggingLevel.NONE):
            self.logger = logging.getLogger()
            self.logger.setLevel(self.logging_level)

            self.logger_fh = RotatingFileHandler(
                logging_file, maxBytes=max_log_file_size * 1024 * 1024, backupCount=100
            )
            self.logger_fh.setLevel(self.logging_level)

            formatter = logging.Formatter(
                "[%(asctime)s] %(name)9s - %(levelname)8s - %(message)s"
            )
            self.logger_fh.setFormatter(formatter)

            # Messages can be filter by logger name
            # blank means all all messages
            # filter = logging.Filter()
            # self.logger_fh.addFilter(filter)

            self.logger.addHandler(self.logger_fh)

        self.logging_file = logging_file
        self._node_info = None
        self.device_node: RTLSNode | None = None
        self._device_info = None
        self.device_comport = device_comport
        self.device_comport_baud_rate = 460800
        self.sync_command = sync_command
        self.max_event_list_size = max_event_list_size
        self.default_timeout = default_timeout

        self.common: BleDeviceCommon | None = None
        self.peripheral: BleDevicePeripheral | None = None
        self.central: BleDeviceCentral | None = None
        self.connection: BleDeviceConnection | None = None
        self.handover: BleDeviceHandover | None = None
        self.pairing: BleDevicePairing | None = None
        self.l2cap: BleDeviceL2CAP | None = None
        self.cs: BleDeviceCs | None = None  # channel sounding
        self.cm: BleDeviceCm | None = None  # Connection Monitor
        self.gatt: BleDeviceGATT | None = None
        self.ca_server: BleDeviceCaServer | None = None

        self.addr_mode = None
        self.id_addr = None
        self.rp_addr = None

        self._rtls_manager = None
        self._rtls_manager_subscriber = None

        self._message_receiver_th = None
        self._message_receiver_stop = False

        self._coordinator_disconnected = threading.Event()
        self._coordinator_disconnected.clear()

        self.custom_message_queue = queue.Queue()

        self.custom_message_filter = None

        self.websocket_port = websocket_port

        self.all_event_list = EventList(max_list_size=self.max_event_list_size)
        self.unknown_event_list = EventList(max_list_size=self.max_event_list_size)

        self._wait_any_event_data = dict(event_type=None, data=None)
        self.is_wait_any_event = threading.Event()
        self.is_wait_any_event.clear()

    def __del__(self):
        self.done()

    def add_user_log(self, msg, print_to_terminal=False):
        if self.logger:
            self.logger.log(self.logging_level, msg)

        if print_to_terminal:
            print(msg)

    def _get_parameters(self):
        parameters = "\n"
        parameters += "Example Input Parameters\n"
        parameters += (
            "----------------------------------------------------------------------\n"
        )
        parameters += (
            f"Device comport                          : {self.device_comport}\n"
        )
        parameters += f"Example Log Dir                         : {os.path.dirname(self.logging_file)}\n"
        parameters += f"Example Log File Name                   : {os.path.basename(self.logging_file)}\n"
        parameters += (
            "----------------------------------------------------------------------\n"
        )
        parameters += "\n"

        return parameters

    def _get_address(self):
        # Extract and print the address of the device to the terminal
        self.common.get_address()
        address_event = self.common.wait_for_event(
            event_type=CommonEventType.NWP_COMMON_DEVICE_ADDRESS,
            timeout=self.default_timeout,
        )
        if address_event is not None:
            self.addr_mode = AddressMode(address_event["address_mode"]).name
            self.id_addr = address_event["id_address"]
            self.rp_addr = (
                address_event["rp_address"]
                if AddressMode(address_event["address_mode"])
                > AddressMode.ADDRMODE_RANDOM
                else None
            )

            adress_str = "\n"
            adress_str += f"Device Address Mode   : {AddressMode(address_event['address_mode']).name}\n"
            adress_str += f"ID Address            : {address_event['id_address']}\n"
            adress_str += f"RP Address            : {address_event['rp_address']}\n"
            adress_str += "----------------------------------------------------------------------\n"
            adress_str += "\n"

            self.add_user_log(adress_str)

    def initialize(self):
        try:
            self.add_user_log(self._get_parameters())

            self._device_info = {
                "com_port": self.device_comport,
                "baud_rate": self.device_comport_baud_rate,
                "name": "BLE Device",
            }
            self.add_user_log(f"Device info : {self._device_info}")

            self.set_device(self._device_info)
            self.add_user_log(
                f"Capability: {[str(c) for c, e in self.device_node.capabilities.items() if e]}"
            )

            self.common = BleDeviceCommon(
                logger=self.logger,
                ble_device=self,
                sync_command=self.sync_command,
                max_event_list_size=self.max_event_list_size,
            )
            self._get_address()

            if self.device_node.capabilities.get("RTLS_CAP_CENTRAL", False):
                self.central = BleDeviceCentral(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_PERIPHERAL", False):
                self.peripheral = BleDevicePeripheral(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_HANDOVER", False):
                self.handover = BleDeviceHandover(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_CONNECTION", False):
                self.connection = BleDeviceConnection(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_PAIRING", False):
                self.pairing = BleDevicePairing(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_CS", False):
                self.cs = BleDeviceCs(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_L2CAPCOC", False):
                self.l2cap = BleDeviceL2CAP(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("CM", False):
                self.cm = BleDeviceCm(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_GATT", False):
                self.gatt = BleDeviceGATT(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            if self.device_node.capabilities.get("RTLS_CAP_CA_SERVER", False):
                self.ca_server = BleDeviceCaServer(
                    logger=self.logger,
                    ble_device=self,
                    sync_command=self.sync_command,
                    max_event_list_size=self.max_event_list_size,
                )

            return True
        
        except BleDeviceException as ex:
            print(f"Error: {ex}")
            return False

        except BleDeviceNodesNotIdentifiedException:
            print("The given device is not identified as RTLS type")
            return False

    def done(self):
        if self._message_receiver_th is not None:
            self._message_receiver_stop = True
            self._message_receiver_th.join()
            self._message_receiver_th = None

        if self._rtls_manager:
            self._rtls_manager.stop()
            self._rtls_manager.join()

            self._rtls_manager_subscriber = None
            self._rtls_manager = None

        if self.logger_fh:
            self.logger_fh.close()
            self.logger.removeHandler(self.logger_fh)

    def _log_incoming_msg(self, item, identifier):
        json_item = json.loads(item.as_json())

        json_item["type"] = "Response" if json_item["type"] == "SyncRsp" else "Event"

        # Filtering out "originator" and "subsystem" fields
        new_dict = {
            k: v
            for (k, v) in json_item.items()
            if k != "originator"
            if k != "subsystem"
        }

        # Get reference to RTLSNode based on identifier in message
        sending_node = self._rtls_manager[identifier]

        event_type = new_dict["payload"]["event_type"]
        event_desc = ""
        if event_type:
            for _enum in [
                CentralEventType,
                PeripheralEventType,
                ConnectionEventType,
                PairingEventType,
                HandoverEventType,
                L2CAPEventType,
                CsEventType,
                CommonEventType,
                CmEventType,
                GATTEventType,
                CaServerEventType
            ]:
                for member in _enum:
                    if member.value == event_type:
                        event_desc = member.name
        if sending_node is self.device_node:
            self.add_user_log(f"NODE : {identifier} {event_desc} --> {new_dict}")

    def _message_receiver(self):
        while not self._message_receiver_stop:
            # Get messages from manager
            try:
                identifier, msg_pri, msg = self._rtls_manager_subscriber.pend(
                    block=True, timeout=0.05
                ).as_tuple()

                self._log_incoming_msg(msg, identifier)
                sending_node = self._rtls_manager[identifier]

                if (
                    self.custom_message_filter
                    and msg.command in self.custom_message_filter
                ):
                    self.custom_message_queue.put(
                        {
                            "name": sending_node.name,
                            "identifier": identifier,
                            "msg": msg,
                        }
                    )

                if not sending_node.identifier == self.device_node.identifier:
                    return

                event_type = msg.payload.event_type
                parsed_data = msg.payload.data

                if self.central and event_type in set(
                    item.value for item in CentralEventType
                ):
                    parsed_data = self.central.message_parser(msg)

                elif self.peripheral and event_type in set(
                    item.value for item in PeripheralEventType
                ):
                    parsed_data = self.peripheral.message_parser(msg)

                elif self.connection and event_type in set(
                    item.value for item in ConnectionEventType
                ):
                    parsed_data = self.connection.message_parser(msg)

                elif self.handover and event_type in set(
                    item.value for item in HandoverEventType
                ):
                    parsed_data = self.handover.message_parser(msg)

                elif self.pairing and event_type in set(
                    item.value for item in PairingEventType
                ):
                    parsed_data = self.pairing.message_parser(msg)

                elif self.cs and event_type in set(
                    item.value for item in CsEventType
                ):
                    parsed_data = self.cs.message_parser(msg)

                elif self.l2cap and event_type in set(
                    item.value for item in L2CAPEventType
                ):
                    parsed_data = self.l2cap.message_parser(msg)

                elif self.common and event_type in set(
                    item.value for item in CommonEventType
                ):
                    parsed_data = self.common.message_parser(msg)

                elif self.cm and event_type in set(
                    item.value for item in CmEventType
                ):
                    parsed_data = self.cm.message_parser(msg)

                elif self.gatt and event_type in set(
                    item.value for item in GATTEventType
                ):
                    parsed_data = self.gatt.message_parser(msg)

                elif self.ca_server and event_type in set(
                    item.value for item in CaServerEventType
                ):
                    parsed_data = self.ca_server.message_parser(msg)

                else:
                    self.unknown_event_list.add_event(event_type, parsed_data)

                self.all_event_list.add_event(event_type, parsed_data)

                if self.is_wait_any_event.is_set():
                    self._wait_any_event_data = dict(
                        event_type=event_type, data=parsed_data
                    )
                    self.is_wait_any_event.clear()

            except queue.Empty:
                pass

    def _start_message_receiver(self):
        self._message_receiver_stop = False
        self._message_receiver_th = threading.Thread(target=self._message_receiver)
        self._message_receiver_th.setDaemon(True)
        self._message_receiver_th.start()

    def set_device(self, device_setting):
        try:
            node = RTLSNode(
                device_setting["com_port"],
                device_setting["baud_rate"],
                device_setting["name"],
            )
            self.add_user_log(f"Setting nodes : {device_setting}")

            self._rtls_manager = RTLSManager([node], websocket_port=self.websocket_port)
            self._rtls_manager_subscriber = self._rtls_manager.create_subscriber()
            self._rtls_manager.auto_params = True

            self._start_message_receiver()
            self.add_user_log("Message receiver started")

            self._rtls_manager.start()
            self.add_user_log("RTLS manager started")
            time.sleep(2)
            _ble_agent_node, failed = self._rtls_manager.wait_identified(
                is_ble_agent=True
            )
            
            print(_ble_agent_node, failed)

            if len(failed) > 0:
                raise BleDeviceNodesNotIdentifiedException(
                    "{} nodes not identified at all".format(len(failed)), failed
                )
            else:
                pass

            _ble_agent_node.ble_connected = False
            _ble_agent_node.device_resets = False

            self.device_node = _ble_agent_node
            self.add_user_log("Done setting node")

            return _ble_agent_node
        except OSError:
            raise BleDeviceException("RTLS manager fail to start")

    def wait_for_any_event(self, timeout):
        self._wait_any_event_data = dict(event_type=None, data=None)
        self.is_wait_any_event.set()

        _timeout = time.time() + timeout
        timeout_reached = time.time() > _timeout

        while self.is_wait_any_event.is_set() and not timeout_reached:
            time.sleep(1e-12)
            timeout_reached = time.time() > _timeout

        if timeout_reached:
            self.add_user_log(
                f"Timeout reached after {timeout} sec while waiting for any event"
            )
            return None, None

        return (
            self._wait_any_event_data["event_type"],
            self._wait_any_event_data["data"],
        )

    def wait_for_event(self, event_type, timeout):
        _timeout = time.time() + timeout
        timeout_reached = time.time() > _timeout

        while (
            not self.all_event_list.is_event_in_list(event_type) and not timeout_reached
        ):
            time.sleep(1e-12)
            timeout_reached = time.time() > _timeout

        if timeout_reached:
            self.add_user_log(
                f"Timeout reached after {timeout} sec while waiting for event type {event_type}"
            )
            return None

        event_data = self.all_event_list.get_event_from_list(event_type)

        return event_data

    def identify(self):
        node = RTLSNode(
            self.device_comport, self.device_comport_baud_rate, "BLE Device"
        )
        _rtls_manager = RTLSManager([node])
        _rtls_manager_subscriber = _rtls_manager.create_subscriber()
        _rtls_manager.auto_params = True

        _rtls_manager.start()
        time.sleep(2)
        _ble_agent_node, failed = _rtls_manager.wait_identified(is_ble_agent=True)

        if len(failed) > 0:
            return None

        return _ble_agent_node

    def get_rtt(self):
        return self.device_node.rtt


class BleDeviceCommon(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=CommonEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.app_specifier = AppSpecifier.APP_SPECIFIER_COMMON

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data

        if event_type == CommonEventType.NWP_COMMON_DEVICE_ADDRESS:
            data_struct = Struct(
                "event" / Int16ul,
                "address_mode" / Int8ul,
                "id_address" / NiceBytes(ReverseBytes(Byte[6])),
                "rp_address" / NiceBytes(ReverseBytes(Byte[6])),
            )
        elif event_type == CommonEventType.NWP_COMMON_COMMAND_STATUS_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "cmd_status" / Int8ul,
                "num_hci_cmd_pkt" / Int8ul,
                "cmd_opcode" / Int16ul,
            )
        elif event_type == CommonEventType.NWP_COMMON_HCI_COMPLETE_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "ble_event_code" / Int16ul,
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul),
            )

        elif event_type == CommonEventType.NWP_COMMON_HCI_LE_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "ble_event_code" / Int8ul,
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul),
            )

        # Add event to queue
        parsed_data = self.parse_struct(data_struct, msg)
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data

    def get_address(self):
        self.cmd = CommonCommands.COMMON_CMD_GET_DEV_ADDR
        self.send_nwp_cmd(None)
    
    def reset_device(self):
        self.cmd = CommonCommands.COMMON_CMD_RESET_DEVICE
        self.send_nwp_cmd(None)


class BleDevicePeripheral(BleDeviceBasic):
    class AdvOptions(enum.IntEnum):
        GAP_ADV_ENABLE_OPTIONS_USE_MAX = 0
        GAP_ADV_ENABLE_OPTIONS_USE_DURATION = 1
        GAP_ADV_ENABLE_OPTIONS_USE_MAX_EVENTS = 2

    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=PeripheralEventType,
            max_event_list_size=max_event_list_size,
        )

        self.is_advertising = False
        self.ble_device = ble_device
        self.sync_command = sync_command
        self.app_specifier = AppSpecifier.APP_SPECIFIER_PERIPHERAL

    def message_parser(self, msg):
        parsed_data = msg.payload.data
        event_type = msg.payload.event_type
        if event_type == PeripheralEventType.NWP_ADV_START_AFTER_ENABLE:
            self.is_advertising = True
            parsed_data = {"adv_handle": msg.payload.data}

        elif event_type == PeripheralEventType.NWP_ADV_END_AFTER_DISABLE:
            self.is_advertising = False
            parsed_data = {"adv_handle": msg.payload.data}

        else:
            parsed_data = {"data": msg.payload.data}

        # Add event to queue
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )
        return parsed_data

    def start_advertising(
        self, adv_handle=0x00, enable_options=0x00, duration_or_max_events=0x0000
    ):
        self.is_advertising = False
        self.cmd = PeripheralCommands.PERI_CMD_ADVERTISE_START
        self.data_struct = Struct(
            "duration_or_max_events" / Int16ul,
            "enable_options" / Int8ul,
            "adv_handle" / Int8ul,
        )
        self.send_nwp_cmd(
            dict(
                duration_or_max_events=duration_or_max_events,
                enable_options=enable_options,
                adv_handle=adv_handle,
            )
        )

        if self.sync_command:
            true_cond_func = lambda ble_device: ble_device.is_advertising
            self.ble_device_wait(true_cond_func, self, "Advertise started")

    def stop_advertising(self, adv_handle=0x00):
        self.cmd = PeripheralCommands.PERI_CMD_ADVERTISE_STOP
        self.data_struct = Struct("adv_handle" / Int8ul)
        self.send_nwp_cmd(dict(adv_handle=adv_handle))

        if self.sync_command:
            true_cond_func = lambda ble_device: not ble_device.is_advertising
            self.ble_device_wait(true_cond_func, self, "Advertise disabled")


class BleDeviceCentral(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=CentralEventType,
            max_event_list_size=max_event_list_size,
        )

        self.is_scanning = False
        self.ble_device = ble_device
        self._last_scan_reports = []
        self.sync_command = sync_command
        self.app_specifier = AppSpecifier.APP_SPECIFIER_CENTRAL

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        if event_type == CentralEventType.NWP_SCAN_ENABLED:
            self.is_scanning = True
            self.add_user_log(f"{self.ble_device.device_node.identifier}: scan enabled")

        elif event_type == CentralEventType.NWP_SCAN_DISABLED:
            self.is_scanning = False

            report_struct = Struct(
                "address_type" / Int8ul, "bd_address" / Array(6, Int8ul)
            )

            data_struct = Struct(
                "event" / Int16ul,
                "reason" / Int8ul,
                "numReport" / Int8ul,
                "data" / Array(this.numReport, report_struct),
            )
            self.add_user_log(
                f"{self.ble_device.device_node.identifier}: scan disabled"
            )
            self._last_scan_reports = self.parse_struct(data_struct, msg)["data"]
            parsed_data = self._last_scan_reports

        elif event_type == CentralEventType.NWP_ADV_REPORT:
            data_struct =  Struct(
                "event" / Int16ul,
                "address_type" / Int8ul,
                "address" / Array(6, Int8ul),
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul)
            )
            self._last_scan_reports = self.parse_struct(data_struct, msg)
            parsed_data = self._last_scan_reports

        # Add to event counter
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data

    def start_scan(self, scan_period=0x0000, scan_duration=0x0064, max_num_report=0x0A):
        self.cmd = CentralCommands.CENTRAL_CMD_SCAN_START
        self.data_struct = Struct(
            "scan_period" / Int16ul,
            "scan_duration" / Int16ul,
            "max_num_report" / Int8ul,
        )

        self.send_nwp_cmd(
            dict(
                scan_period=scan_period,
                scan_duration=scan_duration,
                max_num_report=max_num_report,
            )
        )

        if self.sync_command:
            true_cond_func = lambda ble_device: ble_device.is_scanning
            self.ble_device_wait(true_cond_func, self, "Scan enable")

    def stop_scan(self):
        self.cmd = CentralCommands.CENTRAL_CMD_SCAN_STOP
        self.data_struct = None
        self.send_nwp_cmd(None)

        if self.sync_command:
            true_cond_func = lambda ble_device: not ble_device.is_scanning
            self.ble_device_wait(true_cond_func, self, "Scan disabled")

    def get_scan_results(self):
        return self._last_scan_reports

    def connect(
        self,
        address_type=0,
        address=[0x11, 0x22, 0x33, 0x44, 0x55, 0x66],
        phy=1,
        timeout=0,
    ):
        self.cmd = CentralCommands.CENTRAL_CMD_CONNECT
        self.data_struct = Struct(
            "address_type" / Int8ul,
            "address" / Array(6, Int8ul),
            "phy" / Int8ul,
            "timeout" / Int16ul,
        )

        self.send_nwp_cmd(
            dict(address_type=address_type, address=address, phy=phy, timeout=timeout)
        )

        if self.sync_command:
            true_cond_func = lambda ble_device: ble_device.is_connected
            self.ble_device_wait(
                true_cond_func, self.ble_device.connection, "Connection Established"
            )


class BleDeviceConnection(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=ConnectionEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.connections = []
        self.sync_command = sync_command
        self.event_notify_enabled = False
        self._last_connection_data = None
        self.app_specifier = AppSpecifier.APP_SPECIFIER_CONNECTION
        self.is_connected = False

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data

        if event_type == ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "status" / Int8ul,
                "opcode" / Int8ul,
                "address_type" / Int8ul,
                "dev_address" / Array(6, Int8ul),
                "connection_handle" / Int16ul,
                "connection_role" / Int8ul,
                "connection_interval" / Int16ul,
                "connection_latency" / Int16ul,
                "connection_timeout" / Int16ul,
                "clock_accuracy" / Int8ul,
            )
            self._last_connection_data = self.parse_struct(data_struct, msg)
            self.is_connected = True
            self.connections.append(self._last_connection_data)
            parsed_data = self._last_connection_data

        elif event_type == ConnectionEventType.NWP_LINK_TERMINATED_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "status" / Int8ul,
                "opcode" / Int8ul,
                "connection_handle" / Int16ul,
                "reason" / Int8ul,
            )
            disconnected_data = self.parse_struct(data_struct, msg)
            parsed_data = disconnected_data
            self.connections = [
                c
                for c in self.connections
                if c["connection_handle"] != disconnected_data["connection_handle"]
            ]

        elif event_type == ConnectionEventType.NWP_CONN_NOTI_CONN_EVENT:
            self.event_notify_enabled = True
            data_struct = Struct(
                "event"  / Int16ul,
                "status" / Enum(Int8ul, ConnectionEventStatus),
                "connection_handle" / Int16ul,
                "channel" / Int8ul,
                "phy"     / Enum(Int8ul, ConnectionPhy),
                "last_rssi" / Int8sl,
                "packets" / Int16ul,
                "errors"  / Int16ul,
                "next_task_type" / Enum(Int16ul, ConnectionEventTaskType),
                "next_task_time" / Int32ul,
                "event_counter"  / Int16ul,
                "time_stamp"     / Int32ul,
                "event_type"     / Enum(Int8ul, ConnectionEventNotifyType),
            )

            event_report = self.parse_struct(data_struct, msg)
            parsed_data = event_report

        # Add event to queue
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data

    def get_connections(self):
        return self.connections

    def register_connection_event(self,
                                  connection_handle=0xFFFD,
                                  event_type = ConnectionEventNotifyType.CONNECTION_EVENT_ALL,
                                  report_frequency=0x0A):
        '''
        @brief Register for connection event notification.
        @param connection_handle: Connection handle to register for event notification,
                                  use 0xFFFD to register for all connections.
        @param event_type: Type of event to register for notification.
        @param report_frequency: Frequency of event notification in number of connection events.
        '''
        self.event_notify_enabled = False
        self.cmd = ConnectionCommands.CONNECTION_CMD_REGISTER_CONN_EVENT
        self.data_struct = Struct(
            "connection_handle" / Int16ul,
            "event_type" / Int8ul,
            "report_frequency" / Int8ul,
        )

        self.send_nwp_cmd(
            dict(
                connection_handle=connection_handle,
                report_frequency=report_frequency,
                event_type=event_type
            )
        )

        if self.sync_command:
            true_cond_func = lambda ble_device: ble_device.event_notify_enabled
            self.ble_device_wait(true_cond_func, self, "event call back notification enabled")

    def unregister_connection_event(self):
        self.cmd = ConnectionCommands.CONNECTION_CMD_UNREGISTER_CONN_EVENT
        self.data_struct = None
        self.send_nwp_cmd(None)
        self.event_notify_enabled = False

    def set_connection_phy(self,
                           connection_handle = 0x00,
                           phy_opts = ConnectionPhyOpts.CONNECTION_PHY_OPT_NONE,
                           all_phys = ConnectionPhysPreference.CONNECTION_USE_TX_RX_PHY_PARAM,
                           tx_phy = ConnectionPhy.CONNECTION_PHY_2M,
                           rx_phy = ConnectionPhy.CONNECTION_PHY_2M):
        '''
        @brief Set PHY preferences for a connection.
        @param connection_handle: Connection handle to set PHY preferences.
        @param phy_opts: PHY options.
        @param all_phys: Phy prefrences.
        @param tx_phy: Preferred transmit PHY.
        @param rx_phy: Preferred receive PHY.
        '''
        self.cmd = ConnectionCommands.CONNECTION_CMD_SET_PHY
        self.data_struct = Struct(
            "connection_handle" / Int16ul,
            "phy_opts" / Int16ul,
            "all_phys" / Int8ul,
            "tx_phy" / Int8ul,
            "rx_phy" / Int8ul,
        )

        self.send_nwp_cmd(
            dict(
                connection_handle=connection_handle,
                phy_opts=phy_opts,
                all_phys=all_phys,
                tx_phy=tx_phy,
                rx_phy=rx_phy,
            )
        )

    def terminate_link(self, connection_handle):
        self.cmd = ConnectionCommands.CONNECTION_CMD_TERMINATE_LINK
        self.data_struct = Struct(
            "connection_handle" / Int16ul
        )

        self.send_nwp_cmd(
            dict(connection_handle=connection_handle)
        )


class BleDevicePairing(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=PairingEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.pairing = []
        self._last_pairing_data = None
        self.max_num_char_cfg = 0

        self.app_specifier = AppSpecifier.APP_SPECIFIER_PAIRING

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        if event_type == PairingEventType.NWP_PAIRING_MAX_NUM_CHAR_CFG:
            data_struct = Struct(
                "event" / Int16ul,
                "max_num_char_cfg" / Int8ul
            )
            parsed_data = self.parse_struct(data_struct, msg)
            self.max_num_char_cfg = parsed_data['max_num_char_cfg']

        if event_type == PairingEventType.NWP_PAIRING_STATE_COMPLETE:
            data_struct = Struct(
                "event" / Int16ul,
                "connection_handle" / Int16ul,
                "state" / Int8ul,
                "status" / Int8ul,
            )
            self._last_pairing_data = self.parse_struct(data_struct, msg)
            self.pairing.append(self._last_pairing_data)
            parsed_data = self._last_pairing_data

        elif event_type == PairingEventType.NWP_PAIRING_STATE_ENCRYPTED:
            data_struct = Struct(
                "event" / Int16ul,
                "connection_handle" / Int16ul,
                "state" / Int8ul,
                "status" / Int8ul,
            )
            self._last_pairing_data = self.parse_struct(data_struct, msg)
            self.pairing.append(self._last_pairing_data)
            parsed_data = self._last_pairing_data

        elif event_type == PairingEventType.NWP_PAIRING_READ_BOND:
            data_struct = Struct(
                "event" / Int16ul,
                "peer_address" / Array(6, Int8ul),
                "peer_address_type" / Int8ul,
                "state_flags" / Int8ul,
                "local_ltk" / Array(16, Int8ul),
                "local_div" / Int16ul,
                "local_rand" / Array(8, Int8ul),
                "local_key_size" / Int8ul,
                "peer_ltk" / Array(16, Int8ul),
                "peer_div" / Int16ul,
                "peer_rand" / Array(8, Int8ul),
                "peer_key_size" / Int8ul,
                "irk" / Array(16, Int8ul),
                "srk" / Array(16, Int8ul),
                "sign_count" / Int32ul,
                "char_cfg" / Array(3 * self.max_num_char_cfg, Int8ul)
            )
            self._last_pairing_data = self.parse_struct(data_struct, msg)
            self.pairing.append(self._last_pairing_data)
            parsed_data = self._last_pairing_data

        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )
        return parsed_data


    def get_max_num_char_cfg(self):
        self.cmd = PairingCommands.PAIRING_CMD_GET_MAX_NUM_CHAR_CFG
        self.send_nwp_cmd(None)


    def read_bond_from_nv(self, mode=BondReadTypes.READ_BOND_BY_INDEX,
                          peer_address=None, peer_address_type=0x00):
        self.cmd = PairingCommands.PAIRING_CMD_READ_BOND
        self.data_struct = Struct(
            'mode' / Int8ul,
            'peer_address' / Array(6, Int8ul),
            'peer_address_type' / Int8ul
        )
        self.send_nwp_cmd(dict(
            mode=mode,
            peer_address=peer_address,
            peer_address_type=peer_address_type
        ))

    def write_bond_to_nv(self,
                         peer_address=None,
                         peer_address_type=None,
                         state_flags=None,
                         local_ltk=None,
                         local_div=None,
                         local_rand=None,
                         local_key_size=None,
                         peer_ltk=None,
                         peer_div=None,
                         peer_rand=None,
                         peer_key_size=None,
                         irk=None,
                         srk=None,
                         sign_count=0,
                         char_cfg=None):
        self.cmd = PairingCommands.PAIRING_CMD_WRITE_BOND
        self.data_struct = Struct(
            "peer_address" / Array(6, Int8ul),
            "peer_address_type" / Int8ul,
            "state_flags" / Int8ul,
            "local_ltk" / Array(16, Int8ul),
            "local_div" / Int16ul,
            "local_rand" / Array(8, Int8ul),
            "local_key_size" / Int8ul,
            "peer_ltk" / Array(16, Int8ul),
            "peer_div" / Int16ul,
            "peer_rand" / Array(8, Int8ul),
            "peer_key_size" / Int8ul,
            "irk" / Array(16, Int8ul),
            "srk" / Array(16, Int8ul),
            "sign_count" / Int32ul,
            "char_cfg" / Array(3 * self.max_num_char_cfg, Int8ul)
        )
        self.send_nwp_cmd(dict(
            peer_address=peer_address,
            peer_address_type=peer_address_type,
            state_flags=state_flags,
            local_ltk=local_ltk,
            local_div=local_div,
            local_rand=local_rand,
            local_key_size=local_key_size,
            peer_ltk=peer_ltk,
            peer_div=peer_div,
            peer_rand=peer_rand,
            peer_key_size=peer_key_size,
            irk=irk,
            srk=srk,
            sign_count=sign_count,
            char_cfg=char_cfg
        ))

class BleDeviceGATT(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=GATTEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.app_specifier = AppSpecifier.APP_SPECIFIER_GATT
        self.mtu_size = 0

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data

        if event_type == GATTEventType.NWP_GATT_MTU_UPDATED_EVENT:
            data_struct = Struct(
                "event" / Int16ul,
                "connection_handle" / Int16ul,
                "new_conn_mtu" / Int16ul,
            )
            self.mtu_size = self.parse_struct(data_struct, msg)["new_conn_mtu"]

        # Add event to queue
        parsed_data = self.parse_struct(data_struct, msg)
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data

    def mtu_exchange(self, conn_handle=0x00, mtu_size=255):
        self.cmd = GATTCommands.GATT_CMD_MTU_EXCHANGE
        self.data_struct = Struct(
            'conn_handle' / Int16ul,
            'mtu_size' / Int16ul,
        )
        self.send_nwp_cmd(dict(
            conn_handle=conn_handle,
            mtu_size=mtu_size,
        ))


class BleDeviceHandover(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=HandoverEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.handover = []
        self._last_handover_data = None
        self._last_handover_status = None

        self.app_specifier = AppSpecifier.APP_SPECIFIER_HANDOVER

    def start_serving_node(
        self, conn_handle=0x00, min_gatt_handle=0x0000, max_gatt_handle=0x0000, handover_sn_mode=0
    ):
        self.cmd = HandoverCommands.HANDOVER_CMD_START_SN
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "min_gatt_handle" / Int16ul,
            "max_gatt_handle" / Int16ul,
            "handover_sn_mode" / Int8ul,
        )
        self.send_nwp_cmd(
            dict(
                conn_handle=conn_handle,
                min_gatt_handle=min_gatt_handle,
                max_gatt_handle=max_gatt_handle,
                handover_sn_mode=handover_sn_mode,
            )
        )

    def close_serving_node(self, conn_handle=0x00, status=0):
        self.cmd = HandoverCommands.HANDOVER_CMD_CLOSE_SN
        self.data_struct = Struct(
            'conn_handle' / Int16ul,
            'status' / Int32ul,
        )
        self.send_nwp_cmd(dict(
            conn_handle=conn_handle,
            status=status,
        ))

    def start_candidate_node(self, offset, max_err_time=0, max_num_conn_events=6, tx_burst_ratio=30, data=[]):
        self.cmd = HandoverCommands.HANDOVER_CMD_START_CN
        self.data_struct = Struct(
            "offset" / Int32ul,
            "max_err_time" / Int32ul,
            "max_num_conn_events" / Int32ul,
            "tx_burst_ratio" / Int32ul,
            "data" / Array(len(data), Int8ul),
        )
        self.send_nwp_cmd(dict(
            offset=offset,
            max_err_time=max_err_time,
            max_num_conn_events=max_num_conn_events,
            tx_burst_ratio=tx_burst_ratio,
            data=data
        ))

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        if event_type == HandoverEventType.NWP_HANDOVER_SN_DATA:
            data_struct = Struct(
                "event" / Int16ul,
                "connection_handle" / Int16ul,
                "status" / Int32ul,
                "data_len" / Int32ul,
                "data" / Array(this.data_len, Int8ul),
            )
            self._last_handover_data = self.parse_struct(data_struct, msg)
            self.handover.append(self._last_handover_data)
            parsed_data = self._last_handover_data
            self.events_counter.increment_event(
                event_value=event_type, data_from_event=parsed_data
            )
        elif event_type == HandoverEventType.NWP_HANDOVER_CN_STATUS:
            data_struct = Struct(
                'event' / Int16ul,
                'connection_handle' / Int16ul,
                'status' / Int32ul,
            )
            self._last_handover_data = self.parse_struct(data_struct, msg)
            self.handover.append(self._last_handover_data)
            parsed_data = self._last_handover_data
            self.events_counter.increment_event(
                event_value=event_type, data_from_event=parsed_data
            )
        return parsed_data


class BleDeviceCs(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=CsEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.cs = []
        self._last_cs_data = None

        self.app_specifier = AppSpecifier.APP_SPECIFIER_CS

    def read_local_supported_capabilities(self):
        self.cmd = CsCommands.CS_CMD_READ_LOCAL_CAP
        self.data_struct = None

        self.send_nwp_cmd(None)

    def read_remote_supported_capabilities(self, conn_handle=0):
        self.cmd = CsCommands.CS_CMD_READ_REMOTE_CAP
        self.data_struct = Struct(
            "conn_handle" / Int16ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle)
        )

    def set_default_antenna(self, default_antenna_index=0):
        """
        Sets the default antenna for the BLE device.
        This method configures the BLE device to use the specified antenna index as the default antenna.
        Args:
            default_antenna_index (int, optional): The index of the antenna to set as default. The range of indices is 0 to 3. Defaults to 0.
        Returns:
            None
        """
        self.cmd = CsCommands.CS_CMD_SET_DEFAULT_ANT
        self.data_struct = Struct(
            "default_antenna_index" / Int8ul
        )

        self.send_nwp_cmd(
            dict(default_antenna_index=default_antenna_index)
        )
    
    def create_config(self, param_dict):
        self.cmd = CsCommands.CS_CMD_CREATE_CONFIG
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "config_id" / Int8ul,
            "create_context" / Int8ul,
            "main_mode" / Int8ul,
            "sub_mode" / Int8ul,
            "main_mode_min_steps" / Int8ul,
            "main_mode_max_steps" / Int8ul,
            "main_mode_repetition" / Int8ul,
            "mode_zero_steps" / Int8ul,
            "role" / Int8ul,
            "rtt_type" / Int8ul,
            "cs_sync_phy" / Int8ul,
            "channel_map" / Array(10, Int8ul),
            "channel_map_repetition" / Int8ul,
            "ch_sel" / Int8ul,
            "ch3c_shape" / Int8ul,
            "ch3C_jump" / Int8ul
        )

        self.send_nwp_cmd(param_dict)

    def security_enable(self, conn_handle=0):
        self.cmd = CsCommands.CS_CMD_SECURITY_ENABLE
        self.data_struct = Struct(
            "conn_handle" / Int16ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle)
        )

    def set_default_settings(self, conn_handle=0, role_enable=3, sync_anthenna_selection=1, max_tx_power=0):
        self.cmd = CsCommands.CS_CMD_SET_DEFAULT_SETTINGS
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "role_enable" / Int8ul,
            "sync_anthenna_selection" / Int8ul,
            "max_tx_power" / Int8ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle, role_enable=role_enable, sync_anthenna_selection=sync_anthenna_selection,
                 max_tx_power=max_tx_power)
        )

    def read_remote_fae_table(self, conn_handle=0):
        self.cmd = CsCommands.CS_CMD_READ_FAE_TABLE
        self.data_struct = Struct(
            "conn_handle" / Int16ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle)
        )

    def write_remote_fae_table(self, conn_handle=0, reflector_table=[0]*72):
        self.cmd = CsCommands.CS_CMD_WRITE_FAE_TABLE
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "reflector_table" / Array(72, Int8ul),
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle, reflector_table=reflector_table)
        )

    def remove_config(self, conn_handle=0, config_id=0):
        self.cmd = CsCommands.CS_CMD_REMOVE_CONFIG
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "config_id" / Int8ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle, config_id=config_id)
        )

    def set_channel_classification(self, chnl_class=None):
        if chnl_class is None:
            chnl_class = [0x00]*10
        self.cmd = CsCommands.CS_CMD_SET_CHNL_CLASS
        self.data_struct = Struct(
            "chnl_class" / Array(10, Int8ul)
        )

        self.send_nwp_cmd(
            dict(chnl_class=chnl_class)
        )

    def set_procedure_params(self, param_dict):
        self.cmd = CsCommands.CS_CMD_SET_PROCEDURE_PARAMS
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "config_id" / Int8ul,
            "max_procedure_duration" / Int16ul,
            "min_procedure_interval" / Int16ul,
            "max_procedure_interval" / Int16ul,
            "max_procedure_count" / Int16ul,
            "min_sub_event_length" / Int32ul,
            "max_sub_event_length" / Int32ul,
            "aci" / Int8ul,
            "phy" / Int8ul,
            "tx_power_delta" / Int8ul,
            "preferred_peer_antenna" / Int8ul,
            "snr_ctrl_i" / Int8ul,
            "snr_ctrl_r" / Int8ul,
            "enable" / Int8ul,
        )

        self.send_nwp_cmd(param_dict)

    def procedure_enable(self, conn_handle=0, config_id=0, enable=1):
        self.cmd = CsCommands.CS_CMD_PROCEDURE_ENABLE
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "config_id" / Int8ul,
            "enable" / Int8ul
        )

        self.send_nwp_cmd(
            dict(conn_handle=conn_handle, config_id=config_id, enable=enable)
        )

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        data_struct = None

        if event_type == CsEventType.NWP_CS_READ_REMOTE_CAPS:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "status" / Int8ul,
                "num_config" / Int8ul,
                "max_procedures" / Int16ul,
                "num_antennas" / Int8ul,
                "max_ant_path" / Int8ul,
                "role" / Int8ul,
                "optional_modes" / Int8ul,
                "rtt_cap" / Int8ul,
                "rtt_aa_only_n" / Int8ul,
                "rtt_sounding_n" / Int8ul,
                "rtt_random_payload_n" / Int8ul,
                "nadm_sounding" / Int16ul,
                "nadm_random_seq" / Int16ul,
                "optional_cs_sync_phy" / Int8ul,
                "no_fae" / Int8ul,
                "ch_sel_3c" / Int8ul,
                "cs_based_ranging" / Int8ul,
                "t_ip1_cap" / Int16ul,
                "t_ip2_cap" / Int16ul,
                "t_fcs_cap" / Int16ul,
                "t_pm_csap" / Int16ul,
                "t_sw_cap" / Int8ul,
                "snr_tx_cap" / Int8ul,
            )
        
        elif event_type == CsEventType.NWP_CS_CONFIG_COMPLETE:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "status" / Int8ul,
                "config_id" / Int8ul,
                "state" / Int8ul,
                "main_mode" / Int8ul,
                "sub_mode" / Int8ul,
                "main_mode_min_steps" / Int8ul,
                "main_mode_max_steps" / Int8ul,
                "main_mode_repetition" / Int8ul,
                "mode_zero_steps" / Int8ul,
                "role" / Int8ul,
                "rtt_type" / Int8ul,
                "cs_sync_phy" / Int8ul,
                "channel_map" / Array(10, Int8ul),
                "ch_m_repetition" / Int8ul,
                "ch_sel" / Int8ul,
                "ch3c_shape" / Int8ul,
                "ch3c_jump" / Int8ul,
                "rfu0" / Int8ul,
                "t_ip1" / Int8ul,
                "t_ip2" / Int8ul,
                "t_fcs" / Int8ul,
                "t_pm" / Int8ul,
                "rfu" / Int8ul,
            )

        elif event_type == CsEventType.NWP_CS_READ_REMOTE_FAE_TABLE:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "status" / Int8ul,
                "fae_table" / Array(72, Int8ul),
            )

        elif event_type == CsEventType.NWP_CS_SECURITY_ENABLE_COMPLETE:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "status" / Int8ul,
            )

        elif event_type == CsEventType.NWP_CS_PROCEDURE_ENABLE_COMPLETE:
            data_struct = Struct(
                "event" / Int16ul,
                "status" / Int8ul,
                "conn_handle" / Int16ul,
                "config_id" / Int8ul,
                "enable" / Int8ul,
                "aci" / Int8ul,
                "pwr_delta" / Int8ul,
                "sub_event_len" / Int32ul,
                "sub_events_per_event" / Int8ul,
                "sub_event_interval" / Int16ul,
                "event_interval" / Int16ul,
                "procedure_interval" / Int16ul,
                "procedure_count" / Int16ul
            )

        elif event_type == CsEventType.NWP_CS_SUBEVENT_RESULTS:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "config_id" / Int8ul,
                "start_acl_connection_event" / Int16ul,
                "procedure_counter" / Int16ul,
                "frequency_compensation" / Int16sl,
                "reference_power_level" / Int8sl,
                "procedure_done_status" / Int8ul,
                "subevent_done_status" / Int8ul,
                "abort_reason" / Int8ul,
                "num_antenna_path" / Int8ul,
                "num_steps_reported" / Int8ul,
                "data_len" / Int16ul,
                "data" / NiceBytes((Byte[this.data_len])),
            )

        elif event_type == CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE:
            data_struct = Struct(
                "event" / Int16ul,
                "conn_handle" / Int16ul,
                "config_id" / Int8ul,
                "procedure_done_status" / Int8ul,
                "subevent_done_status" / Int8ul,
                "abort_reason" / Int8ul,
                "num_antenna_path" / Int8ul,
                "num_steps_reported" / Int8ul,
                "data_len" / Int16ul,
                "data" / NiceBytes((Byte[this.data_len])),
            )

        elif event_type == CsEventType.NWP_CS_APP_DISTANCE_RESULTS:
            data_struct = Struct(
                "event" / Int16ul,
                "distance" / Int32ul,
                "quality" / Int32ul,
                "confidence" / Int32ul,
            )

        if data_struct is not None:
            self._last_cs_data = self.parse_struct(data_struct, msg)
            self.cs.append(self._last_cs_data)
            parsed_data = self._last_cs_data

        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data


class BleDeviceL2CAP(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=L2CAPEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.l2cap = []
        self._last_l2cap_data = None

        self.app_specifier = AppSpecifier.APP_SPECIFIER_L2CAP

    def open_psm(self, psm=0x01):
        self.cmd = L2CAPCommands.L2CAP_CMD_CREATE_PSM
        self.data_struct = Struct("psm" / Int16ul)

        self.send_nwp_cmd(dict(psm=psm))

    def close_psm(self, psm):
        self.cmd = L2CAPCommands.L2CAP_CMD_CLOSE_PSM
        self.data_struct = Struct("psm_num" / Int16ul)

        self.send_nwp_cmd(dict(psm_num=psm))

    def send_data(self, conn_handle, channel_id, data):
        self.cmd = L2CAPCommands.L2CAP_CMD_SEND_SDU
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
            "CID" / Int16ul,
            "data_len" / Int16ul,
            "payload" / Array(this.data_len, Int8ul),
        )

        self.send_nwp_cmd(dict(conn_handle=conn_handle, CID=channel_id, data_len=len(data), payload=data))

    def connect_request(self, conn_handle, psm, peer_psm):
        self.cmd = L2CAPCommands.L2CAP_CMD_CONNECT_REQ
        self.data_struct = Struct(
            "conn_handle" / Int16ul, "psm" / Int16ul, "peer_psm" / Int16ul
        )

        self.send_nwp_cmd(dict(conn_handle=conn_handle, psm=psm, peer_psm=peer_psm))

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data

        if event_type == L2CAPEventType.NWP_L2CAP_DATA_RECEIVED:
            data_struct = Struct(
                "event" / Int16ul,
                "connHandle" / Int16ul,
                "CID" / Int16ul,
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul),
            )

        elif event_type == L2CAPEventType.NWP_L2CAP_CHANNEL_ESTABLISHED:
            data_struct = Struct(
                "event" / Int16ul,
                "result" / Int16ul,
                "CID" / Int16ul,
                "psm" / Int16ul,
                "mtu" / Int16ul,
                "mps" / Int16ul,
                "credits" / Int16ul,
                "peerCID" / Int16ul,
                "peerMtu" / Int16ul,
                "peerMps" / Int16ul,
                "peerCredits" / Int16ul,
                "peerCreditThreshold" / Int16ul,
            )

        elif event_type == L2CAPEventType.NWP_L2CAP_CHANNEL_TERMINATED:
            data_struct = Struct(
                "event" / Int16ul,
                "CID" / Int16ul,
                "peerCID" / Int16ul,
                "reason" / Int16ul,
            )

        elif event_type == L2CAPEventType.NWP_L2CAP_OUT_OF_CREDIT:
            data_struct = Struct(
                "event" / Int16ul,
                "CID" / Int16ul,
                "peerCID" / Int16ul,
                "credits" / Int16ul,
            )

        self._last_l2cap_data = self.parse_struct(data_struct, msg)
        self.l2cap.append(self._last_l2cap_data)
        parsed_data = self._last_l2cap_data
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data


class BleDeviceCm(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=CmEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.cm = []
        self._last_cm_data = None
        self._last_cm_status = None

        self.app_specifier = AppSpecifier.APP_SPECIFIER_CM

    def start_cm_serving(
        self, conn_handle=0x00
        ):
        self.cmd = CmCommands.CM_CMD_START_SERVING
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
        )
        self.send_nwp_cmd(
            dict(
                conn_handle=conn_handle,
            )
        )

    def start_monitoring(self, time_delta=0, time_delta_err=0, max_num_conn_events=6, adjustment_events=4, conn_role=0, data_len=0, data=[]):
        self.cmd = CmCommands.CM_CMD_START_MONITORING
        self.data_struct = Struct(
            'time_delta' / Int32ul,
            'time_delta_err' / Int32ul,
            'max_num_conn_events' / Int8ul,
            'adjustment_events' /  Int8ul,
            'conn_role' / Int8ul,
            'data_len' / Int16ul,
            'data' / Array(this.data_len, Int8ul)
        )
        self.send_nwp_cmd(dict(
            time_delta=time_delta,
            time_delta_err=time_delta_err,
            max_num_conn_events=max_num_conn_events,
            adjustment_events=adjustment_events,
            conn_role=conn_role,
            data_len=data_len,
            data=data
        ))

    def stop_monitoring(self, conn_handle=0):
        self.cmd = CmCommands.CM_CMD_STOP_MONITORING
        self.data_struct = Struct(
            "conn_handle" / Int16ul,
        )
        self.send_nwp_cmd(dict(
            conn_handle=conn_handle,
        ))
    
    def update_conn(self, event):
        data_to_send = dict()
        self.data_struct = Struct(
            'access_addr' / Int32ul,
            'event_counter' / Int16ul,
            'update_type' / Int8ul,
            'data_len' / Int8ul,
            'data' / Array(this.data_len, Int8ul)
        )
        data_to_send = dict(
            access_addr=event['access_addr'],
            event_counter=event['event_counter'],
            update_type=event['update_type'],
            data_len=event['data_len'],
            data=event['data']
        )
        
        self.cmd = CmCommands.CM_CMD_UPDATE_CONN
        self.send_nwp_cmd(data_to_send)

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        if event_type == CmEventType.NWP_CM_SERVING_DATA:
            data_struct = Struct(
                "event" / Int16ul,
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul),
            )
        elif event_type == CmEventType.NWP_CM_REPORT:
            data_struct = Struct(
                'event' / Int16ul,
                'connection_handle' / Int16ul,
                'packet_len' / Int16ul,
                'status' / Int8ul,
                'conn_role' / Int8ul,
                'rssi' / Int8sl,
                'channel' / Int8ul,
                'sn' / Int8ul,
                'nesn' / Int8ul
            )
        elif event_type == CmEventType.NWP_CM_START:
            data_struct = Struct(
                'event' / Int16ul,
                'access_addr' / Int32ul,
                'connection_handle' / Int16ul,
                'address_type' / Int8ul,
                'dev_address' / Array(6, Int8ul),
            )
        elif event_type == CmEventType.NWP_CM_STOP:
            data_struct = Struct(
                'event' / Int16ul,
                'access_addr' / Int32ul,
                'connection_handle' / Int16ul,
                'address_type' / Int8ul,
                'dev_address' / Array(6, Int8ul),
                'stop_reason' / Int8ul
            )
        elif event_type == CmEventType.NWP_CM_CONN_UPDATE_EVENT:
            data_struct = Struct(
                'event' / Int16ul,
                'connection_handle' / Int16ul,
                'access_addr' / Int32ul,
                'event_counter' / Int16ul,
                'update_type' / Int8ul,
                'data_len' / Int8ul,
                'data' / Array(this.data_len, Int8ul)
            )

        self._last_cm_data = self.parse_struct(data_struct, msg)
        self.cm.append(self._last_cm_data)
        parsed_data = self._last_cm_data
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data


class BleDeviceCaServer(BleDeviceBasic):
    def __init__(self, logger, ble_device, sync_command, max_event_list_size):
        BleDeviceBasic.__init__(
            self,
            logger,
            event_type_class=CaServerEventType,
            max_event_list_size=max_event_list_size,
        )

        self.ble_device = ble_device
        self.sync_command = sync_command
        self.ca_server = []
        self._last_ca_server_data = None
        self.app_specifier = AppSpecifier.APP_SPECIFIER_CA_SERVER

    def send_indication(self, conn_handle=0x00):
        self.cmd = CaServerCommands.CA_SERVER_CMD_SEND_INDICATION
        self.data_struct = Struct(
            "conn_handle" / Int16ul
        )
        self.send_nwp_cmd(
            dict(
                conn_handle=conn_handle
            )
        )

    def message_parser(self, msg):
        event_type = msg.payload.event_type
        parsed_data = msg.payload.data
        if event_type == CaServerEventType.NWP_CA_SERVER_LONG_WRITE_DONE:
            data_struct = Struct(
                "event" / Int16ul,
                'connection_handle' / Int16ul,
                "data_len" / Int16ul,
                "data" / Array(this.data_len, Int8ul),
            )
        elif event_type == CaServerEventType.NWP_CA_SERVER_CCC_UPDATE:
            data_struct = Struct(
                'event' / Int16ul,
                'connection_handle' / Int16ul,
                'ccc_value' / Int16ul
            )
        elif event_type == CaServerEventType.NWP_CA_SERVER_INDICATION_CNF:
            data_struct = Struct(
                'event' / Int16ul,
                'connection_handle' / Int16ul,
                'status' / Int8ul
            )

        self._last_ca_server_data = self.parse_struct(data_struct, msg)
        self.ca_server.append(self._last_ca_server_data)
        parsed_data = self._last_ca_server_data
        self.events_counter.increment_event(
            event_value=event_type, data_from_event=parsed_data
        )

        return parsed_data
