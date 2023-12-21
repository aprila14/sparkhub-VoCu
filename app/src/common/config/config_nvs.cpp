// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "ConfigNvs";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include "config_nvs.h"

ConfigNvs* pConfig = nullptr; // see comment in header file

ConfigNvs::ConfigNvs()
{
    resetAllConfigurationFields();
}

bool ConfigNvs::init()
{
    if (!openNvs())
        return false;

    m_accessMutex = mutexCreate();
    assert(m_accessMutex);

    // read the initial value for each configuration parameter here
    getUint8Var(reinterpret_cast<uint8_t&>(m_bleConfigurationStatus), ConfigKeyName::BLE_CONFIGURATION_STATUS);
    getUint8Var(reinterpret_cast<uint8_t&>(m_deviceProvisioningStatus), ConfigKeyName::DEVICE_PROVISIONING_STATUS);
    getStruct(m_wiFiCredentials, ConfigKeyName::WIFI_CREDENTIALS);
    getStruct(m_cloudCredentials, ConfigKeyName::CLOUD_CREDENTIALS);
    getStruct(m_certificatePack, ConfigKeyName::CERTIFICATES);

    return true;
}

bool ConfigNvs::openNvs()
{
    LOG_INFO("Opening Non-Volatile Storage (NVS) handle... ");

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &(this->m_nvsHandle));
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }

    return true;
}

void ConfigNvs::commitToNVS() const
{
    esp_err_t err = nvs_commit(this->m_nvsHandle);
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) commiting to NVS!", esp_err_to_name(err));
        return;
    }
}

EBleConfigurationStatus ConfigNvs::getBleConfigurationStatus()
{
    return m_bleConfigurationStatus;
}

void ConfigNvs::setBleConfigurationStatus(EBleConfigurationStatus bleStatus)
{
    setUint8Var(
        reinterpret_cast<uint8_t&>(m_bleConfigurationStatus),
        reinterpret_cast<uint8_t&>(bleStatus),
        ConfigKeyName::BLE_CONFIGURATION_STATUS);
}

ECloudDeviceProvisioningStatus ConfigNvs::getDeviceProvisioningStatus()
{
    return m_deviceProvisioningStatus;
}

void ConfigNvs::setDeviceProvisioningStatus(ECloudDeviceProvisioningStatus deviceProvisioningStatus)
{
    setUint8Var(
        reinterpret_cast<uint8_t&>(m_deviceProvisioningStatus),
        reinterpret_cast<uint8_t&>(deviceProvisioningStatus),
        ConfigKeyName::DEVICE_PROVISIONING_STATUS);
}

const TWiFiCredentials& ConfigNvs::getWifiCredentials()
{
    return m_wiFiCredentials;
}

void ConfigNvs::setWifiCredentials(const TWiFiCredentials& wifiCredentials)
{
    setStruct(m_wiFiCredentials, wifiCredentials, ConfigKeyName::WIFI_CREDENTIALS);
}

const TCloudCredentials& ConfigNvs::getCloudCredentials()
{
    return m_cloudCredentials;
}

void ConfigNvs::setCloudCredentials(const TCloudCredentials& cloudCredentials)
{
    setStruct(m_cloudCredentials, cloudCredentials, ConfigKeyName::CLOUD_CREDENTIALS);
}

void ConfigNvs::setOtaUpdateLink(const TOtaUpdateLink& otaUpdateLink)
{
    setStruct(m_otaUpdateLink, otaUpdateLink, ConfigKeyName::OTA_UPDATE_LINK);
}

const TOtaUpdateLink& ConfigNvs::getOtaUpdateLink()
{
    return m_otaUpdateLink;
}

void ConfigNvs::setCertificatePack(const TCertificatePack& certificatePack)
{
    setStruct(m_certificatePack, certificatePack, ConfigKeyName::CERTIFICATES);
}

const TCertificatePack& ConfigNvs::getCertificatePack()
{
    return m_certificatePack;
}

void ConfigNvs::setWorkflowData(const TWorkflowData& workflowData)
{
    setStruct(m_workflowData, workflowData, ConfigKeyName::UPDATE_WORKFLOW_DATA_NVS_KEY);
}
const TWorkflowData& ConfigNvs::getWorkflowData()
{
    return m_workflowData;
}

bool ConfigNvs::resetConfig()
{
    LOCK_GUARD(this->m_accessMutex, accessMutexGuard);
    resetAllConfigurationFields();
    if (nvs_erase_all(m_nvsHandle) != ESP_OK)
    {
        LOG_ERROR("Failed to nvs_erase_all!");
        return false;
    }

    return true;
}

void ConfigNvs::getUint8Var(uint8_t& variable, const char* key) const
{
    esp_err_t err = nvs_get_u8(this->m_nvsHandle, key, &variable);
    switch (err)
    {
        case ESP_OK:
            LOG_DEBUG("Value read from config: %d (key '%s')", variable, key);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            LOG_DEBUG("Failed to get key: '%s' - not initialized yet", key);
            break;
        default:
            LOG_ERROR("Error (%s) reading! key '%s'", esp_err_to_name(err), key);
            break;
    }
}

