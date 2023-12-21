#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "custom_types.h"
#include "protocol_types.h"
#include <string>

#include "cJSON.h"

namespace
{
    constexpr uint8_t FIRMWARE_VERSION_LENGTH = 32;
    constexpr uint8_t MAX_TIME_STRING_LENGTH = 30;
    constexpr uint8_t MAX_OTA_TOPIC_SUFFIX_LENGTH = 80;
    constexpr uint8_t ALARM_RESPONSE_LENGTH = 32;
}


namespace json_parser
{

    enum class EMsgCode : uint8_t
    {
        MSG_SET_LIGHT_INTENSITY_LEVEL = 1,
        MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE = 2,
        MSG_STATUS_REPORT = 3,
        MSG_STATUS_REPORT_RESPONSE = 4,
        MSG_HEARTBEAT = 5,
        MSG_HEARTBEAT_RESPONSE = 6,
        MSG_GET_LIGHT_INTENSITY_LEVEL = 7,
        MSG_GET_LIGHT_INTENSITY_LEVEL_RESPONSE = 8,
        MSG_OTA_UPDATE_LINK = 9,
        MSG_OTA_UPDATE_LINK_RESPONSE = 10,
        MSG_TIME_SLOTS_LIST = 11,
        MSG_TIME_SLOTS_LIST_RESPONSE = 12
    };

    enum class EMsgMethod : uint8_t
    {
        MSG_METHOD_RPC_COMMAND = 1,
        MSG_METHOD_GET_LIGHT_INTENSITY = 2,
        MSG_METHOD_SET_LIGHT_INTENSITY = 3,
        MSG_METHOD_UNKNOWN = 4,
        MSG_METHOD_FAILED = 5
    };

    struct TSetLightLevel
    {
        uint8_t lightIntensityLevel;
    };

    struct THeartbeat
    {
        bool heartbeat;
    };

    struct TResponse
    {
        bool ACK;
    };

    struct TDeviceStatus
    {
        bool isWiFiConnected;
        bool isBleConnected;
        bool isBelowPressureAlarmThreshold;
        uint8_t lightIntensityLevel;
        uint32_t currentTimeFromStartupMs;
        char firmwareVersion[FIRMWARE_VERSION_LENGTH + 1];
        char currentLocalTime[MAX_TIME_STRING_LENGTH + 1];
        float pressureSensorValue;
        std::string getFirmwareVersion() const;
        std::string getCurrentLocalTime() const;
    };

    struct TRepeatDaysOfWeek
    {
        uint8_t monday : 1;
        uint8_t tuesday : 1;
        uint8_t wednesday : 1;
        uint8_t thursday : 1;
        uint8_t friday : 1;
        uint8_t saturday : 1;
        uint8_t sunday : 1;
    };

    struct TSingleTimer
    {
        uint16_t startMinuteOfDay;
        uint16_t endMinuteOfDay;
        TRepeatDaysOfWeek days;
        uint8_t lightLevel;
    };

    struct TAllTimers
    {
        TSingleTimer timers[42];
    };

    struct TTimeSlotsList
    {
        bool timeSlotListUpdated;
        TAllTimers timersList;
    };

    struct TTimeSlotsListResponse
    {
        bool ACK;
        uint32_t id;
    };

    union TFrameData
    {
        TSetLightLevel setLightLevelStruct;
        TDeviceStatus deviceStatusStruct;
        THeartbeat heartbeatStruct;
        TResponse responseStruct;
        prot::ota_perform::TOtaUpdateLink otaUpdateLinkStruct;
        TTimeSlotsList timeSlotsList;
    };

    struct TMessage
    {
        std::string message;
        int32_t requestId;
    };

    struct TFrame
    {
        EMsgCode msgCode;
        uint32_t msgCounter;
        TFrameData frameData;
    };






struct TDeviceProvisioningInfo
{
    std::string operationId;
    std::string status;
    std::string deviceId;
    std::string assignedHub;
};






