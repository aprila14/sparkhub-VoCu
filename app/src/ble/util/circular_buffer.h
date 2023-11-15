#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stdint.h>


constexpr int32_t CIRCULAR_BUFFER__NO_DATA = -1;


/**
 * @brief Class representing a circular buffer. [https://en.wikipedia.org/wiki/Circular_buffer]
 * Not thread safe
 */
class CircularBuffer
{
public:
    /**
     * @brief Initialize the circular buffer structure
     * @param pData Flat buffer for the data storage
     * @param size Size of the pBuffer
     */
    CircularBuffer(uint8_t* pData, uint32_t size);

    /**
     * @brief Read (extract) a single byte from the buffer circular buffer
     * @return Byte read on success. If there is no new data - CIRCULAR_BUFFER__NO_DATA (-1)
     */
    int readByte();

    /**
     * @brief Write (append, insert) a new byte to the circular buffer.
     * If buffer is full, new data want be appended.
     * @param byte Byte to insert into the buffer
     * @return True on success, false otherwise. Operation may fail due to full buffer
     */
    bool writeByte(uint8_t byte);

    /**
     * @brief Write (append, insert) entire array of data to the circular buffer.
     * If there is not enough space, nothing will be added to the buffer
     * @param pData Array of data to append
     * @param dataLength Length of the pData array
     * @return True on success, false otherwise. Operation may fail due to full buffer
     */
    bool writeData(const uint8_t* pData, uint32_t dataLength);

    /**
     * @brief Check if some any data is available in the buffer
     * @return True if some data availabale, false otherwise
     */
    bool isDataAvailable() const;


    /**
     * @brief Get size of free bytes in buffer (available space)
     * @return Number of bytes
     */
    uint32_t getFreeSize() const;


private:
    uint32_t m_dataSize;  ///< Size of the buffer

    uint32_t m_readIndex;  ///< Index from where extract new data on read. aka head
    uint32_t m_writeIndex;  ///< Index where to insert data on write. aka tail

    uint8_t* m_pData;  ///< flat buffer for the data

    CircularBuffer() = delete;
} ;


#endif  // CIRCULAR_BUFFER_H
