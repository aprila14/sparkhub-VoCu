static const char* LOG_TAG = "AsyncMsg";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#if IS_ESP


#include "asynchronous_message.h"


/** Size of the queue for asynchronous messages from ESP (e.g. when link gets disconnected) */
#define ASYNCHRONOUS_MESSAGES_QUEUE_SIZE (16)


AsynchronousMessagesControl::AsynchronousMessagesControl()
{
    LOG_DEBUG("init started...");
    asynchronousMessagesQueue = xQueueCreate(ASYNCHRONOUS_MESSAGES_QUEUE_SIZE, sizeof(TAsynchronousMessage));
    assert(asynchronousMessagesQueue);
}

void AsynchronousMessagesControl::addNewMessageToQueue(
    EAsynchronousEventType  eventType,
    TAsynchronousEventData* pEventData)
{
    TAsynchronousMessage message = {};
    message.eventType            = eventType;
    if (pEventData != NULL)
    {
        message.eventData = *pEventData;
    }

    if (xQueueSend(asynchronousMessagesQueue, &message, 0) == pdPASS)
        LOG_DEBUG("New asynchronous message added to queue!");
    else
        LOG_WARNING("Asynchronous messages queue full. Discarding event");
}

bool AsynchronousMessagesControl::receiveFromQueue(TAsynchronousMessage* pMessage)
{
    LOG_DEBUG("receiveFromQueue started...");


    if (xQueueReceive(asynchronousMessagesQueue, pMessage, portMAX_DELAY) == pdTRUE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // IS_ESP
