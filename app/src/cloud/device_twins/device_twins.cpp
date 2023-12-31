static const char* LOG_TAG = "DeviceTwins";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "device_twins.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "custom_types.h"
#include "json_parser_defines.h"
#include "sleep.h"

namespace
{
const std::string DEVICE_TWIN_UPDATE_TOPIC            = std::string("$iothub/twin/PATCH/properties/desired/#");
const std::string DEVICE_TWIN_REPORTED_TOPIC_PREFIX   = std::string("$iothub/twin/PATCH/properties/reported/?$rid=");
const std::string DEVICE_TWIN_REPORTED_RESPONSE_TOPIC = std::string("$iothub/twin/res/#");
const std::string DEVICE_TWIN_GET_TOPIC               = std::string("$iothub/twin/GET/?$rid=");

const char* OTA_SUCCESS_STRING      = "ota_success";
const char* OTA_FAIL_STRING         = "ota_fail";
const char* OTA_SAME_VERSION_STRING = "same_version_received";

constexpr uint32_t SLEEP_TIME_BEFORE_RESTART_AFTER_OTA_MS = 2000;
} // unnamed namespace

bool checkIfItIsNewFirmwareVersion(const char* firmwareVersion)
{
    uint32_t majorVersion = 0;
    uint32_t minorVersion = 0;
    uint32_t patchVersion = 0;

    const uint32_t projectMajorVersion = PROJECT_VER_MAJOR;
    const uint32_t projectMinorVersion = PROJECT_VER_MINOR;
    const uint32_t projectPatchVersion = PROJECT_VER_PATCH;

    LOG_INFO("About to check string for version: %s", firmwareVersion);
    LOG_INFO("Current version: %d.%d.%d", projectMajorVersion, projectMinorVersion, projectPatchVersion);

    uint8_t result = sscanf(firmwareVersion, "%d.%d.%d", &majorVersion, &minorVersion, &patchVersion);

    if (result <= 0)
    {
        LOG_ERROR("Error while analyzing firmwareVersion string");
        return false;
    }

    return (
        (majorVersion != projectMajorVersion) || (minorVersion != projectMinorVersion) ||
        (patchVersion != projectPatchVersion));
}

DeviceTwinsController::DeviceTwinsController(
    MqttClientController* mqttClientController,
    CloudController*      cloudController) :
    m_taskHandle(),
    m_desiredFieldFlags(0),
    m_requestId(0),
    m_pMqttClientController(mqttClientController),
    m_pCloudController(cloudController)
{
}

