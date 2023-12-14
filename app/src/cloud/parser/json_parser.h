#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "custom_types.h"
#include "protocol_types.h"
#include <string>

#include "cJSON.h"

namespace
{
constexpr uint8_t FIRMWARE_VERSION_LENGTH     = 32;
constexpr uint8_t MAX_TIME_STRING_LENGTH      = 30;
constexpr uint8_t MAX_OTA_TOPIC_SUFFIX_LENGTH = 80;
} // namespace
namespace json_parser
{

enum class EMsgCode : uint8_t
{
    MSG_STATUS_REPORT            = 1,
    MSG_STATUS_REPORT_RESPONSE   = 2,
    MSG_HEARTBEAT                = 3,
    MSG_HEARTBEAT_RESPONSE       = 4,
    MSG_OTA_UPDATE_LINK          = 5,
    MSG_OTA_UPDATE_LINK_RESPONSE = 6
};

enum class EMsgMethod : uint8_t
{
    MSG_METHOD_RPC_COMMAND         = 1,
    MSG_METHOD_GET_LIGHT_INTENSITY = 2,
    MSG_METHOD_SET_LIGHT_INTENSITY = 3,
    MSG_METHOD_UNKNOWN             = 4,
    MSG_METHOD_FAILED              = 5
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
    bool        isWiFiConnected;
    bool        isBleConnected;
    uint32_t    currentTimeFromStartupMs;
    char        firmwareVersion[FIRMWARE_VERSION_LENGTH + 1];
    char        currentLocalTime[MAX_TIME_STRING_LENGTH + 1];
    float       pressureSensorValue;
    std::string getFirmwareVersion() const;
    std::string getCurrentLocalTime() const;
    uint32_t    msgCounter;
};

union TFrameData
{
    TDeviceStatus                     deviceStatusStruct;
    THeartbeat                        heartbeatStruct;
    TResponse                         responseStruct;
    prot::ota_perform::TOtaUpdateLink otaUpdateLinkStruct;
};

struct TMessage
{
    std::string message;
    int32_t     requestId;
};

struct TDeviceProvisioningInfo
{
    std::string operationId;
    std::string status;
    std::string deviceId;
    std::string assignedHub;
};

/**
 * @brief prepareDeviceStatusMessage - function preparing a DeviceStatusMessage in a JSON format
 * @param deviceStatus  - input structure containing information about device status
 * @param msgCounter - message number to be added
 * @return std::string with a message in a JSON format
 */
std::string prepareDeviceStatusMessage(const TDeviceStatus& deviceStatus, uint32_t msgCounter);

/**
 * @brief prepareDeviceCreateProvisioningMessage
 */
std::string
prepareDeviceCreateProvisioningMessage(char (&deviceId)[prot::cloud_set_credentials::CLOUD_DEVICE_ID_LENGTH]);

/**
 * @brief Function determines if input JSON structure contains field with given keyword
 *
 * @param inputJson JSON structure to check
 * @param keyword key under which field shall be found
 * @retval true - field found
 * @retval false - field not found
 */
bool checkIfFieldExistsInGivenJson(cJSON* inputJson, const char* keyword);

/**
 * @brief parseJsonDeviceProvisioning
 */
bool parseJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* pDeviceProvisioningInfo);

/**
 * @brief Function for FirmwareInfo JSON parsing. We are assuming, that this JSON has following format:
 * {
 *     "firmware_info"
 *     {
 *          "firmware_version": 231209,
 *          "url": "http://127.0.0.1/urlToFirmware.bin"
 *     }
 * }
 * @param inputMessage JSON to be parsed
 * @param firmwareInfo in-out structure containing data extracted from JSON
 * @return true parsing successful
 * @return false error occurred during parsing
 */
bool parseFirmwareInfo(cJSON* pInputJson, TFirmwareInfo* pFirmwareInfo);

cJSON* preprocessInputMessage(const std::string& inputMessage);

cJSON* initiateReportedJson();

bool addFirmwareInfoToReportedJson(cJSON** ppReportedJson, const TFirmwareInfo& firmwareInfo);

std::string prepareReportedMessage(cJSON* pReportedJson);

std::string prepareFirmwareInfoReportedMessage(const TFirmwareInfo& firmwareInfo, const char* otaStatus);

#if TESTING

// functions that are needed for unit tests
bool        processStatusReport(cJSON* dataJson, TDeviceStatus* output);
bool        processResponse(cJSON* dataJson, TResponse* output);
static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);
cJSON*      preprocessInputMessage(const std::string& inputMessage);
EMsgMethod  extractMsgMethod(const std::string& inputMessage);
bool getDataJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* frame, cJSON** dataJson);

bool        processStatusReport(cJSON* dataJson, TDeviceStatus* output);
std::string prepareDeviceStatusMessage(const json_parser::TDeviceStatus& deviceStatus, uint32_t msgCounter);
cJSON*      deviceStatusToJson(const TDeviceStatus& deviceStatus, uint32_t msgCounter);

static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);

#endif

} // namespace json_parser

#endif // JSON_PARSER_H
