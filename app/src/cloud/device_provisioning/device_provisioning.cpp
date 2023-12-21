// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "DeviceProvisioning";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "device_provisioning.h"

#include "app_controller.h"
#include "cloud_config.h"
#include "commons.h"
#include "config_nvs.h"
#include "json_parser.h"
#include "sleep.h"

namespace
{
constexpr uint32_t DEVICE_PROVISIONING_THREAD_SLEEP_MS      = 5000;
constexpr int8_t   MQTT_CONNECTION_WAIT_TIME_INFINITE       = -1;
constexpr char     DEVICE_PROVISIONING_RESPONSE_TOPIC[]     = "$dps/registrations/res/#";
constexpr char     DEVICE_PROVISIONING_REGISTRATION_TOPIC[] = "$dps/registrations/PUT/iotdps-register/?$rid=";
constexpr char     DEVICE_PROVISIONING_REGISTRATION_GET_STATUS_TOPIC[] =
    "$dps/registrations/GET/iotdps-get-operationstatus/?$rid=";
} // unnamed namespace

const char* DEVICE_PROVISIONING_MODEL_ID = "dtmi:azure:iot:deviceUpdateContractModel;2";

DeviceProvisioningController::DeviceProvisioningController(MqttClientController* mqttClientController) :
    m_taskHandle(),
    m_provisioningStatus(ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT),
    m_cloudCredentials()
{
    m_pMqttClientController = mqttClientController;
}

