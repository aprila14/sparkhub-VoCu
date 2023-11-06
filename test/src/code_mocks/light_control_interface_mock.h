#ifndef LIGHT_CONTROL_INTERFACE_MOCK_H
#define LIGHT_CONTROL_INTERFACE_MOCK_H


#include "light_control_interface.h"

#include "gmock/gmock.h"


class LightControlInterfaceMock
        : public LightControlInterface
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(bool, setPower, (uint8_t), (override));
    MOCK_METHOD(uint8_t, getPower, (), (override));
};

#endif // LIGHT_CONTROL_INTERFACE_MOCK_H
