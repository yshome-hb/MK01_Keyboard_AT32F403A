
#include "usb_conf.h"
#include "diskio.h"
#include "usbh_msc_core.h"
/*--------------------------------------------------------------------------

Module Private Functions and Variables

---------------------------------------------------------------------------*/
DWORD get_fattime(void);

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
extern USB_HOST                     USB_Host;

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
                         BYTE drv		/* Physical drive number (0) */
                           )
{
  
  if(USB_HCD_IsDeviceConnected(&USB_OTG_Core_dev))
  {  
    Stat &= ~STA_NOINIT;
  }
  
  return Stat;
  
  
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
                     BYTE drv		/* Physical drive number (0) */
                       )
{
  if (drv) return STA_NOINIT;		/* Supports only single drive */
  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
                   BYTE pdrv,			/* Physical drive number (0) */
                   BYTE *buff,			/* Pointer to the data buffer to store read data */
                   DWORD sector,		/* Start sector number (LBA) */
                   UINT count			/* Sector count (1..255) */
                     )
{
  BYTE status = USBH_MSC_OK;
  
  if (pdrv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  
  if(USB_HCD_IsDeviceConnected(&USB_OTG_Core_dev))
  {  
    
    do
    {
      status = USBH_MSC_Read10(&USB_OTG_Core_dev, buff,sector,512 * count);
      USBH_MSC_HandleBOTXfer(&USB_OTG_Core_dev ,&USB_Host);
      
      if(!USB_HCD_IsDeviceConnected(&USB_OTG_Core_dev))
      { 
        return RES_ERROR;
      }      
    }
    while(status == USBH_MSC_BUSY );
  }
  
  if(status == USBH_MSC_OK)
    return RES_OK;
  return RES_ERROR;
  
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
                    BYTE pdrv,			/* Physical drive number (0) */
                    const BYTE *buff,	/* Pointer to the data to be written */
                    DWORD sector,		/* Start sector number (LBA) */
                    UINT count			/* Sector count (1..255) */
                      )
{
  BYTE status = USBH_MSC_OK;
  if (pdrv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;
  
  
  if(USB_HCD_IsDeviceConnected(&USB_OTG_Core_dev))
  {  
    do
    {
      status = USBH_MSC_Write10(&USB_OTG_Core_dev,(BYTE*)buff,sector,512 * count);
      USBH_MSC_HandleBOTXfer(&USB_OTG_Core_dev, &USB_Host);
      
      if(!USB_HCD_IsDeviceConnected(&USB_OTG_Core_dev))
      { 
        return RES_ERROR;
      }
    }
    
    while(status == USBH_MSC_BUSY );
    
  }
  
  if(status == USBH_MSC_OK)
    return RES_OK;
  return RES_ERROR;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
                    BYTE drv,		/* Physical drive number (0) */
                    BYTE ctrl,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
  DRESULT res = RES_OK;
  
  if (drv) return RES_PARERR;
  
  res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (ctrl) {
  case CTRL_SYNC :		/* Make sure that no pending write process */
    
    res = RES_OK;
    break;
    
  case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
    
    *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
    res = RES_OK;
    break;
    
  case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
    *(WORD*)buff = 512;
    res = RES_OK;
    break;
    
  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
    
    *(DWORD*)buff = 512;
    
    break;
    
    
  default:
    res = RES_PARERR;
  }
  
  
  
  return res;
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  return 0;
}

#endif /* _USE_IOCTL != 0 */
