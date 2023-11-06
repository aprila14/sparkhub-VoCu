#include "defines.h"

#include "test_utils.h"
#include "custom_types.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "json_parser.h"

#include "app_controller_mock.h"
#include "cloud_controller.h"

#include "mock_mqtt_client.h"

using namespace testing;

TEST(CloudController, getConnctionStatus)
{
    CloudController cloudController;

    cloudController.m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_CONNECTED;
    ASSERT_EQ(cloudController.getConnectionStatus(), ECloudConnectionStatus::CLOUD_STATUS_CONNECTED);

    cloudController.m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_DISABLED;
    ASSERT_EQ(cloudController.getConnectionStatus(), ECloudConnectionStatus::CLOUD_STATUS_DISABLED);

    cloudController.m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED;
    ASSERT_EQ(cloudController.getConnectionStatus(), ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED);

    cloudController.m_connectionStatus = ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED;
    ASSERT_EQ(cloudController.getConnectionStatus(), ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED);
}

TEST(CloudController, setConnectionStatus)
{
    resetCmock();

    CloudController cloudController;

    cloudController.setConnectionStatus(ECloudConnectionStatus::CLOUD_STATUS_CONNECTED);
    ASSERT_EQ(cloudController.m_connectionStatus, ECloudConnectionStatus::CLOUD_STATUS_CONNECTED);

    cloudController.setConnectionStatus(ECloudConnectionStatus::CLOUD_STATUS_DISABLED);
    ASSERT_EQ(cloudController.m_connectionStatus, ECloudConnectionStatus::CLOUD_STATUS_DISABLED);

    cloudController.setConnectionStatus(ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED);
    ASSERT_EQ(cloudController.m_connectionStatus, ECloudConnectionStatus::CLOUD_STATUS_NOT_CONFIGURED);

    cloudController.setConnectionStatus(ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED);
    ASSERT_EQ(cloudController.m_connectionStatus, ECloudConnectionStatus::CLOUD_STATUS_NOT_CONNECTED);
}

TEST(CloudController, handleHeartbeatResponse)
{
    resetCmock();

    CloudController cloudController;

    cloudController.m_heartbeatWatchdogCounter = 123;

    cloudController.handleHeartbeatResponse();
    ASSERT_EQ(0, cloudController.m_heartbeatWatchdogCounter);
}

TEST(CloudController, handleSetLightIntensityLevel)
{
    resetCmock();

    CloudController cloudController;
    testing::StrictMock<app::AppControllerMock> appController(NO_LIGHT_CONTROLLER, NO_WIFI_CONTROLLER, NO_BLE_CONTROLLER, NO_CLOUD_CONTROLLER, NO_NTP_CLIENT, NO_AWS_CONTROLLER);

    app::pAppController = &appController; // addEvent function needs pointer to appController and it can't get it another way easily

    json_parser::TSetLightLevel setLightLevelStructure =
    {
        .lightIntensityLevel = 75
    };

    EXPECT_CALL(appController, addEvent(app::EEventType::LIGHT_CONTROL__SET_POWER, app::EEventExecutionType::SYNCHRONOUS, _)).WillOnce(Return(true));
    cloudController.handleSetLightIntensityLevel(setLightLevelStructure);

    app::pAppController = nullptr;

    //No assert needed here, it's just meant to check if it wouldn't crash on the instruction above
}
