/*
 *****************************************************************************
 * Copyright 2016 Impinj, Inc.                                               *
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
#ifndef __IPJ_UTIL_H
#define __IPJ_UTIL_H

#include "iri_lt.h"

/* Macro for helping handle errors */
#define IPJ_UTIL_PRINT_ERROR(e, msg) \
printf("ERROR: %s Failed - [ERROR CODE: 0x%X]\n\n", msg, e)

#define IPJ_UTIL_RETURN_ON_ERROR(e, msg) \
if (e)                                   \
{                                        \
    IPJ_UTIL_PRINT_ERROR(e, msg);        \
    return e;                            \
}

/* Macro for helping check for COM Port input */
#define IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR() \
if (argc != 2) \
{ \
    printf("\n\nUsage:\t%s COMx\n\nwhere x is a COM port number\n\n", argv[0]); \
    return -1; \
} \

extern uint32_t ipj_stopped_flag;

ipj_error ipj_util_setup(ipj_iri_device* iri_device, char* reader_identifier);
ipj_error ipj_util_cleanup(ipj_iri_device* iri_device);

void ipj_util_print_epc(uint16_t* epc, int len, bool little_endian);
void ipj_util_print_divider(char symbol, int width);

ipj_error ipj_util_perform_inventory(
        ipj_iri_device* iri_device,
        uint32_t timeout_ms);

ipj_error ipj_report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        uint32_t report_count_32,
        void* report);
ipj_error ipj_util_tag_operation_report_handler(
        ipj_iri_device* iri_device,
        uint32_t report_size_bytes,
        uint32_t* tag_operation_report);
ipj_error ipj_util_stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* stop_report);
ipj_error ipj_util_gpio_report_handler(
        ipj_iri_device* iri_device,
        ipj_gpio_report* gpio_report);
ipj_error ipj_util_error_report_handler(
        ipj_iri_device* iri_device,
        ipj_error_report* error_report);
ipj_error ipj_util_status_report_handler(
        ipj_iri_device* iri_device,
        ipj_status_report* status_report);
ipj_error ipj_util_test_report_handler(
        ipj_iri_device* iri_device,
        ipj_test_report* test_report);

#endif
