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

#ifndef __ARCH_UART_H
#define __ARCH_UART_H

#include "miio_uart.h"


/**
 * @brief  init uart for mcu platform
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 * @return
 *             - UART_OK: init success
 *             - UART_OPEN_ERROR: init failed
 *             - UART_SET_ARRT_ERROR: init failed
 * @note
 *      - uart parameters must be baudrate: 115200, 8N1
 */
uart_error_t _uart_init(miio_uart_t *uart);
/**
 * @brief  send string data by mcu uart
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 *             str: string pointer
 * @return
 *             - >=0: string length sended
 *             - UART_SEND_ERROR: send failed
 */
int _uart_send_str(miio_uart_t *uart, const char* str);
/**
 * @brief  send byte data by mcu uart
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 *             c: byte data
 * @return
 *             - ==1: byte length sended
 *             - UART_SEND_ERROR: send failed
 */
int _uart_send_byte(miio_uart_t *uart, const char c);
/**
 * @brief  send string data by mcu uart, and check if receive "ok" from wifi module
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 *             str: string pointer
 * @return
 *             - >=0: string length sended
 *             - UART_SEND_ERROR: send failed
 *             - UART_RECV_ACK_ERROR: not recv "ok"
 */
int _uart_send_str_wait_ack(miio_uart_t *uart, const char* str);
/**
 * @brief  receive string data from mcu uart
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 *             str: string buffer to strore data
 *             timeout_ms: overtime in ms
 * @return
 *             - n_read: string length received
 */
int _uart_recv_str(miio_uart_t *uart, uint8_t* buf, int buf_len, uint32_t timeout_ms);
int _Xmodem_recv_str(miio_uart_t *uart, uint8_t* buf, uint32_t timeout_ms);
/**
 * @brief  receive byte data from mcu uart
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 *             str: byte buffer to strore data
 *             timeout_ms: overtime in ms
 * @return
 *             - n_read: byte length received
 */
int _uart_recv_byte(miio_uart_t *uart, uint8_t* c, uint32_t timeout_ms);
/**
 * @brief  deinit uart
 *
 * @param[in]  uart: miio_uart_t pointer(initialized when create miio instance)
 * @return     none
 */
void _uart_deinit(miio_uart_t *uart);
void _uart_interrupt(uint8_t enable);

#endif

