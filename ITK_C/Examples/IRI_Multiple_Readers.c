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
#include "ipj_util.h"
#include "iri.h"
#include "platform.h"

/* PURPOSE: This example illustrates the use of multiple reader modules from 
   one application. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000
#define MAX_READERS 5

/* Declare local report handlers */
static ipj_error report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        void* report);

static ipj_error stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report);

/* Allocate memory for iri device */
static ipj_iri_device iri_devices[MAX_READERS];

static uint32_t ipj_stopped_flags[MAX_READERS];

static struct ipj_handler event_handlers[] =
{
    { E_IPJ_HANDLER_TYPE_REPORT, &report_handler }
};

static ipj_error register_handlers(ipj_iri_device* iri_device)
{
    ipj_error error;
    unsigned int i;
    for (i = 0; i < (sizeof(event_handlers) / sizeof(event_handlers[0])); i++)
    {
        error = ipj_register_handler(
                iri_device,
                event_handlers[i].type,
                event_handlers[i].handler);
        if (error)
        {
            return error;
        }
    }
    return E_IPJ_ERROR_SUCCESS;
}

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

    /*  Perform receive until end time reached */
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

        /*
         * Override the report handler so the local stop_handler can be called
         */
        register_handlers(&iri_devices[i]);
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
static ipj_error report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        void* report)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    /* Case statement for each report type */
    switch (report_id)
    {
        case E_IPJ_REPORT_ID_TAG_OPERATION_REPORT:
            error = ipj_util_tag_operation_report_handler(
                    iri_device,
                    (ipj_tag_operation_report*) report);
            break;
        case E_IPJ_REPORT_ID_STOP_REPORT:
            error = stop_report_handler(iri_device, (ipj_stop_report*) report);
            break;
        case E_IPJ_REPORT_ID_GPIO_REPORT:
            error = ipj_util_gpio_report_handler(
                    iri_device,
                    (ipj_gpio_report*) report);
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
        printf("IPJ_STOP Error. Error Code:%x\n\n", ipj_stop_report->error);
    }
    return ipj_stop_report->error;
}
