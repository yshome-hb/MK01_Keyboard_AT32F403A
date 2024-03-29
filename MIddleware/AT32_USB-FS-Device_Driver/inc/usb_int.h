/**
  ******************************************************************************
  * File   : usb_int.h
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : Endpoint CTRF (Low and High) interrupt's service routines prototypes
  ******************************************************************************
  */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_INT_H
#define __USB_INT_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CTR_LP(void);
void CTR_HP(void);

/* External variables --------------------------------------------------------*/

#endif /* __USB_INT_H */


