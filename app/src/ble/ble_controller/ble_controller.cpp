// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "BleController";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "ble_controller.h"

#include "circular_buffer.h"
#include "handle_command.h"
#include "handle_response.h"
#ifdef IS_PC
#include "console_control.h"
#endif

#include <cstdio>

#ifdef IS_PC
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#else
#include "console_control.h"
#include "sleep.h"
#endif



#ifdef IS_PC
#define READ_BLOCK_UNTIL_DATA_AVAILABLE false
#else
#define READ_BLOCK_UNTIL_DATA_AVAILABLE true
#endif


BleController::BleController(BleuartDriver* pBleuartDriver)
:
m_protocolControl(),
m_shouldStop(false),
m_receivedPacketsCounter(0),
m_pBleuartDriver(pBleuartDriver)
#ifdef IS_ESP
,m_asynchronousMessagesControl(),
m_asynchronousMessagesTaskHandle(nullptr),
m_bleControllerTaskHandle(nullptr),
m_protocolControlAccessMutex(nullptr)
#endif
{
    #ifdef IS_ESP
    m_protocolControlAccessMutex = mutexCreate();
    #endif
}

bool BleController::sendPacket(prot::EPacketType packetType,  uint8_t* payload, uint16_t payloadLen)
{

    // NOTE: m_protocolControl has a shared buffer for output data, therefore we need to protect access to it
    // ideally it should be inside m_protocolControl, but this solution with mutex here is good enough
    #ifdef IS_ESP
    LOCK_GUARD(m_protocolControlAccessMutex, protocolControlAccessMutexGuard);
    #endif
    int bytesToSend = m_protocolControl.parsePacketToOutBuffer((uint16_t) packetType, payload, payloadLen);
    if (bytesToSend == prot::NO_PACKET_ASSEMBLED)
    {
        LOG_ERROR("Failed to assemble packet type %d", (int)packetType);
        return false;
    }

    LOG_INFO("Bytes to send: %d", bytesToSend);

    bool result = m_pBleuartDriver->writeData(
                m_protocolControl.getOutEncodedBytes(),
                (uint32_t) bytesToSend);

    if (!result)
    {
        LOG_ERROR("Failed to send packet type %d", (int)packetType);
    }

    return result;
}

#if IS_ESP
void BleController::addAsynchronousEvent(EAsynchronousEventType eventType, TAsynchronousEventData* pEventData)
{
    m_asynchronousMessagesControl.addNewMessageToQueue(eventType, pEventData);
}

bool BleController::isClientConnected()
{
    return m_pBleuartDriver->isClientConnected();
}
#endif  // IS_ESP

EPacketHandlingResult BleController::handleGeneralPacket(prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen)  // NOLINT, ignore static
{
    UNUSED(payload);

    LOG_DEBUG("Handling general packet type 0x%X with payload size %d", (uint16_t) packetType, payloadLen);

    switch (packetType)
    {
        case prot::EPacketType::ACK:
            LOG_INFO("ACK received");
            return EPacketHandlingResult::HANDLED;
        case prot::EPacketType::NACK:
            LOG_WARNING("NACK received");
            return EPacketHandlingResult::HANDLED;
        default:
            LOG_ERROR("Unknown packet type: %d (0x%04X). Length %u", (int16_t) packetType, (uint16_t) packetType, payloadLen);
            return EPacketHandlingResult::SEND_NACK;
    }
}