    /**
     * @brief prepareHeartbeatMessage - function preparing std::string with heartbeat message in a JSON format based on provided
     * msgCounter value. Function assumes the ACK value of the heartbeat structure shall be set to true, therefore it does not require
     * THeartbeat structure to be passed
     * @param msgCounter - number of the message to be included in the message
     * @return std::string with the message in JSON format
     */

    std::string prepareHeartbeatMessage(uint32_t msgCounter);

    /**
     * @brief prepareDeviceStatusMessage - function preparing a DeviceStatusMessage in a JSON format
     * @param deviceStatus  - input structure containing information about device status
     * @param msgCounter - message number to be added
     * @return std::string with a message in a JSON format
     */
    std::string prepareDeviceStatusMessage(const TDeviceStatus &deviceStatus, uint32_t msgCounter);



    /**
     * @brief prepareDeviceCreateProvisioningMessage
     */
    std::string
    prepareDeviceCreateProvisioningMessage(char (&deviceId)[prot::cloud_set_credentials::CLOUD_DEVICE_ID_LENGTH]);

    /**
     * @brief parseJsonDeviceProvisioning
     */
    bool parseJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* pDeviceProvisioningInfo);



    /**
     * @brief extractMethodAndFillFrame - function that gets the std::string with a message, parses it, interprets it and then fills
     * the passed TFrame structure together with returning EMsgMethod according to the message content
     * @param newMessage - std::string with the message content
     * @param pFrame - TFrame structure to be filled according to the message content
     * @return EMsgMethod according to the message content
     */
    EMsgMethod extractMethodAndFillFrame(std::string newMessage, TFrame *pFrame);

    /**
     * @brief extractRequestIdFromTopic - function extracting rpc command request id, which is given as an integer at the end of the topic
     * @param topic - std::string cotaining topic to extract requestId from
     * @return int32_t value with requestId
     */
    int32_t extractRequestIdFromTopic(const std::string &topic);

    /**
     * @brief printFrame - function allowing to print content of the TFrame structure in an easily-readable way
     * @param frame - TFrame structure to be printed
     */
    void printFrame(const TFrame &frame);

    /**
     * @brief getMsgMethodString - function returning std::string with a text representing the passed EMsgMethod parameter
     * @param msgMethod - EMsgMethod parameter containing a method to be printed, e.g. EMsgMethod::MSG_METHOD_RPC_COMMAND
     * @return std::string with the input parameter in the text form, e.g. "MSG_METHOD_RPC_COMMAND"
     */

    std::string getMsgMethodString(EMsgMethod msgMethod);


#if TESTING

// functions that are needed for unit tests
bool        processStatusReport(cJSON* dataJson, TDeviceStatus* output);
bool        processGetLightIntensityLevel(cJSON* dataJson, TSetLightLevel* output);
bool        processResponse(cJSON* dataJson, TResponse* output);
static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);
cJSON*      preprocessInputMessage(const std::string& inputMessage);
EMsgMethod  extractMsgMethod(const std::string& inputMessage);
bool        getDataJsonAndInitFrame(const std::string& inputMessage, TFrame* frame, cJSON** dataJson);
bool getDataJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* frame, cJSON** dataJson);
bool parseJsonRpcCommand(const std::string& inputMessage, TFrame* frame);
std::string prepareDeviceStatusMessage(const json_parser::TDeviceStatus& deviceStatus, uint32_t msgCounter);
cJSON*      deviceStatusToJson(const TDeviceStatus& deviceStatus);
cJSON*      heartbeatToJson(const THeartbeat& heartbeatStruct);
cJSON*      widgetGetLightLevelToJson(const TSetLightLevel& lightLevelStruct);
cJSON*      widgetSetLightLevelToJson(const TSetLightLevel& lightLevelStruct);
cJSON*      dataJsonToParamsJson(cJSON* dataJson, EMsgCode msgCode, uint32_t msgCounter);
cJSON*      dataJsonToRpcCommandJson(cJSON* dataJson, EMsgCode msgCode, uint32_t msgCounter);

int32_t extractTimeInMinutesFromString(const std::string timeString);

#endif

} // namespace json_parser

#endif // JSON_PARSER_H
