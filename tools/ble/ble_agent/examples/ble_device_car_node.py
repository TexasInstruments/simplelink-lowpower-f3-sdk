import os
import datetime
import sys
import time
import threading

## Uncomment line below for local debug of packages
# sys.path.append(r"../unpi")
# sys.path.append(r"../rtls")
# sys.path.append(r"../ble_device")

from ble_device import (
    BleDevice,
    BleDevicePeripheral,
    BleDeviceCentral,
    BleDeviceCm,
    BleDeviceLoggingLevel,
    PeripheralEventType,
    ConnectionEventType,
    HandoverEventType,
    AddressType,
    CmStopReasonType,
    PairingEventType,
    L2CAPEventType,
    CentralEventType,
    ConnectionRoleType,
    CmEventType,
    CmrConnectionRoleTypes,
    CmConnUpdateType,
    ConnectionEventStatus,
    CmConnUpdateType,
)


def get_logging_file_path():
    data_time = datetime.datetime.now().strftime("%m_%d_%Y_%H_%M_%S")

    logging_file_path = os.path.join(
        os.path.curdir, os.path.basename(__file__).replace(".py", "_log")
    )

    if not os.path.isdir(logging_file_path):
        os.makedirs(logging_file_path)

    logging_file = os.path.join(
        logging_file_path,
        f"{data_time}_{os.path.basename(__file__).replace('.py', '.log')}",
    )

    return os.path.abspath(logging_file)


def print_connection_data(connection_data):
    print("Connection Data")
    print("----------------------------------------------------------------------")
    print(f"Status:                 {connection_data['status']}")
    print(f"Opcode:                 {connection_data['opcode']}")
    print(
        f"Address Type:           {AddressType(connection_data['address_type']).name}"
    )
    print(
        f"Device Address:         {':'.join(format(x, '02X') for x in reversed(connection_data['dev_address']))}"
    )
    print(f"Connection Handle:      {connection_data['connection_handle']}")
    print(f"Connection Role:        {connection_data['connection_role']}")
    print(f"Connection Interval:    {connection_data['connection_interval']}")
    print(f"Connection Latency:     {connection_data['connection_latency']}")
    print(f"Connection Timeout:     {connection_data['connection_timeout']}")
    print(f"Clock Accuracy:         {connection_data['clock_accuracy']}")
    print("----------------------------------------------------------------------")


def print_pairing_data(pairing_data):
    print("Pairing Complete")
    print(f"Connection Handle:     {pairing_data['connection_handle']}")
    print(f"State:                 {pairing_data['state']}")
    print(f"Status:                {pairing_data['status']}")
    print("------------------------------------------------------")


def print_l2cap_conn(l2cap_conn_data):
    print("L2CAP connection established")
    print(f"Result:                {l2cap_conn_data['result']}")
    print(f"CID:                   {l2cap_conn_data['CID']}")
    print(f"PSM:                   {l2cap_conn_data['psm']}")
    print(f"MTU:                   {l2cap_conn_data['mtu']}")
    print(f"MPS:                   {l2cap_conn_data['mps']}")
    print(f"Credits:               {l2cap_conn_data['credits']}")
    print(f"Peer CID:              {l2cap_conn_data['peerCID']}")
    print(f"Peer MTU:              {l2cap_conn_data['peerMtu']}")
    print(f"Peer MPS:              {l2cap_conn_data['peerMps']}")
    print(f"Peer Credits:          {l2cap_conn_data['peerCredits']}")
    print(f"Peer Credit Threshold: {l2cap_conn_data['peerCreditThreshold']}")
    print("------------------------------------------------------")


def print_start_event(start_event):
    print("Start Monitoring")
    print(f"Access Address:    {start_event['access_addr']}")
    print(f"Connection Handle: {start_event['connection_handle']}")
    print(f"Address Type:      {AddressType(start_event['address_type']).name}")
    print(
        f"Device Address:    {':'.join(format(x, '02X') for x in reversed(start_event['dev_address']))}"
    )
    print("------------------------------------------------------")


def print_stop_info(stop_event):
    print("Stop Monitoring")
    print(f"Access Address:    {stop_event['access_addr']}")
    print(f"Connection Handle: {stop_event['connection_handle']}")
    print(f"Address Type:      {AddressType(stop_event['address_type']).name}")
    print(
        f"Device Address:    {':'.join(format(x, '02X') for x in reversed(stop_event['dev_address']))}"
    )
    print(f"Stop Reason:       {CmStopReasonType(stop_event['stop_reason']).name}")
    print("------------------------------------------------------")


def print_report(report):
    print(f"Received new report")
    print(f"Connection Handle: {report['connection_handle']}")
    print(f"Status:            {report['status']}")
    print(f"Packet Length:     {report['packet_len']}")
    print(f"Connection Role:   {CmrConnectionRoleTypes(report['conn_role']).name}")
    print(f"RSSI:              {report['rssi']}")
    print(f"SN:                {report['sn']}")
    print(f"NESN:              {report['nesn']}")
    print("------------------------------------------------------")


def print_update_event(evt):
    print(f"Received an update event")
    print(f"Connection Handle: {evt['connection_handle']}")
    print(f"Access Address:    {evt['access_addr']}")
    print(f"Event Counter:     {evt['event_counter']}")
    print(f"Update Type:       {CmConnUpdateType(evt['update_type']).name}")
    print("------------------------------------------------------")


