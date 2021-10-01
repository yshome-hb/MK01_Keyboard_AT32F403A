/**
  ******************************************************************************
  * File   : usbd_msc_core.c
  * Version: V1.3.2
  * Date   : 2021-08-08
  * Brief  : his file provides all the MSC core functions.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_mem.h"
#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
#include "usbd_req.h"


/** @addtogroup AT32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup MSC_CORE 
  * @brief Mass storage core module
  * @{
  */ 

/** @defgroup MSC_CORE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup MSC_CORE_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup MSC_CORE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup MSC_CORE_Private_FunctionPrototypes
  * @{
  */ 
uint8_t  USBD_MSC_Init (void  *pdev, 
                            uint8_t cfgidx);

uint8_t  USBD_MSC_DeInit (void  *pdev, 
                              uint8_t cfgidx);

uint8_t  USBD_MSC_Setup (void  *pdev, 
                             USB_SETUP_REQEST *req);

uint8_t  USBD_MSC_DataIn (void  *pdev, 
                              uint8_t epnum);


uint8_t  USBD_MSC_DataOut (void  *pdev, 
                               uint8_t epnum);

uint8_t  *USBD_MSC_GetCfgDesc (uint8_t speed, 
                                      uint16_t *length);

#ifdef USB_OTG_HS_CORE  
uint8_t  *USBD_MSC_GetOtherCfgDesc (uint8_t speed, 
                                      uint16_t *length);
#endif


uint8_t USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ];




/**
  * @}
  */ 


/** @defgroup MSC_CORE_Private_Variables
  * @{
  */ 


USBD_Class_cb_Type  USBD_MSC_cb = 
{
  USBD_MSC_Init,
  USBD_MSC_DeInit,
  USBD_MSC_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_MSC_DataIn,
  USBD_MSC_DataOut,
  NULL, /*SOF */ 
  NULL,  
  NULL,     
  USBD_MSC_GetCfgDesc,
#ifdef USB_OTG_HS_CORE  
  USBD_MSC_GetOtherCfgDesc,
#endif
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN uint8_t USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
{
  
  0x09,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  USB_MSC_CONFIG_DESC_SIZ,
  
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: */
  0x04,   /* iConfiguration: */
  0xC0,   /* bmAttributes: */
  0x32,   /* MaxPower 100 mA */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */
  0x06,   /* bInterfaceSubClass : SCSI transparent*/
  0x50,   /* nInterfaceProtocol */
  0x05,          /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  MSC_IN_EP,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_PACKET),
  HIBYTE(MSC_MAX_PACKET),
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_PACKET),
  HIBYTE(MSC_MAX_PACKET),
  0x00     /*Polling interval in milliseconds*/
};
#ifdef USB_OTG_HS_CORE 
 #ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
   #if defined ( __ICCARM__ ) /*!< IAR Compiler */
     #pragma data_alignment=4   
   #endif
 #endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USBD_MSC_OtherCfgDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
{
  
  0x09,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,   
  USB_MSC_CONFIG_DESC_SIZ,
  
  0x00,
  0x01,   /* bNumInterfaces: 1 interface */
  0x01,   /* bConfigurationValue: */
  0x04,   /* iConfiguration: */
  0xC0,   /* bmAttributes: */
  0x32,   /* MaxPower 100 mA */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */
  0x06,   /* bInterfaceSubClass : SCSI transparent command set*/
  0x50,   /* nInterfaceProtocol */
  0x05,          /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  MSC_IN_EP,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00     /*Polling interval in milliseconds*/
};
#endif 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint8_t  USBD_MSC_MaxLun  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint8_t  USBD_MSC_AltSet  __ALIGN_END = 0;

/**
  * @}
  */ 


/** @defgroup MSC_CORE_Private_Functions
  * @{
  */ 

/**
* @brief  USBD_MSC_Init
*         Initialize  the mass storage configuration
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/
uint8_t  USBD_MSC_Init (void  *pdev, 
                            uint8_t cfgidx)
{
  USBD_MSC_DeInit(pdev , cfgidx );
  
  /* Open EP IN */
  USB_DCD_EPT_Open(pdev,
              MSC_IN_EP,
              MSC_EPIN_SIZE,
              USB_OTG_EPT_BULK);
  
  /* Open EP OUT */
  USB_DCD_EPT_Open(pdev,
              MSC_OUT_EP,
              MSC_EPOUT_SIZE,
              USB_OTG_EPT_BULK);
 
  /* Init the BOT  layer */
  MSC_BOT_Init(pdev); 
  
  return USB_DEVICE_OK;
}

