#ifndef SLEEP_H
#define SLEEP_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MS_TO_TICKS(X_MS) ((X_MS) / static_cast<TickType_t>(portTICK_PERIOD_MS))

// needs to be multiple of 10 ms, otherwise it will be rounded down!
#if !TESTING
#define SLEEP_MS(X_MS) vTaskDelay(MS_TO_TICKS((((X_MS) >= 10) ? (X_MS) : 10)))
#else
#define SLEEP_MS(X_MS)
#endif
#ifdef __cplusplus
}
#endif

#endif  // SLEEP_H