def search_peer_name(peer_name, adv_data):
    i = 0
    while i < len(adv_data):
        length = adv_data[i]
        if length == 0:
            return False
        field_type = adv_data[i + 1]
        if field_type == 0x09:  # Complete Local Name
            name = bytes(adv_data[i + 2 : i + 1 + length]).decode("utf-8")
            return name == peer_name
        i += length + 1


def update_connection(conn_node, cm_node):
    # Wait for the connection update event form the connection node
    while True:
        try:
            evt = conn_node.wait_for_event(
                event_type=CmEventType.NWP_CM_CONN_UPDATE_EVENT, timeout=200
            )
        except Exception as e:
            # Timeout on the event, keep listening
            continue
        if evt:
            print_update_event(evt)
            # Update the connection on the cmr side
            cm_node.cm.update_conn(evt)


def enable_connection_monitor(car_node, cmr_node, conn_handle, connection_role):
    # Get the information of the active connection
    car_node.cm.start_cm_serving(conn_handle)

    # Wait for the connection information event
    conn_info = car_node.wait_for_event(
        event_type=CmEventType.NWP_CM_SERVING_DATA, timeout=30
    )

    # Start monitoring
    cmr_node.cm.start_monitoring(
        time_delta=50000,
        time_delta_err=0,
        max_sync_attempts=3,
        adjustment_events=4,
        conn_role=connection_role,
        data_len=conn_info["data_len"],
        data=conn_info["data"],
    )

    start_info = cmr_node.wait_for_event(
        event_type=CmEventType.NWP_CM_START, timeout=10
    )

    if start_info:
        print_start_event(start_info)

    # Wait until receiving 40 report from the connection monitor
    for num_conn_events in range(0, 40):
        report = cmr_node.wait_for_event(
            event_type=CmEventType.NWP_CM_REPORT, timeout=10
        )
        print_report(report)


def main():
    logging_file = get_logging_file_path()

    car_node = BleDevice(
        device_comport="COM60",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    cmr_node = BleDevice(
        device_comport="COM105",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    if car_node.initialize() and cmr_node.initialize():
        # Basic features?
        l2cap_enabled = True
        adv_enabled = False
        auto_update = True
        peer_name = "Key Node"
        peer_address = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]
        peer_addr_type = 0xFF
        connection_data = []
        connection_role = 0

        # If L2CAP is enabled open PSM
        if l2cap_enabled:
            car_node.l2cap.open_psm(1)

        # If Advertisement is needed, start advertising
        if adv_enabled:
            print("Start Advertising")
            car_node.peripheral.start_advertising(0, 0, 0)
            car_node.peripheral.wait_for_event(
                event_type=PeripheralEventType.NWP_ADV_START_AFTER_ENABLE, timeout=30
            )

        # Start scanning
        print("Start Scanning")
        car_node.central.start_scan(0, 0, 0)

        continue_scan = 1
        while continue_scan:
            time.sleep(1e-12)

            if car_node.all_event_list.is_event_in_list(
                CentralEventType.NWP_ADV_REPORT
            ):
                adv_report = car_node.all_event_list.get_event_from_list(
                    CentralEventType.NWP_ADV_REPORT
                )
                if adv_report:
                    # Search the name in the advertising data
                    search_result = search_peer_name(peer_name, adv_report["data"])
                    if search_result:
                        # Stop the scan
                        car_node.central.stop_scan()
                        continue_scan = 0
                        # Save the peer address and address type
                        peer_addr_type = adv_report["address_type"]
                        peer_address = adv_report["address"].copy()
                        print(f"Create a connection with {peer_address} as a central")
                        car_node.central.connect(peer_addr_type, peer_address, 1, 0)
                        connection_data = car_node.wait_for_event(
                            event_type=ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT,
                            timeout=30,
                        )

            if car_node.all_event_list.is_event_in_list(
                ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT
            ):
                print("Connection created as a peripheral")
                # Stop the scan
                car_node.central.stop_scan()
                continue_scan = 0
                connection_data = car_node.all_event_list.get_event_from_list(
                    ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT
                )

        # Print connection data
        if connection_data:
            print_connection_data(connection_data)
            connection_role = connection_data["connection_role"]

            # Wait for pairing complete event
            pairing_data = car_node.wait_for_event(
                event_type=PairingEventType.NWP_PAIRING_STATE_COMPLETE, timeout=30
            )

            if pairing_data:
                print_pairing_data(pairing_data)

            if pairing_data["status"] == 0:
                if l2cap_enabled and connection_role == ConnectionRoleType.CENTRAL_ROLE:
                    # If L2CAP is enabled and the connection is in central role
                    print("Establish L2CAP connection")
                    car_node.l2cap.connect_request(0, 1, 1)

                    l2cap_conn_event = car_node.wait_for_event(
                        event_type=L2CAPEventType.NWP_L2CAP_CHANNEL_ESTABLISHED,
                        timeout=30,
                    )
                    print_l2cap_conn(l2cap_conn_event)

                # Transfer the connection data to the connection monitor to start monitoring the connection
                cm_conn_role = CmrConnectionRoleTypes.CM_CONNECTION_ROLE_PERIPHERAL
                if connection_role == ConnectionRoleType.CENTRAL_ROLE:
                    cm_conn_role = CmrConnectionRoleTypes.CM_CONNECTION_ROLE_CENTRAL

                enable_connection_monitor(
                    car_node,
                    cmr_node,
                    connection_data["connection_handle"],
                    cm_conn_role,
                )

            if auto_update == True:
                # Create a thread to update the connection automaticlly
                update_thread = threading.Thread(
                    target=update_connection, args=(car_node, cmr_node)
                )
                update_thread.start()
                input("Press any key to finish the example")

    car_node.done()
    cmr_node.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
