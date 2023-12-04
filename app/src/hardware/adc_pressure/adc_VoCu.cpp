// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "adcVoCu";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "adc_VoCu.h"
#include "sleep.h"
#include "commons.h"
#include "esp_adc_cal.h"
#include "math.h"

namespace
{
    constexpr adc1_channel_t SENSOR_CHANNEL = ADC1_CHANNEL_4; /*!< ADC1 channel 4 is GPIO32 */
    esp_adc_cal_characteristics_t adcChars = {};

    // Current sensor
    const float FlowMeasurementOffset = 0.2;
    float TotalSumOfLiters = 0;
    uint32_t resolution = 185; // mV/A
    uint32_t thresholdStartReading = 0;
    uint32_t zeroVoltageShiftInmV = 725;

    uint64_t lastTime;
    uint64_t currentTime;
    uint64_t IEndMeasure;
    uint64_t TimeBetweenTwoMeasurements;

    bool firstCall = true;
}

void adcInit(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SENSOR_CHANNEL, ADC_ATTEN_DB_11);
    esp_adc_cal_value_t val_type =
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adcChars);

    // Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        LOG_INFO("Available ADC calibration: eFuse Vref");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        LOG_INFO("Available ADC calibration: Two Point");
    }
    else
    {
        LOG_INFO("Available ADC calibration: Default");
    }
}

static float calculate_current_from_voltage(uint32_t voltageInmV)
{
    
    const float current = (float)(voltageInmV) / resolution;

    return (float)(current);
}

void ExecuteUpdateTotalSumOfLiters(void)
{


    while(firstCall == true)
        {

            TimeBetweenTwoMeasurements = 0;
            lastTime = 0;
            currentTime = 0;
            lastTime = commons::getCurrentTimestampUs();
            firstCall = false;
            LOG_INFO("Inside firstCall while loop");

        }

    currentTime = commons::getCurrentTimestampUs();

    //LOG_INFO("currentTime: %lu", currentTime);
    //LOG_INFO("lastTime: %lu", lastTime);

    TimeBetweenTwoMeasurements = currentTime - lastTime;

    //LOG_INFO("TimeBetweenTwoMeasurements: %lu seconds", TimeBetweenTwoMeasurements);

    lastTime = currentTime;

    currentTime = 0;
    


    //LOG_INFO("Raw ADC reading: %lu", adcReading);

    float SumCurrentValue2 = 0;
    float INoSamples = 0;
    const float gulSamplePeriod_us = 2 * 20000; //sample over 2 period (40ms)

    IEndMeasure = commons::getCurrentTimestampUs() + gulSamplePeriod_us;

    while(commons::getCurrentTimestampUs() < IEndMeasure)
    {
        const uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(SENSOR_CHANNEL));
        const uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars);
        const float CurrentValue = calculate_current_from_voltage(voltage);
        float CurrentValue2 = CurrentValue * CurrentValue;
        SumCurrentValue2 = SumCurrentValue2 + CurrentValue2;
        INoSamples++;
    }


    //const uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars);

    //LOG_INFO("ADC voltage: %lu mV", voltage);

    const float CurrentRMS = sqrt(SumCurrentValue2 / INoSamples);
    
    LOG_INFO("INoSamples: %.6f samples", INoSamples);
    LOG_INFO("Current RMS: %.6f A", CurrentRMS);

        

}


float getTotalSumOfLiters(void)
{
    return TotalSumOfLiters;
}