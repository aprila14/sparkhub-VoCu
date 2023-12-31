#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_ESP_ERR_H
#define _MOCK_ESP_ERR_H

#include "unity.h"
#include "esp_err.h"

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

void mock_esp_err_Init(void);
void mock_esp_err_Destroy(void);
void mock_esp_err_Verify(void);




#define esp_err_to_name_IgnoreAndReturn(cmock_retval) esp_err_to_name_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_err_to_name_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, const char* cmock_to_return);
#define esp_err_to_name_StopIgnore() esp_err_to_name_CMockStopIgnore()
void esp_err_to_name_CMockStopIgnore(void);
#define esp_err_to_name_ExpectAndReturn(code, cmock_retval) esp_err_to_name_CMockExpectAndReturn(__LINE__, code, cmock_retval)
void esp_err_to_name_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t code, const char* cmock_to_return);
typedef const char* (* CMOCK_esp_err_to_name_CALLBACK)(esp_err_t code, int cmock_num_calls);
void esp_err_to_name_AddCallback(CMOCK_esp_err_to_name_CALLBACK Callback);
void esp_err_to_name_Stub(CMOCK_esp_err_to_name_CALLBACK Callback);
#define esp_err_to_name_StubWithCallback esp_err_to_name_Stub
#define esp_err_to_name_IgnoreArg_code() esp_err_to_name_CMockIgnoreArg_code(__LINE__)
void esp_err_to_name_CMockIgnoreArg_code(UNITY_LINE_TYPE cmock_line);
#define esp_err_to_name_r_IgnoreAndReturn(cmock_retval) esp_err_to_name_r_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_err_to_name_r_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, const char* cmock_to_return);
#define esp_err_to_name_r_StopIgnore() esp_err_to_name_r_CMockStopIgnore()
void esp_err_to_name_r_CMockStopIgnore(void);
#define esp_err_to_name_r_ExpectAndReturn(code, buf, buflen, cmock_retval) esp_err_to_name_r_CMockExpectAndReturn(__LINE__, code, buf, buflen, cmock_retval)
void esp_err_to_name_r_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t code, char* buf, size_t buflen, const char* cmock_to_return);
typedef const char* (* CMOCK_esp_err_to_name_r_CALLBACK)(esp_err_t code, char* buf, size_t buflen, int cmock_num_calls);
void esp_err_to_name_r_AddCallback(CMOCK_esp_err_to_name_r_CALLBACK Callback);
void esp_err_to_name_r_Stub(CMOCK_esp_err_to_name_r_CALLBACK Callback);
#define esp_err_to_name_r_StubWithCallback esp_err_to_name_r_Stub
#define esp_err_to_name_r_ReturnThruPtr_buf(buf) esp_err_to_name_r_CMockReturnMemThruPtr_buf(__LINE__, buf, sizeof(char))
#define esp_err_to_name_r_ReturnArrayThruPtr_buf(buf, cmock_len) esp_err_to_name_r_CMockReturnMemThruPtr_buf(__LINE__, buf, cmock_len * sizeof(*buf))
#define esp_err_to_name_r_ReturnMemThruPtr_buf(buf, cmock_size) esp_err_to_name_r_CMockReturnMemThruPtr_buf(__LINE__, buf, cmock_size)
void esp_err_to_name_r_CMockReturnMemThruPtr_buf(UNITY_LINE_TYPE cmock_line, char* buf, size_t cmock_size);
#define esp_err_to_name_r_IgnoreArg_code() esp_err_to_name_r_CMockIgnoreArg_code(__LINE__)
void esp_err_to_name_r_CMockIgnoreArg_code(UNITY_LINE_TYPE cmock_line);
#define esp_err_to_name_r_IgnoreArg_buf() esp_err_to_name_r_CMockIgnoreArg_buf(__LINE__)
void esp_err_to_name_r_CMockIgnoreArg_buf(UNITY_LINE_TYPE cmock_line);
#define esp_err_to_name_r_IgnoreArg_buflen() esp_err_to_name_r_CMockIgnoreArg_buflen(__LINE__)
void esp_err_to_name_r_CMockIgnoreArg_buflen(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_Ignore() _esp_error_check_failed_CMockIgnore()
void _esp_error_check_failed_CMockIgnore(void);
#define _esp_error_check_failed_StopIgnore() _esp_error_check_failed_CMockStopIgnore()
void _esp_error_check_failed_CMockStopIgnore(void);
#define _esp_error_check_failed_Expect(rc, file, line, function, expression) _esp_error_check_failed_CMockExpect(__LINE__, rc, file, line, function, expression)
void _esp_error_check_failed_CMockExpect(UNITY_LINE_TYPE cmock_line, esp_err_t rc, const char* file, int line, const char* function, const char* expression);
typedef void (* CMOCK__esp_error_check_failed_CALLBACK)(esp_err_t rc, const char* file, int line, const char* function, const char* expression, int cmock_num_calls);
void _esp_error_check_failed_AddCallback(CMOCK__esp_error_check_failed_CALLBACK Callback);
void _esp_error_check_failed_Stub(CMOCK__esp_error_check_failed_CALLBACK Callback);
#define _esp_error_check_failed_StubWithCallback _esp_error_check_failed_Stub
#define _esp_error_check_failed_IgnoreArg_rc() _esp_error_check_failed_CMockIgnoreArg_rc(__LINE__)
void _esp_error_check_failed_CMockIgnoreArg_rc(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_IgnoreArg_file() _esp_error_check_failed_CMockIgnoreArg_file(__LINE__)
void _esp_error_check_failed_CMockIgnoreArg_file(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_IgnoreArg_line() _esp_error_check_failed_CMockIgnoreArg_line(__LINE__)
void _esp_error_check_failed_CMockIgnoreArg_line(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_IgnoreArg_function() _esp_error_check_failed_CMockIgnoreArg_function(__LINE__)
void _esp_error_check_failed_CMockIgnoreArg_function(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_IgnoreArg_expression() _esp_error_check_failed_CMockIgnoreArg_expression(__LINE__)
void _esp_error_check_failed_CMockIgnoreArg_expression(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_without_abort_Ignore() _esp_error_check_failed_without_abort_CMockIgnore()
void _esp_error_check_failed_without_abort_CMockIgnore(void);
#define _esp_error_check_failed_without_abort_StopIgnore() _esp_error_check_failed_without_abort_CMockStopIgnore()
void _esp_error_check_failed_without_abort_CMockStopIgnore(void);
#define _esp_error_check_failed_without_abort_Expect(rc, file, line, function, expression) _esp_error_check_failed_without_abort_CMockExpect(__LINE__, rc, file, line, function, expression)
void _esp_error_check_failed_without_abort_CMockExpect(UNITY_LINE_TYPE cmock_line, esp_err_t rc, const char* file, int line, const char* function, const char* expression);
typedef void (* CMOCK__esp_error_check_failed_without_abort_CALLBACK)(esp_err_t rc, const char* file, int line, const char* function, const char* expression, int cmock_num_calls);
void _esp_error_check_failed_without_abort_AddCallback(CMOCK__esp_error_check_failed_without_abort_CALLBACK Callback);
void _esp_error_check_failed_without_abort_Stub(CMOCK__esp_error_check_failed_without_abort_CALLBACK Callback);
#define _esp_error_check_failed_without_abort_StubWithCallback _esp_error_check_failed_without_abort_Stub
#define _esp_error_check_failed_without_abort_IgnoreArg_rc() _esp_error_check_failed_without_abort_CMockIgnoreArg_rc(__LINE__)
void _esp_error_check_failed_without_abort_CMockIgnoreArg_rc(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_without_abort_IgnoreArg_file() _esp_error_check_failed_without_abort_CMockIgnoreArg_file(__LINE__)
void _esp_error_check_failed_without_abort_CMockIgnoreArg_file(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_without_abort_IgnoreArg_line() _esp_error_check_failed_without_abort_CMockIgnoreArg_line(__LINE__)
void _esp_error_check_failed_without_abort_CMockIgnoreArg_line(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_without_abort_IgnoreArg_function() _esp_error_check_failed_without_abort_CMockIgnoreArg_function(__LINE__)
void _esp_error_check_failed_without_abort_CMockIgnoreArg_function(UNITY_LINE_TYPE cmock_line);
#define _esp_error_check_failed_without_abort_IgnoreArg_expression() _esp_error_check_failed_without_abort_CMockIgnoreArg_expression(__LINE__)
void _esp_error_check_failed_without_abort_CMockIgnoreArg_expression(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
