import asyncio

from ble_connection_controller import scan_devices, handle_ble_device


async def main():
    print("App start")

    available_devices = await scan_devices()
    device_selected = int(input("Please type the number of your device and press ENTER: "))

    selected_device_address = available_devices[device_selected]['address']

    await handle_ble_device(selected_device_address)
        
    print("App stop")

if __name__ == "__main__":
    asyncio.run(main())
