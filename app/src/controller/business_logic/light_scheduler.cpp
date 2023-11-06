// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "LightScheduler";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "light_scheduler.h"

#include "sleep.h"


namespace
{

constexpr int32_t TIME_TO_DETECT_RESET_MS = 5000;  // After pressing button for this time, reset will be detected


void _run(void* pObject)
{
    LightScheduler* pLightScheduler = reinterpret_cast<LightScheduler*>(pObject);  // NOLINT
    pLightScheduler->run();
}

}


bool LightScheduler::init() const  // NOLINT - it soon will not be static
{
    LOG_INFO("init()...");
    return true;
}

void LightScheduler::runTask()
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

void LightScheduler::run()
{
    while (true)
    {
        perform();
    }
}

void LightScheduler::perform()  // NOLINT - it will soon not be a const method
{
    SLEEP_MS(10000);
}
