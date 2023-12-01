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

DeviceProvisioningController::DeviceProvisioningController(MqttClientController *mqttClientController, CloudController *cloudController)
    : m_taskHandle(),
      m_provisioningStatus(ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT)
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

void DeviceProvisioningController::_run()
{
    LOG_INFO("Start device provisioning procedure");
}
