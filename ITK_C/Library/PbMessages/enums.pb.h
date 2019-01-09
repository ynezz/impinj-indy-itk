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
#ifndef _PB_ENUMS_PB_H_
#define _PB_ENUMS_PB_H_
#include <pb.h>


/* Enum definitions */
typedef uint32_t ipj_report_id;
#define E_IPJ_REPORT_ID_TAG_OPERATION_REPORT ((uint32_t)(0x2))
#define E_IPJ_REPORT_ID_STOP_REPORT ((uint32_t)(0x5))
#define E_IPJ_REPORT_ID_TEST_REPORT ((uint32_t)(0xC))
#define E_IPJ_REPORT_ID_ERROR_REPORT ((uint32_t)(0xD))
#define E_IPJ_REPORT_ID_STATUS_REPORT ((uint32_t)(0xE))
#define E_IPJ_REPORT_ID_GPIO_REPORT ((uint32_t)(0xF))
#define E_IPJ_REPORT_ID_IDD_REPORT ((uint32_t)(0x10))

typedef uint32_t ipj_handler_type;
#define E_IPJ_HANDLER_TYPE_PLATFORM_OPEN_PORT ((uint32_t)(0x0))
#define E_IPJ_HANDLER_TYPE_PLATFORM_CLOSE_PORT ((uint32_t)(0x1))
#define E_IPJ_HANDLER_TYPE_PLATFORM_TRANSMIT ((uint32_t)(0x2))
#define E_IPJ_HANDLER_TYPE_PLATFORM_RECEIVE ((uint32_t)(0x3))
#define E_IPJ_HANDLER_TYPE_PLATFORM_TIMESTAMP ((uint32_t)(0x4))
#define E_IPJ_HANDLER_TYPE_PLATFORM_SLEEP_MS ((uint32_t)(0x5))
#define E_IPJ_HANDLER_TYPE_REPORT ((uint32_t)(0x6))
#define E_IPJ_HANDLER_TYPE_DIAGNOSTIC ((uint32_t)(0x7))
#define E_IPJ_HANDLER_TYPE_PLATFORM_MODIFY_CONNECTION ((uint32_t)(0x8))
#define E_IPJ_HANDLER_TYPE_PLATFORM_FLUSH_PORT ((uint32_t)(0x9))

typedef uint32_t ipj_idle_power_mode;
#define E_IPJ_IDLE_POWER_MODE_STANDARD ((uint32_t)(0x0))
#define E_IPJ_IDLE_POWER_MODE_LOW_LATENCY ((uint32_t)(0x1))

typedef uint32_t ipj_region;
#define E_IPJ_REGION_FCC_PART_15_247 ((uint32_t)(0x0))
#define E_IPJ_REGION_HONG_KONG_920_925_MHZ ((uint32_t)(0x3))
#define E_IPJ_REGION_TAIWAN_922_928_MHZ ((uint32_t)(0x4))
#define E_IPJ_REGION_ETSI_EN_302_208_V1_4_1 ((uint32_t)(0x7))
#define E_IPJ_REGION_KOREA_917_921_MHZ ((uint32_t)(0x8))
#define E_IPJ_REGION_MALAYSIA_919_923_MHZ ((uint32_t)(0x9))
#define E_IPJ_REGION_CHINA_920_925_MHZ ((uint32_t)(0xA))
#define E_IPJ_REGION_SOUTH_AFRICA_915_919_MHZ ((uint32_t)(0xC))
#define E_IPJ_REGION_BRAZIL_902_907_AND_915_928_MHZ ((uint32_t)(0xD))
#define E_IPJ_REGION_THAILAND_920_925_MHZ ((uint32_t)(0xE))
#define E_IPJ_REGION_SINGAPORE_920_925_MHZ ((uint32_t)(0xF))
#define E_IPJ_REGION_AUSTRALIA_920_926_MHZ ((uint32_t)(0x10))
#define E_IPJ_REGION_INDIA_865_867_MHZ ((uint32_t)(0x11))
#define E_IPJ_REGION_URUGUAY_916_928_MHZ ((uint32_t)(0x12))
#define E_IPJ_REGION_VIETNAM_920_925_MHZ ((uint32_t)(0x13))
#define E_IPJ_REGION_ISRAEL_915_917_MHZ ((uint32_t)(0x14))
#define E_IPJ_REGION_PHILIPPINES_918_920_MHZ ((uint32_t)(0x15))
#define E_IPJ_REGION_INDONESIA_923_925_MHZ ((uint32_t)(0x17))
#define E_IPJ_REGION_NEW_ZEALAND_921P5_928_MHZ ((uint32_t)(0x18))
#define E_IPJ_REGION_JAPAN_916_921_MHZ_NO_LBT ((uint32_t)(0x19))
#define E_IPJ_REGION_PERU_916_928_MHZ ((uint32_t)(0x1A))
#define E_IPJ_REGION_RUSSIA_916_921_MHZ ((uint32_t)(0x1B))
#define E_IPJ_REGION_CUSTOM ((uint32_t)(0x100))

