#include "custom_types.h"

namespace
{
constexpr uint8_t MAX_FIRMWARE_URL_LENGTH = 64;
}

bool TWorkflowData::operator==(const TWorkflowData& rhs) const
{
    if (::strcmp(workflowId, rhs.workflowId) != 0)
        return false;

    if (deviceUpdateAction != rhs.deviceUpdateAction)
        return false;

    return true;
}

bool TWorkflowData::setWorkflowId(const char* newWorkflowId)
{
    if (strlen(newWorkflowId) > MAX_WORKFLOW_ID_LENGTH)
        return false;

    ::bzero(workflowId, sizeof(workflowId));
    ::strncpy(workflowId, newWorkflowId, strlen(newWorkflowId));

    return true;
}

bool TWorkflowData::isSetWorkflowId() const
{
    return (workflowId[0] != 0);
}

bool TUpdateId::operator==(const TUpdateId& rhs) const
{
    if (::strcmp(firmwareVersion, rhs.firmwareVersion) != 0)
        return false;

    if (::strcmp(updateName, rhs.updateName) != 0)
        return false;

    if (::strcmp(providerName, rhs.providerName) != 0)
        return false;

    return true;
}

bool TUpdateId::setProviderName(const char* newProviderName)
{
    if (strlen(newProviderName) > MAX_PROVIDER_NAME_LENGTH)
        return false;

    ::bzero(providerName, sizeof(providerName));
    ::strncpy(providerName, newProviderName, strlen(newProviderName));

    return true;
}

bool TUpdateId::isSetProviderName() const
{
    return (providerName[0] != 0);
}

bool TUpdateId::setUpdateName(const char* newUpdateName)
{
    if (strlen(newUpdateName) > MAX_OTA_UPDATE_NAME_LENGTH)
        return false;

    ::bzero(updateName, sizeof(updateName));
    ::strncpy(updateName, newUpdateName, strlen(newUpdateName));

    return true;
}

bool TUpdateId::isSetUpdateName() const
{
    return (updateName[0] != 0);
}

bool TUpdateId::setFirmwareVersion(const char* newFirmwareVersion)
{
    if (strlen(newFirmwareVersion) > MAX_OTA_VERSION_STRING_LENGTH)
        return false;

    ::bzero(firmwareVersion, sizeof(firmwareVersion));
    ::strncpy(firmwareVersion, newFirmwareVersion, strlen(newFirmwareVersion));

    return true;
}

bool TUpdateId::isSetFirmwareVersion() const
{
    return (firmwareVersion[0] != 0);
}
