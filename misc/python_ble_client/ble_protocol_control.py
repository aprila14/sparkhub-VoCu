from cobs import cobs
import ctypes
from crc import Calculator, Configuration

ble_commands = {
"CMD_GET_WIFI_MAC_ADDRESS" : 32,
"CMD_SEND_CERTIFICATES" : 80
}

class ble_command_get_wifi_mac(ctypes.Structure):
    _fields_ = [('type', ctypes.c_uint16), ('crc', ctypes.c_uint16), ('dummyByte', ctypes.c_uint8)]

def calculate_crc16_ccitt_false(payload: bytearray) -> int:
    crc_config = Configuration(
        width=16,
        polynomial=0x1021,
        init_value=0xFFFF,
        final_xor_value=0x0000,
        reverse_input=False,
        reverse_output=False,
    )

    calculator_crc16 = Calculator(crc_config)

    crc16 = calculator_crc16.checksum(payload)

    return crc16

def prepare_get_wifi_mac_command() -> bytearray:
    # prepare command payload to calculate crc16
    payload = ble_command_get_wifi_mac(ble_commands["CMD_GET_WIFI_MAC_ADDRESS"],0,0)

    payload_bytearray = bytearray(payload)

    payload_bytearray.pop(5) # because of padding an additional byte is added at the end of the payload

    # calculate crc
    crc16 = calculate_crc16_ccitt_false(payload_bytearray)    

    # add calculated crc to payload
    payload_with_crc = ble_command_get_wifi_mac(ble_commands["CMD_GET_WIFI_MAC_ADDRESS"],crc16,0)

    payload_bytearray_with_crc = bytearray(payload_with_crc)

    payload_bytearray_with_crc.pop(5) # because of padding an additional byte is added at the end of the payload

    print(payload_bytearray_with_crc.hex())

    # encode payload with crc with COBS
    encoded_payload = cobs.encode(payload_bytearray_with_crc)

    encoded_payload = b'\x00'+encoded_payload+b'\x00'
    
    return encoded_payload
