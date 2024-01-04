/*
Miscelenous ESP32 functionality, not related to hardware.
Preferably not related to internal libraries
*/

#ifndef COMMONS_H
#define COMMONS_H

#include "defines.h"


#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include <cstdint>
#include <string>

namespace commons
{

constexpr bool RUN_CONSOLE_AS_SEPARATE_TASK = true;


float convertTemperatureIntToFloat(int32_t temperature);

int32_t convertSignedFloatToInt(float temperature);
int32_t convertUnsignedFloatToInt(float humidity);
int32_t convertSignedFloatToUint16(float temperature);
int32_t convertUnsignedFloatToUint16(float humidity);

/**
 * @brief get current timestamp in ms since ESP boot. Consider adding a separate function for the actual time
 * @return ms
 */
int64_t getCurrentTimestampMs(); // Milliseconds since device power up

int64_t getCurrentTimestampUs(); // us since device power up

uint32_t getCurrentTimestampS(); // seconds since device power up

bool isInRange(float value, float min, float max);

void printArrayAsHex(const uint8_t* pData, uint16_t length);

void printArrayAsBin(const uint8_t* pData, uint16_t length);

void printByteAsBin8(uint8_t value);

void busySleepUs(uint32_t us);

void printWarningCode(esp_err_t errorCode, const char* message);
void printErrorCode(esp_err_t errorCode, const char* message);

void printAvailableHeapMemory(int lineNumber, const char* fileName, const char* functionName);

void runConsoleControlIfApplicable(bool asSeparateTask = false);

void waitUntilConsoleFinished();

std::string dataArrayToHexStr(uint32_t maxSize, const uint8_t* pData, uint32_t dataLength);


} // namespace commons

#endif // COMMONS_H
