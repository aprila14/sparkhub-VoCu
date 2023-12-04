/**
 * @file misc.h
 * @brief Misceloneous project functionality
 */


#ifndef MISC_H
#define MISC_H


#include "blzlib_log.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


/**
 * @brief Handles logs from blzlib library
 * @param logLevel Level of the log message
 * @param fmt Format of the message
 * @param ap
 */
void blzlibLogHandler(enum loglevel logLevel, const char* fmt, va_list ap);



// Code snippet below copied from externals/esp-idf/components/esp_netif/include/esp_netif_ip_addr.h
//#define esp_netif_htonl(x) ((uint32_t)(x))
#define esp_netif_htonl(x) ((((x) & (uint32_t)0x000000ffU) << 24U) | \
                      (((x) & (uint32_t)0x0000ff00U) <<  8U) | \
                      (((x) & (uint32_t)0x00ff0000U) >>  8U) | \
                      (((x) & (uint32_t)0xff000000U) >> 24U))

#define ESP_IP6_ADDR_BLOCK1(addr) ((uint16_t)((esp_netif_htonl(addr[0]) >> 16) & 0xffff))
#define ESP_IP6_ADDR_BLOCK2(addr) ((uint16_t)((esp_netif_htonl(addr[0])) & 0xffff))
#define ESP_IP6_ADDR_BLOCK3(addr) ((uint16_t)((esp_netif_htonl(addr[1]) >> 16) & 0xffff))
#define ESP_IP6_ADDR_BLOCK4(addr) ((uint16_t)((esp_netif_htonl(addr[1])) & 0xffff))
#define ESP_IP6_ADDR_BLOCK5(addr) ((uint16_t)((esp_netif_htonl(addr[2]) >> 16) & 0xffff))
#define ESP_IP6_ADDR_BLOCK6(addr) ((uint16_t)((esp_netif_htonl(addr[2])) & 0xffff))
#define ESP_IP6_ADDR_BLOCK7(addr) ((uint16_t)((esp_netif_htonl(addr[3]) >> 16) & 0xffff))
#define ESP_IP6_ADDR_BLOCK8(addr) ((uint16_t)((esp_netif_htonl(addr[3])) & 0xffff))

#define IPV6STR "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"

#define IPV62STR(addr) ESP_IP6_ADDR_BLOCK1(addr),     \
    ESP_IP6_ADDR_BLOCK2(addr),     \
    ESP_IP6_ADDR_BLOCK3(addr),     \
    ESP_IP6_ADDR_BLOCK4(addr),     \
    ESP_IP6_ADDR_BLOCK5(addr),     \
    ESP_IP6_ADDR_BLOCK6(addr),     \
    ESP_IP6_ADDR_BLOCK7(addr),     \
    ESP_IP6_ADDR_BLOCK8(addr)


#define IPSTR "%d.%d.%d.%d"
#define esp_ip4_addr_get_byte(ipaddr, idx) (((const uint8_t*)(&(ipaddr)))[idx])
#define esp_ip4_addr1(ipaddr) esp_ip4_addr_get_byte(ipaddr, 0)
#define esp_ip4_addr2(ipaddr) esp_ip4_addr_get_byte(ipaddr, 1)
#define esp_ip4_addr3(ipaddr) esp_ip4_addr_get_byte(ipaddr, 2)
#define esp_ip4_addr4(ipaddr) esp_ip4_addr_get_byte(ipaddr, 3)


#define esp_ip4_addr1_16(ipaddr) ((uint16_t)esp_ip4_addr1(ipaddr))
#define esp_ip4_addr2_16(ipaddr) ((uint16_t)esp_ip4_addr2(ipaddr))
#define esp_ip4_addr3_16(ipaddr) ((uint16_t)esp_ip4_addr3(ipaddr))
#define esp_ip4_addr4_16(ipaddr) ((uint16_t)esp_ip4_addr4(ipaddr))

#define IP2STR(ipaddr) esp_ip4_addr1_16(ipaddr), \
    esp_ip4_addr2_16(ipaddr), \
    esp_ip4_addr3_16(ipaddr), \
    esp_ip4_addr4_16(ipaddr)

#endif // MISC_H
