/*
Common non hardware related defines
*/

#ifndef DEFINES_H
#define DEFINES_H


#ifdef __cplusplus
extern "C"
{
#endif


#include "esp_log.h"


#include <assert.h>
#include <stdint.h>
#include <string.h>

#ifdef IS_DEBUG_BUILD
    #define DEBUG_ASSERT(x) do { if (!(x)) { \
        LOG_ERROR("Assertion failed. Function %s, file %s, line %d.\n", __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        assert(0); } } while(0)
#else
    #define DEBUG_ASSERT(x) do {} while(0)
#endif


#if TESTING
    #include <cstdio>
    #define LOG_DEBUG(...)       do {} while(0)
    #define LOG_INFO(...)        do { printf(__VA_ARGS__); printf("\n"); } while(0)
    #define LOG_WARNING(...)     do { printf(__VA_ARGS__); printf("\n"); } while(0)
    #define LOG_ERROR(...)       do { printf(__VA_ARGS__); printf("\n"); } while(0)
#else
    #ifdef ENABLE_PRINTS_AND_LOGS
        #define LOG_DEBUG(...)       ESP_LOGD(LOG_TAG, __VA_ARGS__)
        #define LOG_INFO(...)        ESP_LOGI(LOG_TAG, __VA_ARGS__)
        #define LOG_WARNING(...)     ESP_LOGW(LOG_TAG, __VA_ARGS__)
        #define LOG_ERROR(...)       ESP_LOGE(LOG_TAG, __VA_ARGS__)
    #else
        #define LOG_DEBUG(...)       do {} while(0)
        #define LOG_INFO(...)        do {} while(0)
        #define LOG_WARNING(...)     ESP_LOGW(LOG_TAG, __VA_ARGS__)
        #define LOG_ERROR(...)       ESP_LOGE(LOG_TAG, __VA_ARGS__)
    #endif
#endif


#define SMALL_STACK_SIZE 1800
#define DEFAULT_STACK_SIZE 2048
#define DEFAULT_SLIGHTLY_BIGGER_STACK_SIZE 2304
#define DEFAULT_BIGGER_STACK_SIZE 2560
#define DEFAULT_MUCH_BIGGER_STACK_SIZE 2816
#define DEFAULT_HUGE_STACK_SIZE 1024*6
#define LOW_TASK_PRIORITY 2
#define DEFAULT_TASK_PRIORITY 5
#define HIGH_TASK_PRIORITY 20  // (configMAX_PRIORITIES - 5)
#define VERY_HIGH_TASK_PRIORITY 22 //(configMAX_PRIORITIES - 3)



/********** CONSTANTS START **********/

#ifdef __cplusplus

constexpr uint32_t MS_IN_S = 1000;  // milliseconds in second
constexpr uint32_t US_IN_MS = 1000;  // milliseconds in second
constexpr uint32_t SEC_IN_MIN = 60;  // seconds in minute
constexpr uint32_t MIN_IN_HOUR = 60;  // minutes in hour
constexpr uint32_t HOURS_IN_DAY = 24;  // hours in day
constexpr uint32_t MINUTES_IN_WEEK = MIN_IN_HOUR * HOURS_IN_DAY * 7;

constexpr uint16_t MAX_BLE_PACKET_SIZE = 8192;

constexpr int32_t NO_TIMEOUT_VALUE = -1;  // Value to denote that the timeout cannot occue (infinite timeout)
#endif  // __cplusplus


#define UNUSED(x) (void)(x)

#ifdef __cplusplus
}
#endif

#endif // DEFINES_H
