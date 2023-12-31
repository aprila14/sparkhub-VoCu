#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_AWS_ESP_OTA_OPS_H
#define _MOCK_AWS_ESP_OTA_OPS_H

#include "unity.h"
#include "aws_esp_ota_ops.h"

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

void mock_aws_esp_ota_ops_Init(void);
void mock_aws_esp_ota_ops_Destroy(void);
void mock_aws_esp_ota_ops_Verify(void);




#define aws_esp_ota_set_boot_flags_IgnoreAndReturn(cmock_retval) aws_esp_ota_set_boot_flags_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void aws_esp_ota_set_boot_flags_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define aws_esp_ota_set_boot_flags_StopIgnore() aws_esp_ota_set_boot_flags_CMockStopIgnore()
void aws_esp_ota_set_boot_flags_CMockStopIgnore(void);
#define aws_esp_ota_set_boot_flags_ExpectAndReturn(flags, active_part, cmock_retval) aws_esp_ota_set_boot_flags_CMockExpectAndReturn(__LINE__, flags, active_part, cmock_retval)
void aws_esp_ota_set_boot_flags_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t flags, bool active_part, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_aws_esp_ota_set_boot_flags_CALLBACK)(uint32_t flags, bool active_part, int cmock_num_calls);
void aws_esp_ota_set_boot_flags_AddCallback(CMOCK_aws_esp_ota_set_boot_flags_CALLBACK Callback);
void aws_esp_ota_set_boot_flags_Stub(CMOCK_aws_esp_ota_set_boot_flags_CALLBACK Callback);
#define aws_esp_ota_set_boot_flags_StubWithCallback aws_esp_ota_set_boot_flags_Stub
#define aws_esp_ota_set_boot_flags_IgnoreArg_flags() aws_esp_ota_set_boot_flags_CMockIgnoreArg_flags(__LINE__)
void aws_esp_ota_set_boot_flags_CMockIgnoreArg_flags(UNITY_LINE_TYPE cmock_line);
#define aws_esp_ota_set_boot_flags_IgnoreArg_active_part() aws_esp_ota_set_boot_flags_CMockIgnoreArg_active_part(__LINE__)
void aws_esp_ota_set_boot_flags_CMockIgnoreArg_active_part(UNITY_LINE_TYPE cmock_line);
#define aws_esp_ota_get_boot_flags_IgnoreAndReturn(cmock_retval) aws_esp_ota_get_boot_flags_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void aws_esp_ota_get_boot_flags_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define aws_esp_ota_get_boot_flags_StopIgnore() aws_esp_ota_get_boot_flags_CMockStopIgnore()
void aws_esp_ota_get_boot_flags_CMockStopIgnore(void);
#define aws_esp_ota_get_boot_flags_ExpectAndReturn(flags, active_part, cmock_retval) aws_esp_ota_get_boot_flags_CMockExpectAndReturn(__LINE__, flags, active_part, cmock_retval)
void aws_esp_ota_get_boot_flags_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t* flags, bool active_part, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_aws_esp_ota_get_boot_flags_CALLBACK)(uint32_t* flags, bool active_part, int cmock_num_calls);
void aws_esp_ota_get_boot_flags_AddCallback(CMOCK_aws_esp_ota_get_boot_flags_CALLBACK Callback);
void aws_esp_ota_get_boot_flags_Stub(CMOCK_aws_esp_ota_get_boot_flags_CALLBACK Callback);
#define aws_esp_ota_get_boot_flags_StubWithCallback aws_esp_ota_get_boot_flags_Stub
#define aws_esp_ota_get_boot_flags_ExpectWithArrayAndReturn(flags, flags_Depth, active_part, cmock_retval) aws_esp_ota_get_boot_flags_CMockExpectWithArrayAndReturn(__LINE__, flags, flags_Depth, active_part, cmock_retval)
void aws_esp_ota_get_boot_flags_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t* flags, int flags_Depth, bool active_part, esp_err_t cmock_to_return);
#define aws_esp_ota_get_boot_flags_ReturnThruPtr_flags(flags) aws_esp_ota_get_boot_flags_CMockReturnMemThruPtr_flags(__LINE__, flags, sizeof(uint32_t))
#define aws_esp_ota_get_boot_flags_ReturnArrayThruPtr_flags(flags, cmock_len) aws_esp_ota_get_boot_flags_CMockReturnMemThruPtr_flags(__LINE__, flags, cmock_len * sizeof(*flags))
#define aws_esp_ota_get_boot_flags_ReturnMemThruPtr_flags(flags, cmock_size) aws_esp_ota_get_boot_flags_CMockReturnMemThruPtr_flags(__LINE__, flags, cmock_size)
void aws_esp_ota_get_boot_flags_CMockReturnMemThruPtr_flags(UNITY_LINE_TYPE cmock_line, uint32_t* flags, size_t cmock_size);
#define aws_esp_ota_get_boot_flags_IgnoreArg_flags() aws_esp_ota_get_boot_flags_CMockIgnoreArg_flags(__LINE__)
void aws_esp_ota_get_boot_flags_CMockIgnoreArg_flags(UNITY_LINE_TYPE cmock_line);
#define aws_esp_ota_get_boot_flags_IgnoreArg_active_part() aws_esp_ota_get_boot_flags_CMockIgnoreArg_active_part(__LINE__)
void aws_esp_ota_get_boot_flags_CMockIgnoreArg_active_part(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
