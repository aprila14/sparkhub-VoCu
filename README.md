# 1. TLDR - build in docker

TBD


# 2. Development
In this README, the following variables are assumed to be defined: `$PROJECT_DIR`, `$BUILD_DIR`, `BUILD_TEST_DIR`.
E.g.: 
```
PROJECT_DIR=`pwd`  # source root - directory of this README
BUILD_DIR=$(realpath $PROJECT_DIR/../build-skogluft-esp32)
BUILD_TEST_DIR=$(realpath $PROJECT_DIR/../build-skogluft-esp32-test)
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

### 2.2.1. Don't forget to update the submodules first
```
git submodule init && git submodule update
git submodule update --init --recursive
```

### Apply patches:
```
cd $PROJECT_DIR
cd app/externals/Unity
git stash
git apply ../../misc/unity-patch-1.patch


cd $PROJECT_DIR
cd app/externals/CMock
git stash
git apply ../../misc/cmock-patch-1.patch

cd ${$PROJECT_DIR}
cd app/externals/esp-idf/components/bt/host/nimble/nimble
git stash
git apply ${SOURCE_ROOT_DIR}/app/misc/ble-nimble-patch-1.patch

cd $PROJECT_DIR
cd app/externals/aws-iot-device-sdk-embedded-C/libraries/aws/ota-for-aws-iot-embedded-sdk/source/dependency/coreJSON
git stash
git apply ${PROJECT_DIR}/app/misc/core_cjson-patch-1.patch

```

### 2.2.2. Build the esp compiler
```
cd $PROJECT_DIR 
./app/externals/esp-idf/install.sh esp32s3
```

## 2.3. Activate the environment in the terminal
Environment needs to be activated for all commands related to building, flashing and monitoring firmware. 
Needs to be run in every terminal.
```
. $PROJECT_DIR/app/externals/esp-idf/export.sh  # e.g.: . ../
```

## 2.4. Building
To build the firmware, issue the following commands:
```
mkdir $BUILD_DIR
cd $BUILD_DIR
idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DBUILD_IN_DEBUG_MODE=ON build  
# e.g.: idf.py -B . -C ../sparkhub-levelsense/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DBUILD_IN_DEBUG_MODE=ON build 
```

## 2.5. IDE editing
At this point you should be able to open the environment in the QtCreator.
 - fRun QtCreator (e.g. /opt/Qt/Tools/QtCreator/bin/qtcreator) from a terminal with active environment
 - Open the main CMakeLists.txt from the $PROJECT_DIR/
 - Import the automaticly generate kit (disable the desktop kit if active)

## 2.6. Flashing
To flash:
```
idf.py -B ./ -C $PROJECT_DIR/ --baud 921600 flash  # e.g.: idf.py -B ./ -C ./../sparkhub-levelsense/ --baud 921600 flash
```

One can also start the terminal monitor afterwards:
```
idf.py -B ./ -C $PROJECT_DIR/ --baud 921600 flash monitor  # e.g.: idf.py -B ./ -C ./../sparkhub-levelsense/ --baud 921600 flash monitor
```
Other commands to be combined: `build`, `erase_flash`, e.g.:
```
idf.py -B ./ -C ../sparkhub-levelsense/ --baud 921600 erase_flash build flash monitor
```

If the board and serial port converter doesn't support automatic control of RESET and BOOT pins control, one may need to drive them manually, e.g. with buttons.

## 2.7. Testing
To prepare the testing environment, build and run tests, issue the following commands: 
```
# Do not activate idf.py environment!
mkdir $BUILD_TEST_DIR
cd $BUILD_TEST_DIR
${PROJECT_DIR}/build/run_tests.sh  # e.g.: ../sparkhub-levelsense/build/run_tests.sh
```


## 2.8 Bluetooth PC client

Readme for running Bluetooth PC client is available [here](misc/pc_ble_client)

## 2.9 Bluetooth stack

Details about Bluetooth stack and information how to add new BLE commands can be found [here](app/src/ble)