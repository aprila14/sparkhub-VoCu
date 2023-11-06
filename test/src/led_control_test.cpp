#include "app_controller_mock.h"
#include "defines.h"
#include "led_control.h"
#include "test_utils.h"

#include "mock_ledc.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"


TEST(LedControl, initialization)
{
    resetCmock();
    LedControl ledControl(GPIO_NUM_14);

    ledc_timer_config_t ledControlTimer = {};

    ledControlTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledControlTimer.duty_resolution = LEDC_TIMER_10_BIT;
    ledControlTimer.timer_num = LEDC_TIMER_0;
    ledControlTimer.freq_hz = 5000;
    ledControlTimer.clk_cfg = LEDC_AUTO_CLK;

    ledc_channel_config_t ledcControlChannel = {};

    ledcControlChannel.gpio_num = GPIO_NUM_14;
    ledcControlChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcControlChannel.channel = LEDC_CHANNEL_0;
    ledcControlChannel.intr_type = LEDC_INTR_DISABLE;
    ledcControlChannel.timer_sel =LEDC_TIMER_0;
    ledcControlChannel.duty = 0;
    ledcControlChannel.hpoint = 0;

    ledc_timer_config_ExpectAndReturn(&ledControlTimer, ESP_OK);
    ledc_channel_config_ExpectAndReturn(&ledcControlChannel, ESP_OK);
    ledc_fade_func_install_ExpectAndReturn(0,ESP_OK);
    ASSERT_TRUE(ledControl.initPWM());
}

TEST(LedControl, normalUsage)
{
    resetCmock();
    // just go through the functions without any checks...
    LedControl ledControl(GPIO_NUM_38);
    ledc_timer_config_IgnoreAndReturn(ESP_OK);
    ledc_channel_config_IgnoreAndReturn(ESP_OK);

    ledc_set_duty_and_update_ExpectAndReturn(LEDC_LOW_SPEED_MODE,
                                             LEDC_CHANNEL_0,
                                             0,
                                             0,
                                             ESP_OK);
    ASSERT_TRUE(ledControl.setPower(0));

    ledc_set_duty_and_update_ExpectAndReturn(LEDC_LOW_SPEED_MODE,
                                             LEDC_CHANNEL_0,
                                             337,
                                             0,
                                             ESP_OK);
    ASSERT_TRUE(ledControl.setPower(33));

    ledc_set_duty_and_update_ExpectAndReturn(LEDC_LOW_SPEED_MODE,
                                             LEDC_CHANNEL_0,
                                             337,
                                             0,
                                             ESP_OK);
    ASSERT_TRUE(ledControl.setPower(33));

    ledc_set_duty_and_update_ExpectAndReturn(LEDC_LOW_SPEED_MODE,
                                             LEDC_CHANNEL_0,
                                             1012,
                                             0,
                                             ESP_OK);
    ASSERT_TRUE(ledControl.setPower(99));

    ledc_set_duty_and_update_ExpectAndReturn(LEDC_LOW_SPEED_MODE,
                                             LEDC_CHANNEL_0,
                                             1023,
                                             0,
                                             ESP_OK);
    ASSERT_TRUE(ledControl.setPower(100));
}
