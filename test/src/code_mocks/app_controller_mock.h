#ifndef APP_CONTROLLER_MOCK_H
#define APP_CONTROLLER_MOCK_H


#include "app_controller.h"

#include "gmock/gmock.h"



constexpr LightControlInterface* NO_LIGHT_CONTROLLER = nullptr;
constexpr WiFiController* NO_WIFI_CONTROLLER = nullptr;
constexpr BleController* NO_BLE_CONTROLLER = nullptr;
constexpr CloudController* NO_CLOUD_CONTROLLER = nullptr;
constexpr NtpClient* NO_NTP_CLIENT = nullptr;
constexpr AwsOtaController* NO_AWS_CONTROLLER = nullptr;

namespace app
{

class AppControllerMock
        : public AppController
{
public:
    AppControllerMock(
            LightControlInterface* pLightControlInterface,
            WiFiController* pWifiController,
            BleController* pBleController,
            CloudController* pCloudController,
            NtpClient* pNtpClient,
            AwsOtaController* pAwsOtaController)
        : AppController(pLightControlInterface, pWifiController, pBleController, pCloudController, pNtpClient, pAwsOtaController) {}

    MOCK_METHOD(bool, addEvent, (EEventType, EEventExecutionType, TEventData*), (override));
};

}  // namespace app


#endif // APP_CONTROLLER_MOCK_H
