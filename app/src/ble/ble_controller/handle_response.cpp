// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char *LOG_TAG = "HandleResponse";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#ifdef IS_PC

#include "handle_response.h"

#include <assert.h>
#include <string.h>

void printMacAddress(uint8_t *address, const char *type)
{
    LOG_INFO("%s MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
             type, address[0], address[1],
             address[2], address[3],
             address[4], address[5]);
}

void printGlobalStatus(prot::global_get_status::TGlobalStatus status)
{
    std::string wifiConnectionStatusText = status.isWiFiConnected ? "CONNECTED" : "DISCONNECTED";

    std::string cloudConnectionStatusText = prot::cloud_set_credentials::getCloudConnectionStatus(status.cloudConnectionStatus);

    LOG_INFO("\n*****Global status information*****\n"
             "WiFi connection state: %s\n"
             "Cloud connection state: %s\n"
             "Light percentage level: %d\n"
             "Current time [ms]: %u\n"
             "Firmware version: %s\n",
             wifiConnectionStatusText.c_str(),
             cloudConnectionStatusText.c_str(),
             status.lightPercentageLevel,
             static_cast<uint32_t>(status.timeSinceDeviceStartupMs),
             status.projectVersion);

    printMacAddress(status.macBle, "BLE");
    printMacAddress(status.macWiFi, "WiFi");
}
static EPacketHandlingResult handleResponse_test(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'test' command...");

    if (payloadLen != sizeof(prot::test_command::TRes))
    {
        LOG_WARNING("Invalid 'test' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::test_command::TRes *pRes = (prot::test_command::TRes *)payload;

    LOG_INFO("Test Response %s", pRes->message);

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_wiFiConnectToAp(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'WiFi connect to AP' command...");

    if (payloadLen != sizeof(prot::wifi_connect_to_ap::TRes))
    {
        LOG_WARNING("Invalid 'WiFi connect to AP' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::wifi_connect_to_ap::TRes *pRes = (prot::wifi_connect_to_ap::TRes *)payload;

    if (!pRes->succeded)
    {
        LOG_WARNING("Not connected!");
    }

    LOG_INFO("WiFi connection: succeded = %d, status = %d",
             pRes->succeded,
             static_cast<int>(pRes->status));

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_wiFiDisconnectFromAp(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'WiFi Disconnect From AP' command...");
    if (payloadLen != sizeof(prot::wifi_disconnect_from_ap::TRes))
    {
        LOG_WARNING("Invalid 'WiFi Disconnect From AP' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::wifi_disconnect_from_ap::TRes *pRes = (prot::wifi_disconnect_from_ap::TRes *)payload;

    if (!pRes->succeded)
    {
        LOG_WARNING("Failed to disconnect!");
    }

    LOG_INFO("WiFi disconnection: succeded = %d",
             pRes->succeded);

    return EPacketHandlingResult::SEND_ACK;
}
static EPacketHandlingResult handleResponse_wiFiScanAp(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'WiFi scan AP' command...");

    if (payloadLen != sizeof(prot::wifi_scan_ap::TRes))
    {
        LOG_WARNING("Invalid 'WiFi scan AP' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::wifi_scan_ap::TRes *pRes = (prot::wifi_scan_ap::TRes *)payload;

    char scanBuff[prot::wifi_scan_ap::MAX_NUMBER_OF_NETWORKS * 100] = {};
    int charsWritten = 0;
    for (int i = 0; i < pRes->numberOfValidEntries; i++)
    {
        int remainingSize = (int)sizeof(scanBuff) - 1 - charsWritten;
        if (remainingSize <= 1)
            break;

        charsWritten += snprintf(scanBuff + charsWritten, (uint32_t)remainingSize,
                                 "\t %d) '%s' (%d) MAC=%02X:%02X:%02X:%02X:%02X:%02X\n",
                                 i, pRes->availableNetworks[i].ssid, pRes->availableNetworks[i].rssi,
                                 pRes->availableNetworks[i].bssid[0], pRes->availableNetworks[i].bssid[1],
                                 pRes->availableNetworks[i].bssid[2], pRes->availableNetworks[i].bssid[3],
                                 pRes->availableNetworks[i].bssid[4], pRes->availableNetworks[i].bssid[5]);
    }

    LOG_INFO("WiFi scan AP. Found %d AP:\n %s",
             pRes->numberOfValidEntries, scanBuff);

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_wifiSaveApCredentials(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'WiFi Save AP Credentials' command...");

    if (payloadLen != sizeof(prot::wifi_save_ap_credentials::TRes))
    {
        LOG_WARNING("Invalid 'WiFi Save AP Credentials' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::wifi_save_ap_credentials::TRes *pRes = (prot::wifi_save_ap_credentials::TRes *)payload;

    if (!pRes->succeded)
    {
        LOG_WARNING("Failed to save AP credentials!");
    }
    else
    {
        LOG_INFO("AP credentials saved correctly");
    }

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_globalGetStatus(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'Global Get Status' command...");

    if (payloadLen != sizeof(prot::global_get_status::TRes))
    {
        LOG_WARNING("Invalid 'Global Get Status' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::global_get_status::TRes *pRes = (prot::global_get_status::TRes *)payload;

    printGlobalStatus(pRes->status);
    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_otaPerform(BleController *pController, const uint8_t *payload, uint16_t payloadLen)
{
    LOG_DEBUG("Handling response for 'Ota Perform command...");

    if (payloadLen != sizeof(prot::ota_perform::TRes))
    {
        LOG_WARNING("Invalid 'Ota Perform' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::ota_perform::TRes *pRes = (prot::ota_perform::TRes *)payload;

    if (pRes->updateSuccess)
    {
        LOG_INFO("OTA performed correctly, device shall be restarted soon");
    }
    else
    {
        LOG_INFO("Could not perform Ota update");
    }

    return EPacketHandlingResult::SEND_ACK;
}
static EPacketHandlingResult handleResponse_async_wiFiConnectedToAp(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    LOG_INFO("!!! Async event: WIFI CONNECTED TO AP");

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_async_wiFiDisconnectedFromAp(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    LOG_INFO("!!! Async event: WIFI DISCONNECTED FROM AP");

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_async_cloudConnected(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    LOG_INFO("!!! Async event: CLOUD CONNECTED");

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_async_cloudDisconnected(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    LOG_INFO("!!! Async event: CLOUD DISCONNECTED");

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_cloudSendCredentials(
    BleController *pController,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    LOG_INFO("Handling response for Cloud Send Credentials command...");

    if (payloadLen != sizeof(prot::cloud_set_credentials::TRes))
    {
        LOG_WARNING("Invalid 'Cloud Send Credentials' response. Payload legnth: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::cloud_set_credentials::TRes *pRes = (prot::cloud_set_credentials::TRes *)payload;

    LOG_INFO("Cloud credentials set successfully");

    return EPacketHandlingResult::SEND_ACK;
}
EPacketHandlingResult handleResponse(
    BleController *pController,
    prot::EPacketType packetType,
    const uint8_t *payload,
    uint16_t payloadLen)
{
    switch (packetType)
    {
    case prot::EPacketType::RES_TEST:
        return handleResponse_test(pController, payload, payloadLen);

    case prot::EPacketType::RES_WIFI_CONNECT_TO_AP:
        return handleResponse_wiFiConnectToAp(pController, payload, payloadLen);
    case prot::EPacketType::RES_WIFI_DISCONNECT_FROM_AP:
        return handleResponse_wiFiDisconnectFromAp(pController, payload, payloadLen);
    case prot::EPacketType::RES_WIFI_SCAN_AP:
        return handleResponse_wiFiScanAp(pController, payload, payloadLen);
    case prot::EPacketType::RES_WIFI_SAVE_AP_CREDENTIALS:
        return handleResponse_wifiSaveApCredentials(pController, payload, payloadLen);
    case prot::EPacketType::RES_GLOBAL_GET_STATUS:
        return handleResponse_globalGetStatus(pController, payload, payloadLen);
    case prot::EPacketType::RES_ASYNC__WIFI_CONNECTED_TO_AP:
        return handleResponse_async_wiFiConnectedToAp(pController, payload, payloadLen);
    case prot::EPacketType::RES_ASYNC__WIFI_DISCONNECTED_FROM_AP:
        return handleResponse_async_wiFiDisconnectedFromAp(pController, payload, payloadLen);
    case prot::EPacketType::RES_ASYNC__CLOUD_CONNECTED:
        return handleResponse_async_cloudConnected(pController, payload, payloadLen);
    case prot::EPacketType::RES_ASYNC__CLOUD_DISCONNECTED:
        return handleResponse_async_cloudDisconnected(pController, payload, payloadLen);
    case prot::EPacketType::RES_CLOUD_SEND_CREDENTIALS:
        return handleResponse_cloudSendCredentials(pController, payload, payloadLen);
    case prot::EPacketType::RES_OTA_PERFORM:
        return handleResponse_otaPerform(pController, payload, payloadLen);
    default:
        LOG_ERROR("Unknown packet type: %d (0x%04X). Length %u", (int)packetType, (uint16_t)packetType, payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
}

#endif // IS_PC
