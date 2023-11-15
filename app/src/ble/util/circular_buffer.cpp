static const char* LOG_TAG = "CircularBuffer";
#define LOG_LOCAL_LEVEL ESP_LOG_WARN

#include "circular_buffer.h"

#include "defines.h"

#include <cassert>
#include <cstddef>


CircularBuffer::CircularBuffer(uint8_t* pData, uint32_t size)
:
m_dataSize(size),
m_readIndex(0),
m_writeIndex(0),
m_pData(pData)
{
    assert(pData != NULL);
}

bool CircularBuffer::isDataAvailable() const
{
    if (m_readIndex == m_writeIndex)
    {
        return false;
    }

    return true;
}

int CircularBuffer::readByte()
{
    if (m_readIndex == m_writeIndex)
    {
        return CIRCULAR_BUFFER__NO_DATA;
    }
    else
    {
        uint8_t byte = m_pData[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_dataSize;
        return byte;
    }
}

bool CircularBuffer::writeByte(uint8_t byte)
{
    uint32_t newWriteIndex = (m_writeIndex + 1) % m_dataSize;
    if (m_readIndex == newWriteIndex)
    {
        return false;
    }

    m_pData[m_writeIndex] = byte;
    m_writeIndex = newWriteIndex;

    return true;
}

bool CircularBuffer::writeData(const uint8_t* pData, uint32_t dataLength)
{
    uint32_t remainingSize = getFreeSize();

    if (dataLength > remainingSize)
        return false;

    for (uint32_t i = 0; i < dataLength; i++)
    {
        bool result = writeByte(pData[i]);
        assert(result);
    }

    return true;
}

uint32_t CircularBuffer::getFreeSize() const
{
    uint32_t end = m_readIndex;
    uint32_t start = m_writeIndex;
    uint32_t bufferSize = m_dataSize;
    uint32_t remainingSize = (end > start) ? (end - start - 1) : (bufferSize - start + end - 1);

    return remainingSize;
}
