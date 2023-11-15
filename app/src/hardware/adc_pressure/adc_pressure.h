#ifndef ADC_PRESSURE_H
#define ADC_PRESSURE_H

#include "defines.h"

void adcInit(void);
float getAvgPressureSensorValue(void);

#endif // ADC_PRESSURE_H