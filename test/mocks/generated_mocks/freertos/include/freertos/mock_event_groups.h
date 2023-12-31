#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_EVENT_GROUPS_H
#define _MOCK_EVENT_GROUPS_H

#include "unity.h"
#include "event_groups.h"

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

void mock_event_groups_Init(void);
void mock_event_groups_Destroy(void);
void mock_event_groups_Verify(void);




#define xEventGroupCreate_IgnoreAndReturn(cmock_retval) xEventGroupCreate_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupCreate_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t cmock_to_return);
#define xEventGroupCreate_StopIgnore() xEventGroupCreate_CMockStopIgnore()
void xEventGroupCreate_CMockStopIgnore(void);
#define xEventGroupCreate_ExpectAndReturn(cmock_retval) xEventGroupCreate_CMockExpectAndReturn(__LINE__, cmock_retval)
void xEventGroupCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t cmock_to_return);
typedef EventGroupHandle_t (* CMOCK_xEventGroupCreate_CALLBACK)(int cmock_num_calls);
void xEventGroupCreate_AddCallback(CMOCK_xEventGroupCreate_CALLBACK Callback);
void xEventGroupCreate_Stub(CMOCK_xEventGroupCreate_CALLBACK Callback);
#define xEventGroupCreate_StubWithCallback xEventGroupCreate_Stub
#define xEventGroupCreateStatic_IgnoreAndReturn(cmock_retval) xEventGroupCreateStatic_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupCreateStatic_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t cmock_to_return);
#define xEventGroupCreateStatic_StopIgnore() xEventGroupCreateStatic_CMockStopIgnore()
void xEventGroupCreateStatic_CMockStopIgnore(void);
#define xEventGroupCreateStatic_ExpectAndReturn(pxEventGroupBuffer, cmock_retval) xEventGroupCreateStatic_CMockExpectAndReturn(__LINE__, pxEventGroupBuffer, cmock_retval)
void xEventGroupCreateStatic_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, StaticEventGroup_t* pxEventGroupBuffer, EventGroupHandle_t cmock_to_return);
typedef EventGroupHandle_t (* CMOCK_xEventGroupCreateStatic_CALLBACK)(StaticEventGroup_t* pxEventGroupBuffer, int cmock_num_calls);
void xEventGroupCreateStatic_AddCallback(CMOCK_xEventGroupCreateStatic_CALLBACK Callback);
void xEventGroupCreateStatic_Stub(CMOCK_xEventGroupCreateStatic_CALLBACK Callback);
#define xEventGroupCreateStatic_StubWithCallback xEventGroupCreateStatic_Stub
#define xEventGroupCreateStatic_ExpectWithArrayAndReturn(pxEventGroupBuffer, pxEventGroupBuffer_Depth, cmock_retval) xEventGroupCreateStatic_CMockExpectWithArrayAndReturn(__LINE__, pxEventGroupBuffer, pxEventGroupBuffer_Depth, cmock_retval)
void xEventGroupCreateStatic_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, StaticEventGroup_t* pxEventGroupBuffer, int pxEventGroupBuffer_Depth, EventGroupHandle_t cmock_to_return);
#define xEventGroupCreateStatic_ReturnThruPtr_pxEventGroupBuffer(pxEventGroupBuffer) xEventGroupCreateStatic_CMockReturnMemThruPtr_pxEventGroupBuffer(__LINE__, pxEventGroupBuffer, sizeof(StaticEventGroup_t))
#define xEventGroupCreateStatic_ReturnArrayThruPtr_pxEventGroupBuffer(pxEventGroupBuffer, cmock_len) xEventGroupCreateStatic_CMockReturnMemThruPtr_pxEventGroupBuffer(__LINE__, pxEventGroupBuffer, cmock_len * sizeof(*pxEventGroupBuffer))
#define xEventGroupCreateStatic_ReturnMemThruPtr_pxEventGroupBuffer(pxEventGroupBuffer, cmock_size) xEventGroupCreateStatic_CMockReturnMemThruPtr_pxEventGroupBuffer(__LINE__, pxEventGroupBuffer, cmock_size)
void xEventGroupCreateStatic_CMockReturnMemThruPtr_pxEventGroupBuffer(UNITY_LINE_TYPE cmock_line, StaticEventGroup_t* pxEventGroupBuffer, size_t cmock_size);
#define xEventGroupCreateStatic_IgnoreArg_pxEventGroupBuffer() xEventGroupCreateStatic_CMockIgnoreArg_pxEventGroupBuffer(__LINE__)
void xEventGroupCreateStatic_CMockIgnoreArg_pxEventGroupBuffer(UNITY_LINE_TYPE cmock_line);
#define xEventGroupWaitBits_IgnoreAndReturn(cmock_retval) xEventGroupWaitBits_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupWaitBits_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventBits_t cmock_to_return);
#define xEventGroupWaitBits_StopIgnore() xEventGroupWaitBits_CMockStopIgnore()
void xEventGroupWaitBits_CMockStopIgnore(void);
#define xEventGroupWaitBits_ExpectAndReturn(xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait, cmock_retval) xEventGroupWaitBits_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait, cmock_retval)
void xEventGroupWaitBits_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait, EventBits_t cmock_to_return);
typedef EventBits_t (* CMOCK_xEventGroupWaitBits_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait, int cmock_num_calls);
void xEventGroupWaitBits_AddCallback(CMOCK_xEventGroupWaitBits_CALLBACK Callback);
void xEventGroupWaitBits_Stub(CMOCK_xEventGroupWaitBits_CALLBACK Callback);
#define xEventGroupWaitBits_StubWithCallback xEventGroupWaitBits_Stub
#define xEventGroupWaitBits_IgnoreArg_xEventGroup() xEventGroupWaitBits_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupWaitBits_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupWaitBits_IgnoreArg_uxBitsToWaitFor() xEventGroupWaitBits_CMockIgnoreArg_uxBitsToWaitFor(__LINE__)
void xEventGroupWaitBits_CMockIgnoreArg_uxBitsToWaitFor(UNITY_LINE_TYPE cmock_line);
#define xEventGroupWaitBits_IgnoreArg_xClearOnExit() xEventGroupWaitBits_CMockIgnoreArg_xClearOnExit(__LINE__)
void xEventGroupWaitBits_CMockIgnoreArg_xClearOnExit(UNITY_LINE_TYPE cmock_line);
#define xEventGroupWaitBits_IgnoreArg_xWaitForAllBits() xEventGroupWaitBits_CMockIgnoreArg_xWaitForAllBits(__LINE__)
void xEventGroupWaitBits_CMockIgnoreArg_xWaitForAllBits(UNITY_LINE_TYPE cmock_line);
#define xEventGroupWaitBits_IgnoreArg_xTicksToWait() xEventGroupWaitBits_CMockIgnoreArg_xTicksToWait(__LINE__)
void xEventGroupWaitBits_CMockIgnoreArg_xTicksToWait(UNITY_LINE_TYPE cmock_line);
#define xEventGroupClearBits_IgnoreAndReturn(cmock_retval) xEventGroupClearBits_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupClearBits_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventBits_t cmock_to_return);
#define xEventGroupClearBits_StopIgnore() xEventGroupClearBits_CMockStopIgnore()
void xEventGroupClearBits_CMockStopIgnore(void);
#define xEventGroupClearBits_ExpectAndReturn(xEventGroup, uxBitsToClear, cmock_retval) xEventGroupClearBits_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToClear, cmock_retval)
void xEventGroupClearBits_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear, EventBits_t cmock_to_return);
typedef EventBits_t (* CMOCK_xEventGroupClearBits_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear, int cmock_num_calls);
void xEventGroupClearBits_AddCallback(CMOCK_xEventGroupClearBits_CALLBACK Callback);
void xEventGroupClearBits_Stub(CMOCK_xEventGroupClearBits_CALLBACK Callback);
#define xEventGroupClearBits_StubWithCallback xEventGroupClearBits_Stub
#define xEventGroupClearBits_IgnoreArg_xEventGroup() xEventGroupClearBits_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupClearBits_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupClearBits_IgnoreArg_uxBitsToClear() xEventGroupClearBits_CMockIgnoreArg_uxBitsToClear(__LINE__)
void xEventGroupClearBits_CMockIgnoreArg_uxBitsToClear(UNITY_LINE_TYPE cmock_line);
#define xEventGroupClearBitsFromISR_IgnoreAndReturn(cmock_retval) xEventGroupClearBitsFromISR_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupClearBitsFromISR_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BaseType_t cmock_to_return);
#define xEventGroupClearBitsFromISR_StopIgnore() xEventGroupClearBitsFromISR_CMockStopIgnore()
void xEventGroupClearBitsFromISR_CMockStopIgnore(void);
#define xEventGroupClearBitsFromISR_ExpectAndReturn(xEventGroup, uxBitsToClear, cmock_retval) xEventGroupClearBitsFromISR_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToClear, cmock_retval)
void xEventGroupClearBitsFromISR_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear, BaseType_t cmock_to_return);
typedef BaseType_t (* CMOCK_xEventGroupClearBitsFromISR_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear, int cmock_num_calls);
void xEventGroupClearBitsFromISR_AddCallback(CMOCK_xEventGroupClearBitsFromISR_CALLBACK Callback);
void xEventGroupClearBitsFromISR_Stub(CMOCK_xEventGroupClearBitsFromISR_CALLBACK Callback);
#define xEventGroupClearBitsFromISR_StubWithCallback xEventGroupClearBitsFromISR_Stub
#define xEventGroupClearBitsFromISR_IgnoreArg_xEventGroup() xEventGroupClearBitsFromISR_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupClearBitsFromISR_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupClearBitsFromISR_IgnoreArg_uxBitsToClear() xEventGroupClearBitsFromISR_CMockIgnoreArg_uxBitsToClear(__LINE__)
void xEventGroupClearBitsFromISR_CMockIgnoreArg_uxBitsToClear(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSetBits_IgnoreAndReturn(cmock_retval) xEventGroupSetBits_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupSetBits_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventBits_t cmock_to_return);
#define xEventGroupSetBits_StopIgnore() xEventGroupSetBits_CMockStopIgnore()
void xEventGroupSetBits_CMockStopIgnore(void);
#define xEventGroupSetBits_ExpectAndReturn(xEventGroup, uxBitsToSet, cmock_retval) xEventGroupSetBits_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToSet, cmock_retval)
void xEventGroupSetBits_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, EventBits_t cmock_to_return);
typedef EventBits_t (* CMOCK_xEventGroupSetBits_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, int cmock_num_calls);
void xEventGroupSetBits_AddCallback(CMOCK_xEventGroupSetBits_CALLBACK Callback);
void xEventGroupSetBits_Stub(CMOCK_xEventGroupSetBits_CALLBACK Callback);
#define xEventGroupSetBits_StubWithCallback xEventGroupSetBits_Stub
#define xEventGroupSetBits_IgnoreArg_xEventGroup() xEventGroupSetBits_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupSetBits_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSetBits_IgnoreArg_uxBitsToSet() xEventGroupSetBits_CMockIgnoreArg_uxBitsToSet(__LINE__)
void xEventGroupSetBits_CMockIgnoreArg_uxBitsToSet(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSetBitsFromISR_IgnoreAndReturn(cmock_retval) xEventGroupSetBitsFromISR_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupSetBitsFromISR_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BaseType_t cmock_to_return);
#define xEventGroupSetBitsFromISR_StopIgnore() xEventGroupSetBitsFromISR_CMockStopIgnore()
void xEventGroupSetBitsFromISR_CMockStopIgnore(void);
#define xEventGroupSetBitsFromISR_ExpectAndReturn(xEventGroup, uxBitsToSet, pxHigherPriorityTaskWoken, cmock_retval) xEventGroupSetBitsFromISR_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToSet, pxHigherPriorityTaskWoken, cmock_retval)
void xEventGroupSetBitsFromISR_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, BaseType_t* pxHigherPriorityTaskWoken, BaseType_t cmock_to_return);
typedef BaseType_t (* CMOCK_xEventGroupSetBitsFromISR_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, BaseType_t* pxHigherPriorityTaskWoken, int cmock_num_calls);
void xEventGroupSetBitsFromISR_AddCallback(CMOCK_xEventGroupSetBitsFromISR_CALLBACK Callback);
void xEventGroupSetBitsFromISR_Stub(CMOCK_xEventGroupSetBitsFromISR_CALLBACK Callback);
#define xEventGroupSetBitsFromISR_StubWithCallback xEventGroupSetBitsFromISR_Stub
#define xEventGroupSetBitsFromISR_ExpectWithArrayAndReturn(xEventGroup, uxBitsToSet, pxHigherPriorityTaskWoken, pxHigherPriorityTaskWoken_Depth, cmock_retval) xEventGroupSetBitsFromISR_CMockExpectWithArrayAndReturn(__LINE__, xEventGroup, uxBitsToSet, pxHigherPriorityTaskWoken, pxHigherPriorityTaskWoken_Depth, cmock_retval)
void xEventGroupSetBitsFromISR_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, BaseType_t* pxHigherPriorityTaskWoken, int pxHigherPriorityTaskWoken_Depth, BaseType_t cmock_to_return);
#define xEventGroupSetBitsFromISR_ReturnThruPtr_pxHigherPriorityTaskWoken(pxHigherPriorityTaskWoken) xEventGroupSetBitsFromISR_CMockReturnMemThruPtr_pxHigherPriorityTaskWoken(__LINE__, pxHigherPriorityTaskWoken, sizeof(BaseType_t))
#define xEventGroupSetBitsFromISR_ReturnArrayThruPtr_pxHigherPriorityTaskWoken(pxHigherPriorityTaskWoken, cmock_len) xEventGroupSetBitsFromISR_CMockReturnMemThruPtr_pxHigherPriorityTaskWoken(__LINE__, pxHigherPriorityTaskWoken, cmock_len * sizeof(*pxHigherPriorityTaskWoken))
#define xEventGroupSetBitsFromISR_ReturnMemThruPtr_pxHigherPriorityTaskWoken(pxHigherPriorityTaskWoken, cmock_size) xEventGroupSetBitsFromISR_CMockReturnMemThruPtr_pxHigherPriorityTaskWoken(__LINE__, pxHigherPriorityTaskWoken, cmock_size)
void xEventGroupSetBitsFromISR_CMockReturnMemThruPtr_pxHigherPriorityTaskWoken(UNITY_LINE_TYPE cmock_line, BaseType_t* pxHigherPriorityTaskWoken, size_t cmock_size);
#define xEventGroupSetBitsFromISR_IgnoreArg_xEventGroup() xEventGroupSetBitsFromISR_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupSetBitsFromISR_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSetBitsFromISR_IgnoreArg_uxBitsToSet() xEventGroupSetBitsFromISR_CMockIgnoreArg_uxBitsToSet(__LINE__)
void xEventGroupSetBitsFromISR_CMockIgnoreArg_uxBitsToSet(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSetBitsFromISR_IgnoreArg_pxHigherPriorityTaskWoken() xEventGroupSetBitsFromISR_CMockIgnoreArg_pxHigherPriorityTaskWoken(__LINE__)
void xEventGroupSetBitsFromISR_CMockIgnoreArg_pxHigherPriorityTaskWoken(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSync_IgnoreAndReturn(cmock_retval) xEventGroupSync_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupSync_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventBits_t cmock_to_return);
#define xEventGroupSync_StopIgnore() xEventGroupSync_CMockStopIgnore()
void xEventGroupSync_CMockStopIgnore(void);
#define xEventGroupSync_ExpectAndReturn(xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait, cmock_retval) xEventGroupSync_CMockExpectAndReturn(__LINE__, xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait, cmock_retval)
void xEventGroupSync_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor, TickType_t xTicksToWait, EventBits_t cmock_to_return);
typedef EventBits_t (* CMOCK_xEventGroupSync_CALLBACK)(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor, TickType_t xTicksToWait, int cmock_num_calls);
void xEventGroupSync_AddCallback(CMOCK_xEventGroupSync_CALLBACK Callback);
void xEventGroupSync_Stub(CMOCK_xEventGroupSync_CALLBACK Callback);
#define xEventGroupSync_StubWithCallback xEventGroupSync_Stub
#define xEventGroupSync_IgnoreArg_xEventGroup() xEventGroupSync_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupSync_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSync_IgnoreArg_uxBitsToSet() xEventGroupSync_CMockIgnoreArg_uxBitsToSet(__LINE__)
void xEventGroupSync_CMockIgnoreArg_uxBitsToSet(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSync_IgnoreArg_uxBitsToWaitFor() xEventGroupSync_CMockIgnoreArg_uxBitsToWaitFor(__LINE__)
void xEventGroupSync_CMockIgnoreArg_uxBitsToWaitFor(UNITY_LINE_TYPE cmock_line);
#define xEventGroupSync_IgnoreArg_xTicksToWait() xEventGroupSync_CMockIgnoreArg_xTicksToWait(__LINE__)
void xEventGroupSync_CMockIgnoreArg_xTicksToWait(UNITY_LINE_TYPE cmock_line);
#define xEventGroupGetBitsFromISR_IgnoreAndReturn(cmock_retval) xEventGroupGetBitsFromISR_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xEventGroupGetBitsFromISR_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, EventBits_t cmock_to_return);
#define xEventGroupGetBitsFromISR_StopIgnore() xEventGroupGetBitsFromISR_CMockStopIgnore()
void xEventGroupGetBitsFromISR_CMockStopIgnore(void);
#define xEventGroupGetBitsFromISR_ExpectAndReturn(xEventGroup, cmock_retval) xEventGroupGetBitsFromISR_CMockExpectAndReturn(__LINE__, xEventGroup, cmock_retval)
void xEventGroupGetBitsFromISR_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup, EventBits_t cmock_to_return);
typedef EventBits_t (* CMOCK_xEventGroupGetBitsFromISR_CALLBACK)(EventGroupHandle_t xEventGroup, int cmock_num_calls);
void xEventGroupGetBitsFromISR_AddCallback(CMOCK_xEventGroupGetBitsFromISR_CALLBACK Callback);
void xEventGroupGetBitsFromISR_Stub(CMOCK_xEventGroupGetBitsFromISR_CALLBACK Callback);
#define xEventGroupGetBitsFromISR_StubWithCallback xEventGroupGetBitsFromISR_Stub
#define xEventGroupGetBitsFromISR_IgnoreArg_xEventGroup() xEventGroupGetBitsFromISR_CMockIgnoreArg_xEventGroup(__LINE__)
void xEventGroupGetBitsFromISR_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupDelete_Ignore() vEventGroupDelete_CMockIgnore()
void vEventGroupDelete_CMockIgnore(void);
#define vEventGroupDelete_StopIgnore() vEventGroupDelete_CMockStopIgnore()
void vEventGroupDelete_CMockStopIgnore(void);
#define vEventGroupDelete_Expect(xEventGroup) vEventGroupDelete_CMockExpect(__LINE__, xEventGroup)
void vEventGroupDelete_CMockExpect(UNITY_LINE_TYPE cmock_line, EventGroupHandle_t xEventGroup);
typedef void (* CMOCK_vEventGroupDelete_CALLBACK)(EventGroupHandle_t xEventGroup, int cmock_num_calls);
void vEventGroupDelete_AddCallback(CMOCK_vEventGroupDelete_CALLBACK Callback);
void vEventGroupDelete_Stub(CMOCK_vEventGroupDelete_CALLBACK Callback);
#define vEventGroupDelete_StubWithCallback vEventGroupDelete_Stub
#define vEventGroupDelete_IgnoreArg_xEventGroup() vEventGroupDelete_CMockIgnoreArg_xEventGroup(__LINE__)
void vEventGroupDelete_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupSetBitsCallback_Ignore() vEventGroupSetBitsCallback_CMockIgnore()
void vEventGroupSetBitsCallback_CMockIgnore(void);
#define vEventGroupSetBitsCallback_StopIgnore() vEventGroupSetBitsCallback_CMockStopIgnore()
void vEventGroupSetBitsCallback_CMockStopIgnore(void);
#define vEventGroupSetBitsCallback_Expect(pvEventGroup, ulBitsToSet) vEventGroupSetBitsCallback_CMockExpect(__LINE__, pvEventGroup, ulBitsToSet)
void vEventGroupSetBitsCallback_CMockExpect(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, const uint32_t ulBitsToSet);
typedef void (* CMOCK_vEventGroupSetBitsCallback_CALLBACK)(void* pvEventGroup, const uint32_t ulBitsToSet, int cmock_num_calls);
void vEventGroupSetBitsCallback_AddCallback(CMOCK_vEventGroupSetBitsCallback_CALLBACK Callback);
void vEventGroupSetBitsCallback_Stub(CMOCK_vEventGroupSetBitsCallback_CALLBACK Callback);
#define vEventGroupSetBitsCallback_StubWithCallback vEventGroupSetBitsCallback_Stub
#define vEventGroupSetBitsCallback_ExpectWithArray(pvEventGroup, pvEventGroup_Depth, ulBitsToSet) vEventGroupSetBitsCallback_CMockExpectWithArray(__LINE__, pvEventGroup, pvEventGroup_Depth, ulBitsToSet)
void vEventGroupSetBitsCallback_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, int pvEventGroup_Depth, const uint32_t ulBitsToSet);
#define vEventGroupSetBitsCallback_ReturnThruPtr_pvEventGroup(pvEventGroup) vEventGroupSetBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, sizeof(void))
#define vEventGroupSetBitsCallback_ReturnArrayThruPtr_pvEventGroup(pvEventGroup, cmock_len) vEventGroupSetBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, cmock_len * sizeof(*pvEventGroup))
#define vEventGroupSetBitsCallback_ReturnMemThruPtr_pvEventGroup(pvEventGroup, cmock_size) vEventGroupSetBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, cmock_size)
void vEventGroupSetBitsCallback_CMockReturnMemThruPtr_pvEventGroup(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, size_t cmock_size);
#define vEventGroupSetBitsCallback_IgnoreArg_pvEventGroup() vEventGroupSetBitsCallback_CMockIgnoreArg_pvEventGroup(__LINE__)
void vEventGroupSetBitsCallback_CMockIgnoreArg_pvEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupSetBitsCallback_IgnoreArg_ulBitsToSet() vEventGroupSetBitsCallback_CMockIgnoreArg_ulBitsToSet(__LINE__)
void vEventGroupSetBitsCallback_CMockIgnoreArg_ulBitsToSet(UNITY_LINE_TYPE cmock_line);
#define vEventGroupClearBitsCallback_Ignore() vEventGroupClearBitsCallback_CMockIgnore()
void vEventGroupClearBitsCallback_CMockIgnore(void);
#define vEventGroupClearBitsCallback_StopIgnore() vEventGroupClearBitsCallback_CMockStopIgnore()
void vEventGroupClearBitsCallback_CMockStopIgnore(void);
#define vEventGroupClearBitsCallback_Expect(pvEventGroup, ulBitsToClear) vEventGroupClearBitsCallback_CMockExpect(__LINE__, pvEventGroup, ulBitsToClear)
void vEventGroupClearBitsCallback_CMockExpect(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, const uint32_t ulBitsToClear);
typedef void (* CMOCK_vEventGroupClearBitsCallback_CALLBACK)(void* pvEventGroup, const uint32_t ulBitsToClear, int cmock_num_calls);
void vEventGroupClearBitsCallback_AddCallback(CMOCK_vEventGroupClearBitsCallback_CALLBACK Callback);
void vEventGroupClearBitsCallback_Stub(CMOCK_vEventGroupClearBitsCallback_CALLBACK Callback);
#define vEventGroupClearBitsCallback_StubWithCallback vEventGroupClearBitsCallback_Stub
#define vEventGroupClearBitsCallback_ExpectWithArray(pvEventGroup, pvEventGroup_Depth, ulBitsToClear) vEventGroupClearBitsCallback_CMockExpectWithArray(__LINE__, pvEventGroup, pvEventGroup_Depth, ulBitsToClear)
void vEventGroupClearBitsCallback_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, int pvEventGroup_Depth, const uint32_t ulBitsToClear);
#define vEventGroupClearBitsCallback_ReturnThruPtr_pvEventGroup(pvEventGroup) vEventGroupClearBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, sizeof(void))
#define vEventGroupClearBitsCallback_ReturnArrayThruPtr_pvEventGroup(pvEventGroup, cmock_len) vEventGroupClearBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, cmock_len * sizeof(*pvEventGroup))
#define vEventGroupClearBitsCallback_ReturnMemThruPtr_pvEventGroup(pvEventGroup, cmock_size) vEventGroupClearBitsCallback_CMockReturnMemThruPtr_pvEventGroup(__LINE__, pvEventGroup, cmock_size)
void vEventGroupClearBitsCallback_CMockReturnMemThruPtr_pvEventGroup(UNITY_LINE_TYPE cmock_line, void* pvEventGroup, size_t cmock_size);
#define vEventGroupClearBitsCallback_IgnoreArg_pvEventGroup() vEventGroupClearBitsCallback_CMockIgnoreArg_pvEventGroup(__LINE__)
void vEventGroupClearBitsCallback_CMockIgnoreArg_pvEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupClearBitsCallback_IgnoreArg_ulBitsToClear() vEventGroupClearBitsCallback_CMockIgnoreArg_ulBitsToClear(__LINE__)
void vEventGroupClearBitsCallback_CMockIgnoreArg_ulBitsToClear(UNITY_LINE_TYPE cmock_line);
#define uxEventGroupGetNumber_IgnoreAndReturn(cmock_retval) uxEventGroupGetNumber_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void uxEventGroupGetNumber_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, UBaseType_t cmock_to_return);
#define uxEventGroupGetNumber_StopIgnore() uxEventGroupGetNumber_CMockStopIgnore()
void uxEventGroupGetNumber_CMockStopIgnore(void);
#define uxEventGroupGetNumber_ExpectAndReturn(xEventGroup, cmock_retval) uxEventGroupGetNumber_CMockExpectAndReturn(__LINE__, xEventGroup, cmock_retval)
void uxEventGroupGetNumber_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, void* xEventGroup, UBaseType_t cmock_to_return);
typedef UBaseType_t (* CMOCK_uxEventGroupGetNumber_CALLBACK)(void* xEventGroup, int cmock_num_calls);
void uxEventGroupGetNumber_AddCallback(CMOCK_uxEventGroupGetNumber_CALLBACK Callback);
void uxEventGroupGetNumber_Stub(CMOCK_uxEventGroupGetNumber_CALLBACK Callback);
#define uxEventGroupGetNumber_StubWithCallback uxEventGroupGetNumber_Stub
#define uxEventGroupGetNumber_ExpectWithArrayAndReturn(xEventGroup, xEventGroup_Depth, cmock_retval) uxEventGroupGetNumber_CMockExpectWithArrayAndReturn(__LINE__, xEventGroup, xEventGroup_Depth, cmock_retval)
void uxEventGroupGetNumber_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, void* xEventGroup, int xEventGroup_Depth, UBaseType_t cmock_to_return);
#define uxEventGroupGetNumber_ReturnThruPtr_xEventGroup(xEventGroup) uxEventGroupGetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, sizeof(void))
#define uxEventGroupGetNumber_ReturnArrayThruPtr_xEventGroup(xEventGroup, cmock_len) uxEventGroupGetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, cmock_len * sizeof(*xEventGroup))
#define uxEventGroupGetNumber_ReturnMemThruPtr_xEventGroup(xEventGroup, cmock_size) uxEventGroupGetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, cmock_size)
void uxEventGroupGetNumber_CMockReturnMemThruPtr_xEventGroup(UNITY_LINE_TYPE cmock_line, void* xEventGroup, size_t cmock_size);
#define uxEventGroupGetNumber_IgnoreArg_xEventGroup() uxEventGroupGetNumber_CMockIgnoreArg_xEventGroup(__LINE__)
void uxEventGroupGetNumber_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupSetNumber_Ignore() vEventGroupSetNumber_CMockIgnore()
void vEventGroupSetNumber_CMockIgnore(void);
#define vEventGroupSetNumber_StopIgnore() vEventGroupSetNumber_CMockStopIgnore()
void vEventGroupSetNumber_CMockStopIgnore(void);
#define vEventGroupSetNumber_Expect(xEventGroup, uxEventGroupNumber) vEventGroupSetNumber_CMockExpect(__LINE__, xEventGroup, uxEventGroupNumber)
void vEventGroupSetNumber_CMockExpect(UNITY_LINE_TYPE cmock_line, void* xEventGroup, UBaseType_t uxEventGroupNumber);
typedef void (* CMOCK_vEventGroupSetNumber_CALLBACK)(void* xEventGroup, UBaseType_t uxEventGroupNumber, int cmock_num_calls);
void vEventGroupSetNumber_AddCallback(CMOCK_vEventGroupSetNumber_CALLBACK Callback);
void vEventGroupSetNumber_Stub(CMOCK_vEventGroupSetNumber_CALLBACK Callback);
#define vEventGroupSetNumber_StubWithCallback vEventGroupSetNumber_Stub
#define vEventGroupSetNumber_ExpectWithArray(xEventGroup, xEventGroup_Depth, uxEventGroupNumber) vEventGroupSetNumber_CMockExpectWithArray(__LINE__, xEventGroup, xEventGroup_Depth, uxEventGroupNumber)
void vEventGroupSetNumber_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, void* xEventGroup, int xEventGroup_Depth, UBaseType_t uxEventGroupNumber);
#define vEventGroupSetNumber_ReturnThruPtr_xEventGroup(xEventGroup) vEventGroupSetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, sizeof(void))
#define vEventGroupSetNumber_ReturnArrayThruPtr_xEventGroup(xEventGroup, cmock_len) vEventGroupSetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, cmock_len * sizeof(*xEventGroup))
#define vEventGroupSetNumber_ReturnMemThruPtr_xEventGroup(xEventGroup, cmock_size) vEventGroupSetNumber_CMockReturnMemThruPtr_xEventGroup(__LINE__, xEventGroup, cmock_size)
void vEventGroupSetNumber_CMockReturnMemThruPtr_xEventGroup(UNITY_LINE_TYPE cmock_line, void* xEventGroup, size_t cmock_size);
#define vEventGroupSetNumber_IgnoreArg_xEventGroup() vEventGroupSetNumber_CMockIgnoreArg_xEventGroup(__LINE__)
void vEventGroupSetNumber_CMockIgnoreArg_xEventGroup(UNITY_LINE_TYPE cmock_line);
#define vEventGroupSetNumber_IgnoreArg_uxEventGroupNumber() vEventGroupSetNumber_CMockIgnoreArg_uxEventGroupNumber(__LINE__)
void vEventGroupSetNumber_CMockIgnoreArg_uxEventGroupNumber(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
