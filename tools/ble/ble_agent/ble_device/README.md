# Ble Device
This package is a layer between the user and internal modules.
 
This package based on: 
* rtls
* unpi

RTLS package used for RTLS Manager and RTLS Node see [README](../rtls/README.md).
UNPI package user for comunincation and parsing data between RTLS package and devices see [README](../unpi/README.md).
 
## Getting started

Instructions below will setup your python environment with required packages.  

### Prerequisites

- The latest version of [Python 3.10]( https://www.python.org )

    Note: Python 3.10 should be installed at **_C:\Python310_**. 


### Installing
* Install and activate virtual environment
    ```
    cd <ble_agent folder>
    
    c:\Python310\python.exe -m pip install virtualenv [--proxy <www.proxy.com>]
  
    c:\Python310\python.exe -m venv .venv
  
    .venv\Scripts\activate
    ```
* Setup external packages in case you network is behind a proxy use ```[--proxy]```
  ```
  python -m pip install --upgrade pip [--proxy <www.proxy.com>]
         
  pip install --upgrade wheel [--proxy <www.proxy.com>]
         
  pip install -r requirements.txt [--proxy <www.proxy.com>] 
  ```
     
## Running Demo

Before executing example open and update the device comport in [ble_device_async_command.py](../examples/ble_device_async_command.py), [ble_device_sync_scan.py](../examples/ble_device_sync_scan.py) or 
[ble_device_wait_for_connection.py](../examples/ble_device_wait_for_connection.py) .

Executing example: 
```
cd <ble_agent folder>

.venv\Scripts\activate

cd examples

python ble_device_sync_scan.py  
```

 
 ## Code example
```python

    from time import sleep
    from ble_device import BleDevice, BleDevicePeripheral, BleDeviceLoggingLevel, AddressType
    
    logging_file = 'log.txt'

        ble_device = BleDevice(
        device_comport="COM13",
        logging_file=logging_file,
        logging_level=BleDeviceLoggingLevel.DEBUG,
        sync_command=True
    )

    if ble_device.initialize():
        ble_device.central.start_scan(scan_period=0x0000, scan_duration=0x0064, max_num_report=0x0A)
        sleep(4)
        ble_device.central.stop_scan()

        # Print last scan results
        scan_results = ble_device.central.get_scan_results()
        print("Scan Reports")
        print("----------------------------------------------------------------------")
        for report in scan_results:
            print(f"Address Type       : {AddressType(report['address_type']).name}")
            print(f"BD Address         : {':'.join(format(x, '02X') for x in report['bd_address'])}")
            print("----------------------------------------------------------------------")

    ble_device.done()

    print("Example ended")

    exit(0)
```

## Add new command
1. Add the command to the relevant enum in [ble_device_enums.py](ble_device/ble_device_enums.py) 
   `PeripheralCommands`, `CentralCommands`, `HandoverCommands` or `ConnectionCommands`
   ```python
   import enum

   class ConnectionCommands(enum.IntEnum):
        ### other commands
        CONNECTION_CMD_REGISTER_CONN_EVENT = 0x00
   ```
2. Create new function for the command in the related class i.e. `register_connection_event` in `BleDeviceConnection` class.
   1. Initiate `self.cmd` to the desirable command.
   2. Initiate `self.data_struct` to the command structure.
   3. Call `self.send_nwp_cmd` function with a dict of the commands values as argument.
   ```python
   from ble_device.ble_device_enums import ConnectionCommands
   from construct import Struct, Int8ul, Int16ul

   class BleDeviceConnection(BleDeviceBasic):
   
        ### other functions
   
        def register_connection_event(self,
                                      connection_handle=0xFFFD,
                                      event_type = ConnectionEventNotifyType.CONNECTION_EVENT_ALL,
                                      report_frequency=0x0A):
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
   ```

## Add new event
1. Add the event to the relevant enum in [ble_device_enums.py](ble_device/ble_device_enums.py) 
   `PeripheralEventType`, `CentralEventType`, `HandoverEventType` or `ConnectionEventType`
   ```python
   import enum
   class ConnectionEventType(enum.IntEnum):
   
        ### other events
    
        NWP_CONN_NOTI_CONN_EVENT = 0x000A
   ```
2. Update `message_parser` function (in the relevant class) in [ble_device.py](ble_device/ble_device.py) to parse the 
   new event. Update `parsed_data` variable to the data received from the event.
     ```python
    from ble_device.ble_device_enums import  ConnectionEventType
    from construct import Struct, Int8ul, Int16ul, this, Array
    
    class BleDeviceConnection(BleDeviceBasic):
        
        ### other class functions
        
        def message_parser(self, msg):
            event_type = msg.payload.event_type
            parsed_data = msg.payload.data
            
            ### other if's
    
            if event_type == ConnectionEventType.NWP_CONN_NOTI_CONN_EVENT:
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

   ```

## More Examples 

Look at [ble_device_async_command.py](../examples/ble_device_async_command.py), 
[ble_device_sync_scan.py](../examples/ble_device_sync_scan.py) or 
[ble_device_wait_for_connection.py](../examples/ble_device_wait_for_connection.py)  