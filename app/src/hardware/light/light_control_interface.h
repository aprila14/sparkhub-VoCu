#ifndef LIGHT_CONTROL_INTERFACE_H
#define LIGHT_CONTROL_INTERFACE_H


#include "defines.h"



/**
 * @brief Interface to control light, to be used as a common interface for simulated PWM LED and real hardware
 */
class LightControlInterface
{
public:
    LightControlInterface() = default;
    virtual ~LightControlInterface() = default;

    virtual bool init() = 0;

    /**
     * @brief Set light power (0 to 100 %)
     */
    virtual bool setPower(uint8_t powerPercentage) = 0;

    virtual uint8_t getPower() = 0;
};


constexpr LightControlInterface* NO_LIGHT_CONTROL_INTERFACE = nullptr;


#endif // LIGHT_CONTROL_INTERFACE_H
