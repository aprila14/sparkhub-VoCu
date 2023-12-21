#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include "defines.h"

#include "protocol_control.h"
#include "protocol_types.h"

#include "esp_wifi_types.h"

#include <string>

constexpr uint8_t  MAX_WIFI_SSID_LENGTH                = prot::wifi_scan_ap::MAX_WIFI_SSID_LENGTH;
constexpr uint8_t  MAX_WIFI_PASSWORD_LENGTH            = prot::wifi_connect_to_ap::MAX_WIFI_PASSWORD_LENGTH;
constexpr size_t   MAX_NUMBER_OF_SCANNED_ACCESS_POINTS = CONFIG_WIFI_PROV_SCAN_MAX_ENTRIES;
constexpr uint8_t  MAX_FILE_KEY_LENGTH                 = 20U;
constexpr uint8_t  MAX_WORKFLOW_ID_LENGTH              = 40U;
constexpr uint32_t MAX_OTA_URL_LENGTH                  = 300U;
constexpr uint32_t MAX_PROVIDER_NAME_LENGTH            = 30U;
constexpr uint32_t MAX_OTA_UPDATE_NAME_LENGTH          = 30U;
constexpr uint32_t MAX_OTA_VERSION_STRING_LENGTH       = 30U;


static_assert(
    MAX_NUMBER_OF_SCANNED_ACCESS_POINTS == prot::wifi_scan_ap::MAX_NUMBER_OF_NETWORKS,
    "Inconsistent protocol definition!");

struct TFirmwareInfo
{
    std::string version;
    std::string firmwareUrl;

    bool operator==(const TFirmwareInfo& rhs) const;
    bool setFirmwareUrl(const std::string& newFirmwareUrl);
    bool isSetFirmwareUrl() const;
};

enum class EDeviceUpdateAction
{
    ACTION_DOWNLOAD = 3,
    ACTION_CANCEL   = 255
};

struct TUpdateManifest
{
    char fileKey[MAX_FILE_KEY_LENGTH];
    char firmwareVersion[MAX_OTA_VERSION_STRING_LENGTH];
};

struct TWorkflowData
{
    EDeviceUpdateAction deviceUpdateAction;
    char                workflowId[MAX_WORKFLOW_ID_LENGTH];

    bool operator==(const TWorkflowData& rhs) const;
    bool setWorkflowId(const char* newFirmwareUrl);
    bool isSetWorkflowId() const;
};

struct TDeviceUpdate
{
    char            fileUrl[MAX_OTA_URL_LENGTH];
    TUpdateManifest updateManifest;
    TWorkflowData   workflowData;
};

struct TUpdateId
{
    char providerName[MAX_PROVIDER_NAME_LENGTH];
    char updateName[MAX_OTA_UPDATE_NAME_LENGTH];
    char version[MAX_OTA_VERSION_STRING_LENGTH];
};

using TWiFiCredentials             = prot::wifi_connect_to_ap::TWiFiCredentials;
using TCloudCredentials            = prot::cloud_set_credentials::TCloudCredentials;
using TCertificatePack             = prot::send_certificates::TCertificatePack;
using THttpsServerCertificate      = prot::ota_perform::THttpsServerCertificate;
using TConnectToAccessPointCommand = prot::wifi_connect_to_ap::TCmd;
using TConnectToAccessPointResult  = prot::wifi_connect_to_ap::TRes;
using TOtaUpdateLink               = prot::ota_perform::TOtaUpdateLink;

#endif // CUSTOM_TYPES_H
