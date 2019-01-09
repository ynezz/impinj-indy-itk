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
#include "messages.pb.h"

const uint32_t KeyInfo_count_default = 0;
const uint32_t KeyInfo_length_default = 0;
const ipj_key_permissions KeyInfo_key_permissions_default = E_IPJ_KEY_PERMISSIONS_READ_WRITE;
const ipj_reset_type ResetCommand_reset_type_default = E_IPJ_RESET_TYPE_SOFT;
const ipj_error ResetResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_connection_type ModifyConnectionCommand_connection_type_default = E_IPJ_CONNECTION_TYPE_SERIAL;
const uint32_t ModifyConnectionCommand_value_default = 115200;
const bool ModifyConnectionCommand_baud_autoclock_default = false;
const ipj_error ModifyConnectionResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error GetInfoResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error BulkSetResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error BulkGetResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_action StartCommand_action_default = E_IPJ_ACTION_INVENTORY;
const ipj_error StartResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_action ResumeCommand_action_default = E_IPJ_ACTION_INVENTORY;
const ipj_error ResumeResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_action StopCommand_action_default = E_IPJ_ACTION_INVENTORY;
const ipj_error StopResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error InvalidResponse_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error TagOperationReport_error_default = E_IPJ_ERROR_SUCCESS;
const uint32_t TagOperationReport_retries_default = 0;
const ipj_error StopReport_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error TestReport_error_default = E_IPJ_ERROR_SUCCESS;
const ipj_error ErrorReport_error_default = E_IPJ_ERROR_SUCCESS;


const pb_field_t KeyInfo_fields[5] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(KeyInfo, key_type),
    pb_delta(KeyInfo, has_key_type, key_type),
    pb_membersize(KeyInfo, key_type), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyInfo, count, key_type),
    pb_delta(KeyInfo, has_count, count),
    pb_membersize(KeyInfo, count), 0,
    &KeyInfo_count_default},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyInfo, length, count),
    pb_delta(KeyInfo, has_length, length),
    pb_membersize(KeyInfo, length), 0,
    &KeyInfo_length_default},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyInfo, key_permissions, length),
    pb_delta(KeyInfo, has_key_permissions, key_permissions),
    pb_membersize(KeyInfo, key_permissions), 0,
    &KeyInfo_key_permissions_default},

    PB_LAST_FIELD
};

