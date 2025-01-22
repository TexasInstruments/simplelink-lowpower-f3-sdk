# BLE Agent

BLE Agent folder contains custom python package (rtls, unpi and ble_device) and example to present capability of BLE 
Device.

## Getting Started

Instructions below will setup your python environment with required packages.  

### Prerequisites

- The latest version of [Python 3.10.11]( https://www.python.org/downloads/release/python-31011/ )

    Note: Python 3.10 should be installed at **_C:\Python310_**. 

- CC2340R5 devices.

### Installing
* Install and activate virtual environment in case you network is behind a proxy use ```[--proxy]```
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
