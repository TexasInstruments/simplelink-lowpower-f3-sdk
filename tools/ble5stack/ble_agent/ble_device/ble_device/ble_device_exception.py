class BleDeviceException(Exception):
    def __init__(self, *args, **kwargs):
        Exception.__init__(self, *args, **kwargs)


class BleDeviceTimeoutException(BleDeviceException):
    pass


class BleDeviceCoordinatorNotFoundException(BleDeviceException):
    pass


class BleDevicePassiveNotFoundException(BleDeviceException):
    pass


class BleDeviceNodesNotIdentifiedException(BleDeviceException):
    def __init__(self, msg, not_indentified_nodes):
        BleDeviceException.__init__(self, msg)
        self.not_indentified_nodes = not_indentified_nodes


class BleDeviceEmbeddedFailToStopScanException(BleDeviceException):
    pass


class BleDeviceScanNoResultsException(BleDeviceException):
    pass


class BleDeviceScanResponderNotFoundException(BleDeviceException):
    pass


class BleDeviceFailToConnectException(BleDeviceException):
    pass
