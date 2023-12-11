#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include "custom_types.h"
#include "defines.h"
#include "protocol_types.h"

namespace app
{

enum class EEventType
{
    UNKNOWN_EVENT = 0,
    DUMMY_EVENT, // dummy event demonstrating the usage, as simple to copy placeholder
    PERFORM_CONFIGURATION_RESET_AND_RESTART,
    WIFI_CONTROLLER__DISCONNECT,              // disconnect from current Access Point
    WIFI_CONTROLLER__CONNECTION_ESTABLISHED,  // WiFi connection just connected
    WIFI_CONTROLLER__CONNECTION_LOST,         // WiFi connection just lost
    WIFI_CONTROLLER__WAIT_UNTIL_CONNECTED,    // wait until device is connected to WiFi
    CLOUD_CONTROLLER__CONNECTION_ESTABLISHED, // Connection to cloud just established
    CLOUD_CONTROLLER__CONNECTION_LOST,        // Connection to cloud just lost
    CLOUD_CONTROLLER__SEND_CREDENTIALS,       // sending credentials to cloud
    OTA__PERFORM                              // performing OTA based on provided link
};

/**
 * @brief How the event should be executed
 */
enum class EEventExecutionType
{
    SYNCHRONOUS = 0,
    ASYNCHRONOUS,
};


/**
 * @brief Example how to handle event data.
 * Asynchronous events need to have all the fields copied by value.
 * Synchronous events can utilize data pointers.
 */
struct TDummyEventData
{
    uint8_t dummyByte1;
    uint8_t dummyByte2;
};

struct TCloudControlSendCredentialsEventData
{
    bool connectionReady;
};

struct TOtaPerformEventData
{
    bool updateReady;
};

/**
 * @brief Union for data for all events
 */
union TEventData
{
    TDummyEventData                       dummy;
    TCloudControlSendCredentialsEventData cloudControlSendCredentials;
    TOtaPerformEventData                  otaPerform;

    // add here more events ...
};


} // namespace app

#endif // APP_EVENTS_H
