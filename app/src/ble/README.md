

# Communication stack

1. BLE protocol - Nimble stack
2. BLE characteristic - as for Nordic UART
3. Serial communication wrapper - Nordic UART
4. Receive/transmit circular buffers
5. CBOS - Data encoding/decoding for frames with byte stuffing and recovery
6. <FUTURE VERSION> encryption - determine the best layer to apply encryption?
7. Protocol frames
8. Commands/responses handling

# Adding new commands

In order to add a new command, please follow this procedure:

- protocol_types.h 	- add command to `EPacketType` (`CMD_COMMAND_NAME = 0xXXXX`)
- protocol_types.h	- add response to `EPacketType` (`RES_COMMAND_NAME = CMD_COMMAND_NAME | RESULT_FLAG`)
- protocol_types.h	- add command_name namespace to protocol_types.h
	
	```
	namespace command_name
	{
		struct __attribute__((packed)) TCmd
		{
			
		};
		
		struct __attribute__((packed)) TRes
		{
		
		};
	 } // namespace command_name
	 ```
	 
- handle_command.cpp 	-> add case to switch in `handleCommand()` function
- handle_command.cpp	-> add `handleCommand_xxx()` definition in handle_command.cpp
	
	- start command with `LOG_INFO("Handling commmand 'xxx'...");`
	- check if payloadLen is correct (equal to sizeof `TCmd` structure for given command)
	- cast `pCmd` from the payload
	- execute steps for this command based on information from `pCmd` (if needed)
	- check if functions were executed correctly, send `NACK` if otherwise
	    (note: `NACK` should be returned if unexpected error happens. 
	    Unsuccessful result of the operation (e.g. due to wrong credentials given by user) 
	    should be reported in result structure (below)
	- create and fill out the `TRes` (result) structure (if needed)
	- send the response
	- check the return value from `sendPacket` function, send `NACK` if `return == false`
	
PC client application

- console_control.cpp -> add selected key as case in switch instruction

	- start with `LOG_INFO("About to send command CMD_COMMAND_NAME");`
	- create appropiate `TCmd` (`prot::command_name::TCmd`) and fill it out
	- send the command with `pBleController->sendPacket` function, assign the function result to `result` variable

- handle_response.cpp -> create function for handling response on the given command with following signature:
	
	`static EPacketHandlingResult handleResponse_commandName(BleController* pController, const uint8_t* payload, uint16_t payloadLen)`
	
- handle_response.cpp -> add case `prot::EPacketType::RES_COMMAND_NAME`
	- add execution of function `handleResponse_commandName` to that case
