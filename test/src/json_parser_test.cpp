
#include "defines.h"

#include "custom_types.h"
#include "test_utils.h"

#include "json_parser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cloud_controller.h"

#include "cJSON.h"

TEST(CloudController, processStatusReportJsonSimple)
{
    // Preparation of example JSON

    cJSON* deviceStatus = cJSON_CreateObject();

    cJSON* wifiConnectionState = cJSON_CreateBool(false);
    cJSON* bleConnectionState  = cJSON_CreateBool(true);
    cJSON* currentTimeMs       = cJSON_CreateNumber(35467);
    cJSON* firmwareVersion     = cJSON_CreateString("2.3.4");

    cJSON_AddItemToObject(deviceStatus, "wifiConnectionState", wifiConnectionState);
    cJSON_AddItemToObject(deviceStatus, "bleConnectionState", bleConnectionState);
    cJSON_AddItemToObject(deviceStatus, "currentTimeMs", currentTimeMs);
    cJSON_AddItemToObject(deviceStatus, "firmwareVersion", firmwareVersion);

    json_parser::TDeviceStatus deviceStatusStruct = {};

    bool result = json_parser::processStatusReport(deviceStatus, &deviceStatusStruct);

    ASSERT_TRUE(result);
    ASSERT_EQ(deviceStatusStruct.currentTimeFromStartupMs, 35467);
    ASSERT_STREQ(deviceStatusStruct.firmwareVersion, "2.3.4");
    ASSERT_EQ(deviceStatusStruct.isBleConnected, true);
    ASSERT_EQ(deviceStatusStruct.isWiFiConnected, false);

    cJSON_Delete(deviceStatus);
}

TEST(CloudController, prepareDeviceStatusMessage)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs = 23123;
    strcpy(deviceStatusStruct.firmwareVersion, "3.4.5");
    strcpy(deviceStatusStruct.currentLocalTime, "Wed May 04 12:34:45 2022");
    deviceStatusStruct.isBleConnected  = true;
    deviceStatusStruct.isWiFiConnected = false;

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatusStruct, 567);

    cJSON* deviceStatusJson = cJSON_Parse(deviceStatusMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson    = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");

    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* wifiConnectionStateJson      = cJSON_GetObjectItemCaseSensitive(dataJson, "wifiConnectionState");
    cJSON* bleConnectionStateJson       = cJSON_GetObjectItemCaseSensitive(dataJson, "bleConnectionState");
    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson          = cJSON_GetObjectItemCaseSensitive(dataJson, "firmwareVersion");
    cJSON* currentLocalTimeJson         = cJSON_GetObjectItemCaseSensitive(dataJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 567);
    ASSERT_EQ(msgCodeJson->valueint, 3);

    ASSERT_TRUE(cJSON_IsTrue(bleConnectionStateJson));
    ASSERT_FALSE(cJSON_IsTrue(wifiConnectionStateJson));
    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 23123);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "3.4.5");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Wed May 04 12:34:45 2022");

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, prepareDeviceStatusMessageDifferentValues)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs = 34234;
    strcpy(deviceStatusStruct.firmwareVersion, "5.4.3");
    strcpy(deviceStatusStruct.currentLocalTime, "Fri May 06 09:57:12 2022");

    deviceStatusStruct.isBleConnected  = false;
    deviceStatusStruct.isWiFiConnected = true;

    std::string deviceStatusMessage = json_parser::prepareDeviceStatusMessage(deviceStatusStruct, 3254);

    cJSON* deviceStatusJson = cJSON_Parse(deviceStatusMessage.c_str());

    cJSON* methodJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "method");
    cJSON* paramsJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "params");

    cJSON* msgCounterJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCounter");
    cJSON* msgCodeJson    = cJSON_GetObjectItemCaseSensitive(paramsJson, "msgCode");

    cJSON* dataJson = cJSON_GetObjectItemCaseSensitive(paramsJson, "data");

    cJSON* wifiConnectionStateJson      = cJSON_GetObjectItemCaseSensitive(dataJson, "wifiConnectionState");
    cJSON* bleConnectionStateJson       = cJSON_GetObjectItemCaseSensitive(dataJson, "bleConnectionState");
    cJSON* currentTimeFromStartupMsJson = cJSON_GetObjectItemCaseSensitive(dataJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson          = cJSON_GetObjectItemCaseSensitive(dataJson, "firmwareVersion");
    cJSON* currentLocalTimeJson         = cJSON_GetObjectItemCaseSensitive(dataJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);
    ASSERT_NE(methodJson, nullptr);
    ASSERT_NE(paramsJson, nullptr);
    ASSERT_NE(msgCounterJson, nullptr);
    ASSERT_NE(msgCodeJson, nullptr);
    ASSERT_NE(dataJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_STRCASEEQ(methodJson->valuestring, "rpcCommand");
    ASSERT_EQ(msgCounterJson->valueint, 3254);
    ASSERT_EQ(msgCodeJson->valueint, 3);

    ASSERT_FALSE(cJSON_IsTrue(bleConnectionStateJson));
    ASSERT_TRUE(cJSON_IsTrue(wifiConnectionStateJson));
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
    deviceStatusStruct.isBleConnected  = true;
    deviceStatusStruct.isWiFiConnected = true;

    cJSON* deviceStatusJson = json_parser::deviceStatusToJson(deviceStatusStruct);

    cJSON* currentTimeFromStartupMsJson =
        cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson     = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "firmwareVersion");
    cJSON* bleConnectionStateJson  = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "bleConnectionState");
    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "wifiConnectionState");
    cJSON* currentLocalTimeJson    = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);

    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 65321);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "1.2.8");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Fri May 31 11:11:11 2024");
    ASSERT_EQ(cJSON_IsTrue(bleConnectionStateJson), true);
    ASSERT_EQ(cJSON_IsTrue(wifiConnectionStateJson), true);

    cJSON_Delete(deviceStatusJson);
}

