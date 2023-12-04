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

static EPacketHandlingResult handleResponse_test(
        BleController* pController,
        const uint8_t* payload,
        uint16_t payloadLen)
{
    UNUSED(pController);
    LOG_DEBUG("Handling response for 'test' command...");

    if (payloadLen != sizeof(prot::test_command::TRes))
    {
        LOG_WARNING("Invalid 'test' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::test_command::TRes* pRes = (prot::test_command::TRes*)payload;


    LOG_INFO("Test Response %s", pRes->message);

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_getWifiMacAddress(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling response for Get WIFI MAC Address");

    if (payloadLen != sizeof(prot::get_wifi_mac_address::TRes))
    {
        LOG_WARNING("Invalid 'Get Wifi MAC Address' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::get_wifi_mac_address::TRes *pRes = (prot::get_wifi_mac_address::TRes*)payload;
    LOG_INFO("MAC Address obtained from device");
    printMacAddress(pRes->wifiMacAddress, "WIFI");

    return EPacketHandlingResult::SEND_ACK;
}

static EPacketHandlingResult handleResponse_sendCertificates(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling response for Send Certificates command");

    if (payloadLen != sizeof(prot::send_certificates::TRes))
    {
        LOG_WARNING("Invalid 'Send Certificates' response. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    prot::send_certificates::TRes *pRes = (prot::send_certificates::TRes*)payload;

    LOG_INFO("Certificates sent successfully");

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
        case prot::EPacketType::RES_get_wifi_mac_address:
            return handleResponse_getWifiMacAddress(pController, payload, payloadLen);
        case prot::EPacketType::RES_SEND_CERTIFICATES:
            return handleResponse_sendCertificates(pController, payload, payloadLen);
        default:
            LOG_ERROR("Unknown packet type: %d (0x%04X). Length %u", (int)packetType, (uint16_t)packetType, payloadLen);
            return EPacketHandlingResult::SEND_NACK;
    }
}

#endif // IS_PC
