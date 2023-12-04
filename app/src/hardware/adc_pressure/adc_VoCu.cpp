// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "adcVoCu";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "adc_VoCu.h"
#include "sleep.h"
#include "commons.h"
#include "esp_adc_cal.h"

namespace
{
    constexpr adc1_channel_t SENSOR_CHANNEL = ADC1_CHANNEL_4; /*!< ADC1 channel 4 is GPIO32 */
    esp_adc_cal_characteristics_t adcChars = {};

    // Current sensor
    const float FlowMeasurementOffset = 0.2;
    float TotalSumOfLiters = 0;
    uint32_t resolution = 185; // mV/A
    uint32_t thresholdStartReading = 0;

    uint32_t lastTime;
    uint32_t currentTime;
    uint32_t TimeBetweenTwoMeasurements;

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

static float calculate_current_from_voltage(uint32_t voltageInMillivolts)
{
    const float voltage = (float)(voltageInMillivolts / resolution);

    return (float)(voltage);
}

void ExecuteUpdateTotalSumOfLiters(void)
{


    float Liters;

    while(firstCall == true)
        {

            TimeBetweenTwoMeasurements = 0;
            lastTime = 0;
            currentTime = 0;
            lastTime = commons::getCurrentTimestampMs();
            firstCall = false;

        }

    currentTime = commons::getCurrentTimestampMs();

    LOG_INFO("currentTime: %lu", currentTime);
    LOG_INFO("lastTime: %lu", lastTime);

    TimeBetweenTwoMeasurements = currentTime - lastTime;

    LOG_INFO("TimeBetweenTwoMeasurements: %lu", TimeBetweenTwoMeasurements);

    lastTime = currentTime;

    currentTime = 0;
    
    const uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(SENSOR_CHANNEL));

    LOG_INFO("Raw ADC reading: %lu", adcReading);

    if(adcReading > thresholdStartReading)
        {   
            const uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars);

            LOG_INFO("ADC voltage: %lu", voltage);

            const float FlowValue = calculate_current_from_voltage(voltage);

            LOG_INFO("Flow value: %.6f", FlowValue);

            Liters = FlowValue * TimeBetweenTwoMeasurements;

            LOG_INFO("Liters value: %.6f", Liters);

            TotalSumOfLiters = TotalSumOfLiters + Liters;        
                
            LOG_INFO("TotalSumOfLiters: %.6f", TotalSumOfLiters);
        }

}


float getTotalSumOfLiters(void)
{
    return TotalSumOfLiters;
}