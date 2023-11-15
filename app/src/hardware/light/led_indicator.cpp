// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "LedIndicator";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "led_indicator.h"

namespace
{
    constexpr uint32_t TIMER_FREQUENCY = 5000;
    constexpr uint32_t PWM_INITIAL_DUTY = 0;
    constexpr uint32_t PWM_MAX = 1023;

    uint32_t GPIO_LEVEL_HIGH = 1;
    uint32_t GPIO_LEVEL_LOW = 0;
}

LedIndicator::LedIndicator(gpio_num_t gpioNumber)
:
m_gpioNumber(gpioNumber)
{

}

bool LedIndicator::init() // NOLINT - we don't want to make it const
{
    gpio_pad_select_gpio(m_gpioNumber);
    if (gpio_set_direction(m_gpioNumber, GPIO_MODE_OUTPUT) != ESP_OK)
    {
        LOG_INFO("Error while initializing LedIndicator");
    }

    return true;
}

void LedIndicator::ledOn() // NOLINT - we don't want to make it const
{
    gpio_set_level(m_gpioNumber, GPIO_LEVEL_HIGH);
}

void LedIndicator::ledOff() // NOLINT - we don't want to make it const
{
    gpio_set_level(m_gpioNumber, GPIO_LEVEL_LOW);
}

void LedIndicator::blink()
{
    // TODO: to be defined after getting details from customer
}
