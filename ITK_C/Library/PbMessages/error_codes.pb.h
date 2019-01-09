/*
 *****************************************************************************
 * Copyright 2016-2018 Impinj, Inc.                                          *
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
/* Automatically generated nanopb header */
#ifndef _PB_ERROR_CODES_PB_H_
#define _PB_ERROR_CODES_PB_H_
#include <pb.h>


/* Enum definitions */
typedef uint32_t ipj_error;
#define E_IPJ_ERROR_SUCCESS ((uint32_t)(0x0))
#define E_IPJ_ERROR_GENERAL_ERROR ((uint32_t)(0x1))
#define E_IPJ_ERROR_SET_KEY_INVALID ((uint32_t)(0x2))
#define E_IPJ_ERROR_SET_KEY_READ_ONLY ((uint32_t)(0x3))
#define E_IPJ_ERROR_SET_KEY_OUT_OF_RANGE ((uint32_t)(0x4))
#define E_IPJ_ERROR_GET_KEY_INVALID ((uint32_t)(0x5))
#define E_IPJ_ERROR_GET_KEY_WRITE_ONLY ((uint32_t)(0x6))
#define E_IPJ_ERROR_COMMAND_INVALID ((uint32_t)(0x7))
#define E_IPJ_ERROR_COMMAND_START_FAILURE ((uint32_t)(0x8))
#define E_IPJ_ERROR_COMMAND_DECODE_FAILURE ((uint32_t)(0x9))
#define E_IPJ_ERROR_COMMAND_ENCODE_FAILURE ((uint32_t)(0xA))
#define E_IPJ_ERROR_COMMAND_STALLED ((uint32_t)(0xB))
#define E_IPJ_ERROR_VALUE_INVALID ((uint32_t)(0xC))
#define E_IPJ_ERROR_MORE_THAN_ONE_COMMAND_RECEIVED ((uint32_t)(0xD))
#define E_IPJ_ERROR_NOT_IMPLEMENTED ((uint32_t)(0xE))
#define E_IPJ_ERROR_INVALID_PRODUCT_CONFIGURATION ((uint32_t)(0xF))
#define E_IPJ_ERROR_INVALID_FACTORY_SETTINGS ((uint32_t)(0x10))
#define E_IPJ_ERROR_RESPONSE_ENCODE_FAILURE ((uint32_t)(0x11))
#define E_IPJ_ERROR_COMMAND_VERIFY_FAILURE ((uint32_t)(0x12))
#define E_IPJ_ERROR_INTERNAL_NON_RECOVERABLE ((uint32_t)(0x13))
#define E_IPJ_ERROR_TEMPLATE_DECODE_FAILURE ((uint32_t)(0x14))
#define E_IPJ_ERROR_SYSTEM_IN_ERROR_STATE ((uint32_t)(0x15))
#define E_IPJ_ERROR_TEST_ERROR ((uint32_t)(0x16))
#define E_IPJ_ERROR_STORED_SETTING_DECODE ((uint32_t)(0x17))
#define E_IPJ_ERROR_VALUE_INDEX_OUT_OF_RANGE ((uint32_t)(0x18))
#define E_IPJ_ERROR_BANK_INDEX_OUT_OF_RANGE ((uint32_t)(0x19))
#define E_IPJ_ERROR_INVALID_PRODUCT_CALIBRATION ((uint32_t)(0x1A))
#define E_IPJ_ERROR_REPORT_SIZE_WOULD_OVERFLOW ((uint32_t)(0x1B))
#define E_IPJ_ERROR_FIXED_HARDWARE_SETTINGS_LOAD_ERROR ((uint32_t)(0x1C))
#define E_IPJ_ERROR_GEN2_TAG_OTHER_ERROR ((uint32_t)(0x1000001))
#define E_IPJ_ERROR_GEN2_TAG_MEMORY_OVERRUN ((uint32_t)(0x1000002))
#define E_IPJ_ERROR_GEN2_TAG_MEMORY_LOCKED ((uint32_t)(0x1000003))
#define E_IPJ_ERROR_GEN2_TAG_INSUFFICIENT_POWER ((uint32_t)(0x1000004))
#define E_IPJ_ERROR_GEN2_TAG_NON_SPECIFIC_ERROR ((uint32_t)(0x1000005))
#define E_IPJ_ERROR_API_DEVICE_NOT_INITIALIZED ((uint32_t)(0x2000001))
#define E_IPJ_ERROR_API_SERIAL_PORT_ERROR ((uint32_t)(0x2000002))
#define E_IPJ_ERROR_API_CONNECTION_READ_TIMEOUT ((uint32_t)(0x2000003))
#define E_IPJ_ERROR_API_CONNECTION_WRITE_TIMEOUT ((uint32_t)(0x2000004))
#define E_IPJ_ERROR_API_CONNECTION_WRITE_ERROR ((uint32_t)(0x2000005))
#define E_IPJ_ERROR_API_RX_BUFF_TOO_SMALL ((uint32_t)(0x2000006))
#define E_IPJ_ERROR_API_MESSAGE_INVALID ((uint32_t)(0x2000007))
#define E_IPJ_ERROR_API_NO_HANDLER ((uint32_t)(0x2000008))
#define E_IPJ_ERROR_API_INVALID_LOADER_BLOCK ((uint32_t)(0x2000009))
#define E_IPJ_ERROR_API_RESPONSE_MISMATCH ((uint32_t)(0x200000A))
#define E_IPJ_ERROR_API_INVALID_PARAMETER ((uint32_t)(0x200000B))
#define E_IPJ_ERROR_API_NON_LT_PACKET_DETECTED ((uint32_t)(0x200000C))
#define E_IPJ_ERROR_IRI_FRAME_DROPPED ((uint32_t)(0x3000001))
#define E_IPJ_ERROR_IRI_FRAME_INVALID ((uint32_t)(0x3000002))
#define E_IPJ_ERROR_MAC_GENERAL ((uint32_t)(0x4000001))
#define E_IPJ_ERROR_MAC_CRC_MISMATCH ((uint32_t)(0x4000002))
#define E_IPJ_ERROR_MAC_NO_TAG_RESPONSE ((uint32_t)(0x4000003))
#define E_IPJ_ERROR_MAC_TAG_LOST ((uint32_t)(0x4000004))
#define E_IPJ_ERROR_BTS_DEVICE_WATCHDOG_RESET ((uint32_t)(0x5000001))
#define E_IPJ_ERROR_BTS_VALUE_INVALID ((uint32_t)(0x5000002))
#define E_IPJ_ERROR_BTS_FLASH_WRITE ((uint32_t)(0x5000003))
#define E_IPJ_ERROR_BTS_FLASH_READ ((uint32_t)(0x5000004))
#define E_IPJ_ERROR_BTS_FLASH_ADDRESS ((uint32_t)(0x5000005))
#define E_IPJ_ERROR_BTS_FLASH_ERASE ((uint32_t)(0x5000006))
#define E_IPJ_ERROR_BTS_UNKNOWN_COMMAND ((uint32_t)(0x5000007))
#define E_IPJ_ERROR_BTS_COMMAND_DECODE_FAILURE ((uint32_t)(0x5000008))
#define E_IPJ_ERROR_TRANSCEIVER_FAILURE ((uint32_t)(0x6000001))
#define E_IPJ_ERROR_LIMIT_PA_TEMPERATURE_MAX ((uint32_t)(0x7000001))

/* Struct definitions */
/* Default values for struct fields */

/* Struct field encoding specification for nanopb */

#endif
