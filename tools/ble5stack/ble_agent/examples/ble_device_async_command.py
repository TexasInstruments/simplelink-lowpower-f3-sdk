import os
import sys
import datetime
from time import sleep

## Uncomment line below for local debug of packages
# sys.path.append(r"../unpi")
# sys.path.append(r"../rtls")
# sys.path.append(r"../ble_device")

from ble_device import (
    BleDevice,
    BleDevicePeripheral,
    BleDeviceLoggingLevel,
    PeripheralEventType,
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


def main():
    logging_file = get_logging_file_path()

    ble_device = BleDevice(
        device_comport="COM11",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=False,
    )

    if ble_device.initialize():

        for index in range(1, 3):
            print(f"Starting loop {index}")

            ble_device.peripheral.start_advertising(
                adv_handle=0x00,
                enable_options=BleDevicePeripheral.AdvOptions.GAP_ADV_ENABLE_OPTIONS_USE_MAX,
                duration_or_max_events=0x0000,
            )
            sleep(5)
            data_from_event = ble_device.peripheral.wait_for_event(
                event_type=PeripheralEventType.NWP_ADV_START_AFTER_ENABLE,
                timeout=10,
                total_event_count=index,
            )
            print(f"Loop {index}: Data from start_advertising : {data_from_event}")

            ble_device.peripheral.stop_advertising(adv_handle=0x00)
            sleep(5)
            data_from_event = ble_device.peripheral.wait_for_event(
                event_type=PeripheralEventType.NWP_ADV_END_AFTER_DISABLE,
                timeout=10,
                total_event_count=index,
            )
            print(f"Loop {index}: Data from stop_advertising : {data_from_event}")

        print(f"Unknown events list: {ble_device.unknown_event_list}")

    ble_device.done()

    print("Example ended")

    exit(0)


if __name__ == "__main__":
    main()
