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
#include <stdio.h>
#include <string.h>
#include "iri_lt.h"
#include "platform.h"

/*****************************************************************************
 PURPOSE: This example shows how to configure an "rx only" host application.

 This allows for a very minimal host library since all the library
 code that handles commands and packet transmission has been removed.

 It does not use ipj_util, but rather locally copies and modifies
 ipj_util helper functions to support tag report reception only.

 Step 1: Enable the IRI_RX_ONLY compile switch in the host library's config.h

 Step 2: Use the Indy Demo Tool to load a stored settings file with the 
		 following contents:

<?xml version="1.0" ?>
<Settings>
  <setting>
    <key>E_IPJ_KEY_ONBOOT_START_ACTION</key>
    <values>E_IPJ_ACTION_INVENTORY</values>
  </setting>
  <setting>
    <key>E_IPJ_KEY_ENABLE_LT_REPORTS</key>
    <values>1</values>
  </setting>
</Settings>

 Step 3: 'make output/IRI_LT_RxOnly' (Note: other examples are not compatible
          with the IRI_RX_ONLY compile flag, so just calling 'make' would fail)

******************************************************************************/

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

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

ipj_error ipj_tag_operation_report_handler(
        ipj_iri_device* iri_device,
        uint32_t report_count_32,
        uint32_t* tag_operation_report);

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint32_t start_time_ms;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Initialize iri_device structure */
    error = ipj_initialize_iri_device(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_initialize_iri_device");

    /* Connect to iri device - open serial port */
    error = ipj_connect(
            &iri_device,
            argv[1],
            E_IPJ_CONNECTION_TYPE_SERIAL,
            NULL);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_connect");

    start_time_ms = platform_timestamp_ms_handler();

    /*  Receive reports until end time reached */
    while (platform_timestamp_ms_handler() - start_time_ms < IPJ_EXAMPLE_DURATION_MS)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(&iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }

    /* Disconnect IRI device & close serial port */
    error = ipj_disconnect(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_disconnect");

    return error;
}

/* Report handler processes asynchronous reports */
ipj_error ipj_report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        uint32_t report_count_32,
        void* report)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    /* Case statement for each report type */
    switch (report_id)
    {
        case E_IPJ_REPORT_ID_TAG_OPERATION_REPORT:
            error = ipj_tag_operation_report_handler(
                    iri_device,
                    report_count_32*4,
                    (uint32_t*) report);
            break;
        default:
            printf(
                    "%s: REPORT ID: %d NOT HANDLED\n",
                    (char*) iri_device->reader_identifier,
                    report_id);
            error = E_IPJ_ERROR_GENERAL_ERROR;
            break;
    }
    return error;
}

void ipj_util_print_epc(uint16_t* epc, int len, bool little_endian)
{
    int i;
    for (i = 0; i < len; i++)
    {
        /* Print hyphen every two bytes */
        if (i % 1 == 0 && i > 0)
        {
            printf("-");
        }
        /* Print one byte at a time */
        if (little_endian)
        {
            printf("%04X", (((epc[i] & 0xFF00) >> 8) | ((epc[i] & 0xFF) << 8)));
        }
        else
        {
            printf("%04X", epc[i]);
        }
    }
    printf("\n");
}

void ipj_util_print_divider(char symbol, int width)
{
    int i;
    printf("\n");
    for (i = 0; i < width; i++)
    {
        printf("%c", symbol);
    }
    printf("\n");
}

/* Tag report handler processes asynchronous reports */
ipj_error ipj_tag_operation_report_handler(
        ipj_iri_device* iri_device,
        uint32_t report_count_32,
        uint32_t* tag_operation_report)
{
    uint32_t index = 0;
    while (index < report_count_32)
    {
        uint32_t field_header = tag_operation_report[index++];
        switch (REPORT_FIELD_ID(field_header))
        {
            case E_IPJ_TAG_OPERATION_REPORT_EPC:
            {
                printf("%s EPC: ", (char*)iri_device->reader_identifier);
                ipj_util_print_epc(
                        (uint16_t*) &tag_operation_report[index],
                        REPORT_FIELD_LENGTH(field_header) / sizeof(uint16_t),
                        true);
                break;
            }
        }

        index += ROUNDED_COUNT_32(REPORT_FIELD_LENGTH(field_header));

    }

    ipj_util_print_divider('-', 80);
    return E_IPJ_ERROR_SUCCESS;
}

