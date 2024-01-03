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
constexpr uint8_t ALARM_RESPONSE_LENGTH       = 32;
} // namespace


namespace json_parser
{

enum class EMsgCode : uint8_t
{
    MSG_SET_LIGHT_INTENSITY_LEVEL          = 1,
    MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE = 2,
    MSG_STATUS_REPORT                      = 3,
    MSG_STATUS_REPORT_RESPONSE             = 4,
    MSG_HEARTBEAT                          = 5,
    MSG_HEARTBEAT_RESPONSE                 = 6,
    MSG_GET_LIGHT_INTENSITY_LEVEL          = 7,
    MSG_GET_LIGHT_INTENSITY_LEVEL_RESPONSE = 8,
    MSG_OTA_UPDATE_LINK                    = 9,
    MSG_OTA_UPDATE_LINK_RESPONSE           = 10,
    MSG_TIME_SLOTS_LIST                    = 11,
    MSG_TIME_SLOTS_LIST_RESPONSE           = 12
};

enum class EMsgMethod : uint8_t
{
    MSG_METHOD_RPC_COMMAND         = 1,
    MSG_METHOD_GET_LIGHT_INTENSITY = 2,
    MSG_METHOD_SET_LIGHT_INTENSITY = 3,
    MSG_METHOD_UNKNOWN             = 4,
    MSG_METHOD_FAILED              = 5
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
    bool        isWiFiConnected;
    bool        isBleConnected;
    bool        isBelowPressureAlarmThreshold;
    uint8_t     lightIntensityLevel;
    uint32_t    currentTimeFromStartupMs;
    char        firmwareVersion[FIRMWARE_VERSION_LENGTH + 1];
    char        currentLocalTime[MAX_TIME_STRING_LENGTH + 1];
    float       pressureSensorValue;
    float       flowMeterValue;
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
    uint16_t          startMinuteOfDay;
    uint16_t          endMinuteOfDay;
    TRepeatDaysOfWeek days;
    uint8_t           lightLevel;
};

struct TAllTimers
{
    TSingleTimer timers[42];
};

struct TTimeSlotsList
{
    bool       timeSlotListUpdated;
    TAllTimers timersList;
};

struct TTimeSlotsListResponse
{
    bool     ACK;
    uint32_t id;
};

union TFrameData
{
    TSetLightLevel                    setLightLevelStruct;
    TDeviceStatus                     deviceStatusStruct;
    THeartbeat                        heartbeatStruct;
    TResponse                         responseStruct;
    prot::ota_perform::TOtaUpdateLink otaUpdateLinkStruct;
    TTimeSlotsList                    timeSlotsList;
};

struct TMessage
{
    std::string message;
    int32_t     requestId;
};

struct TFrame
{
    EMsgCode   msgCode;
    uint32_t   msgCounter;
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
 * @brief Function for DeviceUpdate JSON parsing (from Azure IoT Hub OTA). We are assuming, that this JSON
 * has following format (example):
 *
 *
 {
    "deviceUpdate":
    {
        "__t": "c",
        "agent":
        {
            "deviceProperties":
            {
                "manufacturer": "sparkhub",
                "model": "levelsense",
                "contractModelId": "dtmi:azure:iot:deviceUpdateContractModel;2",
                "aduVer": "DU;agent/1.0.0"
            },
            "compatPropertyNames": "manufacturer,model",
            "state": 0,
            "installedUpdateId":
            {
                "provider": "Contoso",
                "name": "levelsense",
                "version": "0.4.0"
            }
        }
    }
}
 * @param pInputJson
 * @param pDeviceUpdate
 * @return true
 * @return false
 */
bool parseDeviceUpdate(cJSON* pInputJson, TDeviceUpdate* pDeviceUpdate);

cJSON* preprocessInputMessage(const std::string& inputMessage);

cJSON* initiateReportedJson();

std::string prepareReportedMessage(cJSON* pReportedJson);

std::string prepareDeviceUpdateReport(const TUpdateId& updateId, uint8_t state, const TWorkflowData& workflowData);

bool        parseFlowMeterCalibrationValue(cJSON* pInputJson, float* pFlowMeterCalibrationValue);
std::string prepareFlowMeterCalibrationReport(const float& flowMeterCalibrationValue);


#if TESTING

// functions that are needed for unit tests
bool        processResponse(cJSON* dataJson, TResponse* output);
static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);
cJSON*      preprocessInputMessage(const std::string& inputMessage);
EMsgMethod  extractMsgMethod(const std::string& inputMessage);
bool getDataJsonDeviceProvisioning(const std::string& inputMessage, TDeviceProvisioningInfo* frame, cJSON** dataJson);

std::string prepareDeviceStatusMessage(const json_parser::TDeviceStatus& deviceStatus, uint32_t msgCounter);
cJSON*      deviceStatusToJson(const TDeviceStatus& deviceStatus, uint32_t msgCounter);

static bool processOtaUpdateLink(cJSON* pDataJson, TOtaUpdateLink* pOutput);

#endif

} // namespace json_parser

#endif // JSON_PARSER_H
