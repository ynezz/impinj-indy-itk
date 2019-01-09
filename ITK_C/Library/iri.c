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

#include <pb_decode.h>
#include <pb_encode.h>
#include <packet.pb.h>
#include <version.h>

#define IPJ_ENUMS
#include "iri.h"
#include "platform.h"

#if defined(CI_BUILD)
#include "BuildInfo.h"
char const anIdentString[256] = IDENT_STRING;
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define IRI_SOFT_RESET_DELAY_MS			  100
#define IPJ_MARKER                        0x8D706A21
#define IPJ_HDR_VERSION                   2
#define FIELD_ID(tag)                     ((tag >> 3) & 0xFF)

#define GEN2_CRC_RESIDUE                  0x1d0f
#define LOADER_BLOCK_HEADER_SIZE          12
#define IPJ_CRC16_PRELOAD                 0xFFFF

/* IRI reader device context structure (internal to IRI API functions) */
#define IPJ_HEADER_FRAME_MARKER_SIZE      4
#define IPJ_HEADER_VERSION_SIZE           1
#define IPJ_HEADER_FLAGS_SIZE             1
#define IPJ_HEADER_LENGTH_SIZE            2
#define IPJ_HEADER_FRAME_SYNC_SIZE        1
#define IPJ_HEADER_CHECKSUM_SIZE          1
#define IPJ_HEADER_SIZE                   (IPJ_HEADER_FRAME_MARKER_SIZE + \
                                           IPJ_HEADER_VERSION_SIZE      + \
                                           IPJ_HEADER_FLAGS_SIZE        + \
                                           IPJ_HEADER_LENGTH_SIZE       + \
                                           IPJ_HEADER_FRAME_SYNC_SIZE   + \
                                           IPJ_HEADER_CHECKSUM_SIZE)

/* Header offsets */
#define IPJ_HEADER_FRAME_MARKER_OFFSET    0
#define IPJ_HEADER_VERSION_OFFSET         IPJ_HEADER_FRAME_MARKER_OFFSET + IPJ_HEADER_FRAME_MARKER_SIZE
#define IPJ_HEADER_FLAGS_OFFSET           IPJ_HEADER_VERSION_OFFSET + IPJ_HEADER_VERSION_SIZE
#define IPJ_HEADER_LENGTH_OFFSET          IPJ_HEADER_FLAGS_OFFSET + IPJ_HEADER_FLAGS_SIZE
#define IPJ_HEADER_FRAME_SYNC_OFFSET      IPJ_HEADER_LENGTH_OFFSET + IPJ_HEADER_LENGTH_SIZE
#define IPJ_HEADER_PARITY_OFFSET          IPJ_HEADER_FRAME_SYNC_OFFSET + IPJ_HEADER_FRAME_SYNC_SIZE

/* Flag masks */
#define IPJ_FLAG_CHECKSUM_PARITY_TYPE    (0 << 7)
#define IPJ_FLAG_CHECKSUM_CRC_TYPE       (1 << 7)
#define IPJ_FLAG_CHECKSUM_ALGORITHM_MASK (1 << 7)
#define IPJ_FLAG_ENABLE_CHECKSUM_MASK    (1 << 6)
#define IPJ_FLAG_ENABLE_SYNC_COUNT_MASK  (1 << 5)
#define IPJ_FLAG_RESET_SYNC_COUNT_MASK   ((1 << 4) | (1 << 3))
#define IPJ_FLAG_DEVICE_RESET_SYNC_MASK  (1 << 4)
#define IPJ_FLAG_HOST_RESET_SYNC_MASK    (1 << 3)
#define IPJ_FLAG_SUPPRESS_SET_RESPONSES  (1 << 2)
#define IPJ_FLAG_EXTENDED_COMMAND_MASK   (1 << 0)

/* Protobuf offsets */
#define PB_PKT_SIZE                       8
#define PB_CMD_SIZE                       8
#define PB_MSG_SIZE                       MAX_TX_PAYLOAD_SIZE - PB_PKT_SIZE - PB_CMD_SIZE

#define RETURN_ERROR_ON_INVALID_DEVICE()          \
if (!iri_device || !iri_device->initialized)      \
{                                                 \
    return E_IPJ_ERROR_API_DEVICE_NOT_INITIALIZED;\
}

enum RX_FRAME_SYNC_STATE
{
    E_SEEKING_FRAME_SYNC,
    E_GOT_FRAME_SYNC
};

/* All responses begin with an error code */
typedef struct _GenericResponse
{
    bool has_error;
    ipj_error error;
} GenericResponse;

typedef union _ipj_rr_union
{
    /* Responses */
    GenericResponse          generic_resp;
    ResetResponse            reset_resp;
    ModifyConnectionResponse modifyconnection_resp;
    GetInfoResponse          getinfo_resp;
    BulkSetResponse          bulkset_resp;
    BulkGetResponse          bulkget_resp;
    StartResponse            start_resp;
    ResumeResponse           resume_resp;
    StopResponse             stop_resp;
    InvalidResponse          invalid_resp;
    FlashResponse            flash_resp;
    /* Reports */
    TagOperationReport       tagoperation_rpt;
    StopReport               stop_rpt;
    TestReport               test_rpt;
    ErrorReport              error_rpt;
    StatusReport             status_rpt;
    GPIOReport               gpio_rpt;
} ipj_rr_union;

typedef ipj_error (*ipj_internal_completion_handler)(
        ipj_rr_union *rr_union,
        void* args);

typedef struct
{
    ipj_key_value* key_value;
    ipj_key_list* key_list;
} bulk_get_handler_args;

/* Internal prototype declarations */
static ipj_error ipj_internal_transmit_packet(
        ipj_iri_device* iri_device,
        const pb_field_t* cmd_field_desc,
        void* cmd);
static uint16_t ipj_internal_calculate_crc(uint8_t* data, uint16_t data_length);

static ipj_error ipj_internal_transact(
        ipj_iri_device* iri_device,
        const pb_field_t* cmd_field_dvesc,
        void* command,
        void* handler_args,
        ipj_internal_completion_handler handler);

static void ipj_internal_reset_rx_state_machine(ipj_iri_device* iri_device);
static ipj_error ipj_internal_receive(
        ipj_iri_device* iri_device,
        ipj_rr_union* rr_union,
        uint32_t * response_id);

static ipj_error ipj_get_info_handler(ipj_rr_union *rr_union, void* args);
static ipj_error ipj_bulk_get_handler(ipj_rr_union *rr_union, void* args);
static ipj_error ipj_flash_handler(ipj_rr_union *rr_union, void* args);
static ipj_error ipj_reset_handler(ipj_rr_union *rr_union, void* args);
static bool      ipj_internal_test_frame_sync(ipj_iri_device* iri_device);
static uint32_t  ipj_internal_get_frame_length(ipj_iri_device* iri_device);

/**
 * This function returns the API version.
 *
 * \return uint32_t
 */
uint32_t ipj_get_api_version()
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
ipj_error ipj_initialize_iri_device(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    if (!iri_device)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /* Clear IRI device structure */
    memset(iri_device, 0, sizeof(ipj_iri_device));

    /* Set up default protocol flags */
    iri_device->protocol_flags = IPJ_FLAG_ENABLE_CHECKSUM_MASK |
        IPJ_FLAG_CHECKSUM_PARITY_TYPE |
#if defined(IPJ_ENABLE_FRAME_COUNT)
        IPJ_FLAG_ENABLE_SYNC_COUNT_MASK |
#endif    /* IPJ_ENABLE_FRAME_COUNT */
        IPJ_FLAG_HOST_RESET_SYNC_MASK;

    /* Set timeout values */
    iri_device->transmit_timeout_ms = IPJ_DEFAULT_TRANSMIT_TIMEOUT_MS;
    iri_device->receive_timeout_ms = IPJ_DEFAULT_RECEIVE_TIMEOUT_MS;

    ipj_internal_reset_rx_state_machine(iri_device);

    /* Set initialized flag */
    iri_device->initialized = true;

    /* Return success */
    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function deinitializes the IRI device data structure.
 * User may deinitialize IRI device after disconnecting from the IRI device.
 * Once the IRI device is deinitialized, the User may de-allocate the IRI device data structure.
 *
 * \return ipj_error
 */
ipj_error ipj_deinitialize_iri_device(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    RETURN_ERROR_ON_INVALID_DEVICE();

    /* Clear IRI device structure */
    memset(iri_device, 0, sizeof(ipj_iri_device));

    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function registers platform and report handlers with callback data.
 * Before using an IRI device, the User application must do the following:
 * allocate or declare IRI device data structure,
 * initialize IRI device data structure,
 * register platform handlers (this function or the non-_ext-suffixed version),
 * and connect to the IRI device.
 *
 * User application registers the following platform handlers:
 * platform open port handler,
 * platform close port handler,
 * platform receive handler,
 * platform transmit handler,
 * and platform timestamp handler.
 *
 * User application registers report handler to handle the following reports:
 * tag operation report,
 * error report,
 * and stop report
 *
 * \return ipj_error
 */
ipj_error ipj_register_handler_ext(
        ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
        ipj_handler_type handler_type,  /**< [in] Type of handler to register */
        IPJ_VOID_PFN handler,           /**< [in] Handler pointer */
        void* args)                     /**< [in] Handler opaque argument data */
{
    RETURN_ERROR_ON_INVALID_DEVICE();

    /* if user passed in a null pointer */
    if (!handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    switch (handler_type)
    {
        case E_IPJ_HANDLER_TYPE_PLATFORM_OPEN_PORT:
            iri_device->platform_open_port_handler =
                    (PLATFORM_OPEN_PORT_HANDLER_EXT) handler;
            iri_device->platform_open_port_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_CLOSE_PORT:
            iri_device->platform_close_port_handler =
                    (PLATFORM_CLOSE_PORT_HANDLER_EXT) handler;
            iri_device->platform_close_port_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_TRANSMIT:
            iri_device->platform_transmit_handler =
                    (PLATFORM_TRANSMIT_HANDLER_EXT) handler;
            iri_device->platform_transmit_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_RECEIVE:
            iri_device->platform_receive_handler =
                    (PLATFORM_RECEIVE_HANDLER_EXT) handler;
            iri_device->platform_receive_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_TIMESTAMP:
            iri_device->platform_timestamp_ms_handler =
                    (PLATFORM_TIMESTAMP_MS_HANDLER_EXT) handler;
            iri_device->platform_timestamp_ms_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_SLEEP_MS:
            iri_device->platform_sleep_ms_handler =
                    (PLATFORM_SLEEP_MS_HANDLER_EXT) handler;
            iri_device->platform_sleep_ms_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_REPORT:
            iri_device->report_handler = (REPORT_HANDLER_EXT) handler;
            iri_device->report_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_DIAGNOSTIC:
            iri_device->diagnostic_handler = (DIAGNOSTIC_HANDLER_EXT) handler;
            iri_device->diagnostic_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_MODIFY_CONNECTION:
            iri_device->platform_modify_connection_handler =
                    (PLATFORM_MODIFY_CONNECTION_HANDLER_EXT) handler;
            iri_device->platform_modify_connection_args = args;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_FLUSH_PORT:
            iri_device->platform_flush_port_handler =
                    (PLATFORM_FLUSH_PORT_HANDLER_EXT) handler;
            iri_device->platform_flush_port_args = args;
            break;
        default:
            return E_IPJ_ERROR_VALUE_INVALID;
    }

    return E_IPJ_ERROR_SUCCESS;
}

/*
 * Compatibility callbacks to support the old-style API without an opaque companion pointer.
 */

static uint32_t compat_platform_open_port_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT* reader_context,
        IPJ_READER_IDENTIFIER reader_identifier,
        ipj_connection_type connection_type,
        ipj_connection_params* params)
{
    PLATFORM_OPEN_PORT_HANDLER handler = (PLATFORM_OPEN_PORT_HANDLER) opaque_args;
    return handler(reader_context,
                   reader_identifier,
                   connection_type,
                   params);
}

static uint32_t compat_platform_close_port_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT reader_context)
{
    PLATFORM_CLOSE_PORT_HANDLER handler = (PLATFORM_CLOSE_PORT_HANDLER) opaque_args;
    return handler(reader_context);
}

static uint32_t compat_platform_transmit_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_written)
{
    PLATFORM_TRANSMIT_HANDLER handler = (PLATFORM_TRANSMIT_HANDLER) opaque_args;
    return handler(reader_context,
                   message_buffer,
                   buffer_size,
                   number_bytes_written);
}

static uint32_t compat_platform_receive_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT reader_context,
        uint8_t* message_buffer,
        uint16_t buffer_size,
        uint16_t* number_bytes_received,
        uint16_t timeout_ms)
{
    PLATFORM_RECEIVE_HANDLER handler = (PLATFORM_RECEIVE_HANDLER) opaque_args;
    return handler(reader_context,
                   message_buffer,
                   buffer_size,
                   number_bytes_received,
                   timeout_ms);
}

static uint32_t compat_platform_timestamp_ms_handler(
        void* opaque_args)
{
    PLATFORM_TIMESTAMP_MS_HANDLER handler = (PLATFORM_TIMESTAMP_MS_HANDLER) opaque_args;
    return handler();
}

static void compat_platform_sleep_ms_handler(
        void* opaque_args,
        uint32_t milliseconds)
{
    PLATFORM_SLEEP_MS_HANDLER handler = (PLATFORM_SLEEP_MS_HANDLER) opaque_args;
    handler(milliseconds);
}

static uint32_t compat_platform_flush_port_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT reader_context)
{
    PLATFORM_FLUSH_PORT_HANDLER handler = (PLATFORM_FLUSH_PORT_HANDLER) opaque_args;
    return handler(reader_context);
}

static uint32_t compat_platform_modify_connection_handler(
        void* opaque_args,
        IPJ_READER_CONTEXT reader_context,
        ipj_connection_type connection_type,
        ipj_connection_params* params)
{
    PLATFORM_MODIFY_CONNECTION_HANDLER handler = (PLATFORM_MODIFY_CONNECTION_HANDLER) opaque_args;
    return handler(reader_context,
                   connection_type,
                   params);
}

static ipj_error compat_report_handler(
        void* opaque_args,
        struct ipj_iri_device* iri_device,
        ipj_report_id report_id,
        void* report)
{
    REPORT_HANDLER handler = (REPORT_HANDLER) opaque_args;
    return handler(iri_device,
                   report_id,
                   report);
}

static void compat_diagnostic_handler(
        void* opaque_args,
        struct ipj_iri_device* iri_device,
        ipj_error error)
{
    DIAGNOSTIC_HANDLER handler = (DIAGNOSTIC_HANDLER) opaque_args;
    handler(iri_device,
            error);
}

/**
 * This function registers platform and report handlers.
 * Before using an IRI device, the User application must do the following:
 * allocate or declare IRI device data structure,
 * initialize IRI device data structure,
 * register platform handlers (this function or the _ext-suffixed version),
 * and connect to the IRI device.
 *
 * User application registers the following platform handlers:
 * platform open port handler,
 * platform close port handler,
 * platform receive handler,
 * platform transmit handler,
 * and platform timestamp handler.
 *
 * User application registers report handler to handle the following reports:
 * tag operation report,
 * error report,
 * and stop report
 *
 * \return ipj_error
 */
ipj_error ipj_register_handler(
        ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
        ipj_handler_type handler_type,  /**< [in] Type of handler to register */
        IPJ_VOID_PFN handler)           /**< [in] Handler pointer */
{
    IPJ_VOID_PFN compat_handler;

    RETURN_ERROR_ON_INVALID_DEVICE();

    /* if user passed in a null pointer */
    if (!handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    switch (handler_type)
    {
        case E_IPJ_HANDLER_TYPE_PLATFORM_OPEN_PORT:
            compat_handler = (IPJ_VOID_PFN) compat_platform_open_port_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_CLOSE_PORT:
            compat_handler = (IPJ_VOID_PFN) compat_platform_close_port_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_TRANSMIT:
            compat_handler = (IPJ_VOID_PFN) compat_platform_transmit_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_RECEIVE:
            compat_handler = (IPJ_VOID_PFN) compat_platform_receive_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_TIMESTAMP:
            compat_handler = (IPJ_VOID_PFN) compat_platform_timestamp_ms_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_SLEEP_MS:
            compat_handler = (IPJ_VOID_PFN) compat_platform_sleep_ms_handler;
            break;
        case E_IPJ_HANDLER_TYPE_REPORT:
            compat_handler = (IPJ_VOID_PFN) compat_report_handler;
            break;
        case E_IPJ_HANDLER_TYPE_DIAGNOSTIC:
            compat_handler = (IPJ_VOID_PFN) compat_diagnostic_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_MODIFY_CONNECTION:
            compat_handler = (IPJ_VOID_PFN) compat_platform_modify_connection_handler;
            break;
        case E_IPJ_HANDLER_TYPE_PLATFORM_FLUSH_PORT:
            compat_handler = (IPJ_VOID_PFN) compat_platform_flush_port_handler;
            break;
        default:
            return E_IPJ_ERROR_VALUE_INVALID;
    }

    return ipj_register_handler_ext(iri_device,
                                    handler_type,
                                    compat_handler,
                                    handler);
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

    RETURN_ERROR_ON_INVALID_DEVICE();

    /* Set up default parameters */
    switch (connection_type)
    {
        case E_IPJ_CONNECTION_TYPE_SERIAL:
        {
            default_params.serial.baudrate = E_IPJ_BAUD_RATE_BR115200;
            default_params.serial.parity = E_IPJ_PARITY_PNONE;
            break;
        }
        default:
        {
            return E_IPJ_ERROR_VALUE_INVALID;
        }
    }

    /* if params is NULL, set it to default params */
    if (!params)
    {
        params = &default_params;
    }

    if (!iri_device->platform_open_port_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    /* Open serial port */
    result = iri_device->platform_open_port_handler(
            iri_device->platform_open_port_args,
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
 * This function modifies the serial port baud rate used to talk to an IRI device.
 * Before modifying an iri connection, the user must have already successfully
 * connected to a device.
 *
 * \return ipj_error
 */
ipj_error ipj_modify_connection(
        ipj_iri_device* iri_device,          /**< [in] IRI device data structure */
        ipj_connection_type connection_type, /**< [in] Connection modification type */
        ipj_connection_params* params)       /**< [in] Connection modification parameters  */
{
    ipj_error error;
    ModifyConnectionCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    if (!params)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    command.has_connection_type = true;
    command.has_value           = true;
    command.connection_type     = connection_type;
    command.value               = params->serial.baudrate;

#if defined(IPJ_SERIAL_AUTOBAUD_CLOCK)
    command.has_baud_autoclock  = true;
    command.baud_autoclock      = true;
#endif

    error = ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_MODIFY_CONNECTION],
            &command,
            NULL,
            NULL);

    if (error)
    {
        return error;
    }

    if (!iri_device->platform_flush_port_handler
            || !iri_device->platform_modify_connection_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    /* Flush serial port */
    error = (ipj_error) iri_device->platform_flush_port_handler(
            iri_device->platform_flush_port_args,
            iri_device->reader_context);

    if (error)
    {
        return error;
    }

    /* Modify the host's baud rate */
    error = (ipj_error) iri_device->platform_modify_connection_handler(
            iri_device->platform_modify_connection_args,
            iri_device->reader_context,
            connection_type,
            params);
    if (error)
    {
        return error;
    }

    iri_device->platform_sleep_ms_handler(
            iri_device->platform_sleep_ms_args,
            10);

    /* Send another packet so that the controller can:
     * 1. Latch on for autobaud (if requested)
     * 2. Know for certain we're talking at the same speed
     */
    error = ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_MODIFY_CONNECTION],
            &command,
            NULL,
            NULL);

    if (error)
    {
        /* Default back to 115200 */
        params->serial.baudrate = E_IPJ_BAUD_RATE_BR115200;
        iri_device->platform_modify_connection_handler(
                iri_device->platform_modify_connection_args,
                iri_device->reader_context,
                connection_type,
                params);
        return error;
    }

    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function disconnects the Host from the IRI device and closes the serial port.
 * User may disconnect from the IRI device after all operations are completed.
 *
 * \return ipj_error
 */
ipj_error ipj_disconnect(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    uint32_t result;

    RETURN_ERROR_ON_INVALID_DEVICE();

    if (!iri_device->platform_close_port_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    /* Close serial port */
    result = iri_device->platform_close_port_handler(
                iri_device->platform_close_port_args,
                iri_device->reader_context);

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

/**
 * This function allows the user to suppress all 'set' and 'bulk-set' action
 * responses generated by the reader.  The user will be responsible for checking
 * and dealing with errors on the device.
 *
 * \return ipj_error
 */
ipj_error ipj_suppress_set_responses(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    RETURN_ERROR_ON_INVALID_DEVICE();
    iri_device->protocol_flags |= IPJ_FLAG_SUPPRESS_SET_RESPONSES;
    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function allows the user to resume generation of 'set' action responses
 * by the reader.  All 'set' and 'bulk-set' actions will receive an associated
 * response containing success/failure status of the command (This is the default)
 *
 * \return ipj_error
 */
ipj_error ipj_resume_set_responses(ipj_iri_device* iri_device) /**< [in] IRI device data structure */
{
    RETURN_ERROR_ON_INVALID_DEVICE();
    iri_device->protocol_flags &= ~IPJ_FLAG_SUPPRESS_SET_RESPONSES;
    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function allows the user to set the amount of time the receive
 * state machine should wait before returning a timeout error.
 *
 * \return ipj_error
 */
ipj_error ipj_set_receive_timeout_ms(ipj_iri_device* iri_device,      /**< [in] IRI device data structure */
                                     uint32_t timeout_ms)             /**< [in] Timeout value in milliseconds */
{
    RETURN_ERROR_ON_INVALID_DEVICE();

    iri_device->receive_timeout_ms = timeout_ms;
    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function starts the specified action or operation on the IRI device.
 * This function is used to start inventory to read tags.
 *
 * \return ipj_error
 */
ipj_error ipj_start(ipj_iri_device* iri_device, /**< [in] IRI device data structure      */
                    ipj_action      action)     /**< [in] Actions or operations to start */
{
    StartCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    command.has_action = true;
    command.action = action;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_START],
            &command,
            NULL,
            NULL);
}

/**
 * This function resumes the specified action or operation on the IRI device.
 * This function is used to resume operations after the IRI device has halted.
 *
 * \return ipj_error
 */
ipj_error ipj_resume(
        ipj_iri_device* iri_device,     /**< [in] IRI device data structure */
        ipj_action action)              /**< [in] Actions or operations to be resumed */
{
    ResumeCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    command.has_action = true;
    command.action = action;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_RESUME],
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
    StopCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    command.has_action = true;
    command.action = action;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_STOP],
            &command,
            NULL,
            NULL);
}

static ipj_error ipj_get_info_handler(ipj_rr_union *rr_union, void* args)
{
    ipj_key_info * key_info = (ipj_key_info*) (args);
    if (rr_union->getinfo_resp.has_key_info)
    {
        memcpy(key_info, &rr_union->getinfo_resp, sizeof(ipj_key_info));
        return E_IPJ_ERROR_SUCCESS;
    }
    else
    {
        return E_IPJ_ERROR_COMMAND_INVALID;
    }
}

/**
 * This function retrieves information about the specified key code from the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_get_info(
        ipj_iri_device *iri_device,    /**< [in] IRI device data structure */
        ipj_key         key,           /**< [in] Key code to get info */
        ipj_key_info   *key_info)      /**< [out] Information about the specified key code */
{
    GetInfoCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    if (!key_info)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    command.has_key = true;
    command.key = key;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_GET_INFO],
            &command,
            key_info,
            &ipj_get_info_handler);
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
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    ipj_key_value kvp = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    if (!value)
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    *value = 0xFFFFFFFF;

    kvp.key = key;
    kvp.bank_index = bank_index;
    kvp.value_index = value_index;

    error = ipj_bulk_get(iri_device, &kvp, 1, NULL, 0);

    if (error)
    {
        return error;
    }

    *value = kvp.value;

    return E_IPJ_ERROR_SUCCESS;
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
    ipj_key_value kvp = {0};

    kvp.key = key;
    kvp.value = value;
    kvp.bank_index = bank_index;
    kvp.value_index = value_index;

    return ipj_bulk_set(iri_device, &kvp, 1, NULL, 0);
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

static ipj_error ipj_bulk_get_handler(ipj_rr_union *rr_union, void* args)
{
    uint32_t i;
    bulk_get_handler_args * bulk_get_args = (bulk_get_handler_args*) (args);

    /* Copy in return values */
    for (i = 0; i < rr_union->bulkget_resp.key_value_count; i++)
    {
        memcpy(
                &bulk_get_args->key_value[i],
                &rr_union->bulkget_resp.key_value[i],
                sizeof(ipj_key_value));
    }

    for (i = 0; i < rr_union->bulkget_resp.key_list_count; i++)
    {
        memcpy(
                &bulk_get_args->key_list[i],
                &rr_union->bulkget_resp.key_list[i],
                sizeof(ipj_key_list));
    }

    return E_IPJ_ERROR_SUCCESS;
}

/**
 * This function retrieves the values of the specified key codes from the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_bulk_get(
        ipj_iri_device *iri_device,     /**< [in] IRI device data structure */
        ipj_key_value  *key_value,      /**< [in/out] Array of key codes to retrieve */
        uint32_t        key_value_count,/**< [in] Number of key codes and values to retrieve */
        ipj_key_list   *key_list,       /**< [in/out] Array of key codes to retrieve (used for key codes with lists) */
        uint32_t        key_list_count) /**< [in] Number of keys codes (with lists) to retrieve */
{
    uint32_t i;
    BulkGetCommand command = {0};
    bulk_get_handler_args bulk_get_args = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    /*
     * Error if there is a count specified for an array but the array is NULL
     */
    if ((key_value_count > 0 && !key_value) ||
        (key_list_count  > 0 && !key_list))
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /*
     * Error if the array sizes provided are out of range
     */
    if (key_value_count > ARRAY_SIZE(command.key_value) ||
        key_list_count  > ARRAY_SIZE(command.key_list))
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /*
     * Set GET command parameters
     */

    command.key_value_count = key_value_count;
    for (i = 0; i < key_value_count; i++)
    {
        key_value[i].has_key         = true;
        key_value[i].has_value       = false;
        key_value[i].has_value_index = true;
        key_value[i].has_bank_index  = true;

        memcpy(&command.key_value[i], &key_value[i], sizeof(ipj_key_value));
    }

    command.key_list_count = key_list_count;
    for (i = 0; i < key_list_count; i++)
    {
        /*
         * Make sure the number of values requested can fit in the list
         */
        if (key_list[i].length > ARRAY_SIZE(key_list[i].list))
        {
            return E_IPJ_ERROR_API_INVALID_PARAMETER;
        }

        key_list[i].has_key         = true;
        key_list[i].has_bank_index  = true;
        key_list[i].has_length      = true;
        key_list[i].has_value_index = true;
        key_list[i].list_count = 0;

        memcpy(&command.key_list[i], &key_list[i], sizeof(ipj_key_list));
    }

    bulk_get_args.key_value = key_value;
    bulk_get_args.key_list = key_list;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_BULK_GET],
            &command,
            &bulk_get_args,
            &ipj_bulk_get_handler);
}

/**
 * This function sets the value of the specified key codes on the IRI device.
 *
 * \return ipj_error
 */
ipj_error ipj_bulk_set(
        ipj_iri_device *iri_device,     /**< [in] IRI device data structure */
        ipj_key_value  *key_value,      /**< [in/out] Array of key codes and values to set */
        uint32_t        key_value_count,/**< [in] Number of key codes and values to set */
        ipj_key_list   *key_list,       /**< [in/out] Array of key codes to set (used for key codes with lists) */
        uint32_t        key_list_count) /**< [in] Number of keys codes (with lists) to set */
{
    uint32_t i;
    BulkSetCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    /*
     * Error if there is a count specified for an array but the array is NULL
     */
    if ((key_value_count > 0 && !key_value) || (key_list_count > 0 && !key_list))
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /*
     * Error if the array sizes provided are out of range
     */
    if (key_value_count > ARRAY_SIZE(command.key_value)
            || key_list_count > ARRAY_SIZE(command.key_list))
    {
        return E_IPJ_ERROR_API_INVALID_PARAMETER;
    }

    /*
     * Set BULK_SET command parameters
     */

    command.key_value_count = key_value_count;
    for (i = 0; i < key_value_count; i++)
    {
        key_value[i].has_key         = true;
        key_value[i].has_bank_index  = true;
        key_value[i].has_value       = true;
        key_value[i].has_value_index = true;

        memcpy(&command.key_value[i], &key_value[i], sizeof(ipj_key_value));
    }

    command.key_list_count = key_list_count;
    for (i = 0; i < key_list_count; i++)
    {
        key_list[i].has_key         = true;
        key_list[i].has_bank_index  = true;
        key_list[i].has_length      = false;
        key_list[i].has_value_index = true;

        memcpy(&command.key_list[i], &key_list[i], sizeof(ipj_key_list));
    }

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_BULK_SET],
            &command,
            NULL,
            NULL);
}

static ipj_error ipj_reset_handler(ipj_rr_union *rr_union, void* args)
{
    ipj_iri_device *iri_device = (ipj_iri_device*) (args);

    if (!iri_device->platform_sleep_ms_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    iri_device->platform_sleep_ms_handler(
                    iri_device->platform_sleep_ms_args,
                    IRI_SOFT_RESET_DELAY_MS);

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
    ResetCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    command.has_reset_type = true;
    command.reset_type = reset_type;

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_RESET],
            &command,
            iri_device,
            ipj_reset_handler);
}

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
    uint32_t response_id;
    ipj_rr_union rr_union = {{0}};
    return ipj_internal_receive(iri_device, &rr_union, &response_id);
}

/*
 * Do a parity 8 calculation on the provided buffer.
 *
 * Note the extra 8-bit masking (0xFF) steps. This is done
 * to support some MCU/DSP platforms that define a uint8_t as 16-bits.
 */
static uint8_t ipj_internal_calculate_parity_8(uint8_t* buf, int32_t len)
{
    int32_t i;
    uint8_t res = 0;

    for (i = 0; i < len; i++)
    {
        res ^= (buf[i] & 0xFF);
    }

    return res;
}

/*
 * Do a parity 16 calculation on the provided buffer.
 *
 * Note the extra 8-bit masking (0xFF) steps. This is done
 * to support some MCU/DSP platforms that define a uint8_t as 16-bits.
 */
static uint16_t ipj_internal_calculate_parity_16(uint8_t* buf, int32_t len)
{
    uint8_t high = 0;
    uint8_t low = 0;
    int32_t i = 0;
    uint16_t parity;
    while (i < len)
    {
        high ^= buf[i++];
        high &= 0xFF;
        if (i < len)
        {
            low ^= buf[i++];
            low &= 0xFF;
        }
    }
    parity = 0;
    parity = (high & 0xFF);
    parity <<= 8;
    parity |= (low & 0xFF);
    return parity;
}

