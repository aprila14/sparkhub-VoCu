#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_TLS_FREERTOS_H
#define _MOCK_TLS_FREERTOS_H

#include "unity.h"
#include "tls_freertos.h"

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

void mock_tls_freertos_Init(void);
void mock_tls_freertos_Destroy(void);
void mock_tls_freertos_Verify(void);




#define TLS_FreeRTOS_Connect_IgnoreAndReturn(cmock_retval) TLS_FreeRTOS_Connect_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void TLS_FreeRTOS_Connect_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, TlsTransportStatus_t cmock_to_return);
#define TLS_FreeRTOS_Connect_StopIgnore() TLS_FreeRTOS_Connect_CMockStopIgnore()
void TLS_FreeRTOS_Connect_CMockStopIgnore(void);
#define TLS_FreeRTOS_Connect_ExpectAndReturn(pNetworkContext, pHostName, port, pNetworkCredentials, receiveTimeoutMs, sendTimeoutMs, cmock_retval) TLS_FreeRTOS_Connect_CMockExpectAndReturn(__LINE__, pNetworkContext, pHostName, port, pNetworkCredentials, receiveTimeoutMs, sendTimeoutMs, cmock_retval)
void TLS_FreeRTOS_Connect_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, const char* pHostName, uint16_t port, const NetworkCredentials_t* pNetworkCredentials, uint32_t receiveTimeoutMs, uint32_t sendTimeoutMs, TlsTransportStatus_t cmock_to_return);
typedef TlsTransportStatus_t (* CMOCK_TLS_FreeRTOS_Connect_CALLBACK)(NetworkContext_t* pNetworkContext, const char* pHostName, uint16_t port, const NetworkCredentials_t* pNetworkCredentials, uint32_t receiveTimeoutMs, uint32_t sendTimeoutMs, int cmock_num_calls);
void TLS_FreeRTOS_Connect_AddCallback(CMOCK_TLS_FreeRTOS_Connect_CALLBACK Callback);
void TLS_FreeRTOS_Connect_Stub(CMOCK_TLS_FreeRTOS_Connect_CALLBACK Callback);
#define TLS_FreeRTOS_Connect_StubWithCallback TLS_FreeRTOS_Connect_Stub
#define TLS_FreeRTOS_Connect_ExpectWithArrayAndReturn(pNetworkContext, pNetworkContext_Depth, pHostName, port, pNetworkCredentials, pNetworkCredentials_Depth, receiveTimeoutMs, sendTimeoutMs, cmock_retval) TLS_FreeRTOS_Connect_CMockExpectWithArrayAndReturn(__LINE__, pNetworkContext, pNetworkContext_Depth, pHostName, port, pNetworkCredentials, pNetworkCredentials_Depth, receiveTimeoutMs, sendTimeoutMs, cmock_retval)
void TLS_FreeRTOS_Connect_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, int pNetworkContext_Depth, const char* pHostName, uint16_t port, const NetworkCredentials_t* pNetworkCredentials, int pNetworkCredentials_Depth, uint32_t receiveTimeoutMs, uint32_t sendTimeoutMs, TlsTransportStatus_t cmock_to_return);
#define TLS_FreeRTOS_Connect_ReturnThruPtr_pNetworkContext(pNetworkContext) TLS_FreeRTOS_Connect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, sizeof(NetworkContext_t))
#define TLS_FreeRTOS_Connect_ReturnArrayThruPtr_pNetworkContext(pNetworkContext, cmock_len) TLS_FreeRTOS_Connect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_len * sizeof(*pNetworkContext))
#define TLS_FreeRTOS_Connect_ReturnMemThruPtr_pNetworkContext(pNetworkContext, cmock_size) TLS_FreeRTOS_Connect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_size)
void TLS_FreeRTOS_Connect_CMockReturnMemThruPtr_pNetworkContext(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, size_t cmock_size);
#define TLS_FreeRTOS_Connect_IgnoreArg_pNetworkContext() TLS_FreeRTOS_Connect_CMockIgnoreArg_pNetworkContext(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_pNetworkContext(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Connect_IgnoreArg_pHostName() TLS_FreeRTOS_Connect_CMockIgnoreArg_pHostName(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_pHostName(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Connect_IgnoreArg_port() TLS_FreeRTOS_Connect_CMockIgnoreArg_port(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_port(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Connect_IgnoreArg_pNetworkCredentials() TLS_FreeRTOS_Connect_CMockIgnoreArg_pNetworkCredentials(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_pNetworkCredentials(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Connect_IgnoreArg_receiveTimeoutMs() TLS_FreeRTOS_Connect_CMockIgnoreArg_receiveTimeoutMs(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_receiveTimeoutMs(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Connect_IgnoreArg_sendTimeoutMs() TLS_FreeRTOS_Connect_CMockIgnoreArg_sendTimeoutMs(__LINE__)
void TLS_FreeRTOS_Connect_CMockIgnoreArg_sendTimeoutMs(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_Disconnect_Ignore() TLS_FreeRTOS_Disconnect_CMockIgnore()
void TLS_FreeRTOS_Disconnect_CMockIgnore(void);
#define TLS_FreeRTOS_Disconnect_StopIgnore() TLS_FreeRTOS_Disconnect_CMockStopIgnore()
void TLS_FreeRTOS_Disconnect_CMockStopIgnore(void);
#define TLS_FreeRTOS_Disconnect_Expect(pNetworkContext) TLS_FreeRTOS_Disconnect_CMockExpect(__LINE__, pNetworkContext)
void TLS_FreeRTOS_Disconnect_CMockExpect(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext);
typedef void (* CMOCK_TLS_FreeRTOS_Disconnect_CALLBACK)(NetworkContext_t* pNetworkContext, int cmock_num_calls);
void TLS_FreeRTOS_Disconnect_AddCallback(CMOCK_TLS_FreeRTOS_Disconnect_CALLBACK Callback);
void TLS_FreeRTOS_Disconnect_Stub(CMOCK_TLS_FreeRTOS_Disconnect_CALLBACK Callback);
#define TLS_FreeRTOS_Disconnect_StubWithCallback TLS_FreeRTOS_Disconnect_Stub
#define TLS_FreeRTOS_Disconnect_ExpectWithArray(pNetworkContext, pNetworkContext_Depth) TLS_FreeRTOS_Disconnect_CMockExpectWithArray(__LINE__, pNetworkContext, pNetworkContext_Depth)
void TLS_FreeRTOS_Disconnect_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, int pNetworkContext_Depth);
#define TLS_FreeRTOS_Disconnect_ReturnThruPtr_pNetworkContext(pNetworkContext) TLS_FreeRTOS_Disconnect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, sizeof(NetworkContext_t))
#define TLS_FreeRTOS_Disconnect_ReturnArrayThruPtr_pNetworkContext(pNetworkContext, cmock_len) TLS_FreeRTOS_Disconnect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_len * sizeof(*pNetworkContext))
#define TLS_FreeRTOS_Disconnect_ReturnMemThruPtr_pNetworkContext(pNetworkContext, cmock_size) TLS_FreeRTOS_Disconnect_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_size)
void TLS_FreeRTOS_Disconnect_CMockReturnMemThruPtr_pNetworkContext(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, size_t cmock_size);
#define TLS_FreeRTOS_Disconnect_IgnoreArg_pNetworkContext() TLS_FreeRTOS_Disconnect_CMockIgnoreArg_pNetworkContext(__LINE__)
void TLS_FreeRTOS_Disconnect_CMockIgnoreArg_pNetworkContext(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_recv_IgnoreAndReturn(cmock_retval) TLS_FreeRTOS_recv_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void TLS_FreeRTOS_recv_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int32_t cmock_to_return);
#define TLS_FreeRTOS_recv_StopIgnore() TLS_FreeRTOS_recv_CMockStopIgnore()
void TLS_FreeRTOS_recv_CMockStopIgnore(void);
#define TLS_FreeRTOS_recv_ExpectAndReturn(pNetworkContext, pBuffer, bytesToRecv, cmock_retval) TLS_FreeRTOS_recv_CMockExpectAndReturn(__LINE__, pNetworkContext, pBuffer, bytesToRecv, cmock_retval)
void TLS_FreeRTOS_recv_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv, int32_t cmock_to_return);
typedef int32_t (* CMOCK_TLS_FreeRTOS_recv_CALLBACK)(NetworkContext_t* pNetworkContext, void* pBuffer, size_t bytesToRecv, int cmock_num_calls);
void TLS_FreeRTOS_recv_AddCallback(CMOCK_TLS_FreeRTOS_recv_CALLBACK Callback);
void TLS_FreeRTOS_recv_Stub(CMOCK_TLS_FreeRTOS_recv_CALLBACK Callback);
#define TLS_FreeRTOS_recv_StubWithCallback TLS_FreeRTOS_recv_Stub
#define TLS_FreeRTOS_recv_ExpectWithArrayAndReturn(pNetworkContext, pNetworkContext_Depth, pBuffer, pBuffer_Depth, bytesToRecv, cmock_retval) TLS_FreeRTOS_recv_CMockExpectWithArrayAndReturn(__LINE__, pNetworkContext, pNetworkContext_Depth, pBuffer, pBuffer_Depth, bytesToRecv, cmock_retval)
void TLS_FreeRTOS_recv_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, int pNetworkContext_Depth, void* pBuffer, int pBuffer_Depth, size_t bytesToRecv, int32_t cmock_to_return);
#define TLS_FreeRTOS_recv_ReturnThruPtr_pNetworkContext(pNetworkContext) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, sizeof(NetworkContext_t))
#define TLS_FreeRTOS_recv_ReturnArrayThruPtr_pNetworkContext(pNetworkContext, cmock_len) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_len * sizeof(*pNetworkContext))
#define TLS_FreeRTOS_recv_ReturnMemThruPtr_pNetworkContext(pNetworkContext, cmock_size) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_size)
void TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pNetworkContext(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, size_t cmock_size);
#define TLS_FreeRTOS_recv_ReturnThruPtr_pBuffer(pBuffer) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pBuffer(__LINE__, pBuffer, sizeof(void))
#define TLS_FreeRTOS_recv_ReturnArrayThruPtr_pBuffer(pBuffer, cmock_len) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pBuffer(__LINE__, pBuffer, cmock_len * sizeof(*pBuffer))
#define TLS_FreeRTOS_recv_ReturnMemThruPtr_pBuffer(pBuffer, cmock_size) TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pBuffer(__LINE__, pBuffer, cmock_size)
void TLS_FreeRTOS_recv_CMockReturnMemThruPtr_pBuffer(UNITY_LINE_TYPE cmock_line, void* pBuffer, size_t cmock_size);
#define TLS_FreeRTOS_recv_IgnoreArg_pNetworkContext() TLS_FreeRTOS_recv_CMockIgnoreArg_pNetworkContext(__LINE__)
void TLS_FreeRTOS_recv_CMockIgnoreArg_pNetworkContext(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_recv_IgnoreArg_pBuffer() TLS_FreeRTOS_recv_CMockIgnoreArg_pBuffer(__LINE__)
void TLS_FreeRTOS_recv_CMockIgnoreArg_pBuffer(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_recv_IgnoreArg_bytesToRecv() TLS_FreeRTOS_recv_CMockIgnoreArg_bytesToRecv(__LINE__)
void TLS_FreeRTOS_recv_CMockIgnoreArg_bytesToRecv(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_send_IgnoreAndReturn(cmock_retval) TLS_FreeRTOS_send_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void TLS_FreeRTOS_send_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int32_t cmock_to_return);
#define TLS_FreeRTOS_send_StopIgnore() TLS_FreeRTOS_send_CMockStopIgnore()
void TLS_FreeRTOS_send_CMockStopIgnore(void);
#define TLS_FreeRTOS_send_ExpectAndReturn(pNetworkContext, pBuffer, bytesToSend, cmock_retval) TLS_FreeRTOS_send_CMockExpectAndReturn(__LINE__, pNetworkContext, pBuffer, bytesToSend, cmock_retval)
void TLS_FreeRTOS_send_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend, int32_t cmock_to_return);
typedef int32_t (* CMOCK_TLS_FreeRTOS_send_CALLBACK)(NetworkContext_t* pNetworkContext, const void* pBuffer, size_t bytesToSend, int cmock_num_calls);
void TLS_FreeRTOS_send_AddCallback(CMOCK_TLS_FreeRTOS_send_CALLBACK Callback);
void TLS_FreeRTOS_send_Stub(CMOCK_TLS_FreeRTOS_send_CALLBACK Callback);
#define TLS_FreeRTOS_send_StubWithCallback TLS_FreeRTOS_send_Stub
#define TLS_FreeRTOS_send_ExpectWithArrayAndReturn(pNetworkContext, pNetworkContext_Depth, pBuffer, pBuffer_Depth, bytesToSend, cmock_retval) TLS_FreeRTOS_send_CMockExpectWithArrayAndReturn(__LINE__, pNetworkContext, pNetworkContext_Depth, pBuffer, pBuffer_Depth, bytesToSend, cmock_retval)
void TLS_FreeRTOS_send_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, int pNetworkContext_Depth, const void* pBuffer, int pBuffer_Depth, size_t bytesToSend, int32_t cmock_to_return);
#define TLS_FreeRTOS_send_ReturnThruPtr_pNetworkContext(pNetworkContext) TLS_FreeRTOS_send_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, sizeof(NetworkContext_t))
#define TLS_FreeRTOS_send_ReturnArrayThruPtr_pNetworkContext(pNetworkContext, cmock_len) TLS_FreeRTOS_send_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_len * sizeof(*pNetworkContext))
#define TLS_FreeRTOS_send_ReturnMemThruPtr_pNetworkContext(pNetworkContext, cmock_size) TLS_FreeRTOS_send_CMockReturnMemThruPtr_pNetworkContext(__LINE__, pNetworkContext, cmock_size)
void TLS_FreeRTOS_send_CMockReturnMemThruPtr_pNetworkContext(UNITY_LINE_TYPE cmock_line, NetworkContext_t* pNetworkContext, size_t cmock_size);
#define TLS_FreeRTOS_send_IgnoreArg_pNetworkContext() TLS_FreeRTOS_send_CMockIgnoreArg_pNetworkContext(__LINE__)
void TLS_FreeRTOS_send_CMockIgnoreArg_pNetworkContext(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_send_IgnoreArg_pBuffer() TLS_FreeRTOS_send_CMockIgnoreArg_pBuffer(__LINE__)
void TLS_FreeRTOS_send_CMockIgnoreArg_pBuffer(UNITY_LINE_TYPE cmock_line);
#define TLS_FreeRTOS_send_IgnoreArg_bytesToSend() TLS_FreeRTOS_send_CMockIgnoreArg_bytesToSend(__LINE__)
void TLS_FreeRTOS_send_CMockIgnoreArg_bytesToSend(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
