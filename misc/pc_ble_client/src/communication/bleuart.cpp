#include "bleuart.h"

#include "defines.h"
#include "misc.h"

#include "blzlib_log.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string>


/* Service and characteristics UUID for the Nordic UART */
static const char* serviceUuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
static const char* write_characteristics_uuid = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
static const char* read_characteristics_uuid = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

constexpr uint32_t SLEEP_TIME_AFTER_WRITING_DATA_MS = 70;

#define BLEUART_RX_BUFFER_SIZE 8 * 1024 * 1024  ///< Size of the buffer to store the received data before processing them


uint8_t g_rxBufferData[BLEUART_RX_BUFFER_SIZE] = {0};


std::string dataArrayToHexStr(uint32_t maxSize, const uint8_t* pData, uint32_t dataLength)
{
    std::string txt;
    txt.reserve(dataLength * 3);

    for (uint32_t i = 0; i < dataLength; i++)
    {
        /* i use 5 here since we are going to add at most
           3 chars, need a space for the end '\n' and need
           a null terminator */
        // +3 bytes for "..."
        if (txt.size() + 8 < maxSize)
        {
            if (i > 0)
            {
                txt += ":";
            }
            char buff[16] = {};
            sprintf(buff, "%02X", pData[i]);
            txt += buff;
        }
        else
        {
            txt += "...";
            break;
        }

    }

    return txt;
}


BleuartDriver::BleuartDriver(const char* _adapterName, const char* _targetMac)
:
blzContext(NULL),
dev(NULL),
service(NULL),
writeCharacteristics(NULL),
readCharacteristic(NULL),
event(NULL),
sdbus(NULL),
adapterName(_adapterName),
targetMac(_targetMac),
isConnected(false),
isDisconnectedEventSet(false),
shouldStop(false),
rxBuffer(g_rxBufferData, sizeof(g_rxBufferData))
{
    if (pthread_mutex_init(&rxBufferMutex, NULL) != 0)
    {
        LOG_ERROR("mutex init failed");
    }
}

int BleuartDriver::signalHandler(sd_event_source* s, const struct signalfd_siginfo* si, void* context)
{
    BleuartDriver* pDriver = (BleuartDriver*) context;
    return pDriver->_signalHandler(s, si);
}

int BleuartDriver::_signalHandler(sd_event_source* s, const struct signalfd_siginfo* si)
{
    UNUSED(s);
    UNUSED(si);

    LOG_INFO("Received signal, shutting down bleuart...");

    /* detach sd-bus from event loop because we still need to use it later
     * to disconnect and finish */
    sd_bus_detach_event(sdbus);
    sd_event_exit(event, 0);
    shouldStop = true;
    return 0;
}

bool BleuartDriver::setupBlz()
{
    // Needs to be called in the same thread as further callbacks/notifications setup
    blzContext = blz_init(adapterName);
    if (!blzContext)
    {
        LOG_ERR("Initialization errror - blz_init failed");
        return false;
    }

    sd_event_add_signal(event, NULL, SIGTERM, BleuartDriver::signalHandler, this);
    sd_event_add_signal(event, NULL, SIGINT, BleuartDriver::signalHandler, this);

    return true;
}


void BleuartDriver::cleanupConnection()
{
    LOG_INF("bleuart_cleanupConnection started...");

    isConnected = false;

    sd_event_unref(event);
    sd_bus_unref(sdbus);

    blz_char_notify_stop(readCharacteristic);
    blz_char_free(readCharacteristic);
    blz_char_free(writeCharacteristics);
    blz_serv_free(service);
    blz_disconnect(dev);
    close(writeFileDescriptor);
}


void BleuartDriver::disconnectHandler(void* userData)
{
    BleuartDriver* pDriver = (BleuartDriver*) userData;
    pDriver->_disconnectHandler();
}

void BleuartDriver::_disconnectHandler()
{
    LOG_ERROR("Disconnected %s!", targetMac);
    isDisconnectedEventSet = true;
}

void BleuartDriver::notifyHandler(const uint8_t* pData, size_t dataLength, blz_char* ch, void* userData)
{
    BleuartDriver* pDriver = (BleuartDriver*) userData;
    pDriver->_notifyHandler(pData, dataLength, ch);
}

void BleuartDriver::_notifyHandler(const uint8_t* pData, size_t dataLength, blz_char* ch)
{
    UNUSED(ch);
    static uint32_t buffersReceived = 0;  // for debugging purposes
    buffersReceived++;

    std::string txt = dataArrayToHexStr(128, pData, (uint32_t) dataLength);
    LOG_DEBUG("Received %ld bytes:  %s", dataLength, txt.c_str());

    // for debug purposes
//    if (buffersReceived % 1 == 0 && dataLength >= 6)
//        LOG_DEBUG("Buffers received so far: %u. Data start string: %.6s", buffersReceived, pData);



    pthread_mutex_lock(&rxBufferMutex);
    if (!rxBuffer.writeData(pData, (uint32_t) dataLength))
    {
        LOG_ERROR("Discarding received data from bleuart - RX buffer overflow!");
    }
    pthread_mutex_unlock(&rxBufferMutex);
}

