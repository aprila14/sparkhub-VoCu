// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "Main";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "adc_pressure.h"
#include "app_controller.h"
#include "ble_controller.h"
#include "bleuart_esp.h"
#include "cloud_config.h"
#include "commons.h"
#include "config_nvs.h"
#include "console_control.h"
#include "defines.h"
#include "hw_misc.h"
#include "ntp_client.h"
#include "reset_button_handler.h"
#include "wifi_controller.h"

#if IS_DEBUG_BUILD

void temporaryDevelopmentCode()
{
    // just some quick code for testing purposes
    LOG_WARNING("Running temporaryDevelopmentCode!");

    // Set BLE configuration state to finished
    // pConfig->setBleConfigurationStatus(EBleConfigurationStatus::BLE_CONFIGURATION_STATUS_FINISHED);

    {
        // prot::send_certificates::TCmd* pCmdCertificate = new prot::send_certificates::TCmd();

        // LOG_INFO("fullChainCertificate: %s", pConfig->getCertificatePack().fullChainCertificate);

        // pCmdCertificate->certificates.setFullChainCertificate(std::string(DEFAULT_CLIENT_PUBLIC_CERT));
        // pCmdCertificate->certificates.setPrivateKey(std::string(DEFAULT_CLIENT_PRIVATE_KEY));

        // pConfig->setCertificatePack(pCmdCertificate->certificates);

        // delete pCmdCertificate;
    }
}
#endif // IS_DEBUG_BUILD

static void configureConnectionToLteModem()
{
    LOG_INFO("ssid: %s", pConfig->getWifiCredentials().ssid);
    TWiFiCredentials newWifiCredentials;
    newWifiCredentials.setSsid("4G UFI-4205");
    newWifiCredentials.setPassword("1234567890");
    pConfig->setWifiCredentials(newWifiCredentials);
}

static void configureCloudDeviceIdIfNotSet(WiFiController& wifiController)
{
    const TCloudCredentials& cloudCredentialsFromConfig = pConfig->getCloudCredentials();

    LOG_INFO("cloudDeviceId: %s", cloudCredentialsFromConfig.cloudDeviceId);
    LOG_INFO("cloudAddress: %s", cloudCredentialsFromConfig.cloudAddress);
    LOG_INFO("cloudMqttUsername: %s", cloudCredentialsFromConfig.cloudMqttUsername);

    if (!cloudCredentialsFromConfig.isSetCloudDeviceId())
    {
        uint8_t mac[6] = {};

        wifiController.getWifiMacAddress(mac);

        TCloudCredentials newCloudCredentials = {};

        char cloudDeviceId[prot::cloud_set_credentials::CLOUD_DEVICE_ID_LENGTH] = {};

        sprintf(cloudDeviceId, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        newCloudCredentials.setCloudDeviceId(cloudDeviceId);

        pConfig->setCloudCredentials(newCloudCredentials);

        LOG_INFO("new cloudDeviceId: %s", newCloudCredentials.cloudDeviceId);
    }
}

void initCommonGlobalModules()
{
    hw_misc::initializeNVS();

    static ConfigNvs config;
    pConfig = &config;
    if (!pConfig->init())
    {
        LOG_ERROR("Failed to open the NVS, but there is nothing we can do about it anyway...");
    }

    configureConnectionToLteModem();

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
    g_pBleuartDriver = &bleuartDriver; // we need to have a global singleton for hardware callbacks, see discussion near
                                       // the variable declaration
    static BleController bleController(&bleuartDriver);

    static CloudController cloudController;

    // create and run app controller
    static app::AppController appController(&wifiController, &bleController, &cloudController, &ntpClient);
    app::pAppController = &appController;

    // ADC for pressure sensor
    adcInit();

    configureCloudDeviceIdIfNotSet(wifiController);

    const EBleConfigurationStatus state = pConfig->getBleConfigurationStatus();
    LOG_INFO("BLE Configuration status = %d", state);

    // run modules which are tasks
    if (state == EBleConfigurationStatus::BLE_CONFIGURATION_STATUS_INIT)
    {
        LOG_WARNING("Starting BLE configuration");
        bleuartDriver.runTask(); // keep it first, there is also some initialization there, that I'm not sure about
        bleController.runTask();
    }
    else
    {
        bleuartDriver.cleanup();
        wifiController.runTask();
        wifiController.loadCredentialsFromConfigNvsAndConnectIfSet();
        ntpClient.runTask();
        cloudController.runTask();
    }
}

extern "C"
{

    void app_main(void)
    {
        LOG_WARNING("MAIN START");
        // TODO - consider adding here "quick reset", before initializing other modules,
        // to have a reliable method of resetting the device configuration, before initializing other modules

        initCommonGlobalModules();

        app::pAppController->runTask();

#if USE_CONSOLE
        console::runConsoleControl();
#endif

        LOG_WARNING("Application main finished!");
    }

} // extern "C"
