// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "AppController";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "app_controller.h"

#include "commons.h"
#include "config_nvs.h"
#include "sleep.h"

#include "esp_http_client.h"
#include "esp_https_ota.h"

namespace
{

constexpr uint8_t  EVENTS_QUEUE_SIZE             = 10;
constexpr uint32_t EVENT_QUEUE_ADDING_TIMEOUT_MS = 1000; // some arbitraty value to maintain responsiveness

} // namespace

namespace app
{

AppController* pAppController = nullptr;

AppController::AppController(
    WiFiController*      pwifiController,
    BleController*       pBleController,
    CloudController*     pCloudController,
    NtpClient*           pNtpClient,
    PulseCounterHandler* pPulseCounterHandler) :
    m_eventsQueue(nullptr),
    m_taskHandle(nullptr),
    m_pWifiController(pwifiController),
    m_pBleController(pBleController),
    m_pCloudController(pCloudController),
    m_pNtpClient(pNtpClient),
    m_pPulseCounterHandler(pPulseCounterHandler)
{
    m_eventsQueue = xQueueCreate(EVENTS_QUEUE_SIZE, sizeof(TEventControl));
    assert(m_eventsQueue != nullptr);
}

void AppController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned =
        xTaskCreate(AppController::run, LOG_TAG, DEFAULT_HUGE_STACK_SIZE, this, DEFAULT_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void AppController::run(void* pObject)
{
    AppController* pAppController = reinterpret_cast<AppController*>(pObject); // NOLINT
    pAppController->_run();
}

void AppController::_run()
{
    while (true)
    {
        TEventControl eventControl = {};
        eventControl.type          = EEventType::UNKNOWN_EVENT;

        if (xQueueReceive(m_eventsQueue, &eventControl, portMAX_DELAY) == pdTRUE)
        {
            executeEvent(eventControl);
        }

        SLEEP_MS(10);
    }
}

bool AppController::addEvent(EEventType eventType, EEventExecutionType executionType, TEventData* pData)
{
    LOG_DEBUG(
        "About to add event '%d', execution type '%d'", static_cast<int>(eventType), static_cast<int>(executionType));

    TEventControl eventControl = {
        .type             = eventType,
        .data             = {},
        .semaphore        = nullptr,
        .pExecutionResult = nullptr,
    };

    if (pData != nullptr)
    {
        eventControl.data = *pData;
    }

    bool executionResult =
        false; // optional execution result - only for synchronous events - TODO - do we need to have it "volatile"?
    if (executionType == EEventExecutionType::SYNCHRONOUS)
    {
        eventControl.semaphore        = xSemaphoreCreateBinary();
        eventControl.pExecutionResult = &executionResult;
        assert(eventControl.semaphore != nullptr);
    }
    // we don't need a synchronization semaphore for an asynchronous event

    // add event to queue - "You do not need a mutex to protect accessing a queue" ~ Internet
    if (xQueueSend(m_eventsQueue, &eventControl, pdMS_TO_TICKS(EVENT_QUEUE_ADDING_TIMEOUT_MS)) != pdPASS)
    {
        LOG_ERROR(
            "Failed to add event '%d', execution type '%d'",
            static_cast<int>(eventType),
            static_cast<int>(executionType));
        return false;
    }

    LOG_DEBUG("Event succesfully added to queue!");

    if (executionType == EEventExecutionType::SYNCHRONOUS)
    {
        LOG_DEBUG("About to wait for event execution completion...");
        assert(eventControl.semaphore != nullptr);

        xSemaphoreTake(eventControl.semaphore, portMAX_DELAY);
        vSemaphoreDelete(eventControl.semaphore);

        LOG_INFO(
            "Finished executing event type '%d', execution type '%d', result '%d'",
            static_cast<int>(eventType),
            static_cast<int>(executionType),
            executionResult);
        return executionResult;
    }

    return true;
}

WiFiController* AppController::getWiFiController() // NOLINT - no need to have it const
{
    return m_pWifiController;
}

BleController* AppController::getBleController() // NOLINT - no need to have it const
{
    return m_pBleController;
}

CloudController* AppController::getCloudController() const
{
    return m_pCloudController;
}

NtpClient* AppController::getNtpClient() const
{
    return m_pNtpClient;
}

bool AppController::executeEvent(AppController::TEventControl& eventControl)
{
    bool result = false;

    switch (eventControl.type)
    {
        case EEventType::UNKNOWN_EVENT:
            LOG_ERROR("Cannot execute UNKNOWN_EVENT!");
            break;

        case EEventType::DUMMY_EVENT:
            result = executeEvent_dummy(eventControl.data.dummy);
            break;

        case EEventType::PERFORM_DEVICE_RESTART:
            result = executeEvent_performDeviceRestart();
            break;

        case EEventType::WIFI_CONTROLLER__DISCONNECT:
            result = executeEvent_wifiControllerDisconnect();
            break;
        case EEventType::WIFI_CONTROLLER__CONNECTION_ESTABLISHED:
            result = executeEvent_wifiControllerConnectionEstablished();
            break;

        case EEventType::WIFI_CONTROLLER__CONNECTION_LOST:
            result = executeEvent_wifiControllerConnectionLost();
            break;

        case EEventType::WIFI_CONTROLLER__WAIT_UNTIL_CONNECTED:
            result = executeEvent_wifiControllerWaitUntilConnected();
            break;

        case EEventType::CLOUD_CONTROLLER__CONNECTION_ESTABLISHED:
            result = executeEvent_cloudControllerConnectionEstablished();
            break;

        case EEventType::CLOUD_CONTROLLER__CONNECTION_LOST:
            result = executeEvent_cloudControllerConnectionLost();
            break;
        case EEventType::CLOUD_CONTROLLER__SEND_CREDENTIALS:
            result = executeEvent_cloudControllerSetCredentials();
            break;
        case EEventType::OTA__PERFORM:
            result = executeEvent_otaPerform();
            break;
        case EEventType::CALIBRATE_FLOW_METER:
            result = executeEvent_calibrateFlowMeter(eventControl.data.flowMeterCalibrationValue);
            break;
        default:
            assert(0); // all cases handled above
            break;
    }

    LOG_DEBUG("Event %d executed with result %d", static_cast<int>(eventControl.type), static_cast<int>(result));

    if (eventControl.pExecutionResult != nullptr)
    {
        *eventControl.pExecutionResult = result;
    }

    if (eventControl.semaphore != nullptr)
    {
        xSemaphoreGive(eventControl.semaphore);
    }

    return result;
}

bool AppController::executeEvent_dummy(const TDummyEventData& eventData)
{
    LOG_INFO("executeEvent_dummy...");
    LOG_INFO("Dummy event data: %d, %d", eventData.dummyByte1, eventData.dummyByte2);
    return true;
}

bool AppController::executeEvent_performDeviceRestart()
{
    LOG_WARNING("executeEvent_performDeviceRestart...");
    SLEEP_MS(3000); // give some time for logs to flush...
    esp_restart();

    return true;
}

bool AppController::executeEvent_wifiControllerDisconnect() const
{
    return m_pWifiController->disconnectFromAccessPoint();
}

bool AppController::executeEvent_wifiControllerConnectionEstablished() // NOLINT - no need to have it const
{
    LOG_INFO("AppController: Connection established");

    TAsynchronousEventData asynchronousEventData           = {};
    asynchronousEventData.wiFiConnectedToApEvent.dummyByte = 1; // TODO consider sending e.g. IP instead of a dummy byte
    m_pBleController->addAsynchronousEvent(EAsynchronousEventType::WIFI_CONNECTED_TO_AP, &asynchronousEventData);

    LOG_INFO("Initiating connection");
    m_pCloudController->setReadinessToConnect();

    return true;
}

bool AppController::executeEvent_wifiControllerConnectionLost() // NOLINT - no need to have it const
{
    LOG_INFO("AppController: Connection lost");

    TAsynchronousEventData asynchronousEventData = {};
    asynchronousEventData.wiFiDicsonnectedFromApEvent.dummyByte =
        1; // TODO consider sending e.g. IP instead of a dummy byte
    m_pBleController->addAsynchronousEvent(EAsynchronousEventType::WIFI_DISCONNECTED_FROM_AP, &asynchronousEventData);

    return true;
}

bool AppController::executeEvent_wifiControllerWaitUntilConnected() const
{
    return m_pWifiController->waitUntilConnectedToWifi();
}

bool AppController::executeEvent_cloudControllerConnectionEstablished() const
{
    LOG_INFO("AppController: Cloud connection established");

    TAsynchronousEventData asynchronousEventData        = {};
    asynchronousEventData.cloudConnectedEvent.dummyByte = 1;
    m_pBleController->addAsynchronousEvent(EAsynchronousEventType::CLOUD_CONNECTED, &asynchronousEventData);
    return true;
}

bool AppController::executeEvent_cloudControllerConnectionLost() const
{
    LOG_INFO("AppController: Cloud connection lost");

    TAsynchronousEventData asynchronousEventData        = {};
    asynchronousEventData.cloudConnectedEvent.dummyByte = 1;
    m_pBleController->addAsynchronousEvent(EAsynchronousEventType::CLOUD_DISCONNECTED, &asynchronousEventData);

    return true;
}

bool AppController::executeEvent_cloudControllerSetCredentials() const // NOLINT - we don't want to make it static
{
    app::pAppController->getCloudController()->setReadinessToConnect();
    return true;
}

bool AppController::executeEvent_otaPerform() // NOLINT - we don't want to make it static
{
    LOG_INFO("Handling OTA_PERFORM event");

    esp_http_client_config_t config = {};

    // TODO: add reading firmwareLink from NVS here

    const prot::ota_perform::TOtaUpdateLink& otaUpdateLink = pConfig->getOtaUpdateLink();

    if (!otaUpdateLink.isSet())
    {
        LOG_INFO("Ota update link not set in NVS");
        return false;
    }

    config.url                         = otaUpdateLink.firmwareLink;
    config.skip_cert_common_name_check = true;

    commons::printAvailableHeapMemory(__LINE__, __FILE__, "executeEvent_otaPerform");

    esp_err_t errorCode = esp_https_ota(&config);
    if (errorCode == ESP_OK)
    {
        return true;
    }
    else
    {
        return false;
    }


    return true;
}

bool AppController::executeEvent_calibrateFlowMeter(const float flowMeterCalibrationValue)
{
    LOG_INFO("Perform flow meter calibration");

    m_pPulseCounterHandler->setFlowMeterCalibrationValue(flowMeterCalibrationValue);

    pConfig->setFlowMeterCalibrationValue(flowMeterCalibrationValue);

    return true;
}

} // namespace app
