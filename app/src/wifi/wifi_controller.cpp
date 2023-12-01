// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "WiFiController";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "wifi_controller.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "custom_types.h"
#include "protocol_control.h"
#include "sleep.h"

#include "esp_err.h"
#include "esp_netif_types.h"
#include "esp_wifi_types.h"

#include <string>

namespace
{
    const uint8_t EVENT_QUEUE_SIZE = 15;

    constexpr uint8_t WIFI_MAX_RETRY = 5;
    constexpr uint8_t MAX_QUICK_RECONNECT_ATTEMPTS = 3;

    constexpr int32_t SLEEP_TIME_MS = 100;
    constexpr int32_t DISCONNECT_TIMEOUT_MS = 3000;
    constexpr int32_t SCAN_TIMEOUT_MS = 10000;
    constexpr int32_t SHORT_TIMEOUT_MS = 4000;
    constexpr int32_t RECONNECTION_INTERVAL_MS = 5000;
    constexpr int32_t HANDLERS_RECEIVE_DELAY_MS = 400;

    constexpr uint32_t EVENT_BIT_WIFI_CREDENTIALS_READY = BIT0;
    constexpr uint32_t EVENT_BIT_WIFI_STARTED = BIT1;
    constexpr uint32_t EVENT_BIT_WIFI_CONNECTED = BIT2;
    constexpr uint32_t EVENT_BIT_WIFI_DISCONNECTED = BIT3;
    constexpr uint32_t EVENT_BIT_WIFI_STOPPED = BIT4;
    constexpr uint32_t EVENT_BIT_WIFI_SCAN_RESULTS_READY = BIT5;

    // this event enables to preempt constant reconnection to AP by a public function
    constexpr uint32_t EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING = BIT6;

    constexpr uint32_t EVENT_BIT_WIFI_IP_ASSIGNED = BIT7;

    constexpr bool SHOULD_RECONNECT = true;      // just a constant for passing as a parameter
    constexpr bool SHOULD_NOT_RECONNECT = false; // just a constant for passing as a parameter

    void _wifiEventHandler(void *eventHandlerArg, esp_event_base_t eventBase, int32_t eventId, void *eventData)
    {
        WiFiController *pWifiController = reinterpret_cast<WiFiController *>(eventHandlerArg); // NOLINT
        pWifiController->wifiEventHandler(pWifiController, eventBase, eventId, eventData);
    }
} // unnamed namespace

WiFiController::WiFiController()
    : m_taskHandle(),
      m_connectionErrorReason(EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER),
      m_connectionStatus(false),
      m_disconnectionInitiated(false),
      m_shouldReconnect(false),
      m_scanningActive(false),
      m_maxRecordsFound(MAX_NUMBER_OF_SCANNED_ACCESS_POINTS),
      m_retryNumber(0),
      m_timeoutMs(0),
      m_disconnectionTime(0),
      m_lastConnectionErrorReason(EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR),
      m_wifiScanRecords{},
      m_scanErrorCode(0)
{
    m_publicAccessMutex = mutexCreate();
    m_lastConnectionErrorReasonMutex = mutexCreate();

    m_eventsQueue = xQueueCreate(EVENT_QUEUE_SIZE, sizeof(TWifiEvent));
    assert(m_eventsQueue != nullptr);

    m_eventGroup = xEventGroupCreate();
    assert(m_eventGroup != nullptr);
}

void WiFiController::runTask()
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

void WiFiController::_run()
{
    while (true)
    {
        perform();
    }
}

void WiFiController::run(void *pObject)
{
    WiFiController *pWifiController = reinterpret_cast<WiFiController *>(pObject); // NOLINT
    pWifiController->_run();
}

bool WiFiController::loadCredentialsFromConfigNvsAndConnectIfSet()
{
    const TWiFiCredentials &wifiCredentials = pConfig->getWifiCredentials();
    if (!wifiCredentials.isSet())
    {
        LOG_INFO("WiFi credentials not set in NVS, so nothing to connect to!");
        return false;
    }

    TConnectToAccessPointCommand command =
        {
            .credentials = wifiCredentials,
            .timeoutMs = 0, // so it will return immediately, but keep connecting in the background
        };

    TConnectToAccessPointResult result = {};
    connectToAccessPoint(command, result, SHOULD_RECONNECT);

    return true;
}

