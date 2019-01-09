/*
 *****************************************************************************
 * Copyright 2016-2018 Impinj, Inc.                                          *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 * http://www.apache.org/licenses/LICENSE-2.0                                *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 *****************************************************************************/
/* Automatically generated nanopb header */
#ifndef _PB_MESSAGES_PB_H_
#define _PB_MESSAGES_PB_H_
#include <pb.h>

#include "error_codes.pb.h"
#include "key_codes.pb.h"
#include "enums.pb.h"

/* Enum definitions */
typedef uint32_t ipj_reset_type;
#define E_IPJ_RESET_TYPE_SOFT ((uint32_t)(0x0))
#define E_IPJ_RESET_TYPE_TO_BOOTLOADER ((uint32_t)(0x1))
#define E_IPJ_RESET_TYPE_FACTORY_RESTORE ((uint32_t)(0x2))

typedef uint32_t ipj_connection_type;
#define E_IPJ_CONNECTION_TYPE_SERIAL ((uint32_t)(0x0))

typedef uint32_t ipj_key_type;
#define E_IPJ_KEY_TYPE_UINT32 ((uint32_t)(0x0))
#define E_IPJ_KEY_TYPE_UINT16 ((uint32_t)(0x1))
#define E_IPJ_KEY_TYPE_UINT8 ((uint32_t)(0x2))
#define E_IPJ_KEY_TYPE_INT32 ((uint32_t)(0x3))
#define E_IPJ_KEY_TYPE_INT16 ((uint32_t)(0x4))
#define E_IPJ_KEY_TYPE_INT8 ((uint32_t)(0x5))
#define E_IPJ_KEY_TYPE_BOOL ((uint32_t)(0x6))

typedef uint32_t ipj_key_permissions;
#define E_IPJ_KEY_PERMISSIONS_READ_ONLY ((uint32_t)(0x0))
#define E_IPJ_KEY_PERMISSIONS_WRITE_ONLY ((uint32_t)(0x1))
#define E_IPJ_KEY_PERMISSIONS_READ_WRITE ((uint32_t)(0x2))

typedef uint32_t ipj_action;
#define E_IPJ_ACTION_ALL ((uint32_t)(0x0))
#define E_IPJ_ACTION_INVENTORY ((uint32_t)(0x1))
#define E_IPJ_ACTION_TEST ((uint32_t)(0x2))
#define E_IPJ_ACTION_GPIO ((uint32_t)(0x3))
#define E_IPJ_ACTION_CLEAR_ERROR ((uint32_t)(0x4))
#define E_IPJ_ACTION_STANDBY ((uint32_t)(0x6))
#define E_IPJ_ACTION_SLEEP ((uint32_t)(0x7))
#define E_IPJ_ACTION_NONE ((uint32_t)(0xF))

typedef uint32_t ipj_tag_flag;
#define E_IPJ_TAG_FLAG_BIT_EPC ((uint32_t)(0x1))
#define E_IPJ_TAG_FLAG_BIT_TID ((uint32_t)(0x2))
#define E_IPJ_TAG_FLAG_BIT_PC ((uint32_t)(0x4))
#define E_IPJ_TAG_FLAG_BIT_XPC ((uint32_t)(0x8))
#define E_IPJ_TAG_FLAG_BIT_CRC ((uint32_t)(0x10))
#define E_IPJ_TAG_FLAG_BIT_TIMESTAMP ((uint32_t)(0x20))
#define E_IPJ_TAG_FLAG_BIT_RSSI ((uint32_t)(0x40))
#define E_IPJ_TAG_FLAG_BIT_PHASE ((uint32_t)(0x80))
#define E_IPJ_TAG_FLAG_BIT_CHANNEL ((uint32_t)(0x100))
#define E_IPJ_TAG_FLAG_BIT_ANTENNA ((uint32_t)(0x200))
#define E_IPJ_TAG_FLAG_BIT_RETRIES ((uint32_t)(0x400))

