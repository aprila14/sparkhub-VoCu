#include "config_nvs.h"
#include "defines.h"

#include "mock_nvs.h"
#include "test_utils.h"

#include "gtest/gtest.h"

#include <cstring>


TEST(ConfigNvs, init_ok)
{
    resetCmock();
    // we want be using `pConfig` for tests, at it is the mocked version of global `ConfigNvs`
    ConfigNvs configNvs;


    // opening FLASH
    nvs_open_IgnoreAndReturn(ESP_OK);

    // ignore commmits ot NVS
    nvs_commit_IgnoreAndReturn(ESP_OK);


    // READING MEMBERS (I have no good idea for members order, so just use the same order as in the erading function:

    // getConfigurationFinishedState
    nvs_get_u8_IgnoreAndReturn(ESP_OK);
    nvs_get_u8_StubWithCallback([](nvs_handle_t, const char*, uint8_t* out_value, int) {
        *out_value = 1;
        return ESP_OK;
    });

    // getWifiCredentials
    TWiFiCredentials wifiCredentials;
    wifiCredentials.setPassword("pass");
    wifiCredentials.setSsid("ssid");
    nvs_get_blob_IgnoreAndReturn(ESP_OK);
    nvs_get_blob_StubWithCallback([](nvs_handle_t, const char*, void* out_value, size_t*, int) {
        TWiFiCredentials wifiCredentials; // unfortunaltely our lambda cannot capture, so just copy it
        wifiCredentials.setPassword("pass");
        wifiCredentials.setSsid("ssid");
        ::memcpy(out_value, &wifiCredentials, sizeof(wifiCredentials));
        return ESP_OK;
    });


    // INIT
    ASSERT_TRUE(configNvs.init());

    ASSERT_EQ(configNvs.getConfigurationFinishedState(), true);
    ASSERT_EQ(configNvs.getWifiCredentials(), wifiCredentials);
}


TEST(ConfigNvs, init_error)
{
    resetCmock();
    // we want be using `pConfig` for tests, at it is the mocked version of global `ConfigNvs`
    ConfigNvs configNvs;

    // opening FLASH
    nvs_open_IgnoreAndReturn(ESP_FAIL);


    // INIT
    ASSERT_FALSE(configNvs.init());
}


TEST(ConfigNvs, setWifiCredentials)
{
    resetCmock();
    ConfigNvs configNvs;

    nvs_set_blob_ExpectAndReturn(0, ConfigKeyName::WIFI_CREDENTIALS, nullptr, sizeof(TWiFiCredentials), ESP_OK);
    nvs_set_blob_IgnoreArg_handle();
    nvs_set_blob_IgnoreArg_value();
    nvs_commit_IgnoreAndReturn(ESP_OK);

    TWiFiCredentials wifiCredentials2;
    wifiCredentials2.setSsid("ssid2");
    configNvs.setWifiCredentials(wifiCredentials2);
    ASSERT_EQ(configNvs.getWifiCredentials(), wifiCredentials2);
    ASSERT_FALSE(configNvs.getWifiCredentials() == TWiFiCredentials());
}

TEST(ConfigNvs, setCloudCredentials)
{
    resetCmock();
    ConfigNvs configNvs;

    nvs_set_blob_ExpectAndReturn(0, ConfigKeyName::CLOUD_CREDENTIALS, nullptr, sizeof(TCloudCredentials), ESP_OK);
    nvs_set_blob_IgnoreArg_handle();
    nvs_set_blob_IgnoreArg_value();
    nvs_commit_IgnoreAndReturn(ESP_OK);

    TCloudCredentials cloudCredentials2;
    cloudCredentials2.setCloudAddress("privateKey0123");
    configNvs.setCloudCredentials(cloudCredentials2);

    ASSERT_EQ(configNvs.getCloudCredentials(), cloudCredentials2);
    ASSERT_FALSE(configNvs.getCloudCredentials() == TCloudCredentials());
}

TEST(ConfigNvs, setConfigurationFinishedState)
{
    resetCmock();
    ConfigNvs configNvs;

    nvs_set_u8_ExpectAndReturn(0, ConfigKeyName::CONFIGURATION_FINISHED_STATE, 0, ESP_OK);
    nvs_set_u8_IgnoreArg_handle();
    nvs_commit_IgnoreAndReturn(ESP_OK);

    configNvs.setConfigurationFinishedState(false);
    ASSERT_EQ(configNvs.getConfigurationFinishedState(), false);
}

TEST(ConfigNvs, resetAll)
{
    resetCmock();
    ConfigNvs configNvs1;


    ConfigNvs configNvs2;
    configNvs2.m_configurationFinishedState = true;
    configNvs2.m_wiFiCredentials.setPassword("aaaa");

    nvs_erase_all_IgnoreAndReturn(ESP_OK);
    configNvs2.resetConfig();

    ASSERT_EQ(configNvs2.m_configurationFinishedState, configNvs1.m_configurationFinishedState);
    ASSERT_EQ(configNvs2.m_wiFiCredentials, configNvs1.m_wiFiCredentials);
}