bool WiFiController::init()
{
    m_publicAccessMutex = mutexCreate();

    if (esp_netif_init() != ESP_OK)
    {
        LOG_ERROR("Error while exectuing esp_netif_init()");
        return false;
    }

    if (esp_event_loop_create_default() != ESP_OK)
    {
        LOG_ERROR("Error while executing esp_event_loop_create_default()");
        return false;
    }

    esp_netif_create_default_wifi_sta(); // TODO: consider storing returned esp_netif_t pointer as class member

    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&wifiInitConfig) != ESP_OK)
    {
        LOG_ERROR("Error while executing esp_wifi_init(&wifiInitConfig)");
        return false;
    }

    if (!registerHandlers())
    {
        LOG_ERROR("Error while registering wifiEventHandler");
        return false;
    }

    xEventGroupSetBits(m_eventGroup,
                       EVENT_BIT_WIFI_STOPPED |
                           EVENT_BIT_WIFI_DISCONNECTED);
    xEventGroupClearBits(m_eventGroup,
                         EVENT_BIT_WIFI_CREDENTIALS_READY |
                             EVENT_BIT_WIFI_CONNECTED |
                             EVENT_BIT_WIFI_STARTED |
                             EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING |
                             EVENT_BIT_WIFI_IP_ASSIGNED);

    return true;
}

void WiFiController::perform()
{
#if IS_DEBUG_BUILD
    printConnectionLoop();
#endif
    processWifiEvents();
    SLEEP_MS(SLEEP_TIME_MS);
}

bool WiFiController::wifiConfigModeInit(TWiFiCredentials &credentials)
{
    wifi_config_t wifiConfig = {};

    int ssidLength = strlen(credentials.ssid);
    int passwordLength = strlen(credentials.password);

    strcpy(reinterpret_cast<char *>(wifiConfig.sta.ssid), credentials.ssid);         // NOLINT
    strcpy(reinterpret_cast<char *>(wifiConfig.sta.password), credentials.password); // NOLINT

    if (credentials.authorizationMode == device_wifi_auth_mode_t::DEVICE_WIFI_AUTH_NOT_SET)
    {
        if (passwordLength > 0)
        {
            LOG_INFO("DEVICE_WIFI_AUTH_NOT_SET - and password set - use authorization WIFI_AUTH_WPA2_PSK");
            wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        }
        else
        {
            LOG_INFO("DEVICE_WIFI_AUTH_NOT_SET - and password empty - use authorization WIFI_AUTH_OPEN");
            wifiConfig.sta.threshold.authmode = WIFI_AUTH_OPEN;
        }
    }
    else
    {
        wifiConfig.sta.threshold.authmode = static_cast<wifi_auth_mode_t>(credentials.authorizationMode);
    }

    wifiConfig.sta.pmf_cfg.capable = true;

    // required field shall be set to 'true' only in case of WPA3 authorization mode
    wifiConfig.sta.pmf_cfg.required = (credentials.authorizationMode == device_wifi_auth_mode_t::DEVICE_WIFI_AUTH_WPA3_PSK ||
                                       credentials.authorizationMode == device_wifi_auth_mode_t::DEVICE_WIFI_AUTH_WPA2_WPA3_PSK);

    LOG_INFO("Configured WiFi with SSID: %s", credentials.ssid);
    if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK)
    {
        LOG_ERROR("Error while executing esp_wifi_setmode(WIFI_MODE_STA)");
        return false;
    }

    if (esp_wifi_set_config(WIFI_IF_STA, &wifiConfig) != ESP_OK)
    {
        LOG_ERROR("Error while executing esp_wifi_set_config(WIFI_IF_STA)");
        return false;
    }

    return true;
}

bool WiFiController::registerHandlers()
{
    if (esp_event_handler_instance_register(WIFI_EVENT,
                                            ESP_EVENT_ANY_ID,
                                            _wifiEventHandler,
                                            this, NULL) != ESP_OK)
    {
        LOG_ERROR("Error while registering wifiEventHandler for WIFI_EVENT");
        return false;
    }

    if (esp_event_handler_instance_register(IP_EVENT,
                                            IP_EVENT_STA_GOT_IP,
                                            _wifiEventHandler,
                                            this, NULL) != ESP_OK)
    {
        LOG_ERROR("Error while registering wifiEventHandler for IP_EVENT");
        return false;
    }

    return true;
}

