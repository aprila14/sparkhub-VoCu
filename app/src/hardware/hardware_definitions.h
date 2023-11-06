#ifndef HARDWARE_DEFINITIONS_H
#define HARDWARE_DEFINITIONS_H


#include "driver/gpio.h"


namespace hardware
{

constexpr gpio_num_t RESET_BUTTON_GPIO = GPIO_NUM_15;  // TODO - determine fitting gpio
constexpr gpio_num_t LED_GPIO = GPIO_NUM_30;  // TODO - determine fitting gpio
constexpr gpio_num_t INDICATOR_LED_GPIO = GPIO_NUM_13; // TODO - check if it's fitting GPIO

}


#endif // HARDWARE_DEFINITIONS_H
