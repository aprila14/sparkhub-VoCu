/**
 * @file controller.h
 * @brief Main program conrtoller. Processes packets and and controls the operation.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "asynchronous_message.h"
#include "defines.h"
#include "protocol_control.h"

#if IS_ESP
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bleuart_esp.h"
#else
#include "bleuart.h"
#endif


/**
 * @brief Enum for the packet handling result, to determine if a ACK or NACK should be sent after processing
 */
enum class EPacketHandlingResult
{
    UNKNOWN = 1,
    SEND_NACK,  ///< NACK should be sent after processing is done
    SEND_ACK,  ///< ACK should be sent after processing is done
    HANDLED  ///< no need to send anythinf, response already sent
};


/**
 * @brief class containing all controller variables
 */
class BleController
{

public:
    /**
     * @param pBleuartDriver Handle to the bleuart driver, used for BLE communication
     */
    BleController(BleuartDriver* pBleuartDriver);

    /**
     * @brief Run the BLE controller tasks.
     */
    void runTask();

    /**
     * @brief Send a packet via BLE
     * @param packetType Type of packet to send
     * @param payload Buffer with packet payload
     * @param payloadLen Size of the payload
     * @return true on success, false otherwise
     */
    bool sendPacket(
            prot::EPacketType packetType,
            uint8_t* payload,
            uint16_t payloadLen);

    /**
     * @brief Queue an asynchronous event to be sent to mobile app
     * @param eventType Event to queue
     * @pEventData event data to store. May be nullptr, if that event type doesn't transfer any data
     */
    void addAsynchronousEvent(EAsynchronousEventType eventType, TAsynchronousEventData* pEventData);

    /**
     * @brief isClientConnected Returns true if any client is connected
     * @return
     */
    bool isClientConnected();

private:
    BleController() = delete;

    /**
     * @brief Handle received packet which is a general packet (not command nor response)
     * @param packetType Type of packet to handle
     * @param payload Buffer with packet payload
     * @param payloadLen Size of the payload
     * @return
     */
    EPacketHandlingResult handleGeneralPacket(prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen);

    /**
     * @brief Handle received packet
     * @param packetType Type of packet to handle
     * @param payload Buffer with packet payload
     * @param payloadLen Size of the payload
     * @return
     */
    bool handlePacket(prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen);

    /**
     * @brief Check if there are data received via BLE and process them if there are
     */
    void processReceivedData();

    static void runAsynchoronousMessagesSender(void* userData);
    void _runAsynchoronousMessagesSender();
    void startAsynchronousMessagesSenderTask();

#ifdef IS_ESP
    static void run(void* userData);
    void _run();
#endif  // IS_ESP

    prot::ProtocolControl m_protocolControl;  ///< Instance of Protocol control unit (with buffers used during parsing)

    // TODO - protect with mutex? Or make atomic?
    bool m_shouldStop;  ///< To terminate controller operation

    size_t m_receivedPacketsCounter;  ///< Count received (via BLE) assembled packets. For debugging purposes

    BleuartDriver* m_pBleuartDriver;  ///< Handle to the bleuart driver, used for BLE communication

#ifdef IS_ESP
    AsynchronousMessagesControl m_asynchronousMessagesControl;
    TaskHandle_t m_asynchronousMessagesTaskHandle;  ///< Handle to task to send asynchronous messages from ESP (e.g. errors, link-down operation)
    TaskHandle_t m_bleControllerTaskHandle;  ///< Handle to main task
    mutex_t m_protocolControlAccessMutex;
#endif
};



/******** CHECK BUILD TYPE ******/
// Macro to differentiate the code for the PC/ESP protocol parts
// TODO - consider using separate files wit approptiate functions instead of a common file!

#if defined(IS_PC) && defined(IS_ESP)
#error "Build type cannot be set to IS_PC and IS_ESP at the same time!"
#endif


#ifdef IS_PC
#define PC_OR_ESP_BUILD_TYPE_DEFINED
// OK, it is a build for the PC
#endif

#ifdef IS_ESP
#define PC_OR_ESP_BUILD_TYPE_DEFINED
// OK, it is a build for ESP
#endif

#ifndef PC_OR_ESP_BUILD_TYPE_DEFINED
#error "Build type is not defined! Needs to be set to IS_PC or IS_ESP!"
#endif


#endif // CONTROLLER_H
