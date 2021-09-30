#ifndef __FLASH_H__
#define __FLASH_H__

#include "at32f4xx.h"

#define FLASH_SIZE	    ((uint32_t)0x40000)   //256*1024
#define APP_SIZE		((uint32_t)0x18000)   //96*1024
#define OTA_SIZE		((uint32_t)0x4000)    //16*1024
#define DATA_SIZE		((uint32_t)0x4000)    //16*1024
#define PAGE_SIZE		((uint32_t)0x800)     //2*1024

#define FLASH_APP_ADDR		(FLASH_BASE + OTA_SIZE)  				// APP Area: 0x08004000~0x0801BFFF
#define FLASH_BKP_ADDR		(FLASH_APP_ADDR + APP_SIZE)  			// BKP Area: 0x0801C000~0x08033FFF
#define FLASH_BKP_END_ADDR	(FLASH_BKP_ADDR + APP_SIZE)
#define FLASH_DATA_ADDR	    (FLASH_BASE + (uint32_t)0x3C000)        // DATA Area: 0x0803C000~0x0803FFFF


static inline uint16_t flash_read_halfword(uint32_t addr)
{
	return *(vu16*)addr; 
}


static inline uint32_t flash_read_word(uint32_t addr)
{
	return *(vu32*)addr; 
}


int flash_write_bytes(uint32_t addr, uint8_t *pdata, uint32_t len);


#endif
