#ifndef ADC_PRESSURE_H
#define ADC_PRESSURE_H

#include "defines.h"

void adcInit(void);
uint32_t getPressureSensorValue(void);

#endif // ADC_PRESSURE_H