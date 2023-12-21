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

const char* OTA_SUCCESS_STRING      = "ota_success";
const char* OTA_FAIL_STRING         = "ota_fail";
const char* OTA_SAME_VERSION_STRING = "same_version_received";
} // unnamed namespace

bool checkIfItIsNewFirmwareVersion(const TFirmwareInfo& firmwareInfo)
{
    uint32_t majorVersion = 0;
    uint32_t minorVersion = 0;
    uint32_t patchVersion = 0;

    uint32_t projectMajorVersion = PROJECT_VER_MAJOR;
    uint32_t projectMinorVersion = PROJECT_VER_MINOR;
    uint32_t projectPatchVersion = PROJECT_VER_PATCH;

    LOG_INFO("About to check string for version: %s", firmwareInfo.version.c_str());
    LOG_INFO("Current version: %d.%d.%d", projectMajorVersion, projectMinorVersion, projectPatchVersion);

    uint8_t result = sscanf(firmwareInfo.version.c_str(), "%d.%d.%d", &majorVersion, &minorVersion, &patchVersion);

    if (result <= 0)
    {
        LOG_ERROR("Error while analyzing firmwareInfo.version string");
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
    m_reportedFieldFlags(0),
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
    m_pMqttClientController->subscribeToTopic(DEVICE_TWIN_UPDATE_TOPIC, 0);
    m_pMqttClientController->subscribeToTopic(DEVICE_TWIN_REPORTED_RESPONSE_TOPIC, 0);
}

void DeviceTwinsController::handleDeviceTwinMessage(const json_parser::TMessage& message)
{
    cJSON* pInputJson = json_parser::preprocessInputMessage(message.message);

    if (pInputJson == nullptr)
    {
        LOG_ERROR("Error while preprocessing input DeviceTwin message");
        return;
    }

    if (json_parser::checkIfFieldExistsInGivenJson(pInputJson, json_parser::DEVICE_UPDATE_KEY))
    {
        LOG_INFO("%s field found!", json_parser::DEVICE_UPDATE_KEY);

        TDeviceUpdate deviceUpdateData = {};

        if (!json_parser::parseDeviceUpdate(pInputJson, &deviceUpdateData))
        {
            LOG_ERROR("Error while parsing deviceUpdateData");
        }
        else
        {
            LOG_INFO("Device update data parsed properly");

            LOG_INFO("deviceUpdateData.fileUrl: %s", deviceUpdateData.fileUrl);
            LOG_INFO("deviceUpdateData.updateManifest.fileKey: %s", deviceUpdateData.updateManifest.fileKey);
        }
    }

    if (json_parser::checkIfFieldExistsInGivenJson(pInputJson, json_parser::FIRMWARE_INFO_KEY))
    {
        LOG_INFO("%s field found!", json_parser::FIRMWARE_INFO_KEY);

        TFirmwareInfo firmwareInfoData = {};

        if (!json_parser::parseFirmwareInfo(pInputJson, &firmwareInfoData))
        {
            LOG_ERROR("Error while parsing firmwareInfo");
        }
        else
        {
            // Act upon the new firmware data -> check if firmware needs to be updated, if so -> initate the update

            pConfig->setFirmwareInfo(firmwareInfoData);

            if (checkIfItIsNewFirmwareVersion(firmwareInfoData))
            {
                LOG_INFO("Updating the firmware");
                app::TEventData eventData        = {};
                eventData.otaPerform.updateReady = true;

                TOtaUpdateLink otaUpdateLink = {};
                strncpy(
                    otaUpdateLink.firmwareLink,
                    firmwareInfoData.firmwareUrl.c_str(),
                    strlen(firmwareInfoData.firmwareUrl.c_str()));

                pConfig->setOtaUpdateLink(otaUpdateLink);

                commons::printAvailableHeapMemory(__LINE__, __FILE__, "handleDeviceTwinMessage");

                bool result = app::pAppController->addEvent(
                    app::EEventType::OTA__PERFORM, app::EEventExecutionType::SYNCHRONOUS, &eventData);

                if (!result)
                {
                    LOG_ERROR("Could not perform OTA");
                    reportFirmwareVersion(OTA_FAIL_STRING);
                }
                else
                {
                    LOG_INFO("OTA performed successfully");
                    reportFirmwareVersion(OTA_SUCCESS_STRING);

                    LOG_INFO("About to reset in 2000 ms");
                    SLEEP_MS(2000);
                    esp_restart();
                }
            }
            else
            {
                LOG_WARNING("Received the same version as the current one, not performing the OTA");
                reportFirmwareVersion(OTA_SAME_VERSION_STRING);
            }

            // Add the field to reported

            m_reportedFieldFlags |= (1 << static_cast<uint8_t>((EDeviceTwinField::DEVICE_TWIN_FIELD_FIRMWARE_INFO)));
        }
    }

    // Add handling new fields here, under separate ifs

    cJSON_Delete(pInputJson);

    LOG_INFO("handleDeviceTwinMessage, end");
}

void DeviceTwinsController::handleDeviceTwinResponse()
{
    cJSON* pReportedJson = cJSON_CreateObject();

    if (pReportedJson == NULL)
    {
        LOG_ERROR("Not possible to create reported JSON");
        return;
    }

    if (m_reportedFieldFlags > 0)
    {
        if (m_reportedFieldFlags & (1 << static_cast<uint8_t>(EDeviceTwinField::DEVICE_TWIN_FIELD_FIRMWARE_INFO)))
        {
            // add calling the function for building the reported JSON, probably based on data from config
            TFirmwareInfo firmwareInfo = pConfig->getFirmwareInfo();
            if (!json_parser::addFirmwareInfoToReportedJson(&pReportedJson, firmwareInfo))
            {
                LOG_ERROR("Could not add firmware info to reported JSON");
            }
            else
            {
                LOG_INFO("Response to firmware info message ready!");
            }
        }
    }

    std::string reportedMessage = json_parser::prepareReportedMessage(pReportedJson);
    cJSON_Delete(pReportedJson);

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

void DeviceTwinsController::reportFirmwareVersion(const char* otaStatus)
{
    std::string firmwareVersionReportedMessage =
        json_parser::prepareFirmwareInfoReportedMessage(pConfig->getFirmwareInfo(), otaStatus);

    if (firmwareVersionReportedMessage == std::string(""))
    {
        LOG_ERROR("Error while preparing firmware info reported message");
        return;
    }

    if (!m_pMqttClientController->sendMessage(buildReportedTopic(++m_requestId), firmwareVersionReportedMessage))
    {
        LOG_ERROR("Could not send firmware info reported message");
        return;
    }
}