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

/* PURPOSE: This file is used to demo the external antenna mux feature.
   The four GPIO pins are redirected to being used by the reader firmware
   to switch antennas using an external RF switch. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 2000

#define NUM_EXTERNAL_MUX_ANTENNAS 16

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

ipj_error enable_external_antenna_mux();
ipj_error disable_external_antenna_mux();

ipj_error config_external_antenna_mux(
    unsigned int num_antennas,
    unsigned int physical_port,
    unsigned int delay_microseconds
);

ipj_error set_antenna_sequence();

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error = E_IPJ_ERROR_SUCCESS;

    IPJ_UTIL_CHECK_USER_INPUT_FOR_COM_PORT_RETURN_ON_ERROR()

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    error = set_antenna_sequence();
    IPJ_UTIL_RETURN_ON_ERROR(error, "set_antenna_sequence");

    error = config_external_antenna_mux(NUM_EXTERNAL_MUX_ANTENNAS, 1, 0);
    IPJ_UTIL_RETURN_ON_ERROR(error, "config_external_antenna_mux");

    error = enable_external_antenna_mux();
    IPJ_UTIL_RETURN_ON_ERROR(error, "enable_external_antenna_mux");
	
	/* Enable antenna field in tag operation reports */
	error = ipj_set_value(&iri_device, E_IPJ_KEY_REPORT_CONTROL_TAG, E_IPJ_TAG_FLAG_BIT_EPC | E_IPJ_TAG_FLAG_BIT_TIMESTAMP | E_IPJ_TAG_FLAG_BIT_TID | E_IPJ_TAG_FLAG_BIT_ANTENNA);
	IPJ_UTIL_RETURN_ON_ERROR(error, "set_tag_operation_report_fields");

	/* Perform inventory */
    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return error;
}

ipj_error enable_external_antenna_mux()
{
    return ipj_set_value(
        &iri_device,
        E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_ENABLE,
        true
    );
}

ipj_error disable_external_antenna_mux()
{
    return ipj_set_value(
        &iri_device,
        E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_ENABLE,
        false
    );
}

ipj_error config_external_antenna_mux(
    unsigned int num_antennas,
    unsigned int physical_port,
    unsigned int delay_microseconds
)
{
    ipj_error error;

    error = ipj_set_value(&iri_device, E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_NUM_ANTENNAS, num_antennas);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_NUM_ANTENNAS");

    error = ipj_set_value(&iri_device, E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_PHYSICAL_PORT, physical_port);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value EXTERNAL_ANTENNA_MUX_PHYSICAL_PORT");

    error = ipj_set_value(&iri_device, E_IPJ_KEY_EXTERNAL_ANTENNA_MUX_DELAY_MICROSECONDS, delay_microseconds);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_set_value EXTERNAL_ANTENNA_MUX_DELAY_MICROSECONDS");

    return error;
}

ipj_error set_antenna_sequence()
{
    ipj_error error;
    ipj_key_value key_value[16];
    unsigned int  key_value_count = 0;
    ipj_key_list  key_list;
    unsigned int  key_list_count = 1;
    unsigned int  i;

    memset(&key_value, 0, sizeof(key_value));
    memset(&key_list, 0, sizeof(key_list));

    key_list_count = 1;
    key_list.key = E_IPJ_KEY_ANTENNA_SEQUENCE;
    key_list.list_count = NUM_EXTERNAL_MUX_ANTENNAS;

    for(i = 0; i < key_list.list_count; ++i)
    {
        key_list.list[i] = i + 1;
    }

    error = ipj_bulk_set(&iri_device, &key_value[0], key_value_count, &key_list, key_list_count);

    return error;
}