typedef uint32_t ipj_tag_operation_type;
#define E_IPJ_TAG_OPERATION_TYPE_NONE ((uint32_t)(0x0))
#define E_IPJ_TAG_OPERATION_TYPE_READ ((uint32_t)(0x1))
#define E_IPJ_TAG_OPERATION_TYPE_WRITE ((uint32_t)(0x2))
#define E_IPJ_TAG_OPERATION_TYPE_LOCK ((uint32_t)(0x3))
#define E_IPJ_TAG_OPERATION_TYPE_KILL ((uint32_t)(0x4))
#define E_IPJ_TAG_OPERATION_TYPE_BLOCKPERMALOCK ((uint32_t)(0x5))
#define E_IPJ_TAG_OPERATION_TYPE_WRITE_EPC ((uint32_t)(0x6))
#define E_IPJ_TAG_OPERATION_TYPE_QT ((uint32_t)(0x7))

typedef uint32_t ipj_flash_operation;
#define E_IPJ_FLASH_OPERATION_WRITE ((uint32_t)(0x1))
#define E_IPJ_FLASH_OPERATION_READ ((uint32_t)(0x2))
#define E_IPJ_FLASH_OPERATION_ERASE ((uint32_t)(0x3))
#define E_IPJ_FLASH_OPERATION_UNLOCK ((uint32_t)(0x4))

typedef uint32_t ipj_response_timestamp_flag;
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_RESET ((uint32_t)(0x1))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_MODIFY_CONN ((uint32_t)(0x2))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_GET_INFO ((uint32_t)(0x4))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_BULK_SET ((uint32_t)(0x8))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_BULK_GET ((uint32_t)(0x10))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_START ((uint32_t)(0x20))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_RESUME ((uint32_t)(0x40))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_STOP ((uint32_t)(0x80))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_INVALID ((uint32_t)(0x100))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_FLASH ((uint32_t)(0x200))

typedef uint32_t ipj_report_timestamp_flag;
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_TAG_OP ((uint32_t)(0x1))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_STOP ((uint32_t)(0x2))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_TEST ((uint32_t)(0x4))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_ERROR ((uint32_t)(0x8))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_STATUS ((uint32_t)(0x10))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_GPIO ((uint32_t)(0x20))

/* Struct definitions */
typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} BulkSetResponse;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_param1;
    uint32_t param1;
    bool has_param2;
    uint32_t param2;
    bool has_param3;
    uint32_t param3;
    bool has_param4;
    uint32_t param4;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[5];
} ErrorReport;

typedef struct
{
    size_t size;
    uint8_t bytes[256];
} FlashCommand_data_t;

typedef struct
{
    bool has_action;
    ipj_flash_operation action;
    bool has_address;
    uint32_t address;
    bool has_length;
    uint32_t length;
    bool has_data;
    FlashCommand_data_t data;
} FlashCommand;

typedef struct
{
    size_t size;
    uint8_t bytes[256];
} FlashResponse_data_t;

typedef struct
{
    bool has_action;
    ipj_flash_operation action;
    bool has_address;
    uint32_t address;
    bool has_length;
    uint32_t length;
    bool has_data;
    FlashResponse_data_t data;
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
} FlashResponse;

typedef struct
{
    size_t gpio_modes_count;
    ipj_gpio_mode gpio_modes[5];
    size_t gpio_states_count;
    ipj_gpio_state gpio_states[5];
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[10];
} GPIOReport;

typedef struct
{
    bool has_key;
    ipj_key key;
} GetInfoCommand;

typedef struct
{
    size_t size;
    uint8_t bytes[128];
} IDDReport_idd_data_t;

typedef struct
{
    bool has_idd_data;
    IDDReport_idd_data_t idd_data;
} IDDReport;

typedef struct
{
    bool has_reserved;
    uint32_t reserved;
} InvalidCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} InvalidResponse;

typedef struct
{
    bool has_key_type;
    ipj_key_type key_type;
    bool has_count;
    uint32_t count;
    bool has_length;
    uint32_t length;
    bool has_key_permissions;
    ipj_key_permissions key_permissions;
} KeyInfo;

typedef struct
{
    bool has_key;
    ipj_key key;
    size_t list_count;
    uint32_t list[32];
    bool has_bank_index;
    uint32_t bank_index;
    bool has_value_index;
    uint32_t value_index;
    bool has_length;
    uint32_t length;
} KeyList;

typedef struct
{
    bool has_key;
    ipj_key key;
    bool has_value;
    uint32_t value;
    bool has_bank_index;
    uint32_t bank_index;
    bool has_value_index;
    uint32_t value_index;
} KeyValue;

typedef struct
{
    bool has_connection_type;
    ipj_connection_type connection_type;
    bool has_value;
    uint32_t value;
    bool has_baud_autoclock;
    bool baud_autoclock;
} ModifyConnectionCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
} ModifyConnectionResponse;

typedef struct
{
    bool has_reset_type;
    ipj_reset_type reset_type;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} ResetCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} ResetResponse;

typedef struct
{
    bool has_action;
    ipj_action action;
} ResumeCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
} ResumeResponse;

typedef struct
{
    bool has_action;
    ipj_action action;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} StartCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} StartResponse;

typedef struct
{
    bool has_status_flag;
    ipj_status_flag status_flag;
    bool has_timestamp;
    uint64_t timestamp;
    bool has_status_1;
    uint32_t status_1;
    bool has_status_2;
    uint32_t status_2;
    bool has_status_3;
    uint32_t status_3;
    size_t data_count;
    uint32_t data[16];
    size_t lt_buffer_count;
    uint32_t lt_buffer[20];
} StatusReport;

typedef struct
{
    bool has_action;
    ipj_action action;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} StopCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_action;
    ipj_action action;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[2];
} StopReport;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[1];
} StopResponse;

typedef struct
{
    size_t size;
    uint8_t bytes[64];
} Tag_epc_t;

typedef struct
{
    size_t size;
    uint8_t bytes[48];
} Tag_tid_t;

typedef struct
{
    bool has_epc;
    Tag_epc_t epc;
    bool has_tid;
    Tag_tid_t tid;
    bool has_pc;
    uint32_t pc;
    bool has_xpc;
    uint32_t xpc;
    bool has_crc;
    uint32_t crc;
    bool has_timestamp;
    uint64_t timestamp;
    bool has_rssi;
    int32_t rssi;
    bool has_phase;
    int32_t phase;
    bool has_channel;
    uint32_t channel;
    bool has_antenna;
    uint32_t antenna;
} Tag;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_test_id;
    uint32_t test_id;
    bool has_result_1;
    uint32_t result_1;
    bool has_result_2;
    uint32_t result_2;
    bool has_result_3;
    uint32_t result_3;
    size_t data_count;
    uint32_t data[16];
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[21];
} TestReport;

typedef struct
{
    size_t key_value_count;
    KeyValue key_value[16];
    size_t key_list_count;
    KeyList key_list[1];
    size_t lt_buffer_count;
    uint32_t lt_buffer[3];
} BulkGetCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    size_t key_value_count;
    KeyValue key_value[16];
    size_t key_list_count;
    KeyList key_list[1];
    size_t lt_buffer_count;
    uint32_t lt_buffer[2];
} BulkGetResponse;

typedef struct
{
    size_t key_value_count;
    KeyValue key_value[16];
    size_t key_list_count;
    KeyList key_list[1];
    size_t lt_buffer_count;
    uint32_t lt_buffer[4];
} BulkSetCommand;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_timestamp;
    uint64_t timestamp;
    bool has_key_info;
    KeyInfo key_info;
} GetInfoResponse;

typedef struct
{
    size_t size;
    uint8_t bytes[64];
} TagOperationReport_tag_operation_data_t;

typedef struct
{
    bool has_error;
    ipj_error error;
    bool has_tag;
    Tag tag;
    bool has_tag_operation_type;
    ipj_tag_operation_type tag_operation_type;
    bool has_tag_operation_data;
    TagOperationReport_tag_operation_data_t tag_operation_data;
    bool has_retries;
    uint32_t retries;
    bool has_diagnostic;
    uint32_t diagnostic;
    bool has_timestamp;
    uint64_t timestamp;
    size_t lt_buffer_count;
    uint32_t lt_buffer[30];
} TagOperationReport;

/* Default values for struct fields */
extern const uint32_t KeyInfo_count_default;
extern const uint32_t KeyInfo_length_default;
extern const ipj_key_permissions KeyInfo_key_permissions_default;
extern const ipj_reset_type ResetCommand_reset_type_default;
extern const ipj_error ResetResponse_error_default;
extern const ipj_connection_type ModifyConnectionCommand_connection_type_default;
extern const uint32_t ModifyConnectionCommand_value_default;
extern const bool ModifyConnectionCommand_baud_autoclock_default;
extern const ipj_error ModifyConnectionResponse_error_default;
extern const ipj_error GetInfoResponse_error_default;
extern const ipj_error BulkSetResponse_error_default;
extern const ipj_error BulkGetResponse_error_default;
extern const ipj_action StartCommand_action_default;
extern const ipj_error StartResponse_error_default;
extern const ipj_action ResumeCommand_action_default;
extern const ipj_error ResumeResponse_error_default;
extern const ipj_action StopCommand_action_default;
extern const ipj_error StopResponse_error_default;
extern const ipj_error InvalidResponse_error_default;
extern const ipj_error TagOperationReport_error_default;
extern const uint32_t TagOperationReport_retries_default;
extern const ipj_error StopReport_error_default;
extern const ipj_error TestReport_error_default;
extern const ipj_error ErrorReport_error_default;

/* Struct field encoding specification for nanopb */
#define KEYINFO_FIELDS_INDEX_KEY_TYPE 0
#define KEYINFO_FIELDS_INDEX_COUNT 1
#define KEYINFO_FIELDS_INDEX_LENGTH 2
#define KEYINFO_FIELDS_INDEX_KEY_PERMISSIONS 3
#define KEYVALUE_FIELDS_INDEX_KEY 0
#define KEYVALUE_FIELDS_INDEX_VALUE 1
#define KEYVALUE_FIELDS_INDEX_BANK_INDEX 2
#define KEYVALUE_FIELDS_INDEX_VALUE_INDEX 3
#define KEYLIST_FIELDS_INDEX_KEY 0
#define KEYLIST_FIELDS_INDEX_LIST 1
#define KEYLIST_FIELDS_INDEX_BANK_INDEX 2
#define KEYLIST_FIELDS_INDEX_VALUE_INDEX 3
#define KEYLIST_FIELDS_INDEX_LENGTH 4
#define TAG_FIELDS_INDEX_EPC 0
#define TAG_FIELDS_INDEX_TID 1
#define TAG_FIELDS_INDEX_PC 2
#define TAG_FIELDS_INDEX_XPC 3
#define TAG_FIELDS_INDEX_CRC 4
#define TAG_FIELDS_INDEX_TIMESTAMP 5
#define TAG_FIELDS_INDEX_RSSI 6
#define TAG_FIELDS_INDEX_PHASE 7
#define TAG_FIELDS_INDEX_CHANNEL 8
#define TAG_FIELDS_INDEX_ANTENNA 9
#define RESETCOMMAND_FIELDS_INDEX_RESET_TYPE 0
#define RESETCOMMAND_FIELDS_INDEX_LT_BUFFER 1
#define RESETRESPONSE_FIELDS_INDEX_ERROR 0
#define RESETRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define RESETRESPONSE_FIELDS_INDEX_LT_BUFFER 2
#define MODIFYCONNECTIONCOMMAND_FIELDS_INDEX_CONNECTION_TYPE 0
#define MODIFYCONNECTIONCOMMAND_FIELDS_INDEX_VALUE 1
#define MODIFYCONNECTIONCOMMAND_FIELDS_INDEX_BAUD_AUTOCLOCK 2
#define MODIFYCONNECTIONRESPONSE_FIELDS_INDEX_ERROR 0
#define MODIFYCONNECTIONRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define GETINFOCOMMAND_FIELDS_INDEX_KEY 0
#define GETINFORESPONSE_FIELDS_INDEX_ERROR 0
#define GETINFORESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define GETINFORESPONSE_FIELDS_INDEX_KEY_INFO 2
#define BULKSETCOMMAND_FIELDS_INDEX_KEY_VALUE 0
#define BULKSETCOMMAND_FIELDS_INDEX_KEY_LIST 1
#define BULKSETCOMMAND_FIELDS_INDEX_LT_BUFFER 2
#define BULKSETRESPONSE_FIELDS_INDEX_ERROR 0
#define BULKSETRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define BULKSETRESPONSE_FIELDS_INDEX_LT_BUFFER 2
#define BULKGETCOMMAND_FIELDS_INDEX_KEY_VALUE 0
#define BULKGETCOMMAND_FIELDS_INDEX_KEY_LIST 1
#define BULKGETCOMMAND_FIELDS_INDEX_LT_BUFFER 2
#define BULKGETRESPONSE_FIELDS_INDEX_ERROR 0
#define BULKGETRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define BULKGETRESPONSE_FIELDS_INDEX_KEY_VALUE 2
#define BULKGETRESPONSE_FIELDS_INDEX_KEY_LIST 3
#define BULKGETRESPONSE_FIELDS_INDEX_LT_BUFFER 4
#define STARTCOMMAND_FIELDS_INDEX_ACTION 0
#define STARTCOMMAND_FIELDS_INDEX_LT_BUFFER 1
#define STARTRESPONSE_FIELDS_INDEX_ERROR 0
#define STARTRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define STARTRESPONSE_FIELDS_INDEX_LT_BUFFER 2
#define RESUMECOMMAND_FIELDS_INDEX_ACTION 0
#define RESUMERESPONSE_FIELDS_INDEX_ERROR 0
#define RESUMERESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define STOPCOMMAND_FIELDS_INDEX_ACTION 0
#define STOPCOMMAND_FIELDS_INDEX_LT_BUFFER 1
#define STOPRESPONSE_FIELDS_INDEX_ERROR 0
#define STOPRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define STOPRESPONSE_FIELDS_INDEX_LT_BUFFER 2
#define INVALIDCOMMAND_FIELDS_INDEX_RESERVED 0
#define INVALIDRESPONSE_FIELDS_INDEX_ERROR 0
#define INVALIDRESPONSE_FIELDS_INDEX_TIMESTAMP 1
#define INVALIDRESPONSE_FIELDS_INDEX_LT_BUFFER 2
#define FLASHCOMMAND_FIELDS_INDEX_ACTION 0
#define FLASHCOMMAND_FIELDS_INDEX_ADDRESS 1
#define FLASHCOMMAND_FIELDS_INDEX_LENGTH 2
#define FLASHCOMMAND_FIELDS_INDEX_DATA 3
#define FLASHRESPONSE_FIELDS_INDEX_ACTION 0
#define FLASHRESPONSE_FIELDS_INDEX_ADDRESS 1
#define FLASHRESPONSE_FIELDS_INDEX_LENGTH 2
#define FLASHRESPONSE_FIELDS_INDEX_DATA 3
#define FLASHRESPONSE_FIELDS_INDEX_ERROR 4
#define FLASHRESPONSE_FIELDS_INDEX_TIMESTAMP 5
#define TAGOPERATIONREPORT_FIELDS_INDEX_ERROR 0
#define TAGOPERATIONREPORT_FIELDS_INDEX_TAG 1
#define TAGOPERATIONREPORT_FIELDS_INDEX_TAG_OPERATION_TYPE 2
#define TAGOPERATIONREPORT_FIELDS_INDEX_TAG_OPERATION_DATA 3
#define TAGOPERATIONREPORT_FIELDS_INDEX_RETRIES 4
#define TAGOPERATIONREPORT_FIELDS_INDEX_DIAGNOSTIC 5
#define TAGOPERATIONREPORT_FIELDS_INDEX_TIMESTAMP 6
#define TAGOPERATIONREPORT_FIELDS_INDEX_LT_BUFFER 7
#define STOPREPORT_FIELDS_INDEX_ERROR 0
#define STOPREPORT_FIELDS_INDEX_ACTION 1
#define STOPREPORT_FIELDS_INDEX_TIMESTAMP 2
#define STOPREPORT_FIELDS_INDEX_LT_BUFFER 3
#define TESTREPORT_FIELDS_INDEX_ERROR 0
#define TESTREPORT_FIELDS_INDEX_TEST_ID 1
#define TESTREPORT_FIELDS_INDEX_RESULT_1 2
#define TESTREPORT_FIELDS_INDEX_RESULT_2 3
#define TESTREPORT_FIELDS_INDEX_RESULT_3 4
#define TESTREPORT_FIELDS_INDEX_DATA 5
#define TESTREPORT_FIELDS_INDEX_TIMESTAMP 6
#define TESTREPORT_FIELDS_INDEX_LT_BUFFER 7
#define ERRORREPORT_FIELDS_INDEX_ERROR 0
#define ERRORREPORT_FIELDS_INDEX_PARAM1 1
#define ERRORREPORT_FIELDS_INDEX_PARAM2 2
#define ERRORREPORT_FIELDS_INDEX_PARAM3 3
#define ERRORREPORT_FIELDS_INDEX_PARAM4 4
#define ERRORREPORT_FIELDS_INDEX_TIMESTAMP 5
#define ERRORREPORT_FIELDS_INDEX_LT_BUFFER 6
#define STATUSREPORT_FIELDS_INDEX_STATUS_FLAG 0
#define STATUSREPORT_FIELDS_INDEX_TIMESTAMP 1
#define STATUSREPORT_FIELDS_INDEX_STATUS_1 2
#define STATUSREPORT_FIELDS_INDEX_STATUS_2 3
#define STATUSREPORT_FIELDS_INDEX_STATUS_3 4
#define STATUSREPORT_FIELDS_INDEX_DATA 5
#define STATUSREPORT_FIELDS_INDEX_LT_BUFFER 6
#define GPIOREPORT_FIELDS_INDEX_GPIO_MODES 0
#define GPIOREPORT_FIELDS_INDEX_GPIO_STATES 1
#define GPIOREPORT_FIELDS_INDEX_TIMESTAMP 2
#define GPIOREPORT_FIELDS_INDEX_LT_BUFFER 3
#define IDDREPORT_FIELDS_INDEX_IDD_DATA 0
extern const pb_field_t KeyInfo_fields[5];
extern const pb_field_t KeyValue_fields[5];
extern const pb_field_t KeyList_fields[6];
extern const pb_field_t Tag_fields[11];
extern const pb_field_t ResetCommand_fields[3];
extern const pb_field_t ResetResponse_fields[4];
extern const pb_field_t ModifyConnectionCommand_fields[4];
extern const pb_field_t ModifyConnectionResponse_fields[3];
extern const pb_field_t GetInfoCommand_fields[2];
extern const pb_field_t GetInfoResponse_fields[4];
extern const pb_field_t BulkSetCommand_fields[4];
extern const pb_field_t BulkSetResponse_fields[4];
extern const pb_field_t BulkGetCommand_fields[4];
extern const pb_field_t BulkGetResponse_fields[6];
extern const pb_field_t StartCommand_fields[3];
extern const pb_field_t StartResponse_fields[4];
extern const pb_field_t ResumeCommand_fields[2];
extern const pb_field_t ResumeResponse_fields[3];
extern const pb_field_t StopCommand_fields[3];
extern const pb_field_t StopResponse_fields[4];
extern const pb_field_t InvalidCommand_fields[2];
extern const pb_field_t InvalidResponse_fields[4];
extern const pb_field_t FlashCommand_fields[5];
extern const pb_field_t FlashResponse_fields[7];
extern const pb_field_t TagOperationReport_fields[9];
extern const pb_field_t StopReport_fields[5];
extern const pb_field_t TestReport_fields[9];
extern const pb_field_t ErrorReport_fields[8];
extern const pb_field_t StatusReport_fields[8];
extern const pb_field_t GPIOReport_fields[5];
extern const pb_field_t IDDReport_fields[2];

/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for FlashCommand.data is too large. Define PB_FIELD_16BIT to fix this.
#endif

#if !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(GetInfoResponse, key_info) < 65536 && pb_membersize(BulkSetCommand, key_value[0]) < 65536 && pb_membersize(BulkSetCommand, key_list[0]) < 65536 && pb_membersize(BulkGetCommand, key_value[0]) < 65536 && pb_membersize(BulkGetCommand, key_list[0]) < 65536 && pb_membersize(BulkGetResponse, key_value[0]) < 65536 && pb_membersize(BulkGetResponse, key_list[0]) < 65536 && pb_membersize(TagOperationReport, tag) < 65536), MESSAGES_PB_H__YOU_MUST_DEFINE_PB_FIELD_32BIT)
#endif

#endif
