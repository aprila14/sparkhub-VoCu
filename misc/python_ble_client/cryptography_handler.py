from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography import x509
from cryptography.x509.oid import NameOID
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend

from datetime import datetime, timezone, timedelta

import os.path

INTERMEDIATE_CERTIFICATE_PATH = 'misc/python_ble_client/certs/azure-iot-test-only.intermediate.cert.pem'
INTERMEDIATE_KEY_PATH = 'misc/python_ble_client/certs/azure-iot-test-only.intermediate.key.pem'

CERTS_PRIVATE_DIRECTORY = 'private'
CERTS_CSR_DIRECTORY = 'csr'
CERTS_DIRECTORY = 'certs'

def generate_private_key():
    key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
    )

    return key
def save_private_key_to_file(key):
    if not os.path.isdir(CERTS_PRIVATE_DIRECTORY):
        os.mkdir(CERTS_PRIVATE_DIRECTORY)

    # Write our key to disk for safe keeping
    with open(f"{CERTS_PRIVATE_DIRECTORY}/3C610511BD50.key.pem", "wb") as f:
        f.write(key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.TraditionalOpenSSL,
            encryption_algorithm=serialization.NoEncryption(),
        ))

def create_device_certificate_csr(key):
    if not os.path.isdir(CERTS_CSR_DIRECTORY):
        os.mkdir(CERTS_CSR_DIRECTORY)

    # Create a CSR builder
    csr_builder = x509.CertificateSigningRequestBuilder()
    csr_builder = csr_builder.subject_name(x509.Name([
        x509.NameAttribute(NameOID.COMMON_NAME, u"3C610511BD50")
    ]))

    # Sign the CSR with the private key
    csr = csr_builder.sign(
        key, 
        algorithm=hashes.SHA256(),
        backend=default_backend()
    )

    # Write the CSR to a file
    with open(f"{CERTS_CSR_DIRECTORY}/3C610511BD50.csr.pem", "wb") as csr_file:
        csr_file.write(csr.public_bytes(serialization.Encoding.PEM))

def sign_device_certificate():
    # Load the CA certificate
    with open(f"{CERTS_DIRECTORY}/azure-iot-test-only.intermediate.cert.pem", "rb") as ca_cert_file:
        ca_cert = x509.load_pem_x509_certificate(ca_cert_file.read(), default_backend())

    # Load the CA private key
    with open(f"{CERTS_PRIVATE_DIRECTORY}/azure-iot-test-only.intermediate.key.pem", "rb") as ca_key_file:
        ca_private_key = serialization.load_pem_private_key(
            ca_key_file.read(),
            password = b'1234',  # Use None if the key is not encrypted
            backend = default_backend()
        )

    # Load the CSR
    with open(f"{CERTS_CSR_DIRECTORY}/3C610511BD50.csr.pem", "rb") as csr_file:
        csr = x509.load_pem_x509_csr(csr_file.read(), default_backend())

    # Create a certificate builder
    cert_builder = (
        x509.CertificateBuilder()
        .subject_name(csr.subject)
        .issuer_name(ca_cert.subject)
        .public_key(csr.public_key())
        .serial_number(x509.random_serial_number())
        .not_valid_before(datetime.utcnow())
        .not_valid_after(datetime.utcnow() + timedelta(days=30))
    )

    # Add extensions if needed
    # cert_builder = cert_builder.add_extension(...)

    # Sign the certificate with the CA's private key
    certificate = cert_builder.sign(
        private_key=ca_private_key,
        algorithm=hashes.SHA256(),
        backend=default_backend()
    )

    # Write the signed certificate to a file
    with open(f"{CERTS_DIRECTORY}/3C610511BD50.cert.pem", "wb") as cert_file:
        cert_file.write(certificate.public_bytes(serialization.Encoding.PEM))

def create_full_chain_device_certificate():
    # Read the contents of the certificates
    cert_files = [
        f"{CERTS_DIRECTORY}/3C610511BD50.cert.pem",
        f"{CERTS_DIRECTORY}/azure-iot-test-only.intermediate.cert.pem",
        f"{CERTS_DIRECTORY}/azure-iot-test-only.root.ca.cert.pem"
    ]

    # Concatenate the certificates into a single file
    with open(f"{CERTS_DIRECTORY}/3C610511BD50-full-chain.cert.pem", "wb") as full_chain_file:
        for cert_file in cert_files:
            with open(cert_file, "rb") as cert:
                full_chain_file.write(cert.read())

key = generate_private_key()

save_private_key_to_file(key)

create_device_certificate_csr(key)

sign_device_certificate()

create_full_chain_device_certificate()
