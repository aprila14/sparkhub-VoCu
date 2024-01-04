OUTPUT_DIRECTORY = 'Program_output'

BLE_LOG_FILE_NAME = 'log_data.txt'

ROOT_CA_NAME = 'azure-iot.root.ca.cert.pem'
INTERMEDIATE_CERTIFICATE_NAME = 'azure-iot.intermediate.cert.pem'
INTERMEDIATE_KEY_NAME = 'azure-iot.intermediate.key.pem'

INTERMEDIATE_KEY_PASSWORD = b'1234'  # Use None if the key is not encrypted

CERTS_PRIVATE_DIRECTORY = 'private'
CERTS_CSR_DIRECTORY = 'csr'
CERTS_DIRECTORY = 'certs'

DEVICE_CERTIFICATE_EXPIRATION_TIME_DAYS = 40 * 365 # 40 years