const pb_field_t KeyValue_fields[5] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(KeyValue, key),
    pb_delta(KeyValue, has_key, key),
    pb_membersize(KeyValue, key), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyValue, value, key),
    pb_delta(KeyValue, has_value, value),
    pb_membersize(KeyValue, value), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyValue, bank_index, value),
    pb_delta(KeyValue, has_bank_index, bank_index),
    pb_membersize(KeyValue, bank_index), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyValue, value_index, bank_index),
    pb_delta(KeyValue, has_value_index, value_index),
    pb_membersize(KeyValue, value_index), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t KeyList_fields[6] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(KeyList, key),
    pb_delta(KeyList, has_key, key),
    pb_membersize(KeyList, key), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyList, list, key),
    pb_delta(KeyList, list_count, list),
    pb_membersize(KeyList, list[0]),
    pb_membersize(KeyList, list) / pb_membersize(KeyList, list[0]), 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyList, bank_index, list),
    pb_delta(KeyList, has_bank_index, bank_index),
    pb_membersize(KeyList, bank_index), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyList, value_index, bank_index),
    pb_delta(KeyList, has_value_index, value_index),
    pb_membersize(KeyList, value_index), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(KeyList, length, value_index),
    pb_delta(KeyList, has_length, length),
    pb_membersize(KeyList, length), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t Tag_fields[11] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    offsetof(Tag, epc),
    pb_delta(Tag, has_epc, epc),
    pb_membersize(Tag, epc), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    pb_delta_end(Tag, tid, epc),
    pb_delta(Tag, has_tid, tid),
    pb_membersize(Tag, tid), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, pc, tid),
    pb_delta(Tag, has_pc, pc),
    pb_membersize(Tag, pc), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, xpc, pc),
    pb_delta(Tag, has_xpc, xpc),
    pb_membersize(Tag, xpc), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, crc, xpc),
    pb_delta(Tag, has_crc, crc),
    pb_membersize(Tag, crc), 0, 0},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, timestamp, crc),
    pb_delta(Tag, has_timestamp, timestamp),
    pb_membersize(Tag, timestamp), 0, 0},

    {7, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SVARINT),
    pb_delta_end(Tag, rssi, timestamp),
    pb_delta(Tag, has_rssi, rssi),
    pb_membersize(Tag, rssi), 0, 0},

    {8, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SVARINT),
    pb_delta_end(Tag, phase, rssi),
    pb_delta(Tag, has_phase, phase),
    pb_membersize(Tag, phase), 0, 0},

    {9, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, channel, phase),
    pb_delta(Tag, has_channel, channel),
    pb_membersize(Tag, channel), 0, 0},

    {10, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(Tag, antenna, channel),
    pb_delta(Tag, has_antenna, antenna),
    pb_membersize(Tag, antenna), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t ResetCommand_fields[3] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ResetCommand, reset_type),
    pb_delta(ResetCommand, has_reset_type, reset_type),
    pb_membersize(ResetCommand, reset_type), 0,
    &ResetCommand_reset_type_default},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(ResetCommand, lt_buffer, reset_type),
    pb_delta(ResetCommand, lt_buffer_count, lt_buffer),
    pb_membersize(ResetCommand, lt_buffer[0]),
    pb_membersize(ResetCommand, lt_buffer) / pb_membersize(ResetCommand, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t ResetResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ResetResponse, error),
    pb_delta(ResetResponse, has_error, error),
    pb_membersize(ResetResponse, error), 0,
    &ResetResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ResetResponse, timestamp, error),
    pb_delta(ResetResponse, has_timestamp, timestamp),
    pb_membersize(ResetResponse, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(ResetResponse, lt_buffer, timestamp),
    pb_delta(ResetResponse, lt_buffer_count, lt_buffer),
    pb_membersize(ResetResponse, lt_buffer[0]),
    pb_membersize(ResetResponse, lt_buffer) / pb_membersize(ResetResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t ModifyConnectionCommand_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ModifyConnectionCommand, connection_type),
    pb_delta(ModifyConnectionCommand, has_connection_type, connection_type),
    pb_membersize(ModifyConnectionCommand, connection_type), 0,
    &ModifyConnectionCommand_connection_type_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ModifyConnectionCommand, value, connection_type),
    pb_delta(ModifyConnectionCommand, has_value, value),
    pb_membersize(ModifyConnectionCommand, value), 0,
    &ModifyConnectionCommand_value_default},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ModifyConnectionCommand, baud_autoclock, value),
    pb_delta(ModifyConnectionCommand, has_baud_autoclock, baud_autoclock),
    pb_membersize(ModifyConnectionCommand, baud_autoclock), 0,
    &ModifyConnectionCommand_baud_autoclock_default},

    PB_LAST_FIELD
};

const pb_field_t ModifyConnectionResponse_fields[3] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ModifyConnectionResponse, error),
    pb_delta(ModifyConnectionResponse, has_error, error),
    pb_membersize(ModifyConnectionResponse, error), 0,
    &ModifyConnectionResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ModifyConnectionResponse, timestamp, error),
    pb_delta(ModifyConnectionResponse, has_timestamp, timestamp),
    pb_membersize(ModifyConnectionResponse, timestamp), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t GetInfoCommand_fields[2] = {
    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(GetInfoCommand, key),
    pb_delta(GetInfoCommand, has_key, key),
    pb_membersize(GetInfoCommand, key), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t GetInfoResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(GetInfoResponse, error),
    pb_delta(GetInfoResponse, has_error, error),
    pb_membersize(GetInfoResponse, error), 0,
    &GetInfoResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(GetInfoResponse, timestamp, error),
    pb_delta(GetInfoResponse, has_timestamp, timestamp),
    pb_membersize(GetInfoResponse, timestamp), 0, 0},

    {8, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(GetInfoResponse, key_info, timestamp),
    pb_delta(GetInfoResponse, has_key_info, key_info),
    pb_membersize(GetInfoResponse, key_info), 0,
    &KeyInfo_fields},

    PB_LAST_FIELD
};

