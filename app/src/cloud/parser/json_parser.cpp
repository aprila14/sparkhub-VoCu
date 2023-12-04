// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "jsonParser";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "json_parser.h"

#include <cctype>

// #include "esp_heap_caps.h"

namespace json_parser
{

#if !TESTING // directive added to avoid double declaration of function
    static cJSON *preprocessInputMessage(const std::string &inputMessage);
    static bool parseJsonRpcCommand(const std::string &inputMessage, TFrame *pFrame);
    static cJSON *dataJsonToRpcCommandJson(cJSON *pDataJson, EMsgCode msgCode, uint32_t msgCounter);
    static cJSON *deviceStatusToJson(const TDeviceStatus &deviceStatus);
    static cJSON *heartbeatToJson(const THeartbeat &heartbeatStruct);
    static bool processSetLightIntensityLevel(cJSON *pDataJson, TSetLightLevel *pOutput);
    static bool parseWidgetSetLightIntensityLevelCommand(std::string inputMessage, TSetLightLevel *pOutput);
    static bool processHeartbeat(cJSON *pDataJson, THeartbeat *pOutput);
    static bool processGetLightIntensityLevel(cJSON *pDataJson, TSetLightLevel *pOutput);
    static bool processResponse(cJSON *pDataJson, TResponse *pOutput);
    static bool processOtaUpdateLink(cJSON *pDataJson, TOtaUpdateLink *pOutput);
    static bool processTimeSlotsList(cJSON *pDataJson, TTimeSlotsList *pOutput);
    static bool processTimeSlotsListResponse(cJSON *pDataJson, TTimeSlotsListResponse *pOutput);
    static bool getDataJsonAndInitFrame(const std::string &inputMessage, TFrame *pFrame, cJSON **ppDataJson);
    static EMsgMethod extractMsgMethod(const std::string &inputMessage);
    static cJSON *preprocessInputMessage(const std::string &inputMessage);
    static cJSON *deviceStatusToJson(const TDeviceStatus &deviceStatus);
    static cJSON *heartbeatToJson(const THeartbeat &heartbeatStruct);
    static cJSON *widgetGetLightLevelToJson(const TSetLightLevel &lightLevelStruct);
    static cJSON *widgetSetLightLevelToJson(const TSetLightLevel &lightLevelStruct);
    static cJSON *dataJsonToParamsJson(cJSON *pDataJson, EMsgCode msgCode, uint32_t msgCounter);
    static cJSON *dataJsonToRpcCommandJson(cJSON *pDataJson, EMsgCode msgCode, uint32_t msgCounter);
    static std::string getLightIntensityString(const TSetLightLevel &setLightLevelData);
    static std::string getStatusReportString(const TDeviceStatus &deviceStatus);
    static std::string getHeartbeatString(const THeartbeat &heartbeatData);
    static std::string getResponseString(const TResponse &responseData);
    static std::string getMsgCodeString(EMsgCode msgCode);
    static std::string getConnectionString(bool status);
    static std::string getBooleanString(bool status);
    static std::string getOtaUpdateLinkString(const TOtaUpdateLink &otaUpdateLink);
    // static std::string getTimeSlotsListString(const TTimeSlotsList& timeSlotsList);

    int32_t extractTimeInMinutesFromString(const std::string timeString);
#endif

    // helper functions defined in the end of the file
    static std::string getMsgCodeString(EMsgCode msgCode);
    static std::string getConnectionString(bool status);
    static std::string getBooleanString(bool status);

    /***** JSON TO STRUCTURE *****/

    /**
     * @brief processSetLightIntensityLevel - function parsing setLightIntensityLevel message in cJSON format into the TSetLightLevel structure
     * @param pDataJson - input cJSON object with a data JSON containing the payload of the message
     * @param pOutput - TSetLightLevel structure with the extracted data
     * @return boolean value representing success of the operation
     */
    bool processSetLightIntensityLevel(cJSON *pDataJson, TSetLightLevel *pOutput)
    {
        cJSON *pLightIntensityLevelJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "lightIntensityLevel");

        if (pLightIntensityLevelJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no lightIntensityLevel data");
            return false;
        }

        pOutput->lightIntensityLevel = (uint8_t)pLightIntensityLevelJson->valueint;

