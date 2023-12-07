// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "HwMisc";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "hw_misc.h"

#include "commons.h"
#include "sleep.h"

#include "nvs_flash.h"


namespace hw_misc
{

void initializeNVS()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

int32_t waitForGpioChange(gpio_num_t gpio, bool desiredState, int32_t timeout)
{
    int64_t beginTimestamp = commons::getCurrentTimestampMs();
    int32_t timeElapsed    = 0;
    while (timeElapsed < timeout)
    {
        if (gpio_get_level(gpio) == desiredState)
            return timeElapsed;
        timeElapsed = static_cast<int32_t>(commons::getCurrentTimestampMs() - beginTimestamp);
        SLEEP_MS(10);
    }
    return GPIO_CHANGE_DID_NOT_HAPPEN;
}

bool getGpioState(gpio_num_t gpio)
{
    return gpio_get_level(gpio);
}

bool getStaMacAddressString(char* macAddress, const int32_t len)
{
#if USE_CUSTOM_WIFI_MAC_ADDRESS
    uint8_t macBytes[6] = CUSTOM_WIFI_MAC_ADDRESS;
#else
    uint8_t   macBytes[6] = {0};
    esp_err_t err         = esp_read_mac(macBytes, ESP_MAC_WIFI_STA);
    if (err != ESP_OK)
    {
        LOG_ERROR("Could not get WiFi MAC. Probably WiFi not initialized!");
        return false;
    }
#endif

    char    tempBuffer[32] = {0};
    int32_t macLen         = sprintf(
        tempBuffer,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        macBytes[0],
        macBytes[1],
        macBytes[2],
        macBytes[3],
        macBytes[4],
        macBytes[5]);
    if (macLen > len) // To prevent buffer overflow
    {
        LOG_ERROR("Invalid buffer length!");
        return false;
    }
    else
    {
        memcpy(macAddress, tempBuffer, len);
        return true;
    }
}

} // namespace hw_misc
