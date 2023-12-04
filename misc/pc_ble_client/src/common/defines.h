/**
 * @file project.h
 * @brief General project defines and includes
 */


#ifndef PROJECT_H_
#define PROJECT_H_


#define UNUSED(x) (void)(x)

#ifdef __cplusplus
extern "C"
{
#endif
#include "blzlib_log.h"
#ifdef __cplusplus
}
#endif


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define LOG_DEBUG(...)       blz_log_out(LL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...)        blz_log_out(LL_INFO, __VA_ARGS__)
#define LOG_WARNING(...)     blz_log_out(LL_WARN, __VA_ARGS__)
#define LOG_ERROR(...)       blz_log_out(LL_ERR, __VA_ARGS__)



/**
 * @brief LOG levels used on ESP. Added here only to be defined for compilation, not used on PC side
 */
typedef enum
{
    ESP_LOG_NONE,       /*!< No log output */
    ESP_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    ESP_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    ESP_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    ESP_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    ESP_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} esp_log_level_t;


#ifdef IS_DEBUG_BUILD
    #define DEBUG_ASSERT(x) do { if (!(x)) { \
        LOG_ERROR("Assertion failed. Function %s, file %s, line %d.\n", __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        while (1) { } } } while(0)
#else
    #define DEBUG_ASSERT(x) do {} while(0)
#endif


constexpr uint32_t MS_IN_S = 1000;  // milliseconds in second
constexpr uint32_t US_IN_MS = 1000;  // milliseconds in second
constexpr uint32_t SEC_IN_MIN = 60;  // seconds in minute
constexpr uint32_t MIN_IN_HOUR = 60;  // minutes in hour
constexpr uint32_t HOURS_IN_DAY = 24;  // hours in day
constexpr uint32_t MINUTES_IN_WEEK = MIN_IN_HOUR * HOURS_IN_DAY * 7;

constexpr uint16_t MAX_BLE_PACKET_SIZE = 8192;


#define UNUSED(x) (void)(x)


#endif /* PROJECT_H_ */
