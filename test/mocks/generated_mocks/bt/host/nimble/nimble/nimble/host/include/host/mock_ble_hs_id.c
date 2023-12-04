/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_ble_hs_id.h"

static const char* CMockString_ble_hs_id_copy_addr = "ble_hs_id_copy_addr";
static const char* CMockString_ble_hs_id_gen_rnd = "ble_hs_id_gen_rnd";
static const char* CMockString_ble_hs_id_infer_auto = "ble_hs_id_infer_auto";
static const char* CMockString_ble_hs_id_set_rnd = "ble_hs_id_set_rnd";
static const char* CMockString_id_addr_type = "id_addr_type";
static const char* CMockString_nrpa = "nrpa";
static const char* CMockString_out_addr = "out_addr";
static const char* CMockString_out_addr_type = "out_addr_type";
static const char* CMockString_out_id_addr = "out_id_addr";
static const char* CMockString_out_is_nrpa = "out_is_nrpa";
static const char* CMockString_privacy = "privacy";
static const char* CMockString_rnd_addr = "rnd_addr";

typedef struct _CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;
  int Expected_nrpa;
  ble_addr_t* Expected_out_addr;
  int Expected_out_addr_Depth;
  char ReturnThruPtr_out_addr_Used;
  ble_addr_t* ReturnThruPtr_out_addr_Val;
  size_t ReturnThruPtr_out_addr_Size;
  char IgnoreArg_nrpa;
  char IgnoreArg_out_addr;

} CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE;

typedef struct _CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;
  const uint8_t* Expected_rnd_addr;
  int Expected_rnd_addr_Depth;
  char IgnoreArg_rnd_addr;

} CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE;

typedef struct _CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;
  uint8_t Expected_id_addr_type;
  uint8_t* Expected_out_id_addr;
  int* Expected_out_is_nrpa;
  int Expected_out_id_addr_Depth;
  int Expected_out_is_nrpa_Depth;
  char ReturnThruPtr_out_id_addr_Used;
  uint8_t* ReturnThruPtr_out_id_addr_Val;
  size_t ReturnThruPtr_out_id_addr_Size;
  char ReturnThruPtr_out_is_nrpa_Used;
  int* ReturnThruPtr_out_is_nrpa_Val;
  size_t ReturnThruPtr_out_is_nrpa_Size;
  char IgnoreArg_id_addr_type;
  char IgnoreArg_out_id_addr;
  char IgnoreArg_out_is_nrpa;

} CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE;

typedef struct _CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;
  int Expected_privacy;
  uint8_t* Expected_out_addr_type;
  int Expected_out_addr_type_Depth;
  char ReturnThruPtr_out_addr_type_Used;
  uint8_t* ReturnThruPtr_out_addr_type_Val;
  size_t ReturnThruPtr_out_addr_type_Size;
  char IgnoreArg_privacy;
  char IgnoreArg_out_addr_type;

} CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE;

