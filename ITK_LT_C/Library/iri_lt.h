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

#ifndef __IRI_H
#define __IRI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif

// *****************************************************************************
// IRI API - Defines
// *****************************************************************************
typedef uint32_t ipj_report_id;
#define E_IPJ_REPORT_ID_TAG_OPERATION_REPORT ((ipj_report_id)(0x2))
#define E_IPJ_REPORT_ID_STOP_REPORT          ((ipj_report_id)(0x5))
#define E_IPJ_REPORT_ID_TEST_REPORT          ((ipj_report_id)(0xC))
#define E_IPJ_REPORT_ID_ERROR_REPORT         ((ipj_report_id)(0xD))
#define E_IPJ_REPORT_ID_STATUS_REPORT        ((ipj_report_id)(0xE))
#define E_IPJ_REPORT_ID_GPIO_REPORT          ((ipj_report_id)(0xF))
#define E_IPJ_REPORT_ID_IDD_REPORT           ((ipj_report_id)(0x10))

typedef uint32_t ipj_handler_type;
#define E_IPJ_HANDLER_TYPE_PLATFORM_OPEN_PORT         ((ipj_handler_type)(0x0))
#define E_IPJ_HANDLER_TYPE_PLATFORM_CLOSE_PORT        ((ipj_handler_type)(0x1))
#define E_IPJ_HANDLER_TYPE_PLATFORM_TRANSMIT          ((ipj_handler_type)(0x2))
#define E_IPJ_HANDLER_TYPE_PLATFORM_RECEIVE           ((ipj_handler_type)(0x3))
#define E_IPJ_HANDLER_TYPE_PLATFORM_TIMESTAMP         ((ipj_handler_type)(0x4))
#define E_IPJ_HANDLER_TYPE_PLATFORM_SLEEP_MS          ((ipj_handler_type)(0x5))
#define E_IPJ_HANDLER_TYPE_REPORT                     ((ipj_handler_type)(0x6))
#define E_IPJ_HANDLER_TYPE_DIAGNOSTIC                 ((ipj_handler_type)(0x7))
#define E_IPJ_HANDLER_TYPE_PLATFORM_MODIFY_CONNECTION ((ipj_handler_type)(0x8))
#define E_IPJ_HANDLER_TYPE_PLATFORM_FLUSH_PORT        ((ipj_handler_type)(0x9))

typedef uint32_t ipj_idle_power_mode;
#define E_IPJ_IDLE_POWER_MODE_STANDARD    ((ipj_idle_power_mode)(0x0))
#define E_IPJ_IDLE_POWER_MODE_LOW_LATENCY ((ipj_idle_power_mode)(0x1))

typedef uint32_t ipj_region;
#define E_IPJ_REGION_FCC_PART_15_247                ((ipj_region)(0x0))
#define E_IPJ_REGION_HONG_KONG_920_925_MHZ          ((ipj_region)(0x3))
#define E_IPJ_REGION_TAIWAN_922_928_MHZ             ((ipj_region)(0x4))
#define E_IPJ_REGION_ETSI_EN_302_208_V1_4_1         ((ipj_region)(0x7))
#define E_IPJ_REGION_KOREA_917_921_MHZ              ((ipj_region)(0x8))
#define E_IPJ_REGION_MALAYSIA_919_923_MHZ           ((ipj_region)(0x9))
#define E_IPJ_REGION_CHINA_920_925_MHZ              ((ipj_region)(0xA))
#define E_IPJ_REGION_SOUTH_AFRICA_915_919_MHZ       ((ipj_region)(0xC))
#define E_IPJ_REGION_BRAZIL_902_907_AND_915_928_MHZ ((ipj_region)(0xD))
#define E_IPJ_REGION_THAILAND_920_925_MHZ           ((ipj_region)(0xE))
#define E_IPJ_REGION_SINGAPORE_920_925_MHZ          ((ipj_region)(0xF))
#define E_IPJ_REGION_AUSTRALIA_920_926_MHZ          ((ipj_region)(0x10))
#define E_IPJ_REGION_INDIA_865_867_MHZ              ((ipj_region)(0x11))
#define E_IPJ_REGION_URUGUAY_916_928_MHZ            ((ipj_region)(0x12))
#define E_IPJ_REGION_VIETNAM_920_925_MHZ            ((ipj_region)(0x13))
#define E_IPJ_REGION_ISRAEL_915_917_MHZ             ((ipj_region)(0x14))
#define E_IPJ_REGION_PHILIPPINES_918_920_MHZ        ((ipj_region)(0x15))
#define E_IPJ_REGION_INDONESIA_923_925_MHZ          ((ipj_region)(0x17))
#define E_IPJ_REGION_NEW_ZEALAND_921P5_928_MHZ      ((ipj_region)(0x18))
#define E_IPJ_REGION_JAPAN_916_921_MHZ_NO_LBT       ((ipj_region)(0x19))
#define E_IPJ_REGION_PERU_916_928_MHZ               ((ipj_region)(0x1A))
#define E_IPJ_REGION_RUSSIA_916_921_MHZ             ((ipj_region)(0x1B))
#define E_IPJ_REGION_CUSTOM                         ((ipj_region)(0x100))

