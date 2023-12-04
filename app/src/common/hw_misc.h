/*
Miscelenous ESP32 hardware related functionality
*/

#ifndef HW_MISC_H
#define HW_MISC_H


#include "defines.h"

#include "driver/gpio.h"

#include <cstdint>


constexpr int32_t GPIO_CHANGE_DID_NOT_HAPPEN = -1;


namespace hw_misc
{

void initializeNVS();


int32_t waitForGpioChange(gpio_num_t gpio, bool desiredState, int32_t timeout);
bool getGpioState(gpio_num_t);

bool getStaMacAddressString(char* macAddress, const int32_t len);

}  // namespace hw_misc

#endif
