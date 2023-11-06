/**
 * @file cobs.h
 * @brief Consistent Overhead Byte Stuffing (COBS) Consistent Overhead Byte Stuffing (COBS)
 is an algorithm for encoding data bytes that results in efficient, reliable,
 unambiguous packet framing regardless of packet content, thus making it easy
 for receiving applications to recover from malformed packets. It employs a particular byte value,
 typically zero, to serve as a packet delimiter (a special value that indicates the boundary between packets).
 When zero is used as a delimiter, the algorithm replaces each zero data byte with a non-zero value
 so that no zero data bytes will appear in the packet and thus be misinterpreted as packet boundaries.

 Code from:
 https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c

 */

#ifndef COBS_H
#define COBS_H


#include <stddef.h>
#include <stdint.h>


namespace cobs
{

/**
 * @brief Stuffs "length" bytes of data at the location pointed to by
 * "input", writing the output to the location pointed to by
 * "output". Returns the number of bytes written to "output".
 *
  * @param input - input buffer of data to be encoded
 * @param length - size of the input buffer data
 * @param output - output buffer which will contain encoded data
 * @return number of bytes in encoded (output) buffer
 */
size_t encode(const uint8_t* input, size_t length, uint8_t* output);

/**
 * @brief Unstuffs "length" bytes of data at the location pointed to by
 * "input", writing the output * to the location pointed to by
 * "output". Returns the number of bytes written to "output" if
 * "input" was successfully unstuffed, and 0 if there was an
 * error unstuffing "input".
 *
 * @param input - input buffer of encoded data
 * @param length - size of the input buffer data
 * @param output - output buffer which will contain decoded data
 * @return number of bytes in decoded (output) buffer
 */
size_t decode(const uint8_t* input, size_t length, uint8_t* output);

}  // namespace cobs

#endif /* COBS_H */
