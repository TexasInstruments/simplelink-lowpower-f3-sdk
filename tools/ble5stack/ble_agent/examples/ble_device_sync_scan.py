import os
import sys
import datetime
from time import sleep

## Uncomment line below for local debug of packages
# sys.path.append(r"../unpi")
# sys.path.append(r"../rtls")
# sys.path.append(r"../ble_device")

from ble_device import BleDevice, BleDeviceLoggingLevel, AddressType


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


def main():
    logging_file = get_logging_file_path()

    ble_device = BleDevice(
        device_comport="COM12",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=True,
    )

    if ble_device.initialize():
        print(ble_device.device_node.identifier)
        ble_device.central.start_scan(
            scan_period=0x0000, scan_duration=0x0064, max_num_report=0x0A
        )
        sleep(4)
        ble_device.central.stop_scan()

        # Print last scan results
        scan_results = ble_device.central.get_scan_results()
        print("Scan Reports")
        print("----------------------------------------------------------------------")
        for report in scan_results:
            print(f"Address Type       : {AddressType(report['address_type']).name}")
            print(
                f"BD Address         : {':'.join(format(x, '02X') for x in report['bd_address'])}"
            )
            print(
                "----------------------------------------------------------------------"
            )

    ble_device.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
