#ifndef CONFIGNVS_H
#define CONFIGNVS_H

#include "custom_types.h"
#include "defines.h"
#include "mutex.h"

#include "nvs_flash.h"

namespace ConfigKeyName
{

// make sure this names are unique
constexpr const char* BLE_CONFIGURATION_STATUS     = "ble-conf";
constexpr const char* WIFI_CREDENTIALS             = "wifi-cr";
constexpr const char* CLOUD_CERTIFICATES           = "cloud-cert";
constexpr const char* CLOUD_CREDENTIALS            = "cloud-cr";
constexpr const char* OTA_UPDATE_LINK              = "ota-link";
constexpr const char* CERTIFICATES                 = "cert-pack";
constexpr const char* DEVICE_PROVISIONING_STATUS   = "dev-prov";
constexpr const char* UPDATE_WORKFLOW_DATA_NVS_KEY = "work-id";

} // namespace ConfigKeyName

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
    virtual EBleConfigurationStatus getBleConfigurationStatus();
    /**
     * @brief Set whether initial configuration (via BLE) has been finished
     * @param isFinished
     */
    virtual void setBleConfigurationStatus(EBleConfigurationStatus bleStatus);

    /**
     * @brief Get whether device provisioning has been finished
     * @return
     */
    virtual ECloudDeviceProvisioningStatus getDeviceProvisioningStatus();
    /**
     * @brief Set whether device provisioning has been finished
     * @return
     */
    virtual void setDeviceProvisioningStatus(ECloudDeviceProvisioningStatus deviceProvisioningStatus);

    virtual const TWiFiCredentials& getWifiCredentials();
    virtual void                    setWifiCredentials(const TWiFiCredentials& wifiCredentials);

    virtual const TCloudCredentials& getCloudCredentials();
    virtual void                     setCloudCredentials(const TCloudCredentials& cloudCredentials);

    virtual const TOtaUpdateLink& getOtaUpdateLink();
    virtual void                  setOtaUpdateLink(const TOtaUpdateLink& otaUpdateLink);

    virtual const TCertificatePack& getCertificatePack();
    virtual void                    setCertificatePack(const TCertificatePack& certificatePack);

    virtual const TWorkflowData& getWorkflowData();
    virtual void                 setWorkflowData(const TWorkflowData& workflowData);

#if !TESTING
private:
#endif
    // NOTE - we deliberately do not propagate errors from reading/writing from config at this point
    void getUint8Var(uint8_t& variable, const char* key) const;
    void setUint8Var(uint8_t& variable, const uint8_t newValue, const char* key) const;
    void getUint16Var(uint16_t& variable, const char* key) const;
    void setUint16Var(uint16_t& variable, const uint16_t newValue, const char* key) const;
    void getUint32Var(uint32_t& variable, const char* key) const;
    void setUint32Var(uint32_t& variable, const uint32_t newValue, const char* key) const;
    void getBoolVar(bool& variable, const char* key) const;
    void setBoolVar(bool& variable, const bool newValue, const char* key) const;
    void getInt32Var(int32_t& variable, const char* key) const;
    void setInt32Var(int32_t& variable, const int32_t newValue, const char* key) const;

    template <typename T> void setStruct(T& variable, const T& newValue, const char* key);
    template <typename T> void getStruct(T& variable, const char* key);

    bool openNvs();
    void commitToNVS() const;
    void resetAllConfigurationFields();

    SemaphoreHandle_t m_accessMutex = nullptr; ///< Protects access to all the NVS parameters
    nvs_handle_t      m_nvsHandle   = 0;

    // just assign whatever value below - the config is reset at startup to assign the default value
    EBleConfigurationStatus m_bleConfigurationStatus = EBleConfigurationStatus::BLE_CONFIGURATION_STATUS_INIT;
    TWiFiCredentials        m_wiFiCredentials        = {};
    TCloudCredentials       m_cloudCredentials       = {};
    TOtaUpdateLink          m_otaUpdateLink          = {};

    TCertificatePack               m_certificatePack = {};
    ECloudDeviceProvisioningStatus m_deviceProvisioningStatus =
        ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT;
    TWorkflowData m_workflowData = {};
};

// We deliberately have a globa pointer,
// in order to use the config as a singleton, to not pass it to all the modules all the time.
// We do not want just a global variable, to enable unit tests with mocking.
// We will create and assign the real object in main or during unit tests.
extern ConfigNvs* pConfig;

#endif // CONFIGNVS_H
