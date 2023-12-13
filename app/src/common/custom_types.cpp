#include "custom_types.h"

namespace
{
constexpr uint8_t MAX_FIRMWARE_URL_LENGTH = 64;
}

bool TFirmwareInfo::operator==(const TFirmwareInfo& rhs) const
{
    if (version != rhs.version)
    {
        return false;
    }

    if (strncmp(firmwareUrl.c_str(), rhs.firmwareUrl.c_str(), strlen(firmwareUrl.c_str())) != 0)
    {
        return false;
    }

    if (strlen(firmwareUrl.c_str()) != strlen(rhs.firmwareUrl.c_str()))
    {
        return false;
    }

    return true;
}

bool TFirmwareInfo::setFirmwareUrl(const std::string& newFirmwareUrl)
{
    if (newFirmwareUrl.size() > MAX_FIRMWARE_URL_LENGTH)
    {
        return false;
    }

    firmwareUrl = newFirmwareUrl;

    return true;
}

bool TFirmwareInfo::isSetFirmwareUrl() const
{
    return (!firmwareUrl.empty());
}