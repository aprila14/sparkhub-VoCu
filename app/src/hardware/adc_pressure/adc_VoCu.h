#ifndef ADC_VOCU_H
#define ADC_VOCU_H

#include "defines.h"

void adcInit(void);
float getTotalSumOfLiters(void);
float ExecuteUpdateTotalSumOfLiters(void);
float* getAvgCurrent(void);
void SumOfSparklingWater(void);

#endif // ADC_VOCU_H