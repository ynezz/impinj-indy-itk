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

#ifndef __VERSION_H
#define __VERSION_H

#include <stdint.h>

#define IPJ_VERSION_MAJOR_NUMBER         1
#define IPJ_VERSION_MINOR_NUMBER         8
#define IPJ_VERSION_MAINTENANCE_NUMBER   12
#define IPJ_VERSION_RELEASE_NUMBER       240

#define IPJ_VERSION_STRING "1.8.12.240"

/* VERSION 0x01080CF0 */
#define IPJ_VERSION   (((uint32_t)IPJ_VERSION_MAJOR_NUMBER       << 24)  | \
                       ((uint32_t)IPJ_VERSION_MINOR_NUMBER       << 16)  | \
                       ((uint32_t)IPJ_VERSION_MAINTENANCE_NUMBER <<  8)  | \
                       ((uint32_t)IPJ_VERSION_RELEASE_NUMBER     <<  0))

#endif
