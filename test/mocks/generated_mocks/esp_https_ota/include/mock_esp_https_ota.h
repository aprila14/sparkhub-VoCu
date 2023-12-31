#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_ESP_HTTPS_OTA_H
#define _MOCK_ESP_HTTPS_OTA_H

#include "unity.h"
#include "esp_https_ota.h"

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

void mock_esp_https_ota_Init(void);
void mock_esp_https_ota_Destroy(void);
void mock_esp_https_ota_Verify(void);




#define esp_https_ota_IgnoreAndReturn(cmock_retval) esp_https_ota_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_StopIgnore() esp_https_ota_CMockStopIgnore()
void esp_https_ota_CMockStopIgnore(void);
#define esp_https_ota_ExpectAndReturn(config, cmock_retval) esp_https_ota_CMockExpectAndReturn(__LINE__, config, cmock_retval)
void esp_https_ota_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const esp_http_client_config_t* config, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_CALLBACK)(const esp_http_client_config_t* config, int cmock_num_calls);
void esp_https_ota_AddCallback(CMOCK_esp_https_ota_CALLBACK Callback);
void esp_https_ota_Stub(CMOCK_esp_https_ota_CALLBACK Callback);
#define esp_https_ota_StubWithCallback esp_https_ota_Stub
#define esp_https_ota_ExpectWithArrayAndReturn(config, config_Depth, cmock_retval) esp_https_ota_CMockExpectWithArrayAndReturn(__LINE__, config, config_Depth, cmock_retval)
void esp_https_ota_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const esp_http_client_config_t* config, int config_Depth, esp_err_t cmock_to_return);
#define esp_https_ota_IgnoreArg_config() esp_https_ota_CMockIgnoreArg_config(__LINE__)
void esp_https_ota_CMockIgnoreArg_config(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_begin_IgnoreAndReturn(cmock_retval) esp_https_ota_begin_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_begin_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_begin_StopIgnore() esp_https_ota_begin_CMockStopIgnore()
void esp_https_ota_begin_CMockStopIgnore(void);
#define esp_https_ota_begin_ExpectAndReturn(ota_config, handle, cmock_retval) esp_https_ota_begin_CMockExpectAndReturn(__LINE__, ota_config, handle, cmock_retval)
void esp_https_ota_begin_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_config_t* ota_config, esp_https_ota_handle_t* handle, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_begin_CALLBACK)(esp_https_ota_config_t* ota_config, esp_https_ota_handle_t* handle, int cmock_num_calls);
void esp_https_ota_begin_AddCallback(CMOCK_esp_https_ota_begin_CALLBACK Callback);
void esp_https_ota_begin_Stub(CMOCK_esp_https_ota_begin_CALLBACK Callback);
#define esp_https_ota_begin_StubWithCallback esp_https_ota_begin_Stub
#define esp_https_ota_begin_ExpectWithArrayAndReturn(ota_config, ota_config_Depth, handle, handle_Depth, cmock_retval) esp_https_ota_begin_CMockExpectWithArrayAndReturn(__LINE__, ota_config, ota_config_Depth, handle, handle_Depth, cmock_retval)
void esp_https_ota_begin_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_config_t* ota_config, int ota_config_Depth, esp_https_ota_handle_t* handle, int handle_Depth, esp_err_t cmock_to_return);
#define esp_https_ota_begin_ReturnThruPtr_ota_config(ota_config) esp_https_ota_begin_CMockReturnMemThruPtr_ota_config(__LINE__, ota_config, sizeof(esp_https_ota_config_t))
#define esp_https_ota_begin_ReturnArrayThruPtr_ota_config(ota_config, cmock_len) esp_https_ota_begin_CMockReturnMemThruPtr_ota_config(__LINE__, ota_config, cmock_len * sizeof(*ota_config))
#define esp_https_ota_begin_ReturnMemThruPtr_ota_config(ota_config, cmock_size) esp_https_ota_begin_CMockReturnMemThruPtr_ota_config(__LINE__, ota_config, cmock_size)
void esp_https_ota_begin_CMockReturnMemThruPtr_ota_config(UNITY_LINE_TYPE cmock_line, esp_https_ota_config_t* ota_config, size_t cmock_size);
#define esp_https_ota_begin_ReturnThruPtr_handle(handle) esp_https_ota_begin_CMockReturnMemThruPtr_handle(__LINE__, handle, sizeof(esp_https_ota_handle_t))
#define esp_https_ota_begin_ReturnArrayThruPtr_handle(handle, cmock_len) esp_https_ota_begin_CMockReturnMemThruPtr_handle(__LINE__, handle, cmock_len * sizeof(*handle))
#define esp_https_ota_begin_ReturnMemThruPtr_handle(handle, cmock_size) esp_https_ota_begin_CMockReturnMemThruPtr_handle(__LINE__, handle, cmock_size)
void esp_https_ota_begin_CMockReturnMemThruPtr_handle(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t* handle, size_t cmock_size);
#define esp_https_ota_begin_IgnoreArg_ota_config() esp_https_ota_begin_CMockIgnoreArg_ota_config(__LINE__)
void esp_https_ota_begin_CMockIgnoreArg_ota_config(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_begin_IgnoreArg_handle() esp_https_ota_begin_CMockIgnoreArg_handle(__LINE__)
void esp_https_ota_begin_CMockIgnoreArg_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_perform_IgnoreAndReturn(cmock_retval) esp_https_ota_perform_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_perform_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_perform_StopIgnore() esp_https_ota_perform_CMockStopIgnore()
void esp_https_ota_perform_CMockStopIgnore(void);
#define esp_https_ota_perform_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_perform_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_perform_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_perform_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_perform_AddCallback(CMOCK_esp_https_ota_perform_CALLBACK Callback);
void esp_https_ota_perform_Stub(CMOCK_esp_https_ota_perform_CALLBACK Callback);
#define esp_https_ota_perform_StubWithCallback esp_https_ota_perform_Stub
#define esp_https_ota_perform_IgnoreArg_https_ota_handle() esp_https_ota_perform_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_perform_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_is_complete_data_received_IgnoreAndReturn(cmock_retval) esp_https_ota_is_complete_data_received_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_is_complete_data_received_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define esp_https_ota_is_complete_data_received_StopIgnore() esp_https_ota_is_complete_data_received_CMockStopIgnore()
void esp_https_ota_is_complete_data_received_CMockStopIgnore(void);
#define esp_https_ota_is_complete_data_received_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_is_complete_data_received_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_is_complete_data_received_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, bool cmock_to_return);
typedef bool (* CMOCK_esp_https_ota_is_complete_data_received_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_is_complete_data_received_AddCallback(CMOCK_esp_https_ota_is_complete_data_received_CALLBACK Callback);
void esp_https_ota_is_complete_data_received_Stub(CMOCK_esp_https_ota_is_complete_data_received_CALLBACK Callback);
#define esp_https_ota_is_complete_data_received_StubWithCallback esp_https_ota_is_complete_data_received_Stub
#define esp_https_ota_is_complete_data_received_IgnoreArg_https_ota_handle() esp_https_ota_is_complete_data_received_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_is_complete_data_received_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_finish_IgnoreAndReturn(cmock_retval) esp_https_ota_finish_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_finish_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_finish_StopIgnore() esp_https_ota_finish_CMockStopIgnore()
void esp_https_ota_finish_CMockStopIgnore(void);
#define esp_https_ota_finish_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_finish_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_finish_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_finish_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_finish_AddCallback(CMOCK_esp_https_ota_finish_CALLBACK Callback);
void esp_https_ota_finish_Stub(CMOCK_esp_https_ota_finish_CALLBACK Callback);
#define esp_https_ota_finish_StubWithCallback esp_https_ota_finish_Stub
#define esp_https_ota_finish_IgnoreArg_https_ota_handle() esp_https_ota_finish_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_finish_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_abort_IgnoreAndReturn(cmock_retval) esp_https_ota_abort_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_abort_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_abort_StopIgnore() esp_https_ota_abort_CMockStopIgnore()
void esp_https_ota_abort_CMockStopIgnore(void);
#define esp_https_ota_abort_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_abort_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_abort_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_abort_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_abort_AddCallback(CMOCK_esp_https_ota_abort_CALLBACK Callback);
void esp_https_ota_abort_Stub(CMOCK_esp_https_ota_abort_CALLBACK Callback);
#define esp_https_ota_abort_StubWithCallback esp_https_ota_abort_Stub
#define esp_https_ota_abort_IgnoreArg_https_ota_handle() esp_https_ota_abort_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_abort_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_get_img_desc_IgnoreAndReturn(cmock_retval) esp_https_ota_get_img_desc_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_get_img_desc_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define esp_https_ota_get_img_desc_StopIgnore() esp_https_ota_get_img_desc_CMockStopIgnore()
void esp_https_ota_get_img_desc_CMockStopIgnore(void);
#define esp_https_ota_get_img_desc_ExpectAndReturn(https_ota_handle, new_app_info, cmock_retval) esp_https_ota_get_img_desc_CMockExpectAndReturn(__LINE__, https_ota_handle, new_app_info, cmock_retval)
void esp_https_ota_get_img_desc_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, esp_app_desc_t* new_app_info, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_esp_https_ota_get_img_desc_CALLBACK)(esp_https_ota_handle_t https_ota_handle, esp_app_desc_t* new_app_info, int cmock_num_calls);
void esp_https_ota_get_img_desc_AddCallback(CMOCK_esp_https_ota_get_img_desc_CALLBACK Callback);
void esp_https_ota_get_img_desc_Stub(CMOCK_esp_https_ota_get_img_desc_CALLBACK Callback);
#define esp_https_ota_get_img_desc_StubWithCallback esp_https_ota_get_img_desc_Stub
#define esp_https_ota_get_img_desc_ExpectWithArrayAndReturn(https_ota_handle, new_app_info, new_app_info_Depth, cmock_retval) esp_https_ota_get_img_desc_CMockExpectWithArrayAndReturn(__LINE__, https_ota_handle, new_app_info, new_app_info_Depth, cmock_retval)
void esp_https_ota_get_img_desc_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, esp_app_desc_t* new_app_info, int new_app_info_Depth, esp_err_t cmock_to_return);
#define esp_https_ota_get_img_desc_ReturnThruPtr_new_app_info(new_app_info) esp_https_ota_get_img_desc_CMockReturnMemThruPtr_new_app_info(__LINE__, new_app_info, sizeof(esp_app_desc_t))
#define esp_https_ota_get_img_desc_ReturnArrayThruPtr_new_app_info(new_app_info, cmock_len) esp_https_ota_get_img_desc_CMockReturnMemThruPtr_new_app_info(__LINE__, new_app_info, cmock_len * sizeof(*new_app_info))
#define esp_https_ota_get_img_desc_ReturnMemThruPtr_new_app_info(new_app_info, cmock_size) esp_https_ota_get_img_desc_CMockReturnMemThruPtr_new_app_info(__LINE__, new_app_info, cmock_size)
void esp_https_ota_get_img_desc_CMockReturnMemThruPtr_new_app_info(UNITY_LINE_TYPE cmock_line, esp_app_desc_t* new_app_info, size_t cmock_size);
#define esp_https_ota_get_img_desc_IgnoreArg_https_ota_handle() esp_https_ota_get_img_desc_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_get_img_desc_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_get_img_desc_IgnoreArg_new_app_info() esp_https_ota_get_img_desc_CMockIgnoreArg_new_app_info(__LINE__)
void esp_https_ota_get_img_desc_CMockIgnoreArg_new_app_info(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_get_image_len_read_IgnoreAndReturn(cmock_retval) esp_https_ota_get_image_len_read_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_get_image_len_read_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define esp_https_ota_get_image_len_read_StopIgnore() esp_https_ota_get_image_len_read_CMockStopIgnore()
void esp_https_ota_get_image_len_read_CMockStopIgnore(void);
#define esp_https_ota_get_image_len_read_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_get_image_len_read_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_get_image_len_read_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, int cmock_to_return);
typedef int (* CMOCK_esp_https_ota_get_image_len_read_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_get_image_len_read_AddCallback(CMOCK_esp_https_ota_get_image_len_read_CALLBACK Callback);
void esp_https_ota_get_image_len_read_Stub(CMOCK_esp_https_ota_get_image_len_read_CALLBACK Callback);
#define esp_https_ota_get_image_len_read_StubWithCallback esp_https_ota_get_image_len_read_Stub
#define esp_https_ota_get_image_len_read_IgnoreArg_https_ota_handle() esp_https_ota_get_image_len_read_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_get_image_len_read_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);
#define esp_https_ota_get_image_size_IgnoreAndReturn(cmock_retval) esp_https_ota_get_image_size_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void esp_https_ota_get_image_size_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define esp_https_ota_get_image_size_StopIgnore() esp_https_ota_get_image_size_CMockStopIgnore()
void esp_https_ota_get_image_size_CMockStopIgnore(void);
#define esp_https_ota_get_image_size_ExpectAndReturn(https_ota_handle, cmock_retval) esp_https_ota_get_image_size_CMockExpectAndReturn(__LINE__, https_ota_handle, cmock_retval)
void esp_https_ota_get_image_size_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_https_ota_handle_t https_ota_handle, int cmock_to_return);
typedef int (* CMOCK_esp_https_ota_get_image_size_CALLBACK)(esp_https_ota_handle_t https_ota_handle, int cmock_num_calls);
void esp_https_ota_get_image_size_AddCallback(CMOCK_esp_https_ota_get_image_size_CALLBACK Callback);
void esp_https_ota_get_image_size_Stub(CMOCK_esp_https_ota_get_image_size_CALLBACK Callback);
#define esp_https_ota_get_image_size_StubWithCallback esp_https_ota_get_image_size_Stub
#define esp_https_ota_get_image_size_IgnoreArg_https_ota_handle() esp_https_ota_get_image_size_CMockIgnoreArg_https_ota_handle(__LINE__)
void esp_https_ota_get_image_size_CMockIgnoreArg_https_ota_handle(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
