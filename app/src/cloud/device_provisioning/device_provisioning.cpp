// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "DeviceProvisioning";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "device_provisioning.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "sleep.h"
#include "cloud_config.h"
#include "json_parser.h"

namespace
{
    constexpr int8_t MQTT_CONNECTION_WAIT_TIME_INFINITE = -1;
    constexpr char DEVICE_PROVISIONING_RESPONSE_TOPIC[] = "$dps/registrations/res/#";
    constexpr char DEVICE_PROVISIONING_REGISTRATION_TOPIC[] = "$dps/registrations/PUT/iotdps-register/?$rid=";
    constexpr char DEVICE_PROVISIONING_REGISTRATION_GET_STATUS_TOPIC[] = "$dps/registrations/GET/iotdps-get-operationstatus/?$rid=";
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

void DeviceProvisioningController::createDeviceRegistrationTopic()
{
    m_deviceRegistrationRequestId = rand();
    memset(m_deviceRegistrationTopic, 0, DEVICE_PROVISIONING_MAX_TOPIC_SIZE);
    sprintf(m_deviceRegistrationTopic, "%s%d", DEVICE_PROVISIONING_REGISTRATION_TOPIC, m_deviceRegistrationRequestId);
}

void DeviceProvisioningController::createDeviceRegistrationStatusTopic()
{
    memset(m_deviceRegistrationStatus, 0, DEVICE_PROVISIONING_MAX_TOPIC_SIZE);
    sprintf(m_deviceRegistrationStatus, "%s%d&operationId=%s", DEVICE_PROVISIONING_REGISTRATION_GET_STATUS_TOPIC, m_deviceRegistrationRequestId, m_deviceProvisioningOperationId.c_str());
}

void DeviceProvisioningController::setOperationIdIfStatusAssigning(std::string &provisioningStatus, std::string &provisioningOperationId)
{
    if (!strncmp(DEVICE_PROVISIONING_STATUS_ASSIGNING, provisioningStatus.c_str(), strlen(DEVICE_PROVISIONING_STATUS_ASSIGNING)) && m_provisioningStatus != ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS)
    {
        m_deviceProvisioningOperationId = provisioningOperationId;
        m_provisioningStatus = ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS;
        LOG_INFO("Device provisioning in progress");
        LOG_INFO("m_deviceProvisioningOperationId: %s", m_deviceProvisioningOperationId.c_str());
    }
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

    m_pMqttClientController->subscribeToTopic(std::string(DEVICE_PROVISIONING_RESPONSE_TOPIC), 1);

    createDeviceRegistrationTopic();

    m_pMqttClientController->sendMessage(std::string(m_deviceRegistrationTopic), json_parser::prepareDeviceCreateProvisioningMessage(m_cloudCredentials.cloudDeviceId));

    while (true)
    {
        if (!m_pMqttClientController->checkIfMessageBufferIsEmpty())
        {
            json_parser::TMessage message = m_pMqttClientController->getNextMessage();
            json_parser::TDeviceProvisioningInfo provisioningInfo = {};
            json_parser::parseJsonDeviceProvisioning(message.message, &provisioningInfo);

            LOG_INFO("Provisioning message: operation id: %s, status: %s", provisioningInfo.operationId.c_str(), provisioningInfo.status.c_str());

            setOperationIdIfStatusAssigning(provisioningInfo.status, provisioningInfo.operationId);
        }

        if (m_provisioningStatus == ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS)
        {
            createDeviceRegistrationStatusTopic();
            if (!(m_pMqttClientController->waitUntilMqttConnected(MQTT_CONNECTION_WAIT_TIME_INFINITE)))
            {
                LOG_ERROR("Could not connect to cloud, timeout occured");
            }
            m_pMqttClientController->sendMessage(m_deviceRegistrationStatus, std::string("{}"));
        }

        SLEEP_MS(1000);
    }
}
