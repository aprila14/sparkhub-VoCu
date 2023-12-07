/**
 * @file protocol_types.h
 * @brief Defines the communication protocol types used also in different modules.
 * There is a lot of types. Most of them are copied from esp_idf.
 * As we will copy the data from ESP-IDF types to our (theoretically same) types - we need some
 * sanity checks, to make sure PC and ESP treat all types the same way.
 * NOTE: this file should be revisit while updating ESP-IDF version!
 *
 */

#ifndef PROTOCOL_TYPES_H
#define PROTOCOL_TYPES_H

#include "defines.h"

#include <string>

// enum copied from 'esp_wifi_types.h' (wifi_auth_mode_t)
enum class device_wifi_auth_mode_t : uint32_t
{
    DEVICE_WIFI_AUTH_OPEN = 0,
    /**< authenticate mode : open */ // auth method tested
    // WIFI_AUTH_WEP = 1,              /**< authenticate mode : WEP */
    DEVICE_WIFI_AUTH_WPA_PSK = 2,
    /**< authenticate mode : WPA_PSK */ // auth method not tested
    DEVICE_WIFI_AUTH_WPA2_PSK = 3,
    /**< authenticate mode : WPA2_PSK */ // auth method tested
    DEVICE_WIFI_AUTH_WPA_WPA2_PSK = 4,
    /**< authenticate mode : WPA_WPA2_PSK */ // auth method not tested
    // WIFI_AUTH_WPA2_ENTERPRISE = 5,  /**< authenticate mode : WPA2_ENTERPRISE */
    DEVICE_WIFI_AUTH_WPA3_PSK = 6,
    /**< authenticate mode : WPA3_PSK */ // auth method tested
    DEVICE_WIFI_AUTH_WPA2_WPA3_PSK = 7,
    /**< authenticate mode : WPA2_WPA3_PSK */ // auth method not tested
    // WIFI_AUTH_WAPI_PSK = 8 ,         /**< authenticate mode : WAPI_PSK */
    DEVICE_WIFI_AUTH_NOT_SET = 10, // use WPA2 or open then
};

enum class device_wifi_second_chan_t : uint32_t
{
    DEVICE_WIFI_SECOND_CHAN_NONE = 0, /**< the channel width is HT20 */
    DEVICE_WIFI_SECOND_CHAN_ABOVE, /**< the channel width is HT40 and the secondary channel is above the primary channel
                                    */
    DEVICE_WIFI_SECOND_CHAN_BELOW, /**< the channel width is HT40 and the secondary channel is below the primary channel
                                    */
};

enum class EConnectToSsidErrorReason : uint32_t
{
    DEVICE_WIFI_REASON_NO_ERROR      = 0,
    DEVICE_WIFI_REASON_UNSPECIFIED   = 1, // Internal failure
    DEVICE_WIFI_REASON_AUTH_EXPIRE   = 2, // Previous authenthication is no longer valid
    DEVICE_WIFI_REASON_AUTH_LEAVE    = 3, // Sending station is leaving/has left
    DEVICE_WIFI_REASON_ASSOC_EXPIRE  = 4, // Disassociated due to inactivity
    DEVICE_WIFI_REASON_ASSOC_TOOMANY = 5, // Access point not able to handle all associated STAs
    // DEVICE_WIFI_REASON_NOT_AUTHED               = 6,
    // DEVICE_WIFI_REASON_NOT_ASSOCED              = 7,
    DEVICE_WIFI_REASON_ASSOC_LEAVE = 8, // station disconnected
    // DEVICE_WIFI_REASON_ASSOC_NOT_AUTHED         = 9,
    // DEVICE_WIFI_REASON_DISASSOC_PWRCAP_BAD      = 10,
    // DEVICE_WIFI_REASON_DISASSOC_SUPCHAN_BAD     = 11,
    // DEVICE_WIFI_REASON_IE_INVALID               = 13,
    DEVICE_WIFI_REASON_MIC_FAILURE            = 14, // Message integrity code failure
    DEVICE_WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15, // wrong password or timeout occured
    // DEVICE_WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    // DEVICE_WIFI_REASON_IE_IN_4WAY_DIFFERS       = 17,
    // DEVICE_WIFI_REASON_GROUP_CIPHER_INVALID     = 18,
    // DEVICE_WIFI_REASON_PAIRWISE_CIPHER_INVALID  = 19,
    // DEVICE_WIFI_REASON_AKMP_INVALID             = 20,
    // DEVICE_WIFI_REASON_UNSUPP_RSN_IE_VERSION    = 21,
    // DEVICE_WIFI_REASON_INVALID_RSN_IE_CAP       = 22,
    // DEVICE_WIFI_REASON_802_1X_AUTH_FAILED       = 23,
    // DEVICE_WIFI_REASON_CIPHER_SUITE_REJECTED    = 24,