const pb_field_t BulkSetCommand_fields[4] = {
    {4, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(BulkSetCommand, key_value),
    pb_delta(BulkSetCommand, key_value_count, key_value),
    pb_membersize(BulkSetCommand, key_value[0]),
    pb_membersize(BulkSetCommand, key_value) / pb_membersize(BulkSetCommand, key_value[0]),
    &KeyValue_fields},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(BulkSetCommand, key_list, key_value),
    pb_delta(BulkSetCommand, key_list_count, key_list),
    pb_membersize(BulkSetCommand, key_list[0]),
    pb_membersize(BulkSetCommand, key_list) / pb_membersize(BulkSetCommand, key_list[0]),
    &KeyList_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(BulkSetCommand, lt_buffer, key_list),
    pb_delta(BulkSetCommand, lt_buffer_count, lt_buffer),
    pb_membersize(BulkSetCommand, lt_buffer[0]),
    pb_membersize(BulkSetCommand, lt_buffer) / pb_membersize(BulkSetCommand, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t BulkSetResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(BulkSetResponse, error),
    pb_delta(BulkSetResponse, has_error, error),
    pb_membersize(BulkSetResponse, error), 0,
    &BulkSetResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(BulkSetResponse, timestamp, error),
    pb_delta(BulkSetResponse, has_timestamp, timestamp),
    pb_membersize(BulkSetResponse, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(BulkSetResponse, lt_buffer, timestamp),
    pb_delta(BulkSetResponse, lt_buffer_count, lt_buffer),
    pb_membersize(BulkSetResponse, lt_buffer[0]),
    pb_membersize(BulkSetResponse, lt_buffer) / pb_membersize(BulkSetResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t BulkGetCommand_fields[4] = {
    {4, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(BulkGetCommand, key_value),
    pb_delta(BulkGetCommand, key_value_count, key_value),
    pb_membersize(BulkGetCommand, key_value[0]),
    pb_membersize(BulkGetCommand, key_value) / pb_membersize(BulkGetCommand, key_value[0]),
    &KeyValue_fields},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(BulkGetCommand, key_list, key_value),
    pb_delta(BulkGetCommand, key_list_count, key_list),
    pb_membersize(BulkGetCommand, key_list[0]),
    pb_membersize(BulkGetCommand, key_list) / pb_membersize(BulkGetCommand, key_list[0]),
    &KeyList_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(BulkGetCommand, lt_buffer, key_list),
    pb_delta(BulkGetCommand, lt_buffer_count, lt_buffer),
    pb_membersize(BulkGetCommand, lt_buffer[0]),
    pb_membersize(BulkGetCommand, lt_buffer) / pb_membersize(BulkGetCommand, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t BulkGetResponse_fields[6] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(BulkGetResponse, error),
    pb_delta(BulkGetResponse, has_error, error),
    pb_membersize(BulkGetResponse, error), 0,
    &BulkGetResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(BulkGetResponse, timestamp, error),
    pb_delta(BulkGetResponse, has_timestamp, timestamp),
    pb_membersize(BulkGetResponse, timestamp), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(BulkGetResponse, key_value, timestamp),
    pb_delta(BulkGetResponse, key_value_count, key_value),
    pb_membersize(BulkGetResponse, key_value[0]),
    pb_membersize(BulkGetResponse, key_value) / pb_membersize(BulkGetResponse, key_value[0]),
    &KeyValue_fields},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(BulkGetResponse, key_list, key_value),
    pb_delta(BulkGetResponse, key_list_count, key_list),
    pb_membersize(BulkGetResponse, key_list[0]),
    pb_membersize(BulkGetResponse, key_list) / pb_membersize(BulkGetResponse, key_list[0]),
    &KeyList_fields},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(BulkGetResponse, lt_buffer, key_list),
    pb_delta(BulkGetResponse, lt_buffer_count, lt_buffer),
    pb_membersize(BulkGetResponse, lt_buffer[0]),
    pb_membersize(BulkGetResponse, lt_buffer) / pb_membersize(BulkGetResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t StartCommand_fields[3] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StartCommand, action),
    pb_delta(StartCommand, has_action, action),
    pb_membersize(StartCommand, action), 0,
    &StartCommand_action_default},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StartCommand, lt_buffer, action),
    pb_delta(StartCommand, lt_buffer_count, lt_buffer),
    pb_membersize(StartCommand, lt_buffer[0]),
    pb_membersize(StartCommand, lt_buffer) / pb_membersize(StartCommand, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t StartResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StartResponse, error),
    pb_delta(StartResponse, has_error, error),
    pb_membersize(StartResponse, error), 0,
    &StartResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StartResponse, timestamp, error),
    pb_delta(StartResponse, has_timestamp, timestamp),
    pb_membersize(StartResponse, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StartResponse, lt_buffer, timestamp),
    pb_delta(StartResponse, lt_buffer_count, lt_buffer),
    pb_membersize(StartResponse, lt_buffer[0]),
    pb_membersize(StartResponse, lt_buffer) / pb_membersize(StartResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t ResumeCommand_fields[2] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ResumeCommand, action),
    pb_delta(ResumeCommand, has_action, action),
    pb_membersize(ResumeCommand, action), 0,
    &ResumeCommand_action_default},

    PB_LAST_FIELD
};

