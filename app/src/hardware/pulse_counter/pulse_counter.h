#ifndef PULSE_COUNTER_H
#define PULSE_COUNTER_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/pcnt.h"

class PulseCounterHandler
{
public:
    PulseCounterHandler();
    ~PulseCounterHandler() = default;

    void runTask();

    uint32_t getPulseCounterValue() const;

#if !TESTING
private:
#endif
    static void run(void* pObject);
    void        _run();

    void initiatePulseCounter(pcnt_unit_t unit);

    TaskHandle_t m_taskHandle; // handle to runTask
};

#endif // PULSE_COUNTER_H