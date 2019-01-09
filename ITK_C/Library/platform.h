/*
 *****************************************************************************
 * Copyright 2016-2017 Impinj, Inc.                                          *
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

#ifndef _PLATFORM_H
#define _PLATFORM_H
#include <stdint.h>

#include "iri.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PLATFORM_DEFAULT_WRITE_TIMEOUT_MS   IPJ_DEFAULT_TRANSMIT_TIMEOUT_MS
#define PLATOFRM_DEFAULT_READ_TIMEOUT_MS    IPJ_DEFAULT_RECEIVE_TIMEOUT_MS

uint32_t platform_open_port_handler(
        IPJ_READER_CONTEXT* reader_context,
        IPJ_READER_IDENTIFIER reader_identifier,
        ipj_connection_type connection_type,
        ipj_connection_params* params);
uint32_t platform_close_port_handler(IPJ_READER_CONTEXT reader_context);
uint32_t platform_transmit_handler(
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_written);
uint32_t platform_receive_handler(
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_received,
        uint16_t timeout_ms);
uint32_t platform_timestamp_ms_handler(void);
uint32_t platform_flush_port_handler(IPJ_READER_CONTEXT reader_context);
uint32_t platform_modify_connection_handler(
        IPJ_READER_CONTEXT reader_context,
        ipj_connection_type connection_type,
        ipj_connection_params* params);
void     platform_sleep_ms_handler(uint32_t milliseconds);
uint32_t platform_reset_pin_handler(IPJ_READER_CONTEXT reader_context, bool enable);
uint32_t platform_wakeup_pin_handler(IPJ_READER_CONTEXT reader_context, bool enable);

#ifdef __cplusplus
}
#endif

#endif // _PLATFORM_H