    // DEVICE_WIFI_REASON_INVALID_PMKID            = 53,

    DEVICE_WIFI_REASON_BEACON_TIMEOUT = 200, // lost connection to the Access Point (e.g. Access Point switched off)
    DEVICE_WIFI_REASON_NO_AP_FOUND    = 201, // wrong SSID, authenthication method or AP not found
    DEVICE_WIFI_REASON_AUTH_FAIL      = 202, // other authentication issue
    DEVICE_WIFI_REASON_ASSOC_FAIL     = 203, // otther association failure
    // DEVICE_WIFI_REASON_HANDSHAKE_TIMEOUT        = 204,
    DEVICE_WIFI_REASON_CONNECTION_FAIL = 205, // connection to the AP has failed
    // DEVICE_WIFI_REASON_AP_TSF_RESET             = 206,
    // DEVICE_WIFI_REASON_ROAMING                  = 207,

    DEVICE_WIFI_REASON_TIMEOUT            = 401, // timeout reported (on application level)
    DEVICE_WIFI_REASON_MAX_RETRY_EXCEEDED = 402, // max retry number exceeded (on application level)
    DEVICE_WIFI_REASON_OTHER              = 500  // unknown reason, check low-level code to verify exact cause
};

enum class ECloudConnectionStatus : uint8_t
{
    CLOUD_STATUS_NOT_CONFIGURED = 0,
    CLOUD_STATUS_DISABLED       = 1,
    CLOUD_STATUS_CONNECTED      = 2,
    CLOUD_STATUS_NOT_CONNECTED  = 3,
};

enum class ECloudDeviceProvisioningStatus : uint8_t
{
    PROVISIONING_STATUS_INIT        = 0,
    PROVISIONING_STATUS_IN_PROGRESS = 1,
    PROVISIONING_STATUS_FINISHED    = 2,
};

namespace prot
{

constexpr uint16_t RESULT_FLAG = 0x1000;

/** List of type codes for packets */
enum class EPacketType : uint16_t
{
    // GENERAL
    NONE = 0x0000, ///< No packet

    GENERAL__START  = 0x0001,          ///< Helper enum - beginning of general packets
    ACK             = 0x0001,          ///< Generic acknowledge, no payload
    NACK            = 0x0002,          ///< Generic non-acknowledge, no payload
    CMD_UNSUPPORTED = 0x0003,          ///< Command not supported
    GENERAL__END    = CMD_UNSUPPORTED, ///< Helper enum - end of general packets

    // COMMANDS
    CMD__START = 0x0010, ///< Helper enum - beginning of command
    CMD_TEST   = 0x0010, ///< Communication test command

    CMD_get_wifi_mac_address = 0x0020, ///< Get BLE MAC address

    CMD_SEND_CERTIFICATES = 0x0050, ///< Send files with certificates from BLE device to ESP

    CMD__END = CMD_SEND_CERTIFICATES, ///< Helper enum - end of command

    // RESULTS response for commands. If possible: request | 0x1000
    RES__START = 0x1010,                 ///< Helper enum - beginning of responses
    RES_TEST   = CMD_TEST | RESULT_FLAG, ///< Communication test response

    RES_get_wifi_mac_address = CMD_get_wifi_mac_address | RESULT_FLAG, ///< Response to get BLE MAC address

    RES_SEND_CERTIFICATES = CMD_SEND_CERTIFICATES | RESULT_FLAG,

    // Special "asynchronous" responses - sent without any request
    RES_ASYNC__WIFI_CONNECTED_TO_AP      = 0x10A1, ///< Sent when we connect to AP
    RES_ASYNC__WIFI_DISCONNECTED_FROM_AP = 0x10A2, ///< Sent when we disconnect from a WiFi Access Point
    RES_ASYNC__CLOUD_CONNECTED           = 0x10A3, ///< Send when we get connected to the cloud
    RES_ASYNC__CLOUD_DISCONNECTED        = 0x10A4, ///< Send when we get disconnected from the cloud