static struct mock_ble_hs_idInstance
{
  char ble_hs_id_gen_rnd_IgnoreBool;
  int ble_hs_id_gen_rnd_FinalReturn;
  char ble_hs_id_gen_rnd_CallbackBool;
  CMOCK_ble_hs_id_gen_rnd_CALLBACK ble_hs_id_gen_rnd_CallbackFunctionPointer;
  int ble_hs_id_gen_rnd_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE ble_hs_id_gen_rnd_CallInstance;
  char ble_hs_id_set_rnd_IgnoreBool;
  int ble_hs_id_set_rnd_FinalReturn;
  char ble_hs_id_set_rnd_CallbackBool;
  CMOCK_ble_hs_id_set_rnd_CALLBACK ble_hs_id_set_rnd_CallbackFunctionPointer;
  int ble_hs_id_set_rnd_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE ble_hs_id_set_rnd_CallInstance;
  char ble_hs_id_copy_addr_IgnoreBool;
  int ble_hs_id_copy_addr_FinalReturn;
  char ble_hs_id_copy_addr_CallbackBool;
  CMOCK_ble_hs_id_copy_addr_CALLBACK ble_hs_id_copy_addr_CallbackFunctionPointer;
  int ble_hs_id_copy_addr_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE ble_hs_id_copy_addr_CallInstance;
  char ble_hs_id_infer_auto_IgnoreBool;
  int ble_hs_id_infer_auto_FinalReturn;
  char ble_hs_id_infer_auto_CallbackBool;
  CMOCK_ble_hs_id_infer_auto_CALLBACK ble_hs_id_infer_auto_CallbackFunctionPointer;
  int ble_hs_id_infer_auto_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE ble_hs_id_infer_auto_CallInstance;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void mock_ble_hs_id_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.ble_hs_id_gen_rnd_CallInstance;
  if (Mock.ble_hs_id_gen_rnd_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_ble_hs_id_gen_rnd);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.ble_hs_id_set_rnd_CallInstance;
  if (Mock.ble_hs_id_set_rnd_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_ble_hs_id_set_rnd);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.ble_hs_id_set_rnd_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.ble_hs_id_copy_addr_CallInstance;
  if (Mock.ble_hs_id_copy_addr_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_ble_hs_id_copy_addr);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.ble_hs_id_copy_addr_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.ble_hs_id_infer_auto_CallInstance;
  if (Mock.ble_hs_id_infer_auto_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_ble_hs_id_infer_auto);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.ble_hs_id_infer_auto_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void mock_ble_hs_id_Init(void)
{
  mock_ble_hs_id_Destroy();
}

void mock_ble_hs_id_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

int ble_hs_id_gen_rnd(int nrpa, ble_addr_t* out_addr)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_ble_hs_id_gen_rnd);
  cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ble_hs_id_gen_rnd_CallInstance);
  Mock.ble_hs_id_gen_rnd_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_gen_rnd_CallInstance);
  if (Mock.ble_hs_id_gen_rnd_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.ble_hs_id_gen_rnd_FinalReturn;
    Mock.ble_hs_id_gen_rnd_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.ble_hs_id_gen_rnd_CallbackBool &&
      Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer(nrpa, out_addr, Mock.ble_hs_id_gen_rnd_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->IgnoreArg_nrpa)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_gen_rnd,CMockString_nrpa);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_nrpa, nrpa, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_out_addr)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_gen_rnd,CMockString_out_addr);
    if (cmock_call_instance->Expected_out_addr == NULL)
      { UNITY_TEST_ASSERT_NULL(out_addr, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY((void*)(cmock_call_instance->Expected_out_addr), (void*)(out_addr), sizeof(ble_addr_t), cmock_call_instance->Expected_out_addr_Depth, cmock_line, CMockStringMismatch); }
  }
  if (Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer(nrpa, out_addr, Mock.ble_hs_id_gen_rnd_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_out_addr_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(out_addr, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)out_addr, (void*)cmock_call_instance->ReturnThruPtr_out_addr_Val,
      cmock_call_instance->ReturnThruPtr_out_addr_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ble_hs_id_gen_rnd(CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance, int nrpa, ble_addr_t* out_addr, int out_addr_Depth);
void CMockExpectParameters_ble_hs_id_gen_rnd(CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance, int nrpa, ble_addr_t* out_addr, int out_addr_Depth)
{
  cmock_call_instance->Expected_nrpa = nrpa;
  cmock_call_instance->IgnoreArg_nrpa = 0;
  cmock_call_instance->Expected_out_addr = out_addr;
  cmock_call_instance->Expected_out_addr_Depth = out_addr_Depth;
  cmock_call_instance->IgnoreArg_out_addr = 0;
  cmock_call_instance->ReturnThruPtr_out_addr_Used = 0;
}

void ble_hs_id_gen_rnd_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_gen_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_gen_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)1;
}

void ble_hs_id_gen_rnd_CMockStopIgnore(void)
{
  if(Mock.ble_hs_id_gen_rnd_IgnoreBool)
    Mock.ble_hs_id_gen_rnd_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_gen_rnd_CallInstance);
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
}