typedef uint32_t ipj_test_id;
#define E_IPJ_TEST_ID_NONE                          ((ipj_test_id)(0x0))
#define E_IPJ_TEST_ID_GENERIC                       ((ipj_test_id)(0x1))
#define E_IPJ_TEST_ID_BIST                          ((ipj_test_id)(0x2))
#define E_IPJ_TEST_ID_MEMORY_READ                   ((ipj_test_id)(0x3))
#define E_IPJ_TEST_ID_MEMORY_WRITE                  ((ipj_test_id)(0x4))
#define E_IPJ_TEST_ID_TRANSCEIVER_READ              ((ipj_test_id)(0x5))
#define E_IPJ_TEST_ID_TRANSCEIVER_WRITE             ((ipj_test_id)(0x6))
#define E_IPJ_TEST_ID_SET_FREQUENCY                 ((ipj_test_id)(0x7))
#define E_IPJ_TEST_ID_CW_CONTROL                    ((ipj_test_id)(0x8))
#define E_IPJ_TEST_ID_PRBS_CONTROL                  ((ipj_test_id)(0x9))
#define E_IPJ_TEST_ID_GEN2_TX_CONTROL               ((ipj_test_id)(0xA))
#define E_IPJ_TEST_ID_TRANSCEIVER_READ_MODIFY_WRITE ((ipj_test_id)(0xB))
#define E_IPJ_TEST_ID_CALIBRATE_BEGIN               ((ipj_test_id)(0x65))
#define E_IPJ_TEST_ID_CALIBRATE_END                 ((ipj_test_id)(0x66))
#define E_IPJ_TEST_ID_CALIBRATE_FORWARD_POWER       ((ipj_test_id)(0x67))
#define E_IPJ_TEST_ID_CALIBRATE_REVERSE_POWER       ((ipj_test_id)(0x68))
#define E_IPJ_TEST_ID_CALIBRATE_DC_OFFSET           ((ipj_test_id)(0x69))
#define E_IPJ_TEST_ID_CALIBRATE_PA_BIAS             ((ipj_test_id)(0x6A))
#define E_IPJ_TEST_ID_CALIBRATE_GROSS_GAIN          ((ipj_test_id)(0x6B))
#define E_IPJ_TEST_ID_CALIBRATE_RSSI                ((ipj_test_id)(0x6C))
#define E_IPJ_TEST_ID_CALIBRATE_TEMPERATURE         ((ipj_test_id)(0x6D))
#define E_IPJ_TEST_ID_CALIBRATE_TX_PRE_DISTORTION   ((ipj_test_id)(0x6E))
#define E_IPJ_TEST_ID_CALIBRATE_SET_KEY             ((ipj_test_id)(0x6F))
#define E_IPJ_TEST_ID_CALIBRATE_FINALIZE_KEYS       ((ipj_test_id)(0x70))
#define E_IPJ_TEST_ID_HARDWARE_CONTROL              ((ipj_test_id)(0xC9))
#define E_IPJ_TEST_ID_INVENTORY_CONTROL             ((ipj_test_id)(0xCA))
#define E_IPJ_TEST_ID_GENERIC_TX_CONTROL            ((ipj_test_id)(0xCB))
#define E_IPJ_TEST_ID_POWER_SCALAR_CONTROL          ((ipj_test_id)(0xCC))
#define E_IPJ_TEST_ID_GROSS_GAIN_CONTROL            ((ipj_test_id)(0xCD))
#define E_IPJ_TEST_ID_PA_BIAS_CONTROL               ((ipj_test_id)(0xCE))
#define E_IPJ_TEST_ID_GENERIC_RX_CONTROL            ((ipj_test_id)(0xCF))
#define E_IPJ_TEST_ID_USER_DAC_CONTROL              ((ipj_test_id)(0xD0))
#define E_IPJ_TEST_ID_USER_ADC_CONTROL              ((ipj_test_id)(0xD1))
#define E_IPJ_TEST_ID_AUX_DAC_CONTROL               ((ipj_test_id)(0xD2))
#define E_IPJ_TEST_ID_AUX_ADC_CONTROL               ((ipj_test_id)(0xD3))
#define E_IPJ_TEST_ID_GPIO_CONTROL                  ((ipj_test_id)(0xD4))
#define E_IPJ_TEST_ID_DEBUG_UART_CONTROL            ((ipj_test_id)(0xD5))
#define E_IPJ_TEST_ID_TEMPERATURE_CONTROL           ((ipj_test_id)(0xD6))
#define E_IPJ_TEST_ID_RF_PROFILE_CONTROL            ((ipj_test_id)(0xD7))
#define E_IPJ_TEST_ID_SENSOR_CONTROL                ((ipj_test_id)(0xD8))
#define E_IPJ_TEST_ID_CALIBRATION_CONTROL           ((ipj_test_id)(0xD9))
#define E_IPJ_TEST_ID_ERROR_CONTROL                 ((ipj_test_id)(0xDA))
#define E_IPJ_TEST_ID_RF_SWITCH_CONTROL             ((ipj_test_id)(0xDB))
#define E_IPJ_TEST_ID_GPIO32_CONTROL                ((ipj_test_id)(0xDC))
#define E_IPJ_TEST_ID_DAC_CONTROL                   ((ipj_test_id)(0xDD))
#define E_IPJ_TEST_ID_ADC_CONTROL                   ((ipj_test_id)(0xDE))
#define E_IPJ_TEST_ID_DEBUG_INFO                    ((ipj_test_id)(0xDF))
#define E_IPJ_TEST_ID_TAG_REPORT_GENERATION         ((ipj_test_id)(0xE0))
#define E_IPJ_TEST_ID_ANTENNA_CHARACTERIZATION      ((ipj_test_id)(0xE1))

typedef uint32_t ipj_inventory_select_flag;
#define E_IPJ_INVENTORY_SELECT_FLAG_AUTO_SL ((ipj_inventory_select_flag)(0x0))
#define E_IPJ_INVENTORY_SELECT_FLAG_ALL_SL  ((ipj_inventory_select_flag)(0x1))
#define E_IPJ_INVENTORY_SELECT_FLAG_NOT_SL  ((ipj_inventory_select_flag)(0x2))
#define E_IPJ_INVENTORY_SELECT_FLAG_SL      ((ipj_inventory_select_flag)(0x3))

typedef uint32_t ipj_inventory_search_mode;
#define E_IPJ_INVENTORY_SEARCH_MODE_AUTO_SEARCH          ((ipj_inventory_search_mode)(0x0))
#define E_IPJ_INVENTORY_SEARCH_MODE_DUAL_TARGET          ((ipj_inventory_search_mode)(0x1))
#define E_IPJ_INVENTORY_SEARCH_MODE_SINGLE_TARGET_A_TO_B ((ipj_inventory_search_mode)(0x2))
#define E_IPJ_INVENTORY_SEARCH_MODE_SINGLE_TARGET_B_TO_A ((ipj_inventory_search_mode)(0x3))

typedef uint32_t ipj_mem_bank;
#define E_IPJ_MEM_BANK_RESERVED ((ipj_mem_bank)(0x0))
#define E_IPJ_MEM_BANK_EPC      ((ipj_mem_bank)(0x1))
#define E_IPJ_MEM_BANK_TID      ((ipj_mem_bank)(0x2))
#define E_IPJ_MEM_BANK_USER     ((ipj_mem_bank)(0x3))

typedef uint32_t ipj_select_target;
#define E_IPJ_SELECT_TARGET_SESSION_S0 ((ipj_select_target)(0x0))
#define E_IPJ_SELECT_TARGET_SESSION_S1 ((ipj_select_target)(0x1))
#define E_IPJ_SELECT_TARGET_SESSION_S2 ((ipj_select_target)(0x2))
#define E_IPJ_SELECT_TARGET_SESSION_S3 ((ipj_select_target)(0x3))
#define E_IPJ_SELECT_TARGET_SL_FLAG    ((ipj_select_target)(0x4))
#define E_IPJ_SELECT_TARGET_RFU_1      ((ipj_select_target)(0x5))
#define E_IPJ_SELECT_TARGET_RFU_2      ((ipj_select_target)(0x6))
#define E_IPJ_SELECT_TARGET_RFU_3      ((ipj_select_target)(0x7))

typedef uint32_t ipj_baud_rate;
#define E_IPJ_BAUD_RATE_BR9600   ((ipj_baud_rate)(0x2580))
#define E_IPJ_BAUD_RATE_BR14400  ((ipj_baud_rate)(0x3840))
#define E_IPJ_BAUD_RATE_BR19200  ((ipj_baud_rate)(0x4B00))
#define E_IPJ_BAUD_RATE_BR38400  ((ipj_baud_rate)(0x9600))
#define E_IPJ_BAUD_RATE_BR57600  ((ipj_baud_rate)(0xE100))
#define E_IPJ_BAUD_RATE_BR115200 ((ipj_baud_rate)(0x1C200))
#define E_IPJ_BAUD_RATE_BR230400 ((ipj_baud_rate)(0x38400))
#define E_IPJ_BAUD_RATE_BR460800 ((ipj_baud_rate)(0x70800))
#define E_IPJ_BAUD_RATE_BR921600 ((ipj_baud_rate)(0xE1000))

typedef uint32_t ipj_parity;
#define E_IPJ_PARITY_PNONE ((ipj_parity)(0x0))
#define E_IPJ_PARITY_PEVEN ((ipj_parity)(0x1))
#define E_IPJ_PARITY_PODD  ((ipj_parity)(0x2))

typedef uint32_t ipj_select_action;
#define E_IPJ_SELECT_ACTION_ASLINVA_DSLINVB ((ipj_select_action)(0x0))
#define E_IPJ_SELECT_ACTION_ASLINVA_NOTHING ((ipj_select_action)(0x1))
#define E_IPJ_SELECT_ACTION_NOTHING_DSLINVB ((ipj_select_action)(0x2))
#define E_IPJ_SELECT_ACTION_NSLINVS_NOTHING ((ipj_select_action)(0x3))
#define E_IPJ_SELECT_ACTION_DSLINVB_ASLINVA ((ipj_select_action)(0x4))
#define E_IPJ_SELECT_ACTION_DSLINVB_NOTHING ((ipj_select_action)(0x5))
#define E_IPJ_SELECT_ACTION_NOTHING_ASLINVA ((ipj_select_action)(0x6))
#define E_IPJ_SELECT_ACTION_NOTHING_NSLINVS ((ipj_select_action)(0x7))

typedef uint32_t ipj_blockpermalock_action;
#define E_IPJ_BLOCKPERMALOCK_ACTION_READ      ((ipj_blockpermalock_action)(0x0))
#define E_IPJ_BLOCKPERMALOCK_ACTION_PERMALOCK ((ipj_blockpermalock_action)(0x1))

typedef uint32_t ipj_write_epc_length_control;
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_AUTO       ((ipj_write_epc_length_control)(0x0))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_USER_VALUE ((ipj_write_epc_length_control)(0x1))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_ZERO       ((ipj_write_epc_length_control)(0x2))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_NO_CHANGE  ((ipj_write_epc_length_control)(0x3))

typedef uint32_t ipj_qt_action;
#define E_IPJ_QT_ACTION_READ  ((ipj_qt_action)(0x0))
#define E_IPJ_QT_ACTION_WRITE ((ipj_qt_action)(0x1))

typedef uint32_t ipj_qt_persistence;
#define E_IPJ_QT_PERSISTENCE_TEMPORARY ((ipj_qt_persistence)(0x0))
#define E_IPJ_QT_PERSISTENCE_PERMANENT ((ipj_qt_persistence)(0x1))

typedef uint32_t ipj_qt_data_profile;
#define E_IPJ_QT_DATA_PROFILE_PRIVATE ((ipj_qt_data_profile)(0x0))
#define E_IPJ_QT_DATA_PROFILE_PUBLIC  ((ipj_qt_data_profile)(0x1))

typedef uint32_t ipj_qt_access_range;
#define E_IPJ_QT_ACCESS_RANGE_NORMAL ((ipj_qt_access_range)(0x0))
#define E_IPJ_QT_ACCESS_RANGE_SHORT  ((ipj_qt_access_range)(0x1))

typedef uint32_t ipj_status_flag;
#define E_IPJ_STATUS_FLAG_BIT_CHANNEL_ACTIVITY ((ipj_status_flag)(0x1))
#define E_IPJ_STATUS_FLAG_BIT_IDD              ((ipj_status_flag)(0x2))

typedef uint32_t ipj_gpi_action;
#define E_IPJ_GPI_ACTION_NONE            ((ipj_gpi_action)(0x0))
#define E_IPJ_GPI_ACTION_START_INVENTORY ((ipj_gpi_action)(0x1))
#define E_IPJ_GPI_ACTION_STOP_INVENTORY  ((ipj_gpi_action)(0x2))

typedef uint32_t ipj_gpo_action;
#define E_IPJ_GPO_ACION_TAG_IN_FIELD ((ipj_gpo_action)(0x0))

typedef uint32_t ipj_gpio_mode;
#define E_IPJ_GPIO_MODE_DISABLED            ((ipj_gpio_mode)(0x0))
#define E_IPJ_GPIO_MODE_INPUT               ((ipj_gpio_mode)(0x1))
#define E_IPJ_GPIO_MODE_OUTPUT              ((ipj_gpio_mode)(0x2))
#define E_IPJ_GPIO_MODE_OUTPUT_PULSE        ((ipj_gpio_mode)(0x3))
#define E_IPJ_GPIO_MODE_INPUT_ACTION        ((ipj_gpio_mode)(0x4))
#define E_IPJ_GPIO_MODE_OUTPUT_ACTION       ((ipj_gpio_mode)(0x5))
#define E_IPJ_GPIO_MODE_OUTPUT_PULSE_ACTION ((ipj_gpio_mode)(0x6))

typedef uint32_t ipj_gpio_state;
#define E_IPJ_GPIO_STATE_LO    ((ipj_gpio_state)(0x0))
#define E_IPJ_GPIO_STATE_HI    ((ipj_gpio_state)(0x1))
#define E_IPJ_GPIO_STATE_FLOAT ((ipj_gpio_state)(0x2))

typedef uint32_t ipj_product_id;
#define E_IPJ_PRODUCT_ID_NONE       ((ipj_product_id)(0x0))
#define E_IPJ_PRODUCT_ID_RS500      ((ipj_product_id)(0x1))
#define E_IPJ_PRODUCT_ID_RESERVED_2 ((ipj_product_id)(0x2))
#define E_IPJ_PRODUCT_ID_RS2000     ((ipj_product_id)(0x3))
#define E_IPJ_PRODUCT_ID_IRD_1      ((ipj_product_id)(0x4))

typedef uint32_t ipj_product_sku;
#define E_IPJ_PRODUCT_SKU_NONE ((ipj_product_sku)(0x0))
#define E_IPJ_PRODUCT_SKU_1    ((ipj_product_sku)(0x1))
#define E_IPJ_PRODUCT_SKU_2    ((ipj_product_sku)(0x2))
#define E_IPJ_PRODUCT_SKU_3    ((ipj_product_sku)(0x3))

typedef uint32_t ipj_antenna;
#define E_IPJ_ANTENNA_AUTO ((ipj_antenna)(0x0))
#define E_IPJ_ANTENNA_1    ((ipj_antenna)(0x1))
#define E_IPJ_ANTENNA_2    ((ipj_antenna)(0x2))
#define E_IPJ_ANTENNA_3    ((ipj_antenna)(0x3))
#define E_IPJ_ANTENNA_4    ((ipj_antenna)(0x4))

typedef uint32_t ipj_antenna_rs2000;
#define E_IPJ_ANTENNA_RS2000_AUTO  ((ipj_antenna_rs2000)(0x0))
#define E_IPJ_ANTENNA_RS2000_SMT_1 ((ipj_antenna_rs2000)(0x1))
#define E_IPJ_ANTENNA_RS2000_SMT_2 ((ipj_antenna_rs2000)(0x2))
#define E_IPJ_ANTENNA_RS2000_UFL_1 ((ipj_antenna_rs2000)(0x3))
#define E_IPJ_ANTENNA_RS2000_UFL_2 ((ipj_antenna_rs2000)(0x4))

typedef uint32_t ipj_error;
#define E_IPJ_ERROR_SUCCESS                            ((ipj_error)(0x0))
#define E_IPJ_ERROR_GENERAL_ERROR                      ((ipj_error)(0x1))
#define E_IPJ_ERROR_SET_KEY_INVALID                    ((ipj_error)(0x2))
#define E_IPJ_ERROR_SET_KEY_READ_ONLY                  ((ipj_error)(0x3))
#define E_IPJ_ERROR_SET_KEY_OUT_OF_RANGE               ((ipj_error)(0x4))
#define E_IPJ_ERROR_GET_KEY_INVALID                    ((ipj_error)(0x5))
#define E_IPJ_ERROR_GET_KEY_WRITE_ONLY                 ((ipj_error)(0x6))
#define E_IPJ_ERROR_COMMAND_INVALID                    ((ipj_error)(0x7))
#define E_IPJ_ERROR_COMMAND_START_FAILURE              ((ipj_error)(0x8))
#define E_IPJ_ERROR_COMMAND_DECODE_FAILURE             ((ipj_error)(0x9))
#define E_IPJ_ERROR_COMMAND_ENCODE_FAILURE             ((ipj_error)(0xA))
#define E_IPJ_ERROR_COMMAND_STALLED                    ((ipj_error)(0xB))
#define E_IPJ_ERROR_VALUE_INVALID                      ((ipj_error)(0xC))
#define E_IPJ_ERROR_MORE_THAN_ONE_COMMAND_RECEIVED     ((ipj_error)(0xD))
#define E_IPJ_ERROR_NOT_IMPLEMENTED                    ((ipj_error)(0xE))
#define E_IPJ_ERROR_INVALID_PRODUCT_CONFIGURATION      ((ipj_error)(0xF))
#define E_IPJ_ERROR_INVALID_FACTORY_SETTINGS           ((ipj_error)(0x10))
#define E_IPJ_ERROR_RESPONSE_ENCODE_FAILURE            ((ipj_error)(0x11))
#define E_IPJ_ERROR_COMMAND_VERIFY_FAILURE             ((ipj_error)(0x12))
#define E_IPJ_ERROR_INTERNAL_NON_RECOVERABLE           ((ipj_error)(0x13))
#define E_IPJ_ERROR_TEMPLATE_DECODE_FAILURE            ((ipj_error)(0x14))
#define E_IPJ_ERROR_SYSTEM_IN_ERROR_STATE              ((ipj_error)(0x15))
#define E_IPJ_ERROR_TEST_ERROR                         ((ipj_error)(0x16))
#define E_IPJ_ERROR_STORED_SETTING_DECODE              ((ipj_error)(0x17))
#define E_IPJ_ERROR_VALUE_INDEX_OUT_OF_RANGE           ((ipj_error)(0x18))
#define E_IPJ_ERROR_BANK_INDEX_OUT_OF_RANGE            ((ipj_error)(0x19))
#define E_IPJ_ERROR_INVALID_PRODUCT_CALIBRATION        ((ipj_error)(0x1A))
#define E_IPJ_ERROR_REPORT_SIZE_WOULD_OVERFLOW         ((ipj_error)(0x1B))
#define E_IPJ_ERROR_FIXED_HARDWARE_SETTINGS_LOAD_ERROR ((ipj_error)(0x1C))
#define E_IPJ_ERROR_GEN2_TAG_OTHER_ERROR               ((ipj_error)(0x1000001))
#define E_IPJ_ERROR_GEN2_TAG_MEMORY_OVERRUN            ((ipj_error)(0x1000002))
#define E_IPJ_ERROR_GEN2_TAG_MEMORY_LOCKED             ((ipj_error)(0x1000003))
#define E_IPJ_ERROR_GEN2_TAG_INSUFFICIENT_POWER        ((ipj_error)(0x1000004))
#define E_IPJ_ERROR_GEN2_TAG_NON_SPECIFIC_ERROR        ((ipj_error)(0x1000005))
#define E_IPJ_ERROR_API_DEVICE_NOT_INITIALIZED         ((ipj_error)(0x2000001))
#define E_IPJ_ERROR_API_SERIAL_PORT_ERROR              ((ipj_error)(0x2000002))
#define E_IPJ_ERROR_API_CONNECTION_READ_TIMEOUT        ((ipj_error)(0x2000003))
#define E_IPJ_ERROR_API_CONNECTION_WRITE_TIMEOUT       ((ipj_error)(0x2000004))
#define E_IPJ_ERROR_API_CONNECTION_WRITE_ERROR         ((ipj_error)(0x2000005))
#define E_IPJ_ERROR_API_RX_BUFF_TOO_SMALL              ((ipj_error)(0x2000006))
#define E_IPJ_ERROR_API_MESSAGE_INVALID                ((ipj_error)(0x2000007))
#define E_IPJ_ERROR_API_NO_HANDLER                     ((ipj_error)(0x2000008))
#define E_IPJ_ERROR_API_INVALID_LOADER_BLOCK           ((ipj_error)(0x2000009))
#define E_IPJ_ERROR_API_RESPONSE_MISMATCH              ((ipj_error)(0x200000A))
#define E_IPJ_ERROR_API_INVALID_PARAMETER              ((ipj_error)(0x200000B))
#define E_IPJ_ERROR_API_NON_LT_PACKET_DETECTED         ((ipj_error)(0x200000C))
#define E_IPJ_ERROR_IRI_FRAME_DROPPED                  ((ipj_error)(0x3000001))
#define E_IPJ_ERROR_IRI_FRAME_INVALID                  ((ipj_error)(0x3000002))
#define E_IPJ_ERROR_MAC_GENERAL                        ((ipj_error)(0x4000001))
#define E_IPJ_ERROR_MAC_CRC_MISMATCH                   ((ipj_error)(0x4000002))
#define E_IPJ_ERROR_MAC_NO_TAG_RESPONSE                ((ipj_error)(0x4000003))
#define E_IPJ_ERROR_MAC_TAG_LOST                       ((ipj_error)(0x4000004))
#define E_IPJ_ERROR_BTS_DEVICE_WATCHDOG_RESET          ((ipj_error)(0x5000001))
#define E_IPJ_ERROR_BTS_VALUE_INVALID                  ((ipj_error)(0x5000002))
#define E_IPJ_ERROR_BTS_FLASH_WRITE                    ((ipj_error)(0x5000003))
#define E_IPJ_ERROR_BTS_FLASH_READ                     ((ipj_error)(0x5000004))
#define E_IPJ_ERROR_BTS_FLASH_ADDRESS                  ((ipj_error)(0x5000005))
#define E_IPJ_ERROR_BTS_FLASH_ERASE                    ((ipj_error)(0x5000006))
#define E_IPJ_ERROR_BTS_UNKNOWN_COMMAND                ((ipj_error)(0x5000007))
#define E_IPJ_ERROR_BTS_COMMAND_DECODE_FAILURE         ((ipj_error)(0x5000008))
#define E_IPJ_ERROR_TRANSCEIVER_FAILURE                ((ipj_error)(0x6000001))
#define E_IPJ_ERROR_LIMIT_PA_TEMPERATURE_MAX           ((ipj_error)(0x7000001))