    RES__END = RES_ASYNC__CLOUD_DISCONNECTED, ///< Helper enum - end of responses

};

namespace test_command
{

struct __attribute__((packed)) TCmd
{
    char message[32]; ///< null terminated string message
};

struct __attribute__((packed)) TRes
{
    char message[32]; ///< null terminated string message
};

} // namespace test_command

namespace wifi_scan_ap
{

/** Maximum number of WiFi networks that can be scanned (returned from the scan). Needs to be equal to
 * CONFIG_WIFI_PROV_SCAN_MAX_ENTRIES **/
constexpr uint8_t MAX_NUMBER_OF_NETWORKS = 20;
constexpr uint8_t MAX_WIFI_SSID_LENGTH   = 32;
constexpr uint8_t BSSID_OR_MAC_LENGTH    = 6;

/** Request setup of a 'WiFi scan AP' command */
struct __attribute__((packed)) TCmd
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TScanEntry
{
    uint8_t                   ssid[MAX_WIFI_SSID_LENGTH + 1];
    uint8_t                   bssid[BSSID_OR_MAC_LENGTH];
    uint8_t                   primaryChannel;
    device_wifi_second_chan_t secondaryChannel;
    int8_t                    rssi;
    device_wifi_auth_mode_t   authmode;
};

/** Response of a 'WiFi scan AP' command */
struct __attribute__((packed)) TRes
{
    TScanEntry availableNetworks[MAX_NUMBER_OF_NETWORKS]; ///< Hold data for the scanned networks
    uint8_t    numberOfValidEntries;                      ///< number of meaningful entries in apFound

    bool scannedSuccsfully;
};

} // namespace wifi_scan_ap

namespace wifi_connect_to_ap
{

constexpr uint8_t MAX_WIFI_PASSWORD_LENGTH = 64;
constexpr uint8_t MAX_WIFI_SSID_LENGTH     = wifi_scan_ap::MAX_WIFI_SSID_LENGTH;

struct __attribute__((packed)) TWiFiCredentials
{
    char ssid[MAX_WIFI_SSID_LENGTH + 1];         // null terminated, +1 for the last null
    char password[MAX_WIFI_PASSWORD_LENGTH + 1]; // null terminated, +1 for the last null
    device_wifi_auth_mode_t
        authorizationMode; // optional authorization mode to use. If not set WPA2 or AUTH_OPEN will be used

    TWiFiCredentials();

    bool operator==(const TWiFiCredentials& rhs) const;
    bool setSsid(const std::string& newSsid);
    bool setPassword(const std::string& newPassword);
    bool isSet() const;
};

struct __attribute__((packed)) TCmd
{
    TWiFiCredentials credentials;
    int32_t timeoutMs; ///< [ms]. Not only time to wait for WiFi to connect, but also to retry the connection (WiFi
                       ///< sometimes fails to connect). Doesn't inlcude additional time to stop the WiFi module.
                       ///< Negative value means no timeout
};

struct __attribute__((packed)) TRes
{
    bool succeded; ///< Whether the operation succeded

    EConnectToSsidErrorReason status; ///< Description and reason of failure
};

} // namespace wifi_connect_to_ap

namespace wifi_disconnect_from_ap
{

struct __attribute__((packed)) TCmd
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TRes
{
    bool succeded; ///< Whether the operation succeded
};

} // namespace wifi_disconnect_from_ap

namespace wifi_save_ap_credentials
{

struct __attribute__((packed)) TCmd
{
    wifi_connect_to_ap::TWiFiCredentials credentials;
};

struct __attribute__((packed)) TRes
{
    bool succeded; ///< Whether the operation succeded
};

} // namespace wifi_save_ap_credentials

namespace global_get_status
{

constexpr uint8_t MAX_VERSION_STRING_LENGTH   = 11;
constexpr uint8_t MAC_ADDRESS_LENGTH_IN_BYTES = wifi_scan_ap::BSSID_OR_MAC_LENGTH;

struct __attribute__((packed)) TGlobalStatus
{
    bool                   isWiFiConnected;
    uint64_t               timeSinceDeviceStartupMs;
    uint8_t                macBle[MAC_ADDRESS_LENGTH_IN_BYTES];
    uint8_t                macWiFi[MAC_ADDRESS_LENGTH_IN_BYTES];
    char                   projectVersion[MAX_VERSION_STRING_LENGTH + 1];
    ECloudConnectionStatus cloudConnectionStatus;
};

struct __attribute__((packed)) TCmd
{
    uint8_t dummyByte;
};

struct TRes
{
    TGlobalStatus status; ///< Requested information
};

} // namespace global_get_status

namespace reset_esp
{

struct __attribute__((packed)) TCmd
{
    uint8_t dummyByte;
};

} // namespace reset_esp

namespace cloud_set_credentials
{
constexpr uint8_t  CLOUD_MAX_KEY_LENGTH       = 100;
constexpr uint8_t  CLOUD_MAX_ADDRESS_LENGTH   = 200;
constexpr uint8_t  CLOUD_DEVICE_ID_LENGTH     = 64;
constexpr uint32_t CLOUD_MQTT_USERNAME_LENGTH = CLOUD_MAX_ADDRESS_LENGTH + CLOUD_DEVICE_ID_LENGTH + 32;

std::string getCloudConnectionStatus(enum ECloudConnectionStatus cloudConnectionStatus);

/**
 * @brief Structure to store credentials for cloud connection. privateKey may contain
 *      only alphanumeric characters in ASCII code and needs to be NULL terminated
 */
struct __attribute__((packed)) TCloudCredentials
{
    char cloudAddress[CLOUD_MAX_ADDRESS_LENGTH];
    char cloudDeviceId[CLOUD_DEVICE_ID_LENGTH];
    char cloudMqttUsername[CLOUD_MQTT_USERNAME_LENGTH];

