#ifndef HARDWARE_DEFINITIONS_H
#define HARDWARE_DEFINITIONS_H


#include "driver/gpio.h"


namespace hardware
{

constexpr gpio_num_t RESET_BUTTON_GPIO = GPIO_NUM_15; // TODO - determine fitting gpio

} // namespace hardware


#endif // HARDWARE_DEFINITIONS_H
