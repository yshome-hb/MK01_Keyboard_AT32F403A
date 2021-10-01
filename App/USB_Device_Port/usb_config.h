/**
  ******************************************************************************
  * File   : USB_Device_Port/usb_config.h
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : USB Hardware Configuration & setup header
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONFIG_H
#define __USB_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <at32f4xx.h>
#include "usb_type.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define USB_FIFO_MAX     1024
#define USBCLK_FROM_HSE  1
#define USBCLK_FROM_HSI  2

typedef struct _usb_usart_fifo
{
  uint8_t  fifo[USB_FIFO_MAX];
  uint16_t wrpointer;      /*write pointer*/
  uint16_t curpointer;      /*cur pointer*/
  uint16_t remaindpointer; /*remaind pointer*/
  uint16_t total;
}usb_usart_fifo;

extern usb_usart_fifo usb_txfifo;
extern usb_usart_fifo usb_rxfifo;

/* Exported functions ------------------------------------------------------- */
void USB_GPIO_init(void);
void Set_USBClock(u8 Clk_Source);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
uint16_t CDC_Send_DATA (uint8_t *u8SendBuffer, uint16_t u16Sendlen);
uint16_t CDC_Receive_DATA(uint8_t *u8RcvBuffer, uint16_t u16RecvLen);
void Get_SerialNum(void);
#endif  /*__HW_CONFIG_H*/


