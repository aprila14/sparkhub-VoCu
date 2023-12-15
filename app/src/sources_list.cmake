# Here are listead all the source files to be used for project generation and as part of tests

set(NOT_TESTABLE_APPLICATION_FILES
    ${SRC_DIR}/manual_tests/console_control.cpp
    ${SRC_DIR}/manual_tests/console_control.h
)

set(TESTABLE_APPLICATION_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/hardware/reset_button_handler.h
    ${SRC_DIR}/hardware/reset_button_handler.cpp
    ${SRC_DIR}/wifi/wifi_controller.cpp
    ${SRC_DIR}/wifi/wifi_controller.h
    ${SRC_DIR}/common/hw_misc.cpp
    ${SRC_DIR}/common/hw_misc.h
    ${SRC_DIR}/common/defines.h
    ${SRC_DIR}/common/commons.cpp
    ${SRC_DIR}/common/commons.h
    ${SRC_DIR}/common/custom_types.cpp
    ${SRC_DIR}/common/custom_types.h
    ${SRC_DIR}/common/os/sleep.h
    ${SRC_DIR}/common/os/mutex.h
    ${SRC_DIR}/common/config/config_nvs.cpp
    ${SRC_DIR}/common/config/config_nvs.h
    ${SRC_DIR}/controller/app_controller.cpp
    ${SRC_DIR}/controller/app_controller.h
    ${SRC_DIR}/controller/app_events.h

    # too much hardware interactions for the time beeing?
    ${SRC_DIR}/ble/driver/bleuart.cpp
    ${SRC_DIR}/ble/driver/bleuart.h
    ${SRC_DIR}/ble/driver/bleuart_esp.cpp
    ${SRC_DIR}/ble/driver/bleuart_esp.h

    ${SRC_DIR}/ble/util/circular_buffer.cpp
    ${SRC_DIR}/ble/util/circular_buffer.h
    ${SRC_DIR}/ble/ble_controller/asynchronous_message.cpp
    ${SRC_DIR}/ble/ble_controller/asynchronous_message.h
    ${SRC_DIR}/ble/ble_controller/ble_controller.cpp
    ${SRC_DIR}/ble/ble_controller/ble_controller.h
    ${SRC_DIR}/ble/ble_controller/handle_command.cpp
    ${SRC_DIR}/ble/ble_controller/handle_command.h
    ${SRC_DIR}/ble/ble_controller/handle_response.cpp
    ${SRC_DIR}/ble/ble_controller/handle_response.h
    ${SRC_DIR}/ble/protocol/protocol_control.cpp
    ${SRC_DIR}/ble/protocol/protocol_control.h
    ${SRC_DIR}/ble/protocol/protocol_types.h
    ${SRC_DIR}/ble/protocol/protocol_types.cpp
    ${SRC_DIR}/ble/util/cobs.cpp
    ${SRC_DIR}/ble/util/cobs.h

    ${SRC_DIR}/cloud/cloud_controller/cloud_controller.h
    ${SRC_DIR}/cloud/cloud_controller/cloud_controller.cpp
    ${SRC_DIR}/cloud/mqtt_client_controller/mqtt_client_controller.h
    ${SRC_DIR}/cloud/mqtt_client_controller/mqtt_client_controller.cpp
    ${SRC_DIR}/cloud/ntp_client/ntp_client.cpp
    ${SRC_DIR}/cloud/ntp_client/ntp_client.h
    ${SRC_DIR}/cloud/parser/json_parser.h
    ${SRC_DIR}/cloud/parser/json_parser.cpp

    ${SRC_DIR}/cloud/device_provisioning/device_provisioning.cpp
    ${SRC_DIR}/cloud/device_provisioning/device_provisioning.h

    ${SRC_DIR}/hardware/adc_pressure/adc_pressure.cpp
    ${SRC_DIR}/hardware/adc_pressure/adc_pressure.h
)

set(NOT_TESTABLE_APPLICATION_INCLUDE_DIRS
    ${SRC_DIR}/common/os
    ${SRC_DIR}/manual_tests
    ${SRC_DIR}/ble/driver/
)

set(TESTABLE_APPLICATION_INCLUDE_DIRS
    ${SRC_DIR}
    ${SRC_DIR}/controller
    ${SRC_DIR}/common
    ${SRC_DIR}/manual_tests
    ${SRC_DIR}/common/os
    ${SRC_DIR}/common/config
    ${SRC_DIR}/hardware
    ${SRC_DIR}/hardware/adc_pressure
    ${SRC_DIR}/wifi/
    ${SRC_DIR}/ble/
    ${SRC_DIR}/ble/driver/ # we don't want this file in tests, because we have software mocks for it?
    ${SRC_DIR}/ble/util/
    ${SRC_DIR}/ble/protocol/
    ${SRC_DIR}/ble/ble_controller/
    ${SRC_DIR}/cloud/cloud_controller/
    ${SRC_DIR}/cloud/mqtt_client_controller/
    ${SRC_DIR}/cloud/ntp_client/
    ${SRC_DIR}/cloud/parser/
    ${SRC_DIR}/cloud/device_provisioning/
)
