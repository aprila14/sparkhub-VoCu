// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "CloudController";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "cloud_controller.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "ntp_client.h"
#include "protocol_types.h"
#include "sleep.h"
#include "cloud_config.h"
#include "adc_VoCu.h"

#include <memory>

namespace
{
    constexpr uint32_t SLEEP_TIME_BETWEEN_SENDING_MESSAGES = 10 * 1000; // every 10 seconds
    //constexpr uint32_t SLEEP_TIME_BETWEEN_READADCVALUE_MESSAGES = 10; // every 10 ms
    constexpr uint16_t LOCAL_TIME_OFFSET = UtcOffset::OFFSET_UTC_2;
    constexpr int8_t MQTT_CONNECTION_WAIT_TIME_INFINITE = -1;
    constexpr uint16_t HEARTBEAT_CHECK_TIMER_PERIOD_MS = 1000;
    constexpr uint16_t SAMPLING_FREQUENCY = 100; // 100ms (10Hz)

    void _heartbeatWatchdogTimerCallback(TimerHandle_t timerHandle)
    {
        CloudController *cloudController = static_cast<CloudController *>(pvTimerGetTimerID(timerHandle));
        assert(cloudController);
        cloudController->heartbeatWatchdogTimerCallback();
    }
} // unnamed namespace

CloudController::CloudController()
    : m_taskHandle(),
      m_heartbeatWatchdogTimer(),
      m_heartbeatWatchdogCounter(0),
      m_msgCounter(0),
      m_connectionStatus(ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED),
      m_mqttClientController(this)
{
    m_semaphoreCredentialsReady = xSemaphoreCreateBinary();
}

void CloudController::runTask()
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

ECloudConnectionStatus CloudController::getConnectionStatus() const
{
    return m_connectionStatus;
}

void CloudController::handleStatusReportResponse(bool ACK)
{
    LOG_INFO("Status Report ACK received");
}

void CloudController::handleHeartbeatResponse()
{
    LOG_INFO("Heartbeat ACK received");
    m_heartbeatWatchdogCounter = 0;
}

bool CloudController::handleOtaUpdateLink(const TOtaUpdateLink &otaUpdateLinkStructure) // NOLINT - we don't want to make it static
{
    LOG_INFO("Handling OTA Update Link");

    app::TEventData eventData = {};
    eventData.otaPerform.updateReady = true;

    pConfig->setOtaUpdateLink(otaUpdateLinkStructure); // TODO storing the link in NVS is only temporary solution, finally the link should be
                                                       // to OtaControlller class which will be responsible for performing OTA
                                                       //(AppController will not take part in this)

    bool result = app::pAppController->addEvent(app::EEventType::OTA__PERFORM, app::EEventExecutionType::ASYNCHRONOUS, &eventData);

    // TODO OTA - it might be needed to send some kind of response here - informing the cloud, that ESP32 got the message and is starting the update
    return result;
}

bool CloudController::handleTimeSlotsList(const json_parser::TTimeSlotsList &timeSlotsListStructure) // NOLINT - we don't want to make it static
{
    LOG_INFO("Handling Time Slots List message");

    // TODO add an event informing AppController that TimeSlotsList has been updated

    return true;
}

void CloudController::run(void *pObject)
{
    CloudController *pCloudController = reinterpret_cast<CloudController *>(pObject); // NOLINT - we need reinterpret cast
    pCloudController->_run();
}

void CloudController::_run()
{
    xSemaphoreTake(m_semaphoreCredentialsReady, portMAX_DELAY);
    const prot::cloud_set_credentials::TCloudCredentials &credentials = pConfig->getCloudCredentials();

    configureCloudConnection(credentials);

    m_deviceStatusTopic = std::string(DEFAULT_TELEMETRY_MQTT_TOPIC);
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

    m_heartbeatWatchdogTimer = xTimerCreate("Heartbeat Watchdog Timer", pdMS_TO_TICKS(HEARTBEAT_CHECK_TIMER_PERIOD_MS), true, static_cast<void *>(this), _heartbeatWatchdogTimerCallback);
    xTimerStart(m_heartbeatWatchdogTimer, 0);
    m_mqttClientController.runTask();


    constexpr uint32_t TIME_BETWEEN_SENDING_MESSAGES = 5 * 1000; // every 5 seconds
    uint32_t TIME_COUNTER = 0; // 

    while (true)
    {
        //perform();
        listenToADCInput();
        //LOG_INFO("TIME_BETWEEN_SENDING_MESSAGES %lu", TIME_BETWEEN_SENDING_MESSAGES);
        //LOG_INFO("TIME_COUNTER %lu", TIME_COUNTER);
        if(TIME_BETWEEN_SENDING_MESSAGES < TIME_COUNTER)
        {
            //LOG_INFO("inside listenToADCInput if loop");
            perform();
            TIME_COUNTER = 0;
        }

        TIME_COUNTER = TIME_COUNTER + 1;
        LOG_INFO("TIME_COUNTER %lu", TIME_COUNTER);
        SLEEP_MS(SAMPLING_FREQUENCY);

    }
}

void CloudController::perform()
{
    updateDeviceStatus();
    //SLEEP_MS(SLEEP_TIME_BETWEEN_SENDING_MESSAGES);
}

void CloudController::listenToADCInput()
{
    updateTotalSumOfLiters();
    //SLEEP_MS(SLEEP_TIME_BETWEEN_READADCVALUE_MESSAGES);
}

void CloudController::updateTotalSumOfLiters()
{
    ExecuteUpdateTotalSumOfLiters();
}



void CloudController::updateDeviceStatus() // NOLINT - we don't want to make it const
{
    json_parser::TDeviceStatus deviceStatus = {};
    deviceStatus.isWiFiConnected = app::pAppController->getWiFiController()->getConnectionStatus();
    deviceStatus.isBleConnected = app::pAppController->getBleController()->isClientConnected();
    deviceStatus.currentTimeFromStartupMs = commons::getCurrentTimestampMs();
    deviceStatus.totalSumOfLiters = getTotalSumOfLiters();

    strcpy(deviceStatus.currentLocalTime, app::pAppController->getNtpClient()->getCurrentLocalTimeString(LOCAL_TIME_OFFSET));
    strcpy(deviceStatus.firmwareVersion, PROJECT_VER);

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatus, m_msgCounter);

    m_msgCounter++;
    m_mqttClientController.sendMessage(m_deviceStatusTopic, deviceStatusMessage);
}

void CloudController::setConnectionStatus(ECloudConnectionStatus status)
{
    m_connectionStatus = status;
}

void CloudController::configureCloudConnection(const prot::cloud_set_credentials::TCloudCredentials &credentials)
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
    xSemaphoreGive(m_semaphoreCredentialsReady);
}

void CloudController::setConnectedStatus()
{
    m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_CONNECTED;
    app::pAppController->addEvent(app::EEventType::CLOUD_CONTROLLER__CONNECTION_ESTABLISHED,
                                  app::EEventExecutionType::ASYNCHRONOUS,
                                  nullptr);
}

void CloudController::setNotConnectedStatus()
{
    m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED;
    app::pAppController->addEvent(app::EEventType::CLOUD_CONTROLLER__CONNECTION_LOST,
                                  app::EEventExecutionType::ASYNCHRONOUS,
                                  nullptr);
}

const esp_mqtt_client_handle_t &CloudController::getMqttClientHandle()
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

void CloudController::heartbeatWatchdogTimerCallback()
{
    m_heartbeatWatchdogCounter++;
    if (m_heartbeatWatchdogCounter >= 180)
    {
        setConnectionStatus(ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED);
    }
}