typedef uint32_t ipj_key;
#define E_IPJ_KEY_BOOTSTRAP_VERSION            ((ipj_key)(0x1))
#define E_IPJ_KEY_BOOTSTRAP_CRC                ((ipj_key)(0x2))
#define E_IPJ_KEY_APPLICATION_VERSION          ((ipj_key)(0x3))
#define E_IPJ_KEY_APPLICATION_CRC              ((ipj_key)(0x4))
#define E_IPJ_KEY_SECONDARY_IMAGE_VERSION      ((ipj_key)(0x5))
#define E_IPJ_KEY_SECONDARY_IMAGE_CRC          ((ipj_key)(0x6))
#define E_IPJ_KEY_SECONDARY_IMAGE_TYPE         ((ipj_key)(0x7))
#define E_IPJ_KEY_APPLICATION_REVISION_ID      ((ipj_key)(0x8))
#define E_IPJ_KEY_APPLICATION_BUILD_ID         ((ipj_key)(0x9))
#define E_IPJ_KEY_PRODUCT_ID                   ((ipj_key)(0xA))
#define E_IPJ_KEY_SERIAL_NUMBER                ((ipj_key)(0xB))
#define E_IPJ_KEY_TRANSCEIVER_ID               ((ipj_key)(0xC))
#define E_IPJ_KEY_MICROPROCESSOR_ID            ((ipj_key)(0xD))
#define E_IPJ_KEY_CUSTOMER_VERSION             ((ipj_key)(0xE))
#define E_IPJ_KEY_CUSTOMER_ID                  ((ipj_key)(0xF))
#define E_IPJ_KEY_CUSTOMER_PRODUCT_ID          ((ipj_key)(0x10))
#define E_IPJ_KEY_CALIBRATION_INFO             ((ipj_key)(0x11))
#define E_IPJ_KEY_TEST_INFO                    ((ipj_key)(0x12))
#define E_IPJ_KEY_PRODUCT_SKU                  ((ipj_key)(0x13))
#define E_IPJ_KEY_LOT_DATE_CODE                ((ipj_key)(0x14))
#define E_IPJ_KEY_PRODUCT_KEY                  ((ipj_key)(0x15))
#define E_IPJ_KEY_SECONDARY_IMAGE_LOCATION     ((ipj_key)(0x16))
#define E_IPJ_KEY_SECONDARY_IMAGE_SIZE         ((ipj_key)(0x17))
#define E_IPJ_KEY_UNIQUE_ID                    ((ipj_key)(0x18))
#define E_IPJ_KEY_HARDWARE_REVISION            ((ipj_key)(0x19))
#define E_IPJ_KEY_SYSTEM_TIMESTAMP_MS          ((ipj_key)(0x1A))
#define E_IPJ_KEY_ACTIVE_ACTIONS               ((ipj_key)(0x1B))
#define E_IPJ_KEY_REGION_ID                    ((ipj_key)(0x20))
#define E_IPJ_KEY_REGION_CHANNEL_TABLE         ((ipj_key)(0x21))
#define E_IPJ_KEY_REGION_CHANNEL_TABLE_SIZE    ((ipj_key)(0x22))
#define E_IPJ_KEY_REGION_ON_TIME_NOMINAL       ((ipj_key)(0x23))
#define E_IPJ_KEY_REGION_ON_TIME_ACCESS        ((ipj_key)(0x24))
#define E_IPJ_KEY_REGION_OFF_TIME              ((ipj_key)(0x25))
#define E_IPJ_KEY_REGION_OFF_TIME_SAME_CHANNEL ((ipj_key)(0x26))
#define E_IPJ_KEY_REGION_START_FREQUENCY_KHZ   ((ipj_key)(0x27))
#define E_IPJ_KEY_REGION_CHANNEL_SPACING_KHZ   ((ipj_key)(0x28))
#define E_IPJ_KEY_REGION_RANDOM_HOP            ((ipj_key)(0x29))
#define E_IPJ_KEY_REGION_INDY_PLL_R_DIVIDER    ((ipj_key)(0x2A))
#define E_IPJ_KEY_REGION_RF_FILTER             ((ipj_key)(0x2B))
#define E_IPJ_KEY_ANTENNA_TX_POWER             ((ipj_key)(0x31))
#define E_IPJ_KEY_ANTENNA_SEQUENCE             ((ipj_key)(0x32))
#define E_IPJ_KEY_INVENTORY_TAG_POPULATION     ((ipj_key)(0x40))
#define E_IPJ_KEY_INVENTORY_SELECT_FLAG        ((ipj_key)(0x41))
#define E_IPJ_KEY_INVENTORY_SESSION            ((ipj_key)(0x42))
#define E_IPJ_KEY_INVENTORY_SEARCH_MODE        ((ipj_key)(0x43))
#define E_IPJ_KEY_FAST_ID_ENABLE               ((ipj_key)(0x45))
#define E_IPJ_KEY_TAG_FOCUS_ENABLE             ((ipj_key)(0x46))
#define E_IPJ_KEY_TAG_OPERATION_ENABLE         ((ipj_key)(0x47))
#define E_IPJ_KEY_TAG_OPERATION_RETRIES        ((ipj_key)(0x48))
#define E_IPJ_KEY_SELECT_ENABLE                ((ipj_key)(0x50))
#define E_IPJ_KEY_SELECT_TARGET                ((ipj_key)(0x51))
#define E_IPJ_KEY_SELECT_ACTION                ((ipj_key)(0x52))
#define E_IPJ_KEY_SELECT_MEM_BANK              ((ipj_key)(0x53))
#define E_IPJ_KEY_SELECT_POINTER               ((ipj_key)(0x54))
#define E_IPJ_KEY_SELECT_MASK_LENGTH           ((ipj_key)(0x55))
#define E_IPJ_KEY_SELECT_MASK_VALUE            ((ipj_key)(0x56))
#define E_IPJ_KEY_TAG_OPERATION                ((ipj_key)(0x60))
#define E_IPJ_KEY_ACCESS_PASSWORD              ((ipj_key)(0x61))
#define E_IPJ_KEY_KILL_PASSWORD                ((ipj_key)(0x62))
#define E_IPJ_KEY_READ_MEM_BANK                ((ipj_key)(0x63))
#define E_IPJ_KEY_READ_WORD_POINTER            ((ipj_key)(0x64))
#define E_IPJ_KEY_READ_WORD_COUNT              ((ipj_key)(0x65))
#define E_IPJ_KEY_WRITE_MEM_BANK               ((ipj_key)(0x66))
#define E_IPJ_KEY_WRITE_WORD_POINTER           ((ipj_key)(0x67))
#define E_IPJ_KEY_WRITE_WORD_COUNT             ((ipj_key)(0x68))
#define E_IPJ_KEY_WRITE_DATA                   ((ipj_key)(0x69))
#define E_IPJ_KEY_LOCK_PAYLOAD                 ((ipj_key)(0x6A))
#define E_IPJ_KEY_BLOCKPERMALOCK_ACTION        ((ipj_key)(0x6B))
#define E_IPJ_KEY_BLOCKPERMALOCK_MEM_BANK      ((ipj_key)(0x6C))
#define E_IPJ_KEY_BLOCKPERMALOCK_BLOCK_POINTER ((ipj_key)(0x6D))
#define E_IPJ_KEY_BLOCKPERMALOCK_BLOCK_RANGE   ((ipj_key)(0x6E))
#define E_IPJ_KEY_BLOCKPERMALOCK_MASK          ((ipj_key)(0x6F))
#define E_IPJ_KEY_WRITE_EPC_LENGTH_CONTROL     ((ipj_key)(0x70))
#define E_IPJ_KEY_WRITE_EPC_LENGTH_VALUE       ((ipj_key)(0x71))
#define E_IPJ_KEY_WRITE_EPC_AFI_CONTROL        ((ipj_key)(0x72))
#define E_IPJ_KEY_WRITE_EPC_AFI_VALUE          ((ipj_key)(0x73))
#define E_IPJ_KEY_QT_ACTION                    ((ipj_key)(0x74))
#define E_IPJ_KEY_QT_PERSISTENCE               ((ipj_key)(0x75))
#define E_IPJ_KEY_QT_DATA_PROFILE              ((ipj_key)(0x76))
#define E_IPJ_KEY_QT_ACCESS_RANGE              ((ipj_key)(0x77))
#define E_IPJ_KEY_QT_TAG_OPERATION             ((ipj_key)(0x78))
#define E_IPJ_KEY_AUTOSTOP_DURATION_MS         ((ipj_key)(0x89))
#define E_IPJ_KEY_AUTOSTOP_TAG_COUNT           ((ipj_key)(0x8B))
#define E_IPJ_KEY_AUTOSTOP_ROUND_COUNT         ((ipj_key)(0x8C))
#define E_IPJ_KEY_REPORT_CONTROL_TAG           ((ipj_key)(0xA1))
#define E_IPJ_KEY_REPORT_CONTROL_STATUS        ((ipj_key)(0xA2))
#define E_IPJ_KEY_REPORT_CONTROL_TIMESTAMP     ((ipj_key)(0xA3))
#define E_IPJ_KEY_RESPONSE_CONTROL_TIMESTAMP   ((ipj_key)(0xA4))
#define E_IPJ_KEY_TEMPERATURE_INTERNAL         ((ipj_key)(0xB0))
#define E_IPJ_KEY_TEMPERATURE_EXTERNAL         ((ipj_key)(0xB1))
#define E_IPJ_KEY_TEMPERATURE_PA               ((ipj_key)(0xB2))
#define E_IPJ_KEY_GPIO_MODE                    ((ipj_key)(0xC0))
#define E_IPJ_KEY_GPIO_STATE                   ((ipj_key)(0xC1))
#define E_IPJ_KEY_GPIO_HI_ACTION               ((ipj_key)(0xC2))
#define E_IPJ_KEY_GPIO_LO_ACTION               ((ipj_key)(0xC3))
#define E_IPJ_KEY_GPIO_PULSE_DURATION          ((ipj_key)(0xC4))
#define E_IPJ_KEY_GPIO_DEBOUNCE_MS             ((ipj_key)(0xC5))
#define E_IPJ_KEY_GPIO_CURRENT_STATE           ((ipj_key)(0xC6))
#define E_IPJ_KEY_RF_MODE                      ((ipj_key)(0xD0))
#define E_IPJ_KEY_FIRST_ERROR                  ((ipj_key)(0xE0))
#define E_IPJ_KEY_LAST_ERROR                   ((ipj_key)(0xE1))
#define E_IPJ_KEY_SYSTEM_ERROR                 ((ipj_key)(0xE2))
#define E_IPJ_KEY_CAL_DATA_1                   ((ipj_key)(0xF0))
#define E_IPJ_KEY_CAL_DATA_2                   ((ipj_key)(0xF1))
#define E_IPJ_KEY_CAL_DATA_3                   ((ipj_key)(0xF2))
#define E_IPJ_KEY_CAL_DATA_4                   ((ipj_key)(0xF3))
#define E_IPJ_KEY_CAL_DATA_5                   ((ipj_key)(0xF4))
#define E_IPJ_KEY_CAL_DATA_6                   ((ipj_key)(0xF5))
#define E_IPJ_KEY_CAL_DATA_7                   ((ipj_key)(0xF6))
#define E_IPJ_KEY_CAL_DATA_8                   ((ipj_key)(0xF7))
#define E_IPJ_KEY_DEVICE_BAUDRATE              ((ipj_key)(0x100))
#define E_IPJ_KEY_DEVICE_IDLE_POWER_MODE       ((ipj_key)(0x101))
#define E_IPJ_KEY_ONBOOT_START_ACTION          ((ipj_key)(0x102))
#define E_IPJ_KEY_ENABLE_LT_REPORTS            ((ipj_key)(0x103))
#define E_IPJ_KEY_TEST_ID                      ((ipj_key)(0x400))
#define E_IPJ_KEY_TEST_PARAMETERS              ((ipj_key)(0x401))
#define E_IPJ_KEY_TEST_RESULT_1                ((ipj_key)(0x402))
#define E_IPJ_KEY_TEST_RESULT_2                ((ipj_key)(0x403))
#define E_IPJ_KEY_TEST_RESULT_3                ((ipj_key)(0x404))
#define E_IPJ_KEY_TEST_DATA                    ((ipj_key)(0x405))
#define E_IPJ_KEY_TEST_FREQUENCY               ((ipj_key)(0x406))
#define E_IPJ_KEY_TEST_POWER                   ((ipj_key)(0x407))
#define E_IPJ_KEY_TEST_RF_MODE                 ((ipj_key)(0x408))
#define E_IPJ_KEY_TEST_TIME                    ((ipj_key)(0x409))
#define E_IPJ_KEY_TEST_EVENT                   ((ipj_key)(0x40A))
#define E_IPJ_KEY_TEST_REPORTS                 ((ipj_key)(0x40B))
#define E_IPJ_KEY_TEST_SYSTEM                  ((ipj_key)(0x40C))
#define E_IPJ_KEY_TEST_DEBUG_PORT              ((ipj_key)(0x40D))
#define E_IPJ_KEY_GENERIC_DATA                 ((ipj_key)(0xC00))
#define E_IPJ_KEY_OEM_DATA                     ((ipj_key)(0xC01))
#define E_IPJ_KEY_STORED_SETTINGS_LOAD_STATUS  ((ipj_key)(0x1021))

