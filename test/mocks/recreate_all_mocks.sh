#!/bin/bash

set -e
#set -x

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_REPO_DIR=$(realpath "${SCRIPT_DIR}/../../")
HEADER_FOR_MOCKS_DIR="${SCRIPT_DIR}/headers_for_mocks"
ESP32_COMPONENTS_DIR="${ROOT_REPO_DIR}/app/externals/esp-idf/components/"
AWS_PORT_DIR="${ROOT_REPO_DIR}/app/externals/port/"
AWS_SDK_DIR="${ROOT_REPO_DIR}/app/externals/aws-iot-device-sdk-embedded-C/libraries/"
OUTPUT_COMPONENTS_DIR="${SCRIPT_DIR}/generated_mocks"


# Declare an array variable
declare -a HEADERS_TO_COPY=(\
    "log/include/esp_log.h"
    "log/include/esp_log_internal.h"
    "esp_rom/include/esp_rom_sys.h"
    "esp_rom/include/esp32s3/rom/ets_sys.h"
    # "driver/include/driver/gpio.h"
    "driver/include/driver/ledc.h"
    "driver/include/driver/periph_ctrl.h"
    # "esp_common/include/esp_err.h"  # do not regenerate - manually edited to compile
    "esp_common/include/esp_types.h"
    "esp_common/include/esp_bit_defs.h"

    "esp_common/include/esp_attr.h"

    "esp_hw_support/include/esp_intr_alloc.h"

    # "soc/esp32s3/include/soc/soc_caps.h"  # do not regenerate - manually edited to compile
    # "soc/include/soc/gpio_periph.h"  # do not regenerate - manually edited to compile
    "hal/include/hal/gpio_types.h"
    "hal/include/hal/ledc_types.h"
    "soc/esp32s3/include/soc/io_mux_reg.h"
    "soc/esp32s3/include/soc/gpio_struct.h"
    "soc/esp32s3/include/soc/gpio_reg.h"
    "soc/esp32s3/include/soc/gpio_sig_map.h"
    "soc/esp32s3/include/soc/soc.h"
    "soc/esp32s3/include/soc/periph_defs.h"
    "esp_common/include/esp_assert.h"
    "esp_rom/include/esp32s3/rom/gpio.h"
    # "freertos/include/freertos/FreeRTOS.h"  # do not regenerate - manually edited to compile
    "freertos/include/freertos/task.h"
    # "freertos/include/freertos/event_groups.h"  # do not regenerate - manually edited to compile
     "freertos/include/freertos/timers.h"  # do not regenerate - manually edited to compile
    "freertos/include/freertos/semphr.h"
    "freertos/include/freertos/queue.h"
    "esp_common/include/esp_compiler.h"

    "freertos/include/freertos/projdefs.h"
    # "freertos/include/freertos/portable.h"  # do not regenerate - manually edited to disable some includes
    "freertos/port/xtensa/include/freertos/xtensa_config.h"
    "xtensa/include/xtensa/hal.h"
    # "xtensa/esp32s3/include/xtensa/config/core.h"  # do not regenerate - manually edited to disable some includes
    "xtensa/esp32s3/include/xtensa/config/system.h"
    "freertos/port/xtensa/include/freertos/xtensa_context.h"
    "xtensa/include/xtensa/xtensa-versions.h"
    # "xtensa/include/xtensa/xtensa_context.h"  # do not regenerate - manually edited to disable some includes
    "freertos/include/freertos/deprecated_definitions.h"
    # "freertos/port/xtensa/include/freertos/portmacro.h"  # do not regenerate - manually edited to compile
    # "xtensa/include/xtensa/xtruntime.h"  # do not regenerate - manually edited to disable some includes

    "esp_system/include/esp_private/crosscore_int.h"

    "esp_timer/include/esp_timer.h"
    # "esp_system/include/esp_system.h"  # do not regenerate - manually edited to compile
    "esp_common/include/esp_idf_version.h"
    # "newlib/platform_include/esp_newlib.h"  # do not regenerate - manually edited to disable some includes
    "freertos/include/freertos/list.h"
    "nvs_flash/include/nvs_flash.h"
    "nvs_flash/include/nvs.h"
    "spi_flash/include/esp_partition.h"
    "esp_rom/include/esp32c3/rom/esp_flash.h"
    "spi_flash/include/esp_spi_flash.h"
    "spi_flash/include/esp_spi_flash_counters.h"
    # "esp_hw_support/include/soc/spinlock.h"  # do not regenerate - manually edited to compile
    "esp_wifi/include/esp_wifi.h"
    "esp_wifi/include/esp_wifi_types.h"
    "esp_wifi/include/esp_private/esp_wifi_types_private.h"

    "esp_hw_support/include/esp_interface.h"

    "esp_event/include/esp_event_base.h"
    "esp_event/include/esp_event.h"
    "esp_event/include/esp_event_base.h"
    "esp_event/include/esp_event_legacy.h"
    "esp_netif/include/esp_netif.h"
    # "esp_netif/include/esp_netif_ip_addr.h"  # do not regenerate - manually edited to compile
     "esp_netif/include/esp_netif_types.h"  # do not regenerate - manually edited to compile
    "esp_netif/include/esp_netif_defaults.h"
    "tcpip_adapter/include/tcpip_adapter.h"
    "tcpip_adapter/include/tcpip_adapter_types.h"

    "esp_system/include/esp_task.h"

    "lwip/lwip/src/include/lwip/autoip.h"
    "lwip/lwip/src/include/lwip/errno.h"
    "lwip/lwip/src/include/lwip/arch.h"
    "lwip/lwip/src/include/lwip/altcp.h"
    "lwip/lwip/src/include/lwip/api.h"
    "lwip/lwip/src/include/lwip/snmp.h"
    "lwip/lwip/src/include/lwip/sockets.h"
    "lwip/lwip/src/include/lwip/udp.h"
    "lwip/lwip/src/include/lwip/ethip6.h"
    "lwip/lwip/src/include/lwip/stats.h"
    "lwip/lwip/src/include/lwip/tcpbase.h"
    "lwip/lwip/src/include/lwip/err.h"
    "lwip/lwip/src/include/lwip/mem.h"
    "lwip/lwip/src/include/lwip/ip6_zone.h"
    "lwip/lwip/src/include/lwip/netdb.h"
    "lwip/lwip/src/include/lwip/ip6.h"
    "lwip/lwip/src/include/lwip/priv/raw_priv.h"
    "lwip/lwip/src/include/lwip/priv/api_msg.h"
    "lwip/lwip/src/include/lwip/priv/altcp_priv.h"
    "lwip/lwip/src/include/lwip/priv/mem_priv.h"
    "lwip/lwip/src/include/lwip/priv/memp_priv.h"
    "lwip/lwip/src/include/lwip/priv/tcp_priv.h"
    "lwip/lwip/src/include/lwip/priv/memp_std.h"
    "lwip/lwip/src/include/lwip/priv/sockets_priv.h"
    "lwip/lwip/src/include/lwip/priv/nd6_priv.h"
    "lwip/lwip/src/include/lwip/priv/tcpip_priv.h"
    "lwip/lwip/src/include/lwip/ip6_frag.h"
    "lwip/lwip/src/include/lwip/dns.h"
    "lwip/lwip/src/include/lwip/ip.h"
    "lwip/lwip/src/include/lwip/dhcp6.h"
    "lwip/lwip/src/include/lwip/inet_chksum.h"
    "lwip/lwip/src/include/lwip/ip4_napt.h"
    "lwip/lwip/src/include/lwip/apps/snmp_snmpv2_usm.h"
    "lwip/lwip/src/include/lwip/apps/snmpv3.h"
    "lwip/lwip/src/include/lwip/apps/snmp_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp_mib2.h"
    "lwip/lwip/src/include/lwip/apps/mdns_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp.h"
    "lwip/lwip/src/include/lwip/apps/tftp_server.h"
    "lwip/lwip/src/include/lwip/apps/snmp_threadsync.h"
    "lwip/lwip/src/include/lwip/apps/fs.h"
    "lwip/lwip/src/include/lwip/apps/netbiosns_opts.h"
    "lwip/lwip/src/include/lwip/apps/altcp_tls_mbedtls_opts.h"
    "lwip/lwip/src/include/lwip/apps/mdns.h"
    "lwip/lwip/src/include/lwip/apps/smtp_opts.h"
    "lwip/lwip/src/include/lwip/apps/smtp.h"
    "lwip/lwip/src/include/lwip/apps/sntp.h"
    "lwip/lwip/src/include/lwip/apps/mqtt_priv.h"
    "lwip/lwip/src/include/lwip/apps/snmp_table.h"
    "lwip/lwip/src/include/lwip/apps/mqtt.h"
    "lwip/lwip/src/include/lwip/apps/snmp_snmpv2_framework.h"
    "lwip/lwip/src/include/lwip/apps/lwiperf.h"
    "lwip/lwip/src/include/lwip/apps/netbiosns.h"
    "lwip/lwip/src/include/lwip/apps/snmp_core.h"
    "lwip/lwip/src/include/lwip/apps/httpd_opts.h"
    "lwip/lwip/src/include/lwip/apps/sntp_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp_scalar.h"
    "lwip/lwip/src/include/lwip/apps/altcp_proxyconnect.h"
    "lwip/lwip/src/include/lwip/apps/mdns_priv.h"
    "lwip/lwip/src/include/lwip/apps/httpd.h"
    "lwip/lwip/src/include/lwip/apps/tftp_opts.h"
    "lwip/lwip/src/include/lwip/apps/http_client.h"
    "lwip/lwip/src/include/lwip/apps/mqtt_opts.h"
    "lwip/lwip/src/include/lwip/ip4_frag.h"
    "lwip/lwip/src/include/lwip/if_api.h"
    "lwip/lwip/src/include/lwip/nd6.h"
    "lwip/lwip/src/include/lwip/etharp.h"
    "lwip/lwip/src/include/lwip/tcpip.h"
    "lwip/lwip/src/include/lwip/netifapi.h"
    "lwip/lwip/src/include/lwip/inet.h"
    "lwip/lwip/src/include/lwip/memp.h"
    "lwip/lwip/src/include/lwip/debug.h"
    "lwip/lwip/src/include/lwip/sys.h"
    "lwip/lwip/src/include/lwip/netif.h"
    "lwip/lwip/src/include/lwip/prot/autoip.h"
    "lwip/lwip/src/include/lwip/prot/ethernet.h"
    "lwip/lwip/src/include/lwip/prot/udp.h"
    "lwip/lwip/src/include/lwip/prot/ip6.h"
    "lwip/lwip/src/include/lwip/prot/dns.h"
    "lwip/lwip/src/include/lwip/prot/ip.h"
    "lwip/lwip/src/include/lwip/prot/dhcp6.h"
    "lwip/lwip/src/include/lwip/prot/nd6.h"
    "lwip/lwip/src/include/lwip/prot/etharp.h"
    "lwip/lwip/src/include/lwip/prot/iana.h"
    "lwip/lwip/src/include/lwip/prot/tcp.h"
    "lwip/lwip/src/include/lwip/prot/mld6.h"
    "lwip/lwip/src/include/lwip/prot/icmp6.h"
    "lwip/lwip/src/include/lwip/prot/igmp.h"
    "lwip/lwip/src/include/lwip/prot/icmp.h"
    "lwip/lwip/src/include/lwip/prot/dhcp.h"
    "lwip/lwip/src/include/lwip/prot/ip4.h"
    "lwip/lwip/src/include/lwip/prot/ieee.h"
    "lwip/lwip/src/include/lwip/tcp.h"
    "lwip/lwip/src/include/lwip/timeouts.h"
    "lwip/lwip/src/include/lwip/opt.h"
    "lwip/lwip/src/include/lwip/init.h"
    "lwip/lwip/src/include/lwip/altcp_tls.h"
    "lwip/lwip/src/include/lwip/ip6_addr.h"
    "lwip/lwip/src/include/lwip/netbuf.h"
    "lwip/lwip/src/include/lwip/raw.h"
    "lwip/lwip/src/include/lwip/mld6.h"
    "lwip/lwip/src/include/lwip/icmp6.h"
    "lwip/lwip/src/include/lwip/igmp.h"
    "lwip/lwip/src/include/lwip/ip4_addr.h"
    "lwip/lwip/src/include/lwip/icmp.h"
    "lwip/lwip/src/include/lwip/dhcp.h"
    "lwip/lwip/src/include/lwip/ip4.h"
    "lwip/lwip/src/include/lwip/ip_addr.h"
    "lwip/lwip/src/include/lwip/altcp_tcp.h"
    "lwip/lwip/src/include/lwip/lwip_napt.h"
    "lwip/lwip/src/include/lwip/sio.h"
    "lwip/lwip/src/include/lwip/def.h"
    "lwip/lwip/src/include/lwip/pbuf.h"

    "lwip/port/esp32/include/netif/dhcp_state.h"
    "lwip/port/esp32/include/sntp/sntp_get_set_time.h"
    "lwip/port/esp32/include/lwipopts.h"
    "lwip/port/esp32/include/arch/cc.h"
    "lwip/port/esp32/include/arch/sys_arch.h"
    "lwip/port/esp32/include/arch/vfs_lwip.h"

    "lwip/lwip/src/include/lwip/apps/snmp_snmpv2_usm.h"
    "lwip/lwip/src/include/lwip/apps/snmpv3.h"
    "lwip/lwip/src/include/lwip/apps/snmp_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp_mib2.h"
    "lwip/lwip/src/include/lwip/apps/mdns_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp.h"
    "lwip/lwip/src/include/lwip/apps/tftp_server.h"
    "lwip/lwip/src/include/lwip/apps/snmp_threadsync.h"
    "lwip/lwip/src/include/lwip/apps/fs.h"
    "lwip/lwip/src/include/lwip/apps/netbiosns_opts.h"
    "lwip/lwip/src/include/lwip/apps/altcp_tls_mbedtls_opts.h"
    "lwip/lwip/src/include/lwip/apps/mdns.h"
    "lwip/lwip/src/include/lwip/apps/smtp_opts.h"
    "lwip/lwip/src/include/lwip/apps/smtp.h"
    "lwip/lwip/src/include/lwip/apps/sntp.h"
    "lwip/lwip/src/include/lwip/apps/mqtt_priv.h"
    "lwip/lwip/src/include/lwip/apps/snmp_table.h"
    "lwip/lwip/src/include/lwip/apps/mqtt.h"
    "lwip/lwip/src/include/lwip/apps/snmp_snmpv2_framework.h"
    "lwip/lwip/src/include/lwip/apps/lwiperf.h"
    "lwip/lwip/src/include/lwip/apps/netbiosns.h"
    "lwip/lwip/src/include/lwip/apps/snmp_core.h"
    "lwip/lwip/src/include/lwip/apps/httpd_opts.h"
    "lwip/lwip/src/include/lwip/apps/sntp_opts.h"
    "lwip/lwip/src/include/lwip/apps/snmp_scalar.h"
    "lwip/lwip/src/include/lwip/apps/altcp_proxyconnect.h"
    "lwip/lwip/src/include/lwip/apps/mdns_priv.h"
    "lwip/lwip/src/include/lwip/apps/httpd.h"
    "lwip/lwip/src/include/lwip/apps/tftp_opts.h"
    "lwip/lwip/src/include/lwip/apps/http_client.h"
    "lwip/lwip/src/include/lwip/apps/mqtt_opts.h"

    "lwip/include/apps/dhcpserver/dhcpserver.h"

    "esp_netif/include/esp_netif_sta_list.h"
    "esp_netif/lwip/esp_netif_lwip_internal.h"

    "esp_wifi/include/esp_private/esp_wifi_private.h"
    "esp_wifi/include/esp_wifi_crypto_types.h"
    "esp_wifi/include/esp_private/wifi_os_adapter.h"
    # "esp_wifi/include/esp_wifi_default.h"  # do not regenerate - manually edited to compile

    "esp_netif/include/esp_netif_ppp.h"
    "esp_netif/include/esp_netif_slip.h"

    "soc/esp32s3/include/soc/reset_reasons.h"
    "esp_hw_support/include/esp_mac.h"
    "esp_hw_support/include/esp_chip_info.h"
    "esp_hw_support/include/esp_random.h"

    "freertos/include/esp_additions/freertos/FreeRTOSConfig.h"
    "freertos/port/xtensa/include/freertos/FreeRTOSConfig_arch.h"


    "bt/host/nimble/nimble/nimble/host/include/host/ble_uuid.h"
    "bt/host/nimble/nimble/porting/nimble/include/sysinit/sysinit.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs.h"
    "bt/host/nimble/nimble/nimble/include/nimble/hci_common.h"
    "bt/host/nimble/nimble/nimble/include/nimble/nimble_npl.h"
    # "bt/host/nimble/nimble/porting/npl/freertos/include/nimble/nimble_npl_os.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/endian.h"
    "bt/host/nimble/port/include/console/console.h"
    "bt/host/nimble/port/include/esp_nimble_mem.h"
    "bt/host/nimble/esp-hci/include/esp_nimble_hci.h"
    "bt/host/nimble/nimble/nimble/include/nimble/ble.h"
    "bt/host/nimble/nimble/porting/nimble/include/nimble/nimble_port.h"
    "bt/host/nimble/nimble/nimble/include/nimble/nimble_npl.h"
    "bt/host/nimble/nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h"
    "bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
    "bt/host/nimble/nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_store.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_hci.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_sm.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_l2cap.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_adv.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_pvcy.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_att.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_ibeacon.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_eddystone.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_uuid.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_monitor.h"
    # "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_log.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_id.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_stop.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_gap.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_mbuf.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_gatt.h"
    "bt/host/nimble/nimble/porting/nimble/include/syscfg/syscfg.h"
    "bt/host/nimble/port/include/esp_nimble_cfg.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os_trace_api.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os_mbuf.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os_cputime.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os_error.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/endian.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os_mempool.h"
    "bt/host/nimble/nimble/porting/nimble/include/os/os.h"
    # "bt/host/nimble/nimble/porting/nimble/include/os/queue.h"
    "bt/host/nimble/nimble/porting/npl/freertos/include/nimble/npl_freertos.h"
    "bt/host/nimble/nimble/porting/nimble/include/modlog/modlog.h"
    # "bt/host/nimble/nimble/porting/nimble/include/log/log.h"
    "bt/host/nimble/nimble/porting/nimble/include/log_common/log_common.h"
    "bt/host/nimble/nimble/porting/nimble/include/log_common/ignore.h"
    "bt/host/nimble/nimble/nimble/include/nimble/nimble_opt.h"
    "bt/host/nimble/nimble/nimble/include/nimble/nimble_opt_auto.h"
    "bt/host/nimble/nimble/nimble/include/nimble/ble_hci_trans.h"

    "mqtt/esp-mqtt/include/mqtt_client.h"
    # "lwip/include/apps/esp_sntp.h"  # modified manually to compile
    "esp_https_ota/include/esp_https_ota.h"
    "esp_http_client/include/esp_http_client.h"
    "nghttp/port/include/http_parser.h"
    # "bootloader_support/include/bootloader_common.h" # do not regenerate - manually edited to compile
    # "bootloader_support/include/esp_app_format.h" # do not regenerate - manually edited to compile

    # "heap/include/esp_heap_caps.h" # do not regenerate - manually edited to compile

    "tcp_transport/include/esp_transport.h"
    # "tcp_transport/include/esp_transport_tcp.h" do not regenerate - manually edited to compile
    "tcp_transport/include/esp_transport_ssl.h"
    "esp-tls/esp_tls.h"
    "esp-tls/esp_tls_errors.h"
    # "freertos/port/xtensa/include/freertos/portmacro.h" # do not regenerate - manually edited to compile
    "freertos/port/xtensa/include/freertos/portbenchmark.h"
)

