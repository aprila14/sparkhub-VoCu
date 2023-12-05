// Please keep these 2 lines at the beginning of each cpp module
static const char *LOG_TAG = "ProtocolTypes";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "protocol_types.h"

#include <cstring>

namespace prot
{

    namespace wifi_connect_to_ap
    {

        TWiFiCredentials::TWiFiCredentials()
            : ssid{},
              password{},
              authorizationMode{}
        {
        }

        bool TWiFiCredentials::operator==(const TWiFiCredentials &rhs) const
        {
            if (::strcmp(ssid, rhs.ssid) != 0)
                return false;

            if (::strcmp(password, rhs.password) != 0)
                return false;

            if (authorizationMode != rhs.authorizationMode)
                return false;

            return true;
        }

        bool TWiFiCredentials::setSsid(const std::string &newSsid)
        {
            if (newSsid.size() > MAX_WIFI_SSID_LENGTH)
                return false;

            ::bzero(ssid, sizeof(ssid));
            ::strcpy(ssid, newSsid.c_str());

            return true;
        }

        bool TWiFiCredentials::setPassword(const std::string &newPassword)
        {
            if (newPassword.size() > MAX_WIFI_PASSWORD_LENGTH)
                return false;

            ::bzero(password, sizeof(password));
            ::strcpy(password, newPassword.c_str());

            return true;
        }

        bool TWiFiCredentials::isSet() const
        {
            return ssid[0] != 0; // if SSID is set, then the credentials are valid
        }

    }

    // namespace wifi_connect_to_ap

    namespace cloud_set_credentials
    {

        std::string getCloudConnectionStatus(enum ECloudConnectionStatus cloudConnectionStatus)
        {
            switch (cloudConnectionStatus)
            {
            case ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED:
                return "NOT CONFIGURED";
            case ECloudConnectionStatus::CLOUD_STATUS_DISABLED:
                return "DISABLED";
            case ECloudConnectionStatus::CLOUD_STATUS_CONNECTED:
                return "CONNECTED";
            case ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED:
                return "NOT CONNECTED";
            default:
                return "NOT RECOGNIZED";
            }
        }

        TCloudCredentials::TCloudCredentials()
            : cloudAddress{}
        {
        }

        bool TCloudCredentials::operator==(const TCloudCredentials &rhs) const
        {
            if (::strcmp(cloudAddress, rhs.cloudAddress) != 0)
            {
                return false;
            }

            if (::strcmp(cloudDeviceId, rhs.cloudDeviceId) != 0)
            {
                return false;
            }

            if (::strcmp(cloudMqttUsername, rhs.cloudMqttUsername) != 0)
            {
                return false;
            }

            return true;
        }

        bool TCloudCredentials::setCloudDeviceId(const std::string &newDeviceId)
        {
            if (newDeviceId.size() > CLOUD_DEVICE_ID_LENGTH)
            {
                LOG_ERROR("New device id too long");
                return false;
            }

            ::bzero(cloudDeviceId, sizeof(cloudDeviceId));
            ::strcpy(cloudDeviceId, newDeviceId.c_str());

            return true;
        }

        bool TCloudCredentials::setCloudAddress(const std::string &newAddress)
        {
            if (newAddress.size() > CLOUD_MAX_ADDRESS_LENGTH)
            {
                LOG_ERROR("Cloud Address too long");
                return false;
            }

            ::bzero(cloudAddress, sizeof(cloudAddress));
            ::strcpy(cloudAddress, newAddress.c_str());

            return true;
        }

        bool TCloudCredentials::setCloudMqttUsername(const std::string &newMqttUsername)
        {
            if (newMqttUsername.size() > CLOUD_MQTT_USERNAME_LENGTH)
            {
                LOG_ERROR("Mqtt username too long");
                return false;
            }

            ::bzero(cloudMqttUsername, sizeof(cloudMqttUsername));
            ::strcpy(cloudMqttUsername, newMqttUsername.c_str());

            return true;
        }

        bool TCloudCredentials::isSetCloudAddress() const
        {
            return cloudAddress[0] != 0;
        }
        bool TCloudCredentials::isSetCloudDeviceId() const
        {
            return cloudDeviceId[0] != 0;
        }
        bool TCloudCredentials::isSetCloudMqttUsername() const
        {
            return cloudMqttUsername[0] != 0;
        }
    } // namespace cloud_set_credentials

    namespace ota_perform
    {

        bool THttpsServerCertificate::operator==(const THttpsServerCertificate &rhs) const
        {
            if (::strcmp(serverCertificate, rhs.serverCertificate) != 0)
            {
                return false;
            }

            return true;
        }

        bool THttpsServerCertificate::isSet() const
        {
            return serverCertificate[0] != 0;
        }

        bool TOtaUpdateLink::operator==(const TOtaUpdateLink &rhs) const
        {
            if (::strcmp(firmwareLink, rhs.firmwareLink) != 0)
            {
                return false;
            }

            return true;
        }

        bool TOtaUpdateLink::isSet() const
        {
            return firmwareLink[0] != 0;
        }

    } // namespace ota_perform

    namespace send_certificates
    {
        bool TCertificatePack::operator==(const TCertificatePack &rhs) const
        {
            if (::strcmp(fullChainCertificate, rhs.fullChainCertificate) != 0)
            {
                return false;
            }

            if (::strcmp(privateKey, rhs.privateKey) != 0)
            {
                return false;
            }

            return true;
        }

        bool TCertificatePack::setFullChainCertificate(const std::string &newFullChainCertificate)
        {
            if (newFullChainCertificate.size() > MAX_FULLCHAIN_CERTIFICATE_LENGTH)
            {
                LOG_ERROR("Fullchain certificate too long, length: %d, max length: %d", newFullChainCertificate.size(), MAX_FULLCHAIN_CERTIFICATE_LENGTH);
                return false;
            }

            ::bzero(fullChainCertificate, sizeof(fullChainCertificate));
            ::strcpy(fullChainCertificate, newFullChainCertificate.c_str());

            return true;
        }

        bool TCertificatePack::setPrivateKey(const std::string &newPrivateKey)
        {
            if (newPrivateKey.size() > MAX_PRIVATE_KEY_LENGTH)
            {
                LOG_ERROR("Private key too long");
                return false;
            }

            ::bzero(privateKey, sizeof(privateKey));
            ::strcpy(privateKey, newPrivateKey.c_str());

            return true;
        }

        bool TCertificatePack::isSetFullChainCertificate() const
        {
            return fullChainCertificate[0] != 0;
        }

        bool TCertificatePack::isSetPrivateKey() const
        {
            return privateKey[0] != 0;
        }
    }
} // namespace prot
