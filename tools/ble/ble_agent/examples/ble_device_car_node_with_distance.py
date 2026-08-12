"""
ble_device_car_node_with_distance.py

This example demonstrates how to use the BleDevice API to perform Channel Sounding (CS) operations between
Out-Of-The-Box BLE devices, specifically between a "car node" and a "key node" examples
The script is designed to scan for peer devices, connect and pair with them, and then execute Channel Sounding
procedures to measure distance or/and collect raw data.

Main Features:
--------------
1. **Peer Discovery and Connection**:
   - Scans for BLE devices advertising specific names (see `peer_names` in `main()`).
   - Connects and pairs with the discovered peer devices.

2. **Channel Sounding (CS) Configuration**:
   - Configures CS parameters for each connection.
   - Supports both manual and automatic CS configuration modes (automatic mode still in development).

3. **CS Procedure Execution**:
   - Initiates CS procedures to collect distance measurements and/or raw channel data.
   - Supports repeated measurements and extended results.
   - Allows configuration of which events/results to wait for (distance, raw subevents, remote results, etc.).

How to Use:
-----------
- Set the correct COM port for your BLE device in the `main()` function (`device_comport="COM43"`).
- Adjust the `peer_names` list to match the names of the peer devices you want to connect to.
- Configure CS behavior using the following flags in `main()`:
    - `cs_wait_for_distance`: Wait for distance results events.
    - `cs_extended_results`: Wait for extended distance results (requires `cs_wait_for_distance=True`).
    - `cs_wait_for_raw_events`: Wait for raw subevent results.
    - `cs_wait_for_remote`: Wait for remote device results (requires `cs_wait_for_raw_events=True`).
    - `cs_auto_config`: Enable embedded auto CS configuration (device handles CS procedures internally).
- Run the script. It will:
    1. Optionally start advertising (if `adv_enabled` is set).
    2. Scan and connect to peer devices.
    3. Configure CS for each connection.
    4. Repeatedly execute CS procedures and print/log the results.

Note:
-----
- Ensure the embedded firmware on your BLE devices supports the required CS features and event reporting.
- The script is intended for demonstration and testing purposes; adapt it as needed for your application.

See the `main()` function at the bottom of this file for the primary workflow and configuration options.
"""

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
    BleDeviceLoggingLevel,
    PeripheralEventType,
    ConnectionEventType,
    AddressType,
    PairingEventType,
    CentralEventType,
    CsEventType,
    CsProcedureDoneStatusType,
    CsSubeventDoneStatusType,
)

cs_config_params = {
    "conn_handle": 0,
    "config_id": 0,
    "create_context": 1,
    "main_mode": 2,
    "sub_mode": 0xFF,
    "main_mode_min_steps": 0,
    "main_mode_max_steps": 0,
    "main_mode_repetition": 0,
    "mode_zero_steps": 3,
    "role": 0,  # 0 - initiator, 1 - reflector
    "rtt_type": 0,
    "cs_sync_phy": 1,
    "channel_map": [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF],
    "channel_map_repetition": 1,
    "ch_sel": 0,
    "ch3c_shape": 0,
    "ch3C_jump": 0,
}

cs_set_procedure_params = {
    "conn_handle": 0,
    "config_id": 0,
    "max_procedure_duration": 0xFFFF,
    "min_procedure_interval": 0,
    "max_procedure_interval": 0,
    "max_procedure_count": 1,
    "min_sub_event_length": 55000,
    "max_sub_event_length": 55000,
    "aci": 0,
    "phy": 1,
    "tx_power_delta": 0x80,
    "preferred_peer_antenna": 0b0001,  # Bitmap. set number of bits on as the peer's number of antennas (not more)
    "snr_ctrl_i": 0xFF,
    "snr_ctrl_r": 0xFF,
    "enable": 0,
}

cs_set_procedure_params_repeat = {
    "conn_handle": 0,
    "config_id": 0,
    "max_procedure_duration": 0xFFFF,
    "min_procedure_interval": 15,
    "max_procedure_interval": 15,
    "max_procedure_count": 10,
    "min_sub_event_length": 55000,
    "max_sub_event_length": 55000,
    "aci": 0,
    "phy": 1,
    "tx_power_delta": 0x80,
    "preferred_peer_antenna": 0b0001,  # Bitmap. set number of bits on as the peer's number of antennas (not more)
    "snr_ctrl_i": 0xFF,
    "snr_ctrl_r": 0xFF,
    "enable": 0,
}


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


def print_obj_data(caption, data):
    print(caption)
    print("----------------------------------------------------------------------")
    for key, value in data.items():
        print(f"{key}: {value}")
    print("----------------------------------------------------------------------")


def search_peer_name(peer_names: list, adv_data: bytes) -> str:
    """
    Search for a peer name in the advertising data.
    Args:
        peer_names (list): List of peer names to search for.
        adv_data (bytes): Advertising data to search in.
    Returns:
        str: The peer name if found, otherwise None.
    """
    i = 0
    while i < len(adv_data):
        length = adv_data[i]
        if length == 0:
            return None
        field_type = adv_data[i + 1]
        if field_type == 0x09:  # Complete Local Name
            # There could be devices that have invalid advertising data that cannot be decoded to UTF-8
            # While rare, it has been encountered, so this will protect against it
            try:
                name = bytes(adv_data[i + 2 : i + 1 + length]).decode("utf-8")
            except UnicodeDecodeError:
                # If the Complete Local Name cannot be decoded as Unicode, then something is wrong and return None
                # Our devices should always have valid unicode in the local name
                return None
            # If this decodes cleanly then return the name
            print(f"adv local name: {name}")
            if name in peer_names:
                return name
        i += length + 1

    return None


def search_and_connect_to_peer(car_node: BleDevice, peer_names: list) -> dict:
    """
    Search for peer devices by their names, connect to them and waits for pairing events.
    This function scan infinitely until all peer devices are found.
    Args:
        car_node (BleDevice): The car node object to use for scanning and connecting.
        peer_names (list): List of peer names to search for.
    Returns:
        dict: A dictionary of peer names to connection handles if connections are established
    """
    connection_data = []  # Holds temporary connection data
    peer_name = None  # Holds temporary peer name
    connections = {}  # Dictionary of peer names to connection handle

    for i in range(len(peer_names)):
        # Start scanning
        print("Start Scanning")
        car_node.central.start_scan(0, 0, 0)
        continue_scan = True
        while continue_scan:
            time.sleep(1e-12)

            if car_node.all_event_list.is_event_in_list(
                CentralEventType.NWP_ADV_REPORT
            ):
                adv_report = car_node.all_event_list.get_event_from_list(
                    CentralEventType.NWP_ADV_REPORT
                )
                if adv_report:
                    print("Adv report detected")
                    # Search the name in the advertising data
                    peer_name = search_peer_name(peer_names, adv_report["data"])
                    if peer_name != None:
                        # Stop the scan
                        car_node.central.stop_scan()
                        continue_scan = False
                        # Save the peer address and address type
                        peer_addr_type = adv_report["address_type"]
                        peer_address = adv_report["address"].copy()
                        print(f"Create a connection with {peer_address} as a central")
                        car_node.central.connect(peer_addr_type, peer_address, 1, 0)
                        connection_data = car_node.wait_for_event(
                            event_type=ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT,
                            timeout=60,
                        )

                        if connection_data:
                            print(
                                f"Connection created with: {peer_name}, connection handle: {connection_data['connection_handle']}"
                            )
                            # Stop the scan
                            car_node.central.stop_scan()
                            continue_scan = False
                            # Set peer name in dictionary
                            connections[peer_name] = connection_data[
                                "connection_handle"
                            ]
                        else:
                            print(f"ERROR: Connection failed with {peer_name}")
                            return None

        # Wait for pairing events
        if connection_data:
            # Print connection data
            print_connection_data(connection_data)

            # Holds pairing data
            pairing_data = None

            # Wait for the actual pairing events
            start_time = time.time()
            pairing_timeout = 10
            print(
                f"Waiting for pairing events from peer {peer_name}", end="", flush=True
            )
            while True:
                print(".", end="", flush=True)
                event_type, event_data = car_node.wait_for_any_event(timeout=0.2)

                if event_type is not None and event_data is not None:
                    if event_type == PairingEventType.NWP_PAIRING_STATE_COMPLETE:
                        print("\nPairing Complete Event Received")
                        pairing_data = event_data
                        break
                    elif event_type == PairingEventType.NWP_PAIRING_STATE_ENCRYPTED:
                        print("\nPairing Encrypted Event Received")
                        pairing_data = event_data
                        break

                # Check for timeout
                if time.time() - start_time > pairing_timeout:
                    print(
                        f"\nTimeout waiting for pairing events ({pairing_timeout} seconds)"
                    )
                    return None

            # Print pairing data
            if pairing_data and pairing_data["status"] == 0:
                print_pairing_data(pairing_data)
            else:
                print(f"ERROR: pairing with {peer_name} Failed!")
                return None

    return connections


def start_cs_config(car_node, conn_handle, auto_config=False):
    if not auto_config:
        print("***** read local cap *****")
        car_node.cs.read_local_supported_capabilities()
        time.sleep(1)

        print("***** read remote cap *****")
        car_node.cs.read_remote_supported_capabilities(conn_handle)
        rem_cap_event = car_node.wait_for_event(
            event_type=CsEventType.NWP_CS_READ_REMOTE_CAPS, timeout=10
        )
        if rem_cap_event:
            print_obj_data("Remote Capabilities", rem_cap_event)
        else:
            return False

        print("***** security enable *****")
        car_node.cs.security_enable(conn_handle)
        security_enable_event = car_node.wait_for_event(
            event_type=CsEventType.NWP_CS_SECURITY_ENABLE_COMPLETE, timeout=10
        )
        if security_enable_event:
            print_obj_data("Security Enable", security_enable_event)
        else:
            return False

        time.sleep(1)

        print("***** set default settings *****")
        car_node.cs.set_default_settings(
            conn_handle, role_enable=3, sync_anthenna_selection=1, max_tx_power=10
        )
        time.sleep(1)

        print("***** create config for connection *****")
        cs_config_params["conn_handle"] = conn_handle
        car_node.cs.create_config(cs_config_params)
        create_config_event = car_node.wait_for_event(
            event_type=CsEventType.NWP_CS_CONFIG_COMPLETE, timeout=10
        )
        if create_config_event:
            print_obj_data("Create Config", create_config_event)
        else:
            return False

    return True


def cs_proc_enable(
    car_node,
    conn_handle=0,
    repeat=0,
    wait_for_remote=True,
    wait_for_distance=True,
    extended_results=False,
    wait_for_raw_events=False,
    auto_config=False,
):
    """
    Enables the procedure for the given car node and handles subsequent events.
    Args:
        car_node: The car node object on which the procedure is to be enabled.
        repeat (int, optional): The number of times to wait for results subevents
            if `wait_for_distance` is False. Defaults to 0.
        wait_for_remote (bool, optional): Indicates whether to wait for remote
            results subevents. Only applicable if `wait_for_distance` is False.
            Defaults to True.
        wait_for_distance (bool, optional): Indicates whether to wait for distance
            events. defaults to True.
        wait_for_raw_events (bool, optional): Indicates whether to wait for raw results
            events. Defaults to False.
        auto_config (bool, optional): Indicates whether embedded Auto CS Configuration
            is enabled. Defaults to False.
    Behavior:
        - Enables the procedure on the car node by invoking `procedure_enable`.
        - Waits for the `NWP_CS_PROCEDURE_ENABLE_COMPLETE` event with a timeout of 10 seconds.
        - If `wait_for_distance` is True, waits for distance events.
        - If `wait_for_distance` is False, waits for raw results subevents based on the
          `repeat` count and `wait_for_remote` flag.
        - if 'auto_config' is False, will trigger the CS Procedure Enable command.
        - If 'auto_config' is True, will wait for results only without triggering the command.
    Note:
        Ensure that the system configuration (`syscfg`) is set appropriately if
        `wait_for_distance` is False, as it requires 'Measure Distance' to be disabled.
        Ensure that the embedded code for car_node example raises the remote results if
        'wait_for_remote' is set to True.
        Ensure that the 'Enable Channel Sounding Embedded Mode' is enabled in car_node example
        if 'auto_config' is set to True.
    """
    if not auto_config:
        print("***** procedure enable *****")
        car_node.cs.procedure_enable(conn_handle=conn_handle)

    # wait for Procedure Enable Complete event
    proc_enable_event = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_PROCEDURE_ENABLE_COMPLETE, timeout=10
    )
    if proc_enable_event:
        print_obj_data("Procedure Enable", proc_enable_event)

        # If the Procedure Enable was successful, wait for results
        if proc_enable_event["status"] == 0:
            # Wait for events depends on the number of procedure repetitions
            for i in range(repeat):
                if wait_for_raw_events:
                    # Expect raw results events
                    wait_results_subevents(car_node, wait_for_remote)
                    if wait_for_distance:
                        # Expect also distance results event
                        wait_distance(car_node, extended_results)

                elif wait_for_distance:
                    # Expect only distance results event (need to disable 'Measure Distance' in sysconfig for this)
                    wait_distance(car_node, extended_results)
                else:
                    break


def to_signed12(val: int) -> int:
    """Convert unsigned short to signed int12."""
    return val - 4096 if val >= 2048 else val


def to_signed8(val: int) -> int:
    """Convert unsigned byte to signed int8."""
    return val - 256 if val >= 128 else val


def parse_mode_zero_steps(role: int, data: bytes, num_steps: int):
    # Ensure input is 480 bytes
    numBytes = 5  # number of bytes for each step in case the role is initiator
    if role == 1:
        numBytes = 3  # number of bytes for each step in case the role is reflector

    result = []
    for i in range(0, num_steps * numBytes, numBytes):
        packetQuality = data[i]
        packetRssi = to_signed8(data[i + 1])
        packetAntenna = data[i + 2]
        entry = {
            "packetQuality": packetQuality,
            "packetRssi": packetRssi,
            "packetAntenna": packetAntenna,
        }
        if role == 0:  # for initiator, add also frequency offset parameter
            measuredFreqOffset = data[i + 3] | (data[i + 4] << 8)  # little endian
            entry["measuredFreqOffset"] = measuredFreqOffset
        result.append(entry)
    return result


def parse_mode_two_steps(data: bytes, num_steps: int, num_paths):
    # Ensure input is 480 bytes

    results = []
    for path in range(0, num_paths):
        pathOffset = path * num_steps * 4
        pathResults = []
        for step in range(0, num_steps):
            stepOffset = (step * 4) + pathOffset

            i = data[stepOffset] | ((data[stepOffset + 1] & 0x0F) << 8)
            q = ((data[stepOffset + 1] & 0xF0) | (data[stepOffset + 2] << 8)) >> 4
            tqi = data[stepOffset + 3]

            stepResultsEntry = {"i": to_signed12(i), "q": to_signed12(q), "tqi": tqi}
            pathResults.append(stepResultsEntry)
        results.append(pathResults)
    return results


