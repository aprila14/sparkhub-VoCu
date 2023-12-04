/**
 * @file protocol.h
 * @brief Defines the communication protocol: packets, flags, parsing
 */


#ifndef PROTOCOL_H_
#define PROTOCOL_H_


#include "protocol_types.h"

#include <cstddef>
#include <cstdint>


namespace prot
{

constexpr int32_t NO_PACKET_ASSEMBLED = -1;



/**
 * Structure describing the packet header, or a packet without payload.
 */
struct  __attribute__((packed)) TPacketHeader
{
    uint16_t type; /**< Type of the packet */
    uint16_t crc; /**< CRC of the packet, including header, set to 0 during calculation */
};

/**
 * @brief Represents a single packet.
 * Payload length is variable, no need to specify length, as COBS encoding is used with delimiter
 */
struct __attribute__((packed)) TPacket
{
    TPacketHeader header;
	uint8_t payload[0];
};

/**
 * @brief Used for conveniened access to packet bytes and vice versa
 */
union TPacketBuffer
{
    TPacket packet;
    uint8_t bytes[MAX_BLE_PACKET_SIZE];
};


/**
 * @brief Control class gathering all variables for the protocol packet parsing
 */
class ProtocolControl
{
public:
    ProtocolControl();

    /** Assemble a packet and prepare it to send in buffer
     *
     * @param type Packet type
     * @param plen Payload length
     * @param payload Pointer to payload buffer
     * @return Number of packet bytes to send in raw output buffer
     */
    int parsePacketToOutBuffer(uint16_t type, uint8_t* payload, uint16_t payloadLength);


    /** Receive incoming data, parse them to packets.
     * Reads byte-by-byte, and notifies if a full and valid packet is received.
     * Checks any data between two frame delimiters if it is a valid packet.
     * Packets with CRC errors, or oversized payload indication are discarded.
     * @param inbyte An input byte from the incoming stream
     * @return Assembled packet payload length (when packet assembled) or NO_PACKET_ASSEMBLED otherwise
     */
    int processData(uint8_t inbyte);


    /** Calculates a 16-bit CRC, the "CRC-CCITT (0xFFFF)" variant
     * @param data_p Data pointer
     * @param length Length of data
     * @return 16-bit CRC value
     */
    static uint16_t calculateCrc16(uint8_t* pData, uint16_t length);

    const uint8_t* getOutEncodedBytes();
    const TPacketBuffer& getInPacket();

private:
    int decodeAndCheckReceivedPacket();


    TPacketBuffer m_inPacket;  ///< received packet data after decoding
    uint8_t m_inEncodedBytes[MAX_BLE_PACKET_SIZE];  ///< All bytes received, before decoding

    // TODO - get rid of output buffers - pass output buffer to packet parsing function from higher level (consider it?)
    TPacketBuffer m_outPacket;  ///< packet data to be sent, before encoding
    uint8_t m_outEncodedBytes[MAX_BLE_PACKET_SIZE + 2];  ///< All bytes to be sent, after encoding, with delimiters. +2 for the delimiting zeros

    uint32_t m_packetInBytesCounter;
};


}  // namespace prot

#endif /* PROTOCOL_H_ */