void ble_hs_id_gen_rnd_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int nrpa, ble_addr_t* out_addr, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_gen_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_gen_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_gen_rnd(cmock_call_instance, nrpa, out_addr, 1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_gen_rnd_AddCallback(CMOCK_ble_hs_id_gen_rnd_CALLBACK Callback)
{
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
  Mock.ble_hs_id_gen_rnd_CallbackBool = (char)1;
  Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer = Callback;
}

void ble_hs_id_gen_rnd_Stub(CMOCK_ble_hs_id_gen_rnd_CALLBACK Callback)
{
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
  Mock.ble_hs_id_gen_rnd_CallbackBool = (char)0;
  Mock.ble_hs_id_gen_rnd_CallbackFunctionPointer = Callback;
}

void ble_hs_id_gen_rnd_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, int nrpa, ble_addr_t* out_addr, int out_addr_Depth, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_gen_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_gen_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_gen_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_gen_rnd(cmock_call_instance, nrpa, out_addr, out_addr_Depth);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_gen_rnd_CMockReturnMemThruPtr_out_addr(UNITY_LINE_TYPE cmock_line, ble_addr_t* out_addr, size_t cmock_size)
{
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_gen_rnd_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_out_addr_Used = 1;
  cmock_call_instance->ReturnThruPtr_out_addr_Val = out_addr;
  cmock_call_instance->ReturnThruPtr_out_addr_Size = cmock_size;
}

void ble_hs_id_gen_rnd_CMockIgnoreArg_nrpa(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_gen_rnd_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_nrpa = 1;
}

void ble_hs_id_gen_rnd_CMockIgnoreArg_out_addr(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_gen_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_gen_rnd_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_out_addr = 1;
}

int ble_hs_id_set_rnd(const uint8_t* rnd_addr)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_ble_hs_id_set_rnd);
  cmock_call_instance = (CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ble_hs_id_set_rnd_CallInstance);
  Mock.ble_hs_id_set_rnd_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_set_rnd_CallInstance);
  if (Mock.ble_hs_id_set_rnd_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.ble_hs_id_set_rnd_FinalReturn;
    Mock.ble_hs_id_set_rnd_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.ble_hs_id_set_rnd_CallbackBool &&
      Mock.ble_hs_id_set_rnd_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.ble_hs_id_set_rnd_CallbackFunctionPointer(rnd_addr, Mock.ble_hs_id_set_rnd_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->IgnoreArg_rnd_addr)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_set_rnd,CMockString_rnd_addr);
    if (cmock_call_instance->Expected_rnd_addr == NULL)
      { UNITY_TEST_ASSERT_NULL(rnd_addr, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_rnd_addr, rnd_addr, cmock_call_instance->Expected_rnd_addr_Depth, cmock_line, CMockStringMismatch); }
  }
  if (Mock.ble_hs_id_set_rnd_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.ble_hs_id_set_rnd_CallbackFunctionPointer(rnd_addr, Mock.ble_hs_id_set_rnd_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ble_hs_id_set_rnd(CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance, const uint8_t* rnd_addr, int rnd_addr_Depth);
void CMockExpectParameters_ble_hs_id_set_rnd(CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance, const uint8_t* rnd_addr, int rnd_addr_Depth)
{
  cmock_call_instance->Expected_rnd_addr = rnd_addr;
  cmock_call_instance->Expected_rnd_addr_Depth = rnd_addr_Depth;
  cmock_call_instance->IgnoreArg_rnd_addr = 0;
}

void ble_hs_id_set_rnd_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_set_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_set_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)1;
}

void ble_hs_id_set_rnd_CMockStopIgnore(void)
{
  if(Mock.ble_hs_id_set_rnd_IgnoreBool)
    Mock.ble_hs_id_set_rnd_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_set_rnd_CallInstance);
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
}