typedef uint32_t ipj_reset_type;
#define E_IPJ_RESET_TYPE_SOFT            ((ipj_reset_type)(0x0))
#define E_IPJ_RESET_TYPE_TO_BOOTLOADER   ((ipj_reset_type)(0x1))
#define E_IPJ_RESET_TYPE_FACTORY_RESTORE ((ipj_reset_type)(0x2))

typedef uint32_t ipj_connection_type;
#define E_IPJ_CONNECTION_TYPE_SERIAL ((ipj_connection_type)(0x0))

typedef uint32_t ipj_key_type;
#define E_IPJ_KEY_TYPE_UINT32 ((ipj_key_type)(0x0))
#define E_IPJ_KEY_TYPE_UINT16 ((ipj_key_type)(0x1))
#define E_IPJ_KEY_TYPE_UINT8  ((ipj_key_type)(0x2))
#define E_IPJ_KEY_TYPE_INT32  ((ipj_key_type)(0x3))
#define E_IPJ_KEY_TYPE_INT16  ((ipj_key_type)(0x4))
#define E_IPJ_KEY_TYPE_INT8   ((ipj_key_type)(0x5))
#define E_IPJ_KEY_TYPE_BOOL   ((ipj_key_type)(0x6))

typedef uint32_t ipj_key_permissions;
#define E_IPJ_KEY_PERMISSIONS_READ_ONLY  ((ipj_key_permissions)(0x0))
#define E_IPJ_KEY_PERMISSIONS_WRITE_ONLY ((ipj_key_permissions)(0x1))
#define E_IPJ_KEY_PERMISSIONS_READ_WRITE ((ipj_key_permissions)(0x2))

typedef uint32_t ipj_action;
#define E_IPJ_ACTION_ALL         ((ipj_action)(0x0))
#define E_IPJ_ACTION_INVENTORY   ((ipj_action)(0x1))
#define E_IPJ_ACTION_TEST        ((ipj_action)(0x2))
#define E_IPJ_ACTION_GPIO        ((ipj_action)(0x3))
#define E_IPJ_ACTION_CLEAR_ERROR ((ipj_action)(0x4))
#define E_IPJ_ACTION_STANDBY     ((ipj_action)(0x6))
#define E_IPJ_ACTION_SLEEP       ((ipj_action)(0x7))
#define E_IPJ_ACTION_NONE        ((ipj_action)(0xF))

typedef uint32_t ipj_tag_flag;
#define E_IPJ_TAG_FLAG_BIT_EPC       ((ipj_tag_flag)(0x1))
#define E_IPJ_TAG_FLAG_BIT_TID       ((ipj_tag_flag)(0x2))
#define E_IPJ_TAG_FLAG_BIT_PC        ((ipj_tag_flag)(0x4))
#define E_IPJ_TAG_FLAG_BIT_XPC       ((ipj_tag_flag)(0x8))
#define E_IPJ_TAG_FLAG_BIT_CRC       ((ipj_tag_flag)(0x10))
#define E_IPJ_TAG_FLAG_BIT_TIMESTAMP ((ipj_tag_flag)(0x20))
#define E_IPJ_TAG_FLAG_BIT_RSSI      ((ipj_tag_flag)(0x40))
#define E_IPJ_TAG_FLAG_BIT_PHASE     ((ipj_tag_flag)(0x80))
#define E_IPJ_TAG_FLAG_BIT_CHANNEL   ((ipj_tag_flag)(0x100))
#define E_IPJ_TAG_FLAG_BIT_ANTENNA   ((ipj_tag_flag)(0x200))
#define E_IPJ_TAG_FLAG_BIT_RETRIES   ((ipj_tag_flag)(0x400))

typedef uint32_t ipj_tag_operation_type;
#define E_IPJ_TAG_OPERATION_TYPE_NONE           ((ipj_tag_operation_type)(0x0))
#define E_IPJ_TAG_OPERATION_TYPE_READ           ((ipj_tag_operation_type)(0x1))
#define E_IPJ_TAG_OPERATION_TYPE_WRITE          ((ipj_tag_operation_type)(0x2))
#define E_IPJ_TAG_OPERATION_TYPE_LOCK           ((ipj_tag_operation_type)(0x3))
#define E_IPJ_TAG_OPERATION_TYPE_KILL           ((ipj_tag_operation_type)(0x4))
#define E_IPJ_TAG_OPERATION_TYPE_BLOCKPERMALOCK ((ipj_tag_operation_type)(0x5))
#define E_IPJ_TAG_OPERATION_TYPE_WRITE_EPC      ((ipj_tag_operation_type)(0x6))
#define E_IPJ_TAG_OPERATION_TYPE_QT             ((ipj_tag_operation_type)(0x7))

typedef uint32_t ipj_flash_operation;
#define E_IPJ_FLASH_OPERATION_WRITE  ((ipj_flash_operation)(0x1))
#define E_IPJ_FLASH_OPERATION_READ   ((ipj_flash_operation)(0x2))
#define E_IPJ_FLASH_OPERATION_ERASE  ((ipj_flash_operation)(0x3))
#define E_IPJ_FLASH_OPERATION_UNLOCK ((ipj_flash_operation)(0x4))

typedef uint32_t ipj_response_timestamp_flag;
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_RESET       ((ipj_response_timestamp_flag)(0x1))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_MODIFY_CONN ((ipj_response_timestamp_flag)(0x2))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_GET_INFO    ((ipj_response_timestamp_flag)(0x4))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_BULK_SET    ((ipj_response_timestamp_flag)(0x8))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_BULK_GET    ((ipj_response_timestamp_flag)(0x10))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_START       ((ipj_response_timestamp_flag)(0x20))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_RESUME      ((ipj_response_timestamp_flag)(0x40))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_STOP        ((ipj_response_timestamp_flag)(0x80))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_INVALID     ((ipj_response_timestamp_flag)(0x100))
#define E_IPJ_RES_TIMESTAMP_FLAG_BIT_FLASH       ((ipj_response_timestamp_flag)(0x200))

typedef uint32_t ipj_report_timestamp_flag;
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_TAG_OP ((ipj_report_timestamp_flag)(0x1))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_STOP   ((ipj_report_timestamp_flag)(0x2))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_TEST   ((ipj_report_timestamp_flag)(0x4))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_ERROR  ((ipj_report_timestamp_flag)(0x8))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_STATUS ((ipj_report_timestamp_flag)(0x10))
#define E_IPJ_REP_TIMESTAMP_FLAG_BIT_GPIO   ((ipj_report_timestamp_flag)(0x20))


// ---------------------------------------------------------------------------
// *****************************************************************************
// IRI API - Reports
// *****************************************************************************
typedef struct _ipj_stop_report
{
    uint32_t error;
    uint32_t action;
} ipj_stop_report;

typedef struct _ipj_gpio_report
{
    uint32_t gpio_modes[5];
    uint32_t gpio_states[5];
} ipj_gpio_report;

typedef struct _ipj_error_report
{
    uint32_t error;
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
} ipj_error_report;

typedef struct _ipj_status_report
{
    uint32_t status_flag;
    uint32_t status_1;
    uint32_t status_2;
    uint32_t status_3;
    uint32_t data[16];
} ipj_status_report;

typedef struct _ipj_test_report
{
    uint32_t error;
    uint32_t test_id;
    uint32_t result_1;
    uint32_t result_2;
    uint32_t result_3;
    uint32_t data[16];
} ipj_test_report;

