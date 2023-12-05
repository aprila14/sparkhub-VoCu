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
    uint32_t resolution = 68.027211; //125.8503401; // mV/A
    uint32_t thresholdStartReading = 0;

    uint64_t lastTime;
    uint64_t currentTime;
    uint64_t IEndMeasure;
    uint64_t IStartMeasure;
    uint64_t TimeBetweenTwoMeasurements;

    bool firstCall = true;
    float gIOffset = 0;
    float gVOffset = 0;
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
            gIOffset = 0;    
            gVOffset = 0;                  // initial current zero of the esp32 input from ACS712

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
    float SumCurrentValue = 0;

    float SumVoltageValue2 = 0;
    float SumVoltageValue = 0;

    float INoSamples = 0;
    const float gulSamplePeriod_us = 5 * 20 * 1000; //sample over 4 period (20ms pro Periode bei 50 Hz)

    IEndMeasure = commons::getCurrentTimestampUs() + gulSamplePeriod_us;
    IStartMeasure = commons::getCurrentTimestampUs();

    while(commons::getCurrentTimestampUs() < IEndMeasure)
    {
        const uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(SENSOR_CHANNEL));
        //LOG_INFO("adcReading: %zu ", adcReading);
        const float voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars) - gVOffset;
        //LOG_INFO("esp_adc_cal_raw_to_voltage(adcReading, &adcChars): %lu mV", esp_adc_cal_raw_to_voltage(adcReading, &adcChars));
        //LOG_INFO("gVOffset: %.6f V", gVOffset);
        //LOG_INFO("voltage: %.6f V", voltage);

        const float voltage2 = voltage * voltage;

        SumVoltageValue = SumVoltageValue + voltage;
        SumVoltageValue2 = SumVoltageValue2 + voltage2;

        const float CurrentValue = calculate_current_from_voltage(voltage) - gIOffset;
        const float CurrentValue2 = CurrentValue * CurrentValue;

        SumCurrentValue = SumCurrentValue + CurrentValue;
        SumCurrentValue2 = SumCurrentValue2 + CurrentValue2;
        INoSamples++;
    }

    IEndMeasure = commons::getCurrentTimestampUs();

    uint64_t timeformeasurement_us = IEndMeasure - IStartMeasure;
    //LOG_INFO("timeformeasurement_us: %jd ms", timeformeasurement_us/1000);
    //LOG_INFO("INoSamples: %.6f", INoSamples);



    //const uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars);

    //LOG_INFO("ADC voltage: %lu mV", voltage);

    float VOffset = SumVoltageValue / INoSamples;

    float IOffset = SumCurrentValue / INoSamples;
    
    // correct quadradic current sum for offset: Sum((i(t)+o)^2) = Sum(i(t)^2) + 2*o*Sum(i(t)) + o^2*NoSamples

    const float CurrentRMS = sqrt(SumCurrentValue2 / INoSamples);

    float SumCurrentRMSCorrected2 = SumCurrentValue2 - 2*IOffset*SumCurrentValue - IOffset*IOffset*INoSamples;




    const float VoltageRMS = sqrt(SumVoltageValue2 / INoSamples);

    float SumVoltageRMSCorrected2 = SumVoltageValue2 - 2*VOffset*SumVoltageValue - VOffset*VOffset*INoSamples;


    //LOG_INFO("SumVoltageRMSCorrected2: %.6f V", SumVoltageRMSCorrected2);
    //LOG_INFO("SumVoltageValue: %.6f mV", SumVoltageValue);
    //LOG_INFO("2*VOffset*SumVoltageValue: %.6f V", 2*VOffset*SumVoltageValue);
    //LOG_INFO("VOffset*VOffset*INoSamples: %.6f V", VOffset*VOffset*INoSamples);


    // avoid NaN due to round-off effects
    if(SumVoltageRMSCorrected2<0)
    {
        SumVoltageRMSCorrected2 = 0;
    }



    // avoid NaN due to round-off effects
    if(SumCurrentRMSCorrected2<0)
    {
        SumCurrentRMSCorrected2 = 0;
    }

    float CurrentRMSCorrected = sqrt(SumCurrentRMSCorrected2/INoSamples);

    float VoltageRMSCorrected = sqrt(SumVoltageRMSCorrected2/INoSamples);

    const float Current = calculate_current_from_voltage(VoltageRMSCorrected);
    

    //LOG_INFO("IOffset: %.6f A", IOffset);
    //LOG_INFO("gVOffset: %.6f mV", gVOffset);
    //LOG_INFO("SumCurrentRMSCorrected: %.6f A", SumCurrentRMSCorrected2);
    //LOG_INFO("CurrentRMSCorrected: %.6f A", CurrentRMSCorrected);
    //LOG_INFO("SumVoltageValue over 5 cycles: %.6f mV", SumVoltageValue);
    LOG_INFO("Current: %.6f mA",Current*1000);
    //LOG_INFO("VoltageRMSCorrected: %.6f mV", VoltageRMSCorrected);

    // correct offset for next round
    gIOffset = (gIOffset+IOffset);
    //LOG_INFO("gIOffset: %.6f A", gIOffset);

    gVOffset = (gVOffset+VOffset);
    //LOG_INFO("gVOffset: %.6f A", gVOffset);


}


float getTotalSumOfLiters(void)
{
    return TotalSumOfLiters;
}