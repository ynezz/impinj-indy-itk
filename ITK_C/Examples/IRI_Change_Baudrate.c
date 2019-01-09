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

/* PURPOSE: This example illustrates the use of the change baud rate feature. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 250

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error;

    /* IRI Connection parameters */
    ipj_connection_params connection_params;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    printf("Setting Baud to 57600 and performing inventory\n");

    connection_params.serial.baudrate = E_IPJ_BAUD_RATE_BR57600;
    /* Change Baud rate to 57600 bps */
    error = ipj_modify_connection(
            &iri_device,
            E_IPJ_CONNECTION_TYPE_SERIAL,
            &connection_params);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_modify_connection");

    /* Perform inventory to test results of baud rate change */
    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
}
