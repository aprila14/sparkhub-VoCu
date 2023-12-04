#!/bin/bash

set -x  # Print every command before execution
set -e  # Exit on the very first error


SCRIPT_DIR_RELATIVE="`dirname \"$0\"`"
SCRIPT_DIR="`realpath ${SCRIPT_DIR_RELATIVE}`"
CMOCK_DIR="${SCRIPT_DIR}/../../app/externals/CMock/lib/"
OUTPUT_DIRECTORY="`pwd`"
INPUT_FILE_PATH=""

GENERATE_MOCKS_SCRIPT_PATH="${CMOCK_DIR}/cmock.rb"
CMOCK_YAML_CONFIG_FILE_PATH="${SCRIPT_DIR}/project.yml"

function help (){
    printf "Usage: $0 [OPTION]\n"
    printf "./generate_mock.sh -i <path to header we want to mock> -o <output directory>"
    printf "\n"
    printf "General options:\n"
    printf "  -h Show this help\n"
    printf "  -o  - output directory\n"
    printf "  -i  - input header path\n"

}


process_input_options()
{
   
    #Parse all options provided
    while getopts ":h:o:i:" opt; do
        case $opt in
            h) printf "generate mock \n"
                help
                exit 0
            ;;
            o) echo "Output directory"
                OUTPUT_DIRECTORY=$(readlink -m $OPTARG)
            ;;
            i) echo "Input file path"
                INPUT_FILE_PATH=$(readlink -m $OPTARG)
            ;;
            \?) echo "Invalid option -$OPTARG" >&2
                help
                exit 1
            ;;
        esac
    done
    shift $((OPTIND-1)) #This tells getopts to move on to the next argument.
}

main()
{
    process_input_options $@

    pushd ${OUTPUT_DIRECTORY}
    ruby ${GENERATE_MOCKS_SCRIPT_PATH} -o${CMOCK_YAML_CONFIG_FILE_PATH} ${INPUT_FILE_PATH}

    popd
}
main "$@"
