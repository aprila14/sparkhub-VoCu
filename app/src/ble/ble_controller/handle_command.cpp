// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "HandleCommand";
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



static EPacketHandlingResult handleCommand_test(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'test'...");

    if (payloadLen != sizeof(prot::test_command::TCmd))
    {
        LOG_WARNING("Invalid 'test' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
    const prot::test_command::TCmd* pCmd = reinterpret_cast<const prot::test_command::TCmd*>(payload);  // NOLINT - we need reinterpret cast

    LOG_INFO("Command 'test', message %s", pCmd->message);


    prot::test_command::TRes res = {};
    sprintf(res.message, "HELLO WORLD from ESP!");

    LOG_INFO("About to send response RES_TEST...");
    bool result = pController->sendPacket(prot::EPacketType::RES_TEST, reinterpret_cast<uint8_t*>(&res), sizeof(res));  // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'test' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

static EPacketHandlingResult handleCommand_wiFiConnectToAp(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'WiFi connect to AP'...");

    if (payloadLen != sizeof(prot::wifi_connect_to_ap::TCmd))
    {
        LOG_WARNING("Invalid 'WiFi Scan AP' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
    const prot::wifi_connect_to_ap::TCmd* pCmd = reinterpret_cast<const prot::wifi_connect_to_ap::TCmd*>(payload);  // NOLINT - we need reinterpret cast
    prot::wifi_connect_to_ap::TRes res = {};

    constexpr bool SHOULD_NOT_RECONNECT = false;  // just a constant for passing as a parameter

    bool functionSucceded = app::pAppController->getWiFiController()->connectToAccessPoint(
                *pCmd, res, SHOULD_NOT_RECONNECT);

    LOG_INFO("Connection result: %d (0 - no error)", res.status);
    if (!functionSucceded)
    {
        LOG_ERROR("connectToAccessPoint() failed!");
        return EPacketHandlingResult::SEND_NACK;
    }

    LOG_INFO("About to send response RES_WIFI_CONNECT_TO_AP...");
    bool result = pController->sendPacket(prot::EPacketType::RES_WIFI_CONNECT_TO_AP, reinterpret_cast<uint8_t*>(&res), sizeof(res));  // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'WiFi Connect to AP' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

static EPacketHandlingResult handleCommand_wiFiDisconnectFromAp(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'WiFi Disconnect From AP'...");

    if (payloadLen != sizeof(prot::wifi_disconnect_from_ap::TCmd))
    {
        LOG_WARNING("Invalid WiFi Disconnect From AP command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::wifi_disconnect_from_ap::TCmd* pCmd = reinterpret_cast<const prot::wifi_disconnect_from_ap::TCmd*>(payload); // NOLINT - we need reinterpret cast
    prot::wifi_disconnect_from_ap::TRes res = {};

    bool functionSucceded = app::pAppController->getWiFiController()->disconnectFromAccessPoint();
    if (!functionSucceded)
    {
        LOG_ERROR("disconnectFromAccessPoint() failed!");
        return EPacketHandlingResult::SEND_NACK;
    }

    res.succeded = true;

    LOG_INFO("About to send response RES_WIFI_DISCONNECT_FROM_AP ...");
    bool result = pController->sendPacket(prot::EPacketType::RES_WIFI_DISCONNECT_FROM_AP, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'WiFi Disconnect From AP' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

static EPacketHandlingResult handleCommand_wiFiScanAp(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'WiFi Scan AP'...");

    if (payloadLen != sizeof(prot::wifi_scan_ap::TCmd))
    {
        LOG_WARNING("Invalid 'WiFi Scan AP' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }
    const prot::wifi_scan_ap::TCmd* pCmd = reinterpret_cast<const prot::wifi_scan_ap::TCmd*>(payload);  // NOLINT - we need reinterpret cast

    prot::wifi_scan_ap::TRes res = {};
    bool functionSucceded = app::pAppController->getWiFiController()->scanAvailableAccessPoints(res);

    if (!functionSucceded)
    {
        LOG_ERROR("scanAvailableAccessPoints() failed!");
        return EPacketHandlingResult::SEND_NACK;
    }

    LOG_INFO("About to send response RES_WIFI_SCAN_AP...");
    bool result = pController->sendPacket(prot::EPacketType::RES_WIFI_SCAN_AP, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'WiFi Scan AP' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}
static EPacketHandlingResult handleCommand_wiFiSaveApCredentials(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'WiFi Save AP Credentials'...");

    if (payloadLen != sizeof(prot::wifi_save_ap_credentials::TCmd))
    {
        LOG_WARNING("Invalid 'WiFi Save AP Credentials' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::wifi_save_ap_credentials::TCmd* pCmd = reinterpret_cast<const prot::wifi_save_ap_credentials::TCmd*>(payload); // NOLINT - we need reinterpret cast

    pConfig->setWifiCredentials(pCmd->credentials);

    prot::wifi_save_ap_credentials::TRes res = {};
    res.succeded = true;

    LOG_INFO("About to send response RES_WIFI_SAVE_AP_CREDENTIALS...");
    bool result = pController->sendPacket(prot::EPacketType::RES_WIFI_SAVE_AP_CREDENTIALS,
                                          reinterpret_cast<uint8_t*>(&res), // NOLINT - we need reinterpret cast
                                          sizeof(res));
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'WiFi Save AP Credentials' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

prot::global_get_status::TGlobalStatus fillStatus()
{
    prot::global_get_status::TGlobalStatus result = {};

    result.isWiFiConnected = app::pAppController->getWiFiController()->getConnectionStatus();
    result.timeSinceDeviceStartupMs = commons::getCurrentTimestampMs();
    result.lightPercentageLevel = app::pAppController->getLightControlInterface()->getPower();

    result.cloudConnectionStatus = app::pAppController->getCloudController()->getConnectionStatus();

    commons::printWarningCode(esp_read_mac(result.macBle, ESP_MAC_BT), "BLE MAC address could not be retrieved");
    commons::printWarningCode(esp_read_mac(result.macWiFi, ESP_MAC_WIFI_STA), "WIFI MAC address could not be retrieved");

    static_assert(sizeof(PROJECT_VER) < prot::global_get_status::MAX_VERSION_STRING_LENGTH, "Project version string exceeded maximum length");
    strcpy(result.projectVersion, PROJECT_VER);

    return result;
}

EPacketHandlingResult handleCommand_globalGetStatus(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'global Get Status");

    if (payloadLen != sizeof(prot::global_get_status::TCmd))
    {
        LOG_WARNING("Invalid 'global Get Status' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::global_get_status::TCmd* pCmd =
            reinterpret_cast<const prot::global_get_status::TCmd*>(payload); // NOLINT - we need reinterpret cast

    prot::global_get_status::TRes res = {};

    res.status = fillStatus();

    LOG_INFO("About to send response RES_GLOBAL_GET_STATUS...");
    bool result = pController->sendPacket(prot::EPacketType::RES_GLOBAL_GET_STATUS, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'Global Get Status' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand_lightSetLevel(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'Light Set Level'...");

    if (payloadLen != sizeof(prot::light_set_level::TCmd))
    {
        LOG_WARNING("Invalid 'light Set Level' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::light_set_level::TCmd* pCmd = reinterpret_cast<const prot::light_set_level::TCmd*>(payload); // NOLINT - we need reinterpret cast

    prot::light_set_level::TRes res = {};

    app::TEventData eventData = { };
    eventData.lightControlSetPower.percentage = pCmd->lightPercentageLevel;

    bool functionSucceded = app::pAppController->addEvent(app::EEventType::LIGHT_CONTROL__SET_POWER, app::EEventExecutionType::SYNCHRONOUS, &eventData);

    if (!functionSucceded)
    {
        LOG_ERROR("LightControlInterface::setPower() failed!");
        return EPacketHandlingResult::SEND_NACK;
    }

    LOG_INFO("About to send response RES_LIGHT_SET_LEVEL...");
    bool result = pController->sendPacket(prot::EPacketType::RES_LIGHT_SET_LEVEL, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'Light Set Level' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand_resetEsp(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    UNUSED(pController);
    UNUSED(payload);
    UNUSED(payloadLen);

    LOG_INFO("Handling command 'reset ESP'...");

    LOG_INFO("About to reset in 500 ms...");
    SLEEP_MS(500);
    esp_restart();

    return EPacketHandlingResult::UNKNOWN;  // will not happen, because we called reset above, but compiler is complaining
}

EPacketHandlingResult handleCommand_cloudSendCredentials(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'Cloud Send Credentials'...");
    if (payloadLen != sizeof(prot::cloud_set_credentials::TCmd))
    {
        LOG_WARNING("Invalid 'Cloud Send Credentials' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::cloud_set_credentials::TCmd* pCmd = reinterpret_cast<const prot::cloud_set_credentials::TCmd*>(payload); // NOLINT - we need reinterpret cast

    prot::cloud_set_credentials::TRes res = {};

    app::TEventData eventData = {};
    eventData.cloudControlSendCredentials.connectionReady = true;

    pConfig->setCloudCredentials(pCmd->cloudCredentials);
    pConfig->setCloudCertificates(pCmd->cloudCertificates);

    bool functionSucceeded = app::pAppController->addEvent(app::EEventType::CLOUD_CONTROLLER__SEND_CREDENTIALS, app::EEventExecutionType::SYNCHRONOUS, &eventData);

    if (!functionSucceeded)
    {
        LOG_ERROR("CloudControlModule::sendCredentials() failed!");
        return EPacketHandlingResult::SEND_NACK;
    }

    LOG_INFO("About to send respose RES_CLOUD_SEND_CREDENTIALS...");

    bool result = pController->sendPacket(prot::EPacketType::RES_CLOUD_SEND_CREDENTIALS, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
    if (!result)
    {
        LOG_ERROR("Failed to send response for 'Cloud Send Credentials' command");
        return EPacketHandlingResult::SEND_NACK;
    }

    return EPacketHandlingResult::HANDLED;
}

EPacketHandlingResult handleCommand_otaPerform(BleController* pController, const uint8_t* payload, uint16_t payloadLen)
{
    LOG_INFO("Handling command 'OTA_PEFORM'...");
    if (payloadLen != sizeof(prot::ota_perform::TCmd))
    {
        LOG_WARNING("Invalid 'OTA Perform' command. Payload length: %d", payloadLen);
        return EPacketHandlingResult::SEND_NACK;
    }

    const prot::ota_perform::TCmd* pCmd = reinterpret_cast<const prot::ota_perform::TCmd*>(payload); // NOLINT - we need reinterpret cast
    prot::ota_perform::TRes res = {};

    app::TEventData eventData = {};

    eventData.otaPerform.updateReady = true;
    pConfig->setOtaUpdateLink(pCmd->otaUpdateLink);
    pConfig->setOtaCertificate(pCmd->httpsServerCertificate);

    bool functionSucceeded = app::pAppController->addEvent(app::EEventType::OTA__PERFORM, app::EEventExecutionType::SYNCHRONOUS, &eventData);

    if (functionSucceeded)
    {
        res.updateSuccess = true;
        bool result = pController->sendPacket(prot::EPacketType::RES_OTA_PERFORM, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast
        if (result)
        {
            LOG_INFO("About to reset in 2000 ms...");
            SLEEP_MS(2000);
            esp_restart();
            return EPacketHandlingResult::HANDLED;  // This instruction will not be executed due to esp restart, but compiler would be complaining if it was not here
        }
        else
        {
            LOG_INFO("Failed to send response confirming successful OTA update");
            return EPacketHandlingResult::SEND_NACK;
        }
    }
    else
    {
        LOG_ERROR("Could not perform OTA");
        res.updateSuccess = false;
        bool result = pController->sendPacket(prot::EPacketType::RES_OTA_PERFORM, reinterpret_cast<uint8_t*>(&res), sizeof(res)); // NOLINT - we need reinterpret cast

        if (!result)
        {
            LOG_ERROR("Failed to send response after unsuccessful OTA update");
            return EPacketHandlingResult::SEND_NACK;
        }

        return EPacketHandlingResult::HANDLED;
    }
}

EPacketHandlingResult handleCommand(BleController* pController, prot::EPacketType packetType,  const uint8_t* payload, uint16_t payloadLen)
{
    switch (packetType)
    {
        case prot::EPacketType::CMD_TEST:
            return handleCommand_test(pController, payload, payloadLen);

        case prot::EPacketType::CMD_WIFI_CONNECT_TO_AP:
                return handleCommand_wiFiConnectToAp(pController, payload, payloadLen);
        case prot::EPacketType::CMD_WIFI_DISCONNECT_FROM_AP:
                return handleCommand_wiFiDisconnectFromAp(pController, payload, payloadLen);
        case prot::EPacketType::CMD_WIFI_SCAN_AP:
                return handleCommand_wiFiScanAp(pController, payload, payloadLen);
        case prot::EPacketType::CMD_WIFI_SAVE_AP_CREDENTIALS:
                return handleCommand_wiFiSaveApCredentials(pController, payload, payloadLen);
        case prot::EPacketType::CMD_GLOBAL_GET_STATUS:
                return handleCommand_globalGetStatus(pController, payload, payloadLen);
        case prot::EPacketType::CMD_LIGHT_SET_LEVEL:
                return handleCommand_lightSetLevel(pController, payload, payloadLen);
        case prot::EPacketType::CMD_RESET_ESP:
            return handleCommand_resetEsp(pController, payload, payloadLen);
        case prot::EPacketType::CMD_CLOUD_SEND_CREDENTIALS:
            return handleCommand_cloudSendCredentials(pController, payload, payloadLen);
        case prot::EPacketType::CMD_OTA_PERFORM:
            return handleCommand_otaPerform(pController, payload, payloadLen);
        default:
            LOG_ERROR("Unknown command packet type: %d (0x%04X). Length %u", packetType, packetType, payloadLen);
            return EPacketHandlingResult::SEND_NACK;
    }
}

#endif  // IS_ESP
