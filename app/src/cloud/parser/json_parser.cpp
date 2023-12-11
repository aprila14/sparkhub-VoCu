// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "jsonParser";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "json_parser.h"

#include "json_parser_defines.h"

#include <cctype>

// #include "esp_heap_caps.h"

namespace
{
const char* FIRMWARE_INFO_VERSION_KEY = "version";
const char* FIRMWARE_INFO_URL_KEY     = "url";
} // unnamed namespace

namespace json_parser
{

const char* FIRMWARE_INFO_KEY = "firmware_info";

#if !TESTING // directive added to avoid double declaration of function
static cJSON*      deviceStatusToJson(const TDeviceStatus& deviceStatus, uint32_t msgCounter);
static bool        processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);
static std::string getStatusReportString(const TDeviceStatus& deviceStatus);
static std::string getResponseString(const TResponse& responseData);
static std::string getConnectionString(bool status);
static std::string getBooleanString(bool status);
static std::string getOtaUpdateLinkString(const TOtaUpdateLink& otaUpdateLink);

#endif

static std::string getConnectionString(bool status);
static std::string getBooleanString(bool status);


bool checkIfFieldExistsInGivenJson(cJSON* inputJson, const char* keyword)
{
    cJSON* searchedJson = cJSON_GetObjectItemCaseSensitive(inputJson, keyword);

    if (searchedJson == nullptr)
    {
        return false;
    }

    return true;
}
/***** JSON TO STRUCTURE *****/

/**
 * @brief processStatusReport - function parsing StatusReport message in a cJSON format into TDeviceStatus structure
 * @param pDataJson - input cJSON object with a data JSON containing message payload
 * @param pOutput - TDeviceStatus structure with the extracted data
 * @return boolean value representing success of the operation
 */

bool processStatusReport(cJSON* pDataJson, TDeviceStatus* pOutput)
{
    cJSON* pWifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "wifiConnectionState");
    cJSON* pBleConnectionStateJson  = cJSON_GetObjectItemCaseSensitive(pDataJson, "bleConnectionState");
    cJSON* pCurrentTimeMsJson       = cJSON_GetObjectItemCaseSensitive(pDataJson, "currentTimeMs");
    cJSON* pFirmwareVersionJson     = cJSON_GetObjectItemCaseSensitive(pDataJson, "firmwareVersion");

    if (pWifiConnectionStateJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no wifiConnectionState or wrong format");
        return false;
    }

    if (pBleConnectionStateJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no bleConnectionState or wrong format");
        return false;
    }

    if (pCurrentTimeMsJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no currentTimeMs or wrong format");
        return false;
    }

    if (pFirmwareVersionJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no firmwareVersion or wrong format");
        return false;
    }

    std::string firmware = pFirmwareVersionJson->valuestring;

    pOutput->isWiFiConnected          = cJSON_IsTrue(pWifiConnectionStateJson);
    pOutput->isBleConnected           = cJSON_IsTrue(pBleConnectionStateJson);
    pOutput->currentTimeFromStartupMs = static_cast<uint32_t>(pCurrentTimeMsJson->valueint);
    strcpy(pOutput->firmwareVersion, firmware.c_str());

    return true;
}

/**
 * @brief processOtaUpdateLink - function parsing message with a link from which OTA shall be performed
 * @param pDataJson - cJSON* object containing the payload of the message
 * @param pOutput - pointer to TOtaUpdateLink structure containing extracted payload
 * @return boolean value representing success of the operation
 */
static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput)
{
    cJSON* pOtaUpdateLinkJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "otaUpdateLink");

    if (pOtaUpdateLinkJson == nullptr)
    {
        LOG_INFO("Could not parse Json, no OtaUpdateLink or wrong format");
        return false;
    }

    strcpy(pOutput->firmwareLink, pOtaUpdateLinkJson->valuestring);

    return true;
}

