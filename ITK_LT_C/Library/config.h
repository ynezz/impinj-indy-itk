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
#ifndef __CONFIG_H
#define __CONFIG_H

/*****************************************************************************
 *                                                                           *
 * COMPILE TIME SWITCHES                                                     *
 *                                                                           *
 *****************************************************************************/

/*
 * This switch will eliminate all command and transmission APIs from the
 * library. To be used in conjunction with the 'on boot start action' key
 * in stored settings.
 */
/*#define IRI_RX_ONLY*/

/*
 * This switch will turn off all responses to set commands
 */
/*#define SUPPRESS_SET_RESPONSES*/

/*
 * This switch will turn on printf's of serial traffic
 */
/*#define DEBUG_SERIAL_TRAFFIC*/

/*
 * This switch will enable the firmware update API
 */
/*#define ENABLE_FW_UPDATES*/


#endif
