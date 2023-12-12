from cobs import cobs
import ctypes

from utils import calculate_crc16_ccitt_false

ble_commands = {
"CMD_GET_WIFI_MAC_ADDRESS" : 32,
"CMD_GET_WIFI_MAC_ADDRESS_RESPONSE" : 8208,
"CMD_SEND_CERTIFICATES" : 80
}

MAX_FULLCHAIN_CERTIFICATE_LENGTH = 6001
MAX_PRIVATE_KEY_LENGTH           = 2001

# TODO change to proper paths!
FULLCHAIN_CERTIFICATE_PATH = '/home/lsawicki/Projects/sparkhub/sparkhub-LevelSense/misc/python_ble_client/certs/3C610511BD50-full-chain.cert.pem'
PRIVATE_KEY_PATH = '/home/lsawicki/Projects/sparkhub/sparkhub-LevelSense/misc/python_ble_client/private/3C610511BD50.key.pem'

class ble_command_get_wifi_mac(ctypes.Structure):
    _fields_ = [('type', ctypes.c_uint16), ('crc', ctypes.c_uint16), ('dummyByte', ctypes.c_uint8)]
    _pack_ = 1

class full_chain_certificate_structure(ctypes.Structure):
    _fields_ = [('full_chain_certificate', ctypes.c_char * MAX_FULLCHAIN_CERTIFICATE_LENGTH)]
    _pack_ = 1

class private_key_structure(ctypes.Structure):
    _fields_ = [('private_key', ctypes.c_char * MAX_PRIVATE_KEY_LENGTH)]
    _pack_ = 1
class certificate_pack_structure(ctypes.Structure):
    _fields_ = [('full_chain_certificate', full_chain_certificate_structure), 
                ('private_key', private_key_structure)]
    _pack_ = 1

class ble_command_send_certificates(ctypes.Structure):
    _fields_ = [('type', ctypes.c_uint16), ('crc', ctypes.c_uint16), ('certificates', certificate_pack_structure)]
    _pack_ = 1

def read_full_chain_certificate() -> ctypes.Structure:
    with open(FULLCHAIN_CERTIFICATE_PATH, 'rb') as file:
        full_chain_certificates = full_chain_certificate_structure()

        # Get file size
        file.seek(0, 2)
        file_size = file.tell()
        print(file_size)
        file.seek(0)

        # Read file content into the structure
        file.readinto(full_chain_certificates)
       
        return full_chain_certificates
    
def read_private_key() -> ctypes.Structure:
    with open(PRIVATE_KEY_PATH, 'rb') as file:
        private_key = private_key_structure()

        # Get file size
        file.seek(0, 2)
        file_size = file.tell()
        print(file_size)
        file.seek(0)

        # Read file content into the structure
        file.readinto(private_key)
       
        return private_key

def prepare_get_wifi_mac_command() -> bytearray:
    # prepare command payload to calculate crc16
    payload = ble_command_get_wifi_mac(ble_commands["CMD_GET_WIFI_MAC_ADDRESS"],0,0)

    payload_bytearray = bytearray(payload)

    # calculate crc
    crc16 = calculate_crc16_ccitt_false(payload_bytearray)    

    # add calculated crc to payload
    payload_with_crc = ble_command_get_wifi_mac(ble_commands["CMD_GET_WIFI_MAC_ADDRESS"],crc16,0)

    payload_bytearray_with_crc = bytearray(payload_with_crc)

    print(payload_bytearray_with_crc.hex())

    # encode payload with crc with COBS
    encoded_payload = cobs.encode(payload_bytearray_with_crc)

    encoded_payload = b'\x00'+encoded_payload+b'\x00'
    
    return encoded_payload

def response_preprocessing(payload: bytearray) -> bytearray:
    payload.remove(0) # message start sign
    payload.remove(0) # message end sign

    decoded_payload = cobs.decode(payload)

    return decoded_payload

def handle_get_wifi_mac_response(payload: bytearray) -> str:
    payload_string = payload.hex()
    print(f'Received data: {payload_string}')

    header_command_info = payload_string[0:4]

    get_wifi_mac_command_response_header = str(hex(ble_commands['CMD_GET_WIFI_MAC_ADDRESS_RESPONSE']))[2:6]

    if header_command_info in get_wifi_mac_command_response_header:
        print("Response for get wifi command request")
        mac_address = payload_string[8:20].upper()
        print(mac_address)

        return mac_address
    
    return ""
    

def prepare_send_certificates_command() -> bytearray:
    # read certificate files
    full_chain_certificates = read_full_chain_certificate()
    private_key = read_private_key()

    certificates = certificate_pack_structure(full_chain_certificates, private_key)

    # prepare command payload to calculate crc16
    payload = ble_command_send_certificates(ble_commands["CMD_SEND_CERTIFICATES"],0,certificates)

    payload_bytearray = bytearray(payload)

    # calculate crc
    crc16 = calculate_crc16_ccitt_false(payload_bytearray)

    payload_with_crc = ble_command_send_certificates(ble_commands["CMD_SEND_CERTIFICATES"],crc16,certificates)

    payload_bytearray_with_crc = bytearray(payload_with_crc)

    # encode payload with crc with COBS
    encoded_payload = cobs.encode(payload_bytearray_with_crc)

    encoded_payload = b'\x00'+encoded_payload+b'\x00'
    
    return encoded_payload
