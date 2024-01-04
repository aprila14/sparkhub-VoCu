#ifndef ADC_VOCU_H
#define ADC_VOCU_H

#include "defines.h"

void adcInit(void);
float getRMSCurrent(void);
float* get3RMSCurrentDataPoints(void);
float SumOfSparklingWater(void);
float TimeCoolingIsRunning(void);

#endif // ADC_VOCU_H