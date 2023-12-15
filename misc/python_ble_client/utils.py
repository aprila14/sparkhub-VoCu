from crc import Calculator, Configuration

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

def chunk_byte_array(byte_array, chunk_size):
    return [byte_array[i:i + chunk_size] for i in range(0, len(byte_array), chunk_size)]
