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
#include "ipj_util_lt.h"
#include "iri_lt.h"
#include "platform.h"

/* PURPOSE: This example illustrates the use GPIOs to initiate an inventory
   operation.  It also has an example of how to override the stop_handler
   versus using the default handler. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

static ipj_error perform_gpio_triggered_inventory(
        ipj_iri_device* iri_device,
        uint32_t timeout_ms)
{
    uint32_t end_time_ms;
    ipj_error error;

    /* GPIO 1 as output resting low*/
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_MODE, 1, 0, E_IPJ_GPIO_MODE_OUTPUT);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_MODE");
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_STATE, 1, 0, E_IPJ_GPIO_STATE_LO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_STATE");

    /* GPIO2 as floating input with attached actions */
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_MODE, 2, 0, E_IPJ_GPIO_MODE_INPUT_ACTION);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_MODE");

    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_STATE, 2, 0, E_IPJ_GPIO_STATE_FLOAT);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_STATE");

    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_HI_ACTION, 2, 0, E_IPJ_GPI_ACTION_START_INVENTORY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_HI_ACTION");

    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_LO_ACTION, 2, 0, E_IPJ_GPI_ACTION_STOP_INVENTORY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_LO_ACTION");

    /* start the GPIO set up */
    error = ipj_start(iri_device, E_IPJ_ACTION_GPIO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_GPIO");

    /* set GPIO1 high */
    error = ipj_set(iri_device, E_IPJ_KEY_GPIO_STATE, 1, 0, E_IPJ_GPIO_STATE_HI);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_GPIO_STATE");

    /* Set example end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /*  Run inventory until end time reached */
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
