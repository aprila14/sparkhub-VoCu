#ifndef ADC_PRESSURE_H
#define ADC_PRESSURE_H

#include "defines.h"


void adcInitLevelSense(void);
//LevelSensor
float getAvgPressureSensorValue(void);

#endif // ADC_PRESSURE_H