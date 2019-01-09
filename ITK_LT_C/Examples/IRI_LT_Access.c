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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ipj_util_lt.h"
#include "iri_lt.h"

/* PURPOSE: This example illustrates the use of the tag operations
   Write and Write EPC. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 250

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

static ipj_error write_epc(ipj_iri_device* iri_device, uint16_t* epc)
{
    unsigned int i;
    ipj_error error;

    printf("Setting EPC to: ");

    ipj_util_print_epc(epc, 6, false);

    /* Enable tag write */
    error = ipj_set_value(iri_device, E_IPJ_KEY_TAG_OPERATION_ENABLE, true);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_TAG_OPERATION_ENABLE");

    error = ipj_set_value(iri_device, E_IPJ_KEY_TAG_OPERATION, E_IPJ_TAG_OPERATION_TYPE_WRITE);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_TAG_OPERATION");

    error = ipj_set_value(iri_device, E_IPJ_KEY_WRITE_MEM_BANK, E_IPJ_MEM_BANK_EPC);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_MEM_BANK");

    error = ipj_set_value(iri_device, E_IPJ_KEY_WRITE_WORD_POINTER, 1);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_WORD_POINTER");

    error = ipj_set_value(iri_device, E_IPJ_KEY_WRITE_WORD_COUNT, 7);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_WORD_COUNT");

    error = ipj_set(iri_device, E_IPJ_KEY_WRITE_DATA, 0, 0, 0x3000);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_DATA");

    for (i = 1; i < 7; i++)
    {
        error = ipj_set(iri_device, E_IPJ_KEY_WRITE_DATA, 0, i, epc[i - 1]);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_DATA");
    }

    return E_IPJ_ERROR_SUCCESS;
}

static ipj_error write_epc_feature(ipj_iri_device* iri_device, uint16_t* epc)
{
    unsigned int i;
    ipj_error error;

    printf("Setting EPC to: ");

    ipj_util_print_epc(epc, 6, false);

    error = ipj_set_value(iri_device, E_IPJ_KEY_TAG_OPERATION_ENABLE, true);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_TAG_OPERATION_ENABLE");

    error = ipj_set_value(iri_device, E_IPJ_KEY_TAG_OPERATION, E_IPJ_TAG_OPERATION_TYPE_WRITE_EPC);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_TAG_OPERATION");

    error = ipj_set_value(iri_device, E_IPJ_KEY_WRITE_EPC_LENGTH_CONTROL, E_IPJ_WRITE_EPC_LENGTH_CONTROL_AUTO);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_EPC_LENGTH_CONTROL");

    error = ipj_set_value(iri_device, E_IPJ_KEY_WRITE_WORD_COUNT, 6);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_WORD_COUNT");

    /* Set EPC to random numbers*/
    for (i = 0; i < 6; i++)
    {
        error = ipj_set(iri_device, E_IPJ_KEY_WRITE_DATA, 0, i, epc[i]);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set E_IPJ_KEY_WRITE_DATA");
    }

    return E_IPJ_ERROR_SUCCESS;
}

/* Main */
int main(int argc, char* argv[])
{
    /* Holder for EPC */
    uint16_t epc[6] = { 0 };

    /* Define error code */
    ipj_error error;

    /* Set-Get and Bulk_Set-Get variables */
    uint32_t i = 0;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Seed random number generator */
    srand((unsigned int) time(NULL));

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /* Bulk_Set-Get example */
    /* Set a tag EPC */

    /* Generate 6 random numbers for EPC code */
    for (i = 0; i < 6; i++)
    {
        epc[i] = rand() % 0xffff;
    }

    /* Write EPC to tag */
    write_epc(&iri_device, epc);

    ipj_util_print_divider('*', 80);
    printf("EPC Write\n");
    ipj_util_print_divider('-', 80);

    /* Perform inventory to see results of EPC write */
    ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);

    /* Generate 6 random numbers for EPC code */
    for (i = 0; i < 6; i++)
    {
        epc[i] = rand() % 0xffff;
    }

    /* Write EPC to tag using WRITE_EPC feature */
    write_epc_feature(&iri_device, epc);

    ipj_util_print_divider('*', 80);
    printf("EPC Write Feature\n");
    ipj_util_print_divider('-', 80);

    /* Perform inventory to see results of EPC write */
    ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
}
