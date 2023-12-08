import os.path

import asyncio

import time
import datetime
from datetime import datetime

from cobs import cobs

from ble_protocol_control import prepare_get_wifi_mac_command, prepare_send_certificates_command

from utils import chunk_byte_array

from bleak import BleakClient, BleakScanner
from bleak.backends.characteristic import BleakGATTCharacteristic

# Program global variables
NOTIFY_RECEIVED_DATA = ''

output_directory = 'Program_output'

ble_log_file_name = 'log_data.txt'


async def scan_devices():
    print("Scanning available devices")
    devices_list = []
    devices = await BleakScanner.discover()
    for i, d in enumerate(devices):
        device = {'name': d.name, 'address': d.address}
        devices_list.append(device)
        print(f'{i}. {device}')
    return devices_list


async def find_ble_device(device_address):
    device = await BleakScanner.find_device_by_address(device_address)
    if device is None:
        print(f'Could not find device with address: {device_address}')
        return
    return device


def get_device_characteristics(ble_client):
    device_info = []

    print("Connected to device. Searching for services:")

    for i, service in enumerate(ble_client.services):
        print(f'{i}. [Service] {service}')
        service_item = {'service': service.description, 'uuid': service.uuid, 'characteristics': []}

        for j, char in enumerate(service.characteristics):
            print(f'    {j}. [Characteristic] {char} : {char.properties}')
            characteristics_item = {'uuid': char.uuid, 'characteristic': char.description, 'descriptors': []}

            for k, descriptor in enumerate(char.descriptors):
                print(f'        {k}. [Descriptor] {descriptor}')
                descriptor_item = {'uuid': descriptor.uuid, 'descriptor': descriptor.description}
                characteristics_item['descriptors'].append(descriptor_item)

            service_item['characteristics'].append(characteristics_item)
        device_info.append(service_item)

    return device_info


def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    global NOTIFY_RECEIVED_DATA

    data.remove(0) # message start sign
    data.remove(0) # message end sign

    decoded_payload = cobs.decode(data)

    print(f'Received data: {decoded_payload.hex()}')

    if os.path.isfile(os.path.join(output_directory, ble_log_file_name)):
        with open(os.path.join(output_directory, ble_log_file_name), 'a') as f:
            f.writelines(decoded_payload.hex())
            f.write('\n')


async def connect_to_notifications(ble_client, characteristic_TX_uuid, characteristic_RX_uuid):
    global NOTIFY_RECEIVED_DATA

    if not os.path.isdir(output_directory):
        os.mkdir(output_directory)

    if not os.path.isfile(os.path.join(output_directory, ble_log_file_name)):
        with open(os.path.join(output_directory, ble_log_file_name), 'w') as f:
            f.write("LOGGER file!\n")

    last_notify_received_data = ''

    print("Connected to device. Connecting for notifications:")

    await ble_client.start_notify(characteristic_TX_uuid, notification_handler)

    print("Notify started")

    certificates = prepare_send_certificates_command()

    chunk_size = 250
    chunks = chunk_byte_array(certificates, chunk_size)

    # payload = prepare_get_wifi_mac_command()

    for chunk in chunks:
        print("Sending chunk:", chunk)
        await ble_client.write_gatt_char(characteristic_RX_uuid, chunk, response=False)

    while True:
        print("Waiting for data")

        if last_notify_received_data != NOTIFY_RECEIVED_DATA:
            last_notify_received_data = NOTIFY_RECEIVED_DATA

            print("data incoming")

        await asyncio.sleep(0.1)


async def handle_ble_device(selected_device_address):
    device = await find_ble_device(selected_device_address)

    ble_client = BleakClient(device)
    try:
        await ble_client.connect()

        # available_services_and_characteristics = await get_services_and_characteristics(ble_client)
        available_services_and_characteristics = get_device_characteristics(ble_client)
        service_selected = int(input("Choose your Nordic UART Service service: "))

        if 'TX' in available_services_and_characteristics[service_selected]['characteristics'][0]['characteristic']:
            characteristic_RX_uuid = available_services_and_characteristics[service_selected]['characteristics'][1][
                'uuid']
            characteristic_TX_uuid = available_services_and_characteristics[service_selected]['characteristics'][0][
                'uuid']
        else:
            characteristic_RX_uuid = available_services_and_characteristics[service_selected]['characteristics'][0][
                'uuid']
            characteristic_TX_uuid = available_services_and_characteristics[service_selected]['characteristics'][1][
                'uuid']

        print(f"characteristic_RX_uuid-{characteristic_RX_uuid}")
        print(f"characteristic_TX_uuid-{characteristic_TX_uuid}")

        await connect_to_notifications(ble_client, characteristic_TX_uuid, characteristic_RX_uuid)
    except Exception as e:
        print(e)
    finally:
        print("Stop notify")
        await ble_client.stop_notify(characteristic_TX_uuid)
        print("Start disconnect")
        await ble_client.disconnect()
        print("Disconnected!")