//************************ EVENTS PROCESSING ******************************//

void WiFiController::wifiEventHandler(void *eventHandlerArg, esp_event_base_t eventBase, int32_t eventId, void *eventData)
{
    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_UNKNOWN,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = {},
        };
    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_STA_START;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_CONNECTED)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_CONNECTED;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconnectionEvent = reinterpret_cast<wifi_event_sta_disconnected_t *>(eventData); // NOLINT
        event.eventType = EWifiEvent::WIFI__EVENT_DISCONNECTED;
        m_lastConnectionErrorReason = parseWifiDisconnectReason(disconnectionEvent->reason);
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_BEACON_TIMEOUT)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_STA_BEACON_TIMEOUT;
        event.reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_STOP)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_STA_STOP;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_SCAN_DONE)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_SCAN_DONE;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP)
    {
        event.eventType = EWifiEvent::WIFI__EVENT_IP_ASSIGNED;
        xQueueSend(m_eventsQueue, &event, portMAX_DELAY);
    }
    else
    {
        LOG_INFO("Another event %d", eventId);
    }
}

void WiFiController::processWifiEvents()
{
    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_UNKNOWN,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = {},
        };

    if (xQueueReceive(m_eventsQueue, &event, MS_TO_TICKS(HANDLERS_RECEIVE_DELAY_MS)) == pdTRUE)
    {
        processWifiEvent(event);
    }
}

