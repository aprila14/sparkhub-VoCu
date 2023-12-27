// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "PulseCounter";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "pulse_counter.h"

#include "freertos/task.h"
#include "defines.h"
#include "esp_attr.h"

#if IS_DEBUG_BUILD
#include "driver/ledc.h"
#endif // IS_DEBUG_BUILD

namespace
{
/*
 * An interrupt will be triggered when the counter value:
 *   - reaches 'thresh1' or 'thresh0' value,
 *   - reaches 'l_lim' value or 'h_lim' value,
 *   - will be reset to zero.
 */
constexpr int16_t PCNT_H_LIM_VAL     = 10;
constexpr int16_t PCNT_L_LIM_VAL     = -10;
constexpr int16_t PCNT_THRESH1_VAL   = 5;
constexpr int16_t PCNT_THRESH0_VAL   = -5;
constexpr int     PCNT_INPUT_SIG_IO  = 4; // Pulse Input GPIO
constexpr int     PCNT_INPUT_CTRL_IO = 5; // Control GPIO HIGH=count up, LOW=count down

xQueueHandle pcntEvtQueue = {}; // A queue to handle pulse counter events
} // namespace


/* A structure to pass events from the PCNT interrupt handler
 */
typedef struct
{
    pcnt_unit_t unit;   // the PCNT unit that originated an interrupt
    uint32_t    status; // information on the event type that caused the interrupt
} TPcntEvt;

PulseCounterHandler::PulseCounterHandler() : m_taskHandle()
{
}

void PulseCounterHandler::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned = xTaskCreate(run, LOG_TAG, DEFAULT_STACK_SIZE, this, HIGH_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void PulseCounterHandler::run(void* pObject)
{
    PulseCounterHandler* pPulseCounterHandler =
        reinterpret_cast<PulseCounterHandler*>(pObject); // NOLINT - we need reinterpret cast
    pPulseCounterHandler->_run();
}

/* Decode what PCNT's unit originated an interrupt
 * and pass this information together with the event type
 * using a queue.
 */
static void IRAM_ATTR pulseCounterIntrHandler(void* arg)
{
    pcnt_unit_t* pcnt_unit = (pcnt_unit_t*)arg;
    TPcntEvt     evt       = {};

    if (pcnt_unit != nullptr)
    {
        evt.unit = (*pcnt_unit);
        // Save the PCNT event type that caused an interrupt
        pcnt_get_event_status((*pcnt_unit), &evt.status);
        xQueueSendFromISR(pcntEvtQueue, &evt, NULL);
    }
}

#if IS_DEBUG_BUILD

/**
 *
 * THE FOLLOWING CODE IN "#ifdef IS_DEBUG_BUILD" IS ONLY FOR TEST PURPOSES NOT FOR PRODUCTION
 *
 * Use PCNT module to count rising edges generated by LEDC module.
 *
 * Functionality of GPIOs used in this example:
 *   - GPIO18 - output pin of a sample 1 Hz pulse generator,
 *   - GPIO4 - pulse input pin,
 *   - GPIO5 - control input pin.
 *
 * Load example, open a serial port to view the message printed on your screen.
 *
 * To do this test, you should connect GPIO18 with GPIO4.
 * GPIO5 is the control signal, you can leave it floating with internal pull up,
 * or connect it to ground. If left floating, the count value will be increasing.
 * If you connect GPIO5 to GND, the count value will be decreasing.
 */

#define LEDC_OUTPUT_IO 18 // Output GPIO of a sample 1 Hz pulse generator

/* Configure LED PWM Controller
 * to output sample pulses at 1 Hz with duty of about 10%
 * only for test purposes
 */
static void ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode      = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num       = LEDC_TIMER_1;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz         = 1; // set output frequency at 1 Hz
    ledc_timer.clk_cfg         = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel    = LEDC_CHANNEL_1;
    ledc_channel.timer_sel  = LEDC_TIMER_1;
    ledc_channel.intr_type  = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num   = LEDC_OUTPUT_IO;
    ledc_channel.duty       = 100; // set duty at about 10%
    ledc_channel.hpoint     = 0;
    ledc_channel_config(&ledc_channel);
}

