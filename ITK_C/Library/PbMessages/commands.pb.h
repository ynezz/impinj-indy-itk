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
#ifndef _PB_COMMANDS_PB_H_
#define _PB_COMMANDS_PB_H_
#include <pb.h>

#include "messages.pb.h"

/* Enum definitions */
/* Struct definitions */
typedef struct
{
    bool has_bulk_set;
    BulkSetCommand bulk_set;
    bool has_bulk_get;
    BulkGetCommand bulk_get;
    bool has_start;
    StartCommand start;
    bool has_stop;
    StopCommand stop;
    bool has_resume;
    ResumeCommand resume;
    bool has_get_info;
    GetInfoCommand get_info;
    bool has_reset;
    ResetCommand reset;
    bool has_invalid;
    InvalidCommand invalid;
    bool has_modify_connection;
    ModifyConnectionCommand modify_connection;
    bool has_flash;
    FlashCommand flash;
} Command;

typedef struct
{
    bool has_tag_operation_report;
    TagOperationReport tag_operation_report;
    bool has_stop_report;
    StopReport stop_report;
    bool has_test_report;
    TestReport test_report;
    bool has_error_report;
    ErrorReport error_report;
    bool has_status_report;
    StatusReport status_report;
    bool has_gpio_report;
    GPIOReport gpio_report;
    bool has_idd_report;
    IDDReport idd_report;
} Report;

typedef struct
{
    bool has_bulk_set;
    BulkSetResponse bulk_set;
    bool has_bulk_get;
    BulkGetResponse bulk_get;
    bool has_start;
    StartResponse start;
    bool has_stop;
    StopResponse stop;
    bool has_resume;
    ResumeResponse resume;
    bool has_get_info;
    GetInfoResponse get_info;
    bool has_reset;
    ResetResponse reset;
    bool has_invalid;
    InvalidResponse invalid;
    bool has_modify_connection;
    ModifyConnectionResponse modify_connection;
    bool has_flash;
    FlashResponse flash;
} Response;

/* Default values for struct fields */

/* Struct field encoding specification for nanopb */
#define COMMAND_FIELDS_INDEX_BULK_SET 0
#define COMMAND_FIELDS_INDEX_BULK_GET 1
#define COMMAND_FIELDS_INDEX_START 2
#define COMMAND_FIELDS_INDEX_STOP 3
#define COMMAND_FIELDS_INDEX_RESUME 4
#define COMMAND_FIELDS_INDEX_GET_INFO 5
#define COMMAND_FIELDS_INDEX_RESET 6
#define COMMAND_FIELDS_INDEX_INVALID 7
#define COMMAND_FIELDS_INDEX_MODIFY_CONNECTION 8
#define COMMAND_FIELDS_INDEX_FLASH 9
#define RESPONSE_FIELDS_INDEX_BULK_SET 0
#define RESPONSE_FIELDS_INDEX_BULK_GET 1
#define RESPONSE_FIELDS_INDEX_START 2
#define RESPONSE_FIELDS_INDEX_STOP 3
#define RESPONSE_FIELDS_INDEX_RESUME 4
#define RESPONSE_FIELDS_INDEX_GET_INFO 5
#define RESPONSE_FIELDS_INDEX_RESET 6
#define RESPONSE_FIELDS_INDEX_INVALID 7
#define RESPONSE_FIELDS_INDEX_MODIFY_CONNECTION 8
#define RESPONSE_FIELDS_INDEX_FLASH 9
#define REPORT_FIELDS_INDEX_TAG_OPERATION_REPORT 0
#define REPORT_FIELDS_INDEX_STOP_REPORT 1
#define REPORT_FIELDS_INDEX_TEST_REPORT 2
#define REPORT_FIELDS_INDEX_ERROR_REPORT 3
#define REPORT_FIELDS_INDEX_STATUS_REPORT 4
#define REPORT_FIELDS_INDEX_GPIO_REPORT 5
#define REPORT_FIELDS_INDEX_IDD_REPORT 6
extern const pb_field_t Command_fields[11];
extern const pb_field_t Response_fields[11];
extern const pb_field_t Report_fields[8];

/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Command, bulk_set) < 256 && pb_membersize(Command, bulk_get) < 256 && pb_membersize(Command, start) < 256 && pb_membersize(Command, stop) < 256 && pb_membersize(Command, resume) < 256 && pb_membersize(Command, get_info) < 256 && pb_membersize(Command, reset) < 256 && pb_membersize(Command, invalid) < 256 && pb_membersize(Command, modify_connection) < 256 && pb_membersize(Command, flash) < 256 && pb_membersize(Response, bulk_set) < 256 && pb_membersize(Response, bulk_get) < 256 && pb_membersize(Response, start) < 256 && pb_membersize(Response, stop) < 256 && pb_membersize(Response, resume) < 256 && pb_membersize(Response, get_info) < 256 && pb_membersize(Response, reset) < 256 && pb_membersize(Response, invalid) < 256 && pb_membersize(Response, modify_connection) < 256 && pb_membersize(Response, flash) < 256 && pb_membersize(Report, tag_operation_report) < 256 && pb_membersize(Report, stop_report) < 256 && pb_membersize(Report, test_report) < 256 && pb_membersize(Report, error_report) < 256 && pb_membersize(Report, status_report) < 256 && pb_membersize(Report, gpio_report) < 256 && pb_membersize(Report, idd_report) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT)
#endif

#if !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Command, bulk_set) < 65536 && pb_membersize(Command, bulk_get) < 65536 && pb_membersize(Command, start) < 65536 && pb_membersize(Command, stop) < 65536 && pb_membersize(Command, resume) < 65536 && pb_membersize(Command, get_info) < 65536 && pb_membersize(Command, reset) < 65536 && pb_membersize(Command, invalid) < 65536 && pb_membersize(Command, modify_connection) < 65536 && pb_membersize(Command, flash) < 65536 && pb_membersize(Response, bulk_set) < 65536 && pb_membersize(Response, bulk_get) < 65536 && pb_membersize(Response, start) < 65536 && pb_membersize(Response, stop) < 65536 && pb_membersize(Response, resume) < 65536 && pb_membersize(Response, get_info) < 65536 && pb_membersize(Response, reset) < 65536 && pb_membersize(Response, invalid) < 65536 && pb_membersize(Response, modify_connection) < 65536 && pb_membersize(Response, flash) < 65536 && pb_membersize(Report, tag_operation_report) < 65536 && pb_membersize(Report, stop_report) < 65536 && pb_membersize(Report, test_report) < 65536 && pb_membersize(Report, error_report) < 65536 && pb_membersize(Report, status_report) < 65536 && pb_membersize(Report, gpio_report) < 65536 && pb_membersize(Report, idd_report) < 65536), COMMANDS_PB_H__YOU_MUST_DEFINE_PB_FIELD_32BIT)
#endif

#endif