TEST(CloudController, deviceStatusToJsonDifferentValues)
{
    json_parser::TDeviceStatus deviceStatusStruct = {};

    deviceStatusStruct.currentTimeFromStartupMs = 0;
    strcpy(deviceStatusStruct.firmwareVersion, "1.9.8");
    strcpy(deviceStatusStruct.currentLocalTime, "Sat Jan 02 22:22:22 2048");
    deviceStatusStruct.isBleConnected  = false;
    deviceStatusStruct.isWiFiConnected = false;

    cJSON* deviceStatusJson = json_parser::deviceStatusToJson(deviceStatusStruct);

    cJSON* currentTimeFromStartupMsJson =
        cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentTimeFromStartupMs");
    cJSON* firmwareVersionJson     = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "firmwareVersion");
    cJSON* bleConnectionStateJson  = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "bleConnectionState");
    cJSON* wifiConnectionStateJson = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "wifiConnectionState");
    cJSON* currentLocalTimeJson    = cJSON_GetObjectItemCaseSensitive(deviceStatusJson, "currentLocalTime");

    ASSERT_NE(deviceStatusJson, nullptr);

    ASSERT_NE(currentTimeFromStartupMsJson, nullptr);
    ASSERT_NE(firmwareVersionJson, nullptr);
    ASSERT_NE(bleConnectionStateJson, nullptr);
    ASSERT_NE(wifiConnectionStateJson, nullptr);
    ASSERT_NE(currentLocalTimeJson, nullptr);

    ASSERT_EQ(currentTimeFromStartupMsJson->valueint, 0);
    ASSERT_STRCASEEQ(firmwareVersionJson->valuestring, "1.9.8");
    ASSERT_STRCASEEQ(currentLocalTimeJson->valuestring, "Sat Jan 02 22:22:22 2048");
    ASSERT_EQ(cJSON_IsTrue(bleConnectionStateJson), false);
    ASSERT_EQ(cJSON_IsTrue(wifiConnectionStateJson), false);

    cJSON_Delete(deviceStatusJson);
}