#endif // IS_DEBUG_BUILD

/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 *  - set up the counter events to watch
 */
void PulseCounterHandler::initiatePulseCounter(pcnt_unit_t unit)
{
    // Prepare configuration for the PCNT unit
    pcnt_config_t pcnt_config = {};

    // Set PCNT input signal and control GPIOs
    pcnt_config.pulse_gpio_num = PCNT_INPUT_SIG_IO;
    pcnt_config.ctrl_gpio_num  = PCNT_INPUT_CTRL_IO;
    pcnt_config.channel        = PCNT_CHANNEL_0;
    pcnt_config.unit           = unit;

    // Set what to do on the positive / negative edge of pulse input
    pcnt_config.pos_mode = PCNT_COUNT_INC; // Count up on the positive edge
    pcnt_config.neg_mode = PCNT_COUNT_DIS; // Keep the counter value on the negative edge

    // Set what to do when control input is low or high
    pcnt_config.lctrl_mode = PCNT_MODE_REVERSE; // Reverse counting direction if low
    pcnt_config.hctrl_mode = PCNT_MODE_KEEP;    // Keep the primary counter mode if high

    // Set the maximum and minimum limit values to watch
    pcnt_config.counter_h_lim = PCNT_H_LIM_VAL;
    pcnt_config.counter_l_lim = PCNT_L_LIM_VAL;

    // Initialize PCNT unit
    pcnt_unit_config(&pcnt_config);

    // Configure and enable the input filter
    pcnt_set_filter_value(unit, 100);
    pcnt_filter_enable(unit);

    // Set threshold 0 and 1 values and enable events to watch
    pcnt_set_event_value(unit, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
    pcnt_event_enable(unit, PCNT_EVT_THRES_1);
    pcnt_set_event_value(unit, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
    pcnt_event_enable(unit, PCNT_EVT_THRES_0);

    // Enable events on zero, maximum and minimum limit values
    pcnt_event_enable(unit, PCNT_EVT_ZERO);
    pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_event_enable(unit, PCNT_EVT_L_LIM);

    // Initialize PCNT's counter
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);

    // Install interrupt service and add isr callback handler
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(unit, pulseCounterIntrHandler, (void*)unit);

    // Everything is set up, now go to counting
    pcnt_counter_resume(unit);
}

void PulseCounterHandler::_run()
{
    pcnt_unit_t pcnt_unit = PCNT_UNIT_0;

    int16_t       count = 0;
    TPcntEvt      evt   = {};
    portBASE_TYPE res   = {};

#if IS_DEBUG_BUILD
    // Initialize LEDC to generate sample pulse signal
    ledc_init();
#endif // IS_DEBUG_BUILD

    // Initialize PCNT event queue and PCNT functions
    pcntEvtQueue = xQueueCreate(10, sizeof(TPcntEvt));
    initiatePulseCounter(pcnt_unit);

    while (1)
    {
        /* Wait for the event information passed from PCNT's interrupt handler.
         * Once received, decode the event type and print it on the serial monitor.
         */
        res = xQueueReceive(pcntEvtQueue, &evt, 1000 / portTICK_PERIOD_MS);
        if (res == pdTRUE)
        {
            pcnt_get_counter_value(pcnt_unit, &count);
            LOG_INFO("Event PCNT unit[%d]; cnt: %d", evt.unit, count);
            if (evt.status & PCNT_EVT_THRES_1)
            {
                LOG_INFO("THRES1 EVT");
            }
            if (evt.status & PCNT_EVT_THRES_0)
            {
                LOG_INFO("THRES0 EVT");
            }
            if (evt.status & PCNT_EVT_L_LIM)
            {
                LOG_INFO("L_LIM EVT");
            }
            if (evt.status & PCNT_EVT_H_LIM)
            {
                LOG_INFO("H_LIM EVT");
            }
            if (evt.status & PCNT_EVT_ZERO)
            {
                LOG_INFO("ZERO EVT");
            }
        }
        else
        {
            pcnt_get_counter_value(pcnt_unit, &count);
            LOG_INFO("Current counter value :%d", count);
        }
    }
}