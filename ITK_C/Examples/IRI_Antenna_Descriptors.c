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

/* PURPOSE: This file is used to demo the antenna descriptors feature, 
   using per antenna transmit power as an example. Each logical antenna has
   its own unique transmit power value, and these logical antennas can
   correspond to either physical antennas on the device, or antennas switched
   to using the external antenna mux feature. */

#define ANTENNA_SEQUENCE_OPTION_PHYSICAL 0
#define ANTENNA_SEQUENCE_OPTION_LOGICAL  1
#define NUM_ANTENNA_DESCRIPTORS          16
#define KVP_PER_ANTENNA_DESCRIPTOR       4

#define IPJ_EXAMPLE_DURATION_MS 10000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

/* Each key-value pair consists of 32 bits to specify the key, bank index,
   and value index, followed by 32 bits that specify the value to be set. */
typedef struct
{
    uint16_t key;
    uint8_t bank_index;
    uint8_t value_index;

    uint32_t value;

} antenna_descriptor_kvp;

antenna_descriptor_kvp antenna_descriptors[NUM_ANTENNA_DESCRIPTORS][KVP_PER_ANTENNA_DESCRIPTOR] =
{
    /* Antenna Descriptor 1 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1000},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 2 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1100},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 3 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1200},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 4 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1300},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 5 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1400},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 6 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1500},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* Antenna Descriptor 7 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1600},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 8 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1700},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 9 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1800},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 10 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 1900},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 11 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2000},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 12 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2100},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 13 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2150},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 14 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2200},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* Antenna Descriptor 15 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2250},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    /* Antenna Descriptor 16 */
    {
        {E_IPJ_KEY_ANTENNA_PHYSICAL_PORT, 0, 0, 1},
        {E_IPJ_KEY_ANTENNA_TX_POWER, 0, 0, 2300},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
};

ipj_error set_logical_antenna_sequence_option();
ipj_error set_antenna_descriptors();
uint32_t get_key_bank_index_value_index_encoding(uint16_t key, uint8_t bank_index, uint8_t value_index);
ipj_error set_antenna_sequence();


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

    error = set_logical_antenna_sequence_option();
    IPJ_UTIL_RETURN_ON_ERROR(error, "set_logical_antenna_sequence_option");

    error = set_antenna_descriptors();
    IPJ_UTIL_RETURN_ON_ERROR(error, "set_antenna_descriptors");

    error = ipj_util_perform_inventory(&iri_device, IPJ_EXAMPLE_DURATION_MS);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_perform_inventory");

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return error;
}

/* Setting the antenna sequence option to logical allows the antenna sequence
   key to be filled with logical antenna indexes, which do not have to
   correspond to physical antennas on the device. */
ipj_error set_logical_antenna_sequence_option()
{
    return ipj_set_value(
        &iri_device,
        E_IPJ_KEY_ANTENNA_SEQUENCE_OPTION,
        ANTENNA_SEQUENCE_OPTION_LOGICAL
    );
}

/* The antenna descriptor key consists of 16 banks of
   four key-value pairs. Each key value pair can be used
   to set the value of one other key when the associated
   antenna is to be switched to. */
ipj_error set_antenna_descriptors()
{
    ipj_error error;
    ipj_key_value key_value[16];
    unsigned int  key_value_count = 0;
    ipj_key_list  key_list;
    unsigned int  key_list_count = 1;
    unsigned int  i, j;

    memset(&key_value, 0, sizeof(key_value));
    memset(&key_list, 0, sizeof(key_list));

    key_list.has_key = true;
    key_list.key = E_IPJ_KEY_ANTENNA_DESCRIPTORS;

    key_list.list_count = 2 * KVP_PER_ANTENNA_DESCRIPTOR;

    key_list.has_bank_index = true;

    for(i = 0; i < NUM_ANTENNA_DESCRIPTORS; ++i)
    {
        key_list.bank_index = i;

        for(j = 0; j < KVP_PER_ANTENNA_DESCRIPTOR; ++j)
        {
            antenna_descriptor_kvp *kvp = &(antenna_descriptors[i][j]);

            key_list.list[(2*j)] = get_key_bank_index_value_index_encoding(
                kvp->key,
                kvp->bank_index,
                kvp->value_index
            );

            key_list.list[(2*j) + 1] = kvp->value;
        }

        error = ipj_bulk_set(&iri_device, &key_value[0], key_value_count, &key_list, key_list_count);

        if(error != E_IPJ_ERROR_SUCCESS)
        {
            break;
        }
    }

    return error;
}

/* Returns a 32 bit integer that contains the information specifying the key,
   bank index, and value index to set the value of. */
uint32_t get_key_bank_index_value_index_encoding(uint16_t key, uint8_t bank_index, uint8_t value_index)
{
    uint32_t result = key | ((bank_index & 0xff) << 16) | ((value_index & 0xff) << 24);

    return result;
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
    key_list.list_count = NUM_ANTENNA_DESCRIPTORS;

    for(i = 0; i < key_list.list_count; ++i)
    {
        key_list.list[i] = i + 1;
    }

    error = ipj_bulk_set(&iri_device, &key_value[0], key_value_count, &key_list, key_list_count);

    return error;
}