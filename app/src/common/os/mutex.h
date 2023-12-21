#ifndef MUTEX_H
#define MUTEX_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"


typedef SemaphoreHandle_t mutex_t;


inline void lockGuardTake(SemaphoreHandle_t pHandle)
{
    while (xSemaphoreTake(pHandle, portMAX_DELAY) != pdTRUE)
    {
    }
}

inline void lockGuardRelease(SemaphoreHandle_t* pHandle)
{
    // while (xSemaphoreGive(*pHandle) != pdTRUE) { }
    xSemaphoreGive(*pHandle); // we want to ignore cases where semaphore was not taken before calling release
}

inline SemaphoreHandle_t mutexCreate()
{
    return xSemaphoreCreateMutex();
}

#if !TESTING
// gcc extension to have RAII. In case of other comppiler - cannot be used
#ifdef __GNUC__
#define RAII(free_func) __attribute__((cleanup(free_func)))
#define LOCK_GUARD(mutex_, guardName_) \
    RAII(lockGuardRelease) SemaphoreHandle_t guardName_ = mutex_; \
    lockGuardTake(guardName_)
#else
#error "Compiler not supported - mutex.h!"
#endif //__GNUC__
#else
#define LOCK_GUARD(mutex_, guardName_) \
    do \
    { \
    } while (0)
#endif // TESTING

/*
Mutex usage:
 - define your mutex variable anywhere:
    mutex_t myMutex = NULL;
 - Create it in a task:
    myMutex = mutexCreate();
 - In any function that requires the mutex - use lock_guard (almost like in c++)
    LOCK_GUARD(myMutex, myMutexGuard);
 - No need to unlock it later - it unlock when it gets out of the scope

NOTE:
 As it is a scoped variable - cannot be used directly after `if` as one liner (and it wouldn't make much sense anyway)
*/


typedef SemaphoreHandle_t semaphore_t;


inline SemaphoreHandle_t semaphoreCreateBinary()
{
    return xSemaphoreCreateBinary();
}

inline SemaphoreHandle_t semaphoreCreateCounting()
{
    return xSemaphoreCreateCounting(INT32_MAX - 1, 0);
}

inline void seamphoreTake(SemaphoreHandle_t pHandle)
{
    while (xSemaphoreTake(pHandle, portMAX_DELAY) != pdTRUE)
    {
    }
}

inline void semaphoreGive(SemaphoreHandle_t pHandle)
{
    xSemaphoreGive(pHandle);
}

inline void semaphoreBinaryReset(SemaphoreHandle_t pHandle)
{
    xSemaphoreTake(pHandle, 0);
}

inline bool isSemaphoreGiven(SemaphoreHandle_t pHandle)
{
    return (uxSemaphoreGetCount(pHandle) > 0);
}

#endif // MUTEX_H
