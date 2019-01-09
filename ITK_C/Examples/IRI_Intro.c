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

/* PURPOSE: This example illustrates the use of the basic inventory operation
   and to retrieve Indy Module information. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint32_t value;
    ipj_key_info keyinfo;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /* Reader Info */
    printf("Indy Module Info\n");
    error = ipj_get_value(&iri_device, E_IPJ_KEY_SERIAL_NUMBER, &value);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_get_value E_IPJ_KEY_SERIAL_NUMBER");
    printf("Serial Number      : %d\n", value);
    ipj_get_value(&iri_device, E_IPJ_KEY_BOOTSTRAP_VERSION, &value);
    printf("Bootstrap Version  : 0x%08X\n", value);
    ipj_get_value(&iri_device, E_IPJ_KEY_BOOTSTRAP_CRC, &value);
    printf("Bootstrap CRC      : 0x%08X\n", value);
    ipj_get_value(&iri_device, E_IPJ_KEY_APPLICATION_VERSION, &value);
    printf("Application Version: 0x%08X\n", value);
    ipj_get_value(&iri_device, E_IPJ_KEY_APPLICATION_CRC, &value);
    printf("Application CRC    : 0x%08X\n", value);
    ipj_get_value(&iri_device, E_IPJ_KEY_MICROPROCESSOR_ID, &value);
    printf("Microprocessor     : 0x%08X\n", value);
    ipj_get(&iri_device, E_IPJ_KEY_MICROPROCESSOR_ID, 0, 1, &value);
    printf("Microprocessor Id  : 0x%08X", value);
    ipj_get(&iri_device, E_IPJ_KEY_MICROPROCESSOR_ID, 0, 2, &value);
    printf("-0x%08X", value);
    ipj_get(&iri_device, E_IPJ_KEY_MICROPROCESSOR_ID, 0, 3, &value);
    printf("-0x%08X", value);
    printf("\n\n");

    IPJ_CLEAR_STRUCT(keyinfo);

    /* Verify that we can indeed write the ANTENNA_TX_POWER key */
    error = ipj_get_info(&iri_device, E_IPJ_KEY_ANTENNA_TX_POWER, &keyinfo);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_get_info E_IPJ_KEY_ANTENNA_TX_POWER");

    /* Check to make sure the key has either write, or read/write permissions */
    if (keyinfo.key_permissions == E_IPJ_KEY_PERMISSIONS_READ_ONLY)
    {
        printf("ERROR: Unable to set ANTENNA_TX_POWER KEY\n\n");
        return -1;
    }

    /* Configure transmit power */
    error = ipj_set_value(&iri_device, E_IPJ_KEY_ANTENNA_TX_POWER, 2300);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value E_IPJ_KEY_ANTENNA_TX_POWER");

    /* Start inventory */
    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return error;
}

