#!/bin/bash

# Note: This script is intended to use as an entry script for docker which builds all binaries for ESP32

SCRIPT_DIR_RELATIVE="`dirname \"$0\"`"
PROJECT_DIR="`realpath ${SCRIPT_DIR_RELATIVE}/..`"

IDF_PATH=${PROJECT_DIR}/app/externals/esp-idf

main()
{
    BUILD_DIR="`realpath ${SCRIPT_DIR_RELATIVE}/build_artifacts`"
    echo "PROJECT_DIR = ${PROJECT_DIR}"
    echo "BUILD_DIR = ${BUILD_DIR}"

    # Go to the project folder
    pushd ${PROJECT_DIR}

    # Convert CRLF if there are any to LF (Windows compatibility)
    find . -type f -print0 | xargs -0 dos2unix

    if [ ! -d "${PROJECT_DIR}/app/externals" ]; then
        mkdir ${PROJECT_DIR}/app/externals
    fi

    if [ ! -d "${PROJECT_DIR}/app/externals/CMock" ]; then
        ln -s /modules/app/externals/CMock ${PROJECT_DIR}/app/externals/CMock
    fi

    if [ ! -d "${PROJECT_DIR}/app/externals/googletest" ]; then
        ln -s /modules/app/externals/googletest ${PROJECT_DIR}/app/externals/googletest
    fi

    if [ ! -d "${PROJECT_DIR}/app/externals/esp-idf" ]; then
        ln -s /modules/app/externals/esp-idf ${PROJECT_DIR}/app/externals/esp-idf
    fi

    if [ ! -d "${PROJECT_DIR}/app/externals/Unity" ]; then
        ln -s /modules/app/externals/Unity ${PROJECT_DIR}/app/externals/Unity
    fi

    echo "----------------------------------------------------------------------------------------------------"

    echo "Activate ESP IDF environment..."
    export IDF_PATH=${IDF_PATH}
    . ${IDF_PATH}/export.sh

    if [ ! -d "${BUILD_DIR}" ]; then
        mkdir $BUILD_DIR
    fi

    cd $BUILD_DIR

    idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DIS_DEBUG_BUILD=ON build  
}

main