# Declare an array variable
declare -a HEADERS_TO_COPY_AWS_PORT=(\
    # clock_esp.c - no header file?
    "posix-compat/semaphore.h"
    "ota/aws_esp_ota_ops.h"
    "ota/ota_pal.h"
    "crypto/iot_crypto.h"

    # not adding logging_levels.h and logging_stack.h, probably they shouldn't be here?
    # "network_transport/tls_freertos.h" # do not regenerate - manually edited to compile


    #ota_os_freertos.h to be added from aws-sdk
    #core_pkcs11_pal.h to be added from aws-sdk
)

# Declare an array variable
declare -a HEADERS_TO_COPY_AWS_SDK=(\
    #Files added based on the list from CMake files

    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_cbor_private.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_config_defaults.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_http_interface.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_interface_private.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_mqtt_interface.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_mqtt_private.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_os_interface.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_platform_interface.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_private.h"
    "aws/ota-for-aws-iot-embedded-sdk/source/portable/os/ota_os_freertos.h"

    "standard/backoffAlgorithm/source/include/backoff_algorithm.h"

    "standard/coreJSON/source/include/core_json.h"

    "standard/coreMQTT/source/include/core_mqtt.h"
    "standard/coreMQTT/source/include/core_mqtt_config_defaults.h"
    "standard/coreMQTT/source/include/core_mqtt_state.h"
    "standard/coreMQTT/source/include/core_mqtt_serializer.h"
    "standard/coreMQTT/source/interface/transport_interface.h"

    "standard/corePKCS11/source/include/core_pkcs11_pal.h"
    "standard/corePKCS11/source/include/core_pkcs11.h"
    "standard/corePKCS11/source/include/core_pki_utils.h"
    "standard/corePKCS11/source/dependency/3rdparty/pkcs11/pkcs11t.h"


    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota_appversion32.h"
)

