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
#include "packet.pb.h"



const pb_field_t Packet_fields[4] = {
    {3, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    offsetof(Packet, command),
    pb_delta(Packet, has_command, command),
    pb_membersize(Packet, command), 0,
    &Command_fields},

    {4, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Packet, response, command),
    pb_delta(Packet, has_response, response),
    pb_membersize(Packet, response), 0,
    &Response_fields},

    {5, (pb_type_t) ((int32_t) PB_HTYPE_OPTIONAL | (int32_t) PB_LTYPE_SUBMESSAGE),
    pb_delta_end(Packet, report, response),
    pb_delta(Packet, has_report, report),
    pb_membersize(Packet, report), 0,
    &Report_fields},

    PB_LAST_FIELD
};