bool getDataJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* info, cJSON** dataJson)
{
    cJSON* pDeviceProvisioningJson = preprocessInputMessage(inputMessage);

    if (pDeviceProvisioningJson == nullptr)
    {
        LOG_INFO("Could not parse JSON");
        return false;
    }

    cJSON* pOperationIdJson = cJSON_GetObjectItemCaseSensitive(pDeviceProvisioningJson, "operationId");

    if (pOperationIdJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no operation id or wrong format");
        cJSON_Delete(pDeviceProvisioningJson);
        return false;
    }
    cJSON* pStatusJson = cJSON_GetObjectItemCaseSensitive(pDeviceProvisioningJson, "status");

    if (pStatusJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no status or wrong format");
        cJSON_Delete(pDeviceProvisioningJson);
        return false;
    }

    cJSON* pRegistrationStateJson = cJSON_GetObjectItemCaseSensitive(pDeviceProvisioningJson, "registrationState");

    if (pRegistrationStateJson != nullptr)
    {
        cJSON* pAssignedHubJson = cJSON_GetObjectItemCaseSensitive(pRegistrationStateJson, "assignedHub");

        if (pAssignedHubJson != nullptr)
        {
            LOG_INFO("assignedHub present in JSON");
            info->assignedHub = std::string(pAssignedHubJson->valuestring);
        }

        cJSON* pDeviceIdJson = cJSON_GetObjectItemCaseSensitive(pRegistrationStateJson, "deviceId");

        if (pDeviceIdJson != nullptr)
        {
            LOG_INFO("deviceId present in JSON");
            info->deviceId = std::string(pDeviceIdJson->valuestring);
        }
    }

    info->operationId = std::string(pOperationIdJson->valuestring);
    info->status      = std::string(pStatusJson->valuestring);

    cJSON_Delete(pDeviceProvisioningJson);

    return true;
}

/**
 * @brief preprocessInputMessage - function parsing the message in json format into cJSON* object and checking for
 * errors. NOTE: after executing this function, it is it's user responsibility to free the memory allocated for returned
 * cJSON object after it will no longer be used - remember to call cJSON_Delete(returnedCjsonObject)
 * @param inputMessage - std::string with message in the JSON format
 * @return cJSON* object with the parsed message
 */

cJSON* preprocessInputMessage(const std::string& inputMessage)
{
    cJSON* pInputMessageJson = cJSON_Parse(inputMessage.c_str());

    if (pInputMessageJson == nullptr)
    {
        const char* pJsonError = cJSON_GetErrorPtr();
        if (pJsonError)
        {
            LOG_ERROR("Error parsing JSON occurred before %p", pJsonError);
            return nullptr;
        }
    }

    return pInputMessageJson;
}

/***** MESSAGE PREPARATION *****/

std::string prepareDeviceStatusMessage(const json_parser::TDeviceStatus& deviceStatus, uint32_t msgCounter)
{
    cJSON* pDeviceStatusJson = deviceStatusToJson(deviceStatus, msgCounter);

    if (pDeviceStatusJson == nullptr)
    {
        LOG_INFO("Error while preparing deviceStatusDataJson in prepareDeviceStatusMessage()");
        return "";
    }

    char* pDeviceStatusJsonCString = cJSON_Print(pDeviceStatusJson);
    if (pDeviceStatusJsonCString == nullptr)
    {
        LOG_INFO("Error while printing deviceStatusCommandJson");
        cJSON_Delete(pDeviceStatusJson);
        return std::string("");
    }
    std::string deviceStatusMessage = std::string(pDeviceStatusJsonCString);
    free(pDeviceStatusJsonCString); // NOLINT memory allocated by cJSON_Print needs to be freed manually
    cJSON_Delete(pDeviceStatusJson);

    return deviceStatusMessage;
}

std::string
prepareDeviceCreateProvisioningMessage(char (&deviceId)[prot::cloud_set_credentials::CLOUD_DEVICE_ID_LENGTH])
{
    cJSON* pDeviceProvisioningDataJson = cJSON_CreateObject();

    cJSON* pRegistrationIdJson = cJSON_CreateString(deviceId);
    if (!(cJSON_AddItemToObject(pDeviceProvisioningDataJson, "registrationId", pRegistrationIdJson)))
    {
        LOG_INFO("Cannot add registrationId JSON to pDeviceProvisioningDataJson");
        cJSON_Delete(pDeviceProvisioningDataJson);
        return std::string("");
    }

    if (pDeviceProvisioningDataJson == nullptr)
    {
        LOG_INFO("Error while preparing pDeviceProvisioningDataJson");
        return std::string("");
    }

    char* pDeviceProvisioningDataJsonCString = cJSON_Print(pDeviceProvisioningDataJson);
    if (pDeviceProvisioningDataJsonCString == nullptr)
    {
        LOG_INFO("Error while printing pDeviceProvisioningDataJson");
        cJSON_Delete(pDeviceProvisioningDataJson);
        return std::string("");
    }
    std::string deviceCreateProvisioningMessage = std::string(pDeviceProvisioningDataJsonCString);
    free(pDeviceProvisioningDataJsonCString); // NOLINT memory allocated by cJSON_Print needs to be freed manually
    cJSON_Delete(pDeviceProvisioningDataJson);

    return deviceCreateProvisioningMessage;
}