# Declare an array variable
declare -a MOCKS_TO_PROCESS=(\
    "log/include/esp_log.h"
    "nvs_flash/include/nvs_flash.h"
    # "esp_common/include/esp_err.h"  # do not regenerate - manually edited
    "driver/include/driver/gpio.h"
    "driver/include/driver/ledc.h"
    "esp_system/include/esp_system.h"
    "nvs_flash/include/nvs.h"
    # "freertos/include/freertos/semphr.h"  # do not regenerate - manually edited to compile
    # "freertos/include/freertos/queue.h"  # do not regenerate - manually edited to compile
    # "freertos/include/freertos/task.h"  # do not regenerate - manually edited to comWIFI_EVENTpile
    # "freertos/include/freertos/event_groups.h"  # do not regenerate - manually edited to compile
    # "freertos/include/freertos/timers.h"  # do not regenerate - manually edited to compile
    "esp_event/include/esp_event.h"
    # "esp_wifi/include/esp_wifi_default.h"  # do not regenerate - manually edited to compile
    # "esp_netif/include/esp_netif.h"  # do not regenerate - manually edited to compile
    # "esp_wifi/include/esp_wifi.h"  # do not regenerate - manually edited to compile
    "esp_hw_support/include/esp_mac.h"

    # "bt/host/nimble/nimble/nimble/host/include/host/ble_gatt.h"
    # "bt/host/nimble/nimble/nimble/host/include/host/ble_gap.h"
    "bt/host/nimble/nimble/porting/nimble/include/nimble/nimble_port.h"
    "bt/host/nimble/nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_id.h"
    "bt/host/nimble/nimble/nimble/host/include/host/ble_store.h"
    "bt/host/nimble/nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
    "bt/host/nimble/esp-hci/include/esp_nimble_hci.h"
    # "bt/host/nimble/nimble/nimble/host/include/host/ble_hs_mbuf.h"

    "mqtt/esp-mqtt/include/mqtt_client.h"
    # "lwip/lwip/src/include/lwip/apps/sntp.h" # modified manually to compile
    "esp_https_ota/include/esp_https_ota.h"

    #"heap/include/esp_heap_caps.h" # do not regenerate - modified manually to compile

    # Mocks from AWS_PORT_DIR

    "crypto/iot_crypto.h"
    "network_transport/tls_freertos.h"
    "ota/aws_esp_ota_ops.h"
    "ota/ota_pal.h"
    "posix-compat/semaphore.h"

    # Mocks from AWS_SDK_DIR

    "aws/ota-for-aws-iot-embedded-sdk/source/include/ota.h"

    # "aws/ota-for-aws-iot-embedded-sdk/source/portable/os/ota_os_freertos.h" # Do not regenerate - manually edited to compile
    "standard/coreMQTT/source/include/core_mqtt.h"

    #"esp_rom/include/esp32s3/rom/gpio.h" # Do not regenerate - manually edited to compile

)


