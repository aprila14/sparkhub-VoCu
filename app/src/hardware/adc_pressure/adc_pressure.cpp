// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "adcPressure";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "adc_pressure.h"

#include "esp_adc_cal.h"

namespace
{
    constexpr adc1_channel_t PRESSURE_SENSOR_CHANNEL = ADC1_CHANNEL_4; /*!< ADC1 channel 4 is GPIO32 */
}

void adcInit(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(PRESSURE_SENSOR_CHANNEL, ADC_ATTEN_DB_11);
}

float getPressureSensorValue(void)
{
    uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(PRESSURE_SENSOR_CHANNEL));

    LOG_INFO("Raw ADC reading: %d", adcReading);

    return 0;
}