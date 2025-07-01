import os
import datetime
import sys
import time

## Uncomment line below for local debug of packages
# sys.path.append(r"../unpi")
# sys.path.append(r"../rtls")
# sys.path.append(r"../ble_device")

from ble_device import (
    BleDevice,
    BleDeviceCentral,
    BleDevicePeripheral,
    ConnectionRoleType,
    BleDeviceLoggingLevel,
    PeripheralEventType,
    ConnectionEventType,
    CsEventType,
    AddressType,
)

# --- Default parameters ---

CS_CONFIG_PARAMS = {
    "conn_handle": 0,
    "config_id": 0,
    "create_context": 1,
    "main_mode": 3,
    "sub_mode": 0xFF,
    "main_mode_min_steps": 0,
    "main_mode_max_steps": 0,
    "main_mode_repetition": 0,
    "mode_zero_steps": 3,
    "role": 0,
    "rtt_type": 0,
    "cs_sync_phy": 1,
    "channel_map": [0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0x3F],
    "channel_map_repetition": 1,
    "ch_sel": 0,
    "ch3c_shape": 0,
    "ch3C_jump": 0,
}

CS_SET_PROCEDURE_PARAMS = {
    "conn_handle": 0,
    "config_id": 0,
    "max_procedure_duration": 0xFFFF,
    "min_procedure_interval": 3,
    "max_procedure_interval": 3,
    "max_procedure_count": 2,
    "min_sub_event_length": 0xD6D8,
    "max_sub_event_length": 0xD6D8,
    "aci": 0,
    "phy": 1,
    "tx_power_delta": 0x80,
    "preferred_peer_antenna": 1,
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


def print_obj_data(caption, data):
    print(caption)
    print("----------------------------------------------------------------------")
    for key, value in data.items():
        print(f"{key}: {value}")
    print("----------------------------------------------------------------------")


def main():
    global CS_CONFIG_PARAMS
    global CS_SET_PROCEDURE_PARAMS

    logging_file = get_logging_file_path()

    initiator_node = BleDevice(
        device_comport="COM46",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.ALL,
        sync_command=False,
    )

    reflector_node = BleDevice(
        device_comport="COM49",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.ALL,
        sync_command=False,
    )

    initiator_node.initialize()
    reflector_node.initialize()

    time.sleep(2)

    # -------------- connection prodedure --------------

    reflector_node.peripheral.start_advertising(
        adv_handle=0x00,
        enable_options=BleDevicePeripheral.AdvOptions.GAP_ADV_ENABLE_OPTIONS_USE_MAX,
        duration_or_max_events=0x0000,
    )

    reflector_node.wait_for_event(
        event_type=PeripheralEventType.NWP_ADV_START_AFTER_ENABLE, timeout=5
    )
    peri_add = [0x40, 0x40, 0x40, 0x40, 0x40, 0xC0]
    print("***** connection procedure *****")
    initiator_node.central.connect(address_type=1, address=peri_add, phy=1, timeout=0)
    print("***** Waiting for connection *****")
    connection_data = reflector_node.wait_for_event(
        event_type=ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT, timeout=30
    )

    # Print connection data - has to be encrypted
    if connection_data:
        print_obj_data("Connections Data", connection_data)

    time.sleep(1)

    # -------------- CS testing --------------

    print("***** read local cap *****")
    initiator_node.cs.read_local_supported_capabilities()
    time.sleep(1)

    print("***** read remote cap *****")
    initiator_node.cs.read_remote_supported_capabilities()
    rem_cap_event = initiator_node.wait_for_event(
        event_type=CsEventType.NWP_CS_READ_REMOTE_CAPS, timeout=10
    )
    if rem_cap_event:
        print_obj_data("Remote Capabilities", rem_cap_event)

    print("***** security enable *****")
    initiator_node.cs.security_enable()
    security_enable_event = initiator_node.wait_for_event(
        event_type=CsEventType.NWP_CS_SECURITY_ENABLE_COMPLETE, timeout=10
    )
    if security_enable_event:
        print_obj_data("Security Enable", security_enable_event)

    print("***** set default settings *****")
    initiator_node.cs.set_default_settings()
    time.sleep(1)

    reflector_node.cs.set_default_settings()
    time.sleep(1)

    print("***** create config *****")
    initiator_node.cs.create_config(CS_CONFIG_PARAMS)
    create_config_event = initiator_node.wait_for_event(
        event_type=CsEventType.NWP_CS_CONFIG_COMPLETE, timeout=10
    )
    if create_config_event:
        print_obj_data("Create Config", create_config_event)

    print("***** set procedure params *****")
    initiator_node.cs.set_procedure_params(CS_SET_PROCEDURE_PARAMS)
    time.sleep(1)

    reflector_node.cs.set_procedure_params(CS_SET_PROCEDURE_PARAMS)
    time.sleep(1)

    print("***** procedure enable *****")
    initiator_node.cs.procedure_enable()
    proc_enable_event = initiator_node.wait_for_event(
        event_type=CsEventType.NWP_CS_PROCEDURE_ENABLE_COMPLETE, timeout=10
    )
    if proc_enable_event:
        print_obj_data("Procedure Enable", proc_enable_event)

    # CS print results loop, prints the results as long as it gets them
    while True:
        initiator_results = initiator_node.wait_for_event(
            event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS, timeout=3
        )
        if initiator_results:
            print_obj_data("Subevent Results - initiator", initiator_results)
            initiator_proc_done = initiator_results["procedure_done_status"]

        reflector_results = reflector_node.wait_for_event(
            event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS, timeout=3
        )
        if reflector_results:
            print_obj_data("Subevent Results - reflector", reflector_results)
            reflector_proc_done = reflector_results["procedure_done_status"]

        if initiator_results is None or reflector_results is None:
            break

        while initiator_proc_done or reflector_proc_done:
            if initiator_results:
                initiator_results = initiator_node.wait_for_event(
                    event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE, timeout=3
                )
                print_obj_data(
                    "Subevent Results Continue - initiator", initiator_results
                )
                initiator_proc_done = initiator_results["procedure_done_status"]

            if reflector_results:
                reflector_results = reflector_node.wait_for_event(
                    event_type=CsEventType.NWP_CS_SUBEVENT_RESULTS_CONTINUE, timeout=3
                )
                print_obj_data(
                    "Subevent Results Continue - reflector", reflector_results
                )
                reflector_proc_done = reflector_results["procedure_done_status"]

    initiator_node.done()
    reflector_node.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
