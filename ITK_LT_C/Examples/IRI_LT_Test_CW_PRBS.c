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
#include <string.h>
#include "ipj_util_lt.h"
#include "iri_lt.h"
#include "platform.h"

/* PURPOSE: This example illustrates the use of the test commands to control
   the CW (Continuous Wave) and PRBS (Pseudo-Random Bit Sequence) functions. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 10000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

static ipj_error test_configuration(
        ipj_iri_device* iri_device,
        ipj_test_id test_id,
        uint32_t param0,
        uint32_t param1,
        uint32_t param2,
        uint32_t param3)
{
    ipj_error error;

    error = E_IPJ_ERROR_SUCCESS;

    /* Configure test parameters */
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_ID, 0, 0, test_id);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 0, param0);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 1, param1);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 2, param2);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 3, param3);

    ipj_stopped_flag = 0;

    /* Activate test parameters */
    error = ipj_start(iri_device, E_IPJ_ACTION_TEST);

    /* Wait for Stop Response */
    while (!ipj_stopped_flag)
    {
        ipj_receive(iri_device);
    }

    /* Clear test parameters (prepare for next test parameter set) */
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_ID, 0, 0, E_IPJ_TEST_ID_NONE);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 0, 0);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 1, 0);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 2, 0);
    error = ipj_set(iri_device, E_IPJ_KEY_TEST_PARAMETERS, 0, 3, 0);

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

    /* Set frequency to 912.25 MHz (specified in kHz)*/
    test_configuration(&iri_device, E_IPJ_TEST_ID_SET_FREQUENCY, 912250, 0, 0, 0);

    /* Enable CW */
    test_configuration(&iri_device, E_IPJ_TEST_ID_CW_CONTROL, 1, 0, 0, 0);

    /* Leave CW on for 1 second */
    platform_sleep_ms_handler(1000);

    /* Disable CW */
    test_configuration(&iri_device, E_IPJ_TEST_ID_CW_CONTROL, 0, 0, 0, 0);

    /* Set frequency to 912.75 MHz (specified in kHz)*/
    test_configuration(&iri_device, E_IPJ_TEST_ID_SET_FREQUENCY, 912750, 0, 0, 0);

    /* Enable PRBS */
    test_configuration(&iri_device, E_IPJ_TEST_ID_PRBS_CONTROL, 1, 0, 0, 0);

    /* Leave PRBS on for 1 second */
    platform_sleep_ms_handler(1000);

    /* Disable PRBS */
    test_configuration(&iri_device, E_IPJ_TEST_ID_PRBS_CONTROL, 0, 0, 0, 0);

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
}
