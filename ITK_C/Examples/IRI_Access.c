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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ipj_util.h"
#include "iri.h"

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
    ipj_key_value key_value[16];
    ipj_key_value key_value_2[16];
    uint32_t key_value_count = 0;
    ipj_key_list key_list;
    ipj_key_list key_list_2;
    uint32_t key_list_count = 0;

    printf("Setting EPC to: ");

    ipj_util_print_epc(epc, 6, false);

    /* Enable tag write */
    key_value_count = 5;

    IPJ_CLEAR_STRUCT(key_list);
    IPJ_CLEAR_STRUCT(key_value);

    key_value[0].key = E_IPJ_KEY_TAG_OPERATION_ENABLE;
    key_value[0].value = true;

    key_value[1].key = E_IPJ_KEY_TAG_OPERATION;
    key_value[1].value = E_IPJ_TAG_OPERATION_TYPE_WRITE;

    key_value[2].key = E_IPJ_KEY_WRITE_MEM_BANK;
    key_value[2].value = E_IPJ_MEM_BANK_EPC;

    key_value[3].key = E_IPJ_KEY_WRITE_WORD_POINTER;
    key_value[3].value = 1;

    key_value[4].key = E_IPJ_KEY_WRITE_WORD_COUNT;
    key_value[4].value = 7;

    key_list_count = 1;
    key_list.key = E_IPJ_KEY_WRITE_DATA;
    key_list.list_count = 7;

    key_list.list[0] = 0x3000;

    /* Set last two halfwords of EPC to random numbers*/
    for (i = 1; i < 7; i++)
    {
        key_list.list[i] = epc[i - 1];
    }

    error = ipj_bulk_set(
            iri_device,
            &key_value[0],
            key_value_count,
            &key_list,
            key_list_count);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_set");

    /* Copy over the keys for the sake of simplicity */
    memcpy(&key_value_2, &key_value, sizeof(key_value));
    memcpy(&key_list_2, &key_list, sizeof(key_list));

    /* Set the desired key list read length*/
    key_list_2.length = 7;

    error = ipj_bulk_get(
            iri_device,
            key_value_2,
            key_value_count,
            &key_list_2,
            key_list_count);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_get");

    printf("\n");
    printf("Get Key Value Pairs\n");
    for (i = 0; i < key_value_count; i++)
    {
        printf(
                "Key: 0x%03x Value: %10d Bank Index:%d Value Index:%d\n",
                key_value_2[i].key,
                key_value_2[i].value,
                key_value_2[i].bank_index,
                key_value_2[i].value_index);
    }
    printf("\n");

    printf("Key List\n");
    printf(
            "Key: 0x%03x\t\t     Bank Index:%d Value Index:%d\t",
            key_list_2.key,
            key_list_2.bank_index,
            key_list_2.value_index);

    printf("\n[ ");
    for (i = 0; i < key_list_2.list_count; i++)
    {
        printf("0x%04X ", key_list_2.list[i]);
    }
    printf("]\n\n");

    return E_IPJ_ERROR_SUCCESS;
}

static ipj_error write_epc_feature(ipj_iri_device* iri_device, uint16_t* epc)
{
    unsigned int i;
    ipj_error error;
    ipj_key_value key_value[16];
    uint32_t key_value_count = 0;
    ipj_key_list key_list;
    uint32_t key_list_count = 0;

    printf("Setting EPC to: ");

    ipj_util_print_epc(epc, 6, false);

    /* Enable tag write */
    key_value_count = 4;

    IPJ_CLEAR_STRUCT(key_value);
    IPJ_CLEAR_STRUCT(key_list);

    key_value[0].key = E_IPJ_KEY_TAG_OPERATION_ENABLE;
    key_value[0].value = true;

    key_value[1].key = E_IPJ_KEY_TAG_OPERATION;
    key_value[1].value = E_IPJ_TAG_OPERATION_TYPE_WRITE_EPC;

    key_value[2].key = E_IPJ_KEY_WRITE_EPC_LENGTH_CONTROL;
    key_value[2].value = E_IPJ_WRITE_EPC_LENGTH_CONTROL_AUTO;

    key_value[3].key = E_IPJ_KEY_WRITE_WORD_COUNT;
    key_value[3].value = 6;

    key_list_count = 1;
    key_list.key = E_IPJ_KEY_WRITE_DATA;
    key_list.list_count = 6;

    /* Set EPC to random numbers*/
    for (i = 0; i < 6; i++)
    {
        key_list.list[i] = epc[i];
    }

    error = ipj_bulk_set(
            iri_device,
            &key_value[0],
            key_value_count,
            &key_list,
            key_list_count);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_set");

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
