#!/bin/bash

set -x  # Print every command before execution
set -e  # Exit on the very first error


SCRIPT_DIR_RELATIVE="`dirname \"$0\"`"
SOURCE_ROOT_DIR="`realpath ${SCRIPT_DIR_RELATIVE}/`"
BIN_NAME="pc_ble_client"


help()
{
    printf "Usage: $0 [OPTION]\n"
    printf "\n"
    printf "General options:\n"
    printf "  -h Show this help\n"
    printf "  -o Specify output directory to copy the firmware files (default: None)\n"
}

process_input_options()
{
    #Parse all options provided
    while getopts ":ho:" opt; do
        case $opt in
            h) printf "Build I2A firmware\n"
                help
                exit 0
            ;;
            o) OUTPUT_DIR=$(realpath ${OPTARG})
            ;;
            \?) echo "Invalid option -$OPTARG" >&2
                help
                exit 1
            ;;
        esac
    done
    shift $((OPTIND-1)) #This tells getopts to move on to the next argument.
}

copy_firmware_to_output_directory()
{
    if [ -z "${OUTPUT_DIR}" ]
    then
        echo "Firmware not copied"
    else
        mkdir -p ${OUTPUT_DIR}
        cp ${BIN_NAME} ${OUTPUT_DIR}
        echo "Binary copied to ${OUTPUT_DIR}"
    fi
}

main()
{
    process_input_options $@

    mkdir pc_ble_client_build

    BUILD_DIR="`pwd`"
    echo "SOURCE_ROOT_DIR = ${SOURCE_ROOT_DIR}"
    echo "BUILD_DIR = ${BUILD_DIR}"


    echo "Building the PC BLE client..."
    cmake ${SOURCE_ROOT_DIR}
    make

    copy_firmware_to_output_directory
}

main "$@"
