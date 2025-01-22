import enum


class CommonCommands(enum.IntEnum):
    COMMON_CMD_GET_DEV_ADDR = 0x00


class CentralCommands(enum.IntEnum):
    CENTRAL_CMD_SCAN_START = 0x00
    CENTRAL_CMD_SCAN_STOP = 0x01
    CENTRAL_CMD_CONNECT = 0x02


class PeripheralCommands(enum.IntEnum):
    PERI_CMD_ADVERTISE_START = 0x00
    PERI_CMD_ADVERTISE_STOP = 0x01


class HandoverCommands(enum.IntEnum):
    HANDOVER_CMD_START_SN = 0x00
    HANDOVER_CMD_START_CN = 0x01
    HANDOVER_CMD_CLOSE_SN = 0x02


class CsCommands(enum.IntEnum):
    CS_CMD_PROCEDURE_ENABLE = 0x06


class ConnectionCommands(enum.IntEnum):
    CONNECTION_CMD_RSSI_ENABLE = 0x00
    CONNECTION_CMD_RSSI_DISABLE = 0x01


class L2CAPCommands(enum.IntEnum):
    L2CAP_CMD_CREATE_PSM = 0x00
    L2CAP_CMD_CLOSE_PSM = 0x01
    L2CAP_CMD_CONNECT_REQ = 0x02
    L2CAP_CMD_DISCONNECT_REQ = 0x03
    L2CAP_CMD_SEND_SDU = 0x04


class AppSpecifier(enum.IntEnum):
    APP_SPECIFIER_CENTRAL = 0x00
    APP_SPECIFIER_PERIPHERAL = 0x01
    APP_SPECIFIER_CONNECTION = 0x02
    APP_SPECIFIER_PAIRING = 0x03
    APP_SPECIFIER_HANDOVER = 0x04
    APP_SPECIFIER_CS = 0x05
    APP_SPECIFIER_COMMON = 0x07
    APP_SPECIFIER_L2CAP = 0x08


class RtlsCapabilities(enum.IntEnum):
    RTLS_CAP_NOT_INITIALIZED = 0x00000000
    RTLS_CAP_CM = 0x00000001
    RTLS_CAP_AOA_TX = 0x00000002
    RTLS_CAP_AOA_RX = 0x00000004
    RTLS_CAP_RESERVED1 = 0x00000008
    RTLS_CAP_CL_AOA = 0x00000010
    RTLS_CAP_RESERVED3 = 0x00000020
    RTLS_CAP_RTLS_RESPONDER = 0x00000040
    RTLS_CAP_RTLS_COORDINATOR = 0x00000080
    RTLS_CAP_RTLS_PASSIVE = 0x00000100
    RTLS_CAP_RTLS_CONNECTION_MONITOR = 0x00000200
    RTLS_CAP_CENTRAL = 0x00000400
    RTLS_CAP_PERIPHERAL = 0x00000800
    RTLS_CAP_HANDOVER = 0x00001000
    RTLS_CAP_CONNECTION = 0x00002000
    RTLS_CAP_PAIRING = 0x00004000
    RTLS_CAP_L2CAPCOC = 0x00008000
    RTLS_CAP_CS = 0x000080000  # TODO need to be changed


class CommonEventType(enum.IntEnum):
    NWP_COMMON_DEVICE_ADDRESS = 0x0020


class PeripheralEventType(enum.IntEnum):
    NWP_ADV_START_AFTER_ENABLE = 0x0001
    NWP_ADV_END_AFTER_DISABLE = 0x0002
    NWP_ADV_REPORT = 0x0005


class CentralEventType(enum.IntEnum):
    NWP_SCAN_ENABLED = 0x0003
    NWP_SCAN_DISABLED = 0x0004


class ConnectionEventType(enum.IntEnum):
    NWP_LINK_ESTABLISHED_EVENT = 0x0006
    NWP_LINK_TERMINATED_EVENT = 0x0007
    NWP_LINK_PARAM_UPDATE_REQ_EVENT = 0x0008
    NWP_LINK_PARAM_UPDATE_EVENT = 0x0009
    NWP_CONN_NOTI_CONN_EVENT = 0x000A
    NWP_CONN_DEVICE_ADDRESS = 0x0020


class PairingEventType(enum.IntEnum):
    NWP_PAIRING_STATE_STARTED = 0x000C
    NWP_PAIRING_STATE_COMPLETE = 0x000D
    NWP_PAIRING_STATE_ENCRYPTED = 0x000E
    NWP_PAIRING_STATE_BOND_SAVED = 0x000F


class HandoverEventType(enum.IntEnum):
    NWP_HANDOVER_SN_DATA = 0x000B
    NWP_HANDOVER_CN_STATUS = 0x0010


class CsEventType(enum.IntEnum):
    NWP_CS_PROCEDURE_ENABLE = 0x0010
    NWP_CS_GENERIC_MSG = 0x00F0


class L2CAPEventType(enum.IntEnum):
    NWP_L2CAP_DATA_RECEIVED = 0x0030
    NWP_L2CAP_CHANNEL_ESTABLISHED = 0x0031
    NWP_L2CAP_CHANNEL_TERMINATED = 0x0032
    NWP_L2CAP_OUT_OF_CREDIT = 0x0033


class AddressType(enum.IntEnum):
    ADDRTYPE_PUBLIC = 0x00
    ADDRTYPE_RANDOM = 0x01
    ADDRTYPE_PUBLIC_ID = 0x02
    ADDRTYPE_RANDOM_ID = 0x03
    ADDRTYPE_RANDOM_NR = 0xFE
    ADDRTYPE_NONE = 0xFF


class AddressMode(enum.IntEnum):
    # Use Public Address
    ADDRMODE_PUBLIC = (0x00,)
    # Use Random Static Address
    ADDRMODE_RANDOM = (0x01,)
    # Use Resolvable Private Address with Public Identity Address
    ADDRMODE_RP_WITH_PUBLIC_ID = (0x02,)
    # Use Resolvable Private Address with Random Identity Address
    ADDRMODE_RP_WITH_RANDOM_ID = (0x03,)
    ADDRTYPE_NONE = 0xFF


class ConnectionRoleType(enum.IntEnum):
    PERIPHERAL_ROLE = 0x04
    CENTRAL_ROLE = 0x08