typedef uint32_t ipj_test_id;
#define E_IPJ_TEST_ID_NONE ((uint32_t)(0x0))
#define E_IPJ_TEST_ID_GENERIC ((uint32_t)(0x1))
#define E_IPJ_TEST_ID_BIST ((uint32_t)(0x2))
#define E_IPJ_TEST_ID_MEMORY_READ ((uint32_t)(0x3))
#define E_IPJ_TEST_ID_MEMORY_WRITE ((uint32_t)(0x4))
#define E_IPJ_TEST_ID_TRANSCEIVER_READ ((uint32_t)(0x5))
#define E_IPJ_TEST_ID_TRANSCEIVER_WRITE ((uint32_t)(0x6))
#define E_IPJ_TEST_ID_SET_FREQUENCY ((uint32_t)(0x7))
#define E_IPJ_TEST_ID_CW_CONTROL ((uint32_t)(0x8))
#define E_IPJ_TEST_ID_PRBS_CONTROL ((uint32_t)(0x9))
#define E_IPJ_TEST_ID_GEN2_TX_CONTROL ((uint32_t)(0xA))
#define E_IPJ_TEST_ID_TRANSCEIVER_READ_MODIFY_WRITE ((uint32_t)(0xB))
#define E_IPJ_TEST_ID_CALIBRATE_BEGIN ((uint32_t)(0x65))
#define E_IPJ_TEST_ID_CALIBRATE_END ((uint32_t)(0x66))
#define E_IPJ_TEST_ID_CALIBRATE_FORWARD_POWER ((uint32_t)(0x67))
#define E_IPJ_TEST_ID_CALIBRATE_REVERSE_POWER ((uint32_t)(0x68))
#define E_IPJ_TEST_ID_CALIBRATE_DC_OFFSET ((uint32_t)(0x69))
#define E_IPJ_TEST_ID_CALIBRATE_PA_BIAS ((uint32_t)(0x6A))
#define E_IPJ_TEST_ID_CALIBRATE_GROSS_GAIN ((uint32_t)(0x6B))
#define E_IPJ_TEST_ID_CALIBRATE_RSSI ((uint32_t)(0x6C))
#define E_IPJ_TEST_ID_CALIBRATE_TEMPERATURE ((uint32_t)(0x6D))
#define E_IPJ_TEST_ID_CALIBRATE_TX_PRE_DISTORTION ((uint32_t)(0x6E))
#define E_IPJ_TEST_ID_CALIBRATE_SET_KEY ((uint32_t)(0x6F))
#define E_IPJ_TEST_ID_CALIBRATE_FINALIZE_KEYS ((uint32_t)(0x70))
#define E_IPJ_TEST_ID_CALIBRATE_NOMINAL_GG ((uint32_t)(0x71))
#define E_IPJ_TEST_ID_HARDWARE_CONTROL ((uint32_t)(0xC9))
#define E_IPJ_TEST_ID_INVENTORY_CONTROL ((uint32_t)(0xCA))
#define E_IPJ_TEST_ID_GENERIC_TX_CONTROL ((uint32_t)(0xCB))
#define E_IPJ_TEST_ID_POWER_SCALAR_CONTROL ((uint32_t)(0xCC))
#define E_IPJ_TEST_ID_GROSS_GAIN_CONTROL ((uint32_t)(0xCD))
#define E_IPJ_TEST_ID_PA_BIAS_CONTROL ((uint32_t)(0xCE))
#define E_IPJ_TEST_ID_GENERIC_RX_CONTROL ((uint32_t)(0xCF))
#define E_IPJ_TEST_ID_USER_DAC_CONTROL ((uint32_t)(0xD0))
#define E_IPJ_TEST_ID_USER_ADC_CONTROL ((uint32_t)(0xD1))
#define E_IPJ_TEST_ID_AUX_DAC_CONTROL ((uint32_t)(0xD2))
#define E_IPJ_TEST_ID_AUX_ADC_CONTROL ((uint32_t)(0xD3))
#define E_IPJ_TEST_ID_GPIO_CONTROL ((uint32_t)(0xD4))
#define E_IPJ_TEST_ID_DEBUG_UART_CONTROL ((uint32_t)(0xD5))
#define E_IPJ_TEST_ID_TEMPERATURE_CONTROL ((uint32_t)(0xD6))
#define E_IPJ_TEST_ID_RF_PROFILE_CONTROL ((uint32_t)(0xD7))
#define E_IPJ_TEST_ID_SENSOR_CONTROL ((uint32_t)(0xD8))
#define E_IPJ_TEST_ID_CALIBRATION_CONTROL ((uint32_t)(0xD9))
#define E_IPJ_TEST_ID_ERROR_CONTROL ((uint32_t)(0xDA))
#define E_IPJ_TEST_ID_RF_SWITCH_CONTROL ((uint32_t)(0xDB))
#define E_IPJ_TEST_ID_GPIO32_CONTROL ((uint32_t)(0xDC))
#define E_IPJ_TEST_ID_DAC_CONTROL ((uint32_t)(0xDD))
#define E_IPJ_TEST_ID_ADC_CONTROL ((uint32_t)(0xDE))
#define E_IPJ_TEST_ID_DEBUG_INFO ((uint32_t)(0xDF))
#define E_IPJ_TEST_ID_TAG_REPORT_GENERATION ((uint32_t)(0xE0))
#define E_IPJ_TEST_ID_ANTENNA_CHARACTERIZATION ((uint32_t)(0xE1))
#define E_IPJ_TEST_ID_CLOSED_LOOP_POWER_CONTROL ((uint32_t)(0xE2))
#define E_IPJ_TEST_ID_SIMPLE_FORWARD_POWER_CALIBRATION ((uint32_t)(0xE3))
#define E_IPJ_TEST_ID_SIMPLE_GROSS_GAIN_CALIBRATION ((uint32_t)(0xE4))
#define E_IPJ_TEST_ID_STACK_USAGE ((uint32_t)(0xE5))

