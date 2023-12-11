/**
 * @file bleuart_esp.h
 * @brief Provides BLE handling functionality.
 * One should first create a driver (instance of TBleuartDriver), and then pass it to functions
 */


#ifndef BLEUART_ESP_H
#define BLEUART_ESP_H


#include "defines.h"

#include "circular_buffer.h"
#include "mutex.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


/**
 * @brief Bluetooth driver
 * All data used to control the BLE communication grouped into a driver. Only one instance should be created.
 * Passed to all BLE handling functions
 */
class BleuartDriver
{
public:
    BleuartDriver();
    ~BleuartDriver();


    /**
     * @brief Write data to BLE - transmit it to the client.
     * Actually this function only queues data, which will be sent reliably in another thread later
     * @param pData Data to write buffer
     * @param dataLength Length of the data to write
     * @return true on success, false otherwise
     */
    bool writeData(const uint8_t* pData, uint32_t dataLength);

    /**
     * @brief Read a single byte of the received data
     * @param blockUntilDataAvailable - block until data is available
     * @return Single byte if there are any data to read.  CIRCULAR_BUFFER__NO_DATA (-1) if the buffer is empty
     */
    int readByte(bool blockUntilDataAvailable);

    /**
     * @brief Initialize the BLE module and start BLE handling thread
     * @return Operation error code - see EErrorCode description
     */
    bool runTask();

    /**
     * @brief Stop the thread until BLE client is connected
     */
    void waitUntilClientConnected();

    /**
     * @brief isClientConnected Returns true if BLE client is connected
     * @return
     */
    bool isClientConnected() const;

    /**
     * @brief Get number of available space in TX (transmit) buffer
     * @return Number of bytes
     */
    int getFreeTxBufferSize();

    /**
     * @brief Handle data received from client via BLE interface.
     * @param pData data Received data
     * @param dataSize Size of the received data
     */
    void handleIncomingData(const uint8_t* pData, uint32_t dataSize);

private:
    BleuartDriver(const BleuartDriver&) = delete;


    /**
     * @brief printBleMacAddress - print in the log console the MAC address of this device.
     * Should be called only after the ESP BLE module is already synced!
     */
    static void printBleMacAddress();

    /**
     * The nimble host executes this callback when a GAP event occurs.  The
     * application associates a GAP event callback with each connection that forms.
     * bleuart uses the same callback for all connections.
     *
     * Function comes from Nimble example - see note at the beginning of the file, but is slightly modified
     *
     * @param event                 The type of event being signalled.
     * @param ctxt                  Various information pertaining to the event.
     * @param arg                   Application-specified argument; unuesd by
     *                                  bleuart.
     *
     * @return                      0 if the application successfully handled the
     *                                  event; nonzero on failure.  The semantics
     *                                  of the return code is specific to the
     *                                  particular GAP event being signalled.
     */
    static int handleGapEvent(struct ble_gap_event* event, void* arg);

    void _handleIncomingData(const uint8_t* pData, uint32_t dataSize);

    /**
     * @brief See handleGapEvent
     */
    int _handleGapEvent(struct ble_gap_event* event);

    /**
     * Enables advertising with the following parameters:
     * Function comes from Nimble example - see note at the beginning of the file, but is slightly modified
     *     o General discoverable mode.
     *     o Undirected connectable mode.
     */
    void advertise();

    static void synchronizationCallback();
    void        _synchronizationCallback();

    /**
     * @brief Non returning task for the BLE TX thread
     * @param param bleuart driver handle
     */
    static void performTxLoop(void* pBleuartDriverVoid);

    /**
     * @brief See performTxLoop
     */
    void _performTxLoop();


    CircularBuffer    m_rxBuffer;      ///< Circular buffer queuing raw data received from BLE, for further processing
    SemaphoreHandle_t m_rxBufferMutex; ///<  Mutex to access rxBuffer and to give rxDataAvailableSemaphore
    SemaphoreHandle_t
        m_rxSomeDataAvailableSemaphore; ///< Sempaphore singaling if there is some new data available to be read.
                                        ///< Modified when some data arrives or is read from the buffer

    /**
     * @brief Buffer to queue outgoing data
     * Why is it needed in the first place? Because BLE message are sent as packages with MTU = 250 B.
     * For big packets (e.g. 1900 B), packet needs to be split into multiple transport buffers (exactly 8).
     * Without own queuing, we may send 7 buffer for transmission, and the 8th may fail.
     * There is no point in doing so, because we will sent part of a package, that will not be handled anyway.
     * It will also allow to group shorter packages together in one MTU.
     */
    CircularBuffer    m_txBuffer;        ///< Circular buffer queuing raw data to be transmitted to the PC
    SemaphoreHandle_t m_txBufferMutex;   ///< Mutex to access txBuffer
    SemaphoreHandle_t m_txDataAvailable; ///< Sempaphore singaling if there is any new data to be transmitted
                                         ///< (trasnmission is done in a separate thread)
    TaskHandle_t m_txLoopTaskHandle;     ///< Handle to Data Transmission (TX) task

    bool m_isSynced; ///< whether BLE controller/host are synced and communication can be started - TODO confirm it
    bool m_isClientConnected;                     ///< True if BLE client is connected (e.g. PC or Android app)
    SemaphoreHandle_t m_clientConnectedSemaphore; ///< Sempaphore given when the BLE client gets connected
};


extern BleuartDriver* g_pBleuartDriver; ///< global variable - pointing to the BLE driver


#endif // BLEUART_ESP_H
