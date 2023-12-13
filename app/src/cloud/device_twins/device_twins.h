#ifndef C02039A7_B99C_4703_A208_A34AD2EB4A21
#define C02039A7_B99C_4703_A208_A34AD2EB4A21

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"

enum class EDeviceTwinField
{
    DEVICE_TWIN_FIELD_FIRMWARE_INFO = 0
};

class DeviceTwinsController
{

public:
    DeviceTwinsController(MqttClientController* mqttClientController, CloudController* cloudController);
    ~DeviceTwinsController() = default;
    DeviceTwinsController()  = delete;

    void runTask();
#if !TESTING
private:
#endif // !TESTING
    static void run(void* pObject);
    void        _run();

    void        subscribeDeviceTwinTopics();
    std::string buildReportedTopic();
    void        handleDeviceTwinMessage(const json_parser::TMessage& message);
    void        handleDeviceTwinResponse();

    void reportFirmwareVersion(int32_t requestId);

    TaskHandle_t m_taskHandle; // handle to runTask

    uint32_t m_reportedFieldFlags;
    uint32_t m_desiredFieldFlags;
    uint32_t m_requestId;

    MqttClientController* m_pMqttClientController; // pointer to MqttClientController for calling lower-level functions
    CloudController*      m_pCloudController;
};

#endif /* C02039A7_B99C_4703_A208_A34AD2EB4A21 */
