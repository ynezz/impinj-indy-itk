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

static uint32_t ipj_stopped_flag;

static struct ipj_handler event_handlers[] =
{
    { E_IPJ_HANDLER_TYPE_PLATFORM_OPEN_PORT,        &platform_open_port_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_CLOSE_PORT,       &platform_close_port_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_TRANSMIT,         &platform_transmit_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_RECEIVE,          &platform_receive_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_TIMESTAMP,        &platform_timestamp_ms_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_SLEEP_MS,         &platform_sleep_ms_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_MODIFY_CONNECTION,&platform_modify_connection_handler },
    { E_IPJ_HANDLER_TYPE_PLATFORM_FLUSH_PORT,       &platform_flush_port_handler },
    { E_IPJ_HANDLER_TYPE_REPORT,                    &ipj_util_report_handler },
    { E_IPJ_HANDLER_TYPE_DIAGNOSTIC,                &ipj_util_diagnostic_handler }
};

ipj_error ipj_util_setup(ipj_iri_device* iri_device, char* reader_identifier)
{
    ipj_error error;

    /* Initialize iri_device structure */
    error = ipj_initialize_iri_device(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_initialize_iri_device");

    /* Register all handlers */
    error = ipj_util_register_handlers(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_register_handlers");

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

    /* Deinitialize IRI device */
    error = ipj_deinitialize_iri_device(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_deinitialize_iri_device");

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

ipj_error ipj_util_register_handlers(ipj_iri_device* iri_device)
{
    ipj_error error;
    unsigned int i;
    for (i = 0; i < (sizeof(event_handlers) / sizeof(event_handlers[0])); i++)
    {
        error = ipj_register_handler(
                iri_device,
                event_handlers[i].type,
                event_handlers[i].handler);
        if (error)
        {
            return error;
        }
    }
    return E_IPJ_ERROR_SUCCESS;
}

/*Code copied from IRI_Loader.c*/
ipj_error ipj_util_flash_image(const char * image_name, char * com_port, ipj_iri_device * iri_device)
{
    /* Define error code */
    ipj_error error;

    /* Storage buffer for device read */
    uint8_t file_buf[300];
    FILE* image_file_handle;
    int chunk_size;

    /*Setup Device*/
    error = ipj_util_setup(iri_device, com_port);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /* Put the device in bootloader mode */
    error = ipj_reset(iri_device, E_IPJ_RESET_TYPE_TO_BOOTLOADER);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_reset E_IPJ_RESET_TYPE_TO_BOOTLOADER");

#if defined(__GNUC__)
    image_file_handle = fopen(image_name, "rb");
#else
    fopen_s(&image_file_handle, image_name, "rb");
#endif

    if (image_file_handle == NULL)
    {
        printf("Unable to open image file\n");
        return -1;
    }

    /* Get the image chunk size.  This is stored in the first 32 bits
     * of the upgrade image */
    if (fread(file_buf, 4, 1, image_file_handle) == 0)
    {
        printf("Unable to determine chunk size\n");
        return -1;
    }

    chunk_size = (file_buf[0] & 0xff) | (file_buf[1] << 8) | (file_buf[2] << 16)
            | (file_buf[3] << 24);

    printf("Image chunk size: %d\n", chunk_size);
    printf(
            "(%d bytes header | %d bytes payload | 2 bytes CRC)\n",
            12,
            (chunk_size - 2) - 12);

    if (chunk_size < 22 || chunk_size > 270)
    {
        printf("Invalid chunk size\n");
        return -1;
    }

    /* For each chunk in the image file, write it to the device */
    while (fread(file_buf, chunk_size, 1, image_file_handle) > 0)
    {
        error = ipj_flash_handle_loader_block(iri_device, chunk_size, file_buf);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_flash_handle_loader_block");
    }

    /* Deregister Handlers, Cleanup IRI_Device and Reset */
    error = ipj_util_cleanup(iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return error;
}

/*Util function to wait for receives or until stop handler triggered*/
ipj_error ipj_util_wait_for_receive(ipj_iri_device * iri_device, uint32_t timeout_time_ms)
{
    ipj_error error;
    ipj_stopped_flag = 0;

    /*  Perform receive until end time reached or stop received */
    while (!ipj_stopped_flag && (platform_timestamp_ms_handler() < timeout_time_ms))
    {
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }
    return error;
}

ipj_error ipj_util_perform_inventory(ipj_iri_device* iri_device, uint32_t timeout_ms)
{
    uint32_t end_time_ms;
    ipj_error error;

    /* Clear the stopped flag */
    ipj_stopped_flag = 0;

    error = ipj_start(iri_device, E_IPJ_ACTION_INVENTORY);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_INVENTORY");

    /* Set end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /*  Perform receive until end time reached or stop received */
    while (!ipj_stopped_flag && platform_timestamp_ms_handler() < end_time_ms)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }

    /* Stop inventory if it is still running */
    if (!ipj_stopped_flag)
    {
        error = ipj_stop(iri_device, E_IPJ_ACTION_INVENTORY);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_stop");
    }

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

ipj_error ipj_util_test_command(ipj_iri_device* iri_device, ipj_test_id test_id,
                                uint32_t param0, uint32_t param1, uint32_t param2, uint32_t param3)
{
    ipj_error error;
    uint32_t end_time_ms;
    uint32_t timeout_ms = 5000;
    ipj_key_value key_value[16];
    uint32_t key_value_count;
    ipj_key_list key_list;
    uint32_t key_list_count;

    memset(key_value, 0, sizeof(key_value));
    memset(&key_list, 0, sizeof(key_list));

    key_value_count = 1;
    key_value[0].key = E_IPJ_KEY_TEST_ID;
    key_value[0].value = test_id;

    key_list_count = 1;
    key_list.list_count = 4;
    key_list.key = E_IPJ_KEY_TEST_PARAMETERS;
    key_list.list[0] = param0;
    key_list.list[1] = param1;
    key_list.list[2] = param2;
    key_list.list[3] = param3;

    error = ipj_bulk_set(iri_device, &key_value[0], key_value_count, &key_list, key_list_count);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_set");

    ipj_stopped_flag = 0;

    /* Activate test parameters */
    error = ipj_start(iri_device, E_IPJ_ACTION_TEST);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_start E_IPJ_ACTION_TEST");

    /* Set end time */
    end_time_ms = platform_timestamp_ms_handler() + timeout_ms;

    /* Clear the stopped flag */
    ipj_stopped_flag = 0;

    /*  Perform receive until end time reached or stop received */
    while (!ipj_stopped_flag && platform_timestamp_ms_handler() < end_time_ms)
    {
        /* Call ipj_receive to process tag reports  */
        error = ipj_receive(iri_device);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_receive");
    }

    key_value_count = 1;
    key_value[0].key = E_IPJ_KEY_TEST_ID;
    key_value[0].value = 0;

    key_list_count = 1;
    key_list.list_count = 4;
    key_list.key = E_IPJ_KEY_TEST_PARAMETERS;
    key_list.list[0] = 0;
    key_list.list[1] = 0;
    key_list.list[2] = 0;
    key_list.list[3] = 0;

    error = ipj_bulk_set(iri_device, &key_value[0], key_value_count, &key_list, key_list_count);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_bulk_set");

    return error;
}


/* Report handler processes asynchronous reports */
ipj_error ipj_util_report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        void* report)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    /* Case statement for each report type */
    switch (report_id)
    {
        case E_IPJ_REPORT_ID_TAG_OPERATION_REPORT:
            error = ipj_util_tag_operation_report_handler(
                    iri_device,
                    (ipj_tag_operation_report*) report);
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
        ipj_tag_operation_report* tag_operation_report)
{
    uint32_t i;

    if (tag_operation_report->has_error && tag_operation_report->error > 0)
    {
        IPJ_UTIL_PRINT_ERROR(tag_operation_report->error,"tag_operation_report");
    }

    /* If tag report has epc, print epc */
    if (tag_operation_report->tag.has_epc)
    {
        /* Print reader identifier */
        printf("%s: EPC = ", (char*) iri_device->reader_identifier);

        ipj_util_print_epc(
                (uint16_t*) tag_operation_report->tag.epc.bytes,
                (int) tag_operation_report->tag.epc.size / 2,
                true);
    }
	
    /* If tag report has antenna, print antenna */
	if (tag_operation_report->tag.has_antenna)
	{
		/* Print antenna */
		printf("%s: Antenna = ", (char*)iri_device->reader_identifier);

		printf("%d\r\n", tag_operation_report->tag.antenna);
	}

    if (tag_operation_report->has_diagnostic)
    {
        printf("Diagnostic = 0x%08X\n", tag_operation_report->diagnostic);
    }

    if (tag_operation_report->has_tag_operation_type)
    {
        printf("Tag operation: ");
        switch (tag_operation_report->tag_operation_type)
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
                printf("TYPE=%d\n", tag_operation_report->tag_operation_type);
            }
        }

        if (tag_operation_report->has_tag_operation_data)
        {
            printf(
                    "Report contains data: %d bytes\n",
                    (int) tag_operation_report->tag_operation_data.size);
            for (i = 0; i < tag_operation_report->tag_operation_data.size; i += 2)
            {
                if (i % (2 * 6) == 0 && i > 0)
                {
                    printf("\n");
                }
                printf(
                        "0x%04x|",
                        tag_operation_report->tag_operation_data.bytes[i] << 8
                                | tag_operation_report->tag_operation_data.bytes[i
                                        + 1]);
            }
        }
        printf("\n");

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
    for (i = 1; i < gpio_report->gpio_modes_count; i++)
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

        if (i < gpio_report->gpio_modes_count - 1)
            printf("|");
    }
    printf("\nGPIO States: ");

    for (i = 1; i < gpio_report->gpio_states_count; i++)
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

        if (i < gpio_report->gpio_states_count - 1)
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
    for (i = 0; i < status_report->data_count; i++)
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

    for (i = 0; i < test_report->data_count; i++)
    {
        printf("[%d] 0x%X, %d\n", i, test_report->data[i], test_report->data[i]);
    }

    return E_IPJ_ERROR_SUCCESS;
}

void ipj_util_diagnostic_handler(ipj_iri_device* iri_device, ipj_error error)
{
    switch (error)
    {
        case E_IPJ_ERROR_IRI_FRAME_DROPPED:
        {
            printf("%s Dropped IRI Frame!\n", (char*) iri_device->reader_identifier);
            break;
        }
        default:
        {
            break;
        }
    }
}