void DeviceTwinsController::runTask()
{
    LOG_INFO("runTask() started...");

    LOG_INFO(
        "Currently available free heap memory: %u",
        static_cast<uint32_t>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)));
    LOG_INFO("Minimum available free heap memory in the past(?): %d", esp_get_minimum_free_heap_size());

    BaseType_t xReturned =
        xTaskCreate(run, LOG_TAG, DEFAULT_HUGE_STACK_SIZE, this, DEFAULT_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void DeviceTwinsController::run(void* pObject)
{
    DeviceTwinsController* pDeviceTwinsController =
        reinterpret_cast<DeviceTwinsController*>(pObject); // NOLINT - we need reinterpret cast
    pDeviceTwinsController->_run();
}

void DeviceTwinsController::_run()
{
    subscribeDeviceTwinTopics();

    LOG_INFO("Topics subscribed");

    TWorkflowData workflowData = pConfig->getWorkflowData();
    reportDeviceUpdateStatus(EOtaAgentState::OTA_AGENT_STATE_IDLE, workflowData);

    const float flowMeterCalibrationValueFromFlash = pConfig->getFlowMeterCalibrationValue();
    reportFlowMeterCalibrationValue(flowMeterCalibrationValueFromFlash);

    // publish empty message on the GET topic to be up to date in case there where any changes when device was offline
    publishGetDeviceTwinRequest();

    while (true)
    {
        if (!m_pMqttClientController->checkIfMessageBufferIsEmpty())
        {
            json_parser::TMessage message = m_pMqttClientController->getNextMessage();
            LOG_INFO("Received message: %s", message.message.c_str());

            handleDeviceTwinMessage(message);
            handleDeviceTwinResponse();
        }
        SLEEP_MS(100);
    }
}

void DeviceTwinsController::subscribeDeviceTwinTopics()
{
    m_pMqttClientController->addTopicForSubscription(DEVICE_TWIN_UPDATE_TOPIC, 1);
    m_pMqttClientController->addTopicForSubscription(DEVICE_TWIN_REPORTED_RESPONSE_TOPIC, 1);
}

void DeviceTwinsController::handleDeviceTwinMessage(const json_parser::TMessage& message)
{
    cJSON* pInputJson = json_parser::preprocessInputMessage(message.message);

    cJSON* pDeviceTwinDesiredJson = nullptr;

    if (pInputJson == nullptr)
    {
        LOG_ERROR("Error while preprocessing input DeviceTwin message");
        return;
    }

    if (json_parser::checkIfFieldExistsInGivenJson(pInputJson, json_parser::DEVICE_TWIN_DESIRED_KEY))
    {
        // in response to the GET request full Device Twin document is received,
        // so we have to parse the "desired" key first
        LOG_INFO("%s field found!", json_parser::DEVICE_TWIN_DESIRED_KEY);

        pDeviceTwinDesiredJson = json_parser::parseDeviceTwinDesired(pInputJson);

        if (pDeviceTwinDesiredJson == nullptr)
        {
            LOG_ERROR("Error while processing desired DeviceTwin message");
            return;
        }
    }
    else
    {
        // regular update notification from Device Twin is received, no "desired" key to parse
        pDeviceTwinDesiredJson = pInputJson;
    }

    if (json_parser::checkIfFieldExistsInGivenJson(pDeviceTwinDesiredJson, json_parser::DEVICE_UPDATE_KEY))
    {
        LOG_INFO("%s field found!", json_parser::DEVICE_UPDATE_KEY);

        TDeviceUpdate deviceUpdateData = {};

        if (json_parser::parseDeviceUpdate(pDeviceTwinDesiredJson, &deviceUpdateData))
        {
            LOG_INFO("Device update data parsed properly");

            LOG_INFO("deviceUpdateData.fileUrl: %s", deviceUpdateData.fileUrl);
            LOG_INFO("deviceUpdateData.updateManifest.fileKey: %s", deviceUpdateData.updateManifest.fileKey);
            LOG_INFO(
                "deviceUpdateData.updateManifest.updateId.updateName: %s",
                deviceUpdateData.updateManifest.updateId.updateName);
            LOG_INFO(
                "deviceUpdateData.updateManifest.updateId.providerName: %s",
                deviceUpdateData.updateManifest.updateId.providerName);
            LOG_INFO(
                "deviceUpdateData.updateManifest.updateId.firmwareVersion: %s",
                deviceUpdateData.updateManifest.updateId.firmwareVersion);

            LOG_INFO("action: %d", deviceUpdateData.workflowData.deviceUpdateAction);
            LOG_INFO("actionId: %s", deviceUpdateData.workflowData.workflowId);

            if ((deviceUpdateData.workflowData.deviceUpdateAction == EDeviceUpdateAction::ACTION_DOWNLOAD) &&
                checkIfItIsNewFirmwareVersion(deviceUpdateData.updateManifest.updateId.firmwareVersion))
            {
                TOtaUpdateLink otaUpdateLink = {};
                strncpy(otaUpdateLink.firmwareLink, deviceUpdateData.fileUrl, strlen(deviceUpdateData.fileUrl));
                pConfig->setOtaUpdateLink(otaUpdateLink);
                pConfig->setWorkflowData(deviceUpdateData.workflowData);
                pConfig->setUpdateId(deviceUpdateData.updateManifest.updateId);

                app::TEventData eventData        = {};
                eventData.otaPerform.updateReady = true;

                reportDeviceUpdateStatus(
                    EOtaAgentState::OTA_AGENT_STATE_DEPLOYMENT_IN_PROGRESS, deviceUpdateData.workflowData);

                bool result = app::pAppController->addEvent(
                    app::EEventType::OTA__PERFORM, app::EEventExecutionType::SYNCHRONOUS, &eventData);

                if (!result)
                {
                    LOG_ERROR("Could not perform OTA");
                    reportDeviceUpdateStatus(
                        EOtaAgentState::OTA_AGENT_STATE_DEPLOYMENT_FAILED, deviceUpdateData.workflowData);
                }
                else
                {
                    LOG_INFO("OTA performed successfully");

                    LOG_INFO("About to reset in 2000 ms");
                    SLEEP_MS(SLEEP_TIME_BEFORE_RESTART_AFTER_OTA_MS);
                    esp_restart();
                }
            }
            // decide if update shall be performed based on action
        }
        else
        {
            LOG_ERROR("Error while parsing deviceUpdateData");
        }
    }

    if (json_parser::checkIfFieldExistsInGivenJson(pDeviceTwinDesiredJson, json_parser::FLOW_METER_CALIBRATION_KEY))
    {
        LOG_INFO("%s field found!", json_parser::FLOW_METER_CALIBRATION_KEY);

        float flowMeterCalibrationValue = 0.0;

        if (json_parser::parseFlowMeterCalibrationValue(pDeviceTwinDesiredJson, &flowMeterCalibrationValue))
        {
            LOG_INFO("Flow meter calibration value received %f", flowMeterCalibrationValue);

            app::TEventData eventData           = {};
            eventData.flowMeterCalibrationValue = flowMeterCalibrationValue;

            const bool result = app::pAppController->addEvent(
                app::EEventType::CALIBRATE_FLOW_METER, app::EEventExecutionType::SYNCHRONOUS, &eventData);

            if (result)
            {
                reportFlowMeterCalibrationValue(flowMeterCalibrationValue);
            }
            else
            {
                LOG_ERROR("Error during flow meter calibration");
            }
        }
        else
        {
            LOG_ERROR("Error while parsing flow meter calibration value");
        }
    }


    if (json_parser::checkIfFieldExistsInGivenJson(pDeviceTwinDesiredJson, json_parser::ADC_CALIBRATION_KEY))
    {
        LOG_INFO("%s field found!", json_parser::ADC_CALIBRATION_KEY);

        float ADCCalibrationValue = 0.0;

        if (json_parser::parseADCCalibrationValue(pDeviceTwinDesiredJson, &ADCCalibrationValue))
        {
            LOG_INFO("ADC calibration value received %f", ADCCalibrationValue);

            app::TEventData eventData           = {};
            eventData.ADCCalibrationValue = ADCCalibrationValue;

            const bool result = app::pAppController->addEvent(
                app::EEventType::CALIBRATE_ADC, app::EEventExecutionType::SYNCHRONOUS, &eventData);

            if (result)
            {
                reportADCCalibrationValue(ADCCalibrationValue);
            }
            else
            {
                LOG_ERROR("Error during ADC calibration");
            }
        }
        else
        {
            LOG_ERROR("Error while parsing ADC calibration value");
        }
    }

    // Add handling new fields here, under separate ifs

    cJSON_Delete(pInputJson);

    LOG_INFO("handleDeviceTwinMessage, end");
}

std::string DeviceTwinsController::prepareEmptyJsonMessage()
{
    cJSON* pReportedJson = cJSON_CreateObject();

    if (pReportedJson == NULL)
    {
        LOG_ERROR("Not possible to create reported JSON");
        return std::string("");
    }

    const std::string reportedMessage = json_parser::prepareReportedMessage(pReportedJson);
    cJSON_Delete(pReportedJson);

    return reportedMessage;
}

void DeviceTwinsController::handleDeviceTwinResponse()
{
    const std::string reportedMessage = prepareEmptyJsonMessage();

    if (reportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing reported message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildReportedTopic(++m_requestId), reportedMessage))
    {
        LOG_ERROR("Could not send reported message");
    }
}

