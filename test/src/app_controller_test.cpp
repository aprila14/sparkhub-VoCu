#include "app_controller.h"
#include "led_control.h"
#include "light_control_interface_mock.h"
#include "test_utils.h"

#include "app_controller_mock.h"
#include "mock_queue.h"
#include "mock_esp_system.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


using namespace testing;


TEST(AppController, addingEventsSynchronous)
{
    resetCmock();
    app::AppController appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);  // we don't care about dependent module at the moment...
    app::TEventData dummyEventData = {.dummy = {4, 5} };

    xQueueGenericSend_IgnoreAndReturn(pdPASS);
    xQueueSemaphoreTake_IgnoreAndReturn(pdPASS);
    ASSERT_FALSE(appController.addEvent(app::EEventType::DUMMY_EVENT, app::EEventExecutionType::SYNCHRONOUS, &dummyEventData));  // false because the fummy funtion is not really executed and doesn't set the result

    xQueueGenericSend_StopIgnore();
    xQueueSemaphoreTake_StopIgnore();
}

TEST(AppController, addingEventsAsynchronous)
{
    resetCmock();
    app::AppController appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);  // we don't care about dependent module at the moment...
    app::TEventData dummyEventData = {.dummy = {4, 5} };


    xQueueGenericSend_IgnoreAndReturn(pdFAIL);
    ASSERT_FALSE(appController.addEvent(app::EEventType::DUMMY_EVENT, app::EEventExecutionType::ASYNCHRONOUS, &dummyEventData));

    xQueueGenericSend_IgnoreAndReturn(pdPASS);
    ASSERT_TRUE(appController.addEvent(app::EEventType::DUMMY_EVENT, app::EEventExecutionType::ASYNCHRONOUS, &dummyEventData));

    xQueueGenericSend_StopIgnore();
}

TEST(AppController, runEvent_generalTest)
{
    resetCmock();
    app::AppController appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    appController.runTask();  // just check if executes correctly
}



TEST(AppController, runEvent_dummy_event)
{
    resetCmock();
    app::AppController appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);

    bool value = false;
    app::AppController::TEventControl eventControl = {};
    eventControl.pExecutionResult = &value;

    eventControl.type = app::EEventType::DUMMY_EVENT;
    ASSERT_TRUE(appController.executeEvent(eventControl));
}


TEST(AppController, runEvent_configurationResetAndRestart)
{
    resetCmock();
    app::AppController appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);

    bool value = false;
    app::AppController::TEventControl eventControl = {};
    eventControl.pExecutionResult = &value;


    eventControl.type = app::EEventType::PERFORM_CONFIGURATION_RESET_AND_RESTART;
    esp_restart_Expect();
    ASSERT_TRUE(appController.executeEvent(eventControl));
}


TEST(AppController, runEvent_lightControlSetPower)
{
    resetCmock();
    testing::StrictMock<LightControlInterfaceMock> lightControl;


    app::AppController appController(&lightControl, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);

    bool value = false;
    app::AppController::TEventControl eventControl = {};
    eventControl.pExecutionResult = &value;

    eventControl.type = app::EEventType::LIGHT_CONTROL__SET_POWER;
    eventControl.data.lightControlSetPower.percentage = 35;
    EXPECT_CALL(lightControl, setPower(35)).Times(1).WillOnce(Return(true));;

    ASSERT_TRUE(appController.executeEvent(eventControl));
}
