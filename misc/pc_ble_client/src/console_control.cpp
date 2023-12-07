// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "Console";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBU

#include "console_control.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

const char* CERTIFICATE_PRIVATE_KEY_PATH = "device-02.key.pem";
const char* CERTIFICATE_FULL_CHAIN       = "device-02-full-chain.cert.pem";
/**
 * @brief Take appropriate action for a pressed key.
 *
 * THIS FUCNTION IS ONLY FOR DEBUGGING, so one can modify all the parameters for commands freely,
 * as well as add more key handling
 *
 * @param pController Handle to the controller instance
 * @param key Character pressedon the keyboard
 */
void handleKeyPress(BleController* pBleController, char key)
{
    LOG_INFO("Handling pressed key: %c...", key);
    bool result = false;
    switch (key)
    {
        case 't':
        {
            LOG_INFO("About to send command CMD_WIFI_CONNECT_TO_AP...");
            prot::test_command::TCmd cmd = {0};
            sprintf(cmd.message, "hello world!");
            result = pBleController->sendPacket(prot::EPacketType::CMD_TEST, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'm':
        {
            LOG_INFO("About to send command CMD_get_wifi_mac_address");
            prot::get_wifi_mac_address::TCmd cmd = {0};
            result =
                pBleController->sendPacket(prot::EPacketType::CMD_get_wifi_mac_address, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }
        case 'c':
        {
            LOG_INFO("About to send CMD_CLOUD_SEND_CERTIFICATES");

            // send cloud certificates via BLE

            std::ifstream certificatePrivateKeyFile(CERTIFICATE_PRIVATE_KEY_PATH);

            if (!certificatePrivateKeyFile)
            {
                LOG_ERROR("Error while opening file certificate private key");
                break;
            }

            std::stringstream certificatePrivateKeyBuffer;
            certificatePrivateKeyBuffer << certificatePrivateKeyFile.rdbuf();

            std::ifstream certificateFullChainFile(CERTIFICATE_FULL_CHAIN);

            if (!certificateFullChainFile)
            {
                LOG_ERROR("Error while opening file with certificate full chain");
                break;
            }

            std::stringstream certificateFullChainBuffer;

            certificateFullChainBuffer << certificateFullChainFile.rdbuf();

            prot::send_certificates::TCmd cmd = {};

            LOG_INFO("Length of fullChainCertificate: %d", strlen(certificateFullChainBuffer.str().c_str()));
            LOG_INFO("Length of privateKey: %d", strlen(certificatePrivateKeyBuffer.str().c_str()));

            cmd.certificates.setFullChainCertificate(certificateFullChainBuffer.str());
            cmd.certificates.setPrivateKey(certificatePrivateKeyBuffer.str());

            result =
                pBleController->sendPacket(prot::EPacketType::CMD_SEND_CERTIFICATES, (uint8_t*)(&cmd), sizeof(cmd));
            break;
        }

        case 'h':
        {
            // Print available commands
            LOG_INFO("Available commands:\n\n"
                     "t - send test command\n"
                     "m - get WIFI MAC Address from device\n"
                     "c - send certificates via BLE"
                     "h - display help\n");

            result = true;
            break;
        }

        default:
        {
            result = true;
            LOG_ERROR("Unknown key command: %c", key);
            break;
        }
    }

    if (!result)
        LOG_ERROR("Failed to send a command after key pressed");
}
