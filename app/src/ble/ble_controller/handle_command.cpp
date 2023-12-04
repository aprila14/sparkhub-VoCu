// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char *LOG_TAG = "HandleCommand";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#ifdef IS_ESP

#include "handle_command.h"

#include "app_controller.h"
#include "app_events.h"
#include "commons.h"
#include "config_nvs.h"
#include "sleep.h"
#include "wifi_controller.h"

#include "esp_system.h"

#include <cstdio>

static EPacketHandlingResult handleCommand_test(BleController *pController, const uint8_t *payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'test'...");

    if (payloadLen != sizeof(prot::test_command::TCmd))
    {
        LOG_WARNING("Invalid 'test' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
    const prot::test_command::TCmd *pCmd = reinterpret_cast<const prot::test_command::TCmd *>(payload); // NOLINT - we need reinterpret cast

    LOG_INFO("Command 'test', message %s", pCmd->message);

    prot::test_command::TRes res = {};
    sprintf(res.message, "HELLO WORLD from ESP!");

    LOG_INFO("About to send response RES_TEST...");
    bool result = pController->sendPacket(prot::EPacketType::RES_TEST, reinterpret_cast<uint8_t *>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'test' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand_sendCertificates(BleController *pController, const uint8_t *payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command send certificates");

    if (payloadLen != sizeof(prot::send_certificates::TCmd))
    {
        LOG_WARNING("Invalid 'Send Certificates' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::send_certificates::TCmd *pCmd = reinterpret_cast<const prot::send_certificates::TCmd *>(payload); // NOLINT - we need reinterpret cast
    prot::send_certificates::TRes res = {};

    pConfig->setCertificatePack(pCmd->certificates);

    LOG_INFO("\n\nReceived fullchain certificate: %s\n\n", pCmd->certificates.fullChainCertificate);
    LOG_INFO("\n\nReceived private key: %s\n\n", pCmd->certificates.privateKey);
    LOG_INFO("About to send response RES_CLOUD_SEND_CERTIFICATES");

    bool result = pController->sendPacket(prot::EPacketType::RES_SEND_CERTIFICATES, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast

    if (!result)
    {
        LOG_ERROR("Failed to send response for 'Send Certificates' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand_getWifiMacAddress(BleController *pController, const uint8_t *payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'Get Wifi MAC Address");

    if (payloadLen != sizeof(prot::get_wifi_mac_address::TCmd))
    {
        LOG_WARNING("Invalid 'Get Wifi MAC Address' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::get_wifi_mac_address::TCmd *pCmd = reinterpret_cast<const prot::get_wifi_mac_address::TCmd *>(payload); // NOLINT - we need reinterpret cast
    prot::get_wifi_mac_address::TRes res = {};

    if (esp_read_mac(res.wifiMacAddress, ESP_MAC_WIFI_STA) != ESP_OK)
    {
        LOG_ERROR("Failed to read ESP WIFI MAC Address");
        return EPacketHandlingResult::SEND_NACK;
    }

    bool result = pController->sendPacket(prot::EPacketType::RES_get_wifi_mac_address, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast

    if (!result)
    {
        LOG_ERROR("Failed to send response for 'Get Wifi MAC Address' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand(BleController *pController, prot::EPacketType packetType, const uint8_t *payload, uint16_t payloadLen)
{
    switch (packetType)
    {
    case prot::EPacketType::CMD_TEST:
        return handleCommand_test(pController, payload, payloadLen);
    case prot::EPacketType::CMD_SEND_CERTIFICATES:
        return handleCommand_sendCertificates(pController, payload, payloadLen);
    case prot::EPacketType::CMD_get_wifi_mac_address:
        return handleCommand_getWifiMacAddress(pController, payload, payloadLen);
    default:
        LOG_ERROR("Unknown command packet type: %d (0x%04X). Length %u", packetType, packetType, payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
}

#endif // IS_ESP
