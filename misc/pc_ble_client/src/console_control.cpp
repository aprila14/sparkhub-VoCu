// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "Console";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBU


#include "console_control.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

const char* DEFAULT_SSID_1 = ""; //fill with appropiate SSID
const char* DEFAULT_PASSWORD_1 = ""; //fill with appropiate Password

const char* DEFAULT_SSID_2 = ""; //fill with appropiate SSID
const char* DEFAULT_PASSWORD_2 = ""; //fill with appropiate Password

const char* AWS_CA_CERTIFICATE = "AmazonRootCA1.pem";
const char* AWS_CLIENT_CERTIFICATE = "b4fb5e526d54cbdf340d03c76891c932eb238108d21a70a640dfa548f4ae7d5e-certificate.pem.crt";
const char* AWS_CLIENT_PRIVATE_KEY = "b4fb5e526d54cbdf340d03c76891c932eb238108d21a70a640dfa548f4ae7d5e-private.pem.key";
const char* AWS_ENDPOINT = "mqtts://a2mdrd3iem5np2-ats.iot.eu-central-1.amazonaws.com";

const char* AWS_CLIENT_CERTIFICATE_2 = "608867088452aa73f4380acd66f300b6274ff55a68219a4c67cc2735abbb9e3b-certificate.pem.crt";
const char* AWS_CLIENT_PRIVATE_KEY_2 = "608867088452aa73f4380acd66f300b6274ff55a68219a4c67cc2735abbb9e3b-private.pem.key";