bool parseJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* pDeviceProvisioningInfo)
{
    cJSON* pDataJson = nullptr;
    bool   result    = getDataJsonDeviceProvisioning(inputMessage, pDeviceProvisioningInfo, &pDataJson);
    if (!result)
    {
        LOG_INFO("Error has occured while extracting frame");
        if (pDataJson != nullptr)
        {
            cJSON_Delete(pDataJson);
        }
        return false;
    }

    return true;
}

bool parseFirmwareInfo(cJSON* pInputJson, TFirmwareInfo* pFirmwareInfo)
{

    cJSON* pFirmwareInfoJson = cJSON_GetObjectItemCaseSensitive(pInputJson, "firmware_info");

    if (pFirmwareInfoJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no firmware_info data");
        cJSON_Delete(pFirmwareInfoJson);
        return false;
    }

    cJSON* pFirmwareVersionJson = cJSON_GetObjectItemCaseSensitive(pFirmwareInfoJson, "firmware_version");
    if (pFirmwareVersionJson == nullptr)
    {
        LOG_ERROR("Could not find version information inside firmware_info JSON");
        cJSON_Delete(pFirmwareInfoJson);
        return false;
    }

    cJSON* pFirmwareUrlJson = cJSON_GetObjectItemCaseSensitive(pFirmwareInfoJson, "url");
    if (pFirmwareUrlJson == nullptr)
    {
        LOG_ERROR("Could not find url inside firmware_info JSON");
        cJSON_Delete(pFirmwareInfoJson);
        return false;
    }

    pFirmwareInfo->version     = pFirmwareVersionJson->valueint;
    pFirmwareInfo->firmwareUrl = std::string(pFirmwareUrlJson->valuestring);

    cJSON_Delete(pFirmwareInfoJson);

    return true;
}

/***** REPORTED DEVICE TWINS *******/

cJSON* initiateReportedJson()
{
    cJSON* pOutJson = cJSON_CreateObject();

    if (pOutJson == nullptr)
    {
        LOG_ERROR("Could not create pOutJson");
        return nullptr;
    }
    cJSON* pReportedJson = cJSON_CreateObject();
    if (pReportedJson == nullptr)
    {
        LOG_ERROR("Could not create pReportedJson");
        return nullptr;
    }

    cJSON_AddItemToObject(pOutJson, "reported", pReportedJson);

    return pOutJson;
}

bool addFirmwareInfoToReportedJson(cJSON** ppReportedJson, const TFirmwareInfo& firmwareInfo)
{
    if (ppReportedJson == nullptr)
    {
        LOG_ERROR("Received nullptr instead of reported JSON");
        return false;
    }

    cJSON* firmwareInfoJson = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(firmwareInfoJson, FIRMWARE_INFO_VERSION_KEY, firmwareInfo.version))
    {
        LOG_ERROR("Could not add FIRMWARE_INFO_VERSION_KEY to reported JSON");
        cJSON_Delete(firmwareInfoJson);
        return false;
    }

    if (!cJSON_AddStringToObject(firmwareInfoJson, FIRMWARE_INFO_URL_KEY, firmwareInfo.firmwareUrl.c_str()))
    {
        LOG_ERROR("Could not add FIRMWARE_INFO_URL to reported JSON");
        cJSON_Delete(firmwareInfoJson);
        return false;
    }

    if (!cJSON_AddItemToObject(*ppReportedJson, FIRMWARE_INFO_KEY, firmwareInfoJson))
    {
        LOG_ERROR("Could not add firmwareInfoJson to reported JSON");
        cJSON_Delete(firmwareInfoJson);
        return false;
    }

    return true;
}

