#ifndef CLOUDCONTROLLER_H
#define CLOUDCONTROLLER_H

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_event.h"
#include "json_parser.h"
#include "protocol_types.h"

class MqttClientController;

class CloudController
{
public:
    CloudController();
    ~CloudController() = default;

    void runTask();

    /**
     * Set m_credentials to the values from the given structure
     */
    bool setCredentials(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    /**
     * Return current CloudController state
     */
    ECloudConnectionStatus getConnectionStatus() const;

    /**
     * Function handling required actions after getting setLightIntensityLevel request
     */
    static bool handleSetLightIntensityLevel(const json_parser::TSetLightLevel& setLightLevelStructure);

    /**
     * Function handling response to the StatusReport message
     */
    static void handleStatusReportResponse(bool ACK);

    /**
     * Function handling response to the Heartbeat message
     */
    void handleHeartbeatResponse();


    /**
     * Function handling getLightIntensityLevel request from the widget
     */
    void handleWidgetGetLightIntensityLevel(int32_t requestId);

    /**
     * Function handling setLightIntensityLevel request from the widget
     */
    bool handleWidgetSetLightIntensityLevel(const json_parser::TSetLightLevel& setLightLevelStructure, int32_t requestId);

    /**
     * Function handling OTA update initiated from cloud
     */
    bool handleOtaUpdateLink(const TOtaUpdateLink& otaUpdateLinkStructure);

    /**
     * Function handling message with time slots list for LightScheduler
     */
    bool handleTimeSlotsList(const json_parser::TTimeSlotsList& timeSlotsListStructure);

    /**
     * Timer callback enabling to change CloudController status if the response for the heeartbeat from the cloud
     * is not coming for a specified time period
     */
    void heartbeatWatchdogTimerCallback();

    /**
     * Function giving semaphore informing CloudController task, that CloudConnection is ready to be established
    */
    void setReadinessToConnect();

    /**
     * @brief Function for MqttClientController to let CloudController (and AppController) know that it got connected to a broker
     */
    void setConnectedStatus();

    /**
     *  Function for MqttClientController to let CloudController (and AppController) know that it got disconnected from the broker
     */
    void setNotConnectedStatus();

    const esp_mqtt_client_handle_t& getMqttClientHandle();

#if !BUILD_WITH_THINGSBOARD
    /**
     * Function returning client UUID, needed in case of connection with AWS cloud
     */
    const std::string& getClientUuid() const;

#endif  // !BUILD_WITH_THINGSBOARD

#if !TESTING
private:
#endif

    static void run(void* pObject);
    void _run();
    void perform();

    /**
     * Function preparing and sending device status information
     */
    void updateDeviceStatus();

    /**
     * Function preparing and sending heartbeat message
     */

    void sendHeartbeat();

    /**
     * Function setting connection status to given value
     */
    void setConnectionStatus(ECloudConnectionStatus status);

    /**
     * Function initializing MqttClientController with the given credentials
     */
    void configureCloudConnection(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    /**
     * Function starting MqttClientController connection
     */
    void startCloudConnection();

    TaskHandle_t m_taskHandle;                      //handle to runTask
    TimerHandle_t m_heartbeatWatchdogTimer;         //handle to timer handling heartbeat checking
    uint32_t m_heartbeatWatchdogCounter;            //watchdog counter that shall be zeroed everytime when heartbeat message
                                                    //comes in. If it exceedes specified value, CloudController
                                                    //is regarded as not connected
    uint32_t m_msgCounter;                          //outgoing messages counter

    SemaphoreHandle_t m_semaphoreCredentialsReady;  //sempahore indicating that credentials for cloud connection have already been stored in NVS

    ECloudConnectionStatus m_connectionStatus;

    MqttClientController m_mqttClientController;    //pointer to MqttClientController for calling lower-level functions

#if !BUILD_WITH_THINGSBOARD
    std::string m_clientUuid;
#endif // !BUILD_WITH_THINGSBOARD

    std::string m_deviceStatusTopic;
    std::string m_heartbeatTopic;

};

#endif // CLOUDCONTROLLER_H
