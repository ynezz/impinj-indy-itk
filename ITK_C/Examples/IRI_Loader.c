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
#if defined(__GNUC__)
    #include <unistd.h>
#else
    #include <io.h>
#endif
#include "ipj_util.h"
#include "iri.h"

/* PURPOSE: This example illustrates the use of the image loader to download
   firmware to the device. */

/* Parameters */
#define IPJ_EXAMPLE_DURATION_MS 1000

/* Allocate memory for iri device */
static ipj_iri_device iri_device = { 0 };

/* Main */
int main(int argc, char* argv[])
{
    /* Define error code */
    ipj_error error;

    /* Storage buffer for device read */
    uint8_t file_buf[300];
    FILE* image_file_handle;

    int chunk_size;

    /* connection parameters */
    ipj_connection_params params;

    /* High speed flag */
    bool high_speed = false;

    if ((argc < 3) || (argc > 4))
    {
        printf("\n\nUsage:"
                "\n\tIRI_Loader.exe COMx <image_location> [-s] \n"
                "where: \n"
                "\tx is a COM port number\n"
                "\t<image_location> is an absolute path\n"
                "\t-s for high speed 921600 baud update (optional)\n");
        return -1;
    }

    /* Determine if we're in high speed mode or not */
    if ((argc == 4) && (strcmp(argv[3], "-s") == 0))
    {
        high_speed = true;
    }

    /* Common example setup */
    error = ipj_util_setup(&iri_device, argv[1]);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_setup");

    /* Put the device in bootloader mode */
    error = ipj_reset(&iri_device, E_IPJ_RESET_TYPE_TO_BOOTLOADER);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_reset E_IPJ_RESET_TYPE_TO_BOOTLOADER");

    if (high_speed)
    {
        /* Speed up the upgrade process */

        /* Perform a modify connection command to bring the device
         * and host up to 921600 baud */
        params.serial.baudrate = E_IPJ_BAUD_RATE_BR921600;
        params.serial.parity = E_IPJ_PARITY_PNONE;

        error = ipj_modify_connection(&iri_device,
        E_IPJ_CONNECTION_TYPE_SERIAL, &params);
        if (error)
        {
            printf("Unable to change baud rate");
            return -1;
        }
    }

#if defined(__GNUC__)
    image_file_handle = fopen(argv[2], "rb");
#else
    fopen_s(&image_file_handle, argv[2], "rb");
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

    /* For each chunk in the image file, write it to the Indy Module */
    while (fread(file_buf, chunk_size, 1, image_file_handle) > 0)
    {
        error = ipj_flash_handle_loader_block(&iri_device, chunk_size, file_buf);
        IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_flash_handle_loader_block");
    }

    printf("Image load complete\n");

    if (high_speed)
    {
        /* Return to our previous baud rate */
        params.serial.baudrate = E_IPJ_BAUD_RATE_BR115200;
        error = ipj_modify_connection(&iri_device,
        E_IPJ_CONNECTION_TYPE_SERIAL, &params);
        if (error)
        {
            printf("Unable to change baud rate");
            return -1;
        }
    }

    /* Common example cleanup */
    error = ipj_util_cleanup(&iri_device);
    IPJ_UTIL_RETURN_ON_ERROR(error, "ipj_util_cleanup");

    return 0;
}