static ipj_error ipj_internal_transmit_packet(
        ipj_iri_device* iri_device,
        const pb_field_t* cmd_field_desc,
        void* cmd)
{
    bool b_result = true;
    uint32_t result;
    uint16_t number_transmitted_bytes = 0;
    uint16_t transmit_length;
    uint16_t crc;
    uint8_t* bufptr;

    uint8_t* pkt_ptr = iri_device->transmit_buffer + IPJ_FRAME_HEADER_SIZE;
    uint8_t* cmd_ptr = pkt_ptr + PB_PKT_SIZE;
    uint8_t* msg_ptr = cmd_ptr + PB_CMD_SIZE;

    pb_ostream_t pkt_stream = pb_ostream_from_buffer(pkt_ptr, PB_PKT_SIZE);
    pb_ostream_t cmd_stream = pb_ostream_from_buffer(cmd_ptr, PB_CMD_SIZE);
    pb_ostream_t msg_stream = pb_ostream_from_buffer(msg_ptr, PB_MSG_SIZE);

    /* Clear the transmit buffer to ensure we get a clean start */
    memset(iri_device->transmit_buffer, 0x00, sizeof(iri_device->transmit_buffer));

    /* Encode message */
    b_result &= pb_encode(&msg_stream, (pb_field_t*) cmd_field_desc->ptr, cmd);
    /* Encode the command tag and length */
    b_result &= pb_encode_tag_for_field(&cmd_stream, cmd_field_desc);
    b_result &= pb_encode_varint(&cmd_stream, msg_stream.bytes_written);
    /* Encode the packet tag and length */
    b_result &= pb_encode_tag_for_field(
            &pkt_stream,
            &Packet_fields[PACKET_FIELDS_INDEX_COMMAND]);
    b_result &= pb_encode_varint(
            &pkt_stream,
            msg_stream.bytes_written + cmd_stream.bytes_written);
    if (!b_result)
    {
        return E_IPJ_ERROR_COMMAND_ENCODE_FAILURE;
    }

    /* Transmit length equals 10-byte header plus the total combined length of
     * the protobufs plus 2-byte CRC */
    transmit_length = (uint16_t) (
            IPJ_FRAME_HEADER_SIZE    +
            pkt_stream.bytes_written +
            cmd_stream.bytes_written +
            msg_stream.bytes_written +
            IPJ_FRAME_CRC_SIZE);

    /* If a reset sync flag exists, reset the tx sync counter */
    if (iri_device->protocol_flags & IPJ_FLAG_RESET_SYNC_COUNT_MASK)
    {
        iri_device->tx_frame_sync_count = 0;
    }

    /* Clear the reset flags */
    iri_device->protocol_flags &= ~IPJ_FLAG_RESET_SYNC_COUNT_MASK;

    /* Copy in the framing bytes + version */
    bufptr = iri_device->transmit_buffer;
    *(bufptr++) = (IPJ_MARKER >> 24) & 0xFF;
    *(bufptr++) = (IPJ_MARKER >> 16) & 0xFF;
    *(bufptr++) = (IPJ_MARKER >> 8)  & 0xFF;
    *(bufptr++) = IPJ_MARKER & 0xFF;
    *(bufptr++) = IPJ_HDR_VERSION;

    /* Write iri device flags to header */
    *(bufptr++) = iri_device->protocol_flags;

    /* Write transmit length to header MSB first*/
    *(bufptr++) = (uint8_t) (transmit_length >> 8);
    *(bufptr++) = (uint8_t) (transmit_length & 0xff);

    /* write frame sync count to header*/
    *(bufptr++) = iri_device->tx_frame_sync_count++;

    /* Write Parity byte to header */
    *(bufptr++) = ipj_internal_calculate_parity_8(
            iri_device->transmit_buffer,
            (IPJ_FRAME_HEADER_SIZE - 1));

    /* Pack the protobuf data into the buffer payload area */
    /* Set the buf pointer to the end of the packet stream area */
    bufptr = pkt_ptr + pkt_stream.bytes_written;

    /* Copy in the command data */
    memcpy(bufptr, cmd_ptr, cmd_stream.bytes_written);
    bufptr += cmd_stream.bytes_written;

    /* Copy in the message data */
    memcpy(bufptr, msg_ptr, msg_stream.bytes_written);
    bufptr += msg_stream.bytes_written;

    /* Note: The bufptr is now at the CRC location */

    /* Calculate CRC / Parity16 */
    if (iri_device->protocol_flags & IPJ_FLAG_ENABLE_CHECKSUM_MASK)
    {
        if (iri_device->protocol_flags & IPJ_FLAG_CHECKSUM_ALGORITHM_MASK)
        {
            // CRC - Invert CRC as per GEN2 specification
            crc = ~ipj_internal_calculate_crc(
            iri_device->transmit_buffer,
            (uint16_t) transmit_length - IPJ_FRAME_CRC_SIZE);
        }
        else
        {
            // Parity
            crc = ipj_internal_calculate_parity_16(
                    iri_device->transmit_buffer,
                    (uint16_t) transmit_length - IPJ_FRAME_CRC_SIZE);
        }
    }
    else
    {
        crc = 0x0000;
    }

    /* Append CRC to transmit buffer */
    *(bufptr++) = (uint8_t) (crc >> 8);
    *(bufptr++) = (uint8_t) ((crc >> 0) & 0xff);

    if (!iri_device->platform_transmit_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

#if defined(DEBUG_SERIAL_TRAFFIC)
    printf("Write [%d] ", transmit_length);
    uint32_t i;
    for(i = 0; i < transmit_length; i++)
    {
        printf("%02X ", iri_device->transmit_buffer[i]);
    }
    printf("\n");
#endif

    /* Transmit packet */
    result = iri_device->platform_transmit_handler(
            iri_device->platform_transmit_args,
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
        ipj_iri_device                  *iri_device,
        const pb_field_t                *cmd_field_desc,
        void                            *command,
        void                            *handler_args,
        ipj_internal_completion_handler  handler)
{
    ipj_error error;
    uint32_t current_time_ms;
    uint32_t start_time_ms;
    uint32_t response_id = 0;
    ipj_rr_union rr_union = {{0}};

    if (!iri_device->platform_timestamp_ms_handler
            || !iri_device->platform_receive_handler)
    {
        return E_IPJ_ERROR_API_NO_HANDLER;
    }

    /* Transmit command */
    error = ipj_internal_transmit_packet(iri_device, cmd_field_desc, command);
    if (error)
    {
        return error;
    }

    /* If responses are suppressed, just return a success */
    if ((iri_device->protocol_flags & IPJ_FLAG_SUPPRESS_SET_RESPONSES)
            && cmd_field_desc->tag
                    == Command_fields[COMMAND_FIELDS_INDEX_BULK_SET].tag)
    {
        return E_IPJ_ERROR_SUCCESS;
    }

    /* Set time to timeout - current time pulse timeout */
    start_time_ms = iri_device->platform_timestamp_ms_handler(
                        iri_device->platform_timestamp_ms_args);

    /* Receive response */
    /* DO-WHILE loop until response received (if waiting for response),
     * otherwise only loop once (unless shifting bits for synchronization purposes) */
    do
    {
        /* Get current time */
        current_time_ms = iri_device->platform_timestamp_ms_handler(
                                iri_device->platform_timestamp_ms_args);

        /* Check if current time beyond timeout time */
        if (current_time_ms - start_time_ms >= iri_device->receive_timeout_ms)
        {
            ipj_internal_reset_rx_state_machine(iri_device);
            return E_IPJ_ERROR_API_CONNECTION_READ_TIMEOUT;
        }

        error = ipj_internal_receive(iri_device, &rr_union, &response_id);
        if (error)
        {
            return error;
        }

    } while (!response_id);

    /*
     * Check for expected response.
     * Response id is the same as command id.
     */
    if (response_id == cmd_field_desc->tag)
    {
        /* Special handling for flash since it has error in a different position */
        if (response_id == Response_fields[RESPONSE_FIELDS_INDEX_FLASH].tag)
        {
            /* Cast receive buffer to set response */
            if (rr_union.flash_resp.has_error)
            {
                if (rr_union.flash_resp.error != E_IPJ_ERROR_SUCCESS)
                {
                    error = (ipj_error) rr_union.flash_resp.error;
                    return error;
                }
            }
        }
        else if (rr_union.generic_resp.has_error)
        {
            if (rr_union.generic_resp.error != E_IPJ_ERROR_SUCCESS)
            {
                return rr_union.generic_resp.error;
            }
        }
        else if (handler)
        {
            /* Call a handler to finish processing the response */
            return handler(&rr_union, handler_args);
        }
    }
    else if (response_id == Response_fields[COMMAND_FIELDS_INDEX_INVALID].tag)
    {
        /* Catch an InavlidResponse */
        if (rr_union.generic_resp.has_error)
        {
            if (rr_union.generic_resp.error != E_IPJ_ERROR_SUCCESS)
            {
                return rr_union.generic_resp.error;
            }
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

static ipj_error ipj_internal_process_packet(
        ipj_iri_device *iri_device,
        uint32_t        payload_length,
        ipj_rr_union   *rr_union,
        uint32_t       *response_id
        )
{
    ipj_error error = E_IPJ_ERROR_COMMAND_DECODE_FAILURE;
    pb_istream_t istream;
    uint64_t tmp;
    uint32_t field_index = 0;
    uint32_t response_type;
    uint32_t rr_id;
    const pb_field_t *pb_descriptor;

    /* Setup stream for protobuf decode */
    istream = pb_istream_from_buffer(
            &iri_device->receive_buffer[IPJ_FRAME_HEADER_SIZE],
            (size_t) (payload_length));

    /* Strip off packet layer */
    pb_decode_varint(&istream, &tmp);
    response_type = (uint32_t) FIELD_ID(tmp);
    pb_decode_varint(&istream, &tmp);

    /* Strip off response/report layer */
    pb_decode_varint(&istream, &tmp);
    rr_id = (uint32_t) FIELD_ID(tmp);
    pb_decode_varint(&istream, &tmp);

    /* Check packet type */
    if (response_type == Packet_fields[PACKET_FIELDS_INDEX_RESPONSE].tag)
    {
        *response_id = rr_id;
        pb_descriptor = Response_fields;
    }
    else
    {
        pb_descriptor = Report_fields;
    }

    /* Process packet */
    while (pb_descriptor[field_index].tag != 0)
    {
        /* Find the matching field descriptor */
        if (pb_descriptor[field_index].tag == rr_id)
        {
            /* Descriptor found. Perform protobuf decode */
            if (pb_decode(
                    &istream,
                    (const pb_field_t*) pb_descriptor[field_index].ptr,
                    rr_union))
            {
                if (response_type == Packet_fields[PACKET_FIELDS_INDEX_REPORT].tag
                        && iri_device->report_handler)
                {
                    /* This was a report, call the handler */
                    error = iri_device->report_handler(
                            iri_device->report_args,
                            iri_device,
                            rr_id,
                            (void*) rr_union);
                }

                /* Successful decode */
                error = E_IPJ_ERROR_SUCCESS;
                break;
            }
        }
        field_index++;
    }

    return error;
}

static void ipj_internal_reset_rx_state_machine(ipj_iri_device* iri_device)
{
    /* Reset receive index, flushes buffer */
    iri_device->receive_index = 0;
    iri_device->sync_state    = E_SEEKING_FRAME_SYNC;
    iri_device->frame_length  = IPJ_FRAME_HEADER_SIZE;
}

static bool ipj_internal_test_frame_sync(ipj_iri_device* iri_device)
{
    bool frame_sync;
    bool checksum_match;
    uint8_t result;
    const uint8_t ipj_header_marker[] =
    { 0x8D, 0x70, 0x6A, 0x21, 0x02 };

    /* Check for frame sync */
    result = (uint8_t) memcmp(
            iri_device->receive_buffer,
            ipj_header_marker,
            sizeof(ipj_header_marker));
    frame_sync = (result == 0);

    /* Check checksum */
    result = ipj_internal_calculate_parity_8(
            iri_device->receive_buffer,
            IPJ_FRAME_HEADER_SIZE);
    checksum_match = (result == 0);

    frame_sync &= checksum_match;
    return frame_sync;
}

static uint32_t ipj_internal_get_frame_length(ipj_iri_device* iri_device)
{
    uint32_t payload_length;

    /* Calculate payload length */
    payload_length = (iri_device->receive_buffer[6] << 8)
            | (iri_device->receive_buffer[7] << 0);

    return payload_length;
}

ipj_error ipj_internal_receive(
        ipj_iri_device* iri_device,
        ipj_rr_union* rr_union,
        uint32_t * response_id)
{
    /*Don't assume something bad happened */
    ipj_error error = E_IPJ_ERROR_SUCCESS;
    uint16_t read_length;

    if (iri_device->receive_index < iri_device->frame_length)
    {
        /* Read remaining bits in packet */
        iri_device->platform_receive_handler(
                iri_device->platform_receive_args,
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
                uint32_t frame_length = ipj_internal_get_frame_length(iri_device);

                /* At this point, we have enough data for a complete header
                 * Calculate the payload length and jump to next state */
                if (ipj_internal_test_frame_sync(iri_device) && frame_length <= IPJ_RECEIVE_BUFFER_SIZE)
                {
                    iri_device->sync_state = E_GOT_FRAME_SYNC;
                    iri_device->frame_length = frame_length;
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
                uint16_t calc_crc;
                uint16_t crc_match;
                uint16_t crc_index = iri_device->receive_index - IPJ_FRAME_CRC_SIZE;
                uint16_t pkt_crc = iri_device->receive_buffer[crc_index] << 8 | iri_device->receive_buffer[crc_index + 1];

#if defined(DEBUG_SERIAL_TRAFFIC)
                uint32_t i;
                printf("Read  [%d] ", iri_device->receive_index);
                for(i = 0; i < iri_device->receive_index; i++)
                {
                    printf("%02X ", iri_device->receive_buffer[i]);
                }
                printf("\n");
#endif

                /* Check whether we should be using parity16 or CRC */
                if (iri_device->receive_buffer[IPJ_HEADER_FLAGS_OFFSET] & IPJ_FLAG_ENABLE_CHECKSUM_MASK)
                {
                    if (iri_device->receive_buffer[IPJ_HEADER_FLAGS_OFFSET] & IPJ_FLAG_CHECKSUM_ALGORITHM_MASK)
                    {
                        // CRC - Check residue as per GEN2 spec
                        calc_crc = ipj_internal_calculate_crc(
                                iri_device->receive_buffer,
                                iri_device->frame_length);
                        crc_match = (calc_crc == GEN2_CRC_RESIDUE);
                    }
                    else
                    {
                        // Parity16
                        calc_crc = ipj_internal_calculate_parity_16(
                                iri_device->receive_buffer,
                                iri_device->frame_length - IPJ_FRAME_CRC_SIZE);
                        crc_match = (pkt_crc == calc_crc);
                    }
                }
                else
                {
                    crc_match = 1;
                }

                if (crc_match)
                {
                    /* If we're using sync counting, check to make sure we're on the correct frame */
                    if (iri_device->receive_buffer[IPJ_HEADER_FLAGS_OFFSET]
                                                   & IPJ_FLAG_ENABLE_SYNC_COUNT_MASK)
                    {
                        uint8_t tmp8;
                        tmp8 = iri_device->receive_buffer[IPJ_HEADER_FRAME_SYNC_OFFSET];

                        /* Propagate device reset flag if it exists */
                        iri_device->protocol_flags |= (IPJ_FLAG_DEVICE_RESET_SYNC_MASK & tmp8);

                        if (tmp8 != iri_device->rx_frame_sync_count &&
                                !(iri_device->receive_buffer[IPJ_HEADER_FLAGS_OFFSET] & IPJ_FLAG_RESET_SYNC_COUNT_MASK))
                        {
                            if(iri_device->diagnostic_handler)
                            {
                                iri_device->diagnostic_handler(
                                    iri_device->diagnostic_args,
                                    iri_device,
                                    E_IPJ_ERROR_IRI_FRAME_DROPPED);
                            }

                            iri_device->rx_frame_sync_count = tmp8 + 1;
                        }
                        else
                        {
                            iri_device->rx_frame_sync_count++;
                        }
                    }

                    error = ipj_internal_process_packet(
                            iri_device,
                            iri_device->frame_length - IPJ_FRAME_HEADER_SIZE - IPJ_FRAME_CRC_SIZE,
                            rr_union,
                            response_id);

                }
                else
                {
                    error = E_IPJ_ERROR_IRI_FRAME_INVALID;
                }

                ipj_internal_reset_rx_state_machine(iri_device);
                break;
            }
        }
    }

    return error;
}

static ipj_error ipj_flash_handler(ipj_rr_union *rr_union, void* args)
{
    uint8_t *data = (uint8_t*) args;

    /* Return any data supplied by the response */
    if (rr_union->flash_resp.has_data)
    {
        if (!data)
        {
            return E_IPJ_ERROR_VALUE_INVALID;
        }
        memcpy(
                data,
                rr_union->flash_resp.data.bytes,
                rr_union->flash_resp.data.size);
    }
    return E_IPJ_ERROR_SUCCESS;
}

ipj_error ipj_internal_flash_operation(
        ipj_iri_device *iri_device,
        ipj_flash_operation action,
        uint32_t offset,
        uint32_t len,
        uint8_t *data)
{
    FlashCommand command = {0};

    RETURN_ERROR_ON_INVALID_DEVICE();

    /* Transmit command */

    /* Set SET command parameters */
    command.has_action = true;
    command.action = action;
    command.has_address = true;
    command.address = offset;

    /* Decide if the packet has data */
    switch (action)
    {
        case E_IPJ_FLASH_OPERATION_WRITE:
        {
            /*
             * Error if the array size provided is out of range
             */
            if (len > ARRAY_SIZE(command.data.bytes))
            {
                return E_IPJ_ERROR_API_INVALID_PARAMETER;
            }

            command.has_data = true;
            command.data.size = len;
            memcpy(command.data.bytes, data, len);
            break;
        }
        case E_IPJ_FLASH_OPERATION_READ:
        {
            /*
             * Error if the array size provided is out of range
             * FlashCommand and FlashResponse have the same data size.
             */
            if (len > ARRAY_SIZE(command.data.bytes))
            {
                return E_IPJ_ERROR_API_INVALID_PARAMETER;
            }
        }
        /* Fall through */
        case E_IPJ_FLASH_OPERATION_ERASE:
        {
            command.has_length = true;
            command.length = len;
        }
        /* Fall through */
        case E_IPJ_FLASH_OPERATION_UNLOCK:
        {
            command.has_data = false;
            break;
        }
    }

    return ipj_internal_transact(
            iri_device,
            &Command_fields[COMMAND_FIELDS_INDEX_FLASH],
            &command,
            data,
            &ipj_flash_handler);

}

ipj_error ipj_flash_write(
        ipj_iri_device *iri_device,
        uint32_t offset,
        uint32_t len,
        uint8_t* data)
{
    return ipj_internal_flash_operation(
            iri_device,
            E_IPJ_FLASH_OPERATION_WRITE,
            offset,
            len,
            data);
}

ipj_error ipj_flash_read(
        ipj_iri_device *iri_device,
        uint32_t offset,
        uint32_t len,
        uint8_t* dest)
{
    return ipj_internal_flash_operation(
            iri_device,
            E_IPJ_FLASH_OPERATION_READ,
            offset,
            len,
            dest);
}

ipj_error ipj_flash_erase(ipj_iri_device *iri_device, uint32_t offset, uint32_t len)
{
    return ipj_internal_flash_operation(
            iri_device,
            E_IPJ_FLASH_OPERATION_ERASE,
            offset,
            len,
            NULL);
}

/**
 * This function takes in an IRI_Loader chunk and processes it accordingly.
 *
 * \return ipj_error
 */
ipj_error ipj_flash_handle_loader_block(
        ipj_iri_device *iri_device,  /**< [in] IRI device data structure */
        uint32_t len,                /**< [in] Length of the data chunk (in bytes) */
        uint8_t* data)               /**< [in] Pointer to the data chunk to be processed */
{
    uint32_t offset;
    uint32_t payload_len;
    uint16_t crc_residue;
    uint32_t chunk_no;

    if (!data)
    {
        return E_IPJ_ERROR_VALUE_INVALID;
    }

    chunk_no    = data[0] | data[1] << 8 | (uint32_t)(data[2] ) << 16 | (uint32_t)(data[3] ) << 24;
    offset      = data[4] | data[5] << 8 | (uint32_t)(data[6] ) << 16 | (uint32_t)(data[7] ) << 24;
    payload_len = data[8] | data[9] << 8 | (uint32_t)(data[10]) << 16 | (uint32_t)(data[11]) << 24;

    /* Make sure this is a reasonable data chunk: payload_len + 14 should equal
     * len */
    if ((payload_len + LOADER_BLOCK_HEADER_SIZE + IPJ_FRAME_CRC_SIZE) != len && chunk_no > 0)
    {
        return E_IPJ_ERROR_VALUE_INVALID;
    }

    /* Check if the CRC is valid */
    crc_residue = ipj_internal_calculate_crc(
            (uint8_t*) &data[LOADER_BLOCK_HEADER_SIZE],
            len - LOADER_BLOCK_HEADER_SIZE);

    /* If residue isn't valid, bail */
    if (crc_residue != GEN2_CRC_RESIDUE)
    {
        return E_IPJ_ERROR_API_INVALID_LOADER_BLOCK;
    }

    /* This is the erase block */
    if (chunk_no == 0)
    {
        return ipj_flash_erase(iri_device, offset, payload_len);
    }
    else
    {
        /* Write the flash block to the appropriate location */
        return ipj_flash_write(
                iri_device,
                offset,
                payload_len,
                &data[LOADER_BLOCK_HEADER_SIZE]);
    }

}

/* CRC-16 value table */
static const uint16_t crc16_table[] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static uint16_t ipj_internal_calculate_crc(uint8_t* data, uint16_t data_length)
{
    uint16_t i = 0;
    uint16_t crc = IPJ_CRC16_PRELOAD;

    for (i = 0; i < data_length; i++, data++)
    {
        uint8_t tmp = *data ^ ((crc >> 8) & 0xFF);
        crc = (crc << 8) ^ crc16_table[tmp];
    }

    return crc;
}
