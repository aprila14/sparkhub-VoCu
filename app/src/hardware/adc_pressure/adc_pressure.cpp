// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "adcPressure";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "adc_pressure.h"
#include "sleep.h"

#include "esp_adc_cal.h"

namespace
{
constexpr adc1_channel_t      PRESSURE_SENSOR_CHANNEL = ADC1_CHANNEL_4; /*!< ADC1 channel 4 is GPIO32 */
esp_adc_cal_characteristics_t adcChars                = {};

// Pressure sensor
const float pressureMeasurementOffset = 0.2;
} // namespace

void adcInit(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(PRESSURE_SENSOR_CHANNEL, ADC_ATTEN_DB_11);
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

static float calculate_pressure_from_voltage(uint32_t voltageInMillivolts)
{
    const float voltage = (float)(voltageInMillivolts / 1000.0);

    return (float)(voltage - pressureMeasurementOffset);
}

float getAvgPressureSensorValue(void)
{

    float avgPressureValue = 0;
    float sumPressureValue = 0;
    int numberOfReadValue = 0;

    for(int i=1; i<101; i++)
    {
        const uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(PRESSURE_SENSOR_CHANNEL));

        //LOG_INFO("Raw ADC reading: %lu", adcReading);

        const uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars);

        //LOG_INFO("ADC voltage: %lu", voltage);

        const float pressureValue = calculate_pressure_from_voltage(voltage);

        //LOG_INFO("Pressure value: %.6f", pressureValue);

        sumPressureValue = pressureValue + sumPressureValue;
        numberOfReadValue = i;

        //LOG_INFO("Number of Read Pressure Values: %d", numberOfReadValue);

        //LOG_INFO("Sum of Pressure Values: %.6f", sumPressureValue);

        SLEEP_MS(100);

    }

    //LOG_INFO("Number of Read Pressure Values: %d", numberOfReadValue);

    avgPressureValue = sumPressureValue / numberOfReadValue;

    //LOG_INFO("Average pressure Value: %.6f", avgPressureValue);

    return avgPressureValue;
}