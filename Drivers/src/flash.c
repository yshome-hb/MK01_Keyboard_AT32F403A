#include "flash.h"


int flash_write_bytes(uint32_t addr, uint8_t *pdata, uint32_t len)   	
{
    uint32_t *pdata32 = (uint32_t *)pdata;
	uint32_t addr_index = addr;
	uint32_t addr_end = addr + len;	

  	if(addr_index < FLASH_BASE || addr_end > (FLASH_BASE + FLASH_SIZE) || (addr & 0x03) || (!len))
		return -1;	
	
	FLASH_ClearFlag(FLASH_FLAG_PRGMFLR);
	FLASH_Unlock();
	FLASH_ErasePage(addr);

	while(addr_index < addr_end){
        FLASH_ProgramWord(addr_index, *pdata32);
		addr_index += 4;
        pdata32++;
	}

	FLASH_Lock();
	return 0;
}


// bool flash_read_bytes(uint32_t addr, uint8_t *pdata, uint32_t len)   	
// {
// 	uint32_t i = 0;
// 	uint32_t mod = len & (~((uint32_t)0x03));
// 	uint32_t sur = len & 0x03;

//   	if(addr < FLASH_BASE || addr > (FLASH_BASE + FLASH_SIZE) || (addr & 0x03) || (!len))
// 		return false;	
	

// 	while(i < mod){
// 		*((uint32_t *)pdata) = FLASH_word(addr);
// 		pdata += 4;
// 		addr += 4;
// 		i += 4;
// 	}

// 	uint32_t tmp = FLASH_word(addr+i);	
// 	memcpy(pdata, &tmp, sur);
	
// 	return true;
// }


// void Flash_Write(u32 wAddr, u16 *pBuffer, u16 hwNum2Write)
// {
// 	u32 wAddrTemp=wAddr;
// 	u16 i;
	
// 	for(i=0; i<hwNum2Write; i++)
// 	{
// 		FLASH_ProgramHalfWord(wAddrTemp, pBuffer[i]);
// 		wAddrTemp += 2;
// 	}
// }

// void Flash_Read(u32 wAddr, u16 *pBuffer, u16 hwNum2Read)
// {
// 	u32 wAddrTemp=wAddr;
// 	u16 i;
	
// 	for(i=0; i<hwNum2Read; i++)
// 	{
// 		pBuffer[i] = Flash_ReadHalfWord(wAddrTemp);
// 		wAddrTemp += 2;	
// 	}
// }

// void FLASH_Update(void)
// {
// 	u16 i;
// 	u32 wPageNum=APP_SIZE/PAGE_SIZE;
// 	u32 wPageCnt=0;
// 	u32 wAddrOffset;
	
// 	g_bFlashStatus=SUCCESS;
// 	FLASH_Unlock();
// 	do{
// 		wAddrOffset = wPageCnt*PAGE_SIZE;
// 		Flash_Read(FLASH_BKP_ADDR+wAddrOffset, hwBuffer1, PAGE_SIZE/2);
// 		FLASH_ErasePage(FLASH_APP_ADDR+wAddrOffset);
// 		Flash_Write(FLASH_APP_ADDR+wAddrOffset, hwBuffer1, PAGE_SIZE/2);
// 		Flash_Read(FLASH_APP_ADDR+wAddrOffset, hwBuffer2, PAGE_SIZE/2);

// 		for(i=0; i<(PAGE_SIZE/2); i++)
// 		{
// 			if(hwBuffer1[i] != hwBuffer2[i])
// 			{
// 				g_bFlashStatus = ERROR;
// 				break;
// 			}
// 		}
// 		wPageCnt++;
// 	}while(wPageCnt < wPageNum);
// 	FLASH_Lock();
// }