typedef uint32_t ipj_inventory_select_flag;
#define E_IPJ_INVENTORY_SELECT_FLAG_AUTO_SL ((uint32_t)(0x0))
#define E_IPJ_INVENTORY_SELECT_FLAG_ALL_SL ((uint32_t)(0x1))
#define E_IPJ_INVENTORY_SELECT_FLAG_NOT_SL ((uint32_t)(0x2))
#define E_IPJ_INVENTORY_SELECT_FLAG_SL ((uint32_t)(0x3))

typedef uint32_t ipj_inventory_search_mode;
#define E_IPJ_INVENTORY_SEARCH_MODE_AUTO_SEARCH ((uint32_t)(0x0))
#define E_IPJ_INVENTORY_SEARCH_MODE_DUAL_TARGET ((uint32_t)(0x1))
#define E_IPJ_INVENTORY_SEARCH_MODE_SINGLE_TARGET_A_TO_B ((uint32_t)(0x2))
#define E_IPJ_INVENTORY_SEARCH_MODE_SINGLE_TARGET_B_TO_A ((uint32_t)(0x3))

typedef uint32_t ipj_mem_bank;
#define E_IPJ_MEM_BANK_RESERVED ((uint32_t)(0x0))
#define E_IPJ_MEM_BANK_EPC ((uint32_t)(0x1))
#define E_IPJ_MEM_BANK_TID ((uint32_t)(0x2))
#define E_IPJ_MEM_BANK_USER ((uint32_t)(0x3))

