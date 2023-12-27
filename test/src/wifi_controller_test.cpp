#include "app_controller_mock.h"
#include "defines.h"
#include "wifi_controller.h"

#include "custom_types.h"
#include "test_utils.h"

#include "mock_esp_event.h"
#include "mock_esp_netif.h"
#include "mock_esp_wifi.h"
#include "mock_esp_wifi_default.h"
#include "mock_event_groups.h"
#include "mock_queue.h"
#include "mock_semphr.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

namespace
{
constexpr uint8_t MAX_QUICK_RECONNECT_ATTEMPTS = 3;

constexpr uint32_t EVENT_BIT_WIFI_CREDENTIALS_READY  = BIT0;
constexpr uint32_t EVENT_BIT_WIFI_STARTED            = BIT1;
constexpr uint32_t EVENT_BIT_WIFI_CONNECTED          = BIT2;
constexpr uint32_t EVENT_BIT_WIFI_DISCONNECTED       = BIT3;
constexpr uint32_t EVENT_BIT_WIFI_STOPPED            = BIT4;
constexpr uint32_t EVENT_BIT_WIFI_SCAN_RESULTS_READY = BIT5;

// this event enables to preempt constant reconnection to AP by a public function
constexpr uint32_t EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING = BIT6;

constexpr uint32_t EVENT_BIT_WIFI_IP_ASSIGNED = BIT7;
} // namespace

class WiFiControllerMock_1 : public WiFiController
{
public:
    MOCK_METHOD(bool, disconnectIfNeeded, (), (override));
};

TEST(WiFiController, initialization)
{
    resetCmock();

    xEventGroupCreate_IgnoreAndReturn(reinterpret_cast<EventGroupHandle_t>(1)); // NOLINT

    WiFiController wifiController;

    esp_netif_init_ExpectAndReturn(ESP_OK);
    esp_event_loop_create_default_ExpectAndReturn(ESP_OK);
    esp_netif_create_default_wifi_sta_ExpectAndReturn(ESP_OK);

    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init_ExpectAndReturn(&wifiInitConfig, ESP_OK);

    esp_event_handler_instance_register_IgnoreAndReturn(ESP_OK);
    esp_event_handler_instance_register_IgnoreAndReturn(ESP_OK);

    ASSERT_TRUE(wifiController.registerHandlers());
}

TEST(WiFiController, startConnection)
{
    resetCmock();

    xEventGroupCreate_IgnoreAndReturn(reinterpret_cast<EventGroupHandle_t>(1)); // NOLINT

    WiFiController wifiController;

    WiFiController::TWifiEvent event = {
        .eventType = WiFiController::EWifiEvent::WIFI__EVENT_INITIATE_CONNECTION,
        .reason    = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_OTHER};

    esp_wifi_start_IgnoreAndReturn(ESP_OK);
    wifiController.processWifiEvent(event); // no assert, intended to check only if the test does not crash
}

TEST(WiFiController, connectToAccessPoint)
{
    resetCmock();

    xEventGroupCreate_IgnoreAndReturn(reinterpret_cast<EventGroupHandle_t>(1)); // NOLINT

    testing::StrictMock<WiFiControllerMock_1> wifiController;

    TWiFiCredentials credentials = {};

    credentials.setSsid("TEST1");
    credentials.setPassword("Password1");
    TConnectToAccessPointCommand command = {
        .credentials = credentials,
        .timeoutMs   = 2000,
    };
    TConnectToAccessPointResult result = {};

    xEventGroupSetBits_IgnoreAndReturn(0);   // EVENT_BIT_WIFI_PUBLIC_INTERFACE_HANDLING
    xEventGroupClearBits_IgnoreAndReturn(0); // clear EVENT_BIT_WIFI_CREDENTIALS_READY and EVENT_BIT_WIFI_IP_ASSIGNED

    EXPECT_CALL(wifiController, disconnectIfNeeded()).WillOnce(Return(true));

    xQueueGenericSend_IgnoreAndReturn(pdTRUE); // initiate credentials
    xEventGroupWaitBits_IgnoreAndReturn(BIT0); // wait for credentials
    xEventGroupClearBits_IgnoreAndReturn(0);   // clear PUBLIC INTERFACE HANDLING
    xQueueGenericSend_IgnoreAndReturn(pdTRUE); // initiate connection
    xEventGroupWaitBits_IgnoreAndReturn(EVENT_BIT_WIFI_IP_ASSIGNED);
    wifiController.connectToAccessPoint(command, result, false);

    ASSERT_EQ(wifiController.m_lastConnectionErrorReason, EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR);
}