void DeviceProvisioningController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned =
        xTaskCreate(run, LOG_TAG, DEFAULT_HUGE_STACK_SIZE, this, DEFAULT_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void DeviceProvisioningController::run(void* pObject)
{
    DeviceProvisioningController* pDeviceProvisioningController =
        reinterpret_cast<DeviceProvisioningController*>(pObject); // NOLINT - we need reinterpret cast
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
    const prot::cloud_set_credentials::TCloudCredentials& cloudCredentialsFromConfig = pConfig->getCloudCredentials();

    char mqttUsername[prot::cloud_set_credentials::CLOUD_MQTT_USERNAME_LENGTH];
    memset(mqttUsername, 0, prot::cloud_set_credentials::CLOUD_MQTT_USERNAME_LENGTH);

    if (!cloudCredentialsFromConfig.isSetCloudDeviceId())
    {
        LOG_ERROR("Cloud device id not provided");
        return;
    }

    sprintf(
        mqttUsername,
        "%s/registrations/%s/api-version=2019-03-31",
        DEVICE_PROVISIONING_ID_SCOPE,
        cloudCredentialsFromConfig.cloudDeviceId);

    m_cloudCredentials.setCloudMqttUsername(mqttUsername);
    m_cloudCredentials.setCloudDeviceId(cloudCredentialsFromConfig.cloudDeviceId);
    m_cloudCredentials.setCloudAddress(CLOUD_DEVICE_PROVISIONING_ADDRESS);
}

void DeviceProvisioningController::createMqttUsernameAfterProvisioning(
    json_parser::TDeviceProvisioningInfo&           provisioningInfo,
    prot::cloud_set_credentials::TCloudCredentials& newCloudCredentials)
{
    char mqttUsername[prot::cloud_set_credentials::CLOUD_MQTT_USERNAME_LENGTH];
    memset(mqttUsername, 0, prot::cloud_set_credentials::CLOUD_MQTT_USERNAME_LENGTH);

    sprintf(
        mqttUsername,
        "%s/%s/?api-version=2021-04-12&model-id=%s",
        provisioningInfo.assignedHub.c_str(),
        provisioningInfo.deviceId.c_str(),
        DEVICE_PROVISIONING_MODEL_ID);
    newCloudCredentials.setCloudMqttUsername(mqttUsername);
}

void DeviceProvisioningController::createCloudAddressAfterProvisioning(
    json_parser::TDeviceProvisioningInfo&           provisioningInfo,
    prot::cloud_set_credentials::TCloudCredentials& newCloudCredentials)
{
    char cloudAddress[prot::cloud_set_credentials::CLOUD_MAX_ADDRESS_LENGTH];
    memset(cloudAddress, 0, prot::cloud_set_credentials::CLOUD_MAX_ADDRESS_LENGTH);

    sprintf(cloudAddress, "mqtts://%s", provisioningInfo.assignedHub.c_str());

    newCloudCredentials.setCloudAddress(cloudAddress);
}

void DeviceProvisioningController::saveCredentialsAfterProvisioning(
    json_parser::TDeviceProvisioningInfo& provisioningInfo)
{
    LOG_INFO("cloud address: %s", pConfig->getCloudCredentials().cloudAddress);
    LOG_INFO("device id: %s", pConfig->getCloudCredentials().cloudDeviceId);
    LOG_INFO("mqtt username: %s", pConfig->getCloudCredentials().cloudMqttUsername);

    prot::cloud_set_credentials::TCloudCredentials newCloudCredentials;

    newCloudCredentials.setCloudDeviceId(provisioningInfo.deviceId.c_str());

    createMqttUsernameAfterProvisioning(provisioningInfo, newCloudCredentials);

    createCloudAddressAfterProvisioning(provisioningInfo, newCloudCredentials);

    pConfig->setCloudCredentials(newCloudCredentials);
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
    sprintf(
        m_deviceRegistrationStatus,
        "%s%d&operationId=%s",
        DEVICE_PROVISIONING_REGISTRATION_GET_STATUS_TOPIC,
        m_deviceRegistrationRequestId,
        m_deviceProvisioningOperationId.c_str());
}

bool DeviceProvisioningController::isProvisioningStatusAssigning(std::string& provisioningStatus)
{
    if (!strncmp(
            DEVICE_PROVISIONING_STATUS_ASSIGNING,
            provisioningStatus.c_str(),
            strlen(DEVICE_PROVISIONING_STATUS_ASSIGNING)))
    {
        return true;
    }

    return false;
}

bool DeviceProvisioningController::isProvisioningStatusAssigned(std::string& provisioningStatus)
{
    if (!strncmp(
            DEVICE_PROVISIONING_STATUS_ASSIGNED,
            provisioningStatus.c_str(),
            strlen(DEVICE_PROVISIONING_STATUS_ASSIGNED)))
    {
        return true;
    }

    return false;
}

void DeviceProvisioningController::setOperationIdIfStatusAssigning(std::string& provisioningOperationId)
{
    if (m_provisioningStatus != ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS)
    {
        m_deviceProvisioningOperationId = provisioningOperationId;
        m_provisioningStatus            = ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS;
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
        LOG_ERROR("Could not connect to cloud, timeout occurred");
    }

    m_pMqttClientController->subscribeToTopic(std::string(DEVICE_PROVISIONING_RESPONSE_TOPIC), 1);

    createDeviceRegistrationTopic();

    m_pMqttClientController->sendMessage(
        std::string(m_deviceRegistrationTopic),
        json_parser::prepareDeviceCreateProvisioningMessage(m_cloudCredentials.cloudDeviceId));

    while (m_provisioningStatus != ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_FINISHED)
    {
        if (!m_pMqttClientController->checkIfMessageBufferIsEmpty())
        {
            json_parser::TMessage                message          = m_pMqttClientController->getNextMessage();
            json_parser::TDeviceProvisioningInfo provisioningInfo = {};
            json_parser::parseJsonDeviceProvisioning(message.message, &provisioningInfo);

            LOG_INFO(
                "Provisioning message: operation id: %s, status: %s",
                provisioningInfo.operationId.c_str(),
                provisioningInfo.status.c_str());

            if (isProvisioningStatusAssigning(provisioningInfo.status))
            {
                setOperationIdIfStatusAssigning(provisioningInfo.operationId);
            }
            else if (isProvisioningStatusAssigned(provisioningInfo.status))
            {
                LOG_INFO(
                    "Device assigned message: assignedHub: %s, deviceId: %s",
                    provisioningInfo.assignedHub.c_str(),
                    provisioningInfo.deviceId.c_str());
                saveCredentialsAfterProvisioning(provisioningInfo);

                m_provisioningStatus = ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_FINISHED;
                pConfig->setDeviceProvisioningStatus(m_provisioningStatus);

                app::pAppController->addEvent(app::EEventType::PERFORM_DEVICE_RESTART);
            }
        }

        if (m_provisioningStatus == ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_IN_PROGRESS)
        {
            createDeviceRegistrationStatusTopic();
            if (!(m_pMqttClientController->waitUntilMqttConnected(MQTT_CONNECTION_WAIT_TIME_INFINITE)))
            {
                LOG_ERROR("Could not connect to cloud, timeout occurred");
            }
            m_pMqttClientController->sendMessage(m_deviceRegistrationStatus, std::string("{}"));
        }

        SLEEP_MS(DEVICE_PROVISIONING_THREAD_SLEEP_MS);
    }
}
