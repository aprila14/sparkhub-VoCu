#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "app_events.h"
#include "ble_controller.h"
#include "cloud_controller.h"
#include "defines.h"
#include "mqtt_client_controller.h"
#include "mutex.h"
#include "ntp_client.h"
#include "pulse_counter.h"
#include "wifi_controller.h"

namespace app
{

class AppController
{
public:
    AppController(
        WiFiController*      pwifiController,
        BleController*       pBleController,
        CloudController*     pCloudController,
        NtpClient*           pNtpClient,
        PulseCounterHandler* pPulseCounterHandler);

    void runTask();

    virtual bool addEvent(
        EEventType          eventType,
        EEventExecutionType executionType = EEventExecutionType::SYNCHRONOUS,
        TEventData*         pData         = nullptr);

    WiFiController*      getWiFiController();
    BleController*       getBleController();
    CloudController*     getCloudController() const;
    NtpClient*           getNtpClient() const;
    PulseCounterHandler* getPulseCounterHandler() const;

#if !TESTING
private:
#endif
    AppController() = delete;

    struct TEventControl
    {
        EEventType type; // type of the event to execute
        TEventData data; // data for the event
        SemaphoreHandle_t
              semaphore;        // optional semaphore to set once the event is done (used only for synchronous events)
        bool* pExecutionResult; // optional execution result - only for synchronous events
    };

    static void run(void* pObject);
    void        _run();

    bool        executeEvent(TEventControl& eventControl);
    static bool executeEvent_dummy(const TDummyEventData& eventData);
    static bool executeEvent_performDeviceRestart();
    // bool executeEvent_wifiControllerConnectToAccessPoint(const TWiFiControllerConnectToAccessPointEventData
    // &eventData) const;
    bool executeEvent_wifiControllerDisconnect() const;
    bool executeEvent_wifiControllerScanAvailableAccessPoints() const;
    bool executeEvent_wifiControllerConnectionEstablished();
    bool executeEvent_wifiControllerConnectionLost();
    bool executeEvent_wifiControllerWaitUntilConnected() const;
    bool executeEvent_cloudControllerConnectionEstablished() const;
    bool executeEvent_cloudControllerConnectionLost() const;
    bool executeEvent_cloudControllerSetCredentials() const;
    bool executeEvent_otaPerform();
    bool executeEvent_calibrateFlowMeter(const float flowMeterCalibrationValue);
    bool executeEvent_calibrateADC(const float ADCCalibrationValue);

    QueueHandle_t m_eventsQueue;
    TaskHandle_t  m_taskHandle;

    WiFiController*      m_pWifiController;
    BleController*       m_pBleController;
    CloudController*     m_pCloudController;
    NtpClient*           m_pNtpClient;
    PulseCounterHandler* m_pPulseCounterHandler;
};

// We deliberately have a globa pointer,
// in order to use the AppController as a singleton, to not pass it to all the modules all the time.
// We do not want just a global variable, to enable unit tests with mocking.
// We will create and assign the real object in main or during unit tests.
extern AppController* pAppController;

} // namespace app

#endif // APPCONTROLLER_H
