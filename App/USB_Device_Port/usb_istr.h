/**
  ******************************************************************************
  * File   : USB_Device/Custom_HID/inc/usb_istr.h
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : This file includes the peripherals header files in the user application.
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_ISTR_H
#define __USB_ISTR_H
/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "at32f4xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USB_Istr(void);
/* function prototypes Automatically built defining related macros */
#ifdef CTR_CALLBACK
void CTR_Callback(void);
#endif
#ifdef DOVR_CALLBACK
void DOVR_Callback(void);
#endif
#ifdef ERR_CALLBACK
void ERR_Callback(void);
#endif
#ifdef WKUP_CALLBACK
void WKUP_Callback(void);
#endif
#ifdef SUSP_CALLBACK
void SUSP_Callback(void);
#endif
#ifdef RESET_CALLBACK
void RESET_Callback(void);
#endif
#ifdef SOF_CALLBACK
void SOF_Callback(void);
#endif
#ifdef ESOF_CALLBACK
void ESOF_Callback(void);
#endif

void EP1_IN_Callback(void);
void EP2_IN_Callback(void);
void EP3_IN_Callback(void);
void EP4_IN_Callback(void);
void EP5_IN_Callback(void);
void EP6_IN_Callback(void);
void EP7_IN_Callback(void);

void EP1_OUT_Callback(void);
void EP2_OUT_Callback(void);
void EP3_OUT_Callback(void);
void EP4_OUT_Callback(void);
void EP5_OUT_Callback(void);
void EP6_OUT_Callback(void);
void EP7_OUT_Callback(void);

void EP1_Respond_Data(uint8_t *res_buf, uint32_t len);
#endif /*__USB_ISTR_H*/