bool BleController::handlePacket(prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen)
{
    EPacketHandlingResult result = EPacketHandlingResult::SEND_NACK;

    if (packetType >= prot::EPacketType::CMD__START && packetType <= prot::EPacketType::CMD__END)
    {
#ifdef IS_ESP
        result = handleCommand(this, packetType, payload, payloadLen);
#else
        result = EPacketHandlingResult::SEND_NACK;
        LOG_ERROR("Command received! Only ESP can handle commands!");
#endif
    }
    else if (packetType >= prot::EPacketType::RES__START && packetType <= prot::EPacketType::RES__END)
    {
#ifdef IS_PC
        result = handleResponse(this, packetType, payload, payloadLen);
#else
        result = EPacketHandlingResult::SEND_NACK;
        LOG_ERROR("Response for command received! Only PC can handle responses!");
#endif
    }
    else
    {
        result = handleGeneralPacket(packetType, payload, payloadLen);
    }


    switch (result)
    {
        case EPacketHandlingResult::SEND_ACK:
            LOG_INFO("Sending response - ACK...");
            sendPacket(prot::EPacketType::ACK, NULL, 0);
            break;
        case EPacketHandlingResult::SEND_NACK:
            LOG_INFO("Sending response - NACK...");
            sendPacket(prot::EPacketType::NACK, NULL, 0);
            break;
        case EPacketHandlingResult::HANDLED:
            break;
        default:
            LOG_ERROR("Invalid packet handling result: %d", (int) result);
            return false;
    }

    return true;
}

#ifdef IS_PC


/**
 * @brief Read a key from keyboard, but with timeout
 * @param timeoutUs Time to wait for a key press
 * @return pressed character on success, 0 otherwise
 */
static char readKeyWithTimeout(uint32_t timeoutUs)
{
    char buf[2];
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    usleep(timeoutUs);

    int numRead = (int) read(0, buf, 1);
    if (numRead > 0 && buf[0] != '\n')
    {
        return buf[0];
    }

    return 0;
}
#endif


void BleController::processReceivedData()
{
    while (!m_shouldStop)
    {
        int inByte = m_pBleuartDriver->readByte(READ_BLOCK_UNTIL_DATA_AVAILABLE);
        if (inByte == CIRCULAR_BUFFER__NO_DATA)
            break;



        int ret = m_protocolControl.processData((uint8_t) inByte);
        if (ret != prot::NO_PACKET_ASSEMBLED)
        {
            m_receivedPacketsCounter++;
#ifdef IS_DEBUG_BUILD
            LOG_DEBUG("Total received assembled packets: %u", (uint32_t) m_receivedPacketsCounter);
#endif

            const uint16_t packetPayloadLen = (uint16_t)  ret;
            const prot::TPacketHeader* packetHeader = reinterpret_cast<const prot::TPacketHeader*>(&m_protocolControl.getInPacket().packet.header);  // NOLINT we need reinterpret_cast
            prot::EPacketType packetType = static_cast<prot::EPacketType>(packetHeader->type);
            const uint8_t* payload = m_protocolControl.getInPacket().packet.payload;

            handlePacket(packetType, payload, packetPayloadLen);

#ifdef IS_ESP
            SLEEP_MS(10);  // sleep at least a moment after every received package
#endif
        }
    }
}


#ifdef IS_ESP

void BleController::runAsynchoronousMessagesSender(void* userData)
{
    BleController* pController = static_cast<BleController*>(userData);
    pController->_runAsynchoronousMessagesSender();
}

