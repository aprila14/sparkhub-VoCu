#if USE_CONSOLE
// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "ConsoleControl";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG


#include "console_control.h"

#include "app_controller.h"
#include "ble_controller.h"
#include "bleuart_esp.h"
#include "commons.h"
#include "config_nvs.h"
#include "driver/uart.h"
#include "json_parser.h"
#include "sleep.h"

#define UART_NUMBER                UART_NUM_0
#define UART_RX_BUFF_SIZE          1024
#define UART_TX_BUFF_SIZE          0
#define UART_DATA_READING_TEIMEOUT 1000


namespace
{
const char* DEFAULT_SSID_1     = "xxx"; // fill with the credentials for example Access Point
const char* DEFAULT_PASSWORD_1 = "xxx"; // fill with the credentials for example Access Point

const char* DEFAULT_SSID_2     = ""; // fill with the credentials for example Access Point
const char* DEFAULT_PASSWORD_2 = ""; // fill with the credentials for example Access Point

const char* DEFAULT_SSID_3     = ""; // fill with the credentials for example Access Point
const char* DEFAULT_PASSWORD_3 = ""; // fill with the credentials for example Access Point
} // unnamed namespace
namespace console
{

enum class EShouldQuitConsole
{
    NO,
    YES,
};


/**
 * @brief Read a single byte from UART, if available
 * @param byte
 * @return True if any byte was read, false otherwise
 */
static bool UartReadByteFromMonitor(uint8_t* byte)
{
    // in case of problems with UART - create a big static buffer
    const int rxBytes = uart_read_bytes(UART_NUMBER, byte, 1, UART_DATA_READING_TEIMEOUT / portTICK_RATE_MS);
    if (rxBytes == 1)
    {
        // LOG_DEBUG("Read single byte on UART: %d", *byte);
        return true;
    }
    return false;
}

static EShouldQuitConsole executeCommandFromConsole()
{
    /*
     Simple rules:
      - Remember to add defines if some cases are only for some products
      - For testing some functionality batches - use freely keys 1, 2, 3, 4, 5, 6, 7, 8, 9, 0.
        But before merging make sure to put them in '#if 0'
     */

    uint8_t key = 0;
    if (!UartReadByteFromMonitor(&key))
    {
        // no new byte to process, nothing to process now
        return EShouldQuitConsole::NO;
    }

    LOG_INFO("Handling key press: %c", static_cast<char>(key));

    switch (key)
    {
        /************************ COMMON FUNCTIONS FOR ALL PRODUCTS ************************/
        case '\n':
        case '\r':
        {
            printf("\n\n\n");
            break;
        }

        case '}':
        {
            LOG_INFO(
                "Currently available free heap memory: %u",
                static_cast<uint32_t>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)));
            LOG_INFO("Minimum available free heap memory in the past(?): %d", esp_get_minimum_free_heap_size());
            break;
        }

        case 'b':
        {
            g_pBleuartDriver->writeData((const uint8_t*)"hello\n", 7);
            break;
        }

        case 'B':
        {
            g_pBleuartDriver->waitUntilClientConnected();
            break;
        }

        case 'n':
        {
            TAsynchronousEventData eventData                = {};
            eventData.wiFiDicsonnectedFromApEvent.dummyByte = 'p'; // whatever, just test
            app::pAppController->getBleController()->addAsynchronousEvent(
                EAsynchronousEventType::WIFI_CONNECTED_TO_AP, &eventData);
            break;
        }

        case 'm':
        {
            LOG_INFO("%s", app::pAppController->getNtpClient()->getCurrentLocalTimeString(UtcOffset::OFFSET_UTC_2));
            break;
        }
        case '0' ... '9':
        {
            app::TEventData eventData                 = {};
            eventData.lightControlSetPower.percentage = (key - '0') * 10;
            app::pAppController->addEvent(
                app::EEventType::LIGHT_CONTROL__SET_POWER, app::EEventExecutionType::SYNCHRONOUS, &eventData);
            break;
        }

        case 'd':
        {
            app::TEventData eventData  = {};
            eventData.dummy.dummyByte1 = 55;
            eventData.dummy.dummyByte2 = 77;
            app::pAppController->addEvent(
                app::EEventType::DUMMY_EVENT, app::EEventExecutionType::SYNCHRONOUS, &eventData);
            break;
        }

        case 'D':
        {
            app::TEventData eventData  = {};
            eventData.dummy.dummyByte1 = 44;
            eventData.dummy.dummyByte2 = 66;
            app::pAppController->addEvent(
                app::EEventType::DUMMY_EVENT, app::EEventExecutionType::ASYNCHRONOUS, &eventData);
            break;
        }

        case 'x':
        {
            LOG_WARNING("Hello test X");
            break;
        }

        case 'q':
        {
            return EShouldQuitConsole::YES;
        }

        case 'r':
        {
            // disconnect from current access point
            app::TEventData eventData = {};
            app::pAppController->addEvent(
                app::EEventType::WIFI_CONTROLLER__DISCONNECT, app::EEventExecutionType::ASYNCHRONOUS);
            break;
        }

        case 'u':
        {
            // wait until connected to wifi
            app::TEventData eventData = {};
            app::pAppController->addEvent(
                app::EEventType::WIFI_CONTROLLER__WAIT_UNTIL_CONNECTED, app::EEventExecutionType::SYNCHRONOUS);
            break;
        }

        case 'c':
        {
            LOG_INFO("Resetting NVS config");
            pConfig->resetConfig();
            break;
        }

        default:
        {
            LOG_ERROR("Control for key '%c' not implemented (%d)!", key, static_cast<int>(key));
            break;
        }
    }

    return EShouldQuitConsole::NO;
}

static void _run()
{
    uart_driver_install(UART_NUMBER, UART_RX_BUFF_SIZE, UART_TX_BUFF_SIZE, 0, nullptr, 0);

    bool shouldQuit = false;
    while (!shouldQuit)
    {
        shouldQuit = (executeCommandFromConsole() == EShouldQuitConsole::YES);
        SLEEP_MS(10);
    }
}

void runConsoleControl()
{
    _run();
}

} // namespace console


#endif // USE_CONSOLE