TEST(WiFiController, disconnectFromAccessPoint)
{
    resetCmock();

    testing::StrictMock<WiFiControllerMock_1> wifiController;

    wifiController.setConnectionStatus(true);

    xQueueGenericSend_IgnoreAndReturn(0);                        // EVENT_INITIATE_DISCONNECTION
    xEventGroupWaitBits_IgnoreAndReturn(EVENT_BIT_WIFI_STOPPED); // Wait until Wifi is stopped

    ASSERT_TRUE(wifiController.disconnectFromAccessPoint());
}

TEST(WiFiController, initiateConnection)
{
    resetCmock();

    WiFiController wifiController;

    WiFiController::TWifiEvent event = {
        .eventType   = WiFiController::EWifiEvent::WIFI__EVENT_INITIATE_CONNECTION,
        .reason      = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR,
        .credentials = {},
    };

    esp_wifi_start_IgnoreAndReturn(ESP_OK);
    wifiController.processWifiEvent(event);

    // just checking if test does not crash
}

TEST(WiFiController, eventConnected)
{
    resetCmock();

    WiFiController wifiController;

    WiFiController::TWifiEvent event = {
        .eventType   = WiFiController::EWifiEvent::WIFI__EVENT_CONNECTED,
        .reason      = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR,
        .credentials = {},
    };

    xEventGroupSetBits_IgnoreAndReturn(0);
    xEventGroupClearBits_IgnoreAndReturn(0);

    wifiController.processWifiEvent(event);

    ASSERT_EQ(wifiController.m_retryNumber, 0);
    ASSERT_EQ(wifiController.m_disconnectionTime, 0);
    ASSERT_TRUE(wifiController.m_connectionStatus);
}

TEST(WiFiController, initiatedDisconnection)
{
    resetCmock();

    WiFiController::TWifiEvent event = {
        .eventType   = WiFiController::EWifiEvent::WIFI__EVENT_DISCONNECTED,
        .reason      = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR,
        .credentials = {},
    };

    WiFiController                              wifiController;
    testing::StrictMock<app::AppControllerMock> appController(
        &wifiController, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    app::pAppController = &appController;
    wifiController.setConnectionStatus(true);
    wifiController.setDisconnectionInitiated(true);

    EXPECT_CALL(appController, addEvent(app::EEventType::WIFI_CONTROLLER__CONNECTION_LOST, _, _));

    xEventGroupSetBits_IgnoreAndReturn(0);
    xEventGroupClearBits_IgnoreAndReturn(0);
    esp_wifi_stop_IgnoreAndReturn(ESP_OK);

    wifiController.processWifiEvent(event);
    ASSERT_EQ(wifiController.m_connectionStatus, false);
    ASSERT_EQ(wifiController.m_retryNumber, 1);
}

TEST(WiFiController, notInitiatedDisconnection)
{
    resetCmock();

    WiFiController::TWifiEvent event = {
        .eventType   = WiFiController::EWifiEvent::WIFI__EVENT_DISCONNECTED,
        .reason      = EConnectToSsidErrorReason::DEVICE_WIFI_REASON_NO_ERROR,
        .credentials = {},
    };

    WiFiController                              wifiController;
    testing::StrictMock<app::AppControllerMock> appController(
        &wifiController, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    app::pAppController = &appController;
    wifiController.setConnectionStatus(true);
    wifiController.setDisconnectionInitiated(false);

    EXPECT_CALL(appController, addEvent(app::EEventType::WIFI_CONTROLLER__CONNECTION_LOST, _, _));

    xEventGroupSetBits_IgnoreAndReturn(0);
    xEventGroupClearBits_IgnoreAndReturn(0);

    esp_wifi_connect_ExpectAndReturn(ESP_OK); // attempting reconnection

    wifiController.processWifiEvent(event);

    ASSERT_EQ(wifiController.m_connectionStatus, false);
    ASSERT_EQ(wifiController.m_retryNumber, 1);
}
