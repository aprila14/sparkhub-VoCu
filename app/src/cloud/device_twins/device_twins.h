#ifndef C02039A7_B99C_4703_A208_A34AD2EB4A21
#define C02039A7_B99C_4703_A208_A34AD2EB4A21

#include "mqtt_client_controller.h"

#include "freertos/FreeRTOS.h"

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
    std::string buildReportedTopic(int32_t requestId);
    std::string buildGetTopic(int32_t requestId);
    std::string prepareEmptyJsonMessage();
    void        handleDeviceTwinMessage(const json_parser::TMessage& message);
    void        handleDeviceTwinResponse();

    void reportDeviceUpdateStatus(EOtaAgentState state, const TWorkflowData& workflowData);
    void
    reportInitialDeviceUpdateStatus(EOtaAgentState state, const TWorkflowData& workflowData, const TUpdateId& updateId);

    void reportFlowMeterCalibrationValue(const float& flowMeterCalibrationValue);

    void reportADCCalibrationValue(const float& ADCCalibrationValue);

    void publishGetDeviceTwinRequest();

    TaskHandle_t m_taskHandle; // handle to runTask

    uint32_t m_desiredFieldFlags;
    uint32_t m_requestId;

    MqttClientController* m_pMqttClientController; // pointer to MqttClientController for calling lower-level functions
    CloudController*      m_pCloudController;
};

#endif /* C02039A7_B99C_4703_A208_A34AD2EB4A21 */
