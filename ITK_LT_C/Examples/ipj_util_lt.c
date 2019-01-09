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

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

uint32_t ipj_stopped_flag;

ipj_error ipj_util_setup(ipj_iri_device* iri_device, char* reader_identifier)
{
    ipj_error error;

    /* Initialize iri_device structure */
    error = ipj_initialize_iri_device(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_initialize_iri_device");

    /* Connect to iri device - open serial port */
    error = ipj_connect(
            iri_device,
            reader_identifier,
            E_IPJ_CONNECTION_TYPE_SERIAL,
            NULL);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_connect");

    return error;
}

ipj_error ipj_util_cleanup(ipj_iri_device* iri_device)
{
    ipj_error error;

    /* Reset the IRI device */
    error = ipj_reset(iri_device, E_IPJ_RESET_TYPE_SOFT);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_reset E_IPJ_RESET_TYPE_SOFT");

    /* Disconnect IRI device & close serial port */
    error = ipj_disconnect(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_disconnect");

    return error;
}

void ipj_util_print_epc(uint16_t* epc, int len, bool little_endian)
{
    int i;
    for (i = 0; i < len; i++)
    {
        /* Print hyphen every two bytes */
        if (i % 1 == 0 && i > 0)
        {
            printf("-");
        }
        /* Print one byte at a time */
        if (little_endian)
        {
            printf("%04X", (((epc[i] & 0xFF00) >> 8) | ((epc[i] & 0xFF) << 8)));
        }
        else
        {
            printf("%04X", epc[i]);
        }
    }
    printf("\n");
}

void ipj_util_print_divider(char symbol, int width)
{
    int i;
    printf("\n");
    for (i = 0; i < width; i++)
    {
        printf("%c", symbol);
    }
    printf("\n");
}

ipj_error ipj_util_perform_inventory(ipj_iri_device* iri_device, uint32_t timeout_ms)
{
    uint32_t end_time_ms;
    ipj_error error;

    error = ipj_start(iri_device, E_IPJ_ACTION_INVENTORY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_INVENTORY");

    /* Set example end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /* Clear the stopped flag */
    ipj_stopped_flag = 0;

    /*  Run inventory until end time reached */
    while (platform_timestamp_ms_handler() < end_time_ms)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }

    /* Stop inventory */
    error = ipj_stop(iri_device, E_IPJ_ACTION_INVENTORY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_stop");

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

/* Report handler processes asynchronous reports */
ipj_error ipj_report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        uint32_t report_count_32,
        void* report)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    /* Case statement for each report type */
    switch (report_id)
    {
        case E_IPJ_REPORT_ID_TAG_OPERATION_REPORT:
            error = ipj_util_tag_operation_report_handler(
                    iri_device,
                    report_count_32,
                    (uint32_t*) report);
            break;
        case E_IPJ_REPORT_ID_STOP_REPORT:
            error = ipj_util_stop_report_handler(
                    iri_device,
                    (ipj_stop_report*) report);
            break;
        case E_IPJ_REPORT_ID_GPIO_REPORT:
            error = ipj_util_gpio_report_handler(
                    iri_device,
                    (ipj_gpio_report*) report);
            break;
        case E_IPJ_REPORT_ID_ERROR_REPORT:
            error = ipj_util_error_report_handler(
                    iri_device,
                    (ipj_error_report*) report);
            break;
        case E_IPJ_REPORT_ID_STATUS_REPORT:
            error = ipj_util_status_report_handler(
                    iri_device,
                    (ipj_status_report*) report);
            break;
        case E_IPJ_REPORT_ID_TEST_REPORT:
            error = ipj_util_test_report_handler(
                    iri_device,
                    (ipj_test_report*) report);
            break;
        default:
            printf(
                    "%s: REPORT ID: %d NOT HANDLED\n",
                    (char*) iri_device->reader_identifier,
                    report_id);
            error = E_IPJ_ERROR_GENERAL_ERROR;
            break;
    }
    return error;
}

/* Tag report handler processes asynchronous reports */
ipj_error ipj_util_tag_operation_report_handler(
        ipj_iri_device* iri_device,
        uint32_t report_count_32,
        uint32_t* tag_operation_report)
{
    uint32_t index = 0;
    while (index < report_count_32)
    {
        uint32_t field_header = tag_operation_report[index++];
        switch (REPORT_FIELD_ID(field_header))
        {
            case E_IPJ_TAG_OPERATION_REPORT_ERROR:
            {
                IPJ_UTIL_PRINT_ERROR(tag_operation_report[index], "tag_operation_report");
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_PC:
            {
                printf("PC: %04X\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_XPC:
            {
                printf("XPC: %04X\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_CRC:
            {
                printf("CRC: %04X\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_RSSI:
            {
                printf("RSSI: %d\n", (int)tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_PHASE:
            {
                printf("PHASE: %d\n", (int)tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_CHANNEL:
            {
                printf("CHANNEL: %d\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_ANTENNA:
            {
                printf("ANTENNA: %d\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_TAG_OPERATION_TYPE:
            {
                printf("Tag operation: ");
                switch (tag_operation_report[index])
                {
                    case E_IPJ_TAG_OPERATION_TYPE_READ:
                    {
                        printf("READ\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_WRITE:
                    {
                        printf("WRITE\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_LOCK:
                    {
                        printf("LOCK\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_KILL:
                    {
                        printf("KILL\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_BLOCKPERMALOCK:
                    {
                        printf("BLOCK PERMALOCK\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_WRITE_EPC:
                    {
                        printf("WRITE EPC\n");
                        break;
                    }
                    case E_IPJ_TAG_OPERATION_TYPE_QT:
                    {
                        printf("QT\n");
                        break;
                    }
                    default:
                    {
                        printf("TYPE=%d\n", tag_operation_report[index]);
                    }
                }
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_RETRIES:
            {
                printf("RETRIES: %d\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_DIAGNOSTIC:
            {
                printf("DIAGNOSTIC: %X\n", tag_operation_report[index]);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_TIMESTAMP:
            {
                printf(
                        "TIMESTAMP: %I64d\n",
                        (uint64_t) tag_operation_report[index]
                                | ((uint64_t) (tag_operation_report[index + 1]) << 32));
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_EPC:
            {
                printf("EPC: ");
                ipj_util_print_epc(
                        (uint16_t*) &tag_operation_report[index],
                        REPORT_FIELD_LENGTH(field_header) / sizeof(uint16_t),
                        true);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_TID:
            {
                printf("TID: ");
                /* Reuse the print epc method */
                ipj_util_print_epc(
                        (uint16_t*) &tag_operation_report[index],
                        REPORT_FIELD_LENGTH(field_header) / sizeof(uint16_t),
                        true);
                break;
            }
            case E_IPJ_TAG_OPERATION_REPORT_TAG_OPERATION_DATA:
            {
                printf("Report contains data: %d bytes\n", REPORT_FIELD_LENGTH(field_header));
                /* Reuse the print epc method */
                ipj_util_print_epc(
                        (uint16_t*) &tag_operation_report[index],
                        REPORT_FIELD_LENGTH(field_header) / sizeof(uint16_t),
                        true);
                break;
            }
        }

        index += ROUNDED_COUNT_32(REPORT_FIELD_LENGTH(field_header));

    }

    ipj_util_print_divider('-', 80);
    return E_IPJ_ERROR_SUCCESS;
}

/* Stop report handler processes asynchronous reports */
ipj_error ipj_util_stop_report_handler(
        ipj_iri_device* iri_device,
        ipj_stop_report* ipj_stop_report)
{
    if (ipj_stop_report->error == E_IPJ_ERROR_SUCCESS)
    {
        /* Print reader identifier */
        printf("%s: STOPPED\n", (char*) iri_device->reader_identifier);

        /* Set the stopped flag, the stop report does not have any fields that
         * need to be checked */
        ipj_stopped_flag = 1;
    }
    else
    {
        IPJ_UTIL_PRINT_ERROR(ipj_stop_report->error,"stop_report");
    }
    return ipj_stop_report->error;
}

/* GPIO handler processes asynchronous GPIO event reports */
ipj_error ipj_util_gpio_report_handler(
        ipj_iri_device* iri_device,
        ipj_gpio_report* gpio_report)
{
    unsigned int i;
    for (i = 0; i < 40; i++)
        printf("*");

    printf("\n%s: GPIO Report\n", (char*) iri_device->reader_identifier);

    printf("GPIO Modes:  ");
    for (i = 1; i < ARRAY_SIZE(gpio_report->gpio_modes); i++)
    {
        switch (gpio_report->gpio_modes[i])
        {
            case E_IPJ_GPIO_MODE_DISABLED:
            {
                printf("D  ");
                break;
            }
            case E_IPJ_GPIO_MODE_INPUT:
            {
                printf("I  ");
                break;
            }
            case E_IPJ_GPIO_MODE_OUTPUT:
            {
                printf("O  ");
                break;
            }
            case E_IPJ_GPIO_MODE_OUTPUT_PULSE:
            {
                printf("OP ");
                break;
            }
            case E_IPJ_GPIO_MODE_INPUT_ACTION:
            {
                printf("IA ");
                break;
            }
            case E_IPJ_GPIO_MODE_OUTPUT_ACTION:
            {
                printf("OA ");
                break;
            }
            case E_IPJ_GPIO_MODE_OUTPUT_PULSE_ACTION:
            {
                printf("OPA");
                break;
            }
        }

        if (i < ARRAY_SIZE(gpio_report->gpio_modes) - 1)
            printf("|");
    }
    printf("\nGPIO States: ");

    for (i = 1; i < ARRAY_SIZE(gpio_report->gpio_states); i++)
    {
        switch (gpio_report->gpio_states[i])
        {
            case E_IPJ_GPIO_STATE_LO:
            case E_IPJ_GPIO_STATE_FLOAT:
            {
                printf("0  ");
                break;
            }
            case E_IPJ_GPIO_STATE_HI:
            {
                printf("1  ");
                break;
            }
        }

        if (i < ARRAY_SIZE(gpio_report->gpio_states) - 1)
            printf("|");
    }
    printf("\n");

    for (i = 0; i < 40; i++)
        printf("*");

    printf("\n");

    return E_IPJ_ERROR_SUCCESS;
}

ipj_error ipj_util_error_report_handler(
        ipj_iri_device* iri_device,
        ipj_error_report* error_report)
{
    printf("Error Report\n");
    printf("Error:  0x%X, %d\n", error_report->error, error_report->error);
    printf("Param1: 0x%X, %d\n", error_report->param1, error_report->param1);
    printf("Param2: 0x%X, %d\n", error_report->param2, error_report->param2);
    printf("Param3: 0x%X, %d\n", error_report->param3, error_report->param3);
    printf("Param4: 0x%X, %d\n", error_report->param4, error_report->param4);
    return E_IPJ_ERROR_SUCCESS;
}

ipj_error ipj_util_status_report_handler(
        ipj_iri_device* iri_device,
        ipj_status_report* status_report)
{
    uint32_t i;
    printf("Status Report\n");
    printf("status_flag:  0x%X, %d\n", status_report->status_flag, status_report->status_flag);
    printf("status_1: 0x%X, %d\n",     status_report->status_1, status_report->status_1);
    printf("status_2: 0x%X, %d\n",     status_report->status_2, status_report->status_2);
    printf("status_3: 0x%X, %d\n",     status_report->status_3, status_report->status_3);

    printf("Additional Data:\n");
    for (i = 0; i < ARRAY_SIZE(status_report->data); i++)
    {
        printf("[%d] 0x%X, %d\n", i, status_report->data[i], status_report->data[i]);
    }

    return E_IPJ_ERROR_SUCCESS;
}

ipj_error ipj_util_test_report_handler(
        ipj_iri_device* iri_device,
        ipj_test_report* test_report)
{
    uint32_t i;
    if (test_report->error != E_IPJ_ERROR_SUCCESS)
    {
        IPJ_UTIL_PRINT_ERROR(test_report->error, "test");
    }

    printf("\n*** %s Test Report ***\n", (char*) iri_device->reader_identifier);
    printf("Test ID: %d\n", test_report->test_id);
    printf("Test Results (optional):\n");
    printf("Result 1: 0x%X, %d\n", test_report->result_1, test_report->result_1);
    printf("Result 2: 0x%X, %d\n", test_report->result_2, test_report->result_2);
    printf("Result 3: 0x%X, %d\n", test_report->result_3, test_report->result_3);
    printf("Additional Data:\n");

    for (i = 0; i < ARRAY_SIZE(test_report->data); i++)
    {
        printf("[%d] 0x%X, %d\n", i, test_report->data[i], test_report->data[i]);
    }

    return E_IPJ_ERROR_SUCCESS;
}