void ble_hs_id_set_rnd_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const uint8_t* rnd_addr, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_set_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_set_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_set_rnd(cmock_call_instance, rnd_addr, 1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_set_rnd_AddCallback(CMOCK_ble_hs_id_set_rnd_CALLBACK Callback)
{
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
  Mock.ble_hs_id_set_rnd_CallbackBool = (char)1;
  Mock.ble_hs_id_set_rnd_CallbackFunctionPointer = Callback;
}

void ble_hs_id_set_rnd_Stub(CMOCK_ble_hs_id_set_rnd_CALLBACK Callback)
{
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
  Mock.ble_hs_id_set_rnd_CallbackBool = (char)0;
  Mock.ble_hs_id_set_rnd_CallbackFunctionPointer = Callback;
}

void ble_hs_id_set_rnd_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const uint8_t* rnd_addr, int rnd_addr_Depth, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE));
  CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_set_rnd_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_set_rnd_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_set_rnd_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_set_rnd(cmock_call_instance, rnd_addr, rnd_addr_Depth);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_set_rnd_CMockIgnoreArg_rnd_addr(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_set_rnd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_set_rnd_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_rnd_addr = 1;
}

int ble_hs_id_copy_addr(uint8_t id_addr_type, uint8_t* out_id_addr, int* out_is_nrpa)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_ble_hs_id_copy_addr);
  cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ble_hs_id_copy_addr_CallInstance);
  Mock.ble_hs_id_copy_addr_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_copy_addr_CallInstance);
  if (Mock.ble_hs_id_copy_addr_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.ble_hs_id_copy_addr_FinalReturn;
    Mock.ble_hs_id_copy_addr_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.ble_hs_id_copy_addr_CallbackBool &&
      Mock.ble_hs_id_copy_addr_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.ble_hs_id_copy_addr_CallbackFunctionPointer(id_addr_type, out_id_addr, out_is_nrpa, Mock.ble_hs_id_copy_addr_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->IgnoreArg_id_addr_type)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_copy_addr,CMockString_id_addr_type);
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_id_addr_type, id_addr_type, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_out_id_addr)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_copy_addr,CMockString_out_id_addr);
    if (cmock_call_instance->Expected_out_id_addr == NULL)
      { UNITY_TEST_ASSERT_NULL(out_id_addr, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_out_id_addr, out_id_addr, cmock_call_instance->Expected_out_id_addr_Depth, cmock_line, CMockStringMismatch); }
  }
  if (!cmock_call_instance->IgnoreArg_out_is_nrpa)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_copy_addr,CMockString_out_is_nrpa);
    if (cmock_call_instance->Expected_out_is_nrpa == NULL)
      { UNITY_TEST_ASSERT_NULL(out_is_nrpa, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_INT_ARRAY(cmock_call_instance->Expected_out_is_nrpa, out_is_nrpa, cmock_call_instance->Expected_out_is_nrpa_Depth, cmock_line, CMockStringMismatch); }
  }
  if (Mock.ble_hs_id_copy_addr_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.ble_hs_id_copy_addr_CallbackFunctionPointer(id_addr_type, out_id_addr, out_is_nrpa, Mock.ble_hs_id_copy_addr_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_out_id_addr_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(out_id_addr, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)out_id_addr, (void*)cmock_call_instance->ReturnThruPtr_out_id_addr_Val,
      cmock_call_instance->ReturnThruPtr_out_id_addr_Size);
  }
  if (cmock_call_instance->ReturnThruPtr_out_is_nrpa_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(out_is_nrpa, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)out_is_nrpa, (void*)cmock_call_instance->ReturnThruPtr_out_is_nrpa_Val,
      cmock_call_instance->ReturnThruPtr_out_is_nrpa_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ble_hs_id_copy_addr(CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance, uint8_t id_addr_type, uint8_t* out_id_addr, int out_id_addr_Depth, int* out_is_nrpa, int out_is_nrpa_Depth);
void CMockExpectParameters_ble_hs_id_copy_addr(CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance, uint8_t id_addr_type, uint8_t* out_id_addr, int out_id_addr_Depth, int* out_is_nrpa, int out_is_nrpa_Depth)
{
  cmock_call_instance->Expected_id_addr_type = id_addr_type;
  cmock_call_instance->IgnoreArg_id_addr_type = 0;
  cmock_call_instance->Expected_out_id_addr = out_id_addr;
  cmock_call_instance->Expected_out_id_addr_Depth = out_id_addr_Depth;
  cmock_call_instance->IgnoreArg_out_id_addr = 0;
  cmock_call_instance->ReturnThruPtr_out_id_addr_Used = 0;
  cmock_call_instance->Expected_out_is_nrpa = out_is_nrpa;
  cmock_call_instance->Expected_out_is_nrpa_Depth = out_is_nrpa_Depth;
  cmock_call_instance->IgnoreArg_out_is_nrpa = 0;
  cmock_call_instance->ReturnThruPtr_out_is_nrpa_Used = 0;
}

void ble_hs_id_copy_addr_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE));
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_copy_addr_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_copy_addr_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)1;
}

