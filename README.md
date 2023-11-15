# 1. TLDR - build in docker

## Prerequisites
### Windows
Install Docker Desktop: https://docs.docker.com/desktop/install/windows-install/ (please use WSL 2 backend)

### MAC
Install Docker Desktop: https://docs.docker.com/desktop/install/mac-install/

### Linux
Install Docker Desktop: https://docs.docker.com/desktop/install/linux-install/

Or 

Install Docker Engine: https://docs.docker.com/engine/install/ubuntu/

## Run Docker container and build firmware with ARM compiler
Open a terminal in the project's directory and issue the following command (be sure that the Docker Desktop or Engine runs in the background)
```
docker-compose up
```


# 2. Development
All the following commands were tested on Ubuntu 22.04

In this README, the following variables are assumed to be defined: `$PROJECT_DIR`, `$BUILD_DIR`.
E.g.: 
```
PROJECT_DIR=`pwd`  # source root - directory of this README
BUILD_DIR=$(realpath $PROJECT_DIR/../build-sparkhub-esp32)
```

## 2.1. Prerequisites
To compile without docker you need to get the following packages:
```
sudo apt-get install git wget flex bison gperf python python-pip python-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util clang-tidy-10
```
Add the user to `dialout` group in order to communicate with the ESP32 device without sudo:
```
sudo usermod -a -G dialout $USER
```

## 2.2. Build environment preparation
Before first build, some one-time preparation is required

## Don't forget to update the dependencies first
Instal package manager
```
pip install peru==1.3.1
```
Download dependencies
```
peru sync
cd app/externals && unzip esp-idf.zip && mv esp-idf-v4.4.3 esp-idf && cd ../../
```

### Apply patches:
```
cd ${PROJECT_DIR}/app/externals/esp-idf/components/bt/host/nimble/nimble
git apply ${PROJECT_DIR}/app/misc/ble-nimble-patch-1.patch
```

### 2.2.2. Build the esp compiler
```
cd $PROJECT_DIR 
./app/externals/esp-idf/install.sh esp32
```

## 2.3. Activate the environment in the terminal
Environment needs to be activated for all commands related to building, flashing and monitoring firmware. 
Needs to be run in every terminal.
```
. $PROJECT_DIR/app/externals/esp-idf/export.sh
```

## 2.4. Building
To build the firmware, issue the following commands:
```
mkdir $BUILD_DIR
cd $BUILD_DIR
idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DIS_DEBUG_BUILD=ON build  
```

## 2.5. Flashing
To build and flash:
```
mkdir $BUILD_DIR
cd $BUILD_DIR
idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DIS_DEBUG_BUILD=ON build flash  
```

One can also start the terminal monitor afterwards:
```
mkdir $BUILD_DIR
cd $BUILD_DIR
idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DIS_DEBUG_BUILD=ON build flash monitor
```

# 3. Flashing and monitoring without ESP-IDF

## Partitions for ESP32:
0x1000  bootloader.bin
0x8000  partition-table.bin
0x10000 sparkhub-LevelSense.bin
0xD000  ota_data_initial.bin

## Firmware can be flashed with ESP Tool:

### Command to erase flash:
esptool.py erase_flash

### For individual binaries in ESP32
esptool.py write_flash 0x1000 bootloader/bootloader.bin 0x8000 partition_table/partition-table.bin 0x10000 sparkhub-LevelSense.bin 0xD000 ota_data_initial.bin

### Or with GUI tool from Espressif on Windows, with partitions design as above

### Checking logs on Linux
sudo picocom /dev/ttyUSB0 -b 115200 2>&1 | ts "[%m-%d %H:%M:%S]" 2>&1 | tee -a sparkhub-esp32.logs

# 4. Pressure sensor

Documentation for used pressure sensor is in docs/TIZLA60 - TRANSDUCER ASSEMBLY - used.pdf

For devboard ESP32-DevKitC-32E V4 the sensor is connected as follows:

- IN sensor pin -> 5V0 devboard pin
- GND sensor pin -> GND devboard pin (the one between 13 and 12 pin)
- OUT sensor pin -> 32 devboard pin (ADC1 channel 4 is GPIO32)
