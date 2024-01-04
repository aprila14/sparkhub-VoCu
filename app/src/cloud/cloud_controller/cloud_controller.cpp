// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "CloudController";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "cloud_controller.h"
#include "adc_pressure.h"
#include "app_controller.h"
#include "cloud_config.h"
#include "commons.h"
#include "config_nvs.h"
#include "ntp_client.h"
#include "protocol_types.h"
#include "sleep.h"

#include <memory>

namespace
{

constexpr uint32_t SLEEP_TIME_BEFORE_STARTING_DEVICE_TWINS_CONTROLLER_MS = 1000;
constexpr uint8_t  DEVICE_STATUS_MAX_TOPIC_SIZE                       = 200;
constexpr uint32_t SLEEP_TIME_BETWEEN_SENDING_MESSAGES_MS             = 8 * 60 * 60 * 1000; // every 8 hour
constexpr uint32_t SLEEP_TIME_BETWEEN_CHECKING_PRESSURE_THRESHOLD_MS  = 60 * 1000;    // every 1 minute
constexpr uint16_t LOCAL_TIME_OFFSET                                  = UtcOffset::OFFSET_UTC_2;
constexpr int8_t   MQTT_CONNECTION_WAIT_TIME_INFINITE                 = -1;
constexpr uint16_t HEARTBEAT_CHECK_TIMER_PERIOD_MS                    = 1000;
constexpr float PRESSUREALARMTHRESHOLD                                = 3.7;
bool firstTimePressureAlarmDetected                                   = true;
bool isBelowPressureAlarm                                             = false;
uint32_t timeLastUpdateDeviceStatusMs                                 = commons::getCurrentTimestampMs();


} // unnamed namespace

CloudController::CloudController() :
    m_taskHandle(),
    m_msgCounter(0),
    m_connectionStatus(ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED),
    m_mqttClientController(this),
    m_deviceProvisioningController(&this->m_mqttClientController),
    m_deviceTwinsController(&this->m_mqttClientController, this)
{
    m_semaphoreCredentialsReady    = xSemaphoreCreateBinary();
    m_semaphoreWifiConnectionReady = xSemaphoreCreateBinary();
}

