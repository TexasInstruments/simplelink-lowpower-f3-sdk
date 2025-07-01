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
    L2CAPEventType,
    CentralEventType,
    ConnectionRoleType,
    CmEventType,
    CmrConnectionRoleTypes,
    CmConnUpdateType,
    CmConnUpdateType,
    CsEventType,
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
    print(f"Connection Handle: {start_event['connection_handle']}")
    print(f"Address Type:      {AddressType(start_event['address_type']).name}")
    print(
        f"Device Address:    {':'.join(format(x, '02X') for x in reversed(start_event['dev_address']))}"
    )
    print("------------------------------------------------------")


def print_stop_info(stop_event):
    print("Stop Monitoring")
    print(f"Connection Handle: {stop_event['connection_handle']}")
    print(f"Address Type:      {AddressType(stop_event['address_type']).name}")
    print(
        f"Device Address:    {':'.join(format(x, '02X') for x in reversed(stop_event['dev_address']))}"
    )
    print(f"Stop Reason:       {stop_event['stop_reason']}")
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
            print(f"adv local name: {name}")
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
        device_comport="COM43",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    # if car_node.initialize() and cmr_node.initialize():
    if car_node.initialize():
        # Basic features
        l2cap_enabled = False
        adv_enabled = False
        auto_update = True
        peer_name = "Key Node"
        peer_address = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]
        peer_addr_type = 0xFF
        connection_data = []
        connection_role = 0

        car_node.cs.set_default_antenna(0)

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
                    print("adv report detected")
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
                event_type=PairingEventType.NWP_PAIRING_STATE_COMPLETE, timeout=6
            )

            # If already bonded - wait for pairing encrypted event
            if not pairing_data:
                print(
                    "Didn't get NWP_PAIRING_STATE_COMPLETE, wait for NWP_PAIRING_STATE_ENCRYPTED in case we already bonded"
                )
                # Wait for pairing encrypted event
                pairing_data = car_node.wait_for_event(
                    event_type=PairingEventType.NWP_PAIRING_STATE_ENCRYPTED, timeout=1
                )

            # print pairing data
            if pairing_data:
                print_pairing_data(pairing_data)

            # if success, continue to establish L2CAP connection and start Channel Sounding
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

                # start Channel Sounding
                start_cs(car_node)

    car_node.done()

    print("Example ended")

    exit(0)


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
    "min_sub_event_length": 0xD6D8,
    "max_sub_event_length": 0xD6D8,
    "aci": 0,
    "phy": 1,
    "tx_power_delta": 0x80,
    "preferred_peer_antenna": 0b0001,  # Bitmap. set number of bits on as the peer's number of antennas (not more)
    "snr_ctrl_i": 0xFF,
    "snr_ctrl_r": 0xFF,
    "enable": 0,
}


def print_obj_data(caption, data):
    print(caption)
    print("----------------------------------------------------------------------")
    for key, value in data.items():
        print(f"{key}: {value}")
    print("----------------------------------------------------------------------")


