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
        bool TCloudCredentials::isSetCloudAddress() const
        {
            return cloudAddress[0] != 0;
        }
        bool TCloudCredentials::isSetCloudDeviceId() const
        {
            return cloudDeviceId[0] != 0;
        }

        bool TCloudCertificatePack::operator==(const TCloudCertificatePack &rhs) const
        {
            if (::strcmp(serverPublicCertificate, rhs.serverPublicCertificate) != 0)
            {
                return false;
            }

            if (::strcmp(clientPublicCertificate, rhs.clientPublicCertificate) != 0)
            {
                return false;
            }

            if (::strcmp(clientPrivateKey, rhs.clientPrivateKey) != 0)
            {
                return false;
            }

            return true;
        }

        bool TCloudCertificatePack::setServerPublicCertificate(const std::string &newServerPublicCertificate)
        {
            if (newServerPublicCertificate.size() > CLOUD_MAX_CERT_LENGTH)
            {
                LOG_ERROR("Server Public Certificate too long");
                return false;
            }

            ::bzero(serverPublicCertificate, sizeof(serverPublicCertificate));
            ::strcpy(serverPublicCertificate, newServerPublicCertificate.c_str());

            return true;
        }

        bool TCloudCertificatePack::isSetServerPublicCertificate() const
        {
            return serverPublicCertificate[0] != 0;
        }

        bool TCloudCertificatePack::setClientPublicCertificate(const std::string &newClientPublicCertificate)
        {
            if (newClientPublicCertificate.size() > CLOUD_MAX_CERT_LENGTH)
            {
                LOG_ERROR("Client Public Certificate too long");
                return false;
            }

            ::bzero(clientPublicCertificate, sizeof(clientPublicCertificate));
            ::strcpy(clientPublicCertificate, newClientPublicCertificate.c_str());

            return true;
        }

        bool TCloudCertificatePack::isSetClientPublicCertificate() const
        {
            return clientPublicCertificate[0] != 0;
        }

        bool TCloudCertificatePack::setClientPrivateKey(const std::string &newClientPrivateKey)
        {
            if (newClientPrivateKey.size() > CLOUD_MAX_PRIVATE_KEY_LENGTH)
            {
                LOG_ERROR("Client Private Key too long");
                return false;
            }

            ::bzero(clientPrivateKey, sizeof(clientPrivateKey));
            ::strcpy(clientPrivateKey, newClientPrivateKey.c_str());

            return true;
        }

        bool TCloudCertificatePack::isSetClientPrivateKey() const
        {
            return clientPrivateKey[0] != 0;
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
} // namespace prot
