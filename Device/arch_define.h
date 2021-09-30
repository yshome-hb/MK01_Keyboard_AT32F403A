/**
 * @author  songyu
 * @date    2019
 * @par     Copyright (c):
 *
 *    Copyright 2019 MIoT,MI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */


#ifndef __ARCH_DEFINE_H__
#define __ARCH_DEFINE_H__

/* add ***all*** user header files here */
/* and all header files will be included in ***miio_define.h*** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <time.h>
//#include <termios.h>
//#include <fcntl.h>
//#include <unistd.h>
#include <ctype.h>

#include "at32f4xx.h"
#include "delay.h"
#include "user_config.h" /* introduce USER_OS_ENABLE macro */

#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
#include <FreeRTOS.h>
#include "task.h"
#include "semphr.h"
#endif

#ifndef arch_printf
#define arch_printf						printf
#endif

#ifndef __cplusplus

#ifndef BOOL
#define BOOL							bool
#endif

#ifndef TRUE
#define TRUE							true
#endif

#ifndef FALSE
#define FALSE							false
#endif

#endif

#ifndef NULL
#define NULL 							((void *)0)
#endif

#ifndef uint32_t
#define uint32_t						unsigned int
#endif

#ifndef int32_t
#define int32_t							int
#endif

#ifndef uint8_t
#define uint8_t							unsigned char
#endif

#ifndef int8_t
#define int8_t							char
#endif

#define arch_usleep(us)					delay_us(us)
#define arch_msleep(ms)					delay_ms(ms)

#define arch_memset(str, val, len)		memset(str, val, len)
#define arch_memcpy(dst, src, len)		memcpy(dst, src, len)

#define arch_malloc(len)				pvPortMalloc(len)
#define arch_calloc(num, len)			calloc(num, len)
#define arch_free(ptr)				    vPortFree(ptr)

#define arch_strtok(str, temp)			strtok(str, temp)

#endif /* __ARCH_DEFINE_H__ */
