// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "Commons";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "commons.h"

#include <cstdint>
#include <cstdio>
#include <sys/time.h>


namespace commons
{

uint32_t getCurrentTimestampS()
{
    return static_cast<uint32_t>(getCurrentTimestampMs() / MS_IN_S);
}

int64_t getCurrentTimestampMs()
{
    struct timeval currentTime = {};
    gettimeofday(&currentTime, nullptr);
    int64_t millis = static_cast<int64_t>(currentTime.tv_sec) * 1000 + (currentTime.tv_usec / 1000);
    return millis;
}

int64_t getCurrentTimestampUs()
{
    struct timeval currentTime = {};
    gettimeofday(&currentTime, nullptr);
    int64_t us = static_cast<int64_t>(currentTime.tv_sec) * 1000000 + currentTime.tv_usec;
    return us;
}

bool isInRange(float value, float min, float max)
{
    return (value >= min && value <= max);
}

void printArrayAsHex(const uint8_t* pData, uint16_t length)
{
    printf("\tData length %d bytes:\n\t", length);

    for (int i = 0; i < length; i++)
        printf("%02X ", pData[i]);

    printf("\n");
}

void printArrayAsBin(const uint8_t* pData, uint16_t length)
{
    printf("Data length %d, bytes: \n", length);

    for (int i = 0; i < length; i++)
    {
        if (i != 0)
            printf(" ");

        for (uint8_t bitMask = 0b10000000; bitMask; bitMask = bitMask >> 1u)
        {
            if (pData[i] & bitMask)
                printf("1");
            else
                printf("0");
        }
    }

    printf("\n");
}


void printByteAsBin8(uint8_t value)
{
    for (uint8_t bitMask = 0b10000000; bitMask; bitMask = bitMask >> 1u)
    {
        if (value & bitMask)
            printf("1");
        else
            printf("0");
    }
}

void printWarningCode(esp_err_t errorCode, const char* message)
{
    if (errorCode != ESP_OK)
    {
        LOG_WARNING("%s, error: %s", message, esp_err_to_name(errorCode));
    }
}

void printErrorCode(esp_err_t errorCode, const char* message)
{
    if (errorCode != ESP_OK)
    {
        LOG_ERROR("%s, error: %s", message, esp_err_to_name(errorCode));
    }
}

#if !TESTING
#define OPTIMIZE_O0_ATTRIBUTE __attribute__((optimize("O0")))
#else
#define OPTIMIZE_O0_ATTRIBUTE
#endif

void OPTIMIZE_O0_ATTRIBUTE busySleepUs(uint32_t us)
{
    if (us > 200)
    {
        us = us * 50 /
             51; // 10 x NOP operation + loop iteration takes about 1.02 us - it may be "corrected" by this multiplier
    }

    for (uint32_t i = 0; i < us; i++)
    {
        for (int i = 0; i < 10; i++)
        {
            asm(" nop"); // NOLINT
        }
    }
}

std::string dataArrayToHexStr(uint32_t maxSize, const uint8_t* pData, uint32_t dataLength)
{
    std::string txt;
    txt.reserve(dataLength * 3); // anticipate 3 character for each byte (e.g. "3A ")

    for (uint32_t i = 0; i < dataLength; i++)
    {
        /* i use 5 here since we are going to add at most
           3 chars, need a space for the end '\n' and need
           a null terminator */
        // +3 bytes for "..."
        if (txt.size() + 8 < maxSize)
        {
            if (i > 0)
            {
                txt += ":";
            }
            char buff[16] = {};
            sprintf(buff, "%02X", pData[i]);
            txt += buff;
        }
        else
        {
            txt += "...";
            break;
        }
    }

    return txt;
}

void printAvailableHeapMemory(int lineNumber, const char* fileName, const char* functionName)
{
    printf(
        "\n***************\n"
        "File name: %s\n"
        "Function name: %s\n"
        "Line number: %d\n"
        "Currently available free heap memory: %d\n"
        "Minimum available free heap memory: %d\n"
        "***************\n\n",
        fileName,
        functionName,
        lineNumber,
        static_cast<uint32_t>(
            heap_caps_get_free_size(MALLOC_CAP_INTERNAL)), // NOLINT - this warning does not make sense in that case
        esp_get_minimum_free_heap_size()); // it requires to explicitly set bit shift as unsigned (e.g. 1u << 11u)
                                           // correcting this would require changing the library code (esp_heap_caps.h)
}

} // namespace commons
