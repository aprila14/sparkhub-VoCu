#ifndef LIGHT_SCHEDULER_H
#define LIGHT_SCHEDULER_H


#include "defines.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


/**
 * @brief The LightSchedule class will be responsible for handling the business logic,
 * with cyclic light change due to time passed or sensors interaction - without the user interaction
 */
class LightScheduler
{
public:
    LightScheduler() = default;

    bool init() const;
    void runTask();
    void run();


#if !TESTING
private:
#endif
    void perform();

    TaskHandle_t m_taskHandle = nullptr;
};

#endif // LIGHT_SCHEDULER_H
