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

    uint32_t resolution = 68.027211; // für den 20A Sensor //125.8503401; für den 5A Sensor // mV/A
    uint64_t IEndMeasure;
    uint64_t IStartMeasure;

    float gIOffset = 0;
    float gVOffset = 0;

    float TimeSparklingWater = 0;
    float SumSparklingWater = 0;
    float TimeCooling = 0;
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

float getRMSCurrent(void)
{

    float SumCurrentValue2 = 0;
    float SumCurrentValue = 0;

    float SumVoltageValue2 = 0;
    float SumVoltageValue = 0;

    float INoSamples = 0;
    const float gulSamplePeriod_us = 5 * 20 * 1000; //sample over 4 period (20ms pro Periode bei 50 Hz)

    //define start and end time of the current measurement;
    IEndMeasure = commons::getCurrentTimestampUs() + gulSamplePeriod_us;
    IStartMeasure = commons::getCurrentTimestampUs();

    while(commons::getCurrentTimestampUs() < IEndMeasure)
    {
        //read adc value from the sensor
        const uint32_t adcReading = static_cast<uint32_t>(adc1_get_raw(SENSOR_CHANNEL));

        //substract the voltage offset of the current sensor
        const float voltage = esp_adc_cal_raw_to_voltage(adcReading, &adcChars) - gVOffset;

        const float voltage2 = voltage * voltage;

        SumVoltageValue = SumVoltageValue + voltage;
        SumVoltageValue2 = SumVoltageValue2 + voltage2;

        INoSamples++;
    }

    IEndMeasure = commons::getCurrentTimestampUs();

    uint64_t totalTimeUsedToMeasureCurrent_us = IEndMeasure - IStartMeasure;

    float VOffset = SumVoltageValue / INoSamples;

    const float VoltageRMS = sqrt(SumVoltageValue2 / INoSamples);

    float SumVoltageRMSOffsetCorrected2 = SumVoltageValue2 - 2*VOffset*SumVoltageValue - VOffset*VOffset*INoSamples;

    // avoid NaN due to round-off effects
    if(SumVoltageRMSOffsetCorrected2<0)
    {
        SumVoltageRMSOffsetCorrected2 = 0;
    }

    //calculate the 
    float VoltageRMSCorrected = sqrt(SumVoltageRMSOffsetCorrected2/INoSamples);

    const float Current = calculate_current_from_voltage(VoltageRMSCorrected);

    gVOffset = (gVOffset+VOffset);

    return Current;
}


float* get3RMSCurrentDataPoints(void)
{
    float current = 0;
    float SumCurrent = 0;
    int32_t indexCurrent = 0;
    int32_t indexTime = 1;
    float currentmA = 0;
    static float currentDataPointsAndTimestamp[] = {0, 0, 0, 0, 0, 0};


    while(indexCurrent<5)
    {
        current = getRMSCurrent();        
        currentmA = current*1000;
        SumCurrent = SumCurrent + currentmA;

        currentDataPointsAndTimestamp[indexCurrent] = currentmA;
        currentDataPointsAndTimestamp[indexTime] = float(commons::getCurrentTimestampMs());

        indexCurrent = indexCurrent + 2;
        indexTime = indexTime + 2;
    }
    
    return currentDataPointsAndTimestamp;

    //delete[] currentDataPointsAndTimestamp;

}



float SumOfSparklingWater(void)
{
    //an array with current datapoints and the correspondent timestamp is alternative filled. 
    //static float *currentDataPointsAndTimestamp = new float[6];
    //currentDataPointsAndTimestamp = get3RMSCurrentDataPoints();


    static float *currentDataPointsAndTimestamp = get3RMSCurrentDataPoints();
    float timeBetween3DataPoints = 0;

    //average current over 3 datapoints
    float avgCurrentmA = ((currentDataPointsAndTimestamp[0] + currentDataPointsAndTimestamp[2] + currentDataPointsAndTimestamp[4]) / 3);
    LOG_INFO("avgCurrentmA: %.6f mA \n",(avgCurrentmA));

    //if sparkling pump is on
    if(480 < avgCurrentmA && avgCurrentmA < 650)
    {
        //LOG_INFO("sparkling Water (only pump)");
        timeBetween3DataPoints = (currentDataPointsAndTimestamp[5] - currentDataPointsAndTimestamp[1])/1000; // in seconds
        TimeSparklingWater = TimeSparklingWater + timeBetween3DataPoints;

    }

    //if sparkling pump is on and the cooling is running
    if(avgCurrentmA > 2100)
    {
        timeBetween3DataPoints = (currentDataPointsAndTimestamp[5] - currentDataPointsAndTimestamp[1])/1000; // in seconds
        TimeSparklingWater = TimeSparklingWater + timeBetween3DataPoints;
    }

    SumSparklingWater = TimeSparklingWater * 0.24/4;
    LOG_INFO("SumSparklingWater: %.6f Liter \n",(SumSparklingWater));

    free(currentDataPointsAndTimestamp);

    return SumSparklingWater;

}


float TimeCoolingIsRunning(void)
{
    //an array with current datapoints and the correspondent timestamp is alternative filled. 
    //static float *currentDataPointsAndTimestamp = new float[6];
    //currentDataPointsAndTimestamp = get3RMSCurrentDataPoints();


    static float *currentDataPointsAndTimestamp = get3RMSCurrentDataPoints();
    float timeBetween3DataPoints = 0;

    //average current over 3 datapoints
    float avgCurrentmA = ((currentDataPointsAndTimestamp[0] + currentDataPointsAndTimestamp[2] + currentDataPointsAndTimestamp[4]) / 3);
    LOG_INFO("avgCurrentmA: %.6f mA \n",(avgCurrentmA));

    //if sparkling pump is on and the cooling is running
    if(avgCurrentmA > 2100)
    {
        timeBetween3DataPoints = (currentDataPointsAndTimestamp[5] - currentDataPointsAndTimestamp[1])/1000; // in seconds
        TimeCooling = TimeCooling + timeBetween3DataPoints;
    }

    //if the cooling is running
    if(1700 < avgCurrentmA && avgCurrentmA < 2000)
    {
        timeBetween3DataPoints = (currentDataPointsAndTimestamp[5] - currentDataPointsAndTimestamp[1])/1000; // in seconds
        TimeCooling = TimeCooling + timeBetween3DataPoints;
    }

    LOG_INFO("TimeCooling: %.6f hours %.6f minutes %.6f seconds \n",(TimeCooling)/3600, (TimeCooling)/60, (TimeCooling));

    free(currentDataPointsAndTimestamp);

    return TimeCooling;

}