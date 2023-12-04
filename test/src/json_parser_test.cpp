
#include "defines.h"

#include "test_utils.h"
#include "custom_types.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "json_parser.h"

#include "cloud_controller.h"

#include "cJSON.h"

TEST(CloudController, processStatusReportJsonSimple)
{
    //Preparation of example JSON

    cJSON* deviceStatus = cJSON_CreateObject();

    cJSON* wifiConnectionState = cJSON_CreateBool(false);
    cJSON* bleConnectionState = cJSON_CreateBool(true);
    cJSON* lightIntensityLevel = cJSON_CreateNumber(35);
    cJSON* currentTimeMs = cJSON_CreateNumber(35467);
    cJSON* firmwareVersion = cJSON_CreateString("2.3.4");

    cJSON_AddItemToObject(deviceStatus, "wifiConnectionState", wifiConnectionState);
    cJSON_AddItemToObject(deviceStatus, "bleConnectionState", bleConnectionState);
    cJSON_AddItemToObject(deviceStatus, "lightIntensityLevel", lightIntensityLevel);
    cJSON_AddItemToObject(deviceStatus, "currentTimeMs", currentTimeMs);
    cJSON_AddItemToObject(deviceStatus, "firmwareVersion", firmwareVersion);

    json_parser::TDeviceStatus deviceStatusStruct = {};

    bool result = json_parser::processStatusReport(deviceStatus, &deviceStatusStruct);

    ASSERT_TRUE(result);
    ASSERT_EQ(deviceStatusStruct.currentTimeFromStartupMs, 35467);
    ASSERT_STREQ(deviceStatusStruct.firmwareVersion, "2.3.4");
    ASSERT_EQ(deviceStatusStruct.isBleConnected, true);
    ASSERT_EQ(deviceStatusStruct.isWiFiConnected, false);
    ASSERT_EQ(deviceStatusStruct.lightIntensityLevel, 35);

    cJSON_Delete(deviceStatus);
}

TEST(CloudController, processGetLightIntensityLevelSimple)
{
    //Preparation of example JSON

    cJSON* dataJson = cJSON_CreateObject();
    cJSON* lightIntensityLevel = cJSON_CreateNumber(45);

    cJSON_AddItemToObject(dataJson, "lightIntensityLevel", lightIntensityLevel);

    json_parser::TSetLightLevel lightLevelStruct = {};

    bool result = json_parser::processGetLightIntensityLevel(dataJson, &lightLevelStruct);

    ASSERT_TRUE(result);
    ASSERT_EQ(lightLevelStruct.lightIntensityLevel, 45);

    cJSON_Delete(dataJson);
}

TEST(CloudController, processGetLightIntensityLevelNullptr)
{
    //Preparation of example faulty JSON

    cJSON* dataJson = cJSON_CreateObject();
    cJSON* lightIntensityLevel = nullptr;

    cJSON_AddItemToObject(dataJson, "lightIntensityLevel", lightIntensityLevel);

    json_parser::TSetLightLevel lightLevelStruct = {};

    bool result = json_parser::processGetLightIntensityLevel(dataJson, &lightLevelStruct);

    ASSERT_FALSE(result);

    cJSON_Delete(dataJson);
}

TEST(CloudController, processResponseSimple)
{
    //Preparation of example JSON

    cJSON* dataJson = cJSON_CreateObject();
    cJSON* ACK = cJSON_CreateBool(true);

    cJSON_AddItemToObject(dataJson, "ACK", ACK);

    json_parser::TResponse responseStructure = {};

    bool result = json_parser::processResponse(dataJson, &responseStructure);

    ASSERT_TRUE(result);
    ASSERT_EQ(responseStructure.ACK, true);

    cJSON_Delete(dataJson);
}

TEST(CloudController, processResponseNullptr)
{
    //Preparation of example JSON

    cJSON* dataJson = cJSON_CreateObject();
    cJSON* ACK = nullptr;

    cJSON_AddItemToObject(dataJson, "ACK", ACK);

    json_parser::TResponse responseStructure = {};

    bool result = json_parser::processResponse(dataJson, &responseStructure);

    ASSERT_FALSE(result);

    cJSON_Delete(dataJson);
}

