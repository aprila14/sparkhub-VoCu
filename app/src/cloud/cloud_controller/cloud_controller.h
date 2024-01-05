#ifndef CLOUDCONTROLLER_H
#define CLOUDCONTROLLER_H

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "device_provisioning.h"
#include "device_twins.h"
#include "esp_event.h"
#include "json_parser.h"
#include "protocol_types.h"
#include "pulse_counter.h"

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
     * Function handling response to the StatusReport message
     */
    static void handleStatusReportResponse(bool ACK);

    /**
     * Function handling OTA update initiated from cloud
     */
    bool handleOtaUpdateLink(const TOtaUpdateLink& otaUpdateLinkStructure);

    /**
     * Timer callback enabling to change CloudController status if the response for the heeartbeat from the cloud
     * is not coming for a specified time period
     */
    void heartbeatWatchdogTimerCallback();

    /**
     * Function giving semaphore informing CloudController task, that CloudConnection is ready to be established
     */
    void setReadinessToConnect();
    void setReadinessAfterDeviceProvisioning();

    /**
     * @brief Function for MqttClientController to let CloudController (and AppController) know that it got connected to
     * a broker
     */
    void setConnectedStatus();

    /**
     *  Function for MqttClientController to let CloudController (and AppController) know that it got disconnected from
     * the broker
     */
    void setNotConnectedStatus();

    const esp_mqtt_client_handle_t& getMqttClientHandle();

    float getADCCalibrationValue() const;

    void setADCCalibrationValue(float ADCCalibration);

#if !TESTING
private:
#endif

    static void run(void* pObject);
    void        _run();
    void        perform();

    /**
     * Function preparing and sending device status information
     */
    void updateDeviceStatus();

    /**
     * Function reading the pressure Value and send Alarm if it is below a certain threshold
     */
    void CheckPressureValueBelowThreshold();


    /**
     * Function setting connection status to given value
     */
    void setConnectionStatus(ECloudConnectionStatus status);

    /**
     * Function initializing MqttClientController with the given credentials
     */
    void configureCloudConnection(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    /**
     * Function to set mqtt topic to send device status based on provided device id
     */
    void setDeviceStatusTopic(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    /**
     * Function starting MqttClientController connection
     */
    void startCloudConnection();

    TaskHandle_t m_taskHandle; // handle to runTask
    uint32_t     m_msgCounter; // outgoing messages counter

    SemaphoreHandle_t m_semaphoreCredentialsReady;    // sempahore indicating that credentials for cloud connection have
                                                      // already been stored in NVS
    SemaphoreHandle_t m_semaphoreWifiConnectionReady; // sempahore indicating Wifi connection is established

    ECloudConnectionStatus m_connectionStatus;

    MqttClientController         m_mqttClientController;
    DeviceProvisioningController m_deviceProvisioningController;
    DeviceTwinsController        m_deviceTwinsController;

    std::string m_deviceStatusTopic;

    float m_ADCCalibrationValue;
};

#endif // CLOUDCONTROLLER_H
