cmake_minimum_required(VERSION 3.10)

# CMake run with -P flag therefore CMAKE_CURRENT_SOURCE_DIR points to build dir use COMPONENT_PATH to get current course path
set (SRC_DIR ${COMPONENT_PATH}/src)


set (EXTERNAL_LIBRARIES_SOURCES
    )

set (EXTERNAL_LIBRARIES_INCLUDE_DIRS
    )


include(${COMPONENT_PATH}/src/sources_list.cmake)

set (ALL_SOURCES
    ${EXTERNAL_LIBRARIES_SOURCES}

    "${OTA_SOURCES}"
    "${OTA_MQTT_SOURCES}"
    "${MQTT_SOURCES}"
    "${MQTT_SERIALIZER_SOURCES}"
    "${BACKOFF_ALGORITHM_SOURCES}"

    ${NOT_TESTABLE_APPLICATION_FILES}
    ${TESTABLE_APPLICATION_FILES}
    )


set (ALL_INCLUDE_DIRS
    ${EXTERNAL_LIBRARIES_INCLUDE_DIRS}

    "${CMAKE_CURRENT_LIST_DIR}"
    "${LOGGING_INCLUDE_DIRS}"
    "${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS}"
    "${OTA_INCLUDE_PUBLIC_DIRS}"
    "${OTA_INCLUDE_PRIVATE_DIRS}"
    "${MQTT_INCLUDE_PUBLIC_DIRS}"

    ${NOT_TESTABLE_APPLICATION_INCLUDE_DIRS}
    ${TESTABLE_APPLICATION_INCLUDE_DIRS}
    )


set (COMPONENT_REQUIRES
    nvs_flash cxx log
    esp_wifi bt wpa_supplicant lwip
    freertos driver
    esp_adc_cal
    esp_https_ota app_update
    esp_http_server json
    mqtt
    json
    tcp_transport
    bootloader_support
    app_update
    pthread
    hal
    )



set(COMPONENT_PRIV_REQUIRES)

set(COMPONENT_SRCS ${ALL_SOURCES}
    )

set(COMPONENT_ADD_INCLUDEDIRS ${ALL_INCLUDE_DIRS}
    "."
    )

register_component()


target_compile_options(${COMPONENT_TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>)
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Werror -Wno-format -Wno-unused-variable -Wno-unused-function
    -Wno-stringop-truncation)


foreach(FILE IN ITEMS ${EXTERNAL_LIBRARIES_SOURCES})
    set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS
        "-Wno-comment -Wno-error -Wno-unused-function -Wno-discarded-qualifiers")
endforeach()
