#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H


#include "defines.h"

#include "protocol_control.h"
#include "ble_controller.h"

/**
 * @brief Take appropriate action for a pressed key
 * @param pController Handle to the controller instance
 * @param key Character pressedon the keyboard
 */
void handleKeyPress(BleController* pBleController, char key);


#endif // CONSOLE_CONTROL_H
