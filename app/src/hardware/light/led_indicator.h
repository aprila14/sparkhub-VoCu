#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H

#include "defines.h"
#include "driver/gpio.h"

class LedIndicator
{
public:
    LedIndicator(gpio_num_t gpioNumber);

    bool init();

    void ledOn();
    void ledOff();
    void blink();

#if !TESTING
private:
#endif
    gpio_num_t m_gpioNumber;
};

#endif // LEDINDICATOR_H
