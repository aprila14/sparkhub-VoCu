// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "DeviceProvisioning";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "device_provisioning.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "protocol_types.h"
#include "sleep.h"
#include "cloud_config.h"

namespace
{
    constexpr int8_t MQTT_CONNECTION_WAIT_TIME_INFINITE = -1;
} // unnamed namespace

DeviceProvisioningController::DeviceProvisioningController(MqttClientController *mqttClientController, CloudController *cloudController)
    : m_taskHandle(),
      m_provisioningStatus(ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT),
      m_cloudCredentials()
{
    m_pMqttClientController = mqttClientController;
    m_pCloudController = cloudController;
}

void DeviceProvisioningController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned = xTaskCreate(
        run,
        LOG_TAG,
        DEFAULT_HUGE_STACK_SIZE,
        this,
        DEFAULT_TASK_PRIORITY,
        &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void DeviceProvisioningController::run(void *pObject)
{
    DeviceProvisioningController *pDeviceProvisioningController = reinterpret_cast<DeviceProvisioningController *>(pObject); // NOLINT - we need reinterpret cast
    pDeviceProvisioningController->_run();
}

void DeviceProvisioningController::initiateCloudConnection()
{
    if (!(m_pMqttClientController->init(m_cloudCredentials)))
    {
        LOG_ERROR("Could not init MqttClientController");
        return;
    }

    if (!(m_pMqttClientController->start()))
    {
        LOG_INFO("Error while starting mqttClientController, could not connect to the cloud");
        return;
    }
}

void DeviceProvisioningController::configureCloudConnection()
{
    m_cloudCredentials.setCloudAddress(CLOUD_DEVICE_PROVISIONING_ADDRESS);
    m_cloudCredentials.setCloudDeviceId(DEFAULT_DEVICE_ID);
    m_cloudCredentials.setCloudMqttUsername(DEVICE_PROVISIONING_MQTT_USERNAME);
}

void DeviceProvisioningController::_run()
{
    LOG_INFO("Start device provisioning procedure");

    configureCloudConnection();

    initiateCloudConnection();

    if (!(m_pMqttClientController->waitUntilMqttConnected(MQTT_CONNECTION_WAIT_TIME_INFINITE)))
    {
        LOG_ERROR("Could not connect to cloud, timeout occured");
    }

    m_pMqttClientController->subscribeToTopic(std::string("$dps/registrations/res/#"), 1);

    m_pMqttClientController->sendMessage(std::string("$dps/registrations/PUT/iotdps-register/?$rid=12345"), std::string("{}"));

    while (true)
    {
        SLEEP_MS(1000);
    }
}
