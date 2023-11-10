# 1. TLDR - build in docker

TBD


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
