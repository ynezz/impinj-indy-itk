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
/* Automatically generated nanopb constant definitions */
#include "commands.pb.h"



const pb_field_t Command_fields[11] = {
    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(Command, bulk_set),
    pb_delta(Command, has_bulk_set, bulk_set),
    pb_membersize(Command, bulk_set), 0,
    &BulkSetCommand_fields},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, bulk_get, bulk_set),
    pb_delta(Command, has_bulk_get, bulk_get),
    pb_membersize(Command, bulk_get), 0,
    &BulkGetCommand_fields},

    {7, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, start, bulk_get),
    pb_delta(Command, has_start, start),
    pb_membersize(Command, start), 0,
    &StartCommand_fields},

    {8, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, stop, start),
    pb_delta(Command, has_stop, stop),
    pb_membersize(Command, stop), 0,
    &StopCommand_fields},

    {9, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, resume, stop),
    pb_delta(Command, has_resume, resume),
    pb_membersize(Command, resume), 0,
    &ResumeCommand_fields},

    {13, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, get_info, resume),
    pb_delta(Command, has_get_info, get_info),
    pb_membersize(Command, get_info), 0,
    &GetInfoCommand_fields},

    {14, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, reset, get_info),
    pb_delta(Command, has_reset, reset),
    pb_membersize(Command, reset), 0,
    &ResetCommand_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, invalid, reset),
    pb_delta(Command, has_invalid, invalid),
    pb_membersize(Command, invalid), 0,
    &InvalidCommand_fields},

    {16, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, modify_connection, invalid),
    pb_delta(Command, has_modify_connection, modify_connection),
    pb_membersize(Command, modify_connection), 0,
    &ModifyConnectionCommand_fields},

    {17, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Command, flash, modify_connection),
    pb_delta(Command, has_flash, flash),
    pb_membersize(Command, flash), 0,
    &FlashCommand_fields},

    PB_LAST_FIELD
};

const pb_field_t Response_fields[11] = {
    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(Response, bulk_set),
    pb_delta(Response, has_bulk_set, bulk_set),
    pb_membersize(Response, bulk_set), 0,
    &BulkSetResponse_fields},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, bulk_get, bulk_set),
    pb_delta(Response, has_bulk_get, bulk_get),
    pb_membersize(Response, bulk_get), 0,
    &BulkGetResponse_fields},

    {7, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, start, bulk_get),
    pb_delta(Response, has_start, start),
    pb_membersize(Response, start), 0,
    &StartResponse_fields},

    {8, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, stop, start),
    pb_delta(Response, has_stop, stop),
    pb_membersize(Response, stop), 0,
    &StopResponse_fields},

    {9, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, resume, stop),
    pb_delta(Response, has_resume, resume),
    pb_membersize(Response, resume), 0,
    &ResumeResponse_fields},

    {13, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, get_info, resume),
    pb_delta(Response, has_get_info, get_info),
    pb_membersize(Response, get_info), 0,
    &GetInfoResponse_fields},

    {14, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, reset, get_info),
    pb_delta(Response, has_reset, reset),
    pb_membersize(Response, reset), 0,
    &ResetResponse_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, invalid, reset),
    pb_delta(Response, has_invalid, invalid),
    pb_membersize(Response, invalid), 0,
    &InvalidResponse_fields},

    {16, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, modify_connection, invalid),
    pb_delta(Response, has_modify_connection, modify_connection),
    pb_membersize(Response, modify_connection), 0,
    &ModifyConnectionResponse_fields},

    {17, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Response, flash, modify_connection),
    pb_delta(Response, has_flash, flash),
    pb_membersize(Response, flash), 0,
    &FlashResponse_fields},

    PB_LAST_FIELD
};

const pb_field_t Report_fields[8] = {
    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(Report, tag_operation_report),
    pb_delta(Report, has_tag_operation_report, tag_operation_report),
    pb_membersize(Report, tag_operation_report), 0,
    &TagOperationReport_fields},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, stop_report, tag_operation_report),
    pb_delta(Report, has_stop_report, stop_report),
    pb_membersize(Report, stop_report), 0,
    &StopReport_fields},

    {12, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, test_report, stop_report),
    pb_delta(Report, has_test_report, test_report),
    pb_membersize(Report, test_report), 0,
    &TestReport_fields},

    {13, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, error_report, test_report),
    pb_delta(Report, has_error_report, error_report),
    pb_membersize(Report, error_report), 0,
    &ErrorReport_fields},

    {14, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, status_report, error_report),
    pb_delta(Report, has_status_report, status_report),
    pb_membersize(Report, status_report), 0,
    &StatusReport_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, gpio_report, status_report),
    pb_delta(Report, has_gpio_report, gpio_report),
    pb_membersize(Report, gpio_report), 0,
    &GPIOReport_fields},

    {16, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Report, idd_report, gpio_report),
    pb_delta(Report, has_idd_report, idd_report),
    pb_membersize(Report, idd_report), 0,
    &IDDReport_fields},

    PB_LAST_FIELD
};

