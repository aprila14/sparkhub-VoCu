// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "ResetGpio";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "reset_button_handler.h"

#include "app_controller.h"
#include "sleep.h"


namespace
{

constexpr int32_t TIME_TO_DETECT_RESET_MS = 5000;  // After pressing button for this time, reset will be detected

//delays for user button handling. It assumes there are no 'dead bands' between different functions,
//therefore MAX_TIME_FUNCTION_1 = MIN_TIME_FUNCTION_2

constexpr int32_t DEBOUNCING_DELAY = 400;
constexpr int32_t MAX_TIME_TO_DETECT_RESET_MS = 5000;
constexpr int32_t MIN_TIME_TO_DETECT_CONFIGURATION_RESET_MS = MAX_TIME_TO_DETECT_RESET_MS;
constexpr int32_t MAX_TIME_TO_DETECT_CONFIGURATION_RESET_MS = 15000;
constexpr int32_t MIN_TIME_TO_DETECT_FACTORY_FIRMWARE_RESET_MS = MAX_TIME_TO_DETECT_CONFIGURATION_RESET_MS;
constexpr int32_t ADDITIONAL_DELAY_BEFORE_SOFTWARE_RESET = 500;

void _run(void* pObject)
{
    ResetButtonHandler* pResetButtonHandler = reinterpret_cast<ResetButtonHandler*>(pObject);  // NOLINT
    pResetButtonHandler->run();
}

}


ResetButtonHandler::ResetButtonHandler(gpio_num_t gpioNumber)
:
m_gpioNumber(gpioNumber),
m_taskHandle(nullptr),
m_buttonPressedDurationMs(0),
m_resetSignalDetected(false),
m_softwareRestartReady(false),
m_configurationResetReady(false)
{

}

bool ResetButtonHandler::init() const
{
    LOG_INFO("init()...");

    gpio_config_t gpioConfig = {};
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE; // hardware pull down needs to be used, button used also during booting for factory firmware reset
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;    // hardware pull down needs to be used, button used also during booting for factory firmware reset
    gpioConfig.pin_bit_mask = 1ull << static_cast<uint64_t>(m_gpioNumber);
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    if (gpio_config(&gpioConfig) != ESP_OK)
    {
        LOG_ERROR("Failed to init ResetButtonHandler!");
        return false;
    }

    return true;
}

void ResetButtonHandler::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned = xTaskCreate(
                _run,
                LOG_TAG,
                DEFAULT_STACK_SIZE,
                this,
                DEFAULT_TASK_PRIORITY,
                &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void ResetButtonHandler::run()
{
    while (true)
    {
        perform();
    }
}

void ResetButtonHandler::perform()
{
    constexpr int32_t SLEEP_TIME_MS = 500;

    const EResetButtonState state = getState();
    LOG_DEBUG("state = %d", state);

    if (state == EResetButtonState::PRESSED)
    {
        if (m_buttonPressedDurationMs == 0)
        {
            LOG_INFO("Button pressed!");
        }

        //Check if button press time fits for software restart command
        if ((m_buttonPressedDurationMs > DEBOUNCING_DELAY) && (m_buttonPressedDurationMs < MAX_TIME_TO_DETECT_RESET_MS) && !m_softwareRestartReady)
        {
            LOG_INFO("Release button to order software restart");
            m_softwareRestartReady = true;
        }

        //Check if button press time fits for configuration reset command
        if ((m_buttonPressedDurationMs > MIN_TIME_TO_DETECT_CONFIGURATION_RESET_MS) && (m_buttonPressedDurationMs < MAX_TIME_TO_DETECT_CONFIGURATION_RESET_MS) && !m_configurationResetReady)
        {
            LOG_INFO("Release button to order configuration reset");
            m_configurationResetReady = true;
            m_softwareRestartReady = false;
        }

        //
        if (m_buttonPressedDurationMs > MIN_TIME_TO_DETECT_FACTORY_FIRMWARE_RESET_MS)
        {
            LOG_INFO("ESP32 will execute firmware reset now", m_buttonPressedDurationMs);
            m_configurationResetReady = false;
            SLEEP_MS(ADDITIONAL_DELAY_BEFORE_SOFTWARE_RESET);  // additional delay just in case
            esp_restart();
        }
        m_buttonPressedDurationMs += SLEEP_TIME_MS;
    }
    else
    {
        if (m_buttonPressedDurationMs != 0)
        {
            LOG_INFO("Button released!");
        }

        if (m_softwareRestartReady)
        {
            // ESP32 software restart code here
            LOG_INFO("ESP32 will execute software restart in 1 s from now", m_buttonPressedDurationMs);
            SLEEP_MS(1000);  // give some time for logs to flush...
            esp_restart();
        }
        else if (m_configurationResetReady)
        {
            // ESP32 configuration reset code here
            LOG_INFO("ESP32 will execute configuration reset now", m_buttonPressedDurationMs);
            app::pAppController->addEvent(app::EEventType::PERFORM_CONFIGURATION_RESET_AND_RESTART);
        }

        m_buttonPressedDurationMs = 0;
        m_softwareRestartReady = false;
        m_configurationResetReady = false;
    }


    // potentially we can use interrupts instead of sleep, but no need to optimize now
    SLEEP_MS(SLEEP_TIME_MS);
}

ResetButtonHandler::EResetButtonState ResetButtonHandler::getState() const
{
    if (gpio_get_level(m_gpioNumber))
        return EResetButtonState::PRESSED;
    else
        return EResetButtonState::RELEASED;
}
