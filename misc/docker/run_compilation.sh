#!/bin/bash

set -e

IMAGE_PREFIX="i2a-esp32"
TOP_DIRECTORY=$(realpath $(dirname $0)/../..)

# Change these variables to match your setup or pass as a parameter
INPUT_DIR=${INPUT_DIR:-$(pwd)}
OUTPUT_DIR=${OUTPUT_DIR:-$(pwd)}
NUMBER_OF_PROCESSORS=$(grep -c ^processor /proc/cpuinfo)
INTERACTIVE_PARAMETERS="-i -t"
DEVBOARD_VERSION_PARAMETERS=""
UBUNTU_VERSION="${UBUNTU_VERSION:-18.04}"

SRC_DIR=${TOP_DIRECTORY}
SRC_DIR_IN_DOCKER="/media/src"


help()
{
    printf "Usage: $0 [OPTION]\n"
    printf "\n"
    printf "General options:\n"
    printf "  -h  Show this help\n"
    printf "Compilation options:\n"
    printf "  -i  input directory\n"
    printf "  -o  output directory\n"
    printf "  -j  Number of processors to use for compilation (default: use all CPUs available)\n"
    printf "  -T  do not be interactive (to be used on jenkins)\n"
    printf "  -d  Build firmware for the ESP32 devboard (with hardware mocks)\n"
    printf "  -u  Specify Ubuntu version e.g. 20.04 (default: ${UBUNTU_VERSION})\n"

}

process_input_options()
{
    # Parse all options provided
    while getopts ":hi:o:j:Tdu:" opt; do
        case "${opt}" in
            h) printf "Compile ${IMAGE_PREFIX} firmware binaries in docker container\n"
                help
                exit 0
            ;;
            i) INPUT_DIR=${OPTARG}
            ;;
            o) OUTPUT_DIR=${OPTARG}
            ;;
            j) NUMBER_OF_PROCESSORS=${OPTARG}
            ;;
            T) INTERACTIVE_PARAMETERS=""
            ;;
            d) DEVBOARD_VERSION_PARAMETERS="-d"
                echo "ERROR! NOT IMPLEMENTED"
                exit 1
            ;;
            u) UBUNTU_VERSION=${OPTARG}
            ;;
            \?) echo "Invalid option -$OPTARG" >&2
                help
                exit 1
            ;;
        esac
    done
    shift $((OPTIND-1))  # This tells getopts to move on to the next argument.
}


main()
{
    process_input_options $@

    mkdir -p ${OUTPUT_DIR}
    USER_TO_USE_FOR_RUN="user"

    # add ";/bin/bash" ath the end of the docker command (after running the script) to stay in the docker environment
    docker run --rm ${INTERACTIVE_PARAMETERS} --hostname="wizzdev.pl" \
--memory=8192m --cpus=${NUMBER_OF_PROCESSORS} \
--mount src=${SRC_DIR},target=${SRC_DIR_IN_DOCKER},type=bind \
--mount src=$(realpath ${INPUT_DIR}),target=/media/input,type=bind,readonly \
--mount src=$(realpath ${OUTPUT_DIR}),target=/media/output,type=bind \
${IMAGE_PREFIX}_compile:${UBUNTU_VERSION} \
/bin/bash -c " \
cd /home/${USER_TO_USE_FOR_RUN} \
&& sudo --user=${USER_TO_USE_FOR_RUN} --set-home /bin/bash -c \
  \"mkdir -p tests && cd tests && ${SRC_DIR_IN_DOCKER}/build/run_tests.sh -c -o /media/output -j ${NUMBER_OF_PROCESSORS}\" \
&& sudo --user=${USER_TO_USE_FOR_RUN} --set-home ${SRC_DIR_IN_DOCKER}/build/build_all.sh \
  ${DEVBOARD_VERSION_PARAMETERS} \
  -e ${SRC_DIR_IN_DOCKER}/app/externals/esp-idf -S -o /media/output \
&& sudo --user=${USER_TO_USE_FOR_RUN} --set-home ${SRC_DIR_IN_DOCKER}/misc/pc_ble_client/build_pc_client.sh \
  -o /media/output \
&& sudo --user=${USER_TO_USE_FOR_RUN} --set-home /bin/bash -c \
  \"mkdir -p static_analysis && cd static_analysis && ${SRC_DIR_IN_DOCKER}/build/run_static_analysis.sh -o /media/output -j ${NUMBER_OF_PROCESSORS}\" \
"

}

main "$@"

echo "DONE! Build artifacts in '${OUTPUT_DIR}'"
