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

/* NOTE: For this example, you will need to jumper GPIO0 to GPIO1 */
#include <stdio.h>
#include <string.h>
#include "ipj_util.h"
#include "iri.h"
#include "platform.h"

/* PURPOSE: This example illustrates the use GPIOs to initiate an inventory
   operation.  It also has an example of how to override the stop_handler
   versus using the default handler. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 5000

/* Declare local report handlers */
static ipj_error report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        void* report);

static ipj_error stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report);

static uint32_t ipj_stopped_flag;

static struct ipj_handler event_handlers[] =
{
    { E_IPJ_HANDLER_TYPE_REPORT, &report_handler }
};

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

ipj_error register_handlers(ipj_iri_device* iri_device)
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

static ipj_error perform_gpio_triggered_inventory(
        ipj_iri_device* iri_device,
        uint32_t timeout_ms)
{
    uint32_t end_time_ms;
    ipj_error error;
    ipj_key_value key_value[16];
    uint32_t key_value_count = 0;

    IPJ_CLEAR_STRUCT(key_value);

    /* GPIO 1 as output resting low*/
    key_value[0].key = E_IPJ_KEY_GPIO_MODE;
    key_value[0].value = E_IPJ_GPIO_MODE_OUTPUT;
    key_value[0].bank_index = 1;
    key_value_count++;

    key_value[1].key = E_IPJ_KEY_GPIO_STATE;
    key_value[1].value = E_IPJ_GPIO_STATE_LO;
    key_value[1].bank_index = 1;
    key_value_count++;

    /* GPIO2 as floating input with attached actions */
    key_value[2].key = E_IPJ_KEY_GPIO_MODE;
    key_value[2].value = E_IPJ_GPIO_MODE_INPUT_ACTION;
    key_value[2].bank_index = 2;
    key_value_count++;

    key_value[3].key = E_IPJ_KEY_GPIO_STATE;
    key_value[3].value = E_IPJ_GPIO_STATE_FLOAT;
    key_value[3].bank_index = 2;
    key_value_count++;

    key_value[4].key = E_IPJ_KEY_GPIO_HI_ACTION;
    key_value[4].value = E_IPJ_GPI_ACTION_START_INVENTORY;
    key_value[4].bank_index = 2;
    key_value_count++;

    key_value[5].key = E_IPJ_KEY_GPIO_LO_ACTION;
    key_value[5].value = E_IPJ_GPI_ACTION_STOP_INVENTORY;
    key_value[5].bank_index = 2;
    key_value_count++;

    /* Set the keys */
    error = ipj_bulk_set(iri_device, &key_value[0], key_value_count, NULL, 0);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_set");

    /* start the GPIO set up */
    error = ipj_start(iri_device, E_IPJ_ACTION_GPIO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_GPIO");

    /* set GPIO1 high */
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_STATE, 1, 0, E_IPJ_GPIO_STATE_HI);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_STATE");

    /* Set example end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /*  Perform receive until end time reached */
    while (platform_timestamp_ms_handler() < end_time_ms)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }

    /* Set GPIO1 Low  & Stop inventory  */
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_STATE, 1, 0, E_IPJ_GPIO_STATE_LO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_STATE");

    error = ipj_stop(iri_device, E_IPJ_ACTION_GPIO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_stop E_IPJ_ACTION_GPIO");

    /* Set stop end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /*  Collect the last few tags and look for the stop report */
    while (!ipj_stopped_flag && platform_timestamp_ms_handler() < end_time_ms)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }
    return E_IPJ_ERROR_SUCCESS;
}

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error;

    uint32_t end_message_ms;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /*
     * Override the report handler so the local stop_handler can be called and
     * stop perform_gpio_triggered_inventory
     */
    register_handlers(&iri_device);

    /* Display a message to the user for ~5 seconds */
    end_message_ms = platform_timestamp_ms_handler() + 5000;
    printf("Please jump GPIO1 to GPIO2 for this demonstration\n");
    while (platform_timestamp_ms_handler() < end_message_ms)
    {
        /* Do nothing */
    }

    /* Start inventory */
    error = perform_gpio_triggered_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "perform_gpio_triggered_inventory");

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
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

/* Stop report handler processes asynchronous reports */
static ipj_error stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report)
{
    if (ipj_stop_report->error == E_IPJ_ERROR_SUCCESS)
    {
        /* Print reader identifier */
        printf("%s: STOPPED\n", (char*) iri_device->reader_identifier);

        /* Set the stopped flag, the stop report does not have any fields that
         * need to be checked */
        ipj_stopped_flag = 1;
    }
    else
    {
        IPJ_UTIL_PRINT_ERROR(ipj_stop_report->error,"stop_report");
    }
    return ipj_stop_report->error;
}
