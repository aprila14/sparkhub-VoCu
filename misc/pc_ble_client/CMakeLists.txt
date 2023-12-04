cmake_minimum_required(VERSION 3.10)

project(pc_ble_client)

############ options and definitions ###########

add_subdirectory(externals/blzlib)
set(BLZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/blzlib)
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBSYSTEMD REQUIRED libsystemd)


set(ESP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../app/src)
set(PC_ESP_SHARED_SOURCES
    ${ESP_SRC_DIR}/ble/util/circular_buffer.cpp
    ${ESP_SRC_DIR}/ble/util/circular_buffer.h
    ${ESP_SRC_DIR}/ble/ble_controller/asynchronous_message.cpp
    ${ESP_SRC_DIR}/ble/ble_controller/asynchronous_message.h
    ${ESP_SRC_DIR}/ble/ble_controller/ble_controller.cpp
    ${ESP_SRC_DIR}/ble/ble_controller/ble_controller.h
    ${ESP_SRC_DIR}/ble/ble_controller/handle_command.cpp
    ${ESP_SRC_DIR}/ble/ble_controller/handle_command.h
    ${ESP_SRC_DIR}/ble/ble_controller/handle_response.cpp
    ${ESP_SRC_DIR}/ble/ble_controller/handle_response.h
    ${ESP_SRC_DIR}/ble/protocol/protocol_control.cpp
    ${ESP_SRC_DIR}/ble/protocol/protocol_control.h
    ${ESP_SRC_DIR}/ble/protocol/protocol_types.h
    ${ESP_SRC_DIR}/ble/protocol/protocol_types.cpp
    ${ESP_SRC_DIR}/ble/util/cobs.cpp
    ${ESP_SRC_DIR}/ble/util/cobs.h
    )

set(PC_ESP_SHARED_INCLUDE_DIRS
    ${ESP_SRC_DIR}/ble/
    ${ESP_SRC_DIR}/ble/driver/
    ${ESP_SRC_DIR}/ble/util/
    ${ESP_SRC_DIR}/ble/protocol/
    ${ESP_SRC_DIR}/ble/ble_controller/
    )



set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ALL_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/console_control.cpp
    ${SRC_DIR}/console_control.h
    ${SRC_DIR}/common/defines.h
    ${SRC_DIR}/common/misc.cpp
    ${SRC_DIR}/common/misc.h

    ${SRC_DIR}/communication/bleuart.cpp
    ${SRC_DIR}/communication/bleuart.h

    ${PC_ESP_SHARED_SOURCES}
    )

set(ALL_INCLUDE_DIRS
    ${BLZLIB_INCLUDE_DIR}
    ${SRC_DIR}
    ${SRC_DIR}/common
    ${SRC_DIR}/communication

    ${PC_ESP_SHARED_INCLUDE_DIRS}
    )


set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall -Wextra -Wcast-align -Wfloat-equal -Wuninitialized \
    -Wshadow -Wsign-conversion -Wstrict-overflow=3 -Wswitch-default")


# Flags for development, to reduce uninteresting warnings
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-nonliteral -Wno-unused-variable \
    -Wno-unused-parameter -Wno-strict-aliasing")


add_definitions(-DENABLE_PRINTS_AND_LOGS)
add_definitions(-DIS_DEBUG_BUILD)


# Macro to differentiate the code for the PC/ESP protocol parts
# TODO - consider using separate files wit approptiate functions instead of a common file!
add_definitions(-DIS_PC)


add_executable(pc_ble_client ${ALL_SOURCES})
target_include_directories(pc_ble_client PUBLIC ${ALL_INCLUDE_DIRS})
target_link_libraries(pc_ble_client blzlib pthread ${LIBSYSTEMD_LIBRARIES})
