#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include "defines.h"

#include "protocol_control.h"
#include "protocol_types.h"

#include "esp_wifi_types.h"

#include <string>

constexpr uint8_t MAX_WIFI_SSID_LENGTH                = prot::wifi_scan_ap::MAX_WIFI_SSID_LENGTH;
constexpr uint8_t MAX_WIFI_PASSWORD_LENGTH            = prot::wifi_connect_to_ap::MAX_WIFI_PASSWORD_LENGTH;
constexpr size_t  MAX_NUMBER_OF_SCANNED_ACCESS_POINTS = CONFIG_WIFI_PROV_SCAN_MAX_ENTRIES;

static_assert(
    MAX_NUMBER_OF_SCANNED_ACCESS_POINTS == prot::wifi_scan_ap::MAX_NUMBER_OF_NETWORKS,
    "Inconsistent protocol definition!");

using TWiFiCredentials             = prot::wifi_connect_to_ap::TWiFiCredentials;
using TCloudCredentials            = prot::cloud_set_credentials::TCloudCredentials;
using TCertificatePack             = prot::send_certificates::TCertificatePack;
using THttpsServerCertificate      = prot::ota_perform::THttpsServerCertificate;
using TConnectToAccessPointCommand = prot::wifi_connect_to_ap::TCmd;
using TConnectToAccessPointResult  = prot::wifi_connect_to_ap::TRes;
using TOtaUpdateLink               = prot::ota_perform::TOtaUpdateLink;

#endif // CUSTOM_TYPES_H
