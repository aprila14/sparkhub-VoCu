#ifndef CONFIGNVS_H
#define CONFIGNVS_H

#include "custom_types.h"
#include "defines.h"
#include "mutex.h"

#include "nvs_flash.h"

namespace ConfigKeyName
{

    // make sure this names are unique
    constexpr const char *CONFIGURATION_FINISHED_STATE = "conf";
    constexpr const char *WIFI_CREDENTIALS = "wifi-cr";
    constexpr const char *CLOUD_CERTIFICATES = "cloud-cert-aws";
    constexpr const char *CLOUD_CREDENTIALS = "cloud-cr-aws";
    constexpr const char *OTA_HTTP_CERTIFICATE = "ota-cert";
    constexpr const char *OTA_UPDATE_LINK = "ota-link";
    constexpr const char *LAST_LIGHT_BRIGHTNESS = "lastbr";

}

class ConfigNvs
{
public:
    ConfigNvs();

    /**
     * @brief Initialize config and read parameters from NVS
     */
    virtual bool init();

    /**
     * @brief Reset all configuration fields (also in the NVS)
     */
    virtual bool resetConfig();

    /**
     * @brief Get whether initial configuration (via BLE) has been finished
     * @return
     */
    virtual bool getConfigurationFinishedState();
    /**
     * @brief Set whether initial configuration (via BLE) has been finished
     * @param isFinished
     */
    virtual void setConfigurationFinishedState(bool isFinished);

    virtual const TWiFiCredentials &getWifiCredentials();
    virtual void setWifiCredentials(const TWiFiCredentials &wifiCredentials);

    virtual const TCloudCredentials &getCloudCredentials();
    virtual void setCloudCredentials(const TCloudCredentials &cloudCredentials);

    virtual const TCloudCertificatePack &getCloudCertificates();
    virtual void setCloudCertificates(const TCloudCertificatePack &cloudCertificates);

    virtual const THttpsServerCertificate &getOtaCertificate();
    virtual void setOtaCertificate(const THttpsServerCertificate &httpsServerCertificate);

    virtual const TOtaUpdateLink &getOtaUpdateLink();
    virtual void setOtaUpdateLink(const TOtaUpdateLink &otaUpdateLink);

    virtual void setLastLightBrightness(uint8_t value);
    virtual uint8_t getLastLightBrightness();

#if !TESTING
private:
#endif
    // NOTE - we deliberately do not propagate errors from reading/writing from config at this point
    void getUint8Var(uint8_t &variable, const char *key) const;
    void setUint8Var(uint8_t &variable, const uint8_t newValue, const char *key) const;
    void getUint16Var(uint16_t &variable, const char *key) const;
    void setUint16Var(uint16_t &variable, const uint16_t newValue, const char *key) const;
    void getUint32Var(uint32_t &variable, const char *key) const;
    void setUint32Var(uint32_t &variable, const uint32_t newValue, const char *key) const;
    void getBoolVar(bool &variable, const char *key) const;
    void setBoolVar(bool &variable, const bool newValue, const char *key) const;
    void getInt32Var(int32_t &variable, const char *key) const;
    void setInt32Var(int32_t &variable, const int32_t newValue, const char *key) const;

    template <typename T>
    void setStruct(T &variable, const T &newValue, const char *key);
    template <typename T>
    void getStruct(T &variable, const char *key);

    bool openNvs();
    void commitToNVS() const;
    void resetAllConfigurationFields();

    SemaphoreHandle_t m_accessMutex = nullptr; ///< Protects access to all the NVS parameters
    nvs_handle_t m_nvsHandle = 0;

    // just assign whatever value below - the config is reset at startup to assign the default value
    bool m_configurationFinishedState = false;
    TWiFiCredentials m_wiFiCredentials = {};
    TCloudCredentials m_cloudCredentials = {};
    TCloudCertificatePack m_cloudCertificates = {};
    THttpsServerCertificate m_otaHttpsServerCertificate = {};
    TOtaUpdateLink m_otaUpdateLink = {};

    uint8_t m_lastLightBrightness = 0;
};

// We deliberately have a globa pointer,
// in order to use the config as a singleton, to not pass it to all the modules all the time.
// We do not want just a global variable, to enable unit tests with mocking.
// We will create and assign the real object in main or during unit tests.
extern ConfigNvs *pConfig;

#endif // CONFIGNVS_H
