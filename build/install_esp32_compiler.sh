#!/bin/bash
set -x  # Print every command before execution
set -e  # Exit on the very first error
SCRIPT_DIR_RELATIVE="`dirname \"$0\"`"
SCRIPT_DIR="`realpath ${SCRIPT_DIR_RELATIVE}`"
EXTERNALS_DIR="${SCRIPT_DIR}/../app/externals"
INSTALL_COMPILER_SCRIPT=${EXTERNALS_DIR}/esp-idf/install.sh
#JLINK_INTERFACE_PATH="${HOME}/.espressif/tools/openocd-esp32/v0.10.0-esp32-20191114/openocd-esp32/share/openocd/scripts/interface/jlink.cfg"
${INSTALL_COMPILER_SCRIPT}
# Add missing jlink speed in configuration
#echo "adapter_khz 20000" >> ${JLINK_INTERFACE_PATH}