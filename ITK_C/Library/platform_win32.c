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

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "iri.h"

/* PLATFORM_OPEN_PORT_HANDLER */
uint32_t platform_open_port_handler(IPJ_READER_CONTEXT* reader_context,
                                    IPJ_READER_IDENTIFIER reader_identifier,
                                    ipj_connection_type connection_type,
                                    ipj_connection_params* params)
{
    HANDLE h_port;
    DCB dcb;
    COMMTIMEOUTS commtimeouts;

    uint32_t result;
    char port_name[30];

    /* we currently only support serial */
    (void)connection_type;

    /* Map reader_identifier to COM port name */
#if defined(__GNUC__)
    strcpy(port_name, "\\\\.\\");
    strcat(port_name, (char*)reader_identifier);
#else /* Visual Studio/Intel/etc */
    strcpy_s(port_name, sizeof(port_name), "\\\\.\\");
    strcat_s(port_name, sizeof(port_name), (char*)reader_identifier);
#endif

    /* Open serial port */
    h_port = CreateFileA(port_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    /* Return error if invalid handle */
    if ((h_port == INVALID_HANDLE_VALUE))
    {
        return 0;
    }

    /* Get default serial port parameters */
    dcb.DCBlength = sizeof(dcb);
    result = GetCommState(h_port, &dcb);

    /* Return error if unsuccessful getting serial port parameters */
    if (!result)
    {
        return 0;
    }

    /* Modify serial port parameters */
    if(!params)
    {
        return 0;
    }
    else
    {
        dcb.BaudRate = params->serial.baudrate;
        switch(params->serial.parity)
        {
            case E_IPJ_PARITY_PODD:
            {
                dcb.Parity = ODDPARITY;
                break;
            }
            case E_IPJ_PARITY_PEVEN:
            {
                dcb.Parity = EVENPARITY;
                break;
            }
            default: /* Fall through */
            case E_IPJ_PARITY_PNONE:
            {
                dcb.Parity = NOPARITY;
                break;
            }
        }
    }
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.fParity  = false;
    dcb.fOutX    = false;
    dcb.fInX     = false;
    dcb.fNull    = false;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    /* Set serial port parameters */
    result = SetCommState(h_port, &dcb);

    /* Return error if unsuccessful setting serial port parameters */
    if (!result)
    {
        return 0;
    }

    /* Get timeout values */
    result = GetCommTimeouts(h_port, &commtimeouts);

    // Return error if unsuccessful getting serial port timeout parameters */
    if (!result)
    {
        return 0;
    }

    /* Modify timeout values */
    commtimeouts.ReadIntervalTimeout         = 0;
    commtimeouts.ReadTotalTimeoutConstant    = PLATOFRM_DEFAULT_READ_TIMEOUT_MS;
    commtimeouts.ReadTotalTimeoutMultiplier  = 0;
    commtimeouts.WriteTotalTimeoutConstant   = PLATFORM_DEFAULT_WRITE_TIMEOUT_MS;
    commtimeouts.WriteTotalTimeoutMultiplier = 0;

    /* Set timeout values */
    result = SetCommTimeouts(h_port, &commtimeouts);

    if(!result)
    {
        return 0;
    }

    /* Assign serial port handle to reader context */
    *reader_context = (IPJ_READER_CONTEXT)h_port;
    return 1;
}

/* PLATFORM_CLOSE_PORT_HANDLER */
uint32_t platform_close_port_handler(IPJ_READER_CONTEXT reader_context)
{
    uint32_t result;

    /* Close reader handle */
    result = CloseHandle((HANDLE)reader_context);

    if(!result)
    {
        return 0;
    }

    return 1;
}

/*
 * PLATFORM_TRANSMIT_HANDLER
 *
 * Return: 1: Success, 0:Fail
 */
uint32_t platform_transmit_handler(IPJ_READER_CONTEXT reader_context, uint8_t* message_buffer, uint16_t  buffer_size,
                                   uint16_t* number_bytes_transmitted)
{
    uint32_t result;
    DWORD dw_bytes_written = 0;

    /* Write serial port */
    result = WriteFile((HANDLE)reader_context, message_buffer, buffer_size, &dw_bytes_written, NULL);
    if (!result)
    {
        /* Fail */
        return 0;
    }
    else
    {
        /* Success */
        /* Assign number of bytes transmitted */
        *number_bytes_transmitted = (uint16_t)dw_bytes_written;
        return 1;
    }
}

/* PLATFORM_RECEIVE_HANDLER */
uint32_t platform_receive_handler(IPJ_READER_CONTEXT reader_context, uint8_t* message_buffer, uint16_t buffer_size,
                                  uint16_t* number_bytes_received, uint16_t timeout_ms)
{
    uint32_t result;
    DWORD dw_bytes_received = 0;
    COMMTIMEOUTS commtimeouts;

    /* Get timeout values */
    result = GetCommTimeouts((HANDLE)reader_context, &commtimeouts);

    /* Return error if unsuccessful getting serial port timeout parameters */
    if (!result)
    {
        return 0;
    }

    /* Modify timeout values */
    if (timeout_ms == 0)
    {
        commtimeouts.ReadIntervalTimeout = MAXDWORD;
        commtimeouts.ReadTotalTimeoutConstant = 1;
    }
    else
    {
        commtimeouts.ReadIntervalTimeout = timeout_ms;
        commtimeouts.ReadTotalTimeoutConstant = 1;
    }

    /* Set timeout values */
    result = SetCommTimeouts((HANDLE)reader_context, &commtimeouts);

    /* Return if error if unsuccessful setting timeout parameters */
    if(!result)
    {
        return 0;
    }

    /* Read serial port */
    result = ReadFile((HANDLE)reader_context, message_buffer, buffer_size, &dw_bytes_received, NULL);
    if (result)
    {
        /* Assign bytes received */
        *number_bytes_received = (uint16_t)dw_bytes_received;
         return 1;
    }
    else
    {
        return 0;
    }
}

/* PLATFORM_TIMESTAMP_HANDLER */
uint32_t platform_timestamp_ms_handler()
{
    DWORD timestamp_ms = timeGetTime();

    return (uint32_t)timestamp_ms;
}

void platform_sleep_ms_handler(uint32_t milliseconds)
{
    Sleep(milliseconds);
}

uint32_t platform_flush_port_handler(IPJ_READER_CONTEXT reader_context)
{
    int result;

    result = PurgeComm((HANDLE)reader_context, PURGE_RXCLEAR | PURGE_TXCLEAR);

    return result == 0 ? 1 : 0;
}

uint32_t platform_reset_pin_handler(IPJ_READER_CONTEXT reader_context,
                                    bool enable)
{
    int action = enable ? SETDTR : CLRDTR;
    int result = EscapeCommFunction((HANDLE)reader_context, action);
    return result == 0 ? 1 : 0;
}

uint32_t platform_wakeup_pin_handler(IPJ_READER_CONTEXT reader_context,
                                     bool enable)
{
    int action = enable ? SETRTS : CLRRTS;
    int result = EscapeCommFunction((HANDLE)reader_context, action);
    return result == 0 ? 1 : 0;
}


uint32_t platform_modify_connection_handler(IPJ_READER_CONTEXT reader_context,
                                            ipj_connection_type connection_type,
                                            ipj_connection_params* params)
{
    DCB dcb;
    int result;

    switch(connection_type)
    {
        /* For all serial operations */
        case E_IPJ_CONNECTION_TYPE_SERIAL:
        {
            /* Get serial port parameters */
            result = GetCommState((HANDLE)reader_context, &dcb);
            if (!result)
            {
                return 1;
            }
            /* Set the new baudrate */
            dcb.BaudRate = (DWORD)params->serial.baudrate;

            switch(params->serial.parity)
            {
                case E_IPJ_PARITY_PODD:
                {
                    dcb.Parity = ODDPARITY;
                    break;
                }
                case E_IPJ_PARITY_PEVEN:
                {
                    dcb.Parity = EVENPARITY;
                    break;
                }
                default: /* Fall through */
                case E_IPJ_PARITY_PNONE:
                {
                    dcb.Parity = NOPARITY;
                    break;
                }
            }

            result = SetCommState((HANDLE)reader_context, &dcb);
            if (!result)
            {
                return 1;
            }
            return 0;
        }
        default:
        {
            return 0;
        }
    }
}
