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

/* PURPOSE: This example illustrates the use of the power management modes. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

static ipj_error sleep_ms(uint32_t sleep_time_ms)
{
    uint32_t end_time_ms = platform_timestamp_ms_handler() + sleep_time_ms;
    printf("Sleeping for %d milliseconds\n", sleep_time_ms);

    while (platform_timestamp_ms_handler() < end_time_ms)
        ;
    return E_IPJ_ERROR_SUCCESS;
}

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint32_t val = 0;
    int retries = 10;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /* Configure transmit power */
    error = ipj_set_value(&iri_device, E_IPJ_KEY_ANTENNA_TX_POWER, 2300);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value E_IPJ_KEY_ANTENNA_TX_POWER");

    /* Start inventory */
    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    /* Put the device into standby mode */
    error = ipj_start(&iri_device, E_IPJ_ACTION_STANDBY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_STANDBY");

    /* Sleep for 5 seconds */
    sleep_ms(5000);

    printf("Attempting to wake Indy Module\n");

    /* Set the receive timeout MS to a lower number (100ms) */
    error = ipj_set_receive_timeout_ms(&iri_device, 100);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_receive_timeout_ms");

    /* Spin on IPJ_GET_VALUE (any key will do) until the device responds */
    do
    {
        error = ipj_get_value(&iri_device, E_IPJ_KEY_ANTENNA_TX_POWER, &val);
    } while (error && retries--);
    IPJ_UTIL_RETURN_ON_ERROR(error, "Wake Indy Module");

    printf("Successfully woke Indy Module\n");
    printf("Performing Inventory to check for full functionality\n");

    /* Reset the timeout to it's default value */
    error = ipj_set_receive_timeout_ms(&iri_device, IPJ_DEFAULT_RECEIVE_TIMEOUT_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_receive_timeout_ms");

    /* Perform second inventory */
    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    printf("Inventory Successful, putting device into sleep mode\n");
    error = ipj_start(&iri_device, E_IPJ_ACTION_SLEEP);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_SLEEP");

    printf("Successfully put device into sleep.  To wake:\n");
    printf("Jump WKUP to VCC -or- press the reset button\n");

    /* Disconnect IRI device & close serial port */
    error = ipj_disconnect(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_disconnect");

    /* Deinitialize IRI device */
    error = ipj_deinitialize_iri_device(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_deinitialize_iri_device");

    return error;
}
