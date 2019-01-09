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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "platform.h"
#include "iri.h"


/* The windows/IRI baud rate enums dont match up to Linux, so we need
 * a translation table */
static uint32_t baudrate_lookup(uint32_t iri_baud)
{
    switch (iri_baud)
    {
        case E_IPJ_BAUD_RATE_BR9600:
            return B9600;
        case E_IPJ_BAUD_RATE_BR19200:
            return B19200;
        case E_IPJ_BAUD_RATE_BR38400:
            return B38400;
        case E_IPJ_BAUD_RATE_BR57600:
            return B57600;
        case E_IPJ_BAUD_RATE_BR115200:
            return B115200;
        case E_IPJ_BAUD_RATE_BR230400:
            return B230400;
        case E_IPJ_BAUD_RATE_BR460800:
            return B460800;
        case E_IPJ_BAUD_RATE_BR921600:
            return B921600;
    }

    return 0;
}

/* PLATFORM_OPEN_PORT_HANDLER */
uint32_t platform_open_port_handler(IPJ_READER_CONTEXT* reader_context,
                                    IPJ_READER_IDENTIFIER reader_identifier,
                                    ipj_connection_type connection_type,
                                    ipj_connection_params* params)
{
    struct termios tio;
    int tty_fd;
    uint32_t baudrate_constant;

    /* Ensure the intended connection type is serial */
    if (connection_type != E_IPJ_CONNECTION_TYPE_SERIAL)
    {
        return 0;
    }

    memset(&tio, 0, sizeof(tio));

    tty_fd = open(reader_identifier, O_RDWR | O_NOCTTY);

    /* If FD is bad, we bail */
    if (tty_fd < 0)
    {
        return 0;
    }

    /* Enable at desired baudrate, 8N1, no modem control, able to rx */
    baudrate_constant = baudrate_lookup(params->serial.baudrate);
    tio.c_cflag = baudrate_constant | CS8 | CLOCAL | CREAD;

    /* Raw output */
    tio.c_oflag = 0;

    /* non-canonical input */
    tio.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);

    /* No inter-character timer */
    tio.c_cc[VTIME] = 0;

    /* Completely non-blocking input, no minimum char count */
    tio.c_cc[VMIN] = 0;

    /* flush the port input */
    tcflush(tty_fd, TCIFLUSH);

    /* Set port parameters */
    tcsetattr(tty_fd, TCSANOW, &tio);

    *reader_context = (IPJ_READER_CONTEXT)(intptr_t)tty_fd;

    return 1;
}

/* PLATFORM_CLOSE_PORT_HANDLER */
uint32_t platform_close_port_handler(IPJ_READER_CONTEXT reader_context)
{
    if (close((intptr_t)reader_context))
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
uint32_t platform_transmit_handler(IPJ_READER_CONTEXT reader_context,
                                   uint8_t* message_buffer,
                                   uint16_t  buffer_size,
                                   uint16_t* number_bytes_transmitted)
{
    int bytes_written;
    bytes_written = write((intptr_t)reader_context,
                          message_buffer,
                          (intptr_t)buffer_size);

    if (bytes_written >= 0)
    {
        *number_bytes_transmitted = bytes_written;
        return 1;
    }

    return 0;
}

/* PLATFORM_RECEIVE_HANDLER */
uint32_t platform_receive_handler(IPJ_READER_CONTEXT reader_context,
                                  uint8_t* message_buffer,
                                  uint16_t buffer_size,
                                  uint16_t* number_bytes_received,
                                  uint16_t timeout_ms)
{
    int bytes_recvd;

    /* IRI's framer provides a per-transaction timeout, so we can
     * just operate in non-blocking mode and return nothing (provided
     * there are no errors) */
    (void)timeout_ms;

    bytes_recvd = read((intptr_t)reader_context,
                       message_buffer,
                       (intptr_t)buffer_size);

    if (bytes_recvd >= 0)
    {
        *number_bytes_received = bytes_recvd;
        return 1;
    }

    return 0;

}

/* PLATFORM_TIMESTAMP_HANDLER */
uint32_t platform_timestamp_ms_handler()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
}

void platform_sleep_ms_handler(uint32_t milliseconds)
{
    usleep(milliseconds * 1000);
}

uint32_t platform_flush_port_handler(IPJ_READER_CONTEXT reader_context)
{
    /* flush the port input */
    if (tcflush((intptr_t)reader_context, TCIOFLUSH))
    {
        return 1;
    }

    return 0;
}

static uint32_t platform_flow_control(IPJ_READER_CONTEXT reader_context,
                                      bool enable,
                                      uint32_t pin)
{
    int status;

    ioctl((intptr_t)reader_context, TIOCMGET, &status);

    if (enable)
    {
        status |= pin;
    }
    else
    {
        status &= ~pin;
    }

    ioctl((intptr_t)reader_context, TIOCMSET, &status);

    return 0;
}

/* Used to control the DTR pin on the FTDI chip */
uint32_t platform_reset_pin_handler(IPJ_READER_CONTEXT reader_context,
                                    bool enable)
{
    return platform_flow_control(reader_context, enable, TIOCM_DTR);
}

/* Used to control the RTS pin on the FTDI chip */
uint32_t platform_wakeup_pin_handler(IPJ_READER_CONTEXT reader_context,
                                     bool enable)
{
    return platform_flow_control(reader_context, enable, TIOCM_DTR);
}

uint32_t platform_modify_connection_handler(IPJ_READER_CONTEXT reader_context,
                                            ipj_connection_type connection_type,
                                            ipj_connection_params* params)
{
    struct termios tio;
    uint32_t baudrate_constant;
    (void)connection_type;

    baudrate_constant = baudrate_lookup(params->serial.baudrate);
    tcgetattr((intptr_t)reader_context, &tio);
    cfsetispeed(&tio, baudrate_constant);
    cfsetospeed(&tio, baudrate_constant);

    tcsetattr((intptr_t)reader_context, TCSANOW, &tio);

    return 0;
}
