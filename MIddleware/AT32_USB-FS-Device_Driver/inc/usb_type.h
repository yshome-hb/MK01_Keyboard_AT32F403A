/**
  ******************************************************************************
  * File   : usb_type.h 
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : Type definitions used by the USB Library
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_TYPE_H
#define __USB_TYPE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef NULL
#define NULL ((void *)0)
#endif

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif /* __USB_TYPE_H */


