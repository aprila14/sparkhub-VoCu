#ifndef ASYNCHRONOUS_MESSAGE_H
#define ASYNCHRONOUS_MESSAGE_H

#include "defines.h"

#include "protocol_control.h"

#if IS_ESP
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#endif  // IS_ESP


/**
 * @brief Defines available to send asynchronous events, triggered only by ESP or external conditions, without request from PC/phone.
 * NOTE: After adding a new type, add parsing in `runAsynchoronousMessagesSender`
 */
enum class EAsynchronousEventType : int16_t
{
    UNKNOWN_EVENT = 0,
    WIFI_CONNECTED_TO_AP,  ///< type of event for RES_WIFI_CONNECTED_TO_AP
    WIFI_DISCONNECTED_FROM_AP,  ///< type of event for RES_WIFI_DISCONNECTED_FROM_AP
    CLOUD_CONNECTED,
    CLOUD_DISCONNECTED,
    //    OTA_PROGRESS_REPORT,  ///< type of event for RES_OTA_PROGRESS_REPORT
//    OTA_RESULT_REPORT,  ///< type of event for RES_OTA_RESULT_REPORT
    NUMBER_OF_EVENTS
};

/**
 * @brief Data that the Asynchronous message can transfer (which is a union of all asynchronous messages payloads)
 */
union TAsynchronousEventData
{
    prot::TAsyncEvent_WiFiConnectedToAp wiFiConnectedToApEvent;
    prot::TAsyncEvent_WiFiDisonnectedFromAp wiFiDicsonnectedFromApEvent;
    prot::TAsyncEvent_CloudConnected cloudConnectedEvent;
    prot::TAsyncEvent_CloudDisconnected cloudDisconnectedEvent;

//    xxxx ota_progress_report;
//    xxxx ota_progress_report;
//    xxxx ota_result_report;
};

static_assert(static_cast<int>(EAsynchronousEventType::NUMBER_OF_EVENTS) == 5,
    "WARNING! Did you forget to update the code above/below after adding a new asynchronous event? "
    "This assert makes sure you will not forget!");

/**
 * @brief Defines a asynchronous message to be stored in queue
 */
struct TAsynchronousMessage
{
    EAsynchronousEventType eventType;
    TAsynchronousEventData eventData;
};


#if IS_ESP
class AsynchronousMessagesControl
{

public:

    AsynchronousMessagesControl();

    /**
     * @brief Queue an asynchronous event to be sent to PC/phone
     * @param eventType Event to queue
     * @pEventData event data to store. May be nullptr, if that event type doesn't transfer any data
     */
    void addNewMessageToQueue(EAsynchronousEventType eventType, TAsynchronousEventData* pEventData);

    /**
     * @brief Read a message from queue
     * @param event will be filled with event data
     * @return True if read, false otherwise
     */
    bool receiveFromQueue(TAsynchronousMessage* pMessage);


private:
    QueueHandle_t asynchronousMessagesQueue;  ///< Queue with asynchronous events to be sent to PC/phone without request

};
#endif  // IS_ESP


#endif // ASYNCHRONOUS_MESSAGE_H
