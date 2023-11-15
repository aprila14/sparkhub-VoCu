#!/bin/bash

# Note: This script is intended to use as an entry script for docker which builds all binaries for ESP32
USER_NAME=user
SOURCE_PATH=/home/$USER_NAME/repo
SOURCE_COPY_PATH=/home/$USER_NAME/repo_copy
BUILD_ARTIFACTS_PATH=/home/$USER_NAME/build_artifacts

PROJECT_DIR=${SOURCE_COPY_PATH}

IDF_PATH=${PROJECT_DIR}/app/externals/esp-idf

main()
{
    echo "PROJECT_DIR = ${PROJECT_DIR}"
    echo "BUILD_ARTIFACTS_PATH = ${BUILD_ARTIFACTS_PATH}"

    # if the source_copy folder does not exists, create it
    if [ ! -d $SOURCE_COPY_PATH ]; then
        mkdir $SOURCE_COPY_PATH
    else
        rm -rf $SOURCE_COPY_PATH/*
    fi

    cp -r $SOURCE_PATH/* $SOURCE_COPY_PATH
    
    if [ ! -d $BUILD_ARTIFACTS_PATH ]; then
        mkdir $BUILD_ARTIFACTS_PATH
    else
        find  $BUILD_ARTIFACTS_PATH/* ! -name 'Readme.md' -type f -exec rm -f {} +
    fi

    # Go to the project folder
    pushd ${PROJECT_DIR} || exit

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

    cd $BUILD_ARTIFACTS_PATH || exit

    idf.py -B . -C $PROJECT_DIR/ -DBUILD_WITH_PRINTS_AND_LOGS=ON -DIS_DEBUG_BUILD=ON build  
}

main
