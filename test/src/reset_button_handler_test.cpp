#include "app_controller_mock.h"
#include "reset_button_handler.h"
#include "test_utils.h"


#include "mock_gpio.h"
#include "mock_esp_system.h"
#include "gtest/gtest.h"


using namespace testing;

TEST(ResetButtonHandle, performingSoftwareRestart)
{
    resetCmock();
    testing::StrictMock<app::AppControllerMock> appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    app::pAppController = &appController;

    ResetButtonHandler resetButtonHandler(GPIO_NUM_39);
    gpio_config_IgnoreAndReturn(ESP_OK);
    ASSERT_TRUE(resetButtonHandler.init());

    resetButtonHandler.runTask();  // just call, no point to verify result


    gpio_get_level_IgnoreAndReturn(0);
    resetButtonHandler.perform();
    ASSERT_EQ(resetButtonHandler.m_buttonPressedDurationMs, 0);

    gpio_get_level_IgnoreAndReturn(1);
    resetButtonHandler.perform();
    ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);

    for (int i = 0; i < 6; i++)  // wait the required time for software restart
    {
        gpio_get_level_IgnoreAndReturn(1);
        resetButtonHandler.perform();
        ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);
    }

    ASSERT_EQ(resetButtonHandler.m_softwareRestartReady, true);
    ASSERT_EQ(resetButtonHandler.m_configurationResetReady, false);

    gpio_get_level_IgnoreAndReturn(0);
    esp_restart_Ignore();
    resetButtonHandler.perform();
}

TEST(ResetButtonHandle, performingConfigurationReset)
{
    resetCmock();
    testing::StrictMock<app::AppControllerMock> appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    app::pAppController = &appController;

    ResetButtonHandler resetButtonHandler(GPIO_NUM_39);
    gpio_config_IgnoreAndReturn(ESP_OK);
    ASSERT_TRUE(resetButtonHandler.init());

    resetButtonHandler.runTask();  // just call, no point to verify result


    gpio_get_level_IgnoreAndReturn(0);
    resetButtonHandler.perform();
    ASSERT_EQ(resetButtonHandler.m_buttonPressedDurationMs, 0);

    gpio_get_level_IgnoreAndReturn(1);
    resetButtonHandler.perform();
    ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);



    EXPECT_CALL(appController, addEvent(app::EEventType::PERFORM_CONFIGURATION_RESET_AND_RESTART, _, _));

    for (int i = 0; i < 20; i++)  // wait the required time for configuration reset
    {
        gpio_get_level_IgnoreAndReturn(1);
        resetButtonHandler.perform();
        ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);
    }

    gpio_get_level_IgnoreAndReturn(0);
    resetButtonHandler.perform();
    ASSERT_EQ(resetButtonHandler.m_buttonPressedDurationMs, 0);
}

TEST(ResetButtonHandle, performingFactoryFirmwareResetll)
{
    resetCmock();
    testing::StrictMock<app::AppControllerMock> appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);
    app::pAppController = &appController;

    ResetButtonHandler resetButtonHandler(GPIO_NUM_39);
    gpio_config_IgnoreAndReturn(ESP_OK);
    ASSERT_TRUE(resetButtonHandler.init());

    resetButtonHandler.runTask();  // just call, no point to verify result


    gpio_get_level_IgnoreAndReturn(0);
    resetButtonHandler.perform();
    ASSERT_EQ(resetButtonHandler.m_buttonPressedDurationMs, 0);

    gpio_get_level_IgnoreAndReturn(1);
    resetButtonHandler.perform();
    ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);

    for (int i = 0; i < 30; i++)  // wait the required time for factory firmware reset
    {
        gpio_get_level_IgnoreAndReturn(1);
        resetButtonHandler.perform();
        ASSERT_NE(resetButtonHandler.m_buttonPressedDurationMs, 0);
    }

    gpio_get_level_IgnoreAndReturn(1);
    esp_restart_Ignore();
    resetButtonHandler.perform();

    ASSERT_EQ(resetButtonHandler.m_softwareRestartReady, false);
    ASSERT_EQ(resetButtonHandler.m_configurationResetReady, false);
}
