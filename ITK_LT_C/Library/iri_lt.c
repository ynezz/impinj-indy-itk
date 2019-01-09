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

#include <string.h>
#include <stdio.h>

#include <version.h>
#include <platform.h>
#include "iri_lt.h"

#define IRI_SOFT_RESET_DELAY_MS           300
#define IPJ_MARKER                        0x8D706A21
#define IPJ_HDR_VERSION                   2
#define LENGTH_DELIMITED_FIELD(x)         (((x << 3) | 2) & 0xFF)
#define FIELD_ID(tag)                     ((tag >> 3) & 0xFF)
#define IPJ_FLAG_SUPPRESS_SET_RESPONSES   (1 << 2)

/* Packet field ids */
#define FIELD_ID_COMMAND                  3
#define FIELD_ID_RESPONSE                 4
#define FIELD_ID_REPORT                   5

/* Field ids */
#define FIELD_ID_BULK_SET                 5
#define FIELD_ID_BULK_GET                 6
#define FIELD_ID_START                    7
#define FIELD_ID_STOP                     8
#define FIELD_ID_RESUME                   9
#define FIELD_ID_GET_INFO                 13
#define FIELD_ID_RESET                    14
#define FIELD_ID_INVALID                  15

/* All LT buffer submessages use the same field id */
#define FIELD_ID_LT_BUFFER                15

#if defined(ENABLE_FW_UPDATES)
#define IPJ_GOOD_IRI_RESET_RESPONSE          0x00720222
#define IPJ_FLASH_RESPONSE_SIZE              19
#define IPJ_FLASH_RESPONSE_PAYLOAD_SIZE      7
#define IPJ_FLASH_WRITE_OFFSET_INDEX         20
#define IPJ_FLASH_ERASE_OFFSET_INDEX         18
#define IPJ_FLASH_ERASE_LENGTH_INDEX         23
#define IPJ_SUPPORTED_CHUNK_SIZE             128
#define IPJ_SUPPORTED_APP_ERASE_LENGTH       0x1B000
#define IPJ_SUPPORTED_SETTINGS_ERASE_LENGTH  0x800
#define IPJ_VARINT_5_BYTE_ENCODED_MIN        0x10000000
#define IPJ_VARINT_4_BYTE_ENCODED_MIN        0x200000
#define IPJ_VARINT_3_BYTE_ENCODED_MIN        0x4000
#define IPJ_VARINT_2_BYTE_ENCODED_MIN        0x80

#endif


enum RX_FRAME_SYNC_STATE
{
    E_SEEKING_FRAME_SYNC,
    E_GOT_FRAME_SYNC
};

#if defined(_WIN32)
#pragma pack(1)
#endif

typedef struct IPJ_ATTRIB_PACKED _FrameHeader
{
    uint32_t    nMarker;
    uint8_t     nVersion;
    uint8_t     nFlags;
    uint16_t    nDataLength;
    uint8_t     nFrameSyncCount;
    uint8_t     nChecksum;
} FrameHeader;

typedef struct IPJ_ATTRIB_PACKED _PacketHeader
{
    uint8_t pkt_tag;       /* Command, Response, or Report id */
    uint8_t pkt_length;    /* Remaining length from here      */
    uint8_t sub_tag;       /* Set, Get, Start, Stop, etc. id  */
    uint8_t sub_length;    /* Remaining length from here      */
    uint8_t lt_buf_tag;    /* lt_buffer id                    */
    uint8_t lt_buf_length; /* Remaining length from here      */
} PacketHeader;

typedef struct IPJ_ATTRIB_PACKED _PacketCrc
{
    uint16_t crc;
} PacketCrc;

/* All responses begin with an error code */
typedef struct IPJ_ATTRIB_PACKED _ipj_generic_response
{
    uint32_t error;
} ipj_generic_response;

// ---------------------------------------------------------------------------
// *****************************************************************************
// IRI API - Command/Response
// *****************************************************************************
typedef struct IPJ_ATTRIB_PACKED _ipj_reset_command
{
    uint32_t reset_type;
} ipj_reset_command;

typedef struct IPJ_ATTRIB_PACKED _ipj_reset_response
{
    uint32_t error;
} ipj_reset_response;

typedef struct IPJ_ATTRIB_PACKED _ipj_bulk_set_command
{
    uint32_t key;
    uint32_t value;
    uint32_t bank_index;
    uint32_t value_index;
} ipj_bulk_set_command;

typedef struct IPJ_ATTRIB_PACKED _ipj_bulk_set_response
{
    uint32_t error;
} ipj_bulk_set_response;

typedef struct IPJ_ATTRIB_PACKED _ipj_bulk_get_command
{
    uint32_t key;
    uint32_t bank_index;
    uint32_t value_index;
} ipj_bulk_get_command;

typedef struct IPJ_ATTRIB_PACKED _ipj_bulk_get_response
{
    uint32_t error;
    uint32_t value;
} ipj_bulk_get_response;

typedef struct IPJ_ATTRIB_PACKED _ipj_start_command
{
    uint32_t action;
} ipj_start_command;

typedef struct IPJ_ATTRIB_PACKED _ipj_start_response
{
    uint32_t error;
} ipj_start_response;

typedef struct IPJ_ATTRIB_PACKED _ipj_stop_command
{
    uint32_t action;
} ipj_stop_command;

typedef struct IPJ_ATTRIB_PACKED _ipj_stop_response
{
    uint32_t error;
} ipj_stop_response;

typedef struct IPJ_ATTRIB_PACKED _ipj_invalid_response
{
    uint32_t error;
} ipj_invalid_response;


#if defined(_WIN32)
#pragma pack()
#endif

typedef union _ipj_rsp_union
{
    ipj_generic_response  generic_response;
    ipj_reset_response reset_response;
    ipj_bulk_set_response bulk_set_response;
    ipj_bulk_get_response bulk_get_response;
    ipj_start_response start_response;
    ipj_stop_response stop_response;
    ipj_invalid_response invalid_response;
} ipj_rsp_union;



typedef ipj_error (*ipj_internal_completion_handler)(ipj_rsp_union *rsp_union, void* args);

static uint8_t ipj_internal_calculate_parity_8(uint8_t* buf,int32_t len);
static bool    ipj_validate_frame_header(FrameHeader * frame_header);
static bool    ipj_validate_packet_header(PacketHeader * packet_header);

#if !defined(IRI_RX_ONLY)
/* Internal prototype declarations */
static ipj_error ipj_internal_transmit_packet(
        ipj_iri_device* iri_device,
        uint8_t command_id,
        uint8_t command_size,
        void* cmd);
static ipj_error ipj_internal_transact(
        ipj_iri_device* iri_device,
        uint8_t command_id,
        uint8_t command_size,
        void* command,
        void* handler_args,
        ipj_internal_completion_handler handler);
static ipj_error ipj_bulk_get_handler(ipj_rsp_union *rsp_union, void* args);
static ipj_error ipj_reset_handler(ipj_rsp_union *rsp_union, void* args);
#endif

// Reverse the endianess of 16 and 32-bit values
uint16_t ipj_swap_16(uint16_t nValue)
{
    return (nValue << 8) | (nValue >> 8 );
}

uint32_t ipj_swap_32(uint32_t nValue)
{
    nValue = ((nValue << 8) & 0xFF00FF00 ) | ((nValue >> 8) & 0xFF00FF );
    return (nValue << 16) | (nValue >> 16);
}

/**
 * This function returns the API version.
 *
 * \return uint32_t
 */
uint32_t  ipj_get_api_version()
{
    return IPJ_VERSION;
}

/**
 * This function initializes the IRI device data structure.
 * Before using an IRI device, the User application must do the following:
 * allocate or declare IRI device data structure,
 * initialize IRI device data structure (this function),
 * register platform handlers,
 * and connect to the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_initialize_iri_device(
        ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    if (!iri_device)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /* Clear IRI device structure */
    memset(iri_device, 0, sizeof(ipj_iri_device));

    iri_device->receive_timeout_ms = IPJ_DEFAULT_RECEIVE_TIMEOUT_MS;
    iri_device->frame_length       = IPJ_FRAME_HEADER_SIZE;

    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function opens the serial port and connects the Host to the IRI device.
 * Before using an IRI device, the User application must do the following:
 * allocate or declare IRI device data structure,
 * initialize IRI device data structure,
 * register platform handlers,
 * and connect to the IRI device (this function).
 *
 * \return ipj_error
 */
ipj_error ipj_connect(
        ipj_iri_device        *iri_device,        /**< [in] IRI device data structure */
        IPJ_READER_IDENTIFIER  reader_identifier, /**< [in] Identifier associated with IRI device */
        ipj_connection_type    connection_type,   /**< [in] Type of connection (Serial/TCP/etc)*/
        ipj_connection_params *params)            /**< [in] Connection parameters (baudrate/etc) (NULL==default) */
{
    uint32_t result;
    IPJ_READER_CONTEXT reader_context;
    ipj_connection_params default_params;

    /* Set up default parameters */
    default_params.serial.baudrate = E_IPJ_BAUD_RATE_BR115200;
    default_params.serial.parity = E_IPJ_PARITY_PNONE;

    /* If params is NULL, set it to default params */
    if (!params)
    {
        params = &default_params;
    }

    /* Open serial port */
    result = platform_open_port_handler(
            &reader_context,
            reader_identifier,
            connection_type,
            params);

    if (!result)
    {
        /* Return error if unsuccessful opening serial port */
        return E_IPJ_ERROR_API_SERIAL_PORT_ERROR;
    }

    /* Store reader_context and reader_identifier */
    iri_device->reader_context = reader_context;
    iri_device->reader_identifier = reader_identifier;

    /* Return success */
    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function disconnects the Host from the IRI device and closes the serial port.
 * User may disconnect from the IRI device after all operations are completed.
 *
 * \return ipj_error
 */
ipj_error ipj_disconnect(
        ipj_iri_device* iri_device)     /**< [in] IRI device data structure */
{
    uint32_t result;

    /* Close serial port */
    result = platform_close_port_handler(iri_device->reader_context);

    /* Return error if unsuccessful closing serial port */
    if (!result)
    {
        return E_IPJ_ERROR_API_SERIAL_PORT_ERROR;
    }
    else
    {
        return E_IPJ_ERROR_SUCCESS;
    }
}

#if !defined(IRI_RX_ONLY)
/**
 * This function starts the specified action or operation on the IRI device.
 * This function is used to start inventory to read tags.
 *
 * \return ipj_error
 */
ipj_error ipj_start(ipj_iri_device* iri_device, /**< [in] IRI device data structure      */
                    ipj_action      action)     /**< [in] Actions or operations to start */
{
    ipj_start_command command;
    command.action = action;

    return ipj_internal_transact(
            iri_device,
            FIELD_ID_START,
            sizeof(command),
            &command,
            NULL,
            NULL);
}

/**
 * This function stops the specified action or operation on the IRI device.
 * This function is used to stop inventory to read tags.
 *
 * \return ipj_error
 */
ipj_error ipj_stop(
        ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
        ipj_action action)              /**< [in] Actions or operations to be stopped */
{
    ipj_stop_command command;
    command.action = action;

    return ipj_internal_transact(
            iri_device,
            FIELD_ID_STOP,
            sizeof(command),
            &command,
            NULL,
            NULL);
}

static ipj_error ipj_bulk_get_handler(ipj_rsp_union *rsp_union, void* args)
{
    uint32_t * value = (uint32_t *) (args);

    /* Copy in return values */
    *value = rsp_union->bulk_get_response.value;

    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function retrieves the value of the specified key code from the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_get(
        ipj_iri_device *iri_device,    /**< [in] IRI device data structure */
        ipj_key         key,           /**< [in] Key code to get */
        uint32_t        bank_index,    /**< [in] Key code bank_index to get (applicable if more than one copy of a key) */
        uint32_t        value_index,   /**< [in] Key code value_index to get (applicable for key code with lists) */
        uint32_t       *value)         /**< [out] Data buffer to store retrieved value */
{
    ipj_bulk_get_command command;
    command.key = key;
    command.bank_index = bank_index;
    command.value_index = value_index;

    if (!value)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    return ipj_internal_transact(
            iri_device,
            FIELD_ID_BULK_GET,
            sizeof(command),
            &command,
            value,
            &ipj_bulk_get_handler);
}

/**
 * This function retrieves the value of the specified key code from
 * the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_get_value(
    ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
    ipj_key key,                    /**< [in] Key code to get */
    uint32_t* value)                /**< [out] Data buffer to store retrieved value */
{
    return ipj_get(iri_device, key, 0, 0, value);
}

 /**
 * This function sets the value of the specified key code on the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_set(
        ipj_iri_device *iri_device,   /**< [in] IRI device data structure */
        ipj_key         key,          /**< [in] Key code to set*/
        uint32_t        bank_index,   /**< [in] Key code bank_index to set (applicable for key codes with more than one copy) */
        uint32_t        value_index,  /**< [in] Key code value_index to set (applicable for key codes with lists) */
        uint32_t        value)        /**< [in] Value to set */
{
    ipj_bulk_set_command command;
    command.key = key;
    command.value = value;
    command.bank_index = bank_index;
    command.value_index = value_index;

    return ipj_internal_transact(
            iri_device,
            FIELD_ID_BULK_SET,
            sizeof(command),
            &command,
            NULL,
            NULL);
}

/**
 * This function sets the value of the specified key code on the IRI
 * device.
 *
 * \return ipj_error
 */
ipj_error ipj_set_value(
        ipj_iri_device *iri_device, /**< [in] IRI device data structure */
        ipj_key         key,        /**< [in] Key code to set*/
        uint32_t        value)      /**< [in] Value to set */
{
    return ipj_set(iri_device, key, 0, 0, value);
}

static ipj_error ipj_reset_handler(ipj_rsp_union *rsp_union, void* args)
{
    platform_sleep_ms_handler(IRI_SOFT_RESET_DELAY_MS);
    return E_IPJ_ERROR_SUCCESS;
}

 /**
 * This function resets the IRI device.
 * State of the IRI device is not preserved across resets.
 *
 * \return ipj_error
 */
ipj_error ipj_reset(
        ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
        ipj_reset_type reset_type)      /**< [in] Type of reset to perform*/
{
    ipj_reset_command command;
    command.reset_type = reset_type;

    return ipj_internal_transact(
            iri_device,
            FIELD_ID_RESET,
            sizeof(command),
            &command,
            NULL,
            ipj_reset_handler);
}
#endif

/**
* This function processes incoming reports from the IRI device.
* User calls this function to process incoming data. This function requests available data from the platform receive handler,
* buffers data, and checks for complete reports. This function returns immediately if there is not sufficient data for a report.
* Once a complete report is received, this function calls the report handler before returning.
*
* User decides when to call this function. For example, User can poll or wait for
* an interrupt from the system. User responsibility to call this function to keep
* the receive buffer from overflowing.
*
* \return ipj_error
*/
ipj_error ipj_receive(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    /*Don't assume something bad happened */
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint16_t read_length;

    if (iri_device->receive_index < iri_device->frame_length)
    {
        /* Read remaining bits in packet */
        platform_receive_handler(
                iri_device->reader_context,
                &iri_device->receive_buffer[iri_device->receive_index],
                iri_device->frame_length - iri_device->receive_index,
                &read_length,
                0);

        iri_device->receive_index += read_length;
    }
    else
    {
        switch (iri_device->sync_state)
        {
            case E_SEEKING_FRAME_SYNC:
            {
                FrameHeader * frame_header = (FrameHeader*) iri_device->receive_buffer;

                /* At this point, we have enough data for a complete header
                 * Calculate the payload length and jump to next state */
                if (ipj_validate_frame_header(frame_header) &&
                        iri_device->frame_length <= IPJ_RECEIVE_BUFFER_SIZE)
                {
                    iri_device->sync_state = E_GOT_FRAME_SYNC;
                    iri_device->frame_length =
                            ntohs(frame_header->nDataLength);
                }
                else /* We need to shift the buffer window*/
                {
                    iri_device->receive_index--;
                    memcpy(
                            iri_device->receive_buffer,
                            &iri_device->receive_buffer[1],
                            iri_device->receive_index);
                }
                break;
            }
            case E_GOT_FRAME_SYNC:
            {
                PacketHeader * packet_header = (PacketHeader*) &iri_device->receive_buffer[sizeof(FrameHeader)];
                uint32_t * rr_buf = (uint32_t*) &iri_device->receive_buffer[sizeof(FrameHeader) + sizeof(PacketHeader)];
                uint32_t i;

#if defined(DEBUG_SERIAL_TRAFFIC)
    printf("Read  [%d] ", iri_device->receive_index);
    for(i = 0; i < iri_device->receive_index; i++)
    {
        printf("%02X ", iri_device->receive_buffer[i]);
    }
    printf("\n");
#endif

                /* Reset receive index, flushes buffer */
                iri_device->receive_index = 0;
                iri_device->sync_state    = E_SEEKING_FRAME_SYNC;
                iri_device->frame_length  = IPJ_FRAME_HEADER_SIZE;

                if (ipj_validate_packet_header(packet_header))
                {
                    uint32_t report_count_32 = packet_header->lt_buf_length/sizeof(uint32_t);

                    /* Copy response/report from little endian to host */
                    for (i = 0; i < report_count_32; i++)
                    {
                        rr_buf[i] = letohl(rr_buf[i]);
                    }

                    if (FIELD_ID(packet_header->pkt_tag) == FIELD_ID_REPORT)
                    {
                        error = ipj_report_handler(
                                iri_device,
                                FIELD_ID(packet_header->sub_tag),
                                report_count_32,
                                (void*) rr_buf);
                    }
                    else
                    {
                        // Tell internal transact to stop since we got a response
                        iri_device->wait_for_response = false;
                    }
                }
#if defined(ENABLE_FW_UPDATES)
                else if(*(uint32_t*)packet_header == IPJ_GOOD_IRI_RESET_RESPONSE)
                {
                    /*
                     * This is probably an IRI reset response from the bootstrap
                     */
                    iri_device->wait_for_response = false;
                }
#endif
                else
                {
                    return E_IPJ_ERROR_API_NON_LT_PACKET_DETECTED;
                }
                break;
            }
        }
    }

    return error;
}