TEST(CloudController, preprocessingInputMessageSimple)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":3,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":50}}}");

    cJSON* inputMessageJson = json_parser::preprocessInputMessage(inputMessage);

    cJSON* frameJson = cJSON_GetObjectItemCaseSensitive(inputMessageJson, "params");
    cJSON* msgCodeJson = cJSON_GetObjectItemCaseSensitive(frameJson, "msgCode");
    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(frameJson, "msgCounter");

    ASSERT_NE(frameJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);

    uint8_t msgCode = msgCodeJson->valueint;
    uint32_t msgCounter = msgCounterJson ->valueint;

    ASSERT_EQ(msgCode, 1);
    ASSERT_EQ(msgCounter, 3);

    cJSON_Delete(inputMessageJson);
}

TEST(CloudController, extractingMsgMethodRpcCommand)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":3,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":50}}}");

    json_parser::EMsgMethod msgMethod = json_parser::extractMsgMethod(inputMessage);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND);
}

TEST(CloudController, extractingMsgMethodUnknown)
{
    std::string inputMessage = std::string("{\"method\":\"totallyWrongCommand\",\"params\":{\"msgCounter\":3,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":50}}}");

    json_parser::EMsgMethod msgMethod = json_parser::extractMsgMethod(inputMessage);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_UNKNOWN);
}

TEST(CloudController, extractingMsgMethodFailed)
{
    std::string inputMessage = std::string("{\"methodWithATypo\":\"rpcCommand\",\"params\":{\"msgCounter\":3,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":50}}}");

    json_parser::EMsgMethod msgMethod = json_parser::extractMsgMethod(inputMessage);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_FAILED);
}

TEST(CloudController, getDataJsonAndInitFrame)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":3,\"msgCode\":4,\"data\":{\"lightIntensityLevel\":67}}}");

    json_parser::TFrame frame = {};
    cJSON* dataJson = nullptr;

    bool result = json_parser::getDataJsonAndInitFrame(inputMessage, &frame, &dataJson);

    ASSERT_TRUE(result);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_STATUS_REPORT_RESPONSE);
    ASSERT_EQ(frame.msgCounter, 3);

    cJSON* lightIntensityLevel = cJSON_GetObjectItemCaseSensitive(dataJson, "lightIntensityLevel");

    frame.frameData.setLightLevelStruct.lightIntensityLevel = lightIntensityLevel->valueint;

    ASSERT_EQ(frame.frameData.setLightLevelStruct.lightIntensityLevel, 67);
    cJSON_Delete(dataJson);
}

TEST(CloudController, extractMethodAndFillFrameRpcLightIntenstity)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":37,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":67}}}");
    json_parser::TFrame frame = {};

    json_parser::EMsgMethod msgMethod = json_parser::extractMethodAndFillFrame(inputMessage, &frame);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL);
    ASSERT_EQ(frame.msgCounter, 37);

    ASSERT_EQ(frame.frameData.setLightLevelStruct.lightIntensityLevel, 67);
}

TEST(CloudController, extractMethodAndFillFrameRpcStatus)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":123,\"msgCode\":3,\"data\":{\"wifiConnectionState\":true,\"bleConnectionState\":false,\"lightIntensityLevel\":23,\"currentTimeMs\":54321,\"firmwareVersion\":\"0.8.0\"}}}");
    json_parser::TFrame frame = {};

    json_parser::EMsgMethod msgMethod = json_parser::extractMethodAndFillFrame(inputMessage, &frame);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_STATUS_REPORT);
    ASSERT_EQ(frame.msgCounter, 123);

    ASSERT_EQ(frame.frameData.deviceStatusStruct.isWiFiConnected, true);
    ASSERT_EQ(frame.frameData.deviceStatusStruct.isBleConnected, false);
    ASSERT_EQ(frame.frameData.deviceStatusStruct.lightIntensityLevel, 23);
    ASSERT_EQ(frame.frameData.deviceStatusStruct.currentTimeFromStartupMs, 54321);
    ASSERT_STREQ(frame.frameData.deviceStatusStruct.getFirmwareVersion().c_str(), "0.8.0");
}

TEST(CloudController, extractMethodAndFillFrameRpcStatusResponse)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":287,\"msgCode\":4,\"data\":{\"ACK\":true}}}");
    json_parser::TFrame frame = {};

    json_parser::EMsgMethod msgMethod = json_parser::extractMethodAndFillFrame(inputMessage, &frame);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_STATUS_REPORT_RESPONSE);
    ASSERT_EQ(frame.msgCounter, 287);
    ASSERT_EQ(frame.frameData.responseStruct.ACK, true);
}


TEST(CloudController, extractMethodAndFillFrameRpcHeartbeatResponse)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":234,\"msgCode\":6,\"data\":{\"ACK\":false}}}");
    json_parser::TFrame frame = {};

    json_parser::EMsgMethod msgMethod = json_parser::extractMethodAndFillFrame(inputMessage, &frame);

    ASSERT_EQ(msgMethod, json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_HEARTBEAT_RESPONSE);
    ASSERT_EQ(frame.msgCounter, 234);
    ASSERT_EQ(frame.frameData.responseStruct.ACK, false);
}

TEST(CloudController, parseJsonRpcCommandSetLightIntensity)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":0,\"msgCode\":1,\"data\":{\"lightIntensityLevel\":150}}}");
    json_parser::TFrame frame = {};

    bool result = json_parser::parseJsonRpcCommand(inputMessage, &frame);

    ASSERT_TRUE(result);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL);
    ASSERT_EQ(frame.msgCounter, 0);
    ASSERT_EQ(frame.frameData.setLightLevelStruct.lightIntensityLevel,150);
}

TEST(CloudController, parseJsonRpcCommandStatusResponse)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":65535,\"msgCode\":4,\"data\":{\"ACK\":false}}}");
    json_parser::TFrame frame = {};

    bool result = json_parser::parseJsonRpcCommand(inputMessage, &frame);

    ASSERT_TRUE(result);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_STATUS_REPORT_RESPONSE);
    ASSERT_EQ(frame.msgCounter, 65535);
    ASSERT_EQ(frame.frameData.responseStruct.ACK, false);
}

TEST(CloudController, parseJsonRpcCommandHeartbeatResponse)
{
    std::string inputMessage = std::string("{\"method\":\"rpcCommand\",\"params\":{\"msgCounter\":365,\"msgCode\":6,\"data\":{\"ACK\":true}}}");
    json_parser::TFrame frame = {};

    bool result = json_parser::parseJsonRpcCommand(inputMessage, &frame);

    ASSERT_TRUE(result);
    ASSERT_EQ(frame.msgCode, json_parser::EMsgCode::MSG_HEARTBEAT_RESPONSE);
    ASSERT_EQ(frame.msgCounter, 365);
    ASSERT_EQ(frame.frameData.responseStruct.ACK, true);
}

TEST(CloudController, prepareHeartbeatMessage)
{
    std::string heartbeatMessage = json_parser::prepareHeartbeatMessage(245);

    cJSON* heartbeatMessageJson = cJSON_Parse(heartbeatMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(heartbeatMessageJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(heartbeatMessageJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");
    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* heartbeat = cJSON_GetObjectItemCaseSensitive(dataJson, "heartbeat");

    ASSERT_NE(heartbeatMessageJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(heartbeat, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 245);
    ASSERT_EQ(msgCodeJson->valueint, 5);

    ASSERT_TRUE(cJSON_IsTrue(heartbeat));

    cJSON_Delete(heartbeatMessageJson);
}

TEST(CloudController, prepareHeartbeatMessageMsgCounterZero)
{
    std::string heartbeatMessage = json_parser::prepareHeartbeatMessage(0);

    cJSON* heartbeatMessageJson = cJSON_Parse(heartbeatMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(heartbeatMessageJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(heartbeatMessageJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");
    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* heartbeat = cJSON_GetObjectItemCaseSensitive(dataJson, "heartbeat");

    ASSERT_NE(heartbeatMessageJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(heartbeat, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 0);
    ASSERT_EQ(msgCodeJson->valueint, 5);

    ASSERT_TRUE(cJSON_IsTrue(heartbeat));

    cJSON_Delete(heartbeatMessageJson);
}

TEST(CloudController, prepareDeviceStatusMessage)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs =23123;
    strcpy(deviceStatusStruct.firmwareVersion, "3.4.5");
    strcpy(deviceStatusStruct.currentLocalTime, "Wed May 04 12:34:45 2022");
    deviceStatusStruct.isBleConnected = true;
    deviceStatusStruct.isWiFiConnected = false;
    deviceStatusStruct.lightIntensityLevel = 255;

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatusStruct, 567);

    cJSON* deviceStatusJson = cJSON_Parse(deviceStatusMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");

    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(dataJson, "wifiConnectionState");
    cJSON* bleConnectionStateJson = cJSON_GetObjectItemCaseSensitive(dataJson, "bleConnectionState");
    cJSON* lightIntensityLevelJson = cJSON_GetObjectItemCaseSensitive(dataJson, "lightIntensityLevel");
    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson = cJSON_GetObjectItemCaseSensitive(dataJson, "firmwareVersion");
    cJSON* currentLocalTimeJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(lightIntensityLevelJson, nullptr);
    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 567);
    ASSERT_EQ(msgCodeJson->valueint, 3);

    ASSERT_TRUE(cJSON_IsTrue(bleConnectionStateJson));
    ASSERT_FALSE(cJSON_IsTrue(wifiConnectionStateJson));
    ASSERT_EQ(lightIntensityLevelJson->valueint, 255);
    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 23123);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "3.4.5");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Wed May 04 12:34:45 2022");

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, prepareDeviceStatusMessageDifferentValues)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs =34234;
    strcpy(deviceStatusStruct.firmwareVersion, "5.4.3");
    strcpy(deviceStatusStruct.currentLocalTime, "Fri May 06 09:57:12 2022");

    deviceStatusStruct.isBleConnected = false;
    deviceStatusStruct.isWiFiConnected = true;
    deviceStatusStruct.lightIntensityLevel = 0;

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatusStruct, 3254);

    cJSON* deviceStatusJson = cJSON_Parse(deviceStatusMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");

    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(dataJson, "wifiConnectionState");
    cJSON* bleConnectionStateJson = cJSON_GetObjectItemCaseSensitive(dataJson, "bleConnectionState");
    cJSON* lightIntensityLevelJson = cJSON_GetObjectItemCaseSensitive(dataJson, "lightIntensityLevel");
    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson = cJSON_GetObjectItemCaseSensitive(dataJson, "firmwareVersion");
    cJSON* currentLocalTimeJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(lightIntensityLevelJson, nullptr);
    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 3254);
    ASSERT_EQ(msgCodeJson->valueint, 3);

    ASSERT_FALSE(cJSON_IsTrue(bleConnectionStateJson));
    ASSERT_TRUE(cJSON_IsTrue(wifiConnectionStateJson));
    ASSERT_EQ(lightIntensityLevelJson->valueint, 0);
    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 34234);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "5.4.3");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Fri May 06 09:57:12 2022");

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, deviceStatusToJson)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs = 65321;
    strcpy(deviceStatusStruct.firmwareVersion, "1.2.8");
    strcpy(deviceStatusStruct.currentLocalTime, "Fri May 31 11:11:11 2024");
    deviceStatusStruct.isBleConnected = true;
    deviceStatusStruct.isWiFiConnected = true;
    deviceStatusStruct.lightIntensityLevel = 123;

    cJSON* deviceStatusJson = json_parser::deviceStatusToJson(deviceStatusStruct);

    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "firmwareVersion");
    cJSON* bleConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "bleConnectionState");
    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "wifiConnectionState");
    cJSON* lightIntensityJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "lightIntensityLevel");
    cJSON* currentLocalTimeJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);

    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(lightIntensityJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 65321);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "1.2.8");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Fri May 31 11:11:11 2024");
    ASSERT_EQ(cJSON_IsTrue(bleConnectionStateJson), true);
    ASSERT_EQ(cJSON_IsTrue(wifiConnectionStateJson), true);
    ASSERT_EQ(lightIntensityJson->valueint, 123);

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, deviceStatusToJsonDifferentValues)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs = 0;
    strcpy(deviceStatusStruct.firmwareVersion, "1.9.8");
    strcpy(deviceStatusStruct.currentLocalTime, "Sat Jan 02 22:22:22 2048");
    deviceStatusStruct.isBleConnected = false;
    deviceStatusStruct.isWiFiConnected = false;
    deviceStatusStruct.lightIntensityLevel = 249;

    cJSON* deviceStatusJson = json_parser::deviceStatusToJson(deviceStatusStruct);

    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "firmwareVersion");
    cJSON* bleConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "bleConnectionState");
    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "wifiConnectionState");
    cJSON* lightIntensityJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "lightIntensityLevel");
    cJSON* currentLocalTimeJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);

    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(lightIntensityJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 0);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "1.9.8");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Sat Jan 02 22:22:22 2048");
    ASSERT_EQ(cJSON_IsTrue(bleConnectionStateJson), false);
    ASSERT_EQ(cJSON_IsTrue(wifiConnectionStateJson), false);
    ASSERT_EQ(lightIntensityJson->valueint, 249);

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, heartbeatToJson)
{
    json_parser::THeartbeat heartbeatStruct = {};
    heartbeatStruct.heartbeat = true;

    cJSON* heartbeatJson = json_parser::heartbeatToJson(heartbeatStruct);

    cJSON* heartbeat =cJSON_GetObjectItemCaseSensitive(heartbeatJson, "heartbeat");

    ASSERT_NE(heartbeatJson, nullptr);
    ASSERT_EQ(cJSON_IsTrue(heartbeat), true);

    cJSON_Delete(heartbeatJson);
}

