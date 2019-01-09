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
#ifndef _PB_PACKET_PB_H_
#define _PB_PACKET_PB_H_
#include <pb.h>

#include "commands.pb.h"

/* Enum definitions */
/* Struct definitions */
typedef struct
{
    bool has_command;
    Command command;
    bool has_response;
    Response response;
    bool has_report;
    Report report;
} Packet;

/* Default values for struct fields */

/* Struct field encoding specification for nanopb */
#define PACKET_FIELDS_INDEX_COMMAND 0
#define PACKET_FIELDS_INDEX_RESPONSE 1
#define PACKET_FIELDS_INDEX_REPORT 2
extern const pb_field_t Packet_fields[4];

/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Packet, command) < 256 && pb_membersize(Packet, response) < 256 && pb_membersize(Packet, report) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT)
#endif

#if !defined(PB_FIELD_32BIT)
STATIC_ASSERT((pb_membersize(Packet, command) < 65536 && pb_membersize(Packet, response) < 65536 && pb_membersize(Packet, report) < 65536), PACKET_PB_H__YOU_MUST_DEFINE_PB_FIELD_32BIT)
#endif

#endif