const char* AWS_CLIENT_UUID = "0123-4567-89AB-CDEF";

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

        case 'v':
        {
            LOG_INFO("About to send command CMD_WIFI_CONNECT_TO_AP...");
            prot::wifi_connect_to_ap::TCmd cmd = {};
            cmd.timeoutMs = 5000;
            cmd.credentials.setSsid("notexistingssid");
            cmd.credentials.setPassword("totallywrongpassword");
            cmd.credentials.authorizationMode = i2a_wifi_auth_mode_t::I2A_WIFI_AUTH_NOT_SET;

            result = pBleController->sendPacket(prot::EPacketType::CMD_WIFI_CONNECT_TO_AP, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'e':
        {
            LOG_INFO("About to send command CMD_WIFI_CONNECT_TO_AP...");
            prot::wifi_connect_to_ap::TCmd cmd = {};
            cmd.timeoutMs = 10000;
            cmd.credentials.setSsid(DEFAULT_SSID_1);
            cmd.credentials.setPassword(DEFAULT_PASSWORD_1);
            cmd.credentials.authorizationMode = i2a_wifi_auth_mode_t::I2A_WIFI_AUTH_WPA2_PSK;

            result = pBleController->sendPacket(prot::EPacketType::CMD_WIFI_CONNECT_TO_AP, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'r':
        {
            LOG_INFO("About to send command CMD_WIFI_DISCONNECT_FROM_AP...");
            prot::wifi_disconnect_from_ap::TCmd cmd = {};
            cmd.dummyByte = 0xCD;

            result = pBleController->sendPacket(prot::EPacketType::CMD_WIFI_DISCONNECT_FROM_AP, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 's':
        {
            LOG_INFO("About to send command CMD_WIFI_SCAN_AP...");
            prot::wifi_scan_ap::TCmd cmd = {};

            result = pBleController->sendPacket(prot::EPacketType::CMD_WIFI_SCAN_AP, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'n':
        {
            LOG_INFO("About to send command CMD_WIFI_SAVE_AP_CREDENTIALS");
            prot::wifi_save_ap_credentials::TCmd cmd = {};

            cmd.credentials.setSsid(DEFAULT_SSID_1);
            cmd.credentials.setPassword(DEFAULT_PASSWORD_1);

            result = pBleController->sendPacket(prot::EPacketType::CMD_WIFI_SAVE_AP_CREDENTIALS, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'i':
        {
            LOG_INFO("About to send command CMD_GLOBAL_GET_STATUS...");
            prot::global_get_status::TCmd cmd = {};

            result = pBleController->sendPacket(prot::EPacketType::CMD_GLOBAL_GET_STATUS, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

        case 'o':
        {
            LOG_INFO("About to add command CMD_RESET_ESP");
            prot::reset_esp::TCmd cmd = {};

            result = pBleController->sendPacket(prot::EPacketType::CMD_RESET_ESP, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }

#if BUILD_WITH_THINGSBOARD
        case 'p':
        {
            LOG_INFO("About to send command CMD_CLOUD_SEND_CREDENTIALS");

            std::ifstream serverPublicCertFile("office_server.pem");

            if (!serverPublicCertFile)
            {
                LOG_ERROR("Error while opening file with server public certificate");
                break;
            }

            std::stringstream serverPublicCertBuffer;
            serverPublicCertBuffer << serverPublicCertFile.rdbuf();

            std::ifstream clientPublicCertFile("client_public_cert.pem");

            if (!clientPublicCertFile)
            {
                LOG_ERROR("Error while opening file with client public certificate");
                break;
            }

            std::stringstream clientPublicCertBuffer;
            clientPublicCertBuffer << clientPublicCertFile.rdbuf();

            std::ifstream clientPrivateKeyFile("client_private_key.pem");

            if (!clientPrivateKeyFile)
            {
                LOG_ERROR("Error while opening file with client private key");
                break;
            }

            std::stringstream clientPrivateKeyBuffer;
            clientPrivateKeyBuffer << clientPrivateKeyFile.rdbuf();

            prot::cloud_set_credentials::TCmd cmd = {};

            cmd.cloudCredentials.setCloudAddress("mqtts://10.0.0.155");
            cmd.cloudCertificates.setServerPublicCertificate(serverPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPublicCertificate(clientPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPrivateKey(clientPrivateKeyBuffer.str());

            result = pBleController->sendPacket(prot::EPacketType::CMD_CLOUD_SEND_CREDENTIALS, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }
#endif  // BUILD_WITH_THINGSBOARD
        case 'a':
        {
            LOG_INFO("About to send command CMD_OTA_PERFORM");

            std::ifstream file("cert.pem");

            if (!file)
            {
                LOG_ERROR("Error while opening file with HTTP cert");
                break;
            }
            std::stringstream buffer;
            buffer << file.rdbuf();

            std:: cout << "File content: " << std::endl << std::endl;

            std::cout << buffer.str();

            prot::ota_perform::TCmd cmd = {};

            strcpy(cmd.otaUpdateLink.firmwareLink, "https://10.0.0.155:8081/i2a-esp32.bin");
            strcpy(cmd.httpsServerCertificate.serverCertificate, buffer.str().c_str());

            result = pBleController->sendPacket(prot::EPacketType::CMD_OTA_PERFORM, (uint8_t*)&cmd, sizeof(cmd));
            break;
        }
#if !BUILD_WITH_THINGSBOARD
        case 'f':
        {
            LOG_INFO("About to connect to send CMD_CLOUD_SEND_CREDENTIALS");

            // Connecting to AWS cloud

            std::ifstream serverPublicCertFile(AWS_CA_CERTIFICATE);

            if (!serverPublicCertFile)
            {
                LOG_ERROR("Error while opening file with server public certificate");
                break;
            }

            std::stringstream serverPublicCertBuffer;
            serverPublicCertBuffer << serverPublicCertFile.rdbuf();

            std::ifstream clientPublicCertFile(AWS_CLIENT_CERTIFICATE);

            if (!clientPublicCertFile)
            {
                LOG_ERROR("Error while opening file with client public certificate");
                break;
            }

            std::stringstream clientPublicCertBuffer;
            clientPublicCertBuffer << clientPublicCertFile.rdbuf();

            std::ifstream clientPrivateKeyFile(AWS_CLIENT_PRIVATE_KEY);

            if (!clientPrivateKeyFile)
            {
                LOG_ERROR("Error while opening file with client private key");
                break;
            }

            std::stringstream clientPrivateKeyBuffer;
            clientPrivateKeyBuffer << clientPrivateKeyFile.rdbuf();

            prot::cloud_set_credentials::TCmd cmd = {};

            cmd.cloudCredentials.setCloudAddress(AWS_ENDPOINT);
            cmd.cloudCertificates.setServerPublicCertificate(serverPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPublicCertificate(clientPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPrivateKey(clientPrivateKeyBuffer.str());
            cmd.cloudCertificates.setClientUuid(AWS_CLIENT_UUID);

            result = pBleController->sendPacket(prot::EPacketType::CMD_CLOUD_SEND_CREDENTIALS, (uint8_t*)&cmd, sizeof(cmd));
            break;

        }

        case 'g':
        {
            LOG_INFO("About to connect to send CMD_CLOUD_SEND_CREDENTIALS");

            // Connecting to AWS cloud

            std::ifstream serverPublicCertFile(AWS_CA_CERTIFICATE);

            if (!serverPublicCertFile)
            {
                LOG_ERROR("Error while opening file with server public certificate");
                break;
            }

            std::stringstream serverPublicCertBuffer;
            serverPublicCertBuffer << serverPublicCertFile.rdbuf();

            std::ifstream clientPublicCertFile(AWS_CLIENT_CERTIFICATE_2);

            if (!clientPublicCertFile)
            {
                LOG_ERROR("Error while opening file with client public certificate");
                break;
            }

            std::stringstream clientPublicCertBuffer;
            clientPublicCertBuffer << clientPublicCertFile.rdbuf();

            std::ifstream clientPrivateKeyFile(AWS_CLIENT_PRIVATE_KEY_2);

            if (!clientPrivateKeyFile)
            {
                LOG_ERROR("Error while opening file with client private key");
                break;
            }

            std::stringstream clientPrivateKeyBuffer;
            clientPrivateKeyBuffer << clientPrivateKeyFile.rdbuf();

            prot::cloud_set_credentials::TCmd cmd = {};

            cmd.cloudCredentials.setCloudAddress(AWS_ENDPOINT);
            cmd.cloudCertificates.setServerPublicCertificate(serverPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPublicCertificate(clientPublicCertBuffer.str());
            cmd.cloudCertificates.setClientPrivateKey(clientPrivateKeyBuffer.str());
            cmd.cloudCertificates.setClientUuid(AWS_CLIENT_UUID);

            result = pBleController->sendPacket(prot::EPacketType::CMD_CLOUD_SEND_CREDENTIALS, (uint8_t*)&cmd, sizeof(cmd));
            break;

        }
#endif //!BUILD_WITH_THINGSBOARD

        case 'h':
        {
            //Print available commands
            LOG_INFO("Available commands:\n\n"
                     "t - send test command\n"
                     "v - connect to nonexistingssid\n"
                     "e - connect to DEFAULT_SSID_1\n"
                     "r - disconnect from Access Point\n"
                     "s - scan available Access Points\n"
                     "n - save AP credentials\n"
                     "i - get global status information\n"
                     "o - reset ESP32\n"
                     "p - send cloud credentials to ESP32, connect to Thingsboard cloud\n"
                     "a - simple OTA\n"
                     "f - connect to AWS cloud"
                     "0 - 9 - set Light Intesity Level (from 0%% to 90%%)\n"
                     "h - display help\n");

            result = true;
            break;
        }

        case '0'...'9':
        {
            //send command to change light level
            prot::light_set_level::TCmd cmd = {};

            cmd.lightPercentageLevel = (key - '0') * 10;

            result = pBleController->sendPacket(prot::EPacketType::CMD_LIGHT_SET_LEVEL, (uint8_t*)&cmd, sizeof(cmd));
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
