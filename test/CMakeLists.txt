cmake_minimum_required(VERSION 3.10)

project(esp32_test_on_pc
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_VERBOSE_MAKEFILE OFF)

# ##############################################################################
# LINTING
# ##############################################################################
if(RUN_LINTING AND UNIX)
    find_program(CLANG_TIDY_COMMAND NAMES clang-tidy-10)

    if(NOT CLANG_TIDY_COMMAND)
        message(FATAL_ERROR "CMake_RUN_CLANG_TIDY is ON but clang-tidy is not found!")
        set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
    else()
        set(CLANG_TIDY_CHECKS
            "\
 -*,\
 bugprone-*,\
 -bugprone-branch-clone,\
 -bugprone-reserved-identifier,\
 cert-*,\
 -cert-dcl37-c,\
 -cert-dcl51-cpp,\
 -cert-err58-cpp,\
 cppcoreguidelines-*,\
 -cppcoreguidelines-avoid-c-arrays,\
 -cppcoreguidelines-avoid-magic-numbers,\
 -cppcoreguidelines-avoid-non-const-global-variables,\
 -cppcoreguidelines-macro-usage,\
 -cppcoreguidelines-owning-memory,\
 -cppcoreguidelines-pro-bounds-array-to-pointer-decay,\
 -cppcoreguidelines-pro-bounds-constant-array-index,\
 -cppcoreguidelines-pro-bounds-pointer-arithmetic,\
 -cppcoreguidelines-pro-type-union-access,\
 -cppcoreguidelines-pro-type-vararg,\
 -cppcoreguidelines-special-member-functions,\
 hicpp-*,\
 -hicpp-use-auto,\
 -hicpp-avoid-c-arrays,\
 -hicpp-braces-around-statements,\
 -hicpp-no-array-decay,\
 -hicpp-special-member-functions,\
 -hicpp-uppercase-literal-suffix,\
 -hicpp-vararg,\
 readability-*,\
 -readability-braces-around-statements,\
 -readability-else-after-return,\
 -readability-implicit-bool-conversion,\
 -readability-magic-numbers,\
 -readability-named-parameter,\
 -readability-redundant-member-init,\
 -readability-simplify-boolean-expr,\
 -readability-static-accessed-through-instance,\
 -readability-uppercase-literal-suffix\
 ")

        set(CLANG_TIDY_WARNINGS_AS_ERRORS "-warnings-as-errors=*;")

        # set(CLANG_TIDY_WARNINGS_AS_ERRORS "")  # if you would like to disbale error, to quickly fix many warning instead
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-format-style=file;${CLANG_TIDY_WARNINGS_AS_ERRORS}-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/src/*'")
    endif()
endif()

add_definitions(-DTESTING)

set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TOP_REPO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(COMPONENT_PATH "${TOP_REPO_PATH}/app")
set(SRC_DIR "${TOP_REPO_PATH}/app/src/")

include(${COMPONENT_PATH}/src/sources_list.cmake)

add_definitions(-DIS_ESP)

# Test files - independent from the main project sources
set(TEST_FILES
    ${TEST_SRC_DIR}/main.cpp
    ${TEST_SRC_DIR}/code_mocks/config_nvs_mock.h
    ${TEST_SRC_DIR}/code_mocks/app_controller_mock.h
    ${TEST_SRC_DIR}/code_mocks/light_control_interface_mock.h
    ${TEST_SRC_DIR}/test_utils.cpp
    ${TEST_SRC_DIR}/test_utils.h

    ${TEST_SRC_DIR}/app_controller_test.cpp
    ${TEST_SRC_DIR}/config_nvs_test.cpp
    ${TEST_SRC_DIR}/custom_types_test.cpp
    ${TEST_SRC_DIR}/example_test.cpp
    ${TEST_SRC_DIR}/json_parser_test.cpp
    ${TEST_SRC_DIR}/led_control_test.cpp
    ${TEST_SRC_DIR}/reset_button_handler_test.cpp
    ${TEST_SRC_DIR}/wifi_controller_test.cpp
    ${TEST_SRC_DIR}/mqtt_client_controller_test.cpp
    ${TEST_SRC_DIR}/cloud_controller_test.cpp

    ${TEST_SRC_DIR}/cobs_test.cpp
    ${TEST_SRC_DIR}/test_circular_buffer.cpp
)

set(TEST_INCLUDE_DIRS
    ${TEST_SRC_DIR}
    ${TEST_SRC_DIR}/code_mocks
)

# MOCK_FILES
set(ESP_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mocks/headers_for_mocks/")

# all headers copied directly from the esp-idf (some modified to compile)
set(ESP_HEADERS_FILES
    ${ESP_HEADERS_DIR}/log/include/esp_log.h
    ${ESP_HEADERS_DIR}/log/include/esp_log_internal.h
    ${ESP_HEADERS_DIR}/esp_rom/include/esp_rom_sys.h
    ${ESP_HEADERS_DIR}/esp_rom/include/esp32s3/rom/ets_sys.h
    ${ESP_HEADERS_DIR}/driver/include/driver/gpio.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_err.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_types.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_bit_defs.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_attr.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/esp_intr_alloc.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/soc_caps.h
    ${ESP_HEADERS_DIR}/soc/include/soc/gpio_periph.h
    ${ESP_HEADERS_DIR}/hal/include/hal/gpio_types.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/io_mux_reg.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/gpio_struct.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/gpio_reg.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/gpio_sig_map.h
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/soc.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_assert.h
    ${ESP_HEADERS_DIR}/esp_rom/include/esp32s3/rom/gpio.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/FreeRTOS.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/task.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/semphr.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/queue.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/event_groups.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/timers.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_compiler.h
    ${ESP_HEADERS_DIR}/freertos/include/esp_additions/freertos/FreeRTOSConfig.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/projdefs.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/portable.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/xtensa_config.h
    ${ESP_HEADERS_DIR}/xtensa/include/xtensa/hal.h
    ${ESP_HEADERS_DIR}/xtensa/esp32s3/include/xtensa/config/core.h
    ${ESP_HEADERS_DIR}/xtensa/esp32s3/include/xtensa/config/system.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/xtensa_context.h
    ${ESP_HEADERS_DIR}/xtensa/include/xtensa/xtensa-versions.h
    ${ESP_HEADERS_DIR}/xtensa/include/xtensa/xtensa_context.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/deprecated_definitions.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/portmacro.h
    ${ESP_HEADERS_DIR}/xtensa/include/xtensa/xtruntime.h
    ${ESP_HEADERS_DIR}/esp_system/include/esp_private/crosscore_int.h
    ${ESP_HEADERS_DIR}/esp_timer/include/esp_timer.h
    ${ESP_HEADERS_DIR}/esp_system/include/esp_system.h
    ${ESP_HEADERS_DIR}/esp_common/include/esp_idf_version.h
    ${ESP_HEADERS_DIR}/newlib/platform_include/esp_newlib.h
    ${ESP_HEADERS_DIR}/freertos/include/freertos/list.h
    ${ESP_HEADERS_DIR}/nvs_flash/include/nvs_flash.h
    ${ESP_HEADERS_DIR}/nvs_flash/include/nvs.h
    ${ESP_HEADERS_DIR}/spi_flash/include/esp_partition.h
    ${ESP_HEADERS_DIR}/esp_rom/include/esp32c3/rom/esp_flash.h
    ${ESP_HEADERS_DIR}/spi_flash/include/esp_spi_flash.h
    ${ESP_HEADERS_DIR}/spi_flash/include/esp_spi_flash_counters.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/soc/spinlock.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_wifi.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_wifi_types.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_private/esp_wifi_types_private.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/esp_interface.h
    ${ESP_HEADERS_DIR}/esp_event/include/esp_event_base.h
    ${ESP_HEADERS_DIR}/esp_event/include/esp_event.h
    ${ESP_HEADERS_DIR}/esp_event/include/esp_event_base.h
    ${ESP_HEADERS_DIR}/esp_event/include/esp_event_legacy.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_ip_addr.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_types.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_defaults.h
    ${ESP_HEADERS_DIR}/tcpip_adapter/include/tcpip_adapter.h
    ${ESP_HEADERS_DIR}/tcpip_adapter/include/tcpip_adapter_types.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_sta_list.h
    ${ESP_HEADERS_DIR}/esp_netif/lwip/esp_netif_lwip_internal.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_ppp.h
    ${ESP_HEADERS_DIR}/esp_netif/include/esp_netif_slip.h

    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/autoip.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/errno.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/arch.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/altcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/api.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/snmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/sockets.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/udp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ethip6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/stats.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/tcpbase.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/err.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/mem.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip6_zone.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/netdb.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/raw_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/api_msg.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/altcp_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/mem_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/memp_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/tcp_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/memp_std.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/sockets_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/nd6_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/priv/tcpip_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip6_frag.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/dns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/dhcp6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/inet_chksum.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip4_napt.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_snmpv2_usm.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmpv3.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_mib2.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/tftp_server.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_threadsync.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/fs.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/netbiosns_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/altcp_tls_mbedtls_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/smtp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/smtp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/sntp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_table.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_snmpv2_framework.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/lwiperf.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/netbiosns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_core.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/httpd_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/sntp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_scalar.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/altcp_proxyconnect.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/httpd.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/tftp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/http_client.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip4_frag.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/if_api.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/nd6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/etharp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/tcpip.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/netifapi.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/inet.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/memp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/debug.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/sys.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/netif.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/autoip.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/ethernet.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/udp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/ip6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/dns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/ip.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/dhcp6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/nd6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/etharp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/iana.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/tcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/mld6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/icmp6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/igmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/icmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/dhcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/ip4.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/prot/ieee.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/tcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/timeouts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/opt.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/init.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/altcp_tls.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip6_addr.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/netbuf.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/raw.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/mld6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/icmp6.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/igmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip4_addr.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/icmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/dhcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip4.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/ip_addr.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/altcp_tcp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/lwip_napt.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/sio.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/def.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/pbuf.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/lwipopts.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/arch/cc.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/arch/sys_arch.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/arch/vfs_lwip.h

    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_snmpv2_usm.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmpv3.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_mib2.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/tftp_server.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_threadsync.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/fs.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/netbiosns_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/altcp_tls_mbedtls_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/smtp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/smtp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/sntp.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_table.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_snmpv2_framework.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/lwiperf.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/netbiosns.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_core.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/httpd_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/sntp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/snmp_scalar.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/altcp_proxyconnect.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mdns_priv.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/httpd.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/tftp_opts.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/http_client.h
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/mqtt_opts.h

    ${ESP_HEADERS_DIR}/lwip/include/apps/dhcpserver/dhcpserver.h

    ${ESP_HEADERS_DIR}/esp_system/include/esp_task.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_private/esp_wifi_private.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_wifi_crypto_types.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_private/wifi_os_adapter.h
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_wifi_default.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/netif/dhcp_state.h
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/sntp/sntp_get_set_time.h

    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/reset_reasons.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/esp_mac.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/esp_chip_info.h
    ${ESP_HEADERS_DIR}/esp_hw_support/include/esp_random.h
    ${ESP_HEADERS_DIR}/freertos/include/esp_additions/freertos/FreeRTOSConfig.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/FreeRTOSConfig_arch.h

    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_uuid.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/sysinit/sysinit.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/hci_common.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/nimble_npl.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/nimble_npl_os.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/endian.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/port/include/console/console.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/port/include/esp_nimble_mem.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/esp-hci/include/esp_nimble_hci.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/ble.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/nimble/nimble_port.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/nimble_npl.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_store.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_hci.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_sm.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_l2cap.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_adv.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_pvcy.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_att.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_ibeacon.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_eddystone.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_uuid.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_monitor.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_log.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_id.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_stop.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_gap.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_hs_mbuf.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/ble_gatt.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/syscfg/syscfg.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/port/include/esp_nimble_cfg.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os_trace_api.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os_mbuf.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os_cputime.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os_error.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/endian.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os_mempool.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/os.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/os/queue.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/npl_freertos.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/modlog/modlog.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/log/log.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/log_common/log_common.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/log_common/ignore.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/nimble_opt.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/nimble_opt_auto.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/nimble/ble_hci_trans.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h

    ${ESP_HEADERS_DIR}/mqtt/esp-mqtt/include/mqtt_client.h
    ${ESP_HEADERS_DIR}/esp_https_ota/include/esp_https_ota.h
    ${ESP_HEADERS_DIR}/esp_http_client/include/esp_http_client.h
    ${ESP_HEADERS_DIR}/nghttp/port/include/http_parser.h
    ${ESP_HEADERS_DIR}/bootloader_support/include/bootloader_common.h
    ${ESP_HEADERS_DIR}/bootloader_support/include/esp_app_format.h

    ${ESP_HEADERS_DIR}/heap/include/esp_heap_caps.h
    ${ESP_HEADERS_DIR}/tcp_transport/include/esp_transport.h
    ${ESP_HEADERS_DIR}/tcp_transport/include/esp_transport_tcp.h
    ${ESP_HEADERS_DIR}/tcp_transport/include/esp_transport_ssl.h
    ${ESP_HEADERS_DIR}/esp-tls/esp_tls.h
    ${ESP_HEADERS_DIR}/esp-tls/esp_tls_errors.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/portmacro.h
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/portbenchmark.h

    # OTA
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_cbor_private.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_config_defaults.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_http_interface.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_interface_private.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_mqtt_interface.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_mqtt_private.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_os_interface.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_platform_interface.h
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/ota_private.h

    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/ota_os_freertos.h

    ${ESP_HEADERS_DIR}/standard/coreJSON/source/include/core_json.h

    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/include/core_mqtt.h
    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/include/core_mqtt_config_defaults.h
    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/include/core_mqtt_state.h
    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/include/core_mqtt_serializer.h
    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/interface/transport_interface.h

    ${ESP_HEADERS_DIR}/standard/backoffAlgorithm/source/include/backoff_algorithm.h

    ${ESP_HEADERS_DIR}/crypto/iot_crypto.h
    ${ESP_HEADERS_DIR}/network_transport/tls_freertos.h
    ${ESP_HEADERS_DIR}/ota/aws_esp_ota_ops.h
    ${ESP_HEADERS_DIR}/ota/ota_pal.h
    ${ESP_HEADERS_DIR}/posix-compat/semaphore.h
)

# Directories where ESP_HEADERS_FILES are
set(ESP_HEADERS_INCLUDE_DIRS
    ${ESP_HEADERS_DIR}/
    ${ESP_HEADERS_DIR}/log/include/
    ${ESP_HEADERS_DIR}/esp_rom/include/

    # ${ESP_HEADERS_DIR}/esp_rom/include/esp32s3/rom/
    ${ESP_HEADERS_DIR}/driver/include
    ${ESP_HEADERS_DIR}/driver/include/driver/
    ${ESP_HEADERS_DIR}/esp_common/include/
    ${ESP_HEADERS_DIR}/xtensa/include/
    ${ESP_HEADERS_DIR}/esp_system/include/
    ${ESP_HEADERS_DIR}/esp_hw_support/include/
    ${ESP_HEADERS_DIR}/soc/esp32/include/
    ${ESP_HEADERS_DIR}/soc/include/
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/soc/
    ${ESP_HEADERS_DIR}/hal/include/hal/
    ${ESP_HEADERS_DIR}/hal/include/
    ${ESP_HEADERS_DIR}/freertos/include/
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/
    ${ESP_HEADERS_DIR}/xtensa/esp32s3/include/
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/
    ${ESP_HEADERS_DIR}/freertos/include/esp_additions/freertos/
    ${ESP_HEADERS_DIR}/esp_timer/include/
    ${ESP_HEADERS_DIR}/newlib/platform_include/
    ${ESP_HEADERS_DIR}/nvs_flash/include/
    ${ESP_HEADERS_DIR}/spi_flash/include/
    ${ESP_HEADERS_DIR}/esp_rom/include/esp32c3/rom/
    ${ESP_HEADERS_DIR}/freertos/include/freertos/
    ${ESP_HEADERS_DIR}/esp_event/include
    ${ESP_HEADERS_DIR}/esp_wifi/include/
    ${ESP_HEADERS_DIR}/esp_wifi/include/esp_private/
    ${ESP_HEADERS_DIR}/esp_netif/include/
    ${ESP_HEADERS_DIR}/tcpip_adapter/include/
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/apps/
    ${ESP_HEADERS_DIR}/lwip/include/apps
    ${ESP_HEADERS_DIR}/lwip/port/esp32/include/
    ${ESP_HEADERS_DIR}/lwip/lwip/src/include/lwip/apps/
    ${ESP_HEADERS_DIR}/esp_netif/lwip/
    ${ESP_HEADERS_DIR}/soc/esp32s3/include/
    ${ESP_HEADERS_DIR}/freertos/include/esp_additions/

    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/port/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/port/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/esp-hci/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gatt/include/services/gatt/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gatt/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/nimble/include/nimble/
    ${ESP_HEADERS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/

    ${ESP_HEADERS_DIR}/mqtt/esp-mqtt/include/
    ${ESP_HEADERS_DIR}/esp_https_ota/include/
    ${ESP_HEADERS_DIR}/esp_http_client/include/
    ${ESP_HEADERS_DIR}/nghttp/port/include/
    ${ESP_HEADERS_DIR}/bootloader_support/include/

    ${ESP_HEADERS_DIR}/heap/include/
    ${ESP_HEADERS_DIR}/tcp_transport/include/
    ${ESP_HEADERS_DIR}/esp-tls/
    ${ESP_HEADERS_DIR}/freertos/port/xtensa/include/freertos/

    ${ESP_HEADERS_DIR}/crypto/
    ${ESP_HEADERS_DIR}/network_transport/
    ${ESP_HEADERS_DIR}/ota/
    ${ESP_HEADERS_DIR}/posix-compat/

    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/
    ${ESP_HEADERS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/
    ${ESP_HEADERS_DIR}/standard/corePKCS11/source/include/

    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/include/
    ${ESP_HEADERS_DIR}/standard/coreMQTT/source/interface/
    ${ESP_HEADERS_DIR}/standard/backoffAlgorithm/source/include/
)

set(ESP_GENERATED_MOCKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mocks/generated_mocks/")
set(ESP_GENERATED_MOCKS_FILES
    ${ESP_GENERATED_MOCKS_DIR}/esp_system/include/mock_esp_system.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_system/include/mock_esp_system.h
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/mock_nvs_flash.c
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/mock_nvs_flash.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_common/include/mock_esp_err.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_common/include/mock_esp_err.h
    ${ESP_GENERATED_MOCKS_DIR}/driver/include/driver/mock_gpio.c
    ${ESP_GENERATED_MOCKS_DIR}/driver/include/driver/mock_gpio.h
    ${ESP_GENERATED_MOCKS_DIR}/driver/include/driver/mock_ledc.h
    ${ESP_GENERATED_MOCKS_DIR}/driver/include/driver/mock_ledc.c
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/mock_nvs.c
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/mock_nvs.h
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_queue.c
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_queue.h
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_semphr.c
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_semphr.h
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_task.c
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_task.h
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_event_groups.c
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_event_groups.h
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_timers.c
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/mock_timers.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_event/include/mock_esp_event.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_event/include/mock_esp_event.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_wifi/include/mock_esp_wifi_default.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_wifi/include/mock_esp_wifi_default.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_wifi/include/mock_esp_wifi.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_wifi/include/mock_esp_wifi.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_netif/include/mock_esp_netif.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_netif/include/mock_esp_netif.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_hw_support/include/mock_esp_mac.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_hw_support/include/mock_esp_mac.h

    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_gatt.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_gatt.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_gap.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_gap.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/nimble/include/nimble/mock_nimble_port.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/nimble/include/nimble/mock_nimble_port.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/mock_nimble_port_freertos.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/mock_nimble_port_freertos.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_hs_id.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_hs_id.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_store.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_store.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/mock_ble_svc_gap.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/mock_ble_svc_gap.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/esp-hci/include/mock_esp_nimble_hci.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/esp-hci/include/mock_esp_nimble_hci.h
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_hs_mbuf.c
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/mock_ble_hs_mbuf.h

    ${ESP_GENERATED_MOCKS_DIR}/mqtt/esp-mqtt/include/mock_mqtt_client.c
    ${ESP_GENERATED_MOCKS_DIR}/mqtt/esp-mqtt/include/mock_mqtt_client.h
    ${ESP_GENERATED_MOCKS_DIR}/lwip/lwip/src/include/lwip/apps/mock_sntp.c
    ${ESP_GENERATED_MOCKS_DIR}/lwip/lwip/src/include/lwip/apps/mock_sntp.h
    ${ESP_GENERATED_MOCKS_DIR}/esp_https_ota/include/mock_esp_https_ota.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_https_ota/include/mock_esp_https_ota.h

    ${ESP_GENERATED_MOCKS_DIR}/heap/include/mock_esp_heap_caps.c
    ${ESP_GENERATED_MOCKS_DIR}/heap/include/mock_esp_heap_caps.h

    ${ESP_GENERATED_MOCKS_DIR}/crypto/mock_iot_crypto.c
    ${ESP_GENERATED_MOCKS_DIR}/crypto/mock_iot_crypto.h
    ${ESP_GENERATED_MOCKS_DIR}/network_transport/mock_tls_freertos.c
    ${ESP_GENERATED_MOCKS_DIR}/network_transport/mock_tls_freertos.h
    ${ESP_GENERATED_MOCKS_DIR}/ota/mock_aws_esp_ota_ops.c
    ${ESP_GENERATED_MOCKS_DIR}/ota/mock_aws_esp_ota_ops.h
    ${ESP_GENERATED_MOCKS_DIR}/ota/mock_ota_pal.c
    ${ESP_GENERATED_MOCKS_DIR}/ota/mock_ota_pal.h
    ${ESP_GENERATED_MOCKS_DIR}/posix-compat/mock_semaphore.c
    ${ESP_GENERATED_MOCKS_DIR}/posix-compat/mock_semaphore.h

    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/mock_ota_os_freertos.c
    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/mock_ota_os_freertos.h

    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/mock_ota.c
    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/mock_ota.h

    ${ESP_GENERATED_MOCKS_DIR}/standard/coreMQTT/source/include/mock_core_mqtt.c
    ${ESP_GENERATED_MOCKS_DIR}/standard/coreMQTT/source/include/mock_core_mqtt.h

    ${ESP_GENERATED_MOCKS_DIR}/esp_rom/include/esp32s3/rom/mock_gpio.c
    ${ESP_GENERATED_MOCKS_DIR}/esp_rom/include/esp32s3/rom/mock_gpio.h
)

set(ESP_GENERATED_MOCKS_INCLUDE_DIRS
    ${ESP_GENERATED_MOCKS_DIR}/log/include/
    ${ESP_GENERATED_MOCKS_DIR}/esp_system/include/
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/
    ${ESP_GENERATED_MOCKS_DIR}/esp_common/include/
    ${ESP_GENERATED_MOCKS_DIR}/driver/include/driver/
    ${ESP_GENERATED_MOCKS_DIR}/nvs_flash/include/
    ${ESP_GENERATED_MOCKS_DIR}/freertos/include/freertos/
    ${ESP_GENERATED_MOCKS_DIR}/esp_wifi/include/
    ${ESP_GENERATED_MOCKS_DIR}/esp_event/include/
    ${ESP_GENERATED_MOCKS_DIR}/esp_netif/include/
    ${ESP_GENERATED_MOCKS_DIR}/esp_hw_support/include

    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/nimble/include/nimble/
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/porting/npl/freertos/include/nimble/
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/esp-hci/include/
    ${ESP_GENERATED_MOCKS_DIR}/bt/host/nimble/nimble/nimble/host/include/host/

    ${ESP_GENERATED_MOCKS_DIR}/mqtt/esp-mqtt/include/
    ${ESP_GENERATED_MOCKS_DIR}/lwip/lwip/src/include/lwip/apps/
    ${ESP_GENERATED_MOCKS_DIR}/esp_https_ota/include/

    ${ESP_GENERATED_MOCKS_DIR}/crypto/
    ${ESP_GENERATED_MOCKS_DIR}/network_transport/
    ${ESP_GENERATED_MOCKS_DIR}/ota/
    ${ESP_GENERATED_MOCKS_DIR}/posix-compat/

    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/

    ${ESP_GENERATED_MOCKS_DIR}/aws/ota-for-aws-iot-embedded-sdk/source/include/

    ${ESP_GENERATED_MOCKS_DIR}/standard/coreMQTT/source/include/
)

set(ESP_IDF_FILES_COPIED_MANUALLY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mocks/copied_manually/")
set(ESP_IDF_FILES_COPIED_MANUALLY_FILES
    ${ESP_IDF_FILES_COPIED_MANUALLY_DIR}/cJSON/cJSON.c
    ${ESP_IDF_FILES_COPIED_MANUALLY_DIR}/cJSON/cJSON.h
    ${ESP_IDF_FILES_COPIED_MANUALLY_DIR}/cJSON/cJSON_Utils.c
    ${ESP_IDF_FILES_COPIED_MANUALLY_DIR}/cJSON/cJSON_Utils.h
)
set(ESP_IDF_FILES_COPIED_MANUALLY_INCLUDE_DIRS
    ${ESP_IDF_FILES_COPIED_MANUALLY_DIR}/cJSON/
)

set(MISC_MOCKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mocks/misc_mocks/")
set(MISC_MOCKS_FILES
    ${MISC_MOCKS_DIR}/external_variables.cpp
    ${MISC_MOCKS_DIR}/sdkconfig.h
)

set(EMISC_MOCKS_INCLUDE_DIRS
    ${MISC_MOCKS_DIR}/
)

add_executable(esp32_test_on_pc
    ${TESTABLE_APPLICATION_FILES}
    ${TEST_FILES}
    ${ESP_HEADERS_FILES}
    ${ESP_GENERATED_MOCKS_FILES}
    ${MISC_MOCKS_FILES}
    ${ESP_IDF_FILES_COPIED_MANUALLY_FILES}
)

target_include_directories(esp32_test_on_pc PUBLIC
    ${TEST_INCLUDE_DIRS}
    ${TESTABLE_APPLICATION_INCLUDE_DIRS}
    ${ESP_HEADERS_INCLUDE_DIRS}
    ${ESP_GENERATED_MOCKS_INCLUDE_DIRS}
    ${EMISC_MOCKS_INCLUDE_DIRS}
    ${ESP_IDF_FILES_COPIED_MANUALLY_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-format -Wno-unused-variable -Wno-unused-function")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wno-format -Wno-unused-variable -Wno-unused-function")

foreach(FILE IN ITEMS ${CMOCK_FILES})
    set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS
        "-Wno-comment -Wno-error -Wno-unused-function -Wno-return-type -Wno-cpp")
endforeach()

if(${BUILD_TESTS})
    # Specify compilation switches
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTESTING")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTESTING")

    message("CMake current dir: ${CMAKE_CURRENT_BINARY_DIR}")
    set(GTEST_INSTALLED_DIR "${CMAKE_BINARY_DIR}/externals/googletest/bin")
    set(GTest_DIR "${GTEST_INSTALLED_DIR}/lib/cmake/GTest/")
    find_package(GTest CONFIG REQUIRED NO_DEFAULT_PATH CONFIGS GTestConfig.cmake)
    include_directories("${GTEST_INSTALLED_DIR}/include")
    target_link_libraries(esp32_test_on_pc GTest::gtest GTest::gmock)
endif()

set(UNITY_INSTALLED_DIR "${CMAKE_BINARY_DIR}/externals/unity/bin")
set(unity_DIR "${UNITY_INSTALLED_DIR}/lib/cmake/unity/")
find_package(unity REQUIRED CONFIGS "unityConfig.cmake")
include_directories("${UNITY_INSTALLED_DIR}/include/unity")
target_link_libraries(esp32_test_on_pc unity::framework ${CMOCK_LIBRARY})

# Find CMOCK static library
find_library(CMOCK_LIBRARY
    NAMES cmock
    PATHS "${CMAKE_BINARY_DIR}/externals/cmock/src/")

# Include directory from src dir meson/ninja because the build does not install these files
include_directories("${CMAKE_SOURCE_DIR}/app/externals/CMock/src/")

target_link_libraries(esp32_test_on_pc ${CMOCK_LIBRARY} stdc++fs)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME sparkhub)

# Define coverage run environment
set(DIRECTORIES_TO_INCLUDE_IN_COVERAGE
    -d ${SRC_DIR}/
)

# Run all tests
add_custom_target(run_tests
    COMMAND echo "Start of tests"
    COMMAND esp32_test_on_pc
    COMMAND echo "End of tests"
    DEPENDS esp32_test_on_pc
)

# Run all tests with code coverage extraction
add_custom_target(run_coverage
    COMMAND echo "Start of tests"
    COMMAND lcov --directory . --zerocounters
    COMMAND esp32_test_on_pc
    COMMAND lcov --no-extern ${DIRECTORIES_TO_INCLUDE_IN_COVERAGE}
    --base-directory .
    --directory .
    --capture
    --output-file sparkhub.info
    COMMAND lcov --remove sparkhub.info '${CMAKE_BINARY_DIR}/externals/*' -o sparkhub.info.filtered
    COMMAND genhtml -o ${CMAKE_BINARY_DIR}/test_coverage -t "sparkhub Test Coverage" --num-spaces 4 sparkhub.info.filtered
    COMMAND echo "End of tests"
    DEPENDS esp32_test_on_pc
)

if(RUN_STATIC_ANALYSIS AND UNIX)
    set(STATIC_ANALYSER_FLAGS "--analyze \
    -Xanalyzer -analyzer-output=html \
    -Xanalyzer -o ${CMAKE_BINARY_DIR}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STATIC_ANALYSER_FLAGS} -Wno-unused-command-line-argument")

    add_library(static_analysis OBJECT EXCLUDE_FROM_ALL ${SRC_FILES})
    target_include_directories(static_analysis PUBLIC
        ${TEST_INCLUDE_DIRS}
        ${TESTABLE_APPLICATION_INCLUDE_DIRS}
        ${ESP_HEADERS_INCLUDE_DIRS}
        ${ESP_GENERATED_MOCKS_INCLUDE_DIRS}
        ${EMISC_MOCKS_INCLUDE_DIRS}
    )
endif()