const pb_field_t ResumeResponse_fields[3] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ResumeResponse, error),
    pb_delta(ResumeResponse, has_error, error),
    pb_membersize(ResumeResponse, error), 0,
    &ResumeResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ResumeResponse, timestamp, error),
    pb_delta(ResumeResponse, has_timestamp, timestamp),
    pb_membersize(ResumeResponse, timestamp), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t StopCommand_fields[3] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StopCommand, action),
    pb_delta(StopCommand, has_action, action),
    pb_membersize(StopCommand, action), 0,
    &StopCommand_action_default},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StopCommand, lt_buffer, action),
    pb_delta(StopCommand, lt_buffer_count, lt_buffer),
    pb_membersize(StopCommand, lt_buffer[0]),
    pb_membersize(StopCommand, lt_buffer) / pb_membersize(StopCommand, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t StopResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StopResponse, error),
    pb_delta(StopResponse, has_error, error),
    pb_membersize(StopResponse, error), 0,
    &StopResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StopResponse, timestamp, error),
    pb_delta(StopResponse, has_timestamp, timestamp),
    pb_membersize(StopResponse, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StopResponse, lt_buffer, timestamp),
    pb_delta(StopResponse, lt_buffer_count, lt_buffer),
    pb_membersize(StopResponse, lt_buffer[0]),
    pb_membersize(StopResponse, lt_buffer) / pb_membersize(StopResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t InvalidCommand_fields[2] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(InvalidCommand, reserved),
    pb_delta(InvalidCommand, has_reserved, reserved),
    pb_membersize(InvalidCommand, reserved), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t InvalidResponse_fields[4] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(InvalidResponse, error),
    pb_delta(InvalidResponse, has_error, error),
    pb_membersize(InvalidResponse, error), 0,
    &InvalidResponse_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(InvalidResponse, timestamp, error),
    pb_delta(InvalidResponse, has_timestamp, timestamp),
    pb_membersize(InvalidResponse, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(InvalidResponse, lt_buffer, timestamp),
    pb_delta(InvalidResponse, lt_buffer_count, lt_buffer),
    pb_membersize(InvalidResponse, lt_buffer[0]),
    pb_membersize(InvalidResponse, lt_buffer) / pb_membersize(InvalidResponse, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t FlashCommand_fields[5] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(FlashCommand, action),
    pb_delta(FlashCommand, has_action, action),
    pb_membersize(FlashCommand, action), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashCommand, address, action),
    pb_delta(FlashCommand, has_address, address),
    pb_membersize(FlashCommand, address), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashCommand, length, address),
    pb_delta(FlashCommand, has_length, length),
    pb_membersize(FlashCommand, length), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    pb_delta_end(FlashCommand, data, length),
    pb_delta(FlashCommand, has_data, data),
    pb_membersize(FlashCommand, data), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t FlashResponse_fields[7] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(FlashResponse, action),
    pb_delta(FlashResponse, has_action, action),
    pb_membersize(FlashResponse, action), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashResponse, address, action),
    pb_delta(FlashResponse, has_address, address),
    pb_membersize(FlashResponse, address), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashResponse, length, address),
    pb_delta(FlashResponse, has_length, length),
    pb_membersize(FlashResponse, length), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    pb_delta_end(FlashResponse, data, length),
    pb_delta(FlashResponse, has_data, data),
    pb_membersize(FlashResponse, data), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashResponse, error, data),
    pb_delta(FlashResponse, has_error, error),
    pb_membersize(FlashResponse, error), 0, 0},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(FlashResponse, timestamp, error),
    pb_delta(FlashResponse, has_timestamp, timestamp),
    pb_membersize(FlashResponse, timestamp), 0, 0},

    PB_LAST_FIELD
};