void BleController::_runAsynchoronousMessagesSender()
{
    static_assert(static_cast<int>(EAsynchronousEventType::NUMBER_OF_EVENTS) == 5,
        "WARNING! Did you forget to update the code above/below after adding a new asynchronous event? "
        "This assert makes sure you will not forget!");

    while (!m_shouldStop)
    {
        TAsynchronousMessage message = { .eventType = EAsynchronousEventType::UNKNOWN_EVENT, .eventData = {} };
        if (m_asynchronousMessagesControl.receiveFromQueue(&message))
        {
            switch (message.eventType)
            {
                case EAsynchronousEventType::WIFI_CONNECTED_TO_AP:
                {
                    LOG_INFO("About to send a WIFI_CONNECTED_TO_AP asynchronous event...");
                    prot::TAsyncEvent_WiFiConnectedToAp& cmd = message.eventData.wiFiConnectedToApEvent;
                    bool result = sendPacket(prot::EPacketType::RES_ASYNC__WIFI_CONNECTED_TO_AP,
                                             reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd));  // NOLINT we need reinterpret_cast
                    if (!result)
                        LOG_ERROR("Failed to send a RES_ASYNC__WIFI_CONNECTED_TO_AP!");
                    break;
                }
                case EAsynchronousEventType::WIFI_DISCONNECTED_FROM_AP:
                {
                    LOG_INFO("About to send a WIFI_DISCONNECTED_FROM_AP asynchronous event...");
                    prot::TAsyncEvent_WiFiDisonnectedFromAp& cmd = message.eventData.wiFiDicsonnectedFromApEvent;
                    bool result = sendPacket(prot::EPacketType::RES_ASYNC__WIFI_DISCONNECTED_FROM_AP,
                                             reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd));  // NOLINT we need reinterpret_cast
                    if (!result)
                        LOG_ERROR("Failed to send a RES_ASYNC__WIFI_DISCONNECTED_FROM_AP!");
                    break;
                }

                case EAsynchronousEventType::CLOUD_CONNECTED:
                {
                    LOG_INFO("About to send a CLOUD_CONNECTED asynchronous event...");
                    prot::TAsyncEvent_CloudConnected& cmd = message.eventData.cloudConnectedEvent;
                    bool result = sendPacket(prot::EPacketType::RES_ASYNC__CLOUD_CONNECTED,
                                             reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd)); // NOLINT - we need reinterpret_cast
                    if (!result)
                    {
                        LOG_ERROR("Failed to send a RES_ASYNC__CLOUD_CONNECTED!");
                    }
                    break;
                }

                case EAsynchronousEventType::CLOUD_DISCONNECTED:
                {
                    LOG_INFO("About to send a CLOUD_DISCONNECTED asynchronous event...");
                    prot::TAsyncEvent_CloudDisconnected& cmd = message.eventData.cloudDisconnectedEvent;
                    bool result = sendPacket(prot::EPacketType::RES_ASYNC__CLOUD_DISCONNECTED,
                                             reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd)); // NOLINT - we need reinterpret_cast
                    if (!result)
                    {
                        LOG_ERROR("Failed to send a RES_ASYNC__CLOUD_DISCONNECTED!");
                    }
                    break;
                }
                default:
                {
                    LOG_ERROR("Unknown asynchronous message event!");
                    break;
                }
            }
        }
    }
}

void BleController::startAsynchronousMessagesSenderTask()
{
    LOG_INFO("startAsynchronousMessagesSenderTask started...");
    BaseType_t xReturned = xTaskCreate(
                BleController::runAsynchoronousMessagesSender,
                "AMS",
                DEFAULT_SLIGHTLY_BIGGER_STACK_SIZE,
                this,
                HIGH_TASK_PRIORITY,
                &m_asynchronousMessagesTaskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", "AMS");
}

void BleController::run(void* userData)
{
    BleController* pBleController = static_cast<BleController*>(userData);
    pBleController->_run();
}

void BleController::_run()
{
    while (!m_shouldStop)
    {
        processReceivedData();
        SLEEP_MS(10);  // if performance becomes an issue, consider waiting on a semaphore for new data instead
    }
}
#endif


void BleController::runTask()
{
    LOG_INFO("BleController runTask...");


#ifdef IS_ESP
    startAsynchronousMessagesSenderTask();

    BaseType_t xReturned = xTaskCreate(
                BleController::run,
                LOG_TAG,
                DEFAULT_HUGE_STACK_SIZE,
                this,
                HIGH_TASK_PRIORITY,
                &m_bleControllerTaskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", "AMS");
#endif  // IS_ESP


#ifdef IS_PC
    LOG_INFO("Press at least one key and then press 'enter' to send a command "
             "(or multiple characters to schedule multiple commands)... E.g. 't' to sent the 'test' command");

    while (!m_shouldStop)
    {
        processReceivedData();

        char pressedKey = readKeyWithTimeout(20*1000);
        if (pressedKey)
            handleKeyPress(this, pressedKey);
    }
#endif

}

