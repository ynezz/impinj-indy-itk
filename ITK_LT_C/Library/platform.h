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

#include "iri_lt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Platform macros */
#if defined(_WIN32)
    #define IPJ_ATTRIB_PACKED
    #define __LITTLE_ENDIAN__
#elif defined(__APPLE__)
    #define IPJ_ATTRIB_PACKED  __attribute__ ((__packed__))
#elif defined(__GNUC__)
    #define IPJ_ATTRIB_PACKED  __attribute__ ((__packed__))
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define __LITTLE_ENDIAN__
    #endif
#else
    #error "Platform macros need to be defined"
#endif

// Reverse the endianess of 16 and 32-bit values
extern uint16_t ipj_swap_16(uint16_t nValue);
extern uint32_t ipj_swap_32(uint32_t nValue);

#if defined(__LITTLE_ENDIAN__)
    #define htolel(x) (x)            /* Host to little endian long */
    #define letohl(x) (x)            /* Little endian to host long */
    #if defined(__APPLE__)
        #include <netinet/in.h>
    #else
        #define htons(x)  ipj_swap_16(x)
        #define ntohs(x)  ipj_swap_16(x)
        #define htonl(x)  ipj_swap_32(x)
        #define ntohl(x)  ipj_swap_32(x)
    #endif
#else
    #define htolel(x) ipj_swap_32(x) /* Host to little endian long */
    #define letohl(x) ipj_swap_32(x) /* Little endian to host long */
    #define htons(x)  (x)
    #define ntohs(x)  (x)
    #define htonl(x)  (x)
    #define ntohl(x)  (x)
#endif

#define PLATFORM_DEFAULT_WRITE_TIMEOUT_MS   IPJ_DEFAULT_TRANSMIT_TIMEOUT_MS
#define PLATOFRM_DEFAULT_READ_TIMEOUT_MS    IPJ_DEFAULT_RECEIVE_TIMEOUT_MS

void     platform_init(void);
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
void     platform_sleep_ms_handler(uint32_t milliseconds);

#ifdef __cplusplus
}
#endif

#endif // _PLATFORM_H
