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

    float getTotalFlowInLitres() const;

    float getFlowMeterCalibrationValue() const;

    void setFlowMeterCalibrationValue(float flowMeterCalibration);

#if !TESTING
private:
#endif
    static void run(void* pObject);
    void        _run();

    void initiatePulseCounter(pcnt_unit_t unit);

    TaskHandle_t m_taskHandle; // handle to runTask

    uint32_t m_counterPulses;

    float m_totalLitres;

    float m_flowMeterCalibrationValue;

    uint32_t m_multPulses;
};

#endif // PULSE_COUNTER_H