void ble_hs_id_copy_addr_CMockStopIgnore(void)
{
  if(Mock.ble_hs_id_copy_addr_IgnoreBool)
    Mock.ble_hs_id_copy_addr_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_copy_addr_CallInstance);
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
}

void ble_hs_id_copy_addr_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t id_addr_type, uint8_t* out_id_addr, int* out_is_nrpa, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE));
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_copy_addr_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_copy_addr_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_copy_addr(cmock_call_instance, id_addr_type, out_id_addr, 1, out_is_nrpa, 1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_copy_addr_AddCallback(CMOCK_ble_hs_id_copy_addr_CALLBACK Callback)
{
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
  Mock.ble_hs_id_copy_addr_CallbackBool = (char)1;
  Mock.ble_hs_id_copy_addr_CallbackFunctionPointer = Callback;
}

void ble_hs_id_copy_addr_Stub(CMOCK_ble_hs_id_copy_addr_CALLBACK Callback)
{
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
  Mock.ble_hs_id_copy_addr_CallbackBool = (char)0;
  Mock.ble_hs_id_copy_addr_CallbackFunctionPointer = Callback;
}

void ble_hs_id_copy_addr_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t id_addr_type, uint8_t* out_id_addr, int out_id_addr_Depth, int* out_is_nrpa, int out_is_nrpa_Depth, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE));
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_copy_addr_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_copy_addr_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_copy_addr_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_copy_addr(cmock_call_instance, id_addr_type, out_id_addr, out_id_addr_Depth, out_is_nrpa, out_is_nrpa_Depth);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_copy_addr_CMockReturnMemThruPtr_out_id_addr(UNITY_LINE_TYPE cmock_line, uint8_t* out_id_addr, size_t cmock_size)
{
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_copy_addr_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_out_id_addr_Used = 1;
  cmock_call_instance->ReturnThruPtr_out_id_addr_Val = out_id_addr;
  cmock_call_instance->ReturnThruPtr_out_id_addr_Size = cmock_size;
}

void ble_hs_id_copy_addr_CMockReturnMemThruPtr_out_is_nrpa(UNITY_LINE_TYPE cmock_line, int* out_is_nrpa, size_t cmock_size)
{
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_copy_addr_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_out_is_nrpa_Used = 1;
  cmock_call_instance->ReturnThruPtr_out_is_nrpa_Val = out_is_nrpa;
  cmock_call_instance->ReturnThruPtr_out_is_nrpa_Size = cmock_size;
}

void ble_hs_id_copy_addr_CMockIgnoreArg_id_addr_type(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_copy_addr_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_id_addr_type = 1;
}

void ble_hs_id_copy_addr_CMockIgnoreArg_out_id_addr(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_copy_addr_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_out_id_addr = 1;
}

void ble_hs_id_copy_addr_CMockIgnoreArg_out_is_nrpa(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_copy_addr_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_copy_addr_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_out_is_nrpa = 1;
}