const pb_field_t TagOperationReport_fields[9] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(TagOperationReport, error),
    pb_delta(TagOperationReport, has_error, error),
    pb_membersize(TagOperationReport, error), 0,
    &TagOperationReport_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(TagOperationReport, tag, error),
    pb_delta(TagOperationReport, has_tag, tag),
    pb_membersize(TagOperationReport, tag), 0,
    &Tag_fields},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TagOperationReport, tag_operation_type, tag),
    pb_delta(TagOperationReport, has_tag_operation_type, tag_operation_type),
    pb_membersize(TagOperationReport, tag_operation_type), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    pb_delta_end(TagOperationReport, tag_operation_data, tag_operation_type),
    pb_delta(TagOperationReport, has_tag_operation_data, tag_operation_data),
    pb_membersize(TagOperationReport, tag_operation_data), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TagOperationReport, retries, tag_operation_data),
    pb_delta(TagOperationReport, has_retries, retries),
    pb_membersize(TagOperationReport, retries), 0,
    &TagOperationReport_retries_default},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TagOperationReport, diagnostic, retries),
    pb_delta(TagOperationReport, has_diagnostic, diagnostic),
    pb_membersize(TagOperationReport, diagnostic), 0, 0},

    {7, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TagOperationReport, timestamp, diagnostic),
    pb_delta(TagOperationReport, has_timestamp, timestamp),
    pb_membersize(TagOperationReport, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(TagOperationReport, lt_buffer, timestamp),
    pb_delta(TagOperationReport, lt_buffer_count, lt_buffer),
    pb_membersize(TagOperationReport, lt_buffer[0]),
    pb_membersize(TagOperationReport, lt_buffer) / pb_membersize(TagOperationReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t StopReport_fields[5] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StopReport, error),
    pb_delta(StopReport, has_error, error),
    pb_membersize(StopReport, error), 0,
    &StopReport_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StopReport, action, error),
    pb_delta(StopReport, has_action, action),
    pb_membersize(StopReport, action), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StopReport, timestamp, action),
    pb_delta(StopReport, has_timestamp, timestamp),
    pb_membersize(StopReport, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StopReport, lt_buffer, timestamp),
    pb_delta(StopReport, lt_buffer_count, lt_buffer),
    pb_membersize(StopReport, lt_buffer[0]),
    pb_membersize(StopReport, lt_buffer) / pb_membersize(StopReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t TestReport_fields[9] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(TestReport, error),
    pb_delta(TestReport, has_error, error),
    pb_membersize(TestReport, error), 0,
    &TestReport_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, test_id, error),
    pb_delta(TestReport, has_test_id, test_id),
    pb_membersize(TestReport, test_id), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, result_1, test_id),
    pb_delta(TestReport, has_result_1, result_1),
    pb_membersize(TestReport, result_1), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, result_2, result_1),
    pb_delta(TestReport, has_result_2, result_2),
    pb_membersize(TestReport, result_2), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, result_3, result_2),
    pb_delta(TestReport, has_result_3, result_3),
    pb_membersize(TestReport, result_3), 0, 0},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, data, result_3),
    pb_delta(TestReport, data_count, data),
    pb_membersize(TestReport, data[0]),
    pb_membersize(TestReport, data) / pb_membersize(TestReport, data[0]), 0},

    {7, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(TestReport, timestamp, data),
    pb_delta(TestReport, has_timestamp, timestamp),
    pb_membersize(TestReport, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(TestReport, lt_buffer, timestamp),
    pb_delta(TestReport, lt_buffer_count, lt_buffer),
    pb_membersize(TestReport, lt_buffer[0]),
    pb_membersize(TestReport, lt_buffer) / pb_membersize(TestReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t ErrorReport_fields[8] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(ErrorReport, error),
    pb_delta(ErrorReport, has_error, error),
    pb_membersize(ErrorReport, error), 0,
    &ErrorReport_error_default},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ErrorReport, param1, error),
    pb_delta(ErrorReport, has_param1, param1),
    pb_membersize(ErrorReport, param1), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ErrorReport, param2, param1),
    pb_delta(ErrorReport, has_param2, param2),
    pb_membersize(ErrorReport, param2), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ErrorReport, param3, param2),
    pb_delta(ErrorReport, has_param3, param3),
    pb_membersize(ErrorReport, param3), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ErrorReport, param4, param3),
    pb_delta(ErrorReport, has_param4, param4),
    pb_membersize(ErrorReport, param4), 0, 0},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(ErrorReport, timestamp, param4),
    pb_delta(ErrorReport, has_timestamp, timestamp),
    pb_membersize(ErrorReport, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(ErrorReport, lt_buffer, timestamp),
    pb_delta(ErrorReport, lt_buffer_count, lt_buffer),
    pb_membersize(ErrorReport, lt_buffer[0]),
    pb_membersize(ErrorReport, lt_buffer) / pb_membersize(ErrorReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t StatusReport_fields[8] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    offsetof(StatusReport, status_flag),
    pb_delta(StatusReport, has_status_flag, status_flag),
    pb_membersize(StatusReport, status_flag), 0, 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StatusReport, timestamp, status_flag),
    pb_delta(StatusReport, has_timestamp, timestamp),
    pb_membersize(StatusReport, timestamp), 0, 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StatusReport, status_1, timestamp),
    pb_delta(StatusReport, has_status_1, status_1),
    pb_membersize(StatusReport, status_1), 0, 0},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StatusReport, status_2, status_1),
    pb_delta(StatusReport, has_status_2, status_2),
    pb_membersize(StatusReport, status_2), 0, 0},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StatusReport, status_3, status_2),
    pb_delta(StatusReport, has_status_3, status_3),
    pb_membersize(StatusReport, status_3), 0, 0},

    {6, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(StatusReport, data, status_3),
    pb_delta(StatusReport, data_count, data),
    pb_membersize(StatusReport, data[0]),
    pb_membersize(StatusReport, data) / pb_membersize(StatusReport, data[0]), 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(StatusReport, lt_buffer, data),
    pb_delta(StatusReport, lt_buffer_count, lt_buffer),
    pb_membersize(StatusReport, lt_buffer[0]),
    pb_membersize(StatusReport, lt_buffer) / pb_membersize(StatusReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t GPIOReport_fields[5] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_VARINT),
    offsetof(GPIOReport, gpio_modes),
    pb_delta(GPIOReport, gpio_modes_count, gpio_modes),
    pb_membersize(GPIOReport, gpio_modes[0]),
    pb_membersize(GPIOReport, gpio_modes) / pb_membersize(GPIOReport, gpio_modes[0]), 0},

    {2, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(GPIOReport, gpio_states, gpio_modes),
    pb_delta(GPIOReport, gpio_states_count, gpio_states),
    pb_membersize(GPIOReport, gpio_states[0]),
    pb_membersize(GPIOReport, gpio_states) / pb_membersize(GPIOReport, gpio_states[0]), 0},

    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_VARINT),
    pb_delta_end(GPIOReport, timestamp, gpio_states),
    pb_delta(GPIOReport, has_timestamp, timestamp),
    pb_membersize(GPIOReport, timestamp), 0, 0},

    {15, (pb_type_t) ((int32_t) PB_HTYPE_ARRAY | (int32_t) PB_LTYPE_FIXED32),
    pb_delta_end(GPIOReport, lt_buffer, timestamp),
    pb_delta(GPIOReport, lt_buffer_count, lt_buffer),
    pb_membersize(GPIOReport, lt_buffer[0]),
    pb_membersize(GPIOReport, lt_buffer) / pb_membersize(GPIOReport, lt_buffer[0]), 0},

    PB_LAST_FIELD
};

const pb_field_t IDDReport_fields[2] = {
    {1, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_BYTES),
    offsetof(IDDReport, idd_data),
    pb_delta(IDDReport, has_idd_data, idd_data),
    pb_membersize(IDDReport, idd_data), 0, 0},

    PB_LAST_FIELD
};