void BleuartDriver::deinit()
{
    LOG_INF("bleuart_deinit started...");
    cleanupConnection();
    blz_fini(blzContext);
    pthread_mutex_destroy(&rxBufferMutex);
}

bool BleuartDriver::connect()
{
    LOG_INFO("Connecting to %s...", targetMac);
    dev = blz_connect(blzContext, targetMac, BLZ_ADDR_PUBLIC);
    if (!dev)
    {
        LOG_ERROR("Connecting errror - blz_connect() failed");
        return false;
    }

    LOG_DEBUG("Obtaining service %s", serviceUuid);
    service = blz_get_serv_from_uuid(dev, serviceUuid);
    if (!service)
    {
        LOG_ERROR("Obtaining service errror - blz_get_serv_from_uuid() failed");
        return false;
    }

    LOG_DEBUG("Obtaining RX & TX characteristics");
    writeCharacteristics = blz_get_char_from_uuid(service, write_characteristics_uuid);
    readCharacteristic = blz_get_char_from_uuid(service, read_characteristics_uuid);

    if (!writeCharacteristics || !readCharacteristic)
    {
        LOG_ERROR("Nordic UART characteristics not found");
        return false;
    }

    LOG_DEBUG("Start to get notifications from read characteristic..");
    if (!blz_char_notify_start(readCharacteristic, BleuartDriver::notifyHandler, this))
    {
        LOG_ERROR("Failed to set notifications from read characteristic - blz_char_notify_start() failed");
        return false;
    }


    /* Get a file descriptor we can use to write to the write characteristic.
     * Writing to a fd is more efficient than repeatedly using
     * blz_char_write(wch, buffer, len); */
    writeFileDescriptor = blz_char_write_fd_acquire(writeCharacteristics);
    if (writeFileDescriptor < 0)
    {
        LOG_ERROR("Failed to set notifications from read characteristic");
        return false;
    }


    LOG_DEBUG("Connecting events and handlers...");
    sd_bus_default_system(&sdbus);
    sd_event_default(&event);
    sd_bus_attach_event(sdbus, event, SD_EVENT_PRIORITY_NORMAL);

    blz_set_disconnect_handler(dev, BleuartDriver::disconnectHandler, this);

    isDisconnectedEventSet = false;
    isConnected = true;

    LOG_INFO("bleuart_connect() - succefully connected!");
    return true;
}

void BleuartDriver::runLoop()
{
    LOG_DEBUG("Starting sd_event_run loop...");

    const uint32_t loopTimeoutUs = 100 * 1000;
    while (!isDisconnectedEventSet && !shouldStop)
    {
        sd_event_run(event, loopTimeoutUs);
    }

    LOG_DEBUG("Finished sd_event_run loop");
}

bool BleuartDriver::writeDataSingle(const uint8_t* pData, uint32_t dataLength)
{
    std::string txt = dataArrayToHexStr(128, pData, (uint32_t) dataLength);

    LOG_INFO("Transmitting %d bytes: %s", dataLength, txt.c_str());
    ssize_t bytesWritten = write(writeFileDescriptor, pData, dataLength);
    if (bytesWritten != dataLength)
    {
        LOG_ERROR("Failed to wrtie data to bleuart!");
        return false;
    }

    return true;
}

bool BleuartDriver::writeData(const uint8_t* pData, uint32_t dataLength)
{
    if (!isConnected)
    {
        LOG_ERROR("Cannot write data to bleuart - not connected!");
        return false;
    }

    std::string txt = dataArrayToHexStr(128, pData, (uint32_t) dataLength);

    // We cannot simply write the data with bleuart_writeDataSingle, as we need to use a smaller MTU size
    const uint32_t maxWriteBufferLength = 250;  // smaller than or equal to MTU size
    uint32_t dataIterator = 0;
    uint32_t remainingDataLength = dataLength;

    while (remainingDataLength > 0)
    {
        uint32_t chunkSize = (remainingDataLength > maxWriteBufferLength) ? maxWriteBufferLength : remainingDataLength;
        bool result = writeDataSingle(pData + dataIterator, chunkSize);
        if (!result)
            return false;

        remainingDataLength -= chunkSize;
        dataIterator += chunkSize;
        usleep(US_IN_MS*SLEEP_TIME_AFTER_WRITING_DATA_MS);
    }

    return true;
}

int BleuartDriver::readByte(bool blockUntilDataReady)
{

    int value = CIRCULAR_BUFFER__NO_DATA;

    do
    {
        pthread_mutex_lock(&rxBufferMutex);
        value = rxBuffer.readByte();
        pthread_mutex_unlock(&rxBufferMutex);

        if (value != CIRCULAR_BUFFER__NO_DATA)
            return value;

        if (!blockUntilDataReady)
            return value;

        usleep(100*1000); // just sleep, no mutex needed for this application performance
    } while (blockUntilDataReady);

    return value;
}




