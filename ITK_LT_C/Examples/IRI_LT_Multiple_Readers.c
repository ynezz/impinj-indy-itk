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
#include <stdio.h>
#include <string.h>
#include "iri_lt.h"
#include "platform.h"

/* PURPOSE: This example illustrates the use of multiple readers from one
   application. It does not use ipj_util, but rather locally copies and modifies
   ipj_util helper functions to support multiple readers. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000
#define MAX_READERS 5

#define IPJ_UTIL_PRINT_ERROR(e, msg) \
printf("ERROR: %s Failed - [ERROR CODE: 0x%X]\n\n", msg, e)

#define IPJ_UTIL_RETURN_ON_ERROR(e, msg) \
if (e)                                   \
{                                        \
    IPJ_UTIL_PRINT_ERROR(e, msg);        \
    return e;                            \
}


ipj_error ipj_tag_operation_report_handler(
        ipj_iri_device* iri_device,
        uint32_t report_count_32,
        uint32_t* tag_operation_report);

static ipj_error stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report);

/* Allocate memory for iri device */
static ipj_iri_device iri_devices[MAX_READERS];

static uint32_t ipj_stopped_flags[MAX_READERS];


static ipj_error perform_inventory(int num_readers, uint32_t timeout_ms)
{
    uint32_t end_time_ms;
    ipj_error error;
    int i;
    bool all_stopped_flag = false;

    for (i = 0; i < num_readers; i++)
    {
        ipj_stopped_flags[i] = 0x00;
        error = ipj_start(&iri_devices[i], E_IPJ_ACTION_INVENTORY);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_INVENTORY");
    }

    /* Set example end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /*  Run inventory until end time reached */
    while (platform_timestamp_ms_handler() < end_time_ms)
    {
        for (i = 0; i < num_readers; i++)
        {
            /* Call ipj_receive to process tag reports  */
            error = ipj_receive(&iri_devices[i]);
            IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
        }
    }

    for (i = 0; i < num_readers; i++)
    {
        /* Stop inventory  */
        error = ipj_stop(&iri_devices[i], E_IPJ_ACTION_INVENTORY);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_stop");
    }

    /* Set stop end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    while (!all_stopped_flag && platform_timestamp_ms_handler() < end_time_ms)
    {
        for (i = 0; i < num_readers; i++)
        {
            /*  Collect the last few tags and look for the stop report */
            if (!ipj_stopped_flags[i])
            {
                /* Call ipj_receive to process tag reports  */
                error = ipj_receive(&iri_devices[i]);
                IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
            }
            all_stopped_flag &= ipj_stopped_flags[i];
        }
    }
    return E_IPJ_ERROR_SUCCESS;
}

ipj_error ipj_util_setup(ipj_iri_device* iri_device, char* reader_identifier)
{
    ipj_error error;

    /* Initialize iri_device structure */
    error = ipj_initialize_iri_device(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_initialize_iri_device");

    /* Connect to iri device - open serial port */
    error = ipj_connect(
            iri_device,
            reader_identifier,
            E_IPJ_CONNECTION_TYPE_SERIAL,
            NULL);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_connect");

    return error;
}

ipj_error ipj_util_cleanup(ipj_iri_device* iri_device)
{
    ipj_error error;

    /* Reset the IRI device */
    error = ipj_reset(iri_device, E_IPJ_RESET_TYPE_SOFT);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_reset E_IPJ_RESET_TYPE_SOFT");

    /* Disconnect IRI device & close serial port */
    error = ipj_disconnect(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_disconnect");

    return error;
}

/* Main */
int main(int argc, char* argv[])
{
    /* Loop placeholder */
    int i;

    /* Define error code */
    ipj_error error = E_IPJ_ERROR_SUCCESS;

    if (argc < 2)
    {
        printf(
                "\n\nUsage:\t%s COMx COMy...COMz \n\nwhere x/y/z is a COM port number (Maximum 5 readers)\n\n",
                argv[0]);
        return -1;
    }

    /* Loop through all of the devices we've supplied and set them up */
    for (i = 0; i < argc - 1; i++)
    {
        /* Common example setup */
        error = ipj_util_setup(&iri_devices[i], argv[1 + i]);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");
    }

    /* Start and run inventory with all devices for the specified
     * duration */
    error = perform_inventory(argc - 1, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "perform_inventory");

    /* Loop through and disconnect/deinit each iri_device */
    for (i = 0; i < argc - 1; i++)
    {
        /* Common example cleanup */
        error = ipj_util_cleanup(&iri_devices[i]);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");
    }
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
        case E_IPJ_REPORT_ID_STOP_REPORT:
            error = stop_report_handler(iri_device, (ipj_stop_report*) report);
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

/* Tag report handler processes asynchronous reports */
static ipj_error stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report)
{
    int i;
    if (ipj_stop_report->error == E_IPJ_ERROR_SUCCESS)
    {
        /* Print reader identifier */
        printf("%s: STOPPED\n", (char*) iri_device->reader_identifier);

        /* Set the stopped flag, the stop report does not have any fields that
         * need to be checked */
        for (i = 0; i < MAX_READERS; i++)
        {
            if (&iri_devices[i] == iri_device)
            {
                ipj_stopped_flags[i] = 1;
            }
        }
    }
    else
    {
        printf("%s IPJ_STOP Error. Error Code:%x\n\n", (char*)iri_device->reader_identifier, ipj_stop_report->error);
    }
    return ipj_stop_report->error;
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
            case E_IPJ_TAG_OPERATION_REPORT_ERROR:
            {
                IPJ_UTIL_PRINT_ERROR(tag_operation_report[index], "tag_operation_report");
                break;
            }
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
