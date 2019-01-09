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
#ifndef __CONFIG_H
#define __CONFIG_H

/*****************************************************************************
 *                                                                           *
 * COMPILE TIME SWITCHES                                                     *
 *                                                                           *
 *****************************************************************************/

/*
 * Note: Autobaud is intended for development purposes and can
 * only be used once per device power cycle
 */
/*#define IPJ_SERIAL_AUTOBAUD_CLOCK*/

/*
 * This switch will turn on printf's of serial traffic
 */
/*#define DEBUG_SERIAL_TRAFFIC*/

/*
 * This switch will enable frame counting
 */

/* #define IPJ_ENABLE_FRAME_COUNT */

#endif