void WiFiController::processWifiEvent(TWifiEvent &event)
{
    // execute event here

    switch (event.eventType)
    {
    case EWifiEvent::WIFI__EVENT_INITIATE_CONNECTION:
        LOG_INFO("Connection initiated");

        if (esp_wifi_start() != ESP_OK)
        {
            LOG_ERROR("Error while executing esp_wifi_start()");
        }
        break;
    case EWifiEvent::WIFI__EVENT_INITIATE_CREDENTIALS:
        // get wifi credentials from eventData, fill in the structure and
        if (wifiConfigModeInit(event.credentials))
        {
            xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_CREDENTIALS_READY);
        }
        break;

    case EWifiEvent::WIFI__EVENT_INITIATE_SCAN:
        if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK)
        {
            LOG_INFO("Error with setting mode to WIFI_MODE_STA in Wifi Scan initalization");
        }
        if (esp_wifi_start() != ESP_OK)
        {
            LOG_INFO("Wifi could not be started for scan");
        }
        break;
    case EWifiEvent::WIFI__EVENT_INITIATE_DISCONNECTION:
        setDisconnectionInitiated(true);
        if (esp_wifi_disconnect() != ESP_OK)
        {
            LOG_INFO("Failure to disconnect from access point");
            esp_wifi_stop();
            setDisconnectionInitiated(false);
        }
        break;
    case EWifiEvent::WIFI__EVENT_INITIATE_STOP:
        if (esp_wifi_stop() != ESP_OK)
        {
            LOG_INFO("Failure to stop WiFi");
        }
        break;
    case EWifiEvent::WIFI__EVENT_STA_START:
    {
        xEventGroupClearBits(m_eventGroup, EVENT_BIT_WIFI_STOPPED);
        if (m_scanningActive)
        {
            wifi_scan_config_t wifiScanConfig = {};
            wifiScanConfig.ssid = nullptr;
            wifiScanConfig.bssid = nullptr;
            wifiScanConfig.channel = 0;
            wifiScanConfig.show_hidden = true;

            if (esp_wifi_scan_start(&wifiScanConfig, true) != ESP_OK)
            {
                LOG_INFO("Error occured when performing AP scan");
            }
            break;
        }
        xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_STARTED);
        xEventGroupClearBits(m_eventGroup, EVENT_BIT_WIFI_STOPPED);
        esp_err_t connectError = esp_wifi_connect();
        if (connectError == ESP_ERR_WIFI_CONN)
        {
            LOG_ERROR("ESP_ERR_WIFI_CONN");
            esp_wifi_stop();
        }
        else if (connectError == ESP_ERR_WIFI_SSID)
        {
            LOG_ERROR("ESP_ERR_WIFI_SSID, check if correct SSID was given");
            esp_wifi_stop();
        }
        else if (connectError == ESP_OK)
        {
            LOG_INFO("Connection initiated correctly");
        }
        else
        {
            LOG_ERROR("Error with connection: %d", connectError);
            esp_wifi_stop();
        }
        break;
    }
    case EWifiEvent::WIFI__EVENT_SCAN_DONE:
        LOG_INFO("Starting scan");
        if (esp_wifi_scan_get_ap_records(&m_maxRecordsFound, m_wifiScanRecords) == ESP_OK)
        {
            LOG_INFO("Found %d records", m_maxRecordsFound);
            xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_SCAN_RESULTS_READY);
        }
        else
        {
            LOG_INFO("Error occured while retrieving scan records");
        }
        break;
    case EWifiEvent::WIFI__EVENT_CONNECTED:
    {
        LOG_INFO("Connected to SSID");
        m_retryNumber = 0;
        m_disconnectionTime = 0;
        m_lastConnectionErrorReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR;
        xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_CONNECTED);
        xEventGroupClearBits(m_eventGroup, EVENT_BIT_WIFI_DISCONNECTED);
        setConnectionStatus(true);
        break;
    }
    case EWifiEvent::WIFI__EVENT_DISCONNECTED:
    {
        LOG_INFO("Disconnected");
        m_retryNumber++;

        if (getConnectionStatus())
        {
            app::pAppController->addEvent(app::EEventType::WIFI_CONTROLLER__CONNECTION_LOST,
                                          app::EEventExecutionType::ASYNCHRONOUS,
                                          nullptr);
        }

        setConnectionStatus(false);

        xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_DISCONNECTED);
        xEventGroupClearBits(m_eventGroup, EVENT_BIT_WIFI_CONNECTED | EVENT_BIT_WIFI_CREDENTIALS_READY);

        if ((m_retryNumber < MAX_QUICK_RECONNECT_ATTEMPTS) && !m_disconnectionInitiated)
        {
            esp_wifi_connect();
            break;
        }

        tryToReconnect();

        break;
    }
    case EWifiEvent::WIFI__EVENT_FAIL:
    {
        LOG_ERROR("Failure to connect to SSID");
        break;
    }
    case EWifiEvent::WIFI__EVENT_IP_ASSIGNED:
    {
        LOG_INFO("IP Address assigned");
        xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_IP_ASSIGNED);
        app::pAppController->addEvent(app::EEventType::WIFI_CONTROLLER__CONNECTION_ESTABLISHED,
                                      app::EEventExecutionType::ASYNCHRONOUS,
                                      nullptr);
        break;
    }
    case EWifiEvent::WIFI__EVENT_STA_BEACON_TIMEOUT:
    {
        LOG_INFO("WiFi Beacon Timeout occured");
        break;
    }
    case EWifiEvent::WIFI__EVENT_STA_STOP:
    {
        LOG_INFO("WiFi module stopped");
        xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_STOPPED);
        break;
    }
    case EWifiEvent::WIFI__EVENT_UNKNOWN:
    {
        LOG_INFO("Unknown event");
        break;
    }
    default:
    {
        LOG_INFO("Undeclared event");
        break;
    }
    }
}

