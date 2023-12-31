#ifdef __cplusplus
extern "C" {
#endif

/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_NVS_FLASH_H
#define _MOCK_NVS_FLASH_H

#include "unity.h"
#include "nvs_flash.h"

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

void mock_nvs_flash_Init(void);
void mock_nvs_flash_Destroy(void);
void mock_nvs_flash_Verify(void);




#define nvs_flash_init_IgnoreAndReturn(cmock_retval) nvs_flash_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_init_StopIgnore() nvs_flash_init_CMockStopIgnore()
void nvs_flash_init_CMockStopIgnore(void);
#define nvs_flash_init_ExpectAndReturn(cmock_retval) nvs_flash_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void nvs_flash_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_init_CALLBACK)(int cmock_num_calls);
void nvs_flash_init_AddCallback(CMOCK_nvs_flash_init_CALLBACK Callback);
void nvs_flash_init_Stub(CMOCK_nvs_flash_init_CALLBACK Callback);
#define nvs_flash_init_StubWithCallback nvs_flash_init_Stub
#define nvs_flash_init_partition_IgnoreAndReturn(cmock_retval) nvs_flash_init_partition_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_init_partition_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_init_partition_StopIgnore() nvs_flash_init_partition_CMockStopIgnore()
void nvs_flash_init_partition_CMockStopIgnore(void);
#define nvs_flash_init_partition_ExpectAndReturn(partition_label, cmock_retval) nvs_flash_init_partition_CMockExpectAndReturn(__LINE__, partition_label, cmock_retval)
void nvs_flash_init_partition_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* partition_label, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_init_partition_CALLBACK)(const char* partition_label, int cmock_num_calls);
void nvs_flash_init_partition_AddCallback(CMOCK_nvs_flash_init_partition_CALLBACK Callback);
void nvs_flash_init_partition_Stub(CMOCK_nvs_flash_init_partition_CALLBACK Callback);
#define nvs_flash_init_partition_StubWithCallback nvs_flash_init_partition_Stub
#define nvs_flash_init_partition_IgnoreArg_partition_label() nvs_flash_init_partition_CMockIgnoreArg_partition_label(__LINE__)
void nvs_flash_init_partition_CMockIgnoreArg_partition_label(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_init_partition_ptr_IgnoreAndReturn(cmock_retval) nvs_flash_init_partition_ptr_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_init_partition_ptr_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_init_partition_ptr_StopIgnore() nvs_flash_init_partition_ptr_CMockStopIgnore()
void nvs_flash_init_partition_ptr_CMockStopIgnore(void);
#define nvs_flash_init_partition_ptr_ExpectAndReturn(partition, cmock_retval) nvs_flash_init_partition_ptr_CMockExpectAndReturn(__LINE__, partition, cmock_retval)
void nvs_flash_init_partition_ptr_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_init_partition_ptr_CALLBACK)(const esp_partition_t* partition, int cmock_num_calls);
void nvs_flash_init_partition_ptr_AddCallback(CMOCK_nvs_flash_init_partition_ptr_CALLBACK Callback);
void nvs_flash_init_partition_ptr_Stub(CMOCK_nvs_flash_init_partition_ptr_CALLBACK Callback);
#define nvs_flash_init_partition_ptr_StubWithCallback nvs_flash_init_partition_ptr_Stub
#define nvs_flash_init_partition_ptr_ExpectWithArrayAndReturn(partition, partition_Depth, cmock_retval) nvs_flash_init_partition_ptr_CMockExpectWithArrayAndReturn(__LINE__, partition, partition_Depth, cmock_retval)
void nvs_flash_init_partition_ptr_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, int partition_Depth, esp_err_t cmock_to_return);
#define nvs_flash_init_partition_ptr_IgnoreArg_partition() nvs_flash_init_partition_ptr_CMockIgnoreArg_partition(__LINE__)
void nvs_flash_init_partition_ptr_CMockIgnoreArg_partition(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_deinit_IgnoreAndReturn(cmock_retval) nvs_flash_deinit_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_deinit_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_deinit_StopIgnore() nvs_flash_deinit_CMockStopIgnore()
void nvs_flash_deinit_CMockStopIgnore(void);
#define nvs_flash_deinit_ExpectAndReturn(cmock_retval) nvs_flash_deinit_CMockExpectAndReturn(__LINE__, cmock_retval)
void nvs_flash_deinit_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_deinit_CALLBACK)(int cmock_num_calls);
void nvs_flash_deinit_AddCallback(CMOCK_nvs_flash_deinit_CALLBACK Callback);
void nvs_flash_deinit_Stub(CMOCK_nvs_flash_deinit_CALLBACK Callback);
#define nvs_flash_deinit_StubWithCallback nvs_flash_deinit_Stub
#define nvs_flash_deinit_partition_IgnoreAndReturn(cmock_retval) nvs_flash_deinit_partition_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_deinit_partition_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_deinit_partition_StopIgnore() nvs_flash_deinit_partition_CMockStopIgnore()
void nvs_flash_deinit_partition_CMockStopIgnore(void);
#define nvs_flash_deinit_partition_ExpectAndReturn(partition_label, cmock_retval) nvs_flash_deinit_partition_CMockExpectAndReturn(__LINE__, partition_label, cmock_retval)
void nvs_flash_deinit_partition_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* partition_label, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_deinit_partition_CALLBACK)(const char* partition_label, int cmock_num_calls);
void nvs_flash_deinit_partition_AddCallback(CMOCK_nvs_flash_deinit_partition_CALLBACK Callback);
void nvs_flash_deinit_partition_Stub(CMOCK_nvs_flash_deinit_partition_CALLBACK Callback);
#define nvs_flash_deinit_partition_StubWithCallback nvs_flash_deinit_partition_Stub
#define nvs_flash_deinit_partition_IgnoreArg_partition_label() nvs_flash_deinit_partition_CMockIgnoreArg_partition_label(__LINE__)
void nvs_flash_deinit_partition_CMockIgnoreArg_partition_label(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_erase_IgnoreAndReturn(cmock_retval) nvs_flash_erase_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_erase_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_erase_StopIgnore() nvs_flash_erase_CMockStopIgnore()
void nvs_flash_erase_CMockStopIgnore(void);
#define nvs_flash_erase_ExpectAndReturn(cmock_retval) nvs_flash_erase_CMockExpectAndReturn(__LINE__, cmock_retval)
void nvs_flash_erase_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_erase_CALLBACK)(int cmock_num_calls);
void nvs_flash_erase_AddCallback(CMOCK_nvs_flash_erase_CALLBACK Callback);
void nvs_flash_erase_Stub(CMOCK_nvs_flash_erase_CALLBACK Callback);
#define nvs_flash_erase_StubWithCallback nvs_flash_erase_Stub
#define nvs_flash_erase_partition_IgnoreAndReturn(cmock_retval) nvs_flash_erase_partition_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_erase_partition_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_erase_partition_StopIgnore() nvs_flash_erase_partition_CMockStopIgnore()
void nvs_flash_erase_partition_CMockStopIgnore(void);
#define nvs_flash_erase_partition_ExpectAndReturn(part_name, cmock_retval) nvs_flash_erase_partition_CMockExpectAndReturn(__LINE__, part_name, cmock_retval)
void nvs_flash_erase_partition_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* part_name, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_erase_partition_CALLBACK)(const char* part_name, int cmock_num_calls);
void nvs_flash_erase_partition_AddCallback(CMOCK_nvs_flash_erase_partition_CALLBACK Callback);
void nvs_flash_erase_partition_Stub(CMOCK_nvs_flash_erase_partition_CALLBACK Callback);
#define nvs_flash_erase_partition_StubWithCallback nvs_flash_erase_partition_Stub
#define nvs_flash_erase_partition_IgnoreArg_part_name() nvs_flash_erase_partition_CMockIgnoreArg_part_name(__LINE__)
void nvs_flash_erase_partition_CMockIgnoreArg_part_name(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_erase_partition_ptr_IgnoreAndReturn(cmock_retval) nvs_flash_erase_partition_ptr_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_erase_partition_ptr_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_erase_partition_ptr_StopIgnore() nvs_flash_erase_partition_ptr_CMockStopIgnore()
void nvs_flash_erase_partition_ptr_CMockStopIgnore(void);
#define nvs_flash_erase_partition_ptr_ExpectAndReturn(partition, cmock_retval) nvs_flash_erase_partition_ptr_CMockExpectAndReturn(__LINE__, partition, cmock_retval)
void nvs_flash_erase_partition_ptr_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_erase_partition_ptr_CALLBACK)(const esp_partition_t* partition, int cmock_num_calls);
void nvs_flash_erase_partition_ptr_AddCallback(CMOCK_nvs_flash_erase_partition_ptr_CALLBACK Callback);
void nvs_flash_erase_partition_ptr_Stub(CMOCK_nvs_flash_erase_partition_ptr_CALLBACK Callback);
#define nvs_flash_erase_partition_ptr_StubWithCallback nvs_flash_erase_partition_ptr_Stub
#define nvs_flash_erase_partition_ptr_ExpectWithArrayAndReturn(partition, partition_Depth, cmock_retval) nvs_flash_erase_partition_ptr_CMockExpectWithArrayAndReturn(__LINE__, partition, partition_Depth, cmock_retval)
void nvs_flash_erase_partition_ptr_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, int partition_Depth, esp_err_t cmock_to_return);
#define nvs_flash_erase_partition_ptr_IgnoreArg_partition() nvs_flash_erase_partition_ptr_CMockIgnoreArg_partition(__LINE__)
void nvs_flash_erase_partition_ptr_CMockIgnoreArg_partition(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_secure_init_IgnoreAndReturn(cmock_retval) nvs_flash_secure_init_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_secure_init_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_secure_init_StopIgnore() nvs_flash_secure_init_CMockStopIgnore()
void nvs_flash_secure_init_CMockStopIgnore(void);
#define nvs_flash_secure_init_ExpectAndReturn(cfg, cmock_retval) nvs_flash_secure_init_CMockExpectAndReturn(__LINE__, cfg, cmock_retval)
void nvs_flash_secure_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_secure_init_CALLBACK)(nvs_sec_cfg_t* cfg, int cmock_num_calls);
void nvs_flash_secure_init_AddCallback(CMOCK_nvs_flash_secure_init_CALLBACK Callback);
void nvs_flash_secure_init_Stub(CMOCK_nvs_flash_secure_init_CALLBACK Callback);
#define nvs_flash_secure_init_StubWithCallback nvs_flash_secure_init_Stub
#define nvs_flash_secure_init_ExpectWithArrayAndReturn(cfg, cfg_Depth, cmock_retval) nvs_flash_secure_init_CMockExpectWithArrayAndReturn(__LINE__, cfg, cfg_Depth, cmock_retval)
void nvs_flash_secure_init_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, int cfg_Depth, esp_err_t cmock_to_return);
#define nvs_flash_secure_init_ReturnThruPtr_cfg(cfg) nvs_flash_secure_init_CMockReturnMemThruPtr_cfg(__LINE__, cfg, sizeof(nvs_sec_cfg_t))
#define nvs_flash_secure_init_ReturnArrayThruPtr_cfg(cfg, cmock_len) nvs_flash_secure_init_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_len * sizeof(*cfg))
#define nvs_flash_secure_init_ReturnMemThruPtr_cfg(cfg, cmock_size) nvs_flash_secure_init_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_size)
void nvs_flash_secure_init_CMockReturnMemThruPtr_cfg(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, size_t cmock_size);
#define nvs_flash_secure_init_IgnoreArg_cfg() nvs_flash_secure_init_CMockIgnoreArg_cfg(__LINE__)
void nvs_flash_secure_init_CMockIgnoreArg_cfg(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_secure_init_partition_IgnoreAndReturn(cmock_retval) nvs_flash_secure_init_partition_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_secure_init_partition_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_secure_init_partition_StopIgnore() nvs_flash_secure_init_partition_CMockStopIgnore()
void nvs_flash_secure_init_partition_CMockStopIgnore(void);
#define nvs_flash_secure_init_partition_ExpectAndReturn(partition_label, cfg, cmock_retval) nvs_flash_secure_init_partition_CMockExpectAndReturn(__LINE__, partition_label, cfg, cmock_retval)
void nvs_flash_secure_init_partition_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* partition_label, nvs_sec_cfg_t* cfg, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_secure_init_partition_CALLBACK)(const char* partition_label, nvs_sec_cfg_t* cfg, int cmock_num_calls);
void nvs_flash_secure_init_partition_AddCallback(CMOCK_nvs_flash_secure_init_partition_CALLBACK Callback);
void nvs_flash_secure_init_partition_Stub(CMOCK_nvs_flash_secure_init_partition_CALLBACK Callback);
#define nvs_flash_secure_init_partition_StubWithCallback nvs_flash_secure_init_partition_Stub
#define nvs_flash_secure_init_partition_ExpectWithArrayAndReturn(partition_label, cfg, cfg_Depth, cmock_retval) nvs_flash_secure_init_partition_CMockExpectWithArrayAndReturn(__LINE__, partition_label, cfg, cfg_Depth, cmock_retval)
void nvs_flash_secure_init_partition_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const char* partition_label, nvs_sec_cfg_t* cfg, int cfg_Depth, esp_err_t cmock_to_return);
#define nvs_flash_secure_init_partition_ReturnThruPtr_cfg(cfg) nvs_flash_secure_init_partition_CMockReturnMemThruPtr_cfg(__LINE__, cfg, sizeof(nvs_sec_cfg_t))
#define nvs_flash_secure_init_partition_ReturnArrayThruPtr_cfg(cfg, cmock_len) nvs_flash_secure_init_partition_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_len * sizeof(*cfg))
#define nvs_flash_secure_init_partition_ReturnMemThruPtr_cfg(cfg, cmock_size) nvs_flash_secure_init_partition_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_size)
void nvs_flash_secure_init_partition_CMockReturnMemThruPtr_cfg(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, size_t cmock_size);
#define nvs_flash_secure_init_partition_IgnoreArg_partition_label() nvs_flash_secure_init_partition_CMockIgnoreArg_partition_label(__LINE__)
void nvs_flash_secure_init_partition_CMockIgnoreArg_partition_label(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_secure_init_partition_IgnoreArg_cfg() nvs_flash_secure_init_partition_CMockIgnoreArg_cfg(__LINE__)
void nvs_flash_secure_init_partition_CMockIgnoreArg_cfg(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_generate_keys_IgnoreAndReturn(cmock_retval) nvs_flash_generate_keys_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_generate_keys_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_generate_keys_StopIgnore() nvs_flash_generate_keys_CMockStopIgnore()
void nvs_flash_generate_keys_CMockStopIgnore(void);
#define nvs_flash_generate_keys_ExpectAndReturn(partition, cfg, cmock_retval) nvs_flash_generate_keys_CMockExpectAndReturn(__LINE__, partition, cfg, cmock_retval)
void nvs_flash_generate_keys_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, nvs_sec_cfg_t* cfg, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_generate_keys_CALLBACK)(const esp_partition_t* partition, nvs_sec_cfg_t* cfg, int cmock_num_calls);
void nvs_flash_generate_keys_AddCallback(CMOCK_nvs_flash_generate_keys_CALLBACK Callback);
void nvs_flash_generate_keys_Stub(CMOCK_nvs_flash_generate_keys_CALLBACK Callback);
#define nvs_flash_generate_keys_StubWithCallback nvs_flash_generate_keys_Stub
#define nvs_flash_generate_keys_ExpectWithArrayAndReturn(partition, partition_Depth, cfg, cfg_Depth, cmock_retval) nvs_flash_generate_keys_CMockExpectWithArrayAndReturn(__LINE__, partition, partition_Depth, cfg, cfg_Depth, cmock_retval)
void nvs_flash_generate_keys_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, int partition_Depth, nvs_sec_cfg_t* cfg, int cfg_Depth, esp_err_t cmock_to_return);
#define nvs_flash_generate_keys_ReturnThruPtr_cfg(cfg) nvs_flash_generate_keys_CMockReturnMemThruPtr_cfg(__LINE__, cfg, sizeof(nvs_sec_cfg_t))
#define nvs_flash_generate_keys_ReturnArrayThruPtr_cfg(cfg, cmock_len) nvs_flash_generate_keys_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_len * sizeof(*cfg))
#define nvs_flash_generate_keys_ReturnMemThruPtr_cfg(cfg, cmock_size) nvs_flash_generate_keys_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_size)
void nvs_flash_generate_keys_CMockReturnMemThruPtr_cfg(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, size_t cmock_size);
#define nvs_flash_generate_keys_IgnoreArg_partition() nvs_flash_generate_keys_CMockIgnoreArg_partition(__LINE__)
void nvs_flash_generate_keys_CMockIgnoreArg_partition(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_generate_keys_IgnoreArg_cfg() nvs_flash_generate_keys_CMockIgnoreArg_cfg(__LINE__)
void nvs_flash_generate_keys_CMockIgnoreArg_cfg(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_read_security_cfg_IgnoreAndReturn(cmock_retval) nvs_flash_read_security_cfg_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void nvs_flash_read_security_cfg_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, esp_err_t cmock_to_return);
#define nvs_flash_read_security_cfg_StopIgnore() nvs_flash_read_security_cfg_CMockStopIgnore()
void nvs_flash_read_security_cfg_CMockStopIgnore(void);
#define nvs_flash_read_security_cfg_ExpectAndReturn(partition, cfg, cmock_retval) nvs_flash_read_security_cfg_CMockExpectAndReturn(__LINE__, partition, cfg, cmock_retval)
void nvs_flash_read_security_cfg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, nvs_sec_cfg_t* cfg, esp_err_t cmock_to_return);
typedef esp_err_t (* CMOCK_nvs_flash_read_security_cfg_CALLBACK)(const esp_partition_t* partition, nvs_sec_cfg_t* cfg, int cmock_num_calls);
void nvs_flash_read_security_cfg_AddCallback(CMOCK_nvs_flash_read_security_cfg_CALLBACK Callback);
void nvs_flash_read_security_cfg_Stub(CMOCK_nvs_flash_read_security_cfg_CALLBACK Callback);
#define nvs_flash_read_security_cfg_StubWithCallback nvs_flash_read_security_cfg_Stub
#define nvs_flash_read_security_cfg_ExpectWithArrayAndReturn(partition, partition_Depth, cfg, cfg_Depth, cmock_retval) nvs_flash_read_security_cfg_CMockExpectWithArrayAndReturn(__LINE__, partition, partition_Depth, cfg, cfg_Depth, cmock_retval)
void nvs_flash_read_security_cfg_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const esp_partition_t* partition, int partition_Depth, nvs_sec_cfg_t* cfg, int cfg_Depth, esp_err_t cmock_to_return);
#define nvs_flash_read_security_cfg_ReturnThruPtr_cfg(cfg) nvs_flash_read_security_cfg_CMockReturnMemThruPtr_cfg(__LINE__, cfg, sizeof(nvs_sec_cfg_t))
#define nvs_flash_read_security_cfg_ReturnArrayThruPtr_cfg(cfg, cmock_len) nvs_flash_read_security_cfg_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_len * sizeof(*cfg))
#define nvs_flash_read_security_cfg_ReturnMemThruPtr_cfg(cfg, cmock_size) nvs_flash_read_security_cfg_CMockReturnMemThruPtr_cfg(__LINE__, cfg, cmock_size)
void nvs_flash_read_security_cfg_CMockReturnMemThruPtr_cfg(UNITY_LINE_TYPE cmock_line, nvs_sec_cfg_t* cfg, size_t cmock_size);
#define nvs_flash_read_security_cfg_IgnoreArg_partition() nvs_flash_read_security_cfg_CMockIgnoreArg_partition(__LINE__)
void nvs_flash_read_security_cfg_CMockIgnoreArg_partition(UNITY_LINE_TYPE cmock_line);
#define nvs_flash_read_security_cfg_IgnoreArg_cfg() nvs_flash_read_security_cfg_CMockIgnoreArg_cfg(__LINE__)
void nvs_flash_read_security_cfg_CMockIgnoreArg_cfg(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif


#ifdef __cplusplus
}
#endif