std::string prepareReportedMessage(cJSON* pReportedJson)
{
    char* reportedMessageCString = cJSON_Print(pReportedJson);

    if (reportedMessageCString == nullptr)
    {
        LOG_ERROR("Could not print reported JSON");
        return std::string("");
    }

    std::string reportedMessage = std::string(reportedMessageCString);
    free(reportedMessageCString);

    return reportedMessage;
}

/***** STRUCTURE TO JSON *****/

cJSON* deviceStatusToJson(const TDeviceStatus& deviceStatus, uint32_t msgCounter)
{
    cJSON* pDeviceStatusJson        = cJSON_CreateObject();
    cJSON* pWifiConnectionStateJson = cJSON_CreateBool(deviceStatus.isWiFiConnected);
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "wifiConnectionState", pWifiConnectionStateJson)))
    {
        LOG_INFO("Cannot add wifiConnectionState JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pBleConnectionStateJson = cJSON_CreateBool(deviceStatus.isBleConnected);
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "bleConnectionState", pBleConnectionStateJson)))
    {
        LOG_INFO("Cannot add bleConnectionState JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pCurrentTimeFromStartupMsJson = cJSON_CreateNumber(deviceStatus.currentTimeFromStartupMs);
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "currentTimeFromStartupMs", pCurrentTimeFromStartupMsJson)))
    {
        LOG_INFO("Cannot add currentTimeMs JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pFirmwareVersionJson = cJSON_CreateString(deviceStatus.getFirmwareVersion().c_str());
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "firmwareVersion", pFirmwareVersionJson)))
    {
        LOG_INFO("Cannot add firmwareVersion JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pCurrentLocalTimeJson = cJSON_CreateString(deviceStatus.getCurrentLocalTime().c_str());
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "currentLocalTime", pCurrentLocalTimeJson)))
    {
        LOG_INFO("Cannot add currentLocalTime JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pPressureSensorValueJson = cJSON_CreateNumber(deviceStatus.pressureSensorValue);
    if (!(cJSON_AddItemToObject(pDeviceStatusJson, "pressureSensorValue", pPressureSensorValueJson)))
    {
        LOG_INFO("Cannot add pressureSensorValue JSON to deviceStatusJson");
        cJSON_Delete(pDeviceStatusJson);
        return nullptr;
    }

    cJSON* pOutputJson = cJSON_CreateObject();
    if (!(cJSON_AddItemToObject(pOutputJson, "status", pDeviceStatusJson)))
    {
        LOG_INFO("Cannot add deviceStatusJson to output JSON");
        cJSON_Delete(pDeviceStatusJson);
        cJSON_Delete(pOutputJson);
        return nullptr;
    }

    return pOutputJson;
}

/***** STRING GET FUNCTIONS FOR FRAME PRINTING *****/

std::string getStatusReportString(const TDeviceStatus& deviceStatus)
{
    std::string statusReportString =
        "wifiConnectionState: " + getBooleanString(deviceStatus.isWiFiConnected) + "\n" +
        "bleConnectionState: " + getBooleanString(deviceStatus.isBleConnected) + "\n" +
        "currentTimeFromStartupMs: " + std::to_string(deviceStatus.currentTimeFromStartupMs) + "\n" +
        "currentLocalTime: " + deviceStatus.currentLocalTime + "\n" +
        "firmwareVersion: " + deviceStatus.firmwareVersion + "\n";

    return statusReportString;
}

std::string getResponseString(const TResponse& responseData)
{
    std::string responseString = responseData.ACK ? "true" : "false";
    std::string output         = "ACK: " + responseString + "\n******************\n";
    return output;
}

std::string getOtaUpdateLinkString(const TOtaUpdateLink& otaUpdateLink)
{
    std::string output = "firmwareLink: " + std::string(otaUpdateLink.firmwareLink) + std::string("\n");

    return output;
}

std::string TDeviceStatus::getFirmwareVersion() const
{
    std::string output(firmwareVersion);

    return output;
}

std::string TDeviceStatus::getCurrentLocalTime() const
{
    std::string output(currentLocalTime);

    return output;
}


std::string getConnectionString(bool status)
{
    if (status)
        return std::string("connected");
    else
        return std::string("not connected");
}

std::string getBooleanString(bool status)
{
    if (status)
        return std::string("true");
    else
        return std::string("false");
}

} // namespace json_parser
