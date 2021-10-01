/**
  ******************************************************************************
  * File   : usbh_msc_core.h
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : This file contains all the prototypes for the usbh_msc_core.c
  ******************************************************************************
  */    
  

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_MSC_CORE_H
#define __USBH_MSC_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_stdreq.h"
#include "usb_bsp.h"
#include "usbh_ioreq.h"
#include "usbh_hcs.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */
  
/** @defgroup USBH_MSC_CORE
  * @brief This file is the Header file for usbh_msc_core.c
  * @{
  */ 


/** @defgroup USBH_MSC_CORE_Exported_Types
  * @{
  */ 


/* Structure for MSC process */
typedef struct _MSC_Process
{
  uint8_t              hch_num_in; 
  uint8_t              hch_num_out; 
  uint8_t              MSBulkOutEp;
  uint8_t              MSBulkInEp;
  uint16_t             MSBulkInEpSize;
  uint16_t             MSBulkOutEpSize;
  uint8_t              buff[USBH_MSC_MPS_SIZE];
  uint8_t              maxLun;
}
MSC_Machine_TypeDef; 


/**
  * @}
  */ 



/** @defgroup USBH_MSC_CORE_Exported_Defines
  * @{
  */

#define USB_REQ_BOT_RESET                0xFF
#define USB_REQ_GET_MAX_LUN              0xFE
    

/**
  * @}
  */ 

/** @defgroup USBH_MSC_CORE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_MSC_CORE_Exported_Variables
  * @{
  */ 
extern USB_Host_Class_cb_Type  USBH_MSC_cb;
extern MSC_Machine_TypeDef    MSC_Machine;
extern uint8_t MSCErrorCount;

/**
  * @}
  */ 

/** @defgroup USBH_MSC_CORE_Exported_FunctionsPrototype
  * @{
  */ 



/**
  * @}
  */ 

#endif  /* __USBH_MSC_CORE_H */


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */



