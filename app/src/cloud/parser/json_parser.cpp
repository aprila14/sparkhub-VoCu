// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "jsonParser";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "json_parser.h"

#include "json_parser_defines.h"

#include <cctype>

// #include "esp_heap_caps.h"

namespace
{
uint32_t MAX_MANIFEST_STRING_LENGTH  = 1024U;
uint32_t MAX_UPDATE_ID_STRING_LENGTH = 150;
} // unnamed namespace

extern const char* DEVICE_PROVISIONING_MODEL_ID;

namespace json_parser
{

const char* DEVICE_UPDATE_KEY = "deviceUpdate";

#if !TESTING // directive added to avoid double declaration of function
static bool        processResponse(cJSON* pDataJson, TResponse* pOutput);
static bool        processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);
static EMsgMethod  extractMsgMethod(const std::string& inputMessage);
static cJSON*      deviceStatusToJson(const TDeviceStatus& deviceStatus, uint32_t msgCounter);
static cJSON*      dataJsonToParamsJson(cJSON* pDataJson, EMsgCode msgCode, uint32_t msgCounter);
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
 * @brief processResponse - function parsing any of the response messages (e.g. response to Heartbeat message) into
 * TResponse structure All response messages have the same structure, therefore they do not need separate functions for
 * parsing
 * @param pDataJson - cJSON* object containing the payload of the message
 * @param pOutput - pointer to TReponse structure containing extracted payload
 * @return boolean value representing succes of the operation
 */

bool processResponse(cJSON* pDataJson, TResponse* pOutput)
{
    cJSON* pAckJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "ACK");

    if (pAckJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no ACK or wrong format");
        return false;
    }

    pOutput->ACK = cJSON_IsTrue(pAckJson);

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

/***** FRAME INTERPRETATION *****/

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
 * @brief extractMsgMethod - function recognizing message method (e.g. RPC Command, or message from the widget) based on
 * the provided std::string with a message in JSON format
 * @param inputMessage - std::string containing message to identify
 * @return EMsgMethod with the method recognized by the function
 */

EMsgMethod extractMsgMethod(const std::string& inputMessage)
{
    cJSON* pMessageJson = preprocessInputMessage(inputMessage);

    if (pMessageJson == nullptr)
    {
        LOG_INFO("Could not parse JSON");
        return EMsgMethod::MSG_METHOD_FAILED;
    }

    cJSON* pMethodJson = cJSON_GetObjectItemCaseSensitive(pMessageJson, "method");

    if (pMethodJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no method or wrong format");
        cJSON_Delete(pMessageJson);
        return EMsgMethod::MSG_METHOD_FAILED;
    }

    std::string methodString = std::string(pMethodJson->valuestring);

    cJSON_Delete(pMessageJson);

    if (methodString == "rpcCommand")
    {
        return EMsgMethod::MSG_METHOD_RPC_COMMAND;
    }
    else if (methodString == "setLightIntensity")
    {
        return EMsgMethod::MSG_METHOD_SET_LIGHT_INTENSITY;
    }
    else if (methodString == "getLightIntensity")
    {
        return EMsgMethod::MSG_METHOD_GET_LIGHT_INTENSITY;
    }
    else
    {
        return EMsgMethod::MSG_METHOD_UNKNOWN;
    }
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

cJSON* prepareInstalledUpdateIdJson(const TUpdateId& updateId)
{
    char installedUpdateIdCString[MAX_UPDATE_ID_STRING_LENGTH];

    if (sprintf(
            installedUpdateIdCString,
            "{\"provider\":\"%s\",\"name\":\"%s\",\"version\":\"%s\"}",
            updateId.providerName,
            updateId.updateName,
            updateId.firmwareVersion) < 0)
    {
        LOG_ERROR("Failed to prepare installedUpdateId string");
        return nullptr;
    }

    cJSON* installedUpdateIdJson = cJSON_CreateString(installedUpdateIdCString);

    if (installedUpdateIdJson == nullptr)
    {
        LOG_ERROR("Could not create installedUpdateIdJson");
        return nullptr;
    }

    return installedUpdateIdJson;
}

cJSON* prepareDevicePropertiesJson()
{
    cJSON* pDevicePropertiesJson = cJSON_CreateObject();

    if (pDevicePropertiesJson == nullptr)
    {
        LOG_ERROR("Could not allocate memory for (empty) DeviceUpdate report JSON");
        return nullptr;
    }

    cJSON* pManufacturerJson = cJSON_CreateString("sparkhub");

    if (!cJSON_AddItemToObject(pDevicePropertiesJson, "manufacturer", pManufacturerJson))
    {
        LOG_ERROR("Cannot add manufacturer string to DeviceUpdate report JSON");
        cJSON_Delete(pDevicePropertiesJson);
        return nullptr;
    }

    cJSON* pModelJson = cJSON_CreateString("sparkhub-iot-levelsense");

    if (!cJSON_AddItemToObject(pDevicePropertiesJson, "model", pModelJson))
    {
        LOG_ERROR("Cannot add model string to DeviceUpdate report JSON");
        cJSON_Delete(pDevicePropertiesJson);
        return nullptr;
    }

    cJSON* pContractModelIdJson = cJSON_CreateString(DEVICE_PROVISIONING_MODEL_ID);

    if (!cJSON_AddItemToObject(pDevicePropertiesJson, "contractModelId", pContractModelIdJson))
    {
        LOG_ERROR("Cannot add contract model id string to DeviceUpdate report JSON");
        cJSON_Delete(pDevicePropertiesJson);
        return nullptr;
    }

    cJSON* pAduVerJson = cJSON_CreateString("DU;agent/1.0.0");
    if (!cJSON_AddItemToObject(pDevicePropertiesJson, "aduVer", pAduVerJson))
    {
        LOG_ERROR("Cannot add aduVer string to DeviceUpdate report JSON");
        cJSON_Delete(pDevicePropertiesJson);
        return nullptr;
    }

    return pDevicePropertiesJson;
}

cJSON* prepareAgentJson(const TUpdateId& updateId, uint8_t state, const TWorkflowData& workflowData)
{
    cJSON* pAgentJson = cJSON_CreateObject();
    if (pAgentJson == nullptr)
    {
        LOG_ERROR("Could not allocate memory for (empty) Agent JSON");
        return nullptr;
    }

    cJSON* pDevicePropertiesJson = prepareDevicePropertiesJson();
    if (pDevicePropertiesJson == nullptr)
    {
        LOG_ERROR("Could not create DeviceProperties JSON");
        return nullptr;
    }

    if (!cJSON_AddItemToObject(pAgentJson, "deviceProperties", pDevicePropertiesJson))
    {
        LOG_ERROR("Could not add deviceProperties JSON to Agent JSON");
        cJSON_Delete(pAgentJson);
        return nullptr;
    }

    if (workflowData.workflowId[0] != 0)
    {
        cJSON* pWorkflowJson = cJSON_CreateObject();
        if (pWorkflowJson == nullptr)
        {
            LOG_ERROR("Could not allocate memory for (empty) workflow JSON");
            return nullptr;
        }

        cJSON* pWorkflowIdJson = cJSON_CreateString(workflowData.workflowId);
        if (!cJSON_AddItemToObject(pWorkflowJson, "id", pWorkflowIdJson))
        {
            LOG_ERROR("Could not add workflow id to workflow JSON");
            cJSON_Delete(pWorkflowJson);
            cJSON_Delete(pWorkflowIdJson);
            return nullptr;
        }

        if (!cJSON_AddNumberToObject(pWorkflowJson, "action", static_cast<uint8_t>(workflowData.deviceUpdateAction)))
        {
            LOG_ERROR("Could not add action to workflow JSON");
            cJSON_Delete(pWorkflowJson);
            return nullptr;
        }

        if (!cJSON_AddItemToObject(pAgentJson, "workflow", pWorkflowJson))
        {
            LOG_ERROR("Could not add workflow JSON to agent JSON");
            cJSON_Delete(pWorkflowJson);
            return nullptr;
        }
    }

    cJSON* pCompatPropertyNames = cJSON_CreateString("manufacturer,model");

    if (!cJSON_AddItemToObject(pAgentJson, "compatPropertyNames", pCompatPropertyNames))
    {
        LOG_ERROR("Could not add compatPropertyNames JSON to Agent JSON");
        cJSON_Delete(pAgentJson);
        cJSON_Delete(pCompatPropertyNames);
        return nullptr;
    }

    if (!cJSON_AddNumberToObject(pAgentJson, "state", state))
    {
        LOG_ERROR("Could not add state to Agent JSON");
        cJSON_Delete(pAgentJson);
        return nullptr;
    }

    cJSON* installedUpdateIdJson = prepareInstalledUpdateIdJson(updateId);
    if (installedUpdateIdJson == nullptr)
    {
        LOG_ERROR("Could not prepare installedUpdateIdJson");
        cJSON_Delete(pAgentJson);
        return nullptr;
    }

    if (!cJSON_AddItemToObject(pAgentJson, "installedUpdateId", installedUpdateIdJson))
    {
        LOG_ERROR("Could not add installedUpdateIdJson to agentJson");
        cJSON_Delete(pAgentJson);
        return nullptr;
    }

    return pAgentJson;
}

std::string prepareDeviceUpdateReport(const TUpdateId& updateId, uint8_t state, const TWorkflowData& workflowData)
{
    cJSON* pDeviceUpdateJson = cJSON_CreateObject();

    if (pDeviceUpdateJson == nullptr)
    {
        LOG_ERROR("Could not allocate memory for (empty) DeviceUpdate report JSON");
        return std::string("");
    }

    cJSON* pAgentJson = prepareAgentJson(updateId, state, workflowData);
    if (pAgentJson == nullptr)
    {
        LOG_ERROR("Did not manage to prepare Agent JSON");
        cJSON_Delete(pDeviceUpdateJson);
        return nullptr;
    }

    if (!cJSON_AddItemToObject(pDeviceUpdateJson, "agent", pAgentJson))
    {
        LOG_ERROR("Did not manage to add Agent JSON to deviceUpdate JSON");
        cJSON_Delete(pDeviceUpdateJson);
        return nullptr;
    }

    cJSON* pUnknownParameterJson = cJSON_CreateString("c");
    if (!cJSON_AddItemToObject(pDeviceUpdateJson, "__t", pUnknownParameterJson))
    {
        LOG_ERROR("Cannot add unknown parameter (__t)  to DeviceUpdate report JSON");
        cJSON_Delete(pDeviceUpdateJson);
        return std::string("");
    }

    cJSON* pDeviceUpdateReportJson = cJSON_CreateObject();
    if (pDeviceUpdateReportJson == nullptr)
    {
        LOG_ERROR("Failed to create (empty) DeviceUpdateReport JSON");
        cJSON_Delete(pDeviceUpdateJson);
        return std::string("");
    }

    if (!cJSON_AddItemToObject(pDeviceUpdateReportJson, "deviceUpdate", pDeviceUpdateJson))
    {
        LOG_ERROR("Failed to add pDeviceUpdateJson to pDeviceUpdateReportJson");
        cJSON_Delete(pDeviceUpdateJson);
        cJSON_Delete(pDeviceUpdateReportJson);
        return std::string("");
    }

    char* deviceUpdateReportCString = cJSON_Print(pDeviceUpdateReportJson);
    if (deviceUpdateReportCString == nullptr)
    {
        LOG_ERROR("Error while preparing deviceUpdateReportCString");
        cJSON_Delete(pDeviceUpdateJson);
        return std::string("");
    }

    std::string deviceUpdateReport = std::string(deviceUpdateReportCString);
    free(deviceUpdateReportCString); // NOLINT memory allocated by cJSON_Print needs to be freed manually
    cJSON_Delete(pDeviceUpdateReportJson);

    return deviceUpdateReport;
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

bool parseUpdateManifest(cJSON* pInputJson, TUpdateManifest* pUpdateManifest)
{
    if (strlen(pInputJson->valuestring) > MAX_MANIFEST_STRING_LENGTH)
    {
        LOG_ERROR(
            "Manifest string length (%d) exceeds maximum length (%d)",
            strlen(pInputJson->valuestring),
            MAX_MANIFEST_STRING_LENGTH);
        return false;
    }

    char manifestCString[MAX_MANIFEST_STRING_LENGTH];

    strncpy(manifestCString, pInputJson->valuestring, strlen(pInputJson->valuestring));

    cJSON* pUpdateManifestJson = cJSON_Parse(manifestCString);

    if (pUpdateManifestJson == nullptr)
    {
        LOG_ERROR("Could not parse updateManifest JSON");
        return false;
    }

    cJSON* pUpdateIdJson = cJSON_GetObjectItemCaseSensitive(pUpdateManifestJson, "updateId");
    if (pUpdateIdJson == nullptr)
    {
        LOG_ERROR("Could not find updateIdJson inside updateManifest JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pVersionJson = cJSON_GetObjectItemCaseSensitive(pUpdateIdJson, "version");
    if (pVersionJson == nullptr)
    {
        LOG_ERROR("Could not find versionJson inside updateId JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    if (strlen(pVersionJson->valuestring) > MAX_OTA_VERSION_STRING_LENGTH)
    {
        LOG_ERROR("Version string in update manifest is too long");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pProviderJson = cJSON_GetObjectItemCaseSensitive(pUpdateIdJson, "provider");
    if (pProviderJson == nullptr)
    {
        LOG_ERROR("Could not find providerJson inside updateId JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    if (strlen(pProviderJson->valuestring) > MAX_PROVIDER_NAME_LENGTH)
    {
        LOG_ERROR("Provider name in update manifest is too long");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pNameJson = cJSON_GetObjectItemCaseSensitive(pUpdateIdJson, "name");
    if (pNameJson == nullptr)
    {
        LOG_ERROR("Could not find nameJson inside updateId JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    if (strlen(pNameJson->valuestring) > MAX_OTA_UPDATE_NAME_LENGTH)
    {
        LOG_ERROR("Update name in update manifest is too long");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pInstructionsJson = cJSON_GetObjectItemCaseSensitive(pUpdateManifestJson, "instructions");
    if (pInstructionsJson == nullptr)
    {
        LOG_ERROR("Could not find instructions inside updateManifest JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pStepsJson = cJSON_GetObjectItemCaseSensitive(pInstructionsJson, "steps");
    if (pStepsJson == nullptr)
    {
        LOG_ERROR("Could not find steps inside instructions JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pStepsArrayElement = cJSON_GetArrayItem(pStepsJson, 0);
    if (pStepsArrayElement == nullptr)
    {
        LOG_ERROR("Could not find 0th element of the pStepsJson array");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pStepsFilesJson = cJSON_GetObjectItemCaseSensitive(pStepsArrayElement, "files");
    if (pStepsFilesJson == nullptr)
    {
        LOG_ERROR("Could not find files inside steps JSON");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    cJSON* pStepsFilesArrayElement = cJSON_GetArrayItem(pStepsFilesJson, 0);
    if (pStepsFilesArrayElement == nullptr)
    {
        LOG_ERROR("Could not find 0th element of pStepsFilesJson array");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    if (strlen(pStepsFilesArrayElement->valuestring) > MAX_FILE_KEY_LENGTH)
    {
        LOG_ERROR("File key too long");
        cJSON_Delete(pUpdateManifestJson);
        return false;
    }

    strncpy(pUpdateManifest->updateId.firmwareVersion, pVersionJson->valuestring, strlen(pVersionJson->valuestring));
    strncpy(pUpdateManifest->updateId.providerName, pProviderJson->valuestring, strlen(pProviderJson->valuestring));
    strncpy(pUpdateManifest->updateId.updateName, pNameJson->valuestring, strlen(pNameJson->valuestring));

    strncpy(
        pUpdateManifest->fileKey, pStepsFilesArrayElement->valuestring, strlen(pStepsFilesArrayElement->valuestring));

    cJSON_Delete(pUpdateManifestJson);

    LOG_INFO("Parsing update manifest was correct");

    return true;
}

bool parseWorkflow(cJSON* pWorkflowJson, TWorkflowData* pWorkflowData)
{
    cJSON* pActionJson = cJSON_GetObjectItem(pWorkflowJson, "action");
    if (pActionJson == nullptr)
    {
        LOG_ERROR("Could not find action data in workflow JSON");
        return false;
    }

    cJSON* pIdJson = cJSON_GetObjectItem(pWorkflowJson, "id");
    if (pIdJson == nullptr)
    {
        LOG_ERROR("Could not find id in workflow JSON");
        return false;
    }

    EDeviceUpdateAction deviceUpdateAction = static_cast<EDeviceUpdateAction>(pActionJson->valueint);

    if ((deviceUpdateAction != EDeviceUpdateAction::ACTION_DOWNLOAD) &&
        (deviceUpdateAction != EDeviceUpdateAction::ACTION_CANCEL))
    {
        LOG_ERROR("Unrecognized workflow id obtained");
        return false;
    }

    if (strlen(pIdJson->valuestring) > MAX_WORKFLOW_ID_LENGTH)
    {
        LOG_ERROR("Received too long workflow id");
        return false;
    }

    pWorkflowData->deviceUpdateAction = static_cast<EDeviceUpdateAction>(pActionJson->valueint);
    strncpy(pWorkflowData->workflowId, pIdJson->valuestring, strlen(pIdJson->valuestring));

    return true;
}

bool parseDeviceUpdate(cJSON* pInputJson, TDeviceUpdate* pDeviceUpdate)
{
    cJSON* pDeviceUpdateJson = cJSON_GetObjectItemCaseSensitive(pInputJson, DEVICE_UPDATE_KEY);

    if (pDeviceUpdateJson == nullptr)
    {
        LOG_INFO("Could not parse JSON, no deviceUpdate data");
        return false;
    }

    cJSON* pServiceJson = cJSON_GetObjectItemCaseSensitive(pDeviceUpdateJson, "service");
    if (pServiceJson == nullptr)
    {
        LOG_ERROR("Could not find service data inside deviceUpdate JSON");
        return false;
    }

    cJSON* pWorkflowJson = cJSON_GetObjectItem(pServiceJson, "workflow");
    if (pWorkflowJson == nullptr)
    {
        LOG_ERROR("Could not find workflow data inside deviceUpdate JSON");
        return false;
    }

    if (!parseWorkflow(pWorkflowJson, &pDeviceUpdate->workflowData))
    {
        LOG_ERROR("Did not manage to parse workflow data in deviceUpdate JSON");
        return false;
    }

    cJSON* pUpdateManifestJson = cJSON_GetObjectItemCaseSensitive(pServiceJson, "updateManifest");
    if (pUpdateManifestJson == nullptr)
    {
        LOG_ERROR("Could not find updateManifest data inside deviceUpdate JSON");
        return false;
    }

    if (!parseUpdateManifest(pUpdateManifestJson, &pDeviceUpdate->updateManifest))
    {
        LOG_ERROR("Did not manage to parse updateManifest");
        return false;
    }

    cJSON* pFileUrlsJson = cJSON_GetObjectItemCaseSensitive(pServiceJson, "fileUrls");
    if (pFileUrlsJson == nullptr)
    {
        LOG_ERROR("Did not manage to parse fileUrls JSON");
        return false;
    }

    cJSON* pUrlJson = cJSON_GetObjectItemCaseSensitive(pFileUrlsJson, pDeviceUpdate->updateManifest.fileKey);
    if (pUrlJson == nullptr)
    {
        LOG_ERROR("Did not manage to parse urlJson JSON");
        return false;
    }

    if (strlen(pUrlJson->valuestring) > MAX_OTA_URL_LENGTH)
    {
        LOG_ERROR("File URL too long, length: %d, max length: %d", strlen(pUrlJson->valuestring), MAX_OTA_URL_LENGTH);
        return false;
    }

    strncpy(pDeviceUpdate->fileUrl, pUrlJson->valuestring, strlen(pUrlJson->valuestring));

    LOG_INFO("Parsing Device Update was correct");

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

/**
 * @brief dataJsonToParamsJson - function preparing paramsJSON based on provided dataJson, msgCode and msgCounter
 * (see cloud communication protocol description for reference).
 * NOTE: after calling the function, it is the user's responsibility to free the memory allocated for the returned
 * cJSON* object. Call cJSON_Delete(returnedValue) after returned cJSON* will no longer be used
 * @param pDataJson - cJSON* object with dataJson
 * @param msgCode - msgCode to add to paramsJson
 * @param msgCounter - message number to add to paramsJson
 * @return cJSON* object with paramsJson.
 */
cJSON* dataJsonToParamsJson(cJSON* pDataJson, EMsgCode msgCode, uint32_t msgCounter)
{
    cJSON* pParamsJson = cJSON_CreateObject();

    if (pParamsJson == nullptr)
    {
        LOG_INFO("Error while creating paramsJson - dataJsonToParamsJson()");
        return nullptr;
    }

    if (!(cJSON_AddItemToObject(pParamsJson, "data", pDataJson)))
    {
        LOG_INFO("Cannot add dataJson to paramsJson");
        cJSON_Delete(pParamsJson);
        return nullptr;
    }

    cJSON* pMsgCodeJson    = cJSON_CreateNumber(static_cast<int>(msgCode));
    cJSON* pMsgCounterJson = cJSON_CreateNumber(msgCounter);

    if (!(cJSON_AddItemToObject(pParamsJson, "msgCode", pMsgCodeJson)))
    {
        LOG_INFO("Cannot add msgCodeJson to paramsJson");
        cJSON_Delete(pParamsJson);
        return nullptr;
    }

    if (!(cJSON_AddItemToObject(pParamsJson, "msgCounter", pMsgCounterJson)))
    {
        LOG_INFO("Cannot add msgCounterJson to paramsJson");
        cJSON_Delete(pParamsJson);
        return nullptr;
    }

    return pParamsJson;
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

/***** HELPER FUNCTIONS *****/

std::string getMsgMethodString(EMsgMethod msgMethod)
{
    switch (msgMethod)
    {
        case EMsgMethod::MSG_METHOD_FAILED:
            return std::string("MSG_METHOD_FAILED");
        case EMsgMethod::MSG_METHOD_GET_LIGHT_INTENSITY:
            return std::string("MSG_METHOD_GET_LIGHT_INTENSITY");
        case EMsgMethod::MSG_METHOD_RPC_COMMAND:
            return std::string("MSG_METHOD_RPC_COMMAND");
        case EMsgMethod::MSG_METHOD_SET_LIGHT_INTENSITY:
            return std::string("MSG_METHOD_SET_LIGHT_INTENSITY");
        case EMsgMethod::MSG_METHOD_UNKNOWN:
            return std::string("MSG_METHOD_UNKNOWN");
        default:
            return std::string("MSG_METHOD_ERROR");
    }
}

std::string getMsgCodeString(EMsgCode msgCode)
{
    switch (msgCode)
    {
        case EMsgCode::MSG_HEARTBEAT:
            return std::string("MSG_HEARTBEAT");
        case EMsgCode::MSG_HEARTBEAT_RESPONSE:
            return std::string("MSG_HEARTBEAT_RESPONSE");
        case EMsgCode::MSG_STATUS_REPORT:
            return std::string("MSG_STATUS_REPORT");
        case EMsgCode::MSG_STATUS_REPORT_RESPONSE:
            return std::string("MSG_STATUS_REPORT_RESPONSE");
        case EMsgCode::MSG_OTA_UPDATE_LINK:
            return std::string("MSG_OTA_UPDATE_LINK");
        case EMsgCode::MSG_OTA_UPDATE_LINK_RESPONSE:
            return std::string("MSG_OTA_UPDATE_LINK_RESPONSE");
        default:
            return std::string("MSG_UNKNOWN_MESSAGE");
    }
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
