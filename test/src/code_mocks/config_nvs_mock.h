#ifndef NVS_CONFIG_MOCK_H
#define NVS_CONFIG_MOCK_H

#include "config_nvs.h"

#include "gmock/gmock.h"


class ConfigNvsMock : public ConfigNvs
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(bool, resetConfig, (), (override));

    MOCK_METHOD(bool, getConfigurationFinishedState, (), (override));
    MOCK_METHOD(void, setConfigurationFinishedState, (bool), (override));
};

#endif // NVS_CONFIG_MOCK_H