TEST(CloudController, heartbeatToJsonDifferentValue)
{
    json_parser::THeartbeat heartbeatStruct = {};
    heartbeatStruct.heartbeat = false;

    cJSON* heartbeatJson = json_parser::heartbeatToJson(heartbeatStruct);

    cJSON* heartbeat =cJSON_GetObjectItemCaseSensitive(heartbeatJson, "heartbeat");

    ASSERT_NE(heartbeatJson, nullptr);
    ASSERT_EQ(cJSON_IsTrue(heartbeat), false);

    cJSON_Delete(heartbeatJson);
}

TEST(CloudController, widgetGetLightLevelToJson)
{
    json_parser::TSetLightLevel getLightLevelStruct = {};

    getLightLevelStruct.lightIntensityLevel = 38;

    cJSON* getLightLevelJson = json_parser::widgetGetLightLevelToJson(getLightLevelStruct);
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(getLightLevelJson, "params");
    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(getLightLevelJson, "method");

    ASSERT_NE(getLightLevelJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(methodJson, nullptr);

    ASSERT_EQ(paramsJson->valueint, 38);
    ASSERT_STRCASEEQ(methodJson->valuestring, "getLightIntensity");

    cJSON_Delete(getLightLevelJson);
}

TEST(CloudController, widgetGetLightLevelToJsonDifferentValue)
{
    json_parser::TSetLightLevel getLightLevelStruct = {};

    getLightLevelStruct.lightIntensityLevel = 0;

    cJSON* getLightLevelJson = json_parser::widgetGetLightLevelToJson(getLightLevelStruct);
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(getLightLevelJson, "params");
    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(getLightLevelJson, "method");

    ASSERT_NE(getLightLevelJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(methodJson, nullptr);

    ASSERT_EQ(paramsJson->valueint, 0);
    ASSERT_STRCASEEQ(methodJson->valuestring, "getLightIntensity");

    cJSON_Delete(getLightLevelJson);
}

TEST(CloudController, widgetSetLightLevelToJson)
{
    json_parser::TSetLightLevel setLightLevelStruct = {};

    setLightLevelStruct.lightIntensityLevel = 79;

    cJSON* setLightLevelJson = json_parser::widgetSetLightLevelToJson(setLightLevelStruct);
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(setLightLevelJson, "params");
    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(setLightLevelJson, "method");

    ASSERT_NE(setLightLevelJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(methodJson, nullptr);

    ASSERT_EQ(paramsJson->valueint, 79);
    ASSERT_STRCASEEQ(methodJson->valuestring, "setLightIntensity");

    cJSON_Delete(setLightLevelJson);
}

TEST(CloudController, widgetSetLightLevelToJsonDifferentValue)
{
    json_parser::TSetLightLevel setLightLevelStruct = {};

    setLightLevelStruct.lightIntensityLevel = 255;

    cJSON* setLightLevelJson = json_parser::widgetSetLightLevelToJson(setLightLevelStruct);
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(setLightLevelJson, "params");
    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(setLightLevelJson, "method");

    ASSERT_NE(setLightLevelJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(methodJson, nullptr);

    ASSERT_EQ(paramsJson->valueint, 255);
    ASSERT_STRCASEEQ(methodJson->valuestring, "setLightIntensity");

    cJSON_Delete(setLightLevelJson);
}

TEST(CloudController, dataJsonToParamsJsonHeartbeat)
{
    json_parser::EMsgCode msgCode = json_parser::EMsgCode::MSG_HEARTBEAT;

    cJSON* dataJson = cJSON_CreateObject();
    cJSON* heartbeatJson = cJSON_CreateBool(true);

    cJSON_AddItemToObjectCS(dataJson, "heartbeat", heartbeatJson);

    cJSON* paramsJson = json_parser::dataJsonToParamsJson(dataJson, json_parser::EMsgCode::MSG_HEARTBEAT, 3456);
    cJSON* msgCounterJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");
    cJSON* dataJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");
    cJSON* heartbeatJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "heartbeat");

    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(dataJsonRead, nullptr);
    ASSERT_NE(heartbeatJsonRead, nullptr);

    ASSERT_EQ(msgCounterJsonRead->valueint, 3456);
    ASSERT_EQ(msgCodeJsonRead->valueint, 5);
    ASSERT_EQ(cJSON_IsTrue(heartbeatJsonRead), true);

    cJSON_Delete(dataJson);
}

TEST(CloudController, dataJsonToParamsJsonStatus)
{
    cJSON* dataJson = cJSON_CreateObject();
    cJSON* wifiConnectionStateJson = cJSON_CreateBool(true);
    cJSON* bleConnectionStateJson = cJSON_CreateBool(false);
    cJSON* lightIntensityLevelJson = cJSON_CreateNumber(234);
    cJSON* currentTimeMsJson = cJSON_CreateNumber(25432);
    cJSON* firmwareVersionJson = cJSON_CreateString("3.6.4");

    cJSON_AddItemToObjectCS(dataJson, "wifiConnectionState", wifiConnectionStateJson);
    cJSON_AddItemToObjectCS(dataJson, "bleConnectionState", bleConnectionStateJson);
    cJSON_AddItemToObjectCS(dataJson, "lightIntensityLevel", lightIntensityLevelJson);
    cJSON_AddItemToObjectCS(dataJson, "currentTimeMs", currentTimeMsJson);
    cJSON_AddItemToObjectCS(dataJson, "firmwareVersionJson", firmwareVersionJson);

    cJSON* paramsJson = json_parser::dataJsonToParamsJson(dataJson, json_parser::EMsgCode::MSG_STATUS_REPORT,23546);

    cJSON* msgCounterJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");
    cJSON* dataJsonRead = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* wifiConnectionStateJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "wifiConnectionState");
    cJSON* bleConnectionStateJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "bleConnectionState");
    cJSON* lightIntensityLevelJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "lightIntensityLevel");
    cJSON* currentTimeMsJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "currentTimeMs");
    cJSON* firmwareVersionJsonRead = cJSON_GetObjectItemCaseSensitive(dataJsonRead, "firmwareVersionJson");

    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJsonRead, nullptr);
    ASSERT_NE(msgCodeJsonRead, nullptr);
    ASSERT_NE(dataJsonRead, nullptr);
    ASSERT_NE(wifiConnectionStateJsonRead, nullptr);
    ASSERT_NE(bleConnectionStateJsonRead, nullptr);
    ASSERT_NE(lightIntensityLevelJsonRead, nullptr);
    ASSERT_NE(currentTimeMsJsonRead, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);

    ASSERT_EQ(cJSON_IsTrue(wifiConnectionStateJsonRead), true);
    ASSERT_EQ(cJSON_IsTrue(bleConnectionStateJsonRead), false);
    ASSERT_EQ(lightIntensityLevelJsonRead->valueint, 234);
    ASSERT_EQ(currentTimeMsJsonRead->valueint, 25432);
    ASSERT_STRCASEEQ(firmwareVersionJsonRead->valuestring, "3.6.4");

    ASSERT_EQ(msgCounterJsonRead->valueint, 23546);
    ASSERT_EQ(msgCodeJsonRead->valueint, 3);

    cJSON_Delete(dataJson);
}

