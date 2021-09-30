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

#ifndef __ARCH_OTA_H__
#define __ARCH_OTA_H__

/**
 * @brief  enter for user function for ota(called when xmodem received data)
 *
 * @param[in]  pbuf: xmodem data pointer
 *             len: data length
 *             sum: total data length received so far
 * @return
 *             - MIIO_OK
 *             - error code
 */
int arch_ota_func(unsigned char *pbuf, size_t len, size_t sum);

#endif	/* __ARCH_OTA_H__ */

