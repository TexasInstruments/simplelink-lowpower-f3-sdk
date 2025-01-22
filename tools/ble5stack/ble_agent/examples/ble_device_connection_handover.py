import os
import datetime
import sys

## Uncomment line below for local debug of packages
# sys.path.append(r"../unpi")
# sys.path.append(r"../rtls")
# sys.path.append(r"../ble_device")

from ble_device import (
    BleDevice,
    BleDevicePeripheral,
    BleDeviceLoggingLevel,
    PeripheralEventType,
    ConnectionEventType,
    HandoverEventType,
    AddressType,
    PairingEventType,
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
    print(f"status:                 {connection_data['status']}")
    print(f"opcode:                 {connection_data['opcode']}")
    print(
        f"address_type:           {AddressType(connection_data['address_type']).name}"
    )
    print(
        f"dev_address:            {':'.join(format(x, '02X') for x in reversed(connection_data['dev_address']))}"
    )
    print(f"connection_handle:      {connection_data['connection_handle']}")
    print(f"connection_role:        {connection_data['connection_role']}")
    print(f"connection_interval:    {connection_data['connection_interval']}")
    print(f"connection_latency:     {connection_data['connection_latency']}")
    print(f"connection_timeout:     {connection_data['connection_timeout']}")
    print(f"clock_accuracy:         {connection_data['clock_accuracy']}")
    print("----------------------------------------------------------------------")


def main():
    logging_file = get_logging_file_path()

    serving_node = BleDevice(
        device_comport="COM63",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    candidate_node = BleDevice(
        device_comport="COM67",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    if serving_node.initialize() and candidate_node.initialize():

        # Serving node starts advertising
        serving_node.peripheral.start_advertising(
            adv_handle=0x00,
            enable_options=BleDevicePeripheral.AdvOptions.GAP_ADV_ENABLE_OPTIONS_USE_MAX,
            duration_or_max_events=0x0000,
        )

        serving_node.wait_for_event(
            event_type=PeripheralEventType.NWP_ADV_START_AFTER_ENABLE, timeout=5
        )

        # for now only the serving node will advertise and will wait for the connection
        # Wait for connection to established
        print("Waiting for connection")
        connection_data = serving_node.wait_for_event(
            event_type=ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT, timeout=30
        )

        # Print connection data
        if connection_data:
            print_connection_data(connection_data)

        pairing_data = serving_node.wait_for_event(
            event_type=PairingEventType.NWP_PAIRING_STATE_COMPLETE, timeout=30
        )

        if pairing_data:
            print("Pairing Complete")
            print(f"connection_handle:     {pairing_data['connection_handle']}")
            print(f"state:                 {pairing_data['state']}")
            print(f"status:                {pairing_data['status']}")
            print("------------------------------------------------------")

        print("Start Serving Node")
        # Start Serving Node
        print(pairing_data)
        
        if pairing_data["status"] == 0:
            serving_node.handover.start_serving_node(
                conn_handle=connection_data["connection_handle"],
                handover_sn_mode=1
            )

            handover_data = serving_node.wait_for_event(
                event_type=HandoverEventType.NWP_HANDOVER_SN_DATA, timeout=60
            )
            
            if handover_data:
                print(f"connection handle:      {handover_data['connection_handle']}")
                print(f"status:                 {handover_data['status']}")
                print(f"data length:            {handover_data['data_len']}")
                print(f"data:                   {handover_data['data']}")

                if handover_data["status"] == 0:
                    print("Start Candidate Node")
                    candidate_node.handover.start_candidate_node(
                        offset=65000, max_err_time=0, max_num_conn_events=6, data=handover_data["data"]
                    )

                    # Wait for the candidate node status
                    candidate_node_status = candidate_node.wait_for_event(
                        event_type=HandoverEventType.NWP_HANDOVER_CN_STATUS,
                        timeout=30
                    )

                    # Wait for the connection complete event on the candidate side
                    connection_data = candidate_node.wait_for_event(
                        event_type=ConnectionEventType.NWP_LINK_ESTABLISHED_EVENT,
                        timeout=30,
                    )

                    # Report the candidate handover status to the serving node
                    if candidate_node_status:
                        serving_node.handover.close_serving_node(status=candidate_node_status['status'])

                    # print connection data
                    if connection_data:
                        print_connection_data(connection_data)

    serving_node.done()
    candidate_node.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
