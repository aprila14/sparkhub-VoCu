/**
 * @file handle_response.h
 * @brief Provides functionality to handle received response packets
 */

#ifndef HANDLE_RESPONSE_H
#define HANDLE_RESPONSE_H


#include "ble_controller.h"
#include "defines.h"


/**
 * @brief Handle received packet which is a response for a command
 * @param pController Handle to the controller instance
 * @param packetType Type of packet to handle
 * @param payload Buffer with packet payload
 * @param payloadLen Size of the payload
 * @return
 */
EPacketHandlingResult
handleResponse(BleController* pController, prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen);


#endif // HANDLE_RESPONSE_H