// *****************************************************************************
// IRI API - LT Defines
// *****************************************************************************
#define E_IPJ_TAG_OPERATION_REPORT_ERROR              100
#define E_IPJ_TAG_OPERATION_REPORT_EPC                201
#define E_IPJ_TAG_OPERATION_REPORT_TID                202
#define E_IPJ_TAG_OPERATION_REPORT_PC                 203
#define E_IPJ_TAG_OPERATION_REPORT_XPC                204
#define E_IPJ_TAG_OPERATION_REPORT_CRC                205
#define E_IPJ_TAG_OPERATION_REPORT_TIMESTAMP          206
#define E_IPJ_TAG_OPERATION_REPORT_RSSI               207
#define E_IPJ_TAG_OPERATION_REPORT_PHASE              208
#define E_IPJ_TAG_OPERATION_REPORT_CHANNEL            209
#define E_IPJ_TAG_OPERATION_REPORT_ANTENNA            210
#define E_IPJ_TAG_OPERATION_REPORT_TAG_OPERATION_TYPE 300
#define E_IPJ_TAG_OPERATION_REPORT_TAG_OPERATION_DATA 400
#define E_IPJ_TAG_OPERATION_REPORT_RETRIES            500
#define E_IPJ_TAG_OPERATION_REPORT_DIAGNOSTIC         600

#define MAX_TX_PAYLOAD_SIZE  16
#define MAX_RX_PAYLOAD_SIZE  120

/* Report Decoding */
#define REPORT_FIELD_ID(x)          ((x >> 16) & 0xFFFF)
#define REPORT_FIELD_LENGTH(x)      (x & 0xFFFF)
#define ROUNDED_COUNT_32(x)         (((x + sizeof(uint32_t) - 1) & 0xFFFFFFFC) / sizeof(uint32_t))


/*****************************************************************************
 *                                                                           *
 * IRI API - IRI Connection Structures                                       *
 *                                                                           *
 *****************************************************************************/

typedef struct _ipj_serial_connection
{
    ipj_baud_rate baudrate;
    ipj_parity    parity;
} ipj_serial_connection;

/**
 * \union ipj_connection_params
 * This union holds one of each type of connection parameter struct
 * In the future this will allow connection parameters for a variety
 * of connection mediums to be set
 */
typedef union _ipj_connection_params
{
    ipj_serial_connection serial;
} ipj_connection_params;

/*****************************************************************************
 *                                                                           *
 * IRI API - IRI Device Structure                                            *
 *                                                                           *
 *****************************************************************************/
#if defined (ENABLE_FW_UPDATES)
#define IPJ_FW_UPDATE_TX_SIZE 140
#else
#define IPJ_FW_UPDATE_TX_SIZE 0
#endif
#define IPJ_FRAME_HEADER_SIZE           10
#define IPJ_FRAME_CRC_SIZE              2
#define IPJ_PACKET_HEADER_SIZE          6
#define IPJ_TRANSMIT_BUFFER_SIZE        (IPJ_FRAME_HEADER_SIZE + IPJ_PACKET_HEADER_SIZE + MAX_TX_PAYLOAD_SIZE + IPJ_FW_UPDATE_TX_SIZE + IPJ_FRAME_CRC_SIZE)
#define IPJ_RECEIVE_BUFFER_SIZE         (IPJ_FRAME_HEADER_SIZE + IPJ_PACKET_HEADER_SIZE + MAX_RX_PAYLOAD_SIZE + IPJ_FRAME_CRC_SIZE)
#define IPJ_DEFAULT_TRANSMIT_TIMEOUT_MS 1250
#define IPJ_DEFAULT_RECEIVE_TIMEOUT_MS  1250

/**
 * IPJ_READER_CONTEXT is a HANDLE to the serial port.
 */
typedef void* IPJ_READER_CONTEXT;

/**
 * IPJ_READER_IDENTIFIER is a reference used by the platform to open serial port
 * IPJ_READER_IDENTIFIER is also used by report handler to allow the application
 * to associate reports with a given reader in systems with multiple readers.
 */
typedef void* IPJ_READER_IDENTIFIER;

/**
 * User application allocates or declares structure before connecting
 * or communicating with the IRI device.
 */
typedef struct _ipj_iri_device
{
    IPJ_READER_CONTEXT    reader_context;
    IPJ_READER_IDENTIFIER reader_identifier;
    uint32_t              receive_timeout_ms;

    /* Internal Only */
    uint8_t               sync_state;
    bool                  wait_for_response;
    uint32_t              frame_length;
    uint32_t              receive_index;
    uint8_t               receive_buffer[IPJ_RECEIVE_BUFFER_SIZE];

#if !defined(IRI_RX_ONLY)
    uint8_t               transmit_buffer[IPJ_TRANSMIT_BUFFER_SIZE];
#endif

} ipj_iri_device;

/*****************************************************************************
 *                                                                           *
 * IRI API - External handlers                                               *
 *                                                                           *
 *****************************************************************************/

/*
 * This function must be defined and implemented by the host application
 */
extern ipj_error ipj_report_handler(
        ipj_iri_device* iri_device,
        ipj_report_id report_id,
        uint32_t report_count_32,
        void* report);

/*****************************************************************************
 *                                                                           *
 * IRI API - Library Functions                                               *
 *                                                                           *
 *****************************************************************************/

/* HELPER MACRO */
#define IPJ_CLEAR_STRUCT(ipj_structure) memset(&ipj_structure, 0x00, sizeof(ipj_structure))

/* IRI API FUNCTIONS */
uint32_t  ipj_get_api_version(void);
ipj_error ipj_initialize_iri_device(ipj_iri_device* iri_device);
ipj_error ipj_connect(
        ipj_iri_device* iri_device,
        IPJ_READER_IDENTIFIER reader_identifier,
        ipj_connection_type connection_type,
        ipj_connection_params* params);
ipj_error ipj_disconnect(ipj_iri_device* iri_device);
ipj_error ipj_receive(ipj_iri_device* iri_device);


/*****************************************************************************
 *                                                                           *
 * IRI API                                                                   *
 *                                                                           *
 *****************************************************************************/
#if !defined(IRI_RX_ONLY)

ipj_error ipj_reset(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_reset_type  reset_type       /*[in]*/);

ipj_error ipj_set(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_key         key              /*[in]*/,
        uint32_t        bank_index       /*[in]*/,
        uint32_t        value_index      /*[in]*/,
        uint32_t        value            /*[in]*/);

ipj_error ipj_set_value(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_key         key              /*[in]*/,
        uint32_t        value            /*[in]*/);

ipj_error ipj_get(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_key         key              /*[in]*/,
        uint32_t        bank_index       /*[in]*/,
        uint32_t        value_index      /*[in]*/,
        uint32_t        *value           /*[out]*/);

ipj_error ipj_get_value(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_key         key              /*[in]*/,
        uint32_t        *value           /*[out]*/);

ipj_error ipj_start(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_action      action           /*[in]*/);

ipj_error ipj_stop(
        ipj_iri_device* iri_device       /*[in]*/,
        ipj_action      action           /*[in]*/);

#if defined (ENABLE_FW_UPDATES)
ipj_error ipj_flash_handle_loader_block(
        ipj_iri_device *iri_device       /*[in]*/,
        uint32_t        len              /*[in]*/,
        uint8_t*        data             /*[in]*/);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
