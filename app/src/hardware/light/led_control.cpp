// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "LedControl";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "led_control.h"

#include "commons.h"
#include "config_nvs.h"

#include "driver/ledc.h"


namespace
{
constexpr uint32_t TIMER_FREQUENCY  = 5000;
constexpr uint32_t PWM_INITIAL_DUTY = 0;
constexpr uint32_t PWM_MAX          = 1023;
} // namespace

LedControl::LedControl(gpio_num_t gpioNumber) : m_gpioNumber(gpioNumber), m_powerPercentage(PWM_INITIAL_DUTY)
{
}

bool LedControl::initPWM() const
{
    LOG_INFO("Starting PWM initialization");
    ledc_timer_config_t ledControlTimer = {};

    ledControlTimer.speed_mode      = LEDC_LOW_SPEED_MODE;
    ledControlTimer.duty_resolution = LEDC_TIMER_10_BIT;
    ledControlTimer.timer_num       = LEDC_TIMER_0;
    ledControlTimer.freq_hz         = TIMER_FREQUENCY;
    ledControlTimer.clk_cfg         = LEDC_AUTO_CLK;


    if (ledc_timer_config(&ledControlTimer) != ESP_OK)
    {
        LOG_ERROR("Failed to config ledControlTimer!");
        return false;
    }

    ledc_channel_config_t ledcControlChannel = {};

    ledcControlChannel.gpio_num   = this->m_gpioNumber;
    ledcControlChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcControlChannel.channel    = LEDC_CHANNEL_0;
    ledcControlChannel.intr_type  = LEDC_INTR_DISABLE;
    ledcControlChannel.timer_sel  = LEDC_TIMER_0;
    ledcControlChannel.duty       = percentageToDuty(this->m_powerPercentage);
    ledcControlChannel.hpoint     = 0;


    if (ledc_channel_config(&ledcControlChannel) != ESP_OK)
    {
        LOG_ERROR("Failed to config ledControlTimer!");
        return false;
    }

    if (ledc_fade_func_install(0) != ESP_OK)
    {
        LOG_ERROR("Failed to enable fading");
        return false;
    }

    return true;
}

int32_t LedControl::percentageToDuty(uint8_t percentage)
{
    return percentage / 100.0 * PWM_MAX;
}

bool LedControl::init()
{
    m_powerPercentage = pConfig->getLastLightBrightness();
    LOG_INFO("m_powerPercentage = %d", m_powerPercentage);

    if (!initPWM())
    {
        LOG_ERROR("Error occured while initializing LedControl");
        return false;
    }

    return true;
}

bool LedControl::setPower(uint8_t powerPercentage)
{
    LOG_INFO("Setting power to %d", powerPercentage);

    uint32_t duty = percentageToDuty(powerPercentage);
    if (ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty, 0) != ESP_OK)
    {
        LOG_ERROR("Parameter error while setting PWM duty cycle");
        return false;
    }
    m_powerPercentage = powerPercentage;
    pConfig->setLastLightBrightness(powerPercentage);
    return true;
}

uint8_t LedControl::getPower()
{
    return m_powerPercentage;
}