TEST(CloudController, dataJsonToRpcCommandJsonHeartbeat)
{
    cJSON* pDataJson = cJSON_CreateObject();

    cJSON* pHeartbeatJson = cJSON_CreateBool(true);

    cJSON_AddItemToObjectCS(pDataJson, "heartbeat", pHeartbeatJson);

    cJSON* pRpcCommandJson = json_parser::dataJsonToRpcCommandJson(pDataJson, json_parser::EMsgCode::MSG_HEARTBEAT, 5467);

    cJSON* pMethodJsonRead = cJSON_GetObjectItemCaseSensitive(pRpcCommandJson, "method");
    cJSON* pParamsJsonRead = cJSON_GetObjectItemCaseSensitive(pRpcCommandJson, "params");

    cJSON* pMsgCounterJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "msgCounter");
    cJSON* pMsgCodeJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "msgCode");

    cJSON* pDataJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "data");
    cJSON* pHeartbeatJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "heartbeat");

    ASSERT_NE(pRpcCommandJson, nullptr);
    ASSERT_NE(pMethodJsonRead, nullptr);
    ASSERT_NE(pParamsJsonRead, nullptr);
    ASSERT_NE(pMsgCounterJsonRead, nullptr);
    ASSERT_NE(pMsgCodeJsonRead, nullptr);
    ASSERT_NE(pDataJsonRead, nullptr);
    ASSERT_NE(pHeartbeatJsonRead, nullptr);

    ASSERT_EQ(pMsgCounterJsonRead->valueint, 5467);
    ASSERT_EQ(pMsgCodeJsonRead->valueint, static_cast<uint8_t>(json_parser::EMsgCode::MSG_HEARTBEAT));
    ASSERT_EQ(cJSON_IsTrue(pHeartbeatJsonRead), true);

}

TEST(CloudController, dataJsonToRpcCommandJsonStatusReport)
{
    cJSON* pDataJson = cJSON_CreateObject();

    cJSON* pWifiConnectionStateJson = cJSON_CreateBool(true);
    cJSON* pBleConnectionStateJson = cJSON_CreateBool(false);
    cJSON* pLightIntensityLevelJson = cJSON_CreateNumber(59);
    cJSON* pCurrentTimeMsJson = cJSON_CreateNumber(354672);
    cJSON* pFirmwareVersionJson = cJSON_CreateString("1.6.2");

    cJSON_AddItemToObjectCS(pDataJson, "wifiConnectionState", pWifiConnectionStateJson);
    cJSON_AddItemToObjectCS(pDataJson, "bleConnectionState", pBleConnectionStateJson);
    cJSON_AddItemToObjectCS(pDataJson, "lightIntensityLevel", pLightIntensityLevelJson);
    cJSON_AddItemToObjectCS(pDataJson, "currentTimeMs", pCurrentTimeMsJson);
    cJSON_AddItemToObjectCS(pDataJson, "firmwareVersion", pFirmwareVersionJson);

    cJSON* pRpcCommandJson = json_parser::dataJsonToRpcCommandJson(pDataJson, json_parser::EMsgCode::MSG_STATUS_REPORT, 254317);

    cJSON* pMethodJsonRead = cJSON_GetObjectItemCaseSensitive(pRpcCommandJson, "method");
    cJSON* pParamsJsonRead = cJSON_GetObjectItemCaseSensitive(pRpcCommandJson, "params");

    cJSON* pMsgCounterJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "msgCounter");
    cJSON* pMsgCodeJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "msgCode");
    cJSON* pDataJsonRead = cJSON_GetObjectItemCaseSensitive(pParamsJsonRead, "data");

    cJSON* pWifiConnectionStateJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "wifiConnectionState");
    cJSON* pBleConnectionStateJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "bleConnectionState");
    cJSON* pLightIntensityLevelJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "lightIntensityLevel");
    cJSON* pCurrentTimeMsJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "currentTimeMs");
    cJSON* pFirmwareVersionJsonRead = cJSON_GetObjectItemCaseSensitive(pDataJsonRead, "firmwareVersion");

    ASSERT_NE(pRpcCommandJson, nullptr);
    ASSERT_NE(pMethodJsonRead, nullptr);
    ASSERT_NE(pParamsJsonRead, nullptr);
    ASSERT_NE(pMsgCounterJsonRead, nullptr);
    ASSERT_NE(pMsgCodeJsonRead, nullptr);
    ASSERT_NE(pDataJsonRead, nullptr);
    ASSERT_NE(pWifiConnectionStateJsonRead, nullptr);
    ASSERT_NE(pBleConnectionStateJsonRead, nullptr);
    ASSERT_NE(pLightIntensityLevelJsonRead, nullptr);
    ASSERT_NE(pCurrentTimeMsJsonRead, nullptr);
    ASSERT_NE(pFirmwareVersionJsonRead, nullptr);

    ASSERT_STRCASEEQ(pMethodJsonRead->valuestring, "rpcCommand");
    ASSERT_EQ(pMsgCounterJsonRead->valueint, 254317);
    ASSERT_EQ(pMsgCodeJsonRead->valueint, static_cast<uint8_t>(json_parser::EMsgCode::MSG_STATUS_REPORT));
    ASSERT_EQ(cJSON_IsTrue(pWifiConnectionStateJsonRead), true);
    ASSERT_EQ(cJSON_IsTrue(pBleConnectionStateJsonRead), false);
    ASSERT_EQ(pLightIntensityLevelJsonRead->valueint, 59);
    ASSERT_EQ(pCurrentTimeMsJsonRead->valueint, 354672);
    ASSERT_STREQ(pFirmwareVersionJsonRead->valuestring, "1.6.2");
}