void CloudController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned =
        xTaskCreate(run, LOG_TAG, DEFAULT_HUGE_STACK_SIZE, this, DEFAULT_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

ECloudConnectionStatus CloudController::getConnectionStatus() const
{
    return m_connectionStatus;
}

void CloudController::handleStatusReportResponse(bool ACK)
{
    LOG_INFO("Status Report ACK received");
}

bool CloudController::handleOtaUpdateLink(
    const TOtaUpdateLink& otaUpdateLinkStructure) // NOLINT - we don't want to make it static
{
    LOG_INFO("Handling OTA Update Link");

    app::TEventData eventData        = {};
    eventData.otaPerform.updateReady = true;

    pConfig->setOtaUpdateLink(
        otaUpdateLinkStructure); // TODO storing the link in NVS is only temporary solution, finally the link should be
                                 // to OtaControlller class which will be responsible for performing OTA
                                 //(AppController will not take part in this)

    bool result = app::pAppController->addEvent(
        app::EEventType::OTA__PERFORM, app::EEventExecutionType::ASYNCHRONOUS, &eventData);

    // TODO OTA - it might be needed to send some kind of response here - informing the cloud, that ESP32 got the
    // message and is starting the update
    return result;
}

void CloudController::run(void* pObject)
{
    CloudController* pCloudController =
        reinterpret_cast<CloudController*>(pObject); // NOLINT - we need reinterpret cast
    pCloudController->_run();
}

void CloudController::setDeviceStatusTopic(const prot::cloud_set_credentials::TCloudCredentials& credentials)
{
    if (!credentials.isSetCloudDeviceId())
    {
        LOG_ERROR("Cloud device id not provided");
        return;
    }

    char deviceStatusTopic[DEVICE_STATUS_MAX_TOPIC_SIZE];
    memset(deviceStatusTopic, 0, DEVICE_STATUS_MAX_TOPIC_SIZE);

    const char contentTypeJson[] = "$.ct=application%2Fjson%3Bcharset%3Dutf-8";

    sprintf(deviceStatusTopic, "devices/%s/messages/events/%s", credentials.cloudDeviceId, contentTypeJson);

    m_deviceStatusTopic = std::string(deviceStatusTopic);
}

void CloudController::_run()
{
    xSemaphoreTake(m_semaphoreWifiConnectionReady, portMAX_DELAY);

    const ECloudDeviceProvisioningStatus deviceProvisioningStatus = pConfig->getDeviceProvisioningStatus();

    if (deviceProvisioningStatus == ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT)
    {
        m_deviceProvisioningController.runTask();

        xSemaphoreTake(m_semaphoreCredentialsReady, portMAX_DELAY);
    }

    const prot::cloud_set_credentials::TCloudCredentials& cloudCredentials = pConfig->getCloudCredentials();

    configureCloudConnection(cloudCredentials);

    setDeviceStatusTopic(cloudCredentials);

    if (m_connectionStatus == ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED)
    {
        LOG_INFO("Client could not be configured");
        return;
    }

    startCloudConnection();

    if (!(m_mqttClientController.waitUntilMqttConnected(MQTT_CONNECTION_WAIT_TIME_INFINITE)))
    {
        LOG_INFO("Could not connect to cloud, timeout occured");
    }

    m_mqttClientController.runTask();


    // Adding some delay to allow mqttClientController to start
    SLEEP_MS(SLEEP_TIME_BEFORE_STARTING_DEVICE_TWINS_CONTROLLER_MS);
    m_deviceTwinsController.runTask();

    LOG_INFO("Cloud controller initiated!");


    while (true)
    {
        perform();
    }
}

void CloudController::perform()
{
    CheckPressureValueBelowThreshold();
    SLEEP_MS(SLEEP_TIME_BETWEEN_CHECKING_PRESSURE_THRESHOLD_MS);

    if ((commons::getCurrentTimestampMs() - timeLastUpdateDeviceStatusMs) > SLEEP_TIME_BETWEEN_SENDING_MESSAGES_MS)
    {
        LOG_INFO("SLEEP_TIME_BETWEEN_SENDING_MESSAGES reached; calling updateDeviceStatus() function");
        uint32_t current_time = commons::getCurrentTimestampMs();
        uint32_t time_between_two_messages_InMIN = (current_time - timeLastUpdateDeviceStatusMs) / (1000*60); //in minutes
        LOG_INFO("time_between_two_messages: %d min", time_between_two_messages_InMIN);
        updateDeviceStatus();

        timeLastUpdateDeviceStatusMs = commons::getCurrentTimestampMs();

    }
}


void CloudController::CheckPressureValueBelowThreshold()
{
    float avgPressureSensorValue = getAvgPressureSensorValue();
    if (avgPressureSensorValue < PRESSUREALARMTHRESHOLD)
    {
        LOG_INFO("Pressure is below alarm threshold; NOT calling updateDeviceStatus() function");
        if (firstTimePressureAlarmDetected == true)
        {
            LOG_INFO("Detect first time Pressure Alarm (belowPressureThreshold); calling updateDeviceStatus() function");
            isBelowPressureAlarm = true;
            updateDeviceStatus();
            firstTimePressureAlarmDetected = false;
            // LEDTurnRed();
        }
    }
    else
    {
        LOG_INFO("Pressure above pressure alarm threshold");
        firstTimePressureAlarmDetected = true;
        isBelowPressureAlarm           = false;
        // LEDTurnGreen();
    }
}


void CloudController::updateDeviceStatus() // NOLINT - we don't want to make it const
{
    json_parser::TDeviceStatus deviceStatus = {};

    deviceStatus.isWiFiConnected               = app::pAppController->getWiFiController()->getConnectionStatus();
    deviceStatus.isBleConnected                = app::pAppController->getBleController()->isClientConnected();
    deviceStatus.isBelowPressureAlarmThreshold = isBelowPressureAlarm;
    deviceStatus.currentTimeFromStartupMs      = commons::getCurrentTimestampMs();
    deviceStatus.pressureSensorValue           = getAvgPressureSensorValue();
    deviceStatus.flowMeterValue                = app::pAppController->getPulseCounterHandler()->getTotalFlowInLitres();

    strcpy(
        deviceStatus.currentLocalTime,
        app::pAppController->getNtpClient()->getCurrentLocalTimeString(LOCAL_TIME_OFFSET));

    strcpy(deviceStatus.firmwareVersion, PROJECT_VER);

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatus, m_msgCounter);

    m_msgCounter++;
    m_mqttClientController.sendMessage(m_deviceStatusTopic, deviceStatusMessage);
}

void CloudController::setConnectionStatus(ECloudConnectionStatus status)
{
    m_connectionStatus = status;
}

void CloudController::configureCloudConnection(const prot::cloud_set_credentials::TCloudCredentials& credentials)
{
    if (!(m_mqttClientController.init(credentials)))
    {
        LOG_INFO("Could not init MqttClientController");
        m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED;
    }
    else
    {
        m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_DISABLED;
    }
}

void CloudController::setReadinessToConnect()
{
    xSemaphoreGive(m_semaphoreWifiConnectionReady);
}

void CloudController::setReadinessAfterDeviceProvisioning()
{
    xSemaphoreGive(m_semaphoreCredentialsReady);
}

void CloudController::setConnectedStatus()
{
    m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_CONNECTED;
    app::pAppController->addEvent(
        app::EEventType::CLOUD_CONTROLLER__CONNECTION_ESTABLISHED, app::EEventExecutionType::ASYNCHRONOUS, nullptr);
}

void CloudController::setNotConnectedStatus()
{
    m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED;
    app::pAppController->addEvent(
        app::EEventType::CLOUD_CONTROLLER__CONNECTION_LOST, app::EEventExecutionType::ASYNCHRONOUS, nullptr);
}

const esp_mqtt_client_handle_t& CloudController::getMqttClientHandle()
{
    return m_mqttClientController.getMqttClient();
}

void CloudController::startCloudConnection()
{
    if (m_mqttClientController.start())
    {
        m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED;
    }
    else
    {
        LOG_INFO("Error while starting mqttClientController, could not connect to the cloud");
        m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_DISABLED;
    }
}