int ble_hs_id_infer_auto(int privacy, uint8_t* out_addr_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_ble_hs_id_infer_auto);
  cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ble_hs_id_infer_auto_CallInstance);
  Mock.ble_hs_id_infer_auto_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_infer_auto_CallInstance);
  if (Mock.ble_hs_id_infer_auto_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.ble_hs_id_infer_auto_FinalReturn;
    Mock.ble_hs_id_infer_auto_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.ble_hs_id_infer_auto_CallbackBool &&
      Mock.ble_hs_id_infer_auto_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.ble_hs_id_infer_auto_CallbackFunctionPointer(privacy, out_addr_type, Mock.ble_hs_id_infer_auto_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->IgnoreArg_privacy)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_infer_auto,CMockString_privacy);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_privacy, privacy, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_out_addr_type)
  {
    UNITY_SET_DETAILS(CMockString_ble_hs_id_infer_auto,CMockString_out_addr_type);
    if (cmock_call_instance->Expected_out_addr_type == NULL)
      { UNITY_TEST_ASSERT_NULL(out_addr_type, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_out_addr_type, out_addr_type, cmock_call_instance->Expected_out_addr_type_Depth, cmock_line, CMockStringMismatch); }
  }
  if (Mock.ble_hs_id_infer_auto_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.ble_hs_id_infer_auto_CallbackFunctionPointer(privacy, out_addr_type, Mock.ble_hs_id_infer_auto_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_out_addr_type_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(out_addr_type, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)out_addr_type, (void*)cmock_call_instance->ReturnThruPtr_out_addr_type_Val,
      cmock_call_instance->ReturnThruPtr_out_addr_type_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ble_hs_id_infer_auto(CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance, int privacy, uint8_t* out_addr_type, int out_addr_type_Depth);
void CMockExpectParameters_ble_hs_id_infer_auto(CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance, int privacy, uint8_t* out_addr_type, int out_addr_type_Depth)
{
  cmock_call_instance->Expected_privacy = privacy;
  cmock_call_instance->IgnoreArg_privacy = 0;
  cmock_call_instance->Expected_out_addr_type = out_addr_type;
  cmock_call_instance->Expected_out_addr_type_Depth = out_addr_type_Depth;
  cmock_call_instance->IgnoreArg_out_addr_type = 0;
  cmock_call_instance->ReturnThruPtr_out_addr_type_Used = 0;
}

void ble_hs_id_infer_auto_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE));
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_infer_auto_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_infer_auto_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)1;
}

void ble_hs_id_infer_auto_CMockStopIgnore(void)
{
  if(Mock.ble_hs_id_infer_auto_IgnoreBool)
    Mock.ble_hs_id_infer_auto_CallInstance = CMock_Guts_MemNext(Mock.ble_hs_id_infer_auto_CallInstance);
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
}

void ble_hs_id_infer_auto_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int privacy, uint8_t* out_addr_type, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE));
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_infer_auto_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_infer_auto_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_infer_auto(cmock_call_instance, privacy, out_addr_type, 1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_infer_auto_AddCallback(CMOCK_ble_hs_id_infer_auto_CALLBACK Callback)
{
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
  Mock.ble_hs_id_infer_auto_CallbackBool = (char)1;
  Mock.ble_hs_id_infer_auto_CallbackFunctionPointer = Callback;
}

void ble_hs_id_infer_auto_Stub(CMOCK_ble_hs_id_infer_auto_CALLBACK Callback)
{
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
  Mock.ble_hs_id_infer_auto_CallbackBool = (char)0;
  Mock.ble_hs_id_infer_auto_CallbackFunctionPointer = Callback;
}

void ble_hs_id_infer_auto_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, int privacy, uint8_t* out_addr_type, int out_addr_type_Depth, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE));
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.ble_hs_id_infer_auto_CallInstance = CMock_Guts_MemChain(Mock.ble_hs_id_infer_auto_CallInstance, cmock_guts_index);
  Mock.ble_hs_id_infer_auto_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  CMockExpectParameters_ble_hs_id_infer_auto(cmock_call_instance, privacy, out_addr_type, out_addr_type_Depth);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ble_hs_id_infer_auto_CMockReturnMemThruPtr_out_addr_type(UNITY_LINE_TYPE cmock_line, uint8_t* out_addr_type, size_t cmock_size)
{
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_infer_auto_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_out_addr_type_Used = 1;
  cmock_call_instance->ReturnThruPtr_out_addr_type_Val = out_addr_type;
  cmock_call_instance->ReturnThruPtr_out_addr_type_Size = cmock_size;
}

void ble_hs_id_infer_auto_CMockIgnoreArg_privacy(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_infer_auto_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_privacy = 1;
}

void ble_hs_id_infer_auto_CMockIgnoreArg_out_addr_type(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE* cmock_call_instance = (CMOCK_ble_hs_id_infer_auto_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.ble_hs_id_infer_auto_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_out_addr_type = 1;
}

