#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "light_control_interface.h"

#include "driver/gpio.h"


class LedControl
        : public LightControlInterface
{
public:
    LedControl(gpio_num_t gpioNumber);

    virtual bool init() override;

    virtual bool setPower(uint8_t powerPercentage) override;

    virtual uint8_t getPower() override;

#if !TESTING
private:
#endif
    LedControl() = delete;

    gpio_num_t m_gpioNumber;
    uint8_t m_powerPercentage;
    static constexpr int PWM_MAX = 1023;

    bool initPWM() const;

    static int32_t percentageToDuty(uint8_t percentage);
};

#endif // LED_CONTROL_H
