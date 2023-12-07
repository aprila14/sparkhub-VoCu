#ifndef DEVICE_PROVISIONING_H
#define DEVICE_PROVISIONING_H

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_event.h"
#include "protocol_types.h"

constexpr uint8_t DEVICE_PROVISIONING_MAX_TOPIC_SIZE     = 200;
constexpr char    DEVICE_PROVISIONING_STATUS_ASSIGNING[] = "assigning";
constexpr char    DEVICE_PROVISIONING_STATUS_ASSIGNED[]  = "assigned";

class DeviceProvisioningController
{
public:
    DeviceProvisioningController(MqttClientController* mqttClientController, CloudController* cloudController);
    ~DeviceProvisioningController() = default;

    void runTask();

    ECloudDeviceProvisioningStatus getDeviceProvisioningStatus() const;

#if !TESTING
private:
#endif
    static void run(void* pObject);
    void        _run();

    void initiateCloudConnection();
    void configureCloudConnection();
    void createDeviceRegistrationTopic();
    void createDeviceRegistrationStatusTopic();

    bool isProvisioningStatusAssigning(std::string& provisioningStatus);
    bool isProvisioningStatusAssigned(std::string& provisioningStatus);

    void setOperationIdIfStatusAssigning(std::string& provisioningOperationId);

    void createMqttUsernameAfterProvisioning(
        json_parser::TDeviceProvisioningInfo&           provisioningInfo,
        prot::cloud_set_credentials::TCloudCredentials& newCloudCredentials);
    void createCloudAddressAfterProvisioning(
        json_parser::TDeviceProvisioningInfo&           provisioningInfo,
        prot::cloud_set_credentials::TCloudCredentials& newCloudCredentials);
    void saveCredentialsAfterProvisioning(json_parser::TDeviceProvisioningInfo& provisioningInfo);

    TaskHandle_t m_taskHandle; // handle to runTask

    ECloudDeviceProvisioningStatus m_provisioningStatus = ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT;

    prot::cloud_set_credentials::TCloudCredentials m_cloudCredentials;

    char        m_deviceRegistrationTopic[DEVICE_PROVISIONING_MAX_TOPIC_SIZE];
    char        m_deviceRegistrationStatus[DEVICE_PROVISIONING_MAX_TOPIC_SIZE];
    int32_t     m_deviceRegistrationRequestId = 0;
    std::string m_deviceProvisioningOperationId;

    MqttClientController* m_pMqttClientController; // pointer to MqttClientController for calling lower-level functions
    CloudController*      m_pCloudController;
};

#endif // DEVICE_PROVISIONING_H
