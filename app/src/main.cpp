// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "Main";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "app_controller.h"
#include "bleuart_esp.h"
#include "ble_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "console_control.h"
#include "defines.h"
#include "hardware_definitions.h"
#include "hw_misc.h"
#include "led_control.h"
#include "led_indicator.h"
#include "ntp_client.h"
#include "reset_button_handler.h"
#include "wifi_controller.h"
#include "cloud_config.h"
#include "adc_VoCu.h"

#if IS_DEBUG_BUILD
// TCloudCertificatePack newCloudCertificates;

void temporaryDevelopmentCode()
{
    // just some quick code for testing purposes
    LOG_WARNING("Running temporaryDevelopmentCode!");

    bool state = pConfig->getConfigurationFinishedState();
    LOG_INFO("Configuration finished state = %d", state);
    pConfig->setConfigurationFinishedState(true);

    {
        LOG_INFO("ssid: %s", pConfig->getWifiCredentials().ssid);
        TWiFiCredentials newWifiCredentials;
        newWifiCredentials.setSsid("Marty14");
        newWifiCredentials.setPassword("livefreeordie");
        pConfig->setWifiCredentials(newWifiCredentials);
    }

    {
        LOG_INFO("cloud address: %s", pConfig->getCloudCredentials().cloudAddress);
        TCloudCredentials newCloudCredentials;
        newCloudCredentials.setCloudAddress(CLOUD_ADDRESS);
        pConfig->setCloudCredentials(newCloudCredentials);
    }

    // {
    //     LOG_INFO("clientPublicCertificate: %s", pConfig->getCloudCertificates().clientPublicCertificate);
    //     newCloudCertificates.setServerPublicCertificate(DEFAULT_SERVER_PUBLIC_CERT);
    //     newCloudCertificates.setClientPublicCertificate(DEFAULT_CLIENT_PUBLIC_CERT);
    //     newCloudCertificates.setClientPrivateKey(DEFAULT_CLIENT_PRIVATE_KEY);
    // }
}
#endif // IS_DEBUG_BUILD

void initCommonGlobalModules()
{
    hw_misc::initializeNVS();

    static ConfigNvs config;
    pConfig = &config;
    if (!pConfig->init())
    {
        LOG_ERROR("Failed to open the NVS, but there is nothing we can do about it anyway...");
    }

#if IS_DEBUG_BUILD
    temporaryDevelopmentCode();
#endif



    // create modules
    static WiFiController wifiController;
    if (!wifiController.init())
    {
        LOG_ERROR("Failed to init WiFiController!");
    }
    else
    {
        LOG_INFO("WiFiController initialized correctly");
    }

    static NtpClient ntpClient;
    ntpClient.init(); // no return value

    static BleuartDriver bleuartDriver = BleuartDriver();
    g_pBleuartDriver = &bleuartDriver; // we need to have a global singleton for hardware callbacks, see discussion near the variable declaration
    static BleController bleController(&bleuartDriver);

    static CloudController cloudController;

    // create and run app controller
    static app::AppController appController(&wifiController, &bleController, &cloudController, &ntpClient);
    app::pAppController = &appController;

    // ADC for pressure sensor
    adcInit();

    // run modules which are tasks
    // TODO bleuartDriver.runTask(); // keep it first, there is also some initialization there, that I'm not sure about
    wifiController.runTask();
    wifiController.loadCredentialsFromConfigNvsAndConnectIfSet();
    // TODO bleController.runTask();
    ntpClient.runTask();
    cloudController.runTask();
}



extern "C"
{

    void app_main(void)
    {
        LOG_WARNING("MAIN START");
        // TODO - consider adding here "quick reset", before initializing other modules,
        // to have a reliable method of resetting the device configuration, before initializing other modules

        //initCommonGlobalModules();

        // ADC for pressure sensor
        adcInit();  
        while(true)
        {
            SumOfSparklingWater();
        }

        //app::pAppController->runTask();

#if USE_CONSOLE
        console::runConsoleControl();
#endif

        LOG_WARNING("Application main finished!");
    }

} // extern "C"