TEST(CloudController, extractRequestIdFromTopic)
{
    std::string topicForTest1 = std::string("v1/devices/me/rpc/response/2");
    std::string topicForTest2 = std::string("v1/devices/me/rpc/response/255");
    std::string topicForTest3 = std::string("v1/devices/me/rpc/response/65535");
    std::string topicForTest4 = std::string("v1/devices/me/rpc/response/2147483647");
    std::string topicForTest5 = std::string("v1/devices/me/rpc/response/lwkrj");
    std::string topicForTest6 = std::string("v1/devices/me/rpc/response/0");

    int32_t result1 = json_parser::extractRequestIdFromTopic(topicForTest1);
    int32_t result2 = json_parser::extractRequestIdFromTopic(topicForTest2);
    int32_t result3 = json_parser::extractRequestIdFromTopic(topicForTest3);
    int32_t result4 = json_parser::extractRequestIdFromTopic(topicForTest4);
    int32_t result5 = json_parser::extractRequestIdFromTopic(topicForTest5);
    int32_t result6 = json_parser::extractRequestIdFromTopic(topicForTest6);

    ASSERT_EQ(result1, 2);
    ASSERT_EQ(result2, 255);
    ASSERT_EQ(result3, 65535);
    ASSERT_EQ(result4, 2147483647);
    ASSERT_EQ(result5, -1);
}
