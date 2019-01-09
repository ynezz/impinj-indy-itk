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

#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "iri.h"

/* PLATFORM_OPEN_PORT_HANDLER */
uint32_t platform_open_port_handler(
        IPJ_READER_CONTEXT* reader_context,
        IPJ_READER_IDENTIFIER reader_identifier,
        ipj_connection_type connection_type,
        ipj_connection_params* params)
{
    return 1;
}

/* PLATFORM_CLOSE_PORT_HANDLER */
uint32_t platform_close_port_handler(IPJ_READER_CONTEXT reader_context)
{
    return 1;
}

/*
 * PLATFORM_TRANSMIT_HANDLER
 *
 * Return: 1: Success, 0:Fail
 */
uint32_t platform_transmit_handler(
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_transmitted)
{
    return 1;
}

/* PLATFORM_RECEIVE_HANDLER */
uint32_t platform_receive_handler(
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_received,
        uint16_t timeout_ms)
{
    return 1;
}

/* PLATFORM_TIMESTAMP_HANDLER */
uint32_t platform_timestamp_ms_handler()
{
    return 0;
}

void platform_sleep_ms_handler(uint32_t milliseconds)
{
}

uint32_t platform_flush_port_handler(IPJ_READER_CONTEXT reader_context)
{
    return 1;
}

uint32_t platform_reset_pin_handler(IPJ_READER_CONTEXT reader_context, bool enable)
{
    return 1;
}

uint32_t platform_wakeup_pin_handler(IPJ_READER_CONTEXT reader_context, bool enable)
{
    return 1;
}

uint32_t platform_modify_connection_handler(
        IPJ_READER_CONTEXT reader_context,
        ipj_connection_type connection_type,
        ipj_connection_params* params)
{
    return 1;
}