EConnectToSsidErrorReason WiFiController::parseWifiDisconnectReason(uint8_t reason)
{
    EConnectToSsidErrorReason outputReason;
    switch (reason)
    {
    case WIFI_REASON_UNSPECIFIED:
        LOG_INFO("Connection error, reason: UNSPECIFIED (internal failure)");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_UNSPECIFIED;
        break;
    case WIFI_REASON_AUTH_EXPIRE:
        LOG_INFO("Connection error, reason: AUTH_EXPIRE");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_AUTH_EXPIRE;
        break;
    case WIFI_REASON_AUTH_LEAVE:
        LOG_INFO("Connection error, reason: AUTH_LEAVE");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_AUTH_LEAVE;
        break;
    case WIFI_REASON_ASSOC_EXPIRE:
        LOG_INFO("Connection error, reason: ASSOC_EXPIRE");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_ASSOC_EXPIRE;
        break;
    case WIFI_REASON_ASSOC_TOOMANY:
        LOG_INFO("Connection error, reason: ASSOC_TOOMANY");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_ASSOC_TOOMANY;
        break;
    case WIFI_REASON_ASSOC_LEAVE:
        LOG_INFO("Station disconnected");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_ASSOC_LEAVE;
        break;
    case WIFI_REASON_MIC_FAILURE:
        LOG_INFO("Connection error, reason: MIC_FAILURE (Message integrity code failure)");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_MIC_FAILURE;
        break;
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        LOG_INFO("Reason: 4WAY_HANDSHAKE_TIMEOUT, probably wrong password");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT;
        break;
    case WIFI_REASON_NO_AP_FOUND:
        LOG_INFO("Reason: NO_AP_FOUND (No Access Point found), wrong ssid/authorization method");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_AP_FOUND;
        break;
    case WIFI_REASON_AUTH_FAIL:
        LOG_INFO("Reason: AUTH_FAIL (Other authenthication issue)");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_AUTH_FAIL;
        break;
    case WIFI_REASON_ASSOC_FAIL:
        LOG_INFO("Reason: ASSOC_FAIL (Other association failure)");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_ASSOC_FAIL;
        break;
    case WIFI_REASON_CONNECTION_FAIL:
        LOG_INFO("Reason: CONNECTION_FAIL (Connection failed with Access Point)");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_CONNECTION_FAIL;
        break;
    case WIFI_REASON_BEACON_TIMEOUT:
        LOG_INFO("Reason: BEACON_TIMEOUT, connection lost with Access Point");
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_BEACON_TIMEOUT;
        break;
    default:
        LOG_INFO("Another reason for disconnection, check error code: %d", reason);
        outputReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER;
    }
    return outputReason;
}

//************************ CONNECTION/DISCONNECTION METHODS ******************************//

void WiFiController::tryToReconnect()
{
    if (m_shouldReconnect && !m_disconnectionInitiated)
    {
        if (!(xEventGroupWaitBits(m_eventGroup, EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING, pdTRUE, pdFALSE, MS_TO_TICKS(RECONNECTION_INTERVAL_MS)) & EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING))
        {
            esp_err_t errorCode = esp_wifi_connect();
            printErrorCode(errorCode, "Could not connect to Access Point");
        }
        else
        {
            esp_err_t errorCode = esp_wifi_stop();
            printErrorCode(errorCode, "Could not connect to Access Point");
        }
    }
    else
    {
        LOG_INFO("Stopping WiFi");
        esp_err_t errorCode = esp_wifi_stop();
        printErrorCode(errorCode, "WiFi could not be stopped");
        setDisconnectionInitiated(false);
    }
}

bool WiFiController::initiateDisconnection()
{
    m_shouldReconnect = false;
    setDisconnectionInitiated(true);
    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_INITIATE_DISCONNECTION,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = {},
        };

    if (m_connectionStatus)
    {
        xQueueSend(m_eventsQueue, &event, MS_TO_TICKS(DISCONNECT_TIMEOUT_MS));
    }

    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_STOPPED,
                              pdFALSE,
                              pdFALSE,
                              MS_TO_TICKS(DISCONNECT_TIMEOUT_MS)) &
          EVENT_BIT_WIFI_STOPPED))
    {
        return false;
    }
    else
    {
        LOG_INFO("Disconnected from the access point", m_shouldReconnect, m_connectionStatus);
        return true;
    }
}

bool WiFiController::initiateStop()
{
    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_INITIATE_STOP,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = {},
        };

    setDisconnectionInitiated(true);
    LOG_INFO("Initiating disconnection");
    xQueueSend(m_eventsQueue, &event, MS_TO_TICKS(DISCONNECT_TIMEOUT_MS));
    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_STOPPED,
                              pdFALSE,
                              pdFALSE,
                              MS_TO_TICKS(DISCONNECT_TIMEOUT_MS)) &
          EVENT_BIT_WIFI_STOPPED))
    {
        LOG_INFO("Wifi could not be stopped");
        return false;
    }
    else
    {
        return true;
    }
}

bool WiFiController::disconnectIfNeeded()
{
    if (m_connectionStatus)
    {
        initiateDisconnection();

        if (!(xEventGroupWaitBits(m_eventGroup,
                                  EVENT_BIT_WIFI_STOPPED,
                                  pdFALSE,
                                  pdFALSE,
                                  MS_TO_TICKS(SHORT_TIMEOUT_MS)) &
              EVENT_BIT_WIFI_STOPPED))
        {
            LOG_INFO("Device was already connected and it was not possible to disconnect");
            return false;
        }
    }
    return true;
}