def start_cs(car_node):
    print("***** read local cap *****")
    car_node.cs.read_local_supported_capabilities()
    time.sleep(1)

    print("***** read remote cap *****")
    car_node.cs.read_remote_supported_capabilities()
    rem_cap_event = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_READ_REMOTE_CAPS, timeout=10
    )
    if rem_cap_event:
        print_obj_data("Remote Capabilities", rem_cap_event)

    print("***** security enable *****")
    car_node.cs.security_enable()
    security_enable_event = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_SECURITY_ENABLE_COMPLETE, timeout=10
    )
    if security_enable_event:
        print_obj_data("Security Enable", security_enable_event)

    print("***** set default settings *****")
    car_node.cs.set_default_settings(
        conn_handle=0, role_enable=3, sync_anthenna_selection=1, max_tx_power=10
    )
    time.sleep(1)

    print("***** create config *****")
    car_node.cs.create_config(cs_config_params)
    create_config_event = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_CONFIG_COMPLETE, timeout=10
    )
    if create_config_event:
        print_obj_data("Create Config", create_config_event)

    # set procedure params and enable the procedure multiple times
    for i in range(100):
        car_node.cs.set_procedure_params(cs_set_procedure_params)
        time.sleep(0.1)
        cs_proc_enable(
            car_node,
            cs_set_procedure_params["max_procedure_count"],
            wait_for_remote=True,
            wait_for_distance=True,
            extended_results=False,
            wait_for_raw_events=False,
        )

    # Or - set procedure params with repetition and enable the procedure once (This is still not working properly)
    # car_node.cs.set_procedure_params(cs_set_procedure_params_repeat)
    # cs_proc_enable(car_node, cs_set_procedure_params_repeat["max_procedure_count"], wait_for_remote=True,
    #                wait_for_distance=False, extended_results=False, wait_for_raw_events=False)


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
            "permutationIndexLocal",
            "permutationIndexRemote",
        }
        decimal_fields = {
            "status",
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


def wait_results_subevents(car_node, wait_for_remote=True):
    local_proc_done = None
    remote_proc_done = None

    # CS print results loop, prints the results as long as it gets them
    print("Waiting for CS_SUBEVENT_RESULTS")
    local_results = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS, timeout=3
    )
    if local_results:
        print_obj_data("Subevent Results - local device", local_results)
        local_proc_done = local_results["procedure_done_status"]

    if local_results is None:
        print("ERROR - No CS_SUBEVENT_RESULTS arrived")
        return

    while local_proc_done:
        if local_results:
            print("Waiting for CS_SUBEVENT_RESULTS_CONTINUE")
            local_results = car_node.wait_for_event(
                event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE, timeout=3
            )
            if local_results:
                print_obj_data(
                    "Subevent Results Continue - local device", local_results
                )
                local_proc_done = local_results["procedure_done_status"]
            else:
                print("No CS_SUBEVENT_RESULTS_CONTINUE arrived")
                local_proc_done = False

    # Now, wait for the remote device results
    if wait_for_remote:
        print("Waiting for remote CS_SUBEVENT_RESULTS")
        remote_results = car_node.wait_for_event(
            event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS, timeout=3
        )
        if remote_results:
            print_obj_data("Subevent Results - remote device", remote_results)
            remote_proc_done = remote_results["procedure_done_status"]

        if remote_results is None:
            print("ERROR - No CS_SUBEVENT_RESULTS arrived")
            return

        while remote_proc_done:
            if remote_results:
                print("Waiting for CS_SUBEVENT_RESULTS_CONTINUE")
                remote_results = car_node.wait_for_event(
                    event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE, timeout=3
                )
                if remote_results:
                    print_obj_data(
                        "Subevent Results Continue - remote device", remote_results
                    )
                    remote_proc_done = remote_results["procedure_done_status"]
                else:
                    print("No CS_SUBEVENT_RESULTS_CONTINUE arrived")
                    remote_proc_done = False


def cs_proc_enable(
    car_node,
    repeat=0,
    wait_for_remote=True,
    wait_for_distance=True,
    extended_results=False,
    wait_for_raw_events=False,
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
    Behavior:
        - Enables the procedure on the car node by invoking `procedure_enable`.
        - Waits for the `NWP_CS_PROCEDURE_ENABLE_COMPLETE` event with a timeout of 10 seconds.
        - If `wait_for_distance` is True, waits for distance events.
        - If `wait_for_distance` is False, waits for raw results subevents based on the
          `repeat` count and `wait_for_remote` flag.
    Note:
        Ensure that the system configuration (`syscfg`) is set appropriately if
        `wait_for_distance` is False, as it requires 'Measure Distance' to be disabled.
        Ensure that the embedded code for car_node example raises the remote results if
        'wait_for_remote' is set to True.
    """
    print("***** procedure enable *****")
    car_node.cs.procedure_enable()
    proc_enable_event = car_node.wait_for_event(
        event_type=CsEventType.NWP_CS_PROCEDURE_ENABLE_COMPLETE, timeout=10
    )
    if proc_enable_event:
        print_obj_data("Procedure Enable", proc_enable_event)

    # Wait for events depends on the number of procedure repetitions
    for i in range(repeat):
        if wait_for_raw_events:
            # Expect raw results events
            wait_results_subevents(car_node, wait_for_remote)
            if wait_for_distance:
                # Expect also distance results event
                wait_distance(car_node, extended_results)

        elif wait_for_distance:
            # Expect only distance results event with no distance (need to disable 'Measure Distance' in syscfg for this)
            wait_distance(car_node, extended_results)
        else:
            break


if __name__ == "__main__":
    main()
