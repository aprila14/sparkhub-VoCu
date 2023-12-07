static const char *LOG_TAG = "Protocol";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "protocol_control.h"

#include "cobs.h"
#include "defines.h"

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdint>

namespace prot
{

/** COBS stuffing adds 1 byte per 254 bytes */
#define COBS_STUFF_BYTES(x) (((x) / 254) + 1)
    /** Max payload in unstuffed packet:
     * - minus 4 for the packet header
     * - minus the stuffing bytes added by cobs
     * - minus 2 for the frame delimiters
     */
    constexpr int16_t MAX_PAYLOAD_LEN = (MAX_BLE_PACKET_SIZE - 4 - COBS_STUFF_BYTES(MAX_BLE_PACKET_SIZE) - 2);

    /** Frame delimiter. COBS stuffing allows using 0 as FD value. */
    constexpr uint8_t CBOS_FRAME_DELIMITER = 0x00;

    ProtocolControl::ProtocolControl()
        : m_inPacket{},
          m_inEncodedBytes{},
          m_outPacket{},
          m_outEncodedBytes{},
          m_packetInBytesCounter(0)
    {
        // probably will be set above in constructor, but as we port from C, then keep it for safety
        memset(m_inPacket.bytes, 0, sizeof(m_inPacket.bytes));
        memset(m_inEncodedBytes, 0, sizeof(m_inEncodedBytes));
        memset(m_outEncodedBytes, 0, sizeof(m_outEncodedBytes));
        memset(m_outPacket.bytes, 0, sizeof(m_outPacket.bytes));
    }

    uint16_t ProtocolControl::calculateCrc16(uint8_t *pData, uint16_t length)
    {
        uint16_t crc = 0xFFFF;

        while (length--)
        {
            uint8_t x = crc >> 8u ^ *pData++;                                                               // NOLINT works well, ignore the signess
            x ^= x >> 4u;                                                                                   // NOLINT works well, ignore the signess
            crc = (uint16_t)((crc << 8u) ^ ((uint16_t)(x << 12u)) ^ ((uint16_t)(x << 5u)) ^ ((uint16_t)x)); // NOLINT works well, ignore the signess
        }
        return crc;
    }

    const uint8_t *ProtocolControl::getOutEncodedBytes()
    {
        return m_outEncodedBytes;
    }

    const TPacketBuffer &ProtocolControl::getInPacket()
    {
        return m_inPacket;
    }

    int ProtocolControl::parsePacketToOutBuffer(uint16_t type, uint8_t *payload, uint16_t payloadLength)
    {
        TPacketHeader *header = &m_outPacket.packet.header;

        /* Sanity check */
        if (payloadLength > MAX_PAYLOAD_LEN)
        {
            LOG_DEBUG("Outgoing packet payload (%d) too big, max length: %d", payloadLength, MAX_PAYLOAD_LEN);
            return NO_PACKET_ASSEMBLED;
        }

        LOG_DEBUG("Sending packet tpye: %04X, payload length: %u", type, payloadLength);

        /* Assemble header */
        header->type = type;
        header->crc = 0;

        /* Copy payload */
        if (payload && payloadLength)
            memcpy(m_outPacket.packet.payload, payload, payloadLength);

        /* Calculate CRC */
        header->crc = calculateCrc16(m_outPacket.bytes, sizeof(TPacketHeader) + payloadLength);

        /* COBS stuff header, copying it to output buffer */
        size_t cobslen = cobs::encode(
            m_outPacket.bytes,
            sizeof(TPacketHeader) + payloadLength,
            m_outEncodedBytes + 1); // +1 for delimiter

        /* Add starting Frame Delimiter */
        m_outEncodedBytes[0] = CBOS_FRAME_DELIMITER;

        /* Add ending Frame Delimiter */
        m_outEncodedBytes[1 + cobslen] = CBOS_FRAME_DELIMITER;

        int bytesToSend = (int)cobslen + 2; // 2 for delimeters
        return bytesToSend;
    }

    /** Handle a possibly valid packet:
     * - Unstuff
     * - Check payload length
     * - Check CRC
     * @return NO_PACKET_ASSEMBLED (-1) if packet is invalid, >0 is the unstuffed length of valid packet
     */
    int ProtocolControl::decodeAndCheckReceivedPacket()
    {
        TPacketHeader *header = &m_inPacket.packet.header;
        uint16_t crc_header = 0;
        uint16_t crc_calculated = 0;

        size_t ret = cobs::decode(m_inEncodedBytes, m_packetInBytesCounter, m_inPacket.bytes);
        DEBUG_ASSERT(ret <= UINT16_MAX);

        if (ret == 0)
        {
            LOG_DEBUG("Error unstuffing packet");
            return NO_PACKET_ASSEMBLED;
        }

        if (ret > m_packetInBytesCounter)
        {
            LOG_DEBUG("Unstuffed packet is longer than input: %u, %d", (uint16_t)ret, m_packetInBytesCounter);
            return NO_PACKET_ASSEMBLED;
        }

        /* Set the CRC in the header to 0 while checking, as it was when calculating the CRC */
        crc_header = header->crc;
        header->crc = 0;

        crc_calculated = calculateCrc16(m_inPacket.bytes, (uint16_t)ret);

        if (crc_header == crc_calculated)
            return (int)ret;

        LOG_DEBUG("CRC error: header: %04X, calc: %04X", crc_header, crc_calculated);
        return NO_PACKET_ASSEMBLED;
    }

    int ProtocolControl::processData(uint8_t inbyte)
    {
        /* Frame delimiter received, check buffer for a valid packet */
        if (inbyte == CBOS_FRAME_DELIMITER)
        {
            /* This is only the beginning of a packet, continue receiving */
            if (m_packetInBytesCounter == 0)
                return NO_PACKET_ASSEMBLED;

            /* we already have some data in the buffer */
            /* A packet can't be smaller than the header */
            if (m_packetInBytesCounter < sizeof(TPacketHeader))
            {
                LOG_DEBUG("Broken packet received");
                m_packetInBytesCounter = 0;
                return NO_PACKET_ASSEMBLED;
            }

            int packetLength = decodeAndCheckReceivedPacket();
            if (packetLength == NO_PACKET_ASSEMBLED)
            {
                LOG_DEBUG("Invalid packet received");
                m_packetInBytesCounter = 0;
                return NO_PACKET_ASSEMBLED;
            }

            LOG_DEBUG("Package assembled");
            int payloadLength = packetLength - (int)sizeof(TPacketHeader);
            m_packetInBytesCounter = 0;
            return payloadLength;
        }

        /* Receive while there is enough space in the buffer */
        if (m_packetInBytesCounter < sizeof(m_inEncodedBytes))
        {
            m_inEncodedBytes[m_packetInBytesCounter] = inbyte;
            m_packetInBytesCounter++;
        }
        else
        {
            LOG_DEBUG("Oversized packet received");
            m_packetInBytesCounter = 0;
        }

        return NO_PACKET_ASSEMBLED;
    }

} // namespace prot
