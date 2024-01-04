#ifndef RESET_BUTTON_HANDLER_H
#define RESET_BUTTON_HANDLER_H

#include "defines.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


class ResetButtonHandler
{

public:
    ResetButtonHandler(gpio_num_t gpioNumber);

    bool init() const;
    void runTask();
    void run();

#if !TESTING
private:
#endif
    void perform();

    enum class EResetButtonState
    {
        RELEASED,
        PRESSED,
    };

    EResetButtonState getState() const;

    gpio_num_t   m_gpioNumber;
    TaskHandle_t m_taskHandle;

    int32_t m_buttonPressedDurationMs; // optionally just save press timestamp instead of incrementing time...
    bool    m_resetSignalDetected;     // whether reset signal is detected during current button press

    bool m_softwareRestartReady;    // whether button was held in a proper time to cause software restart
    bool m_configurationResetReady; // whether button was held in a proper time to cause configuration reset
};

#endif // RESET_BUTTON_HANDLER_H