std::string DeviceTwinsController::buildReportedTopic(int32_t requestId)
{
    return DEVICE_TWIN_REPORTED_TOPIC_PREFIX + std::to_string(requestId);
}

std::string DeviceTwinsController::buildGetTopic(int32_t requestId)
{
    return DEVICE_TWIN_GET_TOPIC + std::to_string(requestId);
}

void DeviceTwinsController::reportDeviceUpdateStatus(EOtaAgentState state, const TWorkflowData& workflowData)
{
    TUpdateId updateId = pConfig->getUpdateId();

    if (!updateId.isSetProviderName())
    {
        strncpy(updateId.providerName, "sparkhub", strlen("sparkhub"));
    }

    if (!updateId.isSetUpdateName())
    {
        strncpy(updateId.updateName, "sparkhub-iot-levelsense", strlen("sparkhub-iot-levelsense"));
    }

    if (sprintf(updateId.firmwareVersion, "%d.%d.%d", PROJECT_VER_MAJOR, PROJECT_VER_MINOR, PROJECT_VER_PATCH) < 0)
    {
        LOG_ERROR("Could not fill updateId.firmwareVersion");
        return;
    }

    std::string deviceUpdateStatusReportedMessage =
        json_parser::prepareDeviceUpdateReport(updateId, static_cast<uint8_t>(state), workflowData);

    if (deviceUpdateStatusReportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing deviceUpdateStatus reported message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildReportedTopic(++m_requestId), deviceUpdateStatusReportedMessage))
    {
        LOG_ERROR("Could not send firmware info reported message");
        return;
    }
}

void DeviceTwinsController::reportFlowMeterCalibrationValue(const float& flowMeterCalibrationValue)
{
    const std::string flowMeterCalibrationReportedMessage =
        json_parser::prepareFlowMeterCalibrationReport(flowMeterCalibrationValue);

    if (flowMeterCalibrationReportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing flow meter calibration reported message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildReportedTopic(++m_requestId), flowMeterCalibrationReportedMessage))
    {
        LOG_ERROR("Could not send flow meter calibration reported message");
        return;
    }
}

void DeviceTwinsController::reportADCCalibrationValue(const float& ADCCalibrationValue)
{
    const std::string ADCCalibrationReportedMessage =
        json_parser::prepareADCCalibrationReport(ADCCalibrationValue);

    if (ADCCalibrationReportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing ADC calibration reported message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildReportedTopic(++m_requestId), ADCCalibrationReportedMessage))
    {
        LOG_ERROR("Could not send ADC calibration reported message");
        return;
    }
}

void DeviceTwinsController::publishGetDeviceTwinRequest()
{
    const std::string reportedMessage = prepareEmptyJsonMessage();

    if (reportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing get device twin message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildGetTopic(++m_requestId), reportedMessage))
    {
        LOG_ERROR("Could not send get device twin message");
        return;
    }
}
