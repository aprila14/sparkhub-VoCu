#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_CORE_MQTT_H
#define _MOCK_CORE_MQTT_H

#include "unity.h"
#include "core_mqtt.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void mock_core_mqtt_Init(void);
void mock_core_mqtt_Destroy(void);
void mock_core_mqtt_Verify(void);




#define MQTT_Init_IgnoreAndReturn(cmock_retval) MQTT_Init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Init_StopIgnore() MQTT_Init_CMockStopIgnore()
void MQTT_Init_CMockStopIgnore(void);
#define MQTT_Init_ExpectAndReturn(pContext, pTransportInterface, getTimeFunction, userCallback, pNetworkBuffer, cmock_retval) MQTT_Init_CMockExpectAndReturn(__LINE__, pContext, pTransportInterface, getTimeFunction, userCallback, pNetworkBuffer, cmock_retval)
void MQTT_Init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, const TransportInterface_t* pTransportInterface, MQTTGetCurrentTimeFunc_t getTimeFunction, MQTTEventCallback_t userCallback, const MQTTFixedBuffer_t* pNetworkBuffer, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Init_CALLBACK)(MQTTContext_t* pContext, const TransportInterface_t* pTransportInterface, MQTTGetCurrentTimeFunc_t getTimeFunction, MQTTEventCallback_t userCallback, const MQTTFixedBuffer_t* pNetworkBuffer, int cmock_num_calls);
void MQTT_Init_AddCallback(CMOCK_MQTT_Init_CALLBACK Callback);
void MQTT_Init_Stub(CMOCK_MQTT_Init_CALLBACK Callback);
#define MQTT_Init_StubWithCallback MQTT_Init_Stub
#define MQTT_Init_ExpectWithArrayAndReturn(pContext, pContext_Depth, pTransportInterface, pTransportInterface_Depth, getTimeFunction, userCallback, pNetworkBuffer, pNetworkBuffer_Depth, cmock_retval) MQTT_Init_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, pTransportInterface, pTransportInterface_Depth, getTimeFunction, userCallback, pNetworkBuffer, pNetworkBuffer_Depth, cmock_retval)
void MQTT_Init_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, const TransportInterface_t* pTransportInterface, int pTransportInterface_Depth, MQTTGetCurrentTimeFunc_t getTimeFunction, MQTTEventCallback_t userCallback, const MQTTFixedBuffer_t* pNetworkBuffer, int pNetworkBuffer_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_Init_ReturnThruPtr_pContext(pContext) MQTT_Init_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Init_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Init_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Init_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Init_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Init_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Init_IgnoreArg_pContext() MQTT_Init_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Init_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Init_IgnoreArg_pTransportInterface() MQTT_Init_CMockIgnoreArg_pTransportInterface(__LINE__)
void MQTT_Init_CMockIgnoreArg_pTransportInterface(UNITY_LINE_TYPE cmock_line);
#define MQTT_Init_IgnoreArg_getTimeFunction() MQTT_Init_CMockIgnoreArg_getTimeFunction(__LINE__)
void MQTT_Init_CMockIgnoreArg_getTimeFunction(UNITY_LINE_TYPE cmock_line);
#define MQTT_Init_IgnoreArg_userCallback() MQTT_Init_CMockIgnoreArg_userCallback(__LINE__)
void MQTT_Init_CMockIgnoreArg_userCallback(UNITY_LINE_TYPE cmock_line);
#define MQTT_Init_IgnoreArg_pNetworkBuffer() MQTT_Init_CMockIgnoreArg_pNetworkBuffer(__LINE__)
void MQTT_Init_CMockIgnoreArg_pNetworkBuffer(UNITY_LINE_TYPE cmock_line);
#define MQTT_Connect_IgnoreAndReturn(cmock_retval) MQTT_Connect_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Connect_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Connect_StopIgnore() MQTT_Connect_CMockStopIgnore()
void MQTT_Connect_CMockStopIgnore(void);
#define MQTT_Connect_ExpectAndReturn(pContext, pConnectInfo, pWillInfo, timeoutMs, pSessionPresent, cmock_retval) MQTT_Connect_CMockExpectAndReturn(__LINE__, pContext, pConnectInfo, pWillInfo, timeoutMs, pSessionPresent, cmock_retval)
void MQTT_Connect_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, const MQTTConnectInfo_t* pConnectInfo, const MQTTPublishInfo_t* pWillInfo, uint32_t timeoutMs, bool* pSessionPresent, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Connect_CALLBACK)(MQTTContext_t* pContext, const MQTTConnectInfo_t* pConnectInfo, const MQTTPublishInfo_t* pWillInfo, uint32_t timeoutMs, bool* pSessionPresent, int cmock_num_calls);
void MQTT_Connect_AddCallback(CMOCK_MQTT_Connect_CALLBACK Callback);
void MQTT_Connect_Stub(CMOCK_MQTT_Connect_CALLBACK Callback);
#define MQTT_Connect_StubWithCallback MQTT_Connect_Stub
#define MQTT_Connect_ExpectWithArrayAndReturn(pContext, pContext_Depth, pConnectInfo, pConnectInfo_Depth, pWillInfo, pWillInfo_Depth, timeoutMs, pSessionPresent, pSessionPresent_Depth, cmock_retval) MQTT_Connect_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, pConnectInfo, pConnectInfo_Depth, pWillInfo, pWillInfo_Depth, timeoutMs, pSessionPresent, pSessionPresent_Depth, cmock_retval)
void MQTT_Connect_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, const MQTTConnectInfo_t* pConnectInfo, int pConnectInfo_Depth, const MQTTPublishInfo_t* pWillInfo, int pWillInfo_Depth, uint32_t timeoutMs, bool* pSessionPresent, int pSessionPresent_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_Connect_ReturnThruPtr_pContext(pContext) MQTT_Connect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Connect_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Connect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Connect_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Connect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Connect_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Connect_ReturnThruPtr_pSessionPresent(pSessionPresent) MQTT_Connect_CMockReturnMemThruPtr_pSessionPresent(__LINE__, pSessionPresent, sizeof(bool))
#define MQTT_Connect_ReturnArrayThruPtr_pSessionPresent(pSessionPresent, cmock_len) MQTT_Connect_CMockReturnMemThruPtr_pSessionPresent(__LINE__, pSessionPresent, cmock_len * sizeof(*pSessionPresent))
#define MQTT_Connect_ReturnMemThruPtr_pSessionPresent(pSessionPresent, cmock_size) MQTT_Connect_CMockReturnMemThruPtr_pSessionPresent(__LINE__, pSessionPresent, cmock_size)
void MQTT_Connect_CMockReturnMemThruPtr_pSessionPresent(UNITY_LINE_TYPE cmock_line, bool* pSessionPresent, size_t cmock_size);
#define MQTT_Connect_IgnoreArg_pContext() MQTT_Connect_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Connect_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Connect_IgnoreArg_pConnectInfo() MQTT_Connect_CMockIgnoreArg_pConnectInfo(__LINE__)
void MQTT_Connect_CMockIgnoreArg_pConnectInfo(UNITY_LINE_TYPE cmock_line);
#define MQTT_Connect_IgnoreArg_pWillInfo() MQTT_Connect_CMockIgnoreArg_pWillInfo(__LINE__)
void MQTT_Connect_CMockIgnoreArg_pWillInfo(UNITY_LINE_TYPE cmock_line);
#define MQTT_Connect_IgnoreArg_timeoutMs() MQTT_Connect_CMockIgnoreArg_timeoutMs(__LINE__)
void MQTT_Connect_CMockIgnoreArg_timeoutMs(UNITY_LINE_TYPE cmock_line);
#define MQTT_Connect_IgnoreArg_pSessionPresent() MQTT_Connect_CMockIgnoreArg_pSessionPresent(__LINE__)
void MQTT_Connect_CMockIgnoreArg_pSessionPresent(UNITY_LINE_TYPE cmock_line);
#define MQTT_Subscribe_IgnoreAndReturn(cmock_retval) MQTT_Subscribe_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Subscribe_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Subscribe_StopIgnore() MQTT_Subscribe_CMockStopIgnore()
void MQTT_Subscribe_CMockStopIgnore(void);
#define MQTT_Subscribe_ExpectAndReturn(pContext, pSubscriptionList, subscriptionCount, packetId, cmock_retval) MQTT_Subscribe_CMockExpectAndReturn(__LINE__, pContext, pSubscriptionList, subscriptionCount, packetId, cmock_retval)
void MQTT_Subscribe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, const MQTTSubscribeInfo_t* pSubscriptionList, size_t subscriptionCount, uint16_t packetId, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Subscribe_CALLBACK)(MQTTContext_t* pContext, const MQTTSubscribeInfo_t* pSubscriptionList, size_t subscriptionCount, uint16_t packetId, int cmock_num_calls);
void MQTT_Subscribe_AddCallback(CMOCK_MQTT_Subscribe_CALLBACK Callback);
void MQTT_Subscribe_Stub(CMOCK_MQTT_Subscribe_CALLBACK Callback);
#define MQTT_Subscribe_StubWithCallback MQTT_Subscribe_Stub
#define MQTT_Subscribe_ExpectWithArrayAndReturn(pContext, pContext_Depth, pSubscriptionList, pSubscriptionList_Depth, subscriptionCount, packetId, cmock_retval) MQTT_Subscribe_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, pSubscriptionList, pSubscriptionList_Depth, subscriptionCount, packetId, cmock_retval)
void MQTT_Subscribe_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, const MQTTSubscribeInfo_t* pSubscriptionList, int pSubscriptionList_Depth, size_t subscriptionCount, uint16_t packetId, MQTTStatus_t cmock_to_return);
#define MQTT_Subscribe_ReturnThruPtr_pContext(pContext) MQTT_Subscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Subscribe_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Subscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Subscribe_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Subscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Subscribe_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Subscribe_IgnoreArg_pContext() MQTT_Subscribe_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Subscribe_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Subscribe_IgnoreArg_pSubscriptionList() MQTT_Subscribe_CMockIgnoreArg_pSubscriptionList(__LINE__)
void MQTT_Subscribe_CMockIgnoreArg_pSubscriptionList(UNITY_LINE_TYPE cmock_line);
#define MQTT_Subscribe_IgnoreArg_subscriptionCount() MQTT_Subscribe_CMockIgnoreArg_subscriptionCount(__LINE__)
void MQTT_Subscribe_CMockIgnoreArg_subscriptionCount(UNITY_LINE_TYPE cmock_line);
#define MQTT_Subscribe_IgnoreArg_packetId() MQTT_Subscribe_CMockIgnoreArg_packetId(__LINE__)
void MQTT_Subscribe_CMockIgnoreArg_packetId(UNITY_LINE_TYPE cmock_line);
#define MQTT_Publish_IgnoreAndReturn(cmock_retval) MQTT_Publish_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Publish_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Publish_StopIgnore() MQTT_Publish_CMockStopIgnore()
void MQTT_Publish_CMockStopIgnore(void);
#define MQTT_Publish_ExpectAndReturn(pContext, pPublishInfo, packetId, cmock_retval) MQTT_Publish_CMockExpectAndReturn(__LINE__, pContext, pPublishInfo, packetId, cmock_retval)
void MQTT_Publish_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, const MQTTPublishInfo_t* pPublishInfo, uint16_t packetId, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Publish_CALLBACK)(MQTTContext_t* pContext, const MQTTPublishInfo_t* pPublishInfo, uint16_t packetId, int cmock_num_calls);
void MQTT_Publish_AddCallback(CMOCK_MQTT_Publish_CALLBACK Callback);
void MQTT_Publish_Stub(CMOCK_MQTT_Publish_CALLBACK Callback);
#define MQTT_Publish_StubWithCallback MQTT_Publish_Stub
#define MQTT_Publish_ExpectWithArrayAndReturn(pContext, pContext_Depth, pPublishInfo, pPublishInfo_Depth, packetId, cmock_retval) MQTT_Publish_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, pPublishInfo, pPublishInfo_Depth, packetId, cmock_retval)
void MQTT_Publish_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, const MQTTPublishInfo_t* pPublishInfo, int pPublishInfo_Depth, uint16_t packetId, MQTTStatus_t cmock_to_return);
#define MQTT_Publish_ReturnThruPtr_pContext(pContext) MQTT_Publish_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Publish_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Publish_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Publish_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Publish_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Publish_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Publish_IgnoreArg_pContext() MQTT_Publish_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Publish_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Publish_IgnoreArg_pPublishInfo() MQTT_Publish_CMockIgnoreArg_pPublishInfo(__LINE__)
void MQTT_Publish_CMockIgnoreArg_pPublishInfo(UNITY_LINE_TYPE cmock_line);
#define MQTT_Publish_IgnoreArg_packetId() MQTT_Publish_CMockIgnoreArg_packetId(__LINE__)
void MQTT_Publish_CMockIgnoreArg_packetId(UNITY_LINE_TYPE cmock_line);
#define MQTT_Ping_IgnoreAndReturn(cmock_retval) MQTT_Ping_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Ping_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Ping_StopIgnore() MQTT_Ping_CMockStopIgnore()
void MQTT_Ping_CMockStopIgnore(void);
#define MQTT_Ping_ExpectAndReturn(pContext, cmock_retval) MQTT_Ping_CMockExpectAndReturn(__LINE__, pContext, cmock_retval)
void MQTT_Ping_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Ping_CALLBACK)(MQTTContext_t* pContext, int cmock_num_calls);
void MQTT_Ping_AddCallback(CMOCK_MQTT_Ping_CALLBACK Callback);
void MQTT_Ping_Stub(CMOCK_MQTT_Ping_CALLBACK Callback);
#define MQTT_Ping_StubWithCallback MQTT_Ping_Stub
#define MQTT_Ping_ExpectWithArrayAndReturn(pContext, pContext_Depth, cmock_retval) MQTT_Ping_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, cmock_retval)
void MQTT_Ping_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_Ping_ReturnThruPtr_pContext(pContext) MQTT_Ping_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Ping_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Ping_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Ping_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Ping_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Ping_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Ping_IgnoreArg_pContext() MQTT_Ping_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Ping_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Unsubscribe_IgnoreAndReturn(cmock_retval) MQTT_Unsubscribe_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Unsubscribe_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Unsubscribe_StopIgnore() MQTT_Unsubscribe_CMockStopIgnore()
void MQTT_Unsubscribe_CMockStopIgnore(void);
#define MQTT_Unsubscribe_ExpectAndReturn(pContext, pSubscriptionList, subscriptionCount, packetId, cmock_retval) MQTT_Unsubscribe_CMockExpectAndReturn(__LINE__, pContext, pSubscriptionList, subscriptionCount, packetId, cmock_retval)
void MQTT_Unsubscribe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, const MQTTSubscribeInfo_t* pSubscriptionList, size_t subscriptionCount, uint16_t packetId, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Unsubscribe_CALLBACK)(MQTTContext_t* pContext, const MQTTSubscribeInfo_t* pSubscriptionList, size_t subscriptionCount, uint16_t packetId, int cmock_num_calls);
void MQTT_Unsubscribe_AddCallback(CMOCK_MQTT_Unsubscribe_CALLBACK Callback);
void MQTT_Unsubscribe_Stub(CMOCK_MQTT_Unsubscribe_CALLBACK Callback);
#define MQTT_Unsubscribe_StubWithCallback MQTT_Unsubscribe_Stub
#define MQTT_Unsubscribe_ExpectWithArrayAndReturn(pContext, pContext_Depth, pSubscriptionList, pSubscriptionList_Depth, subscriptionCount, packetId, cmock_retval) MQTT_Unsubscribe_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, pSubscriptionList, pSubscriptionList_Depth, subscriptionCount, packetId, cmock_retval)
void MQTT_Unsubscribe_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, const MQTTSubscribeInfo_t* pSubscriptionList, int pSubscriptionList_Depth, size_t subscriptionCount, uint16_t packetId, MQTTStatus_t cmock_to_return);
#define MQTT_Unsubscribe_ReturnThruPtr_pContext(pContext) MQTT_Unsubscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Unsubscribe_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Unsubscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Unsubscribe_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Unsubscribe_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Unsubscribe_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Unsubscribe_IgnoreArg_pContext() MQTT_Unsubscribe_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Unsubscribe_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_Unsubscribe_IgnoreArg_pSubscriptionList() MQTT_Unsubscribe_CMockIgnoreArg_pSubscriptionList(__LINE__)
void MQTT_Unsubscribe_CMockIgnoreArg_pSubscriptionList(UNITY_LINE_TYPE cmock_line);
#define MQTT_Unsubscribe_IgnoreArg_subscriptionCount() MQTT_Unsubscribe_CMockIgnoreArg_subscriptionCount(__LINE__)
void MQTT_Unsubscribe_CMockIgnoreArg_subscriptionCount(UNITY_LINE_TYPE cmock_line);
#define MQTT_Unsubscribe_IgnoreArg_packetId() MQTT_Unsubscribe_CMockIgnoreArg_packetId(__LINE__)
void MQTT_Unsubscribe_CMockIgnoreArg_packetId(UNITY_LINE_TYPE cmock_line);
#define MQTT_Disconnect_IgnoreAndReturn(cmock_retval) MQTT_Disconnect_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Disconnect_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_Disconnect_StopIgnore() MQTT_Disconnect_CMockStopIgnore()
void MQTT_Disconnect_CMockStopIgnore(void);
#define MQTT_Disconnect_ExpectAndReturn(pContext, cmock_retval) MQTT_Disconnect_CMockExpectAndReturn(__LINE__, pContext, cmock_retval)
void MQTT_Disconnect_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_Disconnect_CALLBACK)(MQTTContext_t* pContext, int cmock_num_calls);
void MQTT_Disconnect_AddCallback(CMOCK_MQTT_Disconnect_CALLBACK Callback);
void MQTT_Disconnect_Stub(CMOCK_MQTT_Disconnect_CALLBACK Callback);
#define MQTT_Disconnect_StubWithCallback MQTT_Disconnect_Stub
#define MQTT_Disconnect_ExpectWithArrayAndReturn(pContext, pContext_Depth, cmock_retval) MQTT_Disconnect_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, cmock_retval)
void MQTT_Disconnect_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_Disconnect_ReturnThruPtr_pContext(pContext) MQTT_Disconnect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_Disconnect_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_Disconnect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_Disconnect_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_Disconnect_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_Disconnect_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_Disconnect_IgnoreArg_pContext() MQTT_Disconnect_CMockIgnoreArg_pContext(__LINE__)
void MQTT_Disconnect_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_ProcessLoop_IgnoreAndReturn(cmock_retval) MQTT_ProcessLoop_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_ProcessLoop_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_ProcessLoop_StopIgnore() MQTT_ProcessLoop_CMockStopIgnore()
void MQTT_ProcessLoop_CMockStopIgnore(void);
#define MQTT_ProcessLoop_ExpectAndReturn(pContext, timeoutMs, cmock_retval) MQTT_ProcessLoop_CMockExpectAndReturn(__LINE__, pContext, timeoutMs, cmock_retval)
void MQTT_ProcessLoop_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, uint32_t timeoutMs, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_ProcessLoop_CALLBACK)(MQTTContext_t* pContext, uint32_t timeoutMs, int cmock_num_calls);
void MQTT_ProcessLoop_AddCallback(CMOCK_MQTT_ProcessLoop_CALLBACK Callback);
void MQTT_ProcessLoop_Stub(CMOCK_MQTT_ProcessLoop_CALLBACK Callback);
#define MQTT_ProcessLoop_StubWithCallback MQTT_ProcessLoop_Stub
#define MQTT_ProcessLoop_ExpectWithArrayAndReturn(pContext, pContext_Depth, timeoutMs, cmock_retval) MQTT_ProcessLoop_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, timeoutMs, cmock_retval)
void MQTT_ProcessLoop_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, uint32_t timeoutMs, MQTTStatus_t cmock_to_return);
#define MQTT_ProcessLoop_ReturnThruPtr_pContext(pContext) MQTT_ProcessLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_ProcessLoop_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_ProcessLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_ProcessLoop_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_ProcessLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_ProcessLoop_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_ProcessLoop_IgnoreArg_pContext() MQTT_ProcessLoop_CMockIgnoreArg_pContext(__LINE__)
void MQTT_ProcessLoop_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_ProcessLoop_IgnoreArg_timeoutMs() MQTT_ProcessLoop_CMockIgnoreArg_timeoutMs(__LINE__)
void MQTT_ProcessLoop_CMockIgnoreArg_timeoutMs(UNITY_LINE_TYPE cmock_line);
#define MQTT_ReceiveLoop_IgnoreAndReturn(cmock_retval) MQTT_ReceiveLoop_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_ReceiveLoop_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_ReceiveLoop_StopIgnore() MQTT_ReceiveLoop_CMockStopIgnore()
void MQTT_ReceiveLoop_CMockStopIgnore(void);
#define MQTT_ReceiveLoop_ExpectAndReturn(pContext, timeoutMs, cmock_retval) MQTT_ReceiveLoop_CMockExpectAndReturn(__LINE__, pContext, timeoutMs, cmock_retval)
void MQTT_ReceiveLoop_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, uint32_t timeoutMs, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_ReceiveLoop_CALLBACK)(MQTTContext_t* pContext, uint32_t timeoutMs, int cmock_num_calls);
void MQTT_ReceiveLoop_AddCallback(CMOCK_MQTT_ReceiveLoop_CALLBACK Callback);
void MQTT_ReceiveLoop_Stub(CMOCK_MQTT_ReceiveLoop_CALLBACK Callback);
#define MQTT_ReceiveLoop_StubWithCallback MQTT_ReceiveLoop_Stub
#define MQTT_ReceiveLoop_ExpectWithArrayAndReturn(pContext, pContext_Depth, timeoutMs, cmock_retval) MQTT_ReceiveLoop_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, timeoutMs, cmock_retval)
void MQTT_ReceiveLoop_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, uint32_t timeoutMs, MQTTStatus_t cmock_to_return);
#define MQTT_ReceiveLoop_ReturnThruPtr_pContext(pContext) MQTT_ReceiveLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_ReceiveLoop_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_ReceiveLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_ReceiveLoop_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_ReceiveLoop_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_ReceiveLoop_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_ReceiveLoop_IgnoreArg_pContext() MQTT_ReceiveLoop_CMockIgnoreArg_pContext(__LINE__)
void MQTT_ReceiveLoop_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_ReceiveLoop_IgnoreArg_timeoutMs() MQTT_ReceiveLoop_CMockIgnoreArg_timeoutMs(__LINE__)
void MQTT_ReceiveLoop_CMockIgnoreArg_timeoutMs(UNITY_LINE_TYPE cmock_line);
#define MQTT_GetPacketId_IgnoreAndReturn(cmock_retval) MQTT_GetPacketId_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_GetPacketId_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t cmock_to_return);
#define MQTT_GetPacketId_StopIgnore() MQTT_GetPacketId_CMockStopIgnore()
void MQTT_GetPacketId_CMockStopIgnore(void);
#define MQTT_GetPacketId_ExpectAndReturn(pContext, cmock_retval) MQTT_GetPacketId_CMockExpectAndReturn(__LINE__, pContext, cmock_retval)
void MQTT_GetPacketId_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, uint16_t cmock_to_return);
typedef uint16_t (* CMOCK_MQTT_GetPacketId_CALLBACK)(MQTTContext_t* pContext, int cmock_num_calls);
void MQTT_GetPacketId_AddCallback(CMOCK_MQTT_GetPacketId_CALLBACK Callback);
void MQTT_GetPacketId_Stub(CMOCK_MQTT_GetPacketId_CALLBACK Callback);
#define MQTT_GetPacketId_StubWithCallback MQTT_GetPacketId_Stub
#define MQTT_GetPacketId_ExpectWithArrayAndReturn(pContext, pContext_Depth, cmock_retval) MQTT_GetPacketId_CMockExpectWithArrayAndReturn(__LINE__, pContext, pContext_Depth, cmock_retval)
void MQTT_GetPacketId_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, int pContext_Depth, uint16_t cmock_to_return);
#define MQTT_GetPacketId_ReturnThruPtr_pContext(pContext) MQTT_GetPacketId_CMockReturnMemThruPtr_pContext(__LINE__, pContext, sizeof(MQTTContext_t))
#define MQTT_GetPacketId_ReturnArrayThruPtr_pContext(pContext, cmock_len) MQTT_GetPacketId_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_len * sizeof(*pContext))
#define MQTT_GetPacketId_ReturnMemThruPtr_pContext(pContext, cmock_size) MQTT_GetPacketId_CMockReturnMemThruPtr_pContext(__LINE__, pContext, cmock_size)
void MQTT_GetPacketId_CMockReturnMemThruPtr_pContext(UNITY_LINE_TYPE cmock_line, MQTTContext_t* pContext, size_t cmock_size);
#define MQTT_GetPacketId_IgnoreArg_pContext() MQTT_GetPacketId_CMockIgnoreArg_pContext(__LINE__)
void MQTT_GetPacketId_CMockIgnoreArg_pContext(UNITY_LINE_TYPE cmock_line);
#define MQTT_MatchTopic_IgnoreAndReturn(cmock_retval) MQTT_MatchTopic_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_MatchTopic_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_MatchTopic_StopIgnore() MQTT_MatchTopic_CMockStopIgnore()
void MQTT_MatchTopic_CMockStopIgnore(void);
#define MQTT_MatchTopic_ExpectAndReturn(pTopicName, topicNameLength, pTopicFilter, topicFilterLength, pIsMatch, cmock_retval) MQTT_MatchTopic_CMockExpectAndReturn(__LINE__, pTopicName, topicNameLength, pTopicFilter, topicFilterLength, pIsMatch, cmock_retval)
void MQTT_MatchTopic_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* pTopicName, const uint16_t topicNameLength, const char* pTopicFilter, const uint16_t topicFilterLength, bool* pIsMatch, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_MatchTopic_CALLBACK)(const char* pTopicName, const uint16_t topicNameLength, const char* pTopicFilter, const uint16_t topicFilterLength, bool* pIsMatch, int cmock_num_calls);
void MQTT_MatchTopic_AddCallback(CMOCK_MQTT_MatchTopic_CALLBACK Callback);
void MQTT_MatchTopic_Stub(CMOCK_MQTT_MatchTopic_CALLBACK Callback);
#define MQTT_MatchTopic_StubWithCallback MQTT_MatchTopic_Stub
#define MQTT_MatchTopic_ExpectWithArrayAndReturn(pTopicName, topicNameLength, pTopicFilter, topicFilterLength, pIsMatch, pIsMatch_Depth, cmock_retval) MQTT_MatchTopic_CMockExpectWithArrayAndReturn(__LINE__, pTopicName, topicNameLength, pTopicFilter, topicFilterLength, pIsMatch, pIsMatch_Depth, cmock_retval)
void MQTT_MatchTopic_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const char* pTopicName, const uint16_t topicNameLength, const char* pTopicFilter, const uint16_t topicFilterLength, bool* pIsMatch, int pIsMatch_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_MatchTopic_ReturnThruPtr_pIsMatch(pIsMatch) MQTT_MatchTopic_CMockReturnMemThruPtr_pIsMatch(__LINE__, pIsMatch, sizeof(bool))
#define MQTT_MatchTopic_ReturnArrayThruPtr_pIsMatch(pIsMatch, cmock_len) MQTT_MatchTopic_CMockReturnMemThruPtr_pIsMatch(__LINE__, pIsMatch, cmock_len * sizeof(*pIsMatch))
#define MQTT_MatchTopic_ReturnMemThruPtr_pIsMatch(pIsMatch, cmock_size) MQTT_MatchTopic_CMockReturnMemThruPtr_pIsMatch(__LINE__, pIsMatch, cmock_size)
void MQTT_MatchTopic_CMockReturnMemThruPtr_pIsMatch(UNITY_LINE_TYPE cmock_line, bool* pIsMatch, size_t cmock_size);
#define MQTT_MatchTopic_IgnoreArg_pTopicName() MQTT_MatchTopic_CMockIgnoreArg_pTopicName(__LINE__)
void MQTT_MatchTopic_CMockIgnoreArg_pTopicName(UNITY_LINE_TYPE cmock_line);
#define MQTT_MatchTopic_IgnoreArg_topicNameLength() MQTT_MatchTopic_CMockIgnoreArg_topicNameLength(__LINE__)
void MQTT_MatchTopic_CMockIgnoreArg_topicNameLength(UNITY_LINE_TYPE cmock_line);
#define MQTT_MatchTopic_IgnoreArg_pTopicFilter() MQTT_MatchTopic_CMockIgnoreArg_pTopicFilter(__LINE__)
void MQTT_MatchTopic_CMockIgnoreArg_pTopicFilter(UNITY_LINE_TYPE cmock_line);
#define MQTT_MatchTopic_IgnoreArg_topicFilterLength() MQTT_MatchTopic_CMockIgnoreArg_topicFilterLength(__LINE__)
void MQTT_MatchTopic_CMockIgnoreArg_topicFilterLength(UNITY_LINE_TYPE cmock_line);
#define MQTT_MatchTopic_IgnoreArg_pIsMatch() MQTT_MatchTopic_CMockIgnoreArg_pIsMatch(__LINE__)
void MQTT_MatchTopic_CMockIgnoreArg_pIsMatch(UNITY_LINE_TYPE cmock_line);
#define MQTT_GetSubAckStatusCodes_IgnoreAndReturn(cmock_retval) MQTT_GetSubAckStatusCodes_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_GetSubAckStatusCodes_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t cmock_to_return);
#define MQTT_GetSubAckStatusCodes_StopIgnore() MQTT_GetSubAckStatusCodes_CMockStopIgnore()
void MQTT_GetSubAckStatusCodes_CMockStopIgnore(void);
#define MQTT_GetSubAckStatusCodes_ExpectAndReturn(pSubackPacket, pPayloadStart, pPayloadSize, cmock_retval) MQTT_GetSubAckStatusCodes_CMockExpectAndReturn(__LINE__, pSubackPacket, pPayloadStart, pPayloadSize, cmock_retval)
void MQTT_GetSubAckStatusCodes_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const MQTTPacketInfo_t* pSubackPacket, uint8_t** pPayloadStart, size_t* pPayloadSize, MQTTStatus_t cmock_to_return);
typedef MQTTStatus_t (* CMOCK_MQTT_GetSubAckStatusCodes_CALLBACK)(const MQTTPacketInfo_t* pSubackPacket, uint8_t** pPayloadStart, size_t* pPayloadSize, int cmock_num_calls);
void MQTT_GetSubAckStatusCodes_AddCallback(CMOCK_MQTT_GetSubAckStatusCodes_CALLBACK Callback);
void MQTT_GetSubAckStatusCodes_Stub(CMOCK_MQTT_GetSubAckStatusCodes_CALLBACK Callback);
#define MQTT_GetSubAckStatusCodes_StubWithCallback MQTT_GetSubAckStatusCodes_Stub
#define MQTT_GetSubAckStatusCodes_ExpectWithArrayAndReturn(pSubackPacket, pSubackPacket_Depth, pPayloadStart, pPayloadStart_Depth, pPayloadSize, pPayloadSize_Depth, cmock_retval) MQTT_GetSubAckStatusCodes_CMockExpectWithArrayAndReturn(__LINE__, pSubackPacket, pSubackPacket_Depth, pPayloadStart, pPayloadStart_Depth, pPayloadSize, pPayloadSize_Depth, cmock_retval)
void MQTT_GetSubAckStatusCodes_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const MQTTPacketInfo_t* pSubackPacket, int pSubackPacket_Depth, uint8_t** pPayloadStart, int pPayloadStart_Depth, size_t* pPayloadSize, int pPayloadSize_Depth, MQTTStatus_t cmock_to_return);
#define MQTT_GetSubAckStatusCodes_ReturnThruPtr_pPayloadStart(pPayloadStart) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadStart(__LINE__, pPayloadStart, sizeof(uint8_t*))
#define MQTT_GetSubAckStatusCodes_ReturnArrayThruPtr_pPayloadStart(pPayloadStart, cmock_len) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadStart(__LINE__, pPayloadStart, cmock_len * sizeof(*pPayloadStart))
#define MQTT_GetSubAckStatusCodes_ReturnMemThruPtr_pPayloadStart(pPayloadStart, cmock_size) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadStart(__LINE__, pPayloadStart, cmock_size)
void MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadStart(UNITY_LINE_TYPE cmock_line, uint8_t** pPayloadStart, size_t cmock_size);
#define MQTT_GetSubAckStatusCodes_ReturnThruPtr_pPayloadSize(pPayloadSize) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadSize(__LINE__, pPayloadSize, sizeof(size_t))
#define MQTT_GetSubAckStatusCodes_ReturnArrayThruPtr_pPayloadSize(pPayloadSize, cmock_len) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadSize(__LINE__, pPayloadSize, cmock_len * sizeof(*pPayloadSize))
#define MQTT_GetSubAckStatusCodes_ReturnMemThruPtr_pPayloadSize(pPayloadSize, cmock_size) MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadSize(__LINE__, pPayloadSize, cmock_size)
void MQTT_GetSubAckStatusCodes_CMockReturnMemThruPtr_pPayloadSize(UNITY_LINE_TYPE cmock_line, size_t* pPayloadSize, size_t cmock_size);
#define MQTT_GetSubAckStatusCodes_IgnoreArg_pSubackPacket() MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pSubackPacket(__LINE__)
void MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pSubackPacket(UNITY_LINE_TYPE cmock_line);
#define MQTT_GetSubAckStatusCodes_IgnoreArg_pPayloadStart() MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pPayloadStart(__LINE__)
void MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pPayloadStart(UNITY_LINE_TYPE cmock_line);
#define MQTT_GetSubAckStatusCodes_IgnoreArg_pPayloadSize() MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pPayloadSize(__LINE__)
void MQTT_GetSubAckStatusCodes_CMockIgnoreArg_pPayloadSize(UNITY_LINE_TYPE cmock_line);
#define MQTT_Status_strerror_IgnoreAndReturn(cmock_retval) MQTT_Status_strerror_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void MQTT_Status_strerror_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, const char* cmock_to_return);
#define MQTT_Status_strerror_StopIgnore() MQTT_Status_strerror_CMockStopIgnore()
void MQTT_Status_strerror_CMockStopIgnore(void);
#define MQTT_Status_strerror_ExpectAndReturn(status, cmock_retval) MQTT_Status_strerror_CMockExpectAndReturn(__LINE__, status, cmock_retval)
void MQTT_Status_strerror_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MQTTStatus_t status, const char* cmock_to_return);
typedef const char* (* CMOCK_MQTT_Status_strerror_CALLBACK)(MQTTStatus_t status, int cmock_num_calls);
void MQTT_Status_strerror_AddCallback(CMOCK_MQTT_Status_strerror_CALLBACK Callback);
void MQTT_Status_strerror_Stub(CMOCK_MQTT_Status_strerror_CALLBACK Callback);
#define MQTT_Status_strerror_StubWithCallback MQTT_Status_strerror_Stub
#define MQTT_Status_strerror_IgnoreArg_status() MQTT_Status_strerror_CMockIgnoreArg_status(__LINE__)
void MQTT_Status_strerror_CMockIgnoreArg_status(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