typedef uint32_t ipj_select_target;
#define E_IPJ_SELECT_TARGET_SESSION_S0 ((uint32_t)(0x0))
#define E_IPJ_SELECT_TARGET_SESSION_S1 ((uint32_t)(0x1))
#define E_IPJ_SELECT_TARGET_SESSION_S2 ((uint32_t)(0x2))
#define E_IPJ_SELECT_TARGET_SESSION_S3 ((uint32_t)(0x3))
#define E_IPJ_SELECT_TARGET_SL_FLAG ((uint32_t)(0x4))
#define E_IPJ_SELECT_TARGET_RFU_1 ((uint32_t)(0x5))
#define E_IPJ_SELECT_TARGET_RFU_2 ((uint32_t)(0x6))
#define E_IPJ_SELECT_TARGET_RFU_3 ((uint32_t)(0x7))

typedef uint32_t ipj_baud_rate;
#define E_IPJ_BAUD_RATE_BR9600 ((uint32_t)(0x2580))
#define E_IPJ_BAUD_RATE_BR14400 ((uint32_t)(0x3840))
#define E_IPJ_BAUD_RATE_BR19200 ((uint32_t)(0x4B00))
#define E_IPJ_BAUD_RATE_BR38400 ((uint32_t)(0x9600))
#define E_IPJ_BAUD_RATE_BR57600 ((uint32_t)(0xE100))
#define E_IPJ_BAUD_RATE_BR115200 ((uint32_t)(0x1C200))
#define E_IPJ_BAUD_RATE_BR230400 ((uint32_t)(0x38400))
#define E_IPJ_BAUD_RATE_BR460800 ((uint32_t)(0x70800))
#define E_IPJ_BAUD_RATE_BR921600 ((uint32_t)(0xE1000))

typedef uint32_t ipj_parity;
#define E_IPJ_PARITY_PNONE ((uint32_t)(0x0))
#define E_IPJ_PARITY_PEVEN ((uint32_t)(0x1))
#define E_IPJ_PARITY_PODD ((uint32_t)(0x2))

typedef uint32_t ipj_select_action;
#define E_IPJ_SELECT_ACTION_ASLINVA_DSLINVB ((uint32_t)(0x0))
#define E_IPJ_SELECT_ACTION_ASLINVA_NOTHING ((uint32_t)(0x1))
#define E_IPJ_SELECT_ACTION_NOTHING_DSLINVB ((uint32_t)(0x2))
#define E_IPJ_SELECT_ACTION_NSLINVS_NOTHING ((uint32_t)(0x3))
#define E_IPJ_SELECT_ACTION_DSLINVB_ASLINVA ((uint32_t)(0x4))
#define E_IPJ_SELECT_ACTION_DSLINVB_NOTHING ((uint32_t)(0x5))
#define E_IPJ_SELECT_ACTION_NOTHING_ASLINVA ((uint32_t)(0x6))
#define E_IPJ_SELECT_ACTION_NOTHING_NSLINVS ((uint32_t)(0x7))

typedef uint32_t ipj_blockpermalock_action;
#define E_IPJ_BLOCKPERMALOCK_ACTION_READ ((uint32_t)(0x0))
#define E_IPJ_BLOCKPERMALOCK_ACTION_PERMALOCK ((uint32_t)(0x1))

typedef uint32_t ipj_write_epc_length_control;
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_AUTO ((uint32_t)(0x0))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_USER_VALUE ((uint32_t)(0x1))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_ZERO ((uint32_t)(0x2))
#define E_IPJ_WRITE_EPC_LENGTH_CONTROL_NO_CHANGE ((uint32_t)(0x3))

typedef uint32_t ipj_qt_action;
#define E_IPJ_QT_ACTION_READ ((uint32_t)(0x0))
#define E_IPJ_QT_ACTION_WRITE ((uint32_t)(0x1))

typedef uint32_t ipj_qt_persistence;
#define E_IPJ_QT_PERSISTENCE_TEMPORARY ((uint32_t)(0x0))
#define E_IPJ_QT_PERSISTENCE_PERMANENT ((uint32_t)(0x1))

typedef uint32_t ipj_qt_data_profile;
#define E_IPJ_QT_DATA_PROFILE_PRIVATE ((uint32_t)(0x0))
#define E_IPJ_QT_DATA_PROFILE_PUBLIC ((uint32_t)(0x1))

