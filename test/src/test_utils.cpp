#include "test_utils.h"

#include "mock_esp_err.h"
#include "mock_esp_log.h"
#include "mock_esp_system.h"
#include "mock_gpio.h"
#include "mock_ledc.h"
#include "mock_nvs.h"
#include "mock_nvs_flash.h"
#include "mock_queue.h"
#include "mock_semphr.h"
#include "mock_task.h"
#include "cmock_internals.h"


void resetCmock()
{
    mock_esp_err_Destroy();
    // mock_esp_log_Init();  // cpp mock not included in the project
    mock_esp_system_Destroy();
    mock_gpio_Destroy();
    mock_ledc_Destroy();
    mock_nvs_Destroy();
    mock_nvs_flash_Destroy();
    mock_queue_Destroy();
    mock_semphr_Destroy();
    mock_task_Destroy();
}
