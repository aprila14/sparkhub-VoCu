/**
 * @file bleuart.h
 * @brief Provides BLE handling functionality.
 * One should first create a driver (instance of TBleuartDriver), and then pass it to functions
 */

#ifndef BLE_UART_H
#define BLE_UART_H


#include "circular_buffer.h"

#include "blzlib.h"

#include <systemd/sd-bus.h>
#include <atomic>


/**
 * @brief Bluetooth driver
 * All data used to control the BLE communication grouped into a driver. Only one instance should be created.
 * Passed to all BLE handling functions
 */
class BleuartDriver
{
public:
    BleuartDriver() = delete;

    /**
     * @brief Initialize the BLE driver.
     * @param adapterName
     * @param targetMac
     * @return true on success, false otherwise
     */
    BleuartDriver(const char* _adapterName, const char* _targetMac);

    /**
     * @brief Setup the BLZ module
     * Should be called in the same thread as other connect and cleanup functions
     * @return true on success, false otherwise
     */
    bool setupBlz();

    /**
     * @brief  Deinitialize the BLE driver
     * Should be called in the same thread as other connect and cleanup functions
     */
    void deinit();

    /**
     * @brief Cleanup last connection, after the device is disconnected. Required before starting a new connection
     * Should be called in the same thread as other connect and cleanup functions
     */
    void cleanupConnection();

    /**
     * @brief Connect to the target BLE peripheral
     * Should be called in the same thread as other connect and cleanup functions
     * @return
     */
    bool connect();

    /**
     * @brief Run loop to handle BLE events until shouldStop flag is set
     */
    void runLoop();

    /**
     * @brief Write data to BLE - transmit it to the peripheral
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
    int readByte(bool blockUntilDataReady);

    static int signalHandler(sd_event_source* s, const struct signalfd_siginfo* si, void* context);
    int _signalHandler(sd_event_source* s, const struct signalfd_siginfo* si);

    /**
     * @brief To be called when the BLE peripheral is disconnected (e.g. due to reset)
     * @param userData Handle to the TBleuartDriver
     */
    static void disconnectHandler(void* userData);
    void _disconnectHandler();

    /**
     * @brief To be called when new data is received from the BLE peripheral as notification
     * @param pData Received data array
     * @param dataLength Size of pData
     * @param ch
     * @param userData Handle to the TBleuartDriver
     */
    static void notifyHandler(const uint8_t* pData, size_t dataLength, blz_char* ch, void* userData);
    void _notifyHandler(const uint8_t* pData, size_t dataLength, blz_char* ch);


    blz_ctx* blzContext;  ///<  blz context
    blz_dev* dev;  ///<  device we connect to
    blz_serv* service;  ///<  NUS service
    blz_char* writeCharacteristics;  ///<  characteristic to write to
    blz_char* readCharacteristic;  ///<  characteristic we read from

    const char* adapterName;   ///<  name of the BLE adapter in Linux system
    const char* targetMac;   ///<  BLuetooth MAC address of the device we want to connect to (ESP32 MAC is printed at startup)
    int writeFileDescriptor;

    sd_event* event;  ///< event to be used for sdbus interface
    sd_bus* sdbus;  ///<  sd-bus object for sd-event


    pthread_mutex_t rxBufferMutex;  ///< To protect the rxBuffer from concurrent access
    CircularBuffer rxBuffer;  ///< Circular buffer storing the received data

    std::atomic_bool isConnected;  ///< true if BLE device is connected
    std::atomic_bool isDisconnectedEventSet;  ///< true if device was connected, but disconnected after some time. May be used to reset logic state machine in the controller
    std::atomic_bool shouldStop;  ///<  flag to stop the thread


private:
    /**
     * @brief Write data to BLE - transmit it to the peripheral
     * @param pData Data to write buffer
     * @param dataLength Length of the data to write
     * @param pDriver - BLE driver - instance of TBleuartDriver
     * @return true on success, false otherwise
     */
    bool writeDataSingle(const uint8_t* pData, uint32_t dataLength);
};


#endif // BLE_UART_H