    TCloudCredentials();

    bool operator==(const TCloudCredentials& rhs) const;
    bool setCloudAddress(const std::string& newAddress);
    bool setCloudDeviceId(const std::string& newDeviceId);
    bool setCloudMqttUsername(const std::string& newMqttUsername);
    bool isSetCloudAddress() const;
    bool isSetCloudDeviceId() const;
    bool isSetCloudMqttUsername() const;
};
struct __attribute__((packed)) TCmd
{
    TCloudCredentials cloudCredentials;
};

struct __attribute__((packed)) TRes
{
    uint8_t dummyByte;
};

} // namespace cloud_set_credentials

namespace send_certificates
{
constexpr uint32_t MAX_FULLCHAIN_CERTIFICATE_LENGTH = 6000;
constexpr uint32_t MAX_PRIVATE_KEY_LENGTH           = 2000;

struct __attribute__((packed)) TCertificatePack
{
    char fullChainCertificate[MAX_FULLCHAIN_CERTIFICATE_LENGTH + 1];
    char privateKey[MAX_PRIVATE_KEY_LENGTH + 1];

    bool operator==(const TCertificatePack& rhs) const;
    bool setFullChainCertificate(const std::string& newFullChainCertificate);
    bool setPrivateKey(const std::string& newPrivateKey);

    bool isSetFullChainCertificate() const;
    bool isSetPrivateKey() const;
};

struct __attribute__((packed)) TCmd
{
    TCertificatePack certificates;
};

struct __attribute__((packed)) TRes
{
    uint8_t dummyByte;
};
} // namespace send_certificates

namespace get_wifi_mac_address
{
constexpr uint8_t WIFI_MAC_ADDRESS_LENGTH = 6;

struct __attribute__((packed)) TCmd
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TRes
{
    uint8_t wifiMacAddress[WIFI_MAC_ADDRESS_LENGTH];
};
} // namespace get_wifi_mac_address

namespace ota_perform
{
constexpr uint8_t  MAX_OTA_LINK_LENGTH       = 200;
constexpr uint32_t CLOUD_OTA_CERT_MAX_LENGTH = 2000;

struct __attribute__((packed)) THttpsServerCertificate
{
    char serverCertificate[CLOUD_OTA_CERT_MAX_LENGTH + 1];

    bool operator==(const THttpsServerCertificate& rhs) const;
    bool isSet() const;
};

struct __attribute__((packed)) TOtaUpdateLink
{
    char firmwareLink[MAX_OTA_LINK_LENGTH + 1];

    bool operator==(const TOtaUpdateLink& rhs) const;

    bool isSet() const;
};

struct __attribute__((packed)) TCmd
{
    TOtaUpdateLink          otaUpdateLink;
    THttpsServerCertificate httpsServerCertificate;
};

struct __attribute__((packed)) TRes
{
    bool updateSuccess;
};
} // namespace ota_perform

// Asynchronous events

struct __attribute__((packed)) TAsyncEvent_WiFiConnectedToAp
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TAsyncEvent_WiFiDisonnectedFromAp
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TAsyncEvent_CloudConnected
{
    uint8_t dummyByte;
};

struct __attribute__((packed)) TAsyncEvent_CloudDisconnected
{
    uint8_t dummyByte;
};

// struct __attribute__((packed)) res_ota_progress_report
//{
//     uint32_t bytesFlashed;  ///< Number of bytes already downloaded from the server and flashed to the device
// };

// struct __attribute__((packed)) res_ota_result_report
//{
//     uint32_t bytesFlashed;  ///< Number of bytes downloaded from the server and flashed to the device
//     bool finishedSuccesfully;  ///< Whether operation finished succesfully
// };

} // namespace prot

#endif // PROTOCOL_WIFI_TYPES_H