        return true;
    }

    /**
     * @brief parseWidgetSetLightIntensityLevelCommand - function parsing the input SetLightLevel message from the widget directly into
     * TSetLightLevel structure
     * @param inputMessage - std::string containing message to parse and analyze
     * @param pOutput - TSetLighLevelStructure with the extracted data
     * @return boolean value representing success of the operation
     */

    bool parseWidgetSetLightIntensityLevelCommand(std::string inputMessage, TSetLightLevel *pOutput)
    {
        cJSON *pMessageJson = preprocessInputMessage(inputMessage);

        if (pMessageJson == nullptr)
        {
            LOG_INFO("Could not preproccess Widget SetLightIntensityLevel command, wrong format or an error occured during parsing");
            return false;
        }

        cJSON *pParamsJson = cJSON_GetObjectItemCaseSensitive(pMessageJson, "params");
        if (pParamsJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no params data or wrong format");
            cJSON_Delete(pMessageJson);
            return false;
        }
        pOutput->lightIntensityLevel = pParamsJson->valueint;

        cJSON_Delete(pMessageJson);
        return true;
    }

    /**
     * @brief processStatusReport - function parsing StatusReport message in a cJSON format into TDeviceStatus structure
     * @param pDataJson - input cJSON object with a data JSON containing message payload
     * @param pOutput - TDeviceStatus structure with the extracted data
     * @return boolean value representing success of the operation
     */

    bool processStatusReport(cJSON *pDataJson, TDeviceStatus *pOutput)
    {
        cJSON *pWifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "wifiConnectionState");
        cJSON *pBleConnectionStateJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "bleConnectionState");
        cJSON *pLightIntensityLevelJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "lightIntensityLevel");
        cJSON *pCurrentTimeMsJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "currentTimeMs");
        cJSON *pFirmwareVersionJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "firmwareVersion");

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

        if (pLightIntensityLevelJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no lightIntensityLevel or wrong format");
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

        pOutput->isWiFiConnected = cJSON_IsTrue(pWifiConnectionStateJson);
        pOutput->isBleConnected = cJSON_IsTrue(pBleConnectionStateJson);
        pOutput->lightIntensityLevel = static_cast<uint8_t>(pLightIntensityLevelJson->valueint);
        pOutput->currentTimeFromStartupMs = static_cast<uint32_t>(pCurrentTimeMsJson->valueint);
        strcpy(pOutput->firmwareVersion, firmware.c_str());

        return true;
    }

    /**
     * @brief processHeartbeat - function responsible for processing heartbeat message. Function added to enable handling of full protocol, it
     * should not be needed in ESP32 (since cloud should not send that message)
     * @param pDataJson - cJSON* object containing payload of the message
     * @param pOutput - pointer to THeartbeat structure containing extracted payload
     * @return boolean value - true if JSON was parsed correctly
     */
    bool processHeartbeat(cJSON *pDataJson, THeartbeat *pOutput)
    {
        cJSON *pHeartbeatJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "heartbeat");

        if (pHeartbeatJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no heartbeat or wrong format");
            return false;
        }

        pOutput->heartbeat = cJSON_IsTrue(pHeartbeatJson);

        return true;
    }

    /**
     * @brief processGetLightIntensityLevel - function parsing getLightIntensityLevel message in cJSON format into the TSetLightLevel structure.
     * Note: This function was planned only to standardize the data flow in the program, the message content is not relevant in
     * that case
     * @param pDataJson - cJSON* object containing the payload of the message
     * @param pOutput - pointer to TSetLightLevel structure containing extracted payload
     * @return success of the operation
     */
    bool processGetLightIntensityLevel(cJSON *pDataJson, TSetLightLevel *pOutput)
    {
        cJSON *pLightIntensityLevelJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "lightIntensityLevel");

        if (pLightIntensityLevelJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no lightIntensityLevel or wrong GetLightIntensityLevel frame format");
            return false;
        }

        pOutput->lightIntensityLevel = pLightIntensityLevelJson->valueint;

        return true;
    }

    /**
     * @brief processResponse - function parsing any of the response messages (e.g. response to Heartbeat message) into TResponse structure
     * All response messages have the same structure, therefore they do not need separate functions for parsing
     * @param pDataJson - cJSON* object containing the payload of the message
     * @param pOutput - pointer to TReponse structure containing extracted payload
     * @return boolean value representing succes of the operation
     */

    bool processResponse(cJSON *pDataJson, TResponse *pOutput)
    {
        cJSON *pAckJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "ACK");

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
    static bool processOtaUpdateLink(cJSON *pDataJson, TOtaUpdateLink *pOutput)
    {
        cJSON *pOtaUpdateLinkJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "otaUpdateLink");

        if (pOtaUpdateLinkJson == nullptr)
        {
            LOG_INFO("Could not parse Json, no OtaUpdateLink or wrong format");
            return false;
        }

        strcpy(pOutput->firmwareLink, pOtaUpdateLinkJson->valuestring);

        return true;
    }

    static bool processTimeSlotsList(cJSON *pDataJson, TTimeSlotsList *pOutput)
    {
        LOG_INFO("Processing TimeSlotsList message"); // TODO add parsing the incoming TimeSlotsList message here

        cJSON *timeSlotJson = nullptr;
        cJSON *timeSlotsListJson = cJSON_GetObjectItemCaseSensitive(pDataJson, "timeSlots");

        if (timeSlotsListJson == nullptr)
        {
            LOG_INFO("Could not parse Json, no timeSlots or wrong format");
            return false;
        }

        TTimeSlotsList timeSlotsList = {};

        cJSON_ArrayForEach(timeSlotJson, timeSlotsListJson)
        {
            cJSON *idJson = cJSON_GetObjectItemCaseSensitive(timeSlotJson, "id");
            if (idJson == nullptr)
            {
                LOG_INFO("Could not parse Json, no id in timeslot or wrong format");
                return false;
            }

            cJSON *fromJson = cJSON_GetObjectItemCaseSensitive(timeSlotJson, "from");
            if (fromJson == nullptr)
            {
                LOG_INFO("Could not parse Json, 'from' time not specified or wrong format");
                return false;
            }

            cJSON *toJson = cJSON_GetObjectItemCaseSensitive(timeSlotJson, "to");
            if (toJson == nullptr)
            {
                LOG_INFO("Could not parse Json, 'to' time not specified or wrong format");
                return false;
            }

            cJSON *lightLvlJson = cJSON_GetObjectItemCaseSensitive(timeSlotJson, "lightLvl");
            if (lightLvlJson == nullptr)
            {
                LOG_INFO("Could not parse Json, light level not specified or wrong format");
                return false;
            }

            cJSON *daysowJson = cJSON_GetObjectItemCaseSensitive(timeSlotJson, "daysow");
            if (daysowJson == nullptr)
            {
                LOG_INFO("Could not parse Json, days of week not specified or wrong format");
                return false;
            }

            cJSON *dayOfWeekJson = nullptr;

            TSingleTimer singleTimer = {};

            cJSON_ArrayForEach(dayOfWeekJson, daysowJson)
            {
                if (dayOfWeekJson == nullptr)
                {
                    LOG_INFO("Unexpected nullptr found while iterating through daysOfWeek array");
                    return false;
                }
                // singleTimer.days |= (1 << dayOfWeekJson->valueint);
            }

            singleTimer.lightLevel = lightLvlJson->valueint;
            singleTimer.startMinuteOfDay = extractTimeInMinutesFromString(std::string(fromJson->valuestring));
            singleTimer.endMinuteOfDay = extractTimeInMinutesFromString(std::string(toJson->valuestring));
            pOutput->timersList.timers[idJson->valueint] = singleTimer;
        }

        return true;
    }

    static bool processTimeSlotsListResponse(cJSON *pDataJson, TTimeSlotsListResponse *pOutput)
    {
        LOG_INFO("Processing TimeSlotsList response"); // TODO add processing of TimeSlotsList response here

        return true;
    }
    /***** FRAME INTERPRETATION *****/

    /**
     * @brief getDataJsonAndInitFrame - function extracting the dataJson - payload of the message with the message-type-specific data
     * and initializing the generic fields of the frame - msgCode and msgCounter
     * NOTE: after executing this function, it is it's user responsibility to free the memory allocated for dataJson object after it
     * will no longer be used - remember to call cJSON_Delete(dataJson)
     * @param inputMessage - std::string with the message to be parsed
     * @param pFrame - pointer to TFrame structure to store extracted data
     * @param ppDataJson - pointer to cJSON* object to store extracted dataJson object
     * @return boolean value representing success of the operation
     */

    bool getDataJsonAndInitFrame(const std::string &inputMessage, TFrame *pFrame, cJSON **ppDataJson)
    {
        cJSON *pRpcJson = preprocessInputMessage(inputMessage);

        if (pRpcJson == nullptr)
        {
            LOG_INFO("Could not parse JSON");
            return false;
        }

        cJSON *pFrameJson = cJSON_GetObjectItemCaseSensitive(pRpcJson, "params");

        if (pFrameJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no params or wrong format");
            cJSON_Delete(pRpcJson);
            return false;
        }
        cJSON *pMsgCodeJson = cJSON_GetObjectItemCaseSensitive(pFrameJson, "msgCode");

        if (pMsgCodeJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no msgCode or wrong format");
            cJSON_Delete(pRpcJson);
            return false;
        }
        cJSON *pMsgCounterJson = cJSON_GetObjectItemCaseSensitive(pFrameJson, "msgCounter");

        if (pMsgCounterJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no msgCounter or wrong format");
            cJSON_Delete(pRpcJson);
            return false;
        }
        *ppDataJson = cJSON_DetachItemFromObjectCaseSensitive(pFrameJson, "data");

        if (*ppDataJson == nullptr)
        {
            LOG_INFO("Could not parse JSON, no dataJson or wrong format");
            cJSON_Delete(pRpcJson);
            return false;
        }

        pFrame->msgCode = static_cast<EMsgCode>(pMsgCodeJson->valueint);
        pFrame->msgCounter = pMsgCounterJson->valueint;

        cJSON_Delete(pRpcJson);
        return true;
    }

    /**
     * @brief extractMsgMethod - function recognizing message method (e.g. RPC Command, or message from the widget) based on the
     * provided std::string with a message in JSON format
     * @param inputMessage - std::string containing message to identify
     * @return EMsgMethod with the method recognized by the function
     */

    EMsgMethod extractMsgMethod(const std::string &inputMessage)
    {
        cJSON *pMessageJson = preprocessInputMessage(inputMessage);

        if (pMessageJson == nullptr)
        {
            LOG_INFO("Could not parse JSON");
            return EMsgMethod::MSG_METHOD_FAILED;
        }

        cJSON *pMethodJson = cJSON_GetObjectItemCaseSensitive(pMessageJson, "method");

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

    EMsgMethod extractMethodAndFillFrame(std::string newMessage, TFrame *pFrame)
    {
        EMsgMethod msgMethod = extractMsgMethod(newMessage);

        switch (msgMethod)
        {
        case EMsgMethod::MSG_METHOD_RPC_COMMAND:
        {
            bool success = parseJsonRpcCommand(newMessage, pFrame);
            if (!success)
            {
                LOG_INFO("It was not possible to parse the incoming Rpc Command");
                return EMsgMethod::MSG_METHOD_FAILED;
            }

            break;
        }

        case EMsgMethod::MSG_METHOD_SET_LIGHT_INTENSITY:
        {
            bool success = parseWidgetSetLightIntensityLevelCommand(newMessage, &(pFrame->frameData.setLightLevelStruct));
            if (!success)
            {
                LOG_INFO("Could not parse message or wrong format");
                return EMsgMethod::MSG_METHOD_FAILED;
            }
            break;
        }

        default:
        {
            break;
        }
        }

        return msgMethod;
    }

    /**
     * @brief preprocessInputMessage - function parsing the message in json format into cJSON* object and checking for errors.
     * NOTE: after executing this function, it is it's user responsibility to free the memory allocated for returned cJSON object after it
     * will no longer be used - remember to call cJSON_Delete(returnedCjsonObject)
     * @param inputMessage - std::string with message in the JSON format
     * @return cJSON* object with the parsed message
     */

    cJSON *preprocessInputMessage(const std::string &inputMessage)
    {
        cJSON *pInputMessageJson = cJSON_Parse(inputMessage.c_str());

        if (pInputMessageJson == nullptr)
        {
            const char *pJsonError = cJSON_GetErrorPtr();
            if (pJsonError)
            {
                LOG_ERROR("Error parsing JSON occured before %p", pJsonError);
                return nullptr;
            }
        }

        return pInputMessageJson;
    }

    /**
     * @brief parseJsonRpcCommand - function filling the TFrame structure based on the inputMessage, assuming, it contains
     * RPC command
     * @param inputMessage - std::string with input message to analyze
     * @param pFrame - structure for storing the extracted data
     * @return boolean value representing the success of the operation
     */
    bool parseJsonRpcCommand(const std::string &inputMessage, TFrame *pFrame)
    {
        cJSON *pDataJson = nullptr;
        bool result = getDataJsonAndInitFrame(inputMessage, pFrame, &pDataJson);
        if (!result)
        {
            LOG_INFO("Error has occured while extracting frame");
            if (pDataJson != nullptr)
            {
                cJSON_Delete(pDataJson);
            }
            return false;
        }

        switch (pFrame->msgCode)
        {
        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL:
        {
            bool result = processSetLightIntensityLevel(pDataJson, &(pFrame->frameData.setLightLevelStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_STATUS_REPORT:
        {
            bool result = processStatusReport(pDataJson, &(pFrame->frameData.deviceStatusStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_HEARTBEAT:
        {
            bool result = processHeartbeat(pDataJson, &(pFrame->frameData.heartbeatStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_GET_LIGHT_INTENSITY_LEVEL:
        {
            bool result = processGetLightIntensityLevel(pDataJson, &(pFrame->frameData.setLightLevelStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE:
        case EMsgCode::MSG_HEARTBEAT_RESPONSE:
        case EMsgCode::MSG_STATUS_REPORT_RESPONSE:
        {
            bool result = processResponse(pDataJson, &(pFrame->frameData.responseStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_OTA_UPDATE_LINK:
        {
            LOG_INFO("About to process OTA_UPDATE_LINK"); // TODO complete processing OTA_UPDATE_LINK message
            bool result = processOtaUpdateLink(pDataJson, &(pFrame->frameData.otaUpdateLinkStruct));
            cJSON_Delete(pDataJson);
            if (!result)
            {
                LOG_INFO("Error while processing %s", getMsgCodeString(pFrame->msgCode).c_str());
                return false;
            }
            break;
        }

        case EMsgCode::MSG_TIME_SLOTS_LIST:
        {
            LOG_INFO("About to process TIME_SLOTS_LIST");
            bool result = processTimeSlotsList(pDataJson, &(pFrame->frameData.timeSlotsList));
            cJSON_Delete(pDataJson);
            break;
        }

        case EMsgCode::MSG_TIME_SLOTS_LIST_RESPONSE:
        {
            LOG_INFO("About to process TIME_SLOTS_LIST_RESPONSE");
            cJSON_Delete(pDataJson);
            break;
        }
        default:
        {
            cJSON_Delete(pDataJson);
            LOG_INFO("Unknown command received: %d", pFrame->msgCode);
            return false;
        }
        }

        return true;
    }

    /***** MESSAGE PREPARATION *****/

    std::string prepareHeartbeatMessage(uint32_t msgCounter)
    {
        json_parser::THeartbeat heartbeatStruct = {};
        heartbeatStruct.heartbeat = true;

        cJSON *pHeartbeatDataJson = heartbeatToJson(heartbeatStruct);

        if (pHeartbeatDataJson == nullptr)
        {
            LOG_INFO("Error while preparing heartbatDataJson in prepareHeartbeatMessage()");
            return std::string("");
        }

        cJSON *pHeartbeatCommandJson = dataJsonToRpcCommandJson(pHeartbeatDataJson, EMsgCode::MSG_HEARTBEAT, msgCounter);

        if (pHeartbeatCommandJson == nullptr)
        {
            LOG_INFO("Error while preparing heartbeatCommandJson in prepareHeartbeatMessage()");
            return std::string("");
        }

        char *pHeartbeatJsonCString = cJSON_Print(pHeartbeatCommandJson);
        if (pHeartbeatJsonCString == nullptr)
        {
            LOG_INFO("Error while printing heartbeatCommandJson");
            cJSON_Delete(pHeartbeatCommandJson);
            return std::string("");
        }

        std::string heartbeatJsonString = std::string(pHeartbeatJsonCString);
        free(pHeartbeatJsonCString); // NOLINT memory allocated by cJSON_Print needs to be freed manually
        cJSON_Delete(pHeartbeatCommandJson);

        return heartbeatJsonString;
    }

    std::string prepareDeviceStatusMessage(const json_parser::TDeviceStatus &deviceStatus, uint32_t msgCounter)
    {
        cJSON *pDeviceStatusDataJson = deviceStatusToJson(deviceStatus);

        if (pDeviceStatusDataJson == nullptr)
        {
            LOG_INFO("Error while preparing deviceStatusDataJson in prepareDeviceStatusMessage()");
            return "";
        }

        cJSON *pDeviceStatusCommandDataJson = dataJsonToRpcCommandJson(pDeviceStatusDataJson, EMsgCode::MSG_STATUS_REPORT, msgCounter);

        if (pDeviceStatusCommandDataJson == nullptr)
        {
            LOG_INFO("Error while preparing deviceStatusCommandJson in prepareDeviceStatusMessage()");
            return "";
        }

        char *pDeviceStatusJsonCString = cJSON_Print(pDeviceStatusCommandDataJson);
        if (pDeviceStatusJsonCString == nullptr)
        {
            LOG_INFO("Error while printing deviceStatusCommandJson");
            cJSON_Delete(pDeviceStatusCommandDataJson);
            return std::string("");
        }
        std::string deviceStatusMessage = std::string(pDeviceStatusJsonCString);
        free(pDeviceStatusJsonCString); // NOLINT memory allocated by cJSON_Print needs to be freed manually
        cJSON_Delete(pDeviceStatusCommandDataJson);

        return deviceStatusMessage;
    }

    /***** STRUCTURE TO JSON *****/

    cJSON *deviceStatusToJson(const TDeviceStatus &deviceStatus)
    {
        cJSON *pDeviceStatusJson = cJSON_CreateObject();
        cJSON *pWifiConnectionStateJson = cJSON_CreateBool(deviceStatus.isWiFiConnected);
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "wifiConnectionState", pWifiConnectionStateJson)))
        {
            LOG_INFO("Cannot add wifiConnectionState JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        cJSON *pBleConnectionStateJson = cJSON_CreateBool(deviceStatus.isBleConnected);
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "bleConnectionState", pBleConnectionStateJson)))
        {
            LOG_INFO("Cannot add bleConnectionState JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        cJSON *pCurrentTimeFromStartupMsJson = cJSON_CreateNumber(deviceStatus.currentTimeFromStartupMs);
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "currentTimeFromStartupMs", pCurrentTimeFromStartupMsJson)))
        {
            LOG_INFO("Cannot add currentTimeMs JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        cJSON *pFirmwareVersionJson = cJSON_CreateString(deviceStatus.getFirmwareVersion().c_str());
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "firmwareVersion", pFirmwareVersionJson)))
        {
            LOG_INFO("Cannot add firmwareVersion JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        cJSON *pCurrentLocalTimeJson = cJSON_CreateString(deviceStatus.getCurrentLocalTime().c_str());
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "currentLocalTime", pCurrentLocalTimeJson)))
        {
            LOG_INFO("Cannot add currentLocalTime JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        cJSON *pTotalSumOfLitersJson = cJSON_CreateNumber(deviceStatus.totalSumOfLiters);
        if (!(cJSON_AddItemToObject(pDeviceStatusJson, "totalSumOfLiters", pTotalSumOfLitersJson)))
        {
            LOG_INFO("Cannot add totalSumOfLiters JSON to deviceStatusJson");
            cJSON_Delete(pDeviceStatusJson);
            return nullptr;
        }

        return pDeviceStatusJson;
    }

    cJSON *heartbeatToJson(const THeartbeat &heartbeatStruct)
    {
        cJSON *pDataJson = cJSON_CreateObject();

        if (pDataJson == nullptr)
        {
            LOG_INFO("Error while preparing heartbeatJson - heartbeatToJson");
            return nullptr;
        }

        cJSON *pHeartbeatJson = cJSON_CreateBool(heartbeatStruct.heartbeat);

        if (!(cJSON_AddItemToObject(pDataJson, "heartbeat", pHeartbeatJson)))
        {
            LOG_INFO("Cannot add heartbeat JSON to dataJson");
            cJSON_Delete(pDataJson);
            return nullptr;
        }

        return pDataJson;
    }

    cJSON *widgetGetLightLevelToJson(const TSetLightLevel &lightLevelStruct)
    {
        cJSON *pLightIntensityJson = cJSON_CreateObject();

        if (pLightIntensityJson == nullptr)
        {
            LOG_INFO("Error while creating lightIntensityJson - widgetGetLightLevelToJson()");
            return nullptr;
        }

        cJSON *pParamsJson = cJSON_CreateNumber(lightLevelStruct.lightIntensityLevel);
        cJSON *pMethodJson = cJSON_CreateString("getLightIntensity");

        if (!(cJSON_AddItemToObject(pLightIntensityJson, "params", pParamsJson)))
        {
            LOG_INFO("Cannot add paramsJson to lightIntensityJson");
            cJSON_Delete(pLightIntensityJson);
            return nullptr;
        }
        if (!(cJSON_AddItemToObject(pLightIntensityJson, "method", pMethodJson)))
        {
            LOG_INFO("Cannot add methodJson to lightIntesnityJson");
            cJSON_Delete(pLightIntensityJson);
            return nullptr;
        }

        return pLightIntensityJson;
    }

    cJSON *widgetSetLightLevelToJson(const TSetLightLevel &lightLevelStruct)
    {
        cJSON *pLightIntensityJson = cJSON_CreateObject();

        if (pLightIntensityJson == nullptr)
        {
            LOG_INFO("Error while creating lightIntensity Json - widgetSetLightLevelToJson()");
            return nullptr;
        }

        cJSON *pParamsJson = cJSON_CreateNumber(lightLevelStruct.lightIntensityLevel);
        cJSON *pMethodJson = cJSON_CreateString("setLightIntensity");

        if (!(cJSON_AddItemToObject(pLightIntensityJson, "params", pParamsJson)))
        {
            LOG_INFO("Cannot add paramsJson to lightIntensityJson");
            cJSON_Delete(pLightIntensityJson);
            return nullptr;
        }

        if (!(cJSON_AddItemToObject(pLightIntensityJson, "method", pMethodJson)))
        {
            LOG_INFO("Cannot add methodJson to lightIntensityJson");
            cJSON_Delete(pLightIntensityJson);
            return nullptr;
        }

        return pLightIntensityJson;
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
    cJSON *dataJsonToParamsJson(cJSON *pDataJson, EMsgCode msgCode, uint32_t msgCounter)
    {
        cJSON *pParamsJson = cJSON_CreateObject();

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

        cJSON *pMsgCodeJson = cJSON_CreateNumber(static_cast<int>(msgCode));
        cJSON *pMsgCounterJson = cJSON_CreateNumber(msgCounter);

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

    /**
     * @brief dataJsonToRpcCommandJson - function creating RpcCommandJson (the most generic JSON in that protocol) from dataJson.
     * NOTE: after calling the function, it is the user's responsibility to free the memory allocated for the returned
     * cJSON* object. Call cJSON_Delete(returnedValue) after returned cJSON* will no longer be used
     * @param pDataJson - cJSON* object with dataJson
     * @param msgCode - msgCode to add to paramsJson
     * @param msgCounter - msgCounter to add to paramsJson
     * @return
     */

    cJSON *dataJsonToRpcCommandJson(cJSON *pDataJson, EMsgCode msgCode, uint32_t msgCounter)
    {
        cJSON *pParamsJson = dataJsonToParamsJson(pDataJson, msgCode, msgCounter);

        if (pParamsJson == nullptr)
        {
            LOG_INFO("Error while creating paramsJson - dataJsonToRpcCommandJson()");
            return nullptr;
        }

        cJSON *pRpcCommandJson = cJSON_CreateObject();

        if (!(cJSON_AddItemToObject(pRpcCommandJson, "params", pParamsJson)))
        {
            LOG_INFO("Cannot add paramsJson to rpcCommandJson");
            cJSON_Delete(pParamsJson);
            cJSON_Delete(pRpcCommandJson);
            return nullptr;
        }

        return pRpcCommandJson;
    }

    /***** STRING GET FUNCTIONS FOR FRAME PRINTING *****/

    std::string getLightIntensityString(const TSetLightLevel &setLightLevelData)
    {
        std::string output = "lightIntensityLevel: " + std::to_string(setLightLevelData.lightIntensityLevel) + "\n******************\n";
        return output;
    }

    std::string getStatusReportString(const TDeviceStatus &deviceStatus)
    {
        std::string statusReportString = "wifiConnectionState: " + getBooleanString(deviceStatus.isWiFiConnected) + "\n" + "bleConnectionState: " + getBooleanString(deviceStatus.isBleConnected) + "\n" + "lightIntensityLevel: " + std::to_string(deviceStatus.lightIntensityLevel) + "\n" + "currentTimeFromStartupMs: " + std::to_string(deviceStatus.currentTimeFromStartupMs) + "\n" + "currentLocalTime: " + deviceStatus.currentLocalTime + "\n" + "firmwareVersion: " + deviceStatus.firmwareVersion + "\n";

        return statusReportString;
    }

    std::string getHeartbeatString(const THeartbeat &heartbeatData)
    {
        std::string heartbeatString = heartbeatData.heartbeat ? "true" : "false";
        std::string output = "heartbeat: " + heartbeatString + "\n******************\n";
        return output;
    }

    std::string getResponseString(const TResponse &responseData)
    {
        std::string responseString = responseData.ACK ? "true" : "false";
        std::string output = "ACK: " + responseString + "\n******************\n";
        return output;
    }

    std::string getOtaUpdateLinkString(const TOtaUpdateLink &otaUpdateLink)
    {
        std::string output = "firmwareLink: " + std::string(otaUpdateLink.firmwareLink) + std::string("\n");

        return output;
    }

    // std::string getTimeSlotsListString(const TTimeSlotsList& timeSlotsList)
    // {
    //     constexpr int8_t MAX_NUM_OF_TIMERS = 42;
    //     std::string result("");

    //     for (int i=0; i < MAX_NUM_OF_TIMERS; i++)
    //     {
    //         if (timeSlotsList.timersList.timers[i].lightLevel != 200)
    //         {

    //             result += std::string("Id: %d: \n"
    //                                   "startMinuteOfDay: %d\n"
    //                                   "endMinuteOfDay: %d\n"
    //                                   "days: 0x%x\n"
    //                                   "lightLevel: %d",
    //                                   i,
    //                                   timeSlotsList.timersList.timers[i].startMinuteOfDay,
    //                                   timeSlotsList.timersList.timers[i].endMinuteOfDay,
    //                                   timeSlotsList.timersList.timers[i].days,
    //                                   timeSlotsList.timersList.timers[i].lightLevel);
    //         }
    //     }
    //     LOG_INFO("Printing Time Slots List");
    //     // TODO - decide if json_parser should get access to NVS to print the message
    //     return result;
    // }

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

    /***** FRAME PRINTING *****/

    void printFrame(const TFrame &frame)
    {
        std::string dataString;

        switch (frame.msgCode)
        {
        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL:
            dataString = getLightIntensityString(frame.frameData.setLightLevelStruct);
            break;

        case EMsgCode::MSG_STATUS_REPORT:
            dataString = getStatusReportString(frame.frameData.deviceStatusStruct);
            break;

        case EMsgCode::MSG_HEARTBEAT:
            dataString = getHeartbeatString(frame.frameData.heartbeatStruct);
            break;

        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE:
        case EMsgCode::MSG_STATUS_REPORT_RESPONSE:
        case EMsgCode::MSG_HEARTBEAT_RESPONSE:

            dataString = getResponseString(frame.frameData.responseStruct);
            break;

        case EMsgCode::MSG_OTA_UPDATE_LINK:
            dataString = getOtaUpdateLinkString(frame.frameData.otaUpdateLinkStruct);
            break;
        case EMsgCode::MSG_TIME_SLOTS_LIST:
            // dataString = getTimeSlotsListString(frame.frameData.timeSlotsList);
            break;
        default:
            LOG_INFO("Unknown message code, could not print the frame");
        }

        LOG_INFO("\n\n"
                 "******************\n"
                 "msgCounter: %d\n"
                 "msgCode: %d\n"
                 "%s",
                 frame.msgCounter,
                 frame.msgCode,
                 dataString.c_str());
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
        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL:
            return std::string("MSG_SET_LIGHT_INTENSITY_LEVEL");
        case EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE:
            return std::string("MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE");
        case EMsgCode::MSG_STATUS_REPORT:
            return std::string("MSG_STATUS_REPORT");
        case EMsgCode::MSG_STATUS_REPORT_RESPONSE:
            return std::string("MSG_STATUS_REPORT_RESPONSE");
        case EMsgCode::MSG_GET_LIGHT_INTENSITY_LEVEL:
            return std::string("MSG_GET_LIGHT_INTENSITY_LEVEL");
        case EMsgCode::MSG_GET_LIGHT_INTENSITY_LEVEL_RESPONSE:
            return std::string("MSG_GET_LIGHT_INTENSITY_LEVEL_RESPONSE");
        case EMsgCode::MSG_OTA_UPDATE_LINK:
            return std::string("MSG_OTA_UPDATE_LINK");
        case EMsgCode::MSG_OTA_UPDATE_LINK_RESPONSE:
            return std::string("MSG_OTA_UPDATE_LINK_RESPONSE");
        case EMsgCode::MSG_TIME_SLOTS_LIST:
            return std::string("MSG_TIME_SLOTS_LIST");
        case EMsgCode::MSG_TIME_SLOTS_LIST_RESPONSE:
            return std::string("MSG_TIME_SLOTS_LIST_RESPONSE");
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

    int32_t extractRequestIdFromTopic(const std::string &topic)
    {
        int32_t requestId1 = 0;
        int32_t requestId2 = 0;

        int resultTopic1 = sscanf(topic.c_str(), "v1/devices/me/rpc/request/%10d", &requestId1); // NOLINT - we don't want exceptions handling
        if (resultTopic1 > 0)
        {
            return requestId1;
        }

        int resultTopic2 = sscanf(topic.c_str(), "v1/devices/me/rpc/response/%10d", &requestId2); // NOLINT - we don't want exceptions handling
        if (resultTopic2 > 0)
        {
            return requestId2;
        }

        return -1;
    }

    int32_t extractTimeInMinutesFromString(const std::string timeString)
    {
        int32_t hours = 0;
        int32_t minutes = 0;
        int32_t parseResult = sscanf(timeString.c_str(), "%2d:%2d", &hours, &minutes);

        if (parseResult > 0)
        {
            return hours * 60 + minutes;
        }
        else
        {
            LOG_INFO("Parsing problem occured in extractTimeInMinutesFromString()");
            return -1;
        }

        return 0;
    }

} // namespace json_parser