//************************ PRINTING METHODS ******************************//

#if IS_DEBUG_BUILD
void WiFiController::printConnectionLoop() const
{
    // display connection state approximately every 10 seconds
    // takes into account delays: SLEEP_TIME_MS and HANDLERS_RECEIVE_DELAY_MS

    static int i = 0;
    if (i >= 20)
    {
        i = 0;
        printConnectionState();
    }
    i++;
}
#endif

void WiFiController::printErrorCode(esp_err_t errorCode, const char *message)
{
    if (errorCode != ESP_OK)
    {
        LOG_INFO("%s, error: %s", message, esp_err_to_name(errorCode));
    }
}

//************************ PUBLIC METHODS ******************************//

bool WiFiController::connectToAccessPoint(
    const TConnectToAccessPointCommand &command,
    TConnectToAccessPointResult &result,
    bool shouldReconnect)
{
    LOCK_GUARD(this->m_publicAccessMutex, accessMutexGuard);
    result.succeded = false;
    result.status = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER;

    xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING);

    xEventGroupClearBits(m_eventGroup,
                         EVENT_BIT_WIFI_CREDENTIALS_READY |
                             EVENT_BIT_WIFI_IP_ASSIGNED);

    m_shouldReconnect = false; // needed in case device keeps connecting to different Access Point
    m_lastConnectionErrorReason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR;
    m_timeoutMs = command.timeoutMs;
    m_retryNumber = 0;

    if (!disconnectIfNeeded())
    {
        LOG_INFO("Could not connect to Access Point");
        return false;
    }

    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_INITIATE_CREDENTIALS,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = command.credentials,
        };

    xQueueSend(m_eventsQueue, &event, command.timeoutMs); // assign credentials
    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_CREDENTIALS_READY,
                              pdTRUE,
                              pdFALSE, SHORT_TIMEOUT_MS) &
          EVENT_BIT_WIFI_CREDENTIALS_READY))
    {
        LOG_INFO("Credentials could not be set, timeout occured");
        return false;
    }

    event.eventType = EWifiEvent::WIFI__EVENT_INITIATE_CONNECTION;

    m_shouldReconnect = shouldReconnect;

    const int32_t timeout = (command.timeoutMs < 0) ? portMAX_DELAY : command.timeoutMs;

    xEventGroupClearBits(m_eventGroup, EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING);

    xQueueSend(m_eventsQueue, &event, timeout); // initiate conection

    if (timeout == 0)
    {
        return true;
    }

    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_IP_ASSIGNED,
                              pdTRUE,
                              pdFALSE,
                              MS_TO_TICKS(command.timeoutMs)) &
          EVENT_BIT_WIFI_IP_ASSIGNED))
    {
        result.succeded = false;
        m_shouldReconnect = false;

        if (m_lastConnectionErrorReason != EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR)
        {
            result.status = m_lastConnectionErrorReason;
        }
        else
        {
            result.status = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_TIMEOUT;
        }

        disconnectIfNeeded();
    }
    else
    {
        result.succeded = true;
        result.status = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR;
    }

    return true;
}

bool WiFiController::disconnectFromAccessPoint()
{
    LOCK_GUARD(this->m_publicAccessMutex, accessMutexGuard);

    xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING);

    return initiateDisconnection();
}