typedef uint32_t ipj_qt_access_range;
#define E_IPJ_QT_ACCESS_RANGE_NORMAL ((uint32_t)(0x0))
#define E_IPJ_QT_ACCESS_RANGE_SHORT ((uint32_t)(0x1))

typedef uint32_t ipj_status_flag;
#define E_IPJ_STATUS_FLAG_BIT_CHANNEL_ACTIVITY ((uint32_t)(0x1))
#define E_IPJ_STATUS_FLAG_BIT_IDD ((uint32_t)(0x2))

typedef uint32_t ipj_gpi_action;
#define E_IPJ_GPI_ACTION_NONE ((uint32_t)(0x0))
#define E_IPJ_GPI_ACTION_START_INVENTORY ((uint32_t)(0x1))
#define E_IPJ_GPI_ACTION_STOP_INVENTORY ((uint32_t)(0x2))

typedef uint32_t ipj_gpo_action;
#define E_IPJ_GPO_ACION_TAG_IN_FIELD ((uint32_t)(0x0))

typedef uint32_t ipj_gpio_mode;
#define E_IPJ_GPIO_MODE_DISABLED ((uint32_t)(0x0))
#define E_IPJ_GPIO_MODE_INPUT ((uint32_t)(0x1))
#define E_IPJ_GPIO_MODE_OUTPUT ((uint32_t)(0x2))
#define E_IPJ_GPIO_MODE_OUTPUT_PULSE ((uint32_t)(0x3))
#define E_IPJ_GPIO_MODE_INPUT_ACTION ((uint32_t)(0x4))
#define E_IPJ_GPIO_MODE_OUTPUT_ACTION ((uint32_t)(0x5))
#define E_IPJ_GPIO_MODE_OUTPUT_PULSE_ACTION ((uint32_t)(0x6))

typedef uint32_t ipj_gpio_state;
#define E_IPJ_GPIO_STATE_LO ((uint32_t)(0x0))
#define E_IPJ_GPIO_STATE_HI ((uint32_t)(0x1))
#define E_IPJ_GPIO_STATE_FLOAT ((uint32_t)(0x2))

typedef uint32_t ipj_product_id;
#define E_IPJ_PRODUCT_ID_NONE ((uint32_t)(0x0))
#define E_IPJ_PRODUCT_ID_RS500 ((uint32_t)(0x1))
#define E_IPJ_PRODUCT_ID_RESERVED_2 ((uint32_t)(0x2))
#define E_IPJ_PRODUCT_ID_RS2000 ((uint32_t)(0x3))
#define E_IPJ_PRODUCT_ID_IRD_1 ((uint32_t)(0x4))
#define E_IPJ_PRODUCT_ID_RS1000 ((uint32_t)(0x5))

typedef uint32_t ipj_product_sku;
#define E_IPJ_PRODUCT_SKU_NONE ((uint32_t)(0x0))
#define E_IPJ_PRODUCT_SKU_1 ((uint32_t)(0x1))
#define E_IPJ_PRODUCT_SKU_2 ((uint32_t)(0x2))
#define E_IPJ_PRODUCT_SKU_3 ((uint32_t)(0x3))

typedef uint32_t ipj_antenna;
#define E_IPJ_ANTENNA_AUTO ((uint32_t)(0x0))
#define E_IPJ_ANTENNA_1 ((uint32_t)(0x1))
#define E_IPJ_ANTENNA_2 ((uint32_t)(0x2))
#define E_IPJ_ANTENNA_3 ((uint32_t)(0x3))
#define E_IPJ_ANTENNA_4 ((uint32_t)(0x4))

typedef uint32_t ipj_antenna_rs2000;
#define E_IPJ_ANTENNA_RS2000_AUTO ((uint32_t)(0x0))
#define E_IPJ_ANTENNA_RS2000_SMT_1 ((uint32_t)(0x1))
#define E_IPJ_ANTENNA_RS2000_SMT_2 ((uint32_t)(0x2))
#define E_IPJ_ANTENNA_RS2000_UFL_1 ((uint32_t)(0x3))
#define E_IPJ_ANTENNA_RS2000_UFL_2 ((uint32_t)(0x4))

/* Struct definitions */
/* Default values for struct fields */

/* Struct field encoding specification for nanopb */

#endif
