#ifndef DEVICE_PROVISIONING_H
#define DEVICE_PROVISIONING_H

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_event.h"
#include "json_parser.h"
#include "protocol_types.h"

class DeviceProvisioningController
{
public:
    DeviceProvisioningController(MqttClientController *mqttClientController, CloudController *cloudController);
    ~DeviceProvisioningController() = default;

    void runTask();

    ECloudDeviceProvisioningStatus getDeviceProvisioningStatus() const;

#if !TESTING
private:
#endif
    static void run(void *pObject);
    void _run();

    TaskHandle_t m_taskHandle; // handle to runTask

    ECloudDeviceProvisioningStatus m_provisioningStatus;

    MqttClientController *m_pMqttClientController; // pointer to MqttClientController for calling lower-level functions
    CloudController *m_pCloudController;
};

#endif // DEVICE_PROVISIONING_H