void ConfigNvs::setUint8Var(uint8_t& variable, const uint8_t newValue, const char* key) const
{
    if (variable == newValue)
    {
        LOG_DEBUG("Same value - not overwritng");
        return;
    }

    LOCK_GUARD(this->m_accessMutex, accessMutexGuard);

    variable = newValue;

    esp_err_t err = nvs_set_u8(this->m_nvsHandle, key, variable);
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) writing to NVS (key '%s')!", esp_err_to_name(err), key);
        return;
    }

    commitToNVS();
    LOG_INFO("key '%s' - written %d to config", key, variable);
}

void ConfigNvs::getUint16Var(uint16_t& variable, const char* key) const
{
    esp_err_t err = nvs_get_u16(this->m_nvsHandle, key, &variable);
    switch (err)
    {
        case ESP_OK:
            LOG_DEBUG("Value read from config: %d (key '%s')", variable, key);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            LOG_DEBUG("Failed to get key: '%s' - not initialized yet", key);
            break;
        default:
            LOG_ERROR("Error (%s) reading! key '%s'", esp_err_to_name(err), key);
            break;
    }
}

void ConfigNvs::setUint16Var(uint16_t& variable, const uint16_t newValue, const char* key) const
{
    if (variable == newValue)
    {
        LOG_DEBUG("Same value - not overwritng");
        return;
    }

    LOCK_GUARD(this->m_accessMutex, accessMutexGuard);

    variable = newValue;

    esp_err_t err = nvs_set_u16(this->m_nvsHandle, key, variable);
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) writing to NVS (key '%s')!", esp_err_to_name(err), key);
        return;
    }

    commitToNVS();
    LOG_INFO("key '%s' - written %d to config", key, variable);
}

void ConfigNvs::getUint32Var(uint32_t& variable, const char* key) const
{
    esp_err_t err = nvs_get_u32(this->m_nvsHandle, key, &variable);
    switch (err)
    {
        case ESP_OK:
            LOG_DEBUG("Value read from config: %d (key '%s')", variable, key);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            LOG_DEBUG("Failed to get key: '%s' - not initialized yet", key);
            break;
        default:
            LOG_ERROR("Error (%s) reading! key '%s'", esp_err_to_name(err), key);
            break;
    }
}

void ConfigNvs::setUint32Var(uint32_t& variable, const uint32_t newValue, const char* key) const
{
    if (variable == newValue)
    {
        LOG_DEBUG("Same value - not overwritng");
        return;
    }

    LOCK_GUARD(this->m_accessMutex, accessMutexGuard);

    variable = newValue;

    esp_err_t err = nvs_set_u32(this->m_nvsHandle, key, variable);
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) writing to NVS (key '%s')!", esp_err_to_name(err), key);
        return;
    }

    commitToNVS();
    LOG_INFO("key '%s' - written %d to config", key, variable);
}

void ConfigNvs::getBoolVar(bool& variable, const char* key) const
{
    getUint8Var(*reinterpret_cast<uint8_t*>(&variable), key); // NOLINT
}

void ConfigNvs::setBoolVar(bool& variable, const bool newValue, const char* key) const
{
    setUint8Var(*reinterpret_cast<uint8_t*>(&variable), newValue, key); // NOLINT
}

void ConfigNvs::getInt32Var(int32_t& variable, const char* key) const
{
    getUint32Var(*reinterpret_cast<uint32_t*>(&variable), key); // NOLINT
}

void ConfigNvs::setInt32Var(int32_t& variable, const int32_t newValue, const char* key) const
{
    setUint32Var(*reinterpret_cast<uint32_t*>(&variable), static_cast<uint32_t>(newValue), key); // NOLINT
}

void ConfigNvs::resetAllConfigurationFields()
{
    // default values for the configuration:

    m_bleConfigurationStatus   = EBleConfigurationStatus::BLE_CONFIGURATION_STATUS_INIT;
    m_wiFiCredentials          = TWiFiCredentials();
    m_cloudCredentials         = TCloudCredentials();
    m_otaUpdateLink            = TOtaUpdateLink();
    m_certificatePack          = TCertificatePack();
    m_deviceProvisioningStatus = ECloudDeviceProvisioningStatus::PROVISIONING_STATUS_INIT;
}

template <typename T> void ConfigNvs::setStruct(T& variable, const T& newValue, const char* key)
{
    if (variable == newValue)
    {
        LOG_DEBUG("Same value - not overwritng");
        return;
    }

    LOCK_GUARD(this->m_accessMutex, accessMutexGuard);

    variable = newValue;

    esp_err_t err = nvs_set_blob(this->m_nvsHandle, key, &variable, sizeof(variable));
    if (err != ESP_OK)
    {
        LOG_ERROR("Error (%s) writing to NVS (key '%s')!", esp_err_to_name(err), key);
        return;
    }

    commitToNVS();
    LOG_INFO("key '%s' - written to config", key);
}

template <typename T> void ConfigNvs::getStruct(T& variable, const char* key)
{
    variable        = T();
    size_t dataRead = sizeof(variable);

    esp_err_t err = nvs_get_blob(this->m_nvsHandle, key, &variable, &dataRead);
    switch (err)
    {
        case ESP_OK:
            LOG_DEBUG("Value read from config (key '%s')", key);
            if (dataRead != sizeof(variable))
            {
                LOG_WARNING("getStruct - Unexpected data size read: %u", static_cast<uint32_t>(dataRead));
            }
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            LOG_DEBUG("Failed to get key: %s - not initialized yet", key);
            break;
        default:
            LOG_ERROR("Error (%s) reading! key %s", esp_err_to_name(err), key);
            break;
    }
}
