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

/* PURPOSE: This example illustrates the use of Gen2 Select operation. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 250

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

/* This select command will cause only tags with a TID starting with
 * 0xe280 to set their select flags  */
static ipj_error setup_select_1(ipj_iri_device* iri_device)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_ENABLE, 0, 0, true);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_TARGET, 0, 0,
    E_IPJ_SELECT_TARGET_SL_FLAG);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_ACTION, 0, 0,
    E_IPJ_SELECT_ACTION_ASLINVA_DSLINVB);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MEM_BANK, 0, 0,
    E_IPJ_MEM_BANK_TID);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_POINTER, 0, 0, 0x0);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MASK_LENGTH, 0, 0, 16);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MASK_VALUE, 0, 0, 0xE280);

    return error;
}

/* This select command will cause only tags with 1 at bit 19
 to maintain their select flag, else their SL flag will de-assert.
 This causes and AND effect with the first select.
 The 2 selects combined will select Monza4/5 tags*/
static ipj_error setup_select_2(ipj_iri_device* iri_device)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_ENABLE, 1, 0, true);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_TARGET, 1, 0,
    E_IPJ_SELECT_TARGET_SL_FLAG);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_ACTION, 1, 0,
    E_IPJ_SELECT_ACTION_NOTHING_DSLINVB);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MEM_BANK, 1, 0,
    E_IPJ_MEM_BANK_TID);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_POINTER, 1, 0, 19);

    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MASK_LENGTH, 1, 0, 1);

    /* Note: When the mod of the length is less than 16, the MSBs
     of the mask are used.  So for 1 bit length, the msb of the
     mask (bit 15) is the mask and the LSBs are ignored. */
    error |= ipj_set(iri_device,
    E_IPJ_KEY_SELECT_MASK_VALUE, 1, 0, 0x8000);

    return error;
}

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    printf("Sending Select Command for Monza4/5 tags (TID = 0xE280.1xxx)\n");
    /* Send Select Command */
    error = setup_select_1(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "setup_select_1");

    error = setup_select_2(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "setup_select_2");

    /*
     * NOTE: In this example, we target the SL flag, hence setting it
     * here.  Your Select use case may or may not require this.
     * Please refer to the GEN2 spec for details
     */
    error = ipj_set_value(&iri_device, E_IPJ_KEY_INVENTORY_SELECT_FLAG, E_IPJ_INVENTORY_SELECT_FLAG_SL);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value E_IPJ_KEY_INVENTORY_SELECT_FLAG");

    error = ipj_set_value(&iri_device, E_IPJ_KEY_TAG_OPERATION, 0);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value E_IPJ_KEY_TAG_OPERATION");

    /* Perform inventory to see results of EPC write */
    ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
}
