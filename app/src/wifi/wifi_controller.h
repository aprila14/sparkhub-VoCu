#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include "defines.h"
#include "custom_types.h"
#include "mutex.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_wifi_types.h"

class WiFiController
{
public:
    WiFiController();

    /**
     * @brief Create a task for WiFiController
     */
    void runTask();

    /**
     * @brief Initialize WiFi, set mode, config and register handlers
     * @return
     * - true: initialization ended with success
     * - false: initialization failed
     */
    bool init();

    /**
     * @brief Load from NVS Configuration and start connecting if set
     */
    bool loadCredentialsFromConfigNvsAndConnectIfSet();

    /**
     * @brief Event handler for all WiFi Events
     * @param eventHandlerArg - user parameter passed to the event handler
     * @param eventBase - the base ID of the event
     * @param eventId   - the ID of the event
     * @param eventData - the data specific to the event occurenc, that gets passed to the handler
     */
    void wifiEventHandler(void* eventHandlerArg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    /**
     * @brief ... . If already connected to AP, it will disconnect and connect to another.
     * @param config Configuration for the command
     * @return true if internal logic executed properly (even if it didn't connect to the WiFi)
     */
    bool connectToAccessPoint(
            const TConnectToAccessPointCommand& command,
            TConnectToAccessPointResult& result,
            bool shouldReconnect);

    /**
     * @brief Disconnect from the AP that we are connected to right now. To distinguish
     * between non-intentional disconnection from WiFi, m_disconnectionInitiated is set to true
     * @return
     *  - true: device was disconnected properly
     *  - false: internal logic fault or device not disconnected properly
     */
    bool disconnectFromAccessPoint();
    // TODO - determine if we need command/result

    /**
     * @brief scanAvailableAccessPoints
     * @param result
     * @return
     */
    bool scanAvailableAccessPoints(prot::wifi_scan_ap::TRes& result);

    /**
     * @brief Determine if connection to Access Point has been established
     * @return
     *  - true: connection has been established
     *  - false: connection has not been established yet or connection has been lost
     */
    bool getConnectionStatus() const;

    /**
     * @brief waitUntilConnectedToWifi
     * @return
     */
    bool waitUntilConnectedToWifi() const;

    /**
     * @brief
     */
    void connectWithCredentialsFromNvs();

    static void displayScanRecords(prot::wifi_scan_ap::TScanEntry* records, int recordsNumber);

#if IS_DEBUG_BUILD
    void printConnectionState() const;
#endif

#if !TESTING
private:
#endif
    enum class EWifiEvent : uint32_t
    {
        WIFI__EVENT_INITIATE_START = 0,
        WIFI__EVENT_INITIATE_CREDENTIALS,
        WIFI__EVENT_INITIATE_CONNECTION,
        WIFI__EVENT_INITIATE_DISCONNECTION,
        WIFI__EVENT_INITIATE_STOP,
        WIFI__EVENT_INITIATE_SCAN,
        WIFI__EVENT_STA_START,
        WIFI__EVENT_SCAN_DONE,
        WIFI__EVENT_CONNECTED,
        WIFI__EVENT_FAIL,
        WIFI__EVENT_IP_ASSIGNED,
        WIFI__EVENT_DISCONNECTED,
        WIFI__EVENT_STA_BEACON_TIMEOUT,
        WIFI__EVENT_STA_STOP,
        WIFI__EVENT_UNKNOWN,
    };

    struct TWifiEvent
    {
        EWifiEvent eventType;
        EConnectToSsidErrorReason reason;
        TWiFiCredentials credentials;
    };


    /**
     * @brief Part of the run method executed in endless loop
     */
    void perform();

    /**
     * @brief Main logic of the module
     */
    static void run(void* pObject);
    void _run();

    bool registerHandlers();
    void processWifiEvents();
    void processWifiEvent(TWifiEvent& event);

    static bool wifiConfigModeInit(TWiFiCredentials& credentials);

    static EConnectToSsidErrorReason parseWifiDisconnectReason(uint8_t reason);

    void tryToReconnect();
    bool initiateDisconnection();
    bool initiateStop();
    virtual bool disconnectIfNeeded();

    void setConnectionStatus(bool status);

    void setScanningActive(bool status);
    void setDisconnectionInitiated(bool status);

    static prot::wifi_scan_ap::TScanEntry convertRecordToScanEntry(wifi_ap_record_t& record);

    static void printErrorCode(esp_err_t errorCode, const char *message);

#if IS_DEBUG_BUILD
    void printConnectionLoop() const;
#endif

    TaskHandle_t m_taskHandle;                              //handle to runTask
    mutex_t m_publicAccessMutex = nullptr;  ///< Protects access to all public API functions
    mutex_t m_lastConnectionErrorReasonMutex = nullptr; ///< Protects access to lastConnectionErrorReason field
    EventGroupHandle_t m_eventGroup;

    EConnectToSsidErrorReason m_connectionErrorReason;      //reason for connection issue, passed from wifi handler

    bool m_connectionStatus;                                //indicates that connection was established
    bool m_disconnectionInitiated;                          //indicates that disconnection was initiated by user
    bool m_shouldReconnect;                                 //indicates that connection should be attempted until ended with success
    bool m_scanningActive;                                  //indicates that scan of available Access Points is currently being performed

    uint16_t m_maxRecordsFound;                             //max records found during last WiFi scan
    uint32_t m_retryNumber;                                 //amount of times reconnection was attempted so far
    int32_t m_timeoutMs;                                    //time after which reconnection is not attempted
    uint64_t m_disconnectionTime;                           //time at which disconnection event occured

    EConnectToSsidErrorReason m_lastConnectionErrorReason;  //error that has occured during last attempt to connect

    QueueHandle_t m_eventsQueue;

    wifi_ap_record_t m_wifiScanRecords[MAX_NUMBER_OF_SCANNED_ACCESS_POINTS];

    esp_err_t m_scanErrorCode;

};

#endif // WIFI_CONTROLLER_H