bool WiFiController::scanAvailableAccessPoints(prot::wifi_scan_ap::TRes &result)
{
    LOCK_GUARD(this->m_publicAccessMutex, accessMutexGuard);

    xEventGroupSetBits(m_eventGroup, EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING);

    if (!disconnectIfNeeded())
    {
        LOG_INFO("Could not disconnect from Access Point");
        return false;
    }

    setScanningActive(true);
    TWifiEvent event =
        {
            .eventType = EWifiEvent::WIFI__EVENT_INITIATE_SCAN,
            .reason = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER,
            .credentials = {}};
    xQueueSend(m_eventsQueue, &event, MS_TO_TICKS(SCAN_TIMEOUT_MS));

    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_SCAN_RESULTS_READY,
                              pdTRUE,
                              pdFALSE,
                              MS_TO_TICKS(SCAN_TIMEOUT_MS)) &
          EVENT_BIT_WIFI_SCAN_RESULTS_READY))
    {
        LOG_INFO("Could not retrieve Wifi scan results");
        return false;
    }

    setScanningActive(false);
    bool scanningSuccess = false;

    switch (m_scanErrorCode)
    {
    case ESP_OK:
        LOG_INFO("Scanning ended with succes");
        scanningSuccess = true;
        break;
    case ESP_ERR_WIFI_NOT_INIT:
        LOG_INFO("Scanning error, WiFi was not initialized");
        result.scannedSuccsfully = false;
        setScanningActive(false);
        break;
    case ESP_ERR_WIFI_NOT_STARTED:
        LOG_INFO("Scanning error, WiFi was not started");
        result.scannedSuccsfully = false;
        setScanningActive(false);
        break;
    case ESP_ERR_INVALID_ARG:
        LOG_INFO("Scanning error, invalid argument passed");
        result.scannedSuccsfully = false;
        setScanningActive(false);
        break;
    case ESP_ERR_NO_MEM:
        LOG_INFO("Scanning error, out of memory");
        result.scannedSuccsfully = false;
        setScanningActive(false);
        break;
    default:
        LOG_INFO("Unknown error when trying to retrieve scan results");
        result.scannedSuccsfully = false;
        setScanningActive(false);
        break;
    }

    if (scanningSuccess)
    {
        result.numberOfValidEntries = m_maxRecordsFound;
        result.scannedSuccsfully = true;
        for (int i = 0; i < m_maxRecordsFound; i++)
        {
            result.availableNetworks[i] = convertRecordToScanEntry(m_wifiScanRecords[i]);
        }
    }
    if (!initiateStop())
    {
        LOG_INFO("Could not stop WiFi after the scan");
    }
    return scanningSuccess;
}

bool WiFiController::waitUntilConnectedToWifi() const
{
    LOG_INFO("Waiting until connected to WiFi");

    if (!(xEventGroupWaitBits(m_eventGroup,
                              EVENT_BIT_WIFI_CONNECTED,
                              pdFALSE,
                              pdFALSE,
                              portMAX_DELAY) &
          EVENT_BIT_WIFI_CONNECTED))
    {
        LOG_INFO("Timeout occured while watingUntilConnectedToWifi execution");
        return false;
    }
    else
    {
        LOG_INFO("Waiting finished, connected to Wifi");
        return true;
    }
}

void WiFiController::displayScanRecords(prot::wifi_scan_ap::TScanEntry *records, int recordsNumber)
{
    LOG_INFO("               SSID              | Channel | RSSI |   Authorization Mode \n");
    LOG_INFO("-------------------------------------------------------------------------\n");
    for (int i = 0; i < recordsNumber; i++)
    {
        LOG_INFO("%32s | %7d | %4d | %21d\n",
                 reinterpret_cast<char *>(records[i].ssid), //  NOLINT
                 records[i].primaryChannel,
                 records[i].rssi,
                 records[i].authmode); //  NOLINT
    }
}

#if IS_DEBUG_BUILD
void WiFiController::printConnectionState() const
{
    LOG_DEBUG("isWifiConnected: %d", getConnectionStatus());
}
#endif

prot::wifi_scan_ap::TScanEntry WiFiController::convertRecordToScanEntry(wifi_ap_record_t &record)
{
    prot::wifi_scan_ap::TScanEntry result = {};
    memcpy(result.ssid, record.ssid, sizeof(result.ssid));
    memcpy(result.bssid, record.bssid, sizeof(result.bssid));
    result.primaryChannel = record.primary;
    result.secondaryChannel = static_cast<device_wifi_second_chan_t>(record.second);
    result.rssi = record.rssi;
    result.authmode = static_cast<device_wifi_auth_mode_t>(record.authmode);

    return result;
}

//************************ GET/SET METHODS ******************************//

bool WiFiController::getConnectionStatus() const
{
    return m_connectionStatus;
}

void WiFiController::getWifiMacAddress(uint8_t *mac) const
{
    esp_wifi_get_mac(WIFI_IF_STA, mac);
}

void WiFiController::setConnectionStatus(bool status)
{
    m_connectionStatus = status;
}

void WiFiController::setScanningActive(bool status)
{
    m_scanningActive = status;
}

void WiFiController::setDisconnectionInitiated(bool status)
{
    m_disconnectionInitiated = status;
}