def wait_distance_extended_results_event(car_node):
    """
    Wait for the NWP_CS_APP_DISTANCE_EXTENDED_RESULTS event and print the results.
    Parse and print the event data
    """
    print("Waiting for NWP_CS_APP_DISTANCE_EXTENDED_RESULTS")
    initiator_distance_extended_results = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_APP_DISTANCE_EXTENDED_RESULTS, timeout=3
    )
    if initiator_distance_extended_results:
        print("Distance Extended Results - initiator")
        print("----------------------------------------------------------------------")
        # Fields to print as decimal
        decimals_fields_lists = {
            "distanceMusic",
            "distanceNN",
            "numMpcPaths",
            "qualityPaths",
            "confidencePaths",
            "localRpl",
            "remoteRpl",
            "permutationIndexLocal",
            "permutationIndexRemote",
        }
        decimal_fields = {
            "status",
            "connHandle",
            "distance",
            "quality",
            "confidence",
            "numMpc",
        }.union(decimals_fields_lists)
        for key, value in initiator_distance_extended_results.items():
            if isinstance(value, list):
                # For arrays, print each element in hex, unless key is in decimal_fields
                if key in decimals_fields_lists:
                    # Print as decimal
                    print(f"{key}: [{', '.join(str(v) for v in value)}]")
                else:
                    if key in {"modeZeroStepsInit"}:
                        parsed_data = parse_mode_zero_steps(
                            role=0, data=value, num_steps=96
                        )
                        print(f"Initiator Mode0 Steps: {parsed_data}")
                    elif key in {"modeZeroStepsRef"}:
                        parsed_data = parse_mode_zero_steps(
                            role=1, data=value, num_steps=96
                        )
                        print(f"Reflector Mode0 Steps: {parsed_data}")
                    elif key in {"stepsDataLocal"}:
                        parsed_data = parse_mode_two_steps(
                            data=value, num_steps=75, num_paths=4
                        )
                        print(f"Local Mode2 Steps Results: {parsed_data}")
                    elif key in {"stepsDataRemote"}:
                        parsed_data = parse_mode_two_steps(
                            data=value, num_steps=75, num_paths=4
                        )
                        print(f"Remote Mode2 Steps Results: {parsed_data}")
                    else:
                        print(
                            f"{key}: [{', '.join(hex(v) if isinstance(v, int) else str(v) for v in value)}]"
                        )
            else:
                if key in decimal_fields:
                    print(f"{key}: {value}")
                elif isinstance(value, int):
                    print(f"{key}: {hex(value)}")
                else:
                    print(f"{key}: {value}")
        print("----------------------------------------------------------------------")
    else:
        print("No NWP_CS_APP_DISTANCE_EXTENDED_RESULTS event received")
        return None


def wait_distance_event(car_node):
    print("Waiting for NWP_CS_APP_DISTANCE_RESULTS")
    # print the distance
    initiator_distance_results = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_APP_DISTANCE_RESULTS, timeout=3
    )
    if initiator_distance_results:
        print_obj_data("Distance Results - initiator", initiator_distance_results)


def wait_distance(car_node, extended_results=False):
    # Expect only distance results event with no distance (need to disable 'Measure Distance' in syscfg for this)
    if extended_results:
        wait_distance_extended_results_event(car_node)
    else:
        wait_distance_event(car_node)


def wait_results_subevents(car_node, wait_for_remote):
    print("**** Waiting for Local Results ****")
    wait_results_subevents_data(car_node)

    if wait_for_remote:
        print("**** Waiting for Remote Results ****")
        wait_results_subevents_data(car_node)


