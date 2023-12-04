#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_OTA_PAL_H
#define _MOCK_OTA_PAL_H

#include "unity.h"
#include "ota_pal.h"

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

void mock_ota_pal_Init(void);
void mock_ota_pal_Destroy(void);
void mock_ota_pal_Verify(void);




#define otaPal_Abort_IgnoreAndReturn(cmock_retval) otaPal_Abort_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_Abort_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_Abort_StopIgnore() otaPal_Abort_CMockStopIgnore()
void otaPal_Abort_CMockStopIgnore(void);
#define otaPal_Abort_ExpectAndReturn(pFileContext, cmock_retval) otaPal_Abort_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_Abort_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_Abort_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_Abort_AddCallback(CMOCK_otaPal_Abort_CALLBACK Callback);
void otaPal_Abort_Stub(CMOCK_otaPal_Abort_CALLBACK Callback);
#define otaPal_Abort_StubWithCallback otaPal_Abort_Stub
#define otaPal_Abort_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_Abort_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_Abort_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalStatus_t cmock_to_return);
#define otaPal_Abort_ReturnThruPtr_pFileContext(pFileContext) otaPal_Abort_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_Abort_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_Abort_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_Abort_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_Abort_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_Abort_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_Abort_IgnoreArg_pFileContext() otaPal_Abort_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_Abort_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_CreateFileForRx_IgnoreAndReturn(cmock_retval) otaPal_CreateFileForRx_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_CreateFileForRx_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_CreateFileForRx_StopIgnore() otaPal_CreateFileForRx_CMockStopIgnore()
void otaPal_CreateFileForRx_CMockStopIgnore(void);
#define otaPal_CreateFileForRx_ExpectAndReturn(pFileContext, cmock_retval) otaPal_CreateFileForRx_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_CreateFileForRx_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_CreateFileForRx_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_CreateFileForRx_AddCallback(CMOCK_otaPal_CreateFileForRx_CALLBACK Callback);
void otaPal_CreateFileForRx_Stub(CMOCK_otaPal_CreateFileForRx_CALLBACK Callback);
#define otaPal_CreateFileForRx_StubWithCallback otaPal_CreateFileForRx_Stub
#define otaPal_CreateFileForRx_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_CreateFileForRx_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_CreateFileForRx_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalStatus_t cmock_to_return);
#define otaPal_CreateFileForRx_ReturnThruPtr_pFileContext(pFileContext) otaPal_CreateFileForRx_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_CreateFileForRx_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_CreateFileForRx_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_CreateFileForRx_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_CreateFileForRx_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_CreateFileForRx_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_CreateFileForRx_IgnoreArg_pFileContext() otaPal_CreateFileForRx_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_CreateFileForRx_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_CloseFile_IgnoreAndReturn(cmock_retval) otaPal_CloseFile_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_CloseFile_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_CloseFile_StopIgnore() otaPal_CloseFile_CMockStopIgnore()
void otaPal_CloseFile_CMockStopIgnore(void);
#define otaPal_CloseFile_ExpectAndReturn(pFileContext, cmock_retval) otaPal_CloseFile_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_CloseFile_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_CloseFile_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_CloseFile_AddCallback(CMOCK_otaPal_CloseFile_CALLBACK Callback);
void otaPal_CloseFile_Stub(CMOCK_otaPal_CloseFile_CALLBACK Callback);
#define otaPal_CloseFile_StubWithCallback otaPal_CloseFile_Stub
#define otaPal_CloseFile_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_CloseFile_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_CloseFile_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalStatus_t cmock_to_return);
#define otaPal_CloseFile_ReturnThruPtr_pFileContext(pFileContext) otaPal_CloseFile_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_CloseFile_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_CloseFile_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_CloseFile_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_CloseFile_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_CloseFile_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_CloseFile_IgnoreArg_pFileContext() otaPal_CloseFile_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_CloseFile_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_WriteBlock_IgnoreAndReturn(cmock_retval) otaPal_WriteBlock_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_WriteBlock_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int16_t cmock_to_return);
#define otaPal_WriteBlock_StopIgnore() otaPal_WriteBlock_CMockStopIgnore()
void otaPal_WriteBlock_CMockStopIgnore(void);
#define otaPal_WriteBlock_ExpectAndReturn(pFileContext, ulOffset, pData, ulBlockSize, cmock_retval) otaPal_WriteBlock_CMockExpectAndReturn(__LINE__, pFileContext, ulOffset, pData, ulBlockSize, cmock_retval)
void otaPal_WriteBlock_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, uint32_t ulOffset, uint8_t* const pData, uint32_t ulBlockSize, int16_t cmock_to_return);
typedef int16_t (* CMOCK_otaPal_WriteBlock_CALLBACK)(OtaFileContext_t* const pFileContext, uint32_t ulOffset, uint8_t* const pData, uint32_t ulBlockSize, int cmock_num_calls);
void otaPal_WriteBlock_AddCallback(CMOCK_otaPal_WriteBlock_CALLBACK Callback);
void otaPal_WriteBlock_Stub(CMOCK_otaPal_WriteBlock_CALLBACK Callback);
#define otaPal_WriteBlock_StubWithCallback otaPal_WriteBlock_Stub
#define otaPal_WriteBlock_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, ulOffset, pData, pData_Depth, ulBlockSize, cmock_retval) otaPal_WriteBlock_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, ulOffset, pData, pData_Depth, ulBlockSize, cmock_retval)
void otaPal_WriteBlock_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, uint32_t ulOffset, uint8_t* const pData, int pData_Depth, uint32_t ulBlockSize, int16_t cmock_to_return);
#define otaPal_WriteBlock_ReturnThruPtr_pFileContext(pFileContext) otaPal_WriteBlock_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_WriteBlock_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_WriteBlock_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_WriteBlock_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_WriteBlock_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_WriteBlock_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_WriteBlock_ReturnThruPtr_pData(pData) otaPal_WriteBlock_CMockReturnMemThruPtr_pData(__LINE__, pData, sizeof(uint8_t))
#define otaPal_WriteBlock_ReturnArrayThruPtr_pData(pData, cmock_len) otaPal_WriteBlock_CMockReturnMemThruPtr_pData(__LINE__, pData, cmock_len * sizeof(*pData))
#define otaPal_WriteBlock_ReturnMemThruPtr_pData(pData, cmock_size) otaPal_WriteBlock_CMockReturnMemThruPtr_pData(__LINE__, pData, cmock_size)
void otaPal_WriteBlock_CMockReturnMemThruPtr_pData(UNITY_LINE_TYPE cmock_line, uint8_t* pData, size_t cmock_size);
#define otaPal_WriteBlock_IgnoreArg_pFileContext() otaPal_WriteBlock_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_WriteBlock_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_WriteBlock_IgnoreArg_ulOffset() otaPal_WriteBlock_CMockIgnoreArg_ulOffset(__LINE__)
void otaPal_WriteBlock_CMockIgnoreArg_ulOffset(UNITY_LINE_TYPE cmock_line);
#define otaPal_WriteBlock_IgnoreArg_pData() otaPal_WriteBlock_CMockIgnoreArg_pData(__LINE__)
void otaPal_WriteBlock_CMockIgnoreArg_pData(UNITY_LINE_TYPE cmock_line);
#define otaPal_WriteBlock_IgnoreArg_ulBlockSize() otaPal_WriteBlock_CMockIgnoreArg_ulBlockSize(__LINE__)
void otaPal_WriteBlock_CMockIgnoreArg_ulBlockSize(UNITY_LINE_TYPE cmock_line);
#define otaPal_ActivateNewImage_IgnoreAndReturn(cmock_retval) otaPal_ActivateNewImage_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_ActivateNewImage_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_ActivateNewImage_StopIgnore() otaPal_ActivateNewImage_CMockStopIgnore()
void otaPal_ActivateNewImage_CMockStopIgnore(void);
#define otaPal_ActivateNewImage_ExpectAndReturn(pFileContext, cmock_retval) otaPal_ActivateNewImage_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_ActivateNewImage_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_ActivateNewImage_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_ActivateNewImage_AddCallback(CMOCK_otaPal_ActivateNewImage_CALLBACK Callback);
void otaPal_ActivateNewImage_Stub(CMOCK_otaPal_ActivateNewImage_CALLBACK Callback);
#define otaPal_ActivateNewImage_StubWithCallback otaPal_ActivateNewImage_Stub
#define otaPal_ActivateNewImage_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_ActivateNewImage_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_ActivateNewImage_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalStatus_t cmock_to_return);
#define otaPal_ActivateNewImage_ReturnThruPtr_pFileContext(pFileContext) otaPal_ActivateNewImage_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_ActivateNewImage_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_ActivateNewImage_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_ActivateNewImage_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_ActivateNewImage_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_ActivateNewImage_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_ActivateNewImage_IgnoreArg_pFileContext() otaPal_ActivateNewImage_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_ActivateNewImage_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_ResetDevice_IgnoreAndReturn(cmock_retval) otaPal_ResetDevice_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_ResetDevice_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_ResetDevice_StopIgnore() otaPal_ResetDevice_CMockStopIgnore()
void otaPal_ResetDevice_CMockStopIgnore(void);
#define otaPal_ResetDevice_ExpectAndReturn(pFileContext, cmock_retval) otaPal_ResetDevice_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_ResetDevice_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_ResetDevice_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_ResetDevice_AddCallback(CMOCK_otaPal_ResetDevice_CALLBACK Callback);
void otaPal_ResetDevice_Stub(CMOCK_otaPal_ResetDevice_CALLBACK Callback);
#define otaPal_ResetDevice_StubWithCallback otaPal_ResetDevice_Stub
#define otaPal_ResetDevice_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_ResetDevice_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_ResetDevice_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalStatus_t cmock_to_return);
#define otaPal_ResetDevice_ReturnThruPtr_pFileContext(pFileContext) otaPal_ResetDevice_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_ResetDevice_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_ResetDevice_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_ResetDevice_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_ResetDevice_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_ResetDevice_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_ResetDevice_IgnoreArg_pFileContext() otaPal_ResetDevice_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_ResetDevice_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_SetPlatformImageState_IgnoreAndReturn(cmock_retval) otaPal_SetPlatformImageState_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_SetPlatformImageState_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalStatus_t cmock_to_return);
#define otaPal_SetPlatformImageState_StopIgnore() otaPal_SetPlatformImageState_CMockStopIgnore()
void otaPal_SetPlatformImageState_CMockStopIgnore(void);
#define otaPal_SetPlatformImageState_ExpectAndReturn(pFileContext, eState, cmock_retval) otaPal_SetPlatformImageState_CMockExpectAndReturn(__LINE__, pFileContext, eState, cmock_retval)
void otaPal_SetPlatformImageState_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaImageState_t eState, OtaPalStatus_t cmock_to_return);
typedef OtaPalStatus_t (* CMOCK_otaPal_SetPlatformImageState_CALLBACK)(OtaFileContext_t* const pFileContext, OtaImageState_t eState, int cmock_num_calls);
void otaPal_SetPlatformImageState_AddCallback(CMOCK_otaPal_SetPlatformImageState_CALLBACK Callback);
void otaPal_SetPlatformImageState_Stub(CMOCK_otaPal_SetPlatformImageState_CALLBACK Callback);
#define otaPal_SetPlatformImageState_StubWithCallback otaPal_SetPlatformImageState_Stub
#define otaPal_SetPlatformImageState_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, eState, cmock_retval) otaPal_SetPlatformImageState_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, eState, cmock_retval)
void otaPal_SetPlatformImageState_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaImageState_t eState, OtaPalStatus_t cmock_to_return);
#define otaPal_SetPlatformImageState_ReturnThruPtr_pFileContext(pFileContext) otaPal_SetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_SetPlatformImageState_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_SetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_SetPlatformImageState_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_SetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_SetPlatformImageState_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_SetPlatformImageState_IgnoreArg_pFileContext() otaPal_SetPlatformImageState_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_SetPlatformImageState_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);
#define otaPal_SetPlatformImageState_IgnoreArg_eState() otaPal_SetPlatformImageState_CMockIgnoreArg_eState(__LINE__)
void otaPal_SetPlatformImageState_CMockIgnoreArg_eState(UNITY_LINE_TYPE cmock_line);
#define otaPal_GetPlatformImageState_IgnoreAndReturn(cmock_retval) otaPal_GetPlatformImageState_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void otaPal_GetPlatformImageState_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, OtaPalImageState_t cmock_to_return);
#define otaPal_GetPlatformImageState_StopIgnore() otaPal_GetPlatformImageState_CMockStopIgnore()
void otaPal_GetPlatformImageState_CMockStopIgnore(void);
#define otaPal_GetPlatformImageState_ExpectAndReturn(pFileContext, cmock_retval) otaPal_GetPlatformImageState_CMockExpectAndReturn(__LINE__, pFileContext, cmock_retval)
void otaPal_GetPlatformImageState_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, OtaPalImageState_t cmock_to_return);
typedef OtaPalImageState_t (* CMOCK_otaPal_GetPlatformImageState_CALLBACK)(OtaFileContext_t* const pFileContext, int cmock_num_calls);
void otaPal_GetPlatformImageState_AddCallback(CMOCK_otaPal_GetPlatformImageState_CALLBACK Callback);
void otaPal_GetPlatformImageState_Stub(CMOCK_otaPal_GetPlatformImageState_CALLBACK Callback);
#define otaPal_GetPlatformImageState_StubWithCallback otaPal_GetPlatformImageState_Stub
#define otaPal_GetPlatformImageState_ExpectWithArrayAndReturn(pFileContext, pFileContext_Depth, cmock_retval) otaPal_GetPlatformImageState_CMockExpectWithArrayAndReturn(__LINE__, pFileContext, pFileContext_Depth, cmock_retval)
void otaPal_GetPlatformImageState_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* const pFileContext, int pFileContext_Depth, OtaPalImageState_t cmock_to_return);
#define otaPal_GetPlatformImageState_ReturnThruPtr_pFileContext(pFileContext) otaPal_GetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, sizeof(OtaFileContext_t))
#define otaPal_GetPlatformImageState_ReturnArrayThruPtr_pFileContext(pFileContext, cmock_len) otaPal_GetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_len * sizeof(*pFileContext))
#define otaPal_GetPlatformImageState_ReturnMemThruPtr_pFileContext(pFileContext, cmock_size) otaPal_GetPlatformImageState_CMockReturnMemThruPtr_pFileContext(__LINE__, pFileContext, cmock_size)
void otaPal_GetPlatformImageState_CMockReturnMemThruPtr_pFileContext(UNITY_LINE_TYPE cmock_line, OtaFileContext_t* pFileContext, size_t cmock_size);
#define otaPal_GetPlatformImageState_IgnoreArg_pFileContext() otaPal_GetPlatformImageState_CMockIgnoreArg_pFileContext(__LINE__)
void otaPal_GetPlatformImageState_CMockIgnoreArg_pFileContext(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif