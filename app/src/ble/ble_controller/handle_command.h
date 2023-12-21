/**
 * @file handle_command.h
 * @brief Provides functionality to handle received commands
 */


#ifndef HANDLE_COMMAND_H
#define HANDLE_COMMAND_H


#include "ble_controller.h"
#include "defines.h"


/**
 * @brief Handle received packet which is a acommand
 * @param pController Handle to the controller instance
 * @param packetType Type of packet to handle
 * @param payload Buffer with packet payload
 * @param payloadLen Size of the payload
 * @return
 */
EPacketHandlingResult
handleCommand(BleController* pController, prot::EPacketType packetType, const uint8_t* payload, uint16_t payloadLen);


#endif // HANDLE_COMMAND_H
