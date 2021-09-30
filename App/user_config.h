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

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define MANUFACTURER                "xbiao"
#define PRODUCT                     "XWINDER-4CUPS"

/* user modle name & user mcu version number*/
#define USER_MODEL                  "xbiao.winder.834717"
#define USER_MCU_VERSION            "0001"

#define BLE_PID                     "9543"
#define HAVE_BLE                     (1)

/* log uart output level */
#define MIIO_LOG_LEVEL              LOG_LEVEL_OFF //LOG_LEVEL_VERBOSE

/* enable exit signal to exit loop on linux platform */
#define USER_SIG_FUNC_ENABLE        (0)

/* if mcu system support os */
#define USER_OS_ENABLE              (1)

/* user ota configurations */
#define USER_OTA_ENABLE             (1)
#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
#define XMODEM_PRINT_LOG            (1)
#endif

/* print uart uplink and downlink messages */
#define USER_UART_PRINT_ENABLE      (0)
#if (defined(USER_UART_PRINT_ENABLE) && USER_UART_PRINT_ENABLE)
#define PRINT_SEND_BUFF             (1)
#define PRINT_RECV_BUFF             (1)
#else
#define PRINT_SEND_BUFF             (0)
#define PRINT_RECV_BUFF             (0)
#endif

/* user uart configurations */
#define USER_UART_RXBUF_SIZE        (256)
#define USER_UART_TXBUF_SIZE        (256)
#define USER_POLL_INTERVAL_MS       (200)
#define USER_UART_TIMEOUT_MS        (200)
#define USER_RECEIVE_RETRY          (25)

/* buffer used to receive command string from wifi module */
#define CMD_STR_MAX_LEN             (800) //max 800 byte
/* max method name register with miio_cmd_method_register() function */
#define CMD_METHOD_LEN_MAX          (64)
/* buffer used to response command execute result */
#define RESULT_BUF_SIZE             CMD_STR_MAX_LEN

#define ACK_BUF_SIZE                CMD_STR_MAX_LEN
#define DATA_STRING_MAX_LENGTH      (800)

#define RINGBUFF_LEN                (1000) //USER_UART_RXBUF_SIZE

/* user wifi ssid&passwd, used in app_func_setwifi() */
#define USER_SSID                   "user_wifi"
#define USER_PASSWD                 "12345678"

#define ERROR_MESSAGE_UNCMD         "undefined command"
#define ERROR_MESSAGE_UNPARAMS      "error command params"
#define ERROR_CODE_UNCMD            -9999
#define ERROR_CODE_UNPARAMS         -9998
#endif  /* __USER_CONFIG_H__ */