def wait_results_subevents_data(car_node):
    proc_done = CsProcedureDoneStatusType.NWP_CS_PROCEDURE_ACTIVE
    subevent_done = CsSubeventDoneStatusType.NWP_CS_SUBEVENT_ACTIVE

    while proc_done == CsProcedureDoneStatusType.NWP_CS_PROCEDURE_ACTIVE:
        # CS print results loop, prints the results as long as it gets them
        print("Waiting for CS_SUBEVENT_RESULTS")
        results = car_node.wait_for_event(
            event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS, timeout=3
        )
        if results:
            print_obj_data("Subevent Results", results)
            proc_done = results["procedure_done_status"]
            subevent_done = results["subevent_done_status"]

        if results is None:
            print("ERROR: No CS_SUBEVENT_RESULTS arrived")
            return

        while subevent_done == CsSubeventDoneStatusType.NWP_CS_SUBEVENT_ACTIVE:
            print("Waiting for CS_SUBEVENT_RESULTS_CONTINUE")
            results = car_node.wait_for_event(
                event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE, timeout=3
            )
            if results:
                print_obj_data("Subevent Results Continue", results)
                proc_done = results["procedure_done_status"]
                subevent_done = results["subevent_done_status"]
            else:
                print("ERROR: No CS_SUBEVENT_RESULTS_CONTINUE arrived")
                return

            if (
                proc_done == CsProcedureDoneStatusType.NWP_CS_PROCEDURE_DONE
                and subevent_done != CsSubeventDoneStatusType.NWP_CS_SUBEVENT_DONE
            ):
                print("ERROR: Procedure done although subevent didn't!")
                return


def main():
    logging_file = get_logging_file_path()
    print(f"Logging to file: {logging_file}")

    # Create a BleDevice instance for the car node
    car_node = BleDevice(
        device_comport="COM43",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    if car_node.initialize():
        # Basic Features
        peer_names = [
            "Key Node"
        ]  # Names of the peer devices to search for while scanning
        adv_enabled = False

        # CS Features
        # Change these parameters to configure which events to wait for.
        # The events will be raised depneds on the embedded configuration of the car node example.
        cs_wait_for_distance = True  # Wait for distance results
        cs_extended_results = False  # While waiting for distance, expect for extended results event (need to enable cs_wait_for_distance)
        cs_wait_for_raw_events = False  # Wait for subevent results raw data
        cs_wait_for_remote = True  # While waiting for raw events - also wait for remote device results (need to enable cs_wait_for_raw_events)
        cs_auto_config = False  # Let the device do the CS procedures on its own without sending commands to it

        # Set default antenna for non-CS BLE communications
        car_node.cs.set_default_antenna(0)

        # If Advertisement is needed, start advertising
        if adv_enabled:
            print("Start Advertising")
            car_node.peripheral.start_advertising(0, 0, 0)
            car_node.peripheral.wait_for_event(
                event_type=PeripheralEventType.NWP_ADV_START_AFTER_ENABLE, timeout=30
            )

        # Scan and connect to peer devices
        print("Start Scanning for peer devices")
        connections = search_and_connect_to_peer(car_node, peer_names)
        if not connections:
            print("ERROR: No peer devices found!")
            return

        # Print connection dictionary
        print(f"Connections: {connections}")

        # Start Channel Sounding configuration for each connection
        for conn_handle in connections.values():

            status = start_cs_config(car_node, conn_handle)
            if status == False:
                print(f"ERROR: CS Config with connection {conn_handle} Failed!")
                return

        # Start Channel Sounding procedures
        for i in range(100):
            for conn_handle in connections.values():
                # Update procedure params with the current connection handle
                cs_set_procedure_params["conn_handle"] = conn_handle

                # Set procedure params
                if not cs_auto_config:
                    car_node.cs.set_procedure_params(cs_set_procedure_params)
                time.sleep(0.2)

                # Enable Procedure and wait for events
                cs_proc_enable(
                    car_node,
                    conn_handle,
                    cs_set_procedure_params["max_procedure_count"],
                    wait_for_remote=cs_wait_for_remote,
                    wait_for_distance=cs_wait_for_distance,
                    extended_results=cs_extended_results,
                    wait_for_raw_events=cs_wait_for_raw_events,
                    auto_config=cs_auto_config,
                )

    car_node.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