static uint8_t ipj_internal_calculate_parity_8(uint8_t* buf,int32_t len)
{
    int32_t i;
    uint8_t res = 0;

    for (i = 0; i < len; i++)
    {
        res ^= buf[i];
    }

    return res;
}

static bool ipj_validate_frame_header(FrameHeader * frame_header)
{
    /*
     * Make sure the Marker is found.
     * Make sure the version matches.
     * Make sure the parity is correct.
     */
    return ntohl(frame_header->nMarker) == IPJ_MARKER
            && frame_header->nVersion == IPJ_HDR_VERSION
            && ipj_internal_calculate_parity_8(
                    (uint8_t*)frame_header,
                    IPJ_FRAME_HEADER_SIZE) == 0;
}

static bool ipj_validate_packet_header(PacketHeader * packet_header)
{
    /*
     * Make sure the first four fields are not varints.
     * Make sure the lt_buf_tag has the expected id.
     * Make sure the remaining packet size is not too big.
     */
    return ((packet_header->pkt_tag    & 0x80) == 0 &&
            (packet_header->pkt_length & 0x80) == 0 &&
            (packet_header->sub_tag    & 0x80) == 0 &&
            (packet_header->sub_length & 0x80) == 0 &&
            (FIELD_ID(packet_header->lt_buf_tag) == FIELD_ID_LT_BUFFER &&
            packet_header->lt_buf_length <= MAX_RX_PAYLOAD_SIZE));
}

#if !defined(IRI_RX_ONLY)
static ipj_error ipj_internal_transmit_packet(
        ipj_iri_device* iri_device,
        uint8_t command_id,
        uint8_t command_size,
        void* cmd)
{
    FrameHeader * frame_header = (FrameHeader*) iri_device->transmit_buffer;
    PacketHeader * packet_header = (PacketHeader*) &iri_device->transmit_buffer[sizeof(FrameHeader)];
    uint32_t * command = (uint32_t*) &iri_device->transmit_buffer[sizeof(FrameHeader) + sizeof(PacketHeader)];
    uint16_t number_transmitted_bytes;
    uint32_t result;
    uint32_t i;

    /* Transmit length equals 10-byte header plus payload length plus 2-byte CRC */
    uint16_t transmit_length = sizeof(FrameHeader) + sizeof(PacketHeader) + command_size + sizeof(PacketCrc);

    /* Clear the transmit buffer to ensure we get a clean start */
    memset(iri_device->transmit_buffer, 0x00, sizeof(iri_device->transmit_buffer));

    /* Frame Header */
    frame_header->nMarker        = htonl(IPJ_MARKER);
    frame_header->nVersion       = IPJ_HDR_VERSION;
    frame_header->nDataLength    = htons(transmit_length);
#if defined(SUPPRESS_SET_RESPONSES)
    frame_header->nFlags |= IPJ_FLAG_SUPPRESS_SET_RESPONSES;
#endif
    frame_header->nChecksum      = ipj_internal_calculate_parity_8(
                iri_device->transmit_buffer,
                (IPJ_FRAME_HEADER_SIZE - 1));

    /* Packet Header */
    packet_header->pkt_tag       = LENGTH_DELIMITED_FIELD(FIELD_ID_COMMAND);
    packet_header->pkt_length    = command_size + sizeof(PacketHeader) - 2;
    packet_header->sub_tag       = LENGTH_DELIMITED_FIELD(command_id);
    packet_header->sub_length    = command_size + sizeof(PacketHeader) - 4;
    packet_header->lt_buf_tag    = LENGTH_DELIMITED_FIELD(FIELD_ID_LT_BUFFER);
    packet_header->lt_buf_length = command_size;

    /* Copy in the command (in little endian) */
    for (i = 0; i < command_size/sizeof(uint32_t); i++)
    {
        *command++ = htolel(((uint32_t*)cmd)[i]);
    }

#if defined(DEBUG_SERIAL_TRAFFIC)
    printf("Write [%d] ", transmit_length);
    for(i = 0; i < transmit_length; i++)
    {
        printf("%02X ", iri_device->transmit_buffer[i]);
    }
    printf("\n");
#endif

    /* Transmit packet */
    result = platform_transmit_handler(
            iri_device->reader_context,
            iri_device->transmit_buffer,
            transmit_length,
            &number_transmitted_bytes);

    if (!result || (number_transmitted_bytes != transmit_length))
    {
        return E_IPJ_ERROR_API_CONNECTION_WRITE_ERROR;
    }

    return E_IPJ_ERROR_SUCCESS;
}

static ipj_error ipj_internal_transact(
        ipj_iri_device* iri_device,
        uint8_t command_id,
        uint8_t command_size,
        void* command,
        void* handler_args,
        ipj_internal_completion_handler handler)
{
    ipj_error error;
    uint32_t current_time_ms;
    uint32_t start_time_ms;

    PacketHeader * packet_header = (PacketHeader*) &iri_device->receive_buffer[sizeof(FrameHeader)];
    ipj_rsp_union * rsp_union = (ipj_rsp_union*) &iri_device->receive_buffer[sizeof(FrameHeader) + sizeof(PacketHeader)];

    /* Transmit command */
    error = ipj_internal_transmit_packet(iri_device, command_id, command_size, command);
    if (error)
    {
        return error;
    }

#if defined(SUPPRESS_SET_RESPONSES)
    /* If responses are suppressed, just return a success */
    if (command_id == FIELD_ID_BULK_SET)
    {
        return E_IPJ_ERROR_SUCCESS;
    }
#endif

    iri_device->wait_for_response = true;

    /* Set time to timeout - current time pulse timeout */
    start_time_ms = platform_timestamp_ms_handler();

    /* Receive response */
    /* DO-WHILE loop until response received (if waiting for response),
     * otherwise only loop once (unless shifting bits for synchronization purposes) */
    do
    {
        /* Get current time */
        current_time_ms = platform_timestamp_ms_handler();

        /* Check if current time beyond timeout time */
        if (current_time_ms - start_time_ms >= iri_device->receive_timeout_ms)
        {
            /* Reset the Rx state machine */
            iri_device->receive_index     = 0;
            iri_device->sync_state        = E_SEEKING_FRAME_SYNC;
            iri_device->frame_length      = IPJ_FRAME_HEADER_SIZE;
            iri_device->wait_for_response = false;

            return E_IPJ_ERROR_API_CONNECTION_READ_TIMEOUT;
        }

        error = ipj_receive(iri_device);
        if (error)
        {
            return error;
        }

    } while (iri_device->wait_for_response);

    /*
     * Check for expected response.
     * Response id is the same as command id.
     */
    if (FIELD_ID(packet_header->sub_tag) == command_id)
    {
        if (rsp_union->generic_response.error != E_IPJ_ERROR_SUCCESS)
        {
#if defined(ENABLE_FW_UPDATES)
            if(*(uint32_t*)packet_header == IPJ_GOOD_IRI_RESET_RESPONSE)
            {
                /*
                 * This is probably an IRI reset response from the bootstrap
                 */
                return E_IPJ_ERROR_SUCCESS;
            }
#endif
            return rsp_union->generic_response.error;
        }
        else if (handler)
        {
            /* Call a handler to finish processing the response */
            return handler(rsp_union, handler_args);
        }
    }
    else if (FIELD_ID(packet_header->sub_tag) == FIELD_ID_INVALID)
    {
        /* Catch an InavlidResponse */
        if (rsp_union->generic_response.error != E_IPJ_ERROR_SUCCESS)
        {
            return rsp_union->generic_response.error;
        }

        /* The error should have been set, if not then just report a mismatch */
        return E_IPJ_ERROR_API_RESPONSE_MISMATCH;
    }
    else
    {
        /* Response not correct */
        return E_IPJ_ERROR_API_RESPONSE_MISMATCH;
    }

    return E_IPJ_ERROR_SUCCESS;
}

#if defined (ENABLE_FW_UPDATES)
/*
 * Optimized for values that will fit into 4 encoded bytes
 * i.e. that the top 4 bits are zero
 *   (value & 0xF0000000 == 0)
 */
static uint32_t ipj_encode_varint(uint32_t value)
{
    uint32_t encoded  = 0;
    uint8_t *buffer = (uint8_t*)&encoded;
    int i = 0;

    while (value && i < sizeof(encoded))
    {
        buffer[i] = (uint8_t) ((value & 0x7F) | 0x80);
        value >>= 7;
        i++;
    }
    buffer[i - 1] &= 0x7F; /* Unset top bit on last byte */
    return encoded;
}

/*
 * Assumes:
 *
 * Write 128 byte chunks
 * Erase length can be varint encoded in 2 or 3 bytes
 */
ipj_error ipj_internal_flash_operation(
        ipj_iri_device *iri_device,
        ipj_flash_operation action,
        uint32_t offset,
        uint32_t len,
        uint8_t *data)
{
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint32_t current_time_ms;
    uint32_t start_time_ms;
    uint16_t transmit_length = 0;
    uint16_t number_transmitted_bytes;
    uint32_t result;
    uint32_t i;
    uint16_t read_length;

    static const uint8_t good_flash_erase_resp[] = { 0x22, 0x05, 0x8A, 0x01, 0x02, 0x08, 0x03 };
    static const uint8_t good_flash_write_resp[] = { 0x22, 0x05, 0x8A, 0x01, 0x02, 0x08, 0x01 };
    const uint8_t *good_flash_resp = good_flash_erase_resp;


    /*
     * Offsets must be varint encoded in 4 bytes
     */
    if (offset < IPJ_VARINT_4_BYTE_ENCODED_MIN ||
            offset >= IPJ_VARINT_5_BYTE_ENCODED_MIN)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /* Clear the transmit buffer to ensure we get a clean start */
    memset(iri_device->transmit_buffer, 0x00, sizeof(iri_device->transmit_buffer));

    switch (action)
    {
        case E_IPJ_FLASH_OPERATION_WRITE:
        {
            static uint8_t write_command[] =
            {
                0x8D, 0x70, 0x6A, 0x21, 0x02, 0x00, 0x00, 0x9D, 0x00, 0x29,
                0x1A, 0x8E, 0x01, 0x8A, 0x01, 0x8A, 0x01, 0x08, 0x01, 0x10,
                0xFF, 0xFF, 0xFF, 0xFF, 0x22, 0x80, 0x01
            };

            if (len != IPJ_SUPPORTED_CHUNK_SIZE)
            {
                return E_IPJ_ERROR_API_INVALID_PARAMETER;
            }

            /*
             * Overwrite offset
             */
            *(uint32_t*) (&write_command[IPJ_FLASH_WRITE_OFFSET_INDEX]) =
                    ipj_encode_varint(offset);

            memcpy(
                    iri_device->transmit_buffer,
                    write_command,
                    sizeof(write_command));

            memcpy(&iri_device->transmit_buffer[sizeof(write_command)], data, len);

            transmit_length = sizeof(write_command) + len + sizeof(PacketCrc);
            good_flash_resp = good_flash_write_resp;
            break;
        }
        case E_IPJ_FLASH_OPERATION_ERASE:
        {
            uint32_t len_encoded = 0;
            uint8_t  len_encoded_size = 0;

            static uint8_t erase_app[] =
            {
                0x8D, 0x70, 0x6A, 0x21, 0x02, 0x00, 0x00, 0x1C, 0x00, 0xA8,
                0x1A, 0x0E, 0x8A, 0x01, 0x0B, 0x08, 0x03, 0x10, 0x80, 0xE0,
                0x80, 0x40, 0x18, 0x80, 0xE0, 0x06, 0x00, 0x00
            };

            static uint8_t erase_stored_settings[] =
            {
                 0x8D, 0x70, 0x6A, 0x21, 0x02, 0x00, 0x00, 0x1B, 0x00, 0xAF,
                 0x1A, 0x0D, 0x8A, 0x01, 0x0A, 0x08, 0x03, 0x10, 0x80, 0xE0,
                 0x87, 0x40, 0x18, 0x80, 0x10, 0x00, 0x00
            };

            uint8_t * erase_command;

            if (len < IPJ_VARINT_4_BYTE_ENCODED_MIN
                    && len >= IPJ_VARINT_3_BYTE_ENCODED_MIN)
            {
                len_encoded_size = 3;
                erase_command = erase_app;
                transmit_length = sizeof(erase_app);
            }
            else if (len < IPJ_VARINT_3_BYTE_ENCODED_MIN
                    && len >= IPJ_VARINT_2_BYTE_ENCODED_MIN)
            {
                len_encoded_size = 2;
                erase_command = erase_stored_settings;
                transmit_length = sizeof(erase_stored_settings);
            }
            else
            {
                return E_IPJ_ERROR_API_INVALID_PARAMETER;
            }

            /*
             * Overwrite offset
             */
            *(uint32_t*) (&erase_command[IPJ_FLASH_ERASE_OFFSET_INDEX]) =
                    ipj_encode_varint(offset);

            /*
             * Overwrite len
             */
            len_encoded = ipj_encode_varint(len);
            memcpy(&erase_command[IPJ_FLASH_ERASE_LENGTH_INDEX], &len_encoded, len_encoded_size);

            memcpy(
                    iri_device->transmit_buffer,
                    erase_command,
                    transmit_length);

            good_flash_resp = good_flash_erase_resp;
            break;
        }
    }

#if defined(DEBUG_SERIAL_TRAFFIC)
    printf("Write [%d] ", transmit_length);
    for (i = 0; i < transmit_length; i++)
    {
        printf("%02X ", iri_device->transmit_buffer[i]);
    }
    printf("\n");
#endif

    /* Transmit packet */
    result = platform_transmit_handler(
            iri_device->reader_context,
            iri_device->transmit_buffer,
            transmit_length,
            &number_transmitted_bytes);

    if (!result || (number_transmitted_bytes != transmit_length))
    {
        return E_IPJ_ERROR_API_CONNECTION_WRITE_ERROR;
    }

    /* Set time to timeout - current time pulse timeout */
    start_time_ms = platform_timestamp_ms_handler();

    /* Receive response or timeout*/
    do
    {
        /* Get current time */
        current_time_ms = platform_timestamp_ms_handler();

        /* Check if current time beyond timeout time */
        if (current_time_ms - start_time_ms >= iri_device->receive_timeout_ms)
        {
            iri_device->receive_index = 0;
            return E_IPJ_ERROR_API_CONNECTION_READ_TIMEOUT;
        }

        /* Read remaining bits in packet */
        platform_receive_handler(
                iri_device->reader_context,
                &iri_device->receive_buffer[iri_device->receive_index],
                IPJ_FLASH_RESPONSE_SIZE - iri_device->receive_index,
                &read_length,
                0);

        iri_device->receive_index += read_length;

    } while (iri_device->receive_index < IPJ_FLASH_RESPONSE_SIZE);

#if defined(DEBUG_SERIAL_TRAFFIC)
    printf("Read  [%d] ", iri_device->receive_index);
    for (i = 0; i < iri_device->receive_index; i++)
    {
        printf("%02X ", iri_device->receive_buffer[i]);
    }
    printf("\n");
#endif

    /*
     * Validate flash response
     */
    for (i = 0; i < IPJ_FLASH_RESPONSE_PAYLOAD_SIZE; i++)
    {
        if (iri_device->receive_buffer[sizeof(FrameHeader) + i]
                != good_flash_resp[i])
        {
            error = E_IPJ_ERROR_API_RESPONSE_MISMATCH;
            break;
        }
    }

    iri_device->receive_index = 0;

    return error;
}

/**
 * This function takes in an IRI_Loader chunk and processes it accordingly.
 *
 * \return ipj_error
 */
typedef struct _ipj_loader_block_header
{
    uint32_t chunk_no;
    uint32_t offset;
    uint32_t payload_len;
} ipj_loader_block_header;

ipj_error ipj_flash_handle_loader_block(
        ipj_iri_device *iri_device, /**< [in] IRI device data structure                 */
        uint32_t len,               /**< [in] Length of the data chunk (in bytes)       */
        uint8_t* data)              /**< [in] Pointer to the data chunk to be processed */
{
    ipj_loader_block_header *loader_block_header = (ipj_loader_block_header*) data;

    /* This is the erase block */
    if (loader_block_header->chunk_no == 0)
    {
        return ipj_internal_flash_operation(
                iri_device,
                E_IPJ_FLASH_OPERATION_ERASE,
                loader_block_header->offset,
                loader_block_header->payload_len,
                NULL);
    }
    else
    {
        /* Write the flash block to the appropriate location */
        return ipj_internal_flash_operation(
                iri_device,
                E_IPJ_FLASH_OPERATION_WRITE,
                loader_block_header->offset,
                loader_block_header->payload_len,
                &data[sizeof(ipj_loader_block_header)]);
    }
}
#endif

#endif