for INPUT_FILE in "${HEADERS_TO_COPY_AWS_PORT[@]}"; do
    OUTPUT_DIR=$(dirname "${HEADER_FOR_MOCKS_DIR}/${INPUT_FILE}")
    HEADER_FILE_PATH=$(realpath "${AWS_PORT_DIR}/${INPUT_FILE}")

    mkdir -p "${OUTPUT_DIR}"
    cp ${HEADER_FILE_PATH} ${OUTPUT_DIR}/
done

for INPUT_FILE in "${HEADERS_TO_COPY_AWS_SDK[@]}"; do
    OUTPUT_DIR=$(dirname "${HEADER_FOR_MOCKS_DIR}/${INPUT_FILE}")
    HEADER_FILE_PATH=$(realpath "${AWS_SDK_DIR}/${INPUT_FILE}")

    mkdir -p "${OUTPUT_DIR}"
    cp ${HEADER_FILE_PATH} ${OUTPUT_DIR}/
done

for INPUT_FILE in "${HEADERS_TO_COPY[@]}"; do
    OUTPUT_DIR=$(dirname "${HEADER_FOR_MOCKS_DIR}/${INPUT_FILE}")
    HEADER_FILE_PATH=$(realpath "${ESP32_COMPONENTS_DIR}/${INPUT_FILE}")

    mkdir -p "${OUTPUT_DIR}"
    cp ${HEADER_FILE_PATH} ${OUTPUT_DIR}/
done


# Process all to be mocked files
for INPUT_FILE in "${MOCKS_TO_PROCESS[@]}"; do
    # Create a mock
    HEADER_FILE_PATH=$(realpath "${HEADER_FOR_MOCKS_DIR}/${INPUT_FILE}")

    pushd ${SCRIPT_DIR}
    ${SCRIPT_DIR}/generate_mock.sh -i "${HEADER_FILE_PATH}"

    OUTPUT_DIR=$(dirname "${OUTPUT_COMPONENTS_DIR}/${INPUT_FILE}")

    mkdir -p "${OUTPUT_DIR}"
    # ... and move the produced file to the output directory
    pushd mocks

    for FILE_NAME in *.h
    do
        printf "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n" | cat - "${FILE_NAME}" > tmpfile && mv tmpfile ${FILE_NAME} && printf "\n\n#ifdef __cplusplus\n}\n#endif\n" >> ${FILE_NAME}
    done


    mv *.{c,h} ${OUTPUT_DIR}
    popd
    rm -r mocks

    popd
done

echo "DONE!"
