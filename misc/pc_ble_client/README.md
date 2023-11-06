# BLE PC client
This is a project to demonstrate the communication protocol on a PC.

# Prerequisities

Prerequsisities for blzlib:

```
sudo apt install libsystemd-dev
```

# Building
- Remember to run export.sh script (described in the main README file) in the terminal you are going to use for PC BLE client build.
- From now on, $PROJECT_DIR is assumed to be [misc/pc_ble_client](misc/pc_ble_client), $BUILD_DIR should be separate directory for PC BLE client build. 
- Remember to pull submodules first
- To build run the following commands:
```
cd $BUILD_DIR
cmake $PROJECT_DIR
make
./pc_ble_client "MA:CA:DD:RR:EE:SS"
```
- one can set MAC address also at the beginning of the main function for convenience

# Usage
After starting, the application will automatically connect to the BLE peripheral, with a reconnection mechanism.
After it happens, one can operate the application from the terminal to send commands to ESP32.

It is recommended to quit the application gracefully (instead of killing) by pressing 'q' key. 
It will disconnect the BLE device first.

# Some notes:
- It uses blzlib for BLE communication https://github.com/infsoft-locaware/blzlib
- Dependency: sudo apt install libsystemd-dev
- Blzlib gets built and then packages in a .a so that it can be statically linked

- BLZlib uses the new 'ConnectDevice' Bluez DBUS method, which is only available if the bluetooth daemon is started with the 'experimental' flag:
On Ubuntu-based systems, before using the utility do:
```
sudo service bluetooth stop
sudo /usr/lib/bluetooth/bluetoothd -E &
```

- The bluetooth daemon caches the characteristics and services of a bluetooth device and doesn't re-read them on new connections.
  If you make any changes on the bluetooth interface of device (like changing characteristics and UUIDs), delete the cache by removing:
    - the folder `/var/lib/bluetooth/<BT adapter MAC>/<BLE device MAC>`
    - the file `/var/lib/bluetooth/<BT adapter MAC>/cache/<BLE device MAC>`

The Arduino sketch is for the ESP32, which simply loops back any data received so that the BLE communication and the protocol parser can be tested.

With older distributions, newer `Bluez` library may be needed. To install it on your system:
```
sudo apt-get install libglib2.0-dev libdbus-1-dev libudev-dev libical-dev libreadline7 libreadline-dev
# follow this tutorial https://askubuntu.com/questions/612295/how-to-upgrade-to-the-latest-bluez-version-on-ubuntu-12-04-lts
# and then use locally installed bluetoothd:
sudo /usr/local/libexec/bluetooth/bluetoothd -E  
```

- if BLE dongle doesn't work
- check kernel logs `cat /var/log/kern.log`
- if `for rtl_bt/rtl8761b_fw.bin failed with error -2` then visit `https://askubuntu.com/questions/1339825/ubuntu-21-04-bluetooth-5-0-dongle-does-not-work-and-says-theres-no-firmware
