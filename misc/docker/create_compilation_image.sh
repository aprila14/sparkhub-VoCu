#!/bin/bash

# Note: This script must be run from the top repository directory due to docker restrictions
# related to docker build COPY command. Do not run the script from the subdirectories e.g. build.

set -e

IMAGE_PREFIX="${IMAGE_PREFIX:-i2a-esp32}"
UBUNTU_VERSION="${UBUNTU_VERSION:-18.04}"
SCRIPT_DIRECTORY=$(dirname "$0")


help()
{
    printf "Usage: $0 [OPTION]\n"
    printf "\n"
    printf "General options:\n"
    printf "  -h Show this help\n"
    printf "  -p docker image prefix\n"
    printf "  -u Specify Ubuntu version e.g. 20.04 (default: ${UBUNTU_VERSION})\n"
}

process_input_options()
{
    #Parse all options provided
    while getopts ":hp:u:" opt; do
        case $opt in
            h) printf "Build compilation docker file\n"
                help
                exit 0
            ;;
            p) IMAGE_PREFIX=${OPTARG}
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

    docker build \
      --file ${SCRIPT_DIRECTORY}/base.dockerfile \
      --build-arg UBUNTU_VERSION=${UBUNTU_VERSION} \
      --tag ${IMAGE_PREFIX}_base:${UBUNTU_VERSION} \
    .

    docker build \
      --file ${SCRIPT_DIRECTORY}/compile.dockerfile \
      --build-arg UBUNTU_VERSION=${UBUNTU_VERSION} \
      --build-arg IMAGE_PREFIX=${IMAGE_PREFIX} \
      --build-arg USER_TO_USE_FOR_RUN=user \
      --build-arg UID=${UID} \
      --tag ${IMAGE_PREFIX}_compile:${UBUNTU_VERSION} \
    .
}

main "$@"