/**
* @brief  USBD_MSC_DeInit
*         DeInitilaize  the mass storage configuration
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/
uint8_t  USBD_MSC_DeInit (void  *pdev, 
                              uint8_t cfgidx)
{
  /* Close MSC EPs */
  USB_DCD_EPT_Close (pdev , MSC_IN_EP);
  USB_DCD_EPT_Close (pdev , MSC_OUT_EP);
  
  /* Un Init the BOT layer */
  MSC_BOT_DeInit(pdev);   
  return USB_DEVICE_OK;
}
/**
* @brief  USBD_MSC_Setup
*         Handle the MSC specific requests
* @param  pdev: device instance
* @param  req: USB request
* @retval status
*/
uint8_t  USBD_MSC_Setup (void  *pdev, USB_SETUP_REQEST *req)
{
  
  switch (req->bmRequest & USB_REQUEST_TYPE_MASK)
  {

  /* Class request */
  case USB_REQUEST_TYPE_CLASS :
    switch (req->bRequest)
    {
    case BOT_GET_MAX_LUN :

      if((req->wValue  == 0) && 
         (req->wLength == 1) &&
         ((req->bmRequest & 0x80) == 0x80))
      {
        USBD_MSC_MaxLun = USBD_STORAGE_fops->GetMaxLun();
        if((int8_t)USBD_MSC_MaxLun >= 0)
        {
           USB_Device_CtrlSendData (pdev,
                             &USBD_MSC_MaxLun,
                              1);
        }
        else
        {
          USB_Device_CtlError(pdev , req);
          return USB_DEVICE_FAIL; 
          
        }
      }
      else
      {
         USB_Device_CtlError(pdev , req);
         return USB_DEVICE_FAIL; 
      }
      break;
      
    case BOT_RESET :
      if((req->wValue  == 0) && 
         (req->wLength == 0) &&
        ((req->bmRequest & 0x80) != 0x80))
      {      
         MSC_BOT_Reset(pdev);
      }
      else
      {
         USB_Device_CtlError(pdev , req);
         return USB_DEVICE_FAIL; 
      }
      break;

    default:
       USB_Device_CtlError(pdev , req);
       return USB_DEVICE_FAIL; 
    }
    break;
  /* Interface & Endpoint request */
  case USB_REQUEST_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQUEST_GET_INTERFACE :
      USB_Device_CtrlSendData (pdev,
                        &USBD_MSC_AltSet,
                        1);
      break;
      
    case USB_REQUEST_SET_INTERFACE :
      USBD_MSC_AltSet = (uint8_t)(req->wValue);
      break;
    
    case USB_REQUEST_CLEAR_FEATURE:  
      
      /* Flush the FIFO and Clear the stall status */    
      USB_DCD_EPT_Flush(pdev, (uint8_t)req->wIndex);
      
      /* Re-activate the EP */      
      USB_DCD_EPT_Close (pdev , (uint8_t)req->wIndex);
      if((((uint8_t)req->wIndex) & 0x80) == 0x80)
      {
        USB_DCD_EPT_Open(pdev,
                    ((uint8_t)req->wIndex),
                    MSC_EPIN_SIZE,
                    USB_OTG_EPT_BULK);
      }
      else
      {
        USB_DCD_EPT_Open(pdev,
                    ((uint8_t)req->wIndex),
                    MSC_EPOUT_SIZE,
                    USB_OTG_EPT_BULK);
      }
      
      /* Handle BOT error */
      MSC_BOT_CplClrFeature(pdev, (uint8_t)req->wIndex);
      break;
      
    }  
    break;
   
  default:
    break;
  }
  return USB_DEVICE_OK;
}

/**
* @brief  USBD_MSC_DataIn
*         handle data IN Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
uint8_t  USBD_MSC_DataIn (void  *pdev, 
                              uint8_t epnum)
{
  MSC_BOT_DataIn(pdev , epnum);
  return USB_DEVICE_OK;
}

/**
* @brief  USBD_MSC_DataOut
*         handle data OUT Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
uint8_t  USBD_MSC_DataOut (void  *pdev, 
                               uint8_t epnum)
{
  MSC_BOT_DataOut(pdev , epnum);
  return USB_DEVICE_OK;
}

/**
* @brief  USBD_MSC_GetCfgDesc 
*         return configuration descriptor
* @param  speed : current device speed
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_MSC_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_MSC_CfgDesc);
  return USBD_MSC_CfgDesc;
}

/**
* @brief  USBD_MSC_GetOtherCfgDesc 
*         return other speed configuration descriptor
* @param  speed : current device speed
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
#ifdef USB_OTG_HS_CORE  
uint8_t  *USBD_MSC_GetOtherCfgDesc (uint8_t speed, 
                                      uint16_t *length)
{
  *length = sizeof (USBD_MSC_OtherCfgDesc);
  return USBD_MSC_OtherCfgDesc;
}
#endif
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

