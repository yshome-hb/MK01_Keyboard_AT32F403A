#include "sys.h"
#include "flash.h"
#include "ota.h"

otafun Jump2App;

static inline void Reset_Periph_Clock(void)
{
	RCC->APB1RST = 0xFFFFFFFF;
	RCC->APB2RST = 0xFFFFFFFF;
	RCC->APB1RST = 0x00000000;
	RCC->APB2RST = 0x00000000;
}


static void ota_load_app(uint32_t addr)
{
	Reset_Periph_Clock();
	if((flash_read_word(addr) & 0x2FFE0000) == SRAM_BASE){
		Jump2App = (otafun)flash_read_word(addr + 4);		
		MSR_MSP(flash_read_word(addr));
		Jump2App();
	}
}


void ota_init_app(void)
{	
	SCB->VTOR = FLASH_BASE | APP_VECT_TAB_OFFSET;
}


int ota_valid_app(void)
{
	return (flash_read_word(APP_UPGRADE_FLAG_ADDR) == APP_UPGRADE_FLAG);
}


void ota_clear_app(void)
{
	FLASH_Unlock();
	FLASH_ErasePage(APP_UPGRADE_FLAG_ADDR);
	FLASH_Lock();
}

void ota_update_app(void)
{
	FLASH_Unlock();
	FLASH_ErasePage(APP_UPGRADE_FLAG_ADDR);
	FLASH_ProgramWord(APP_UPGRADE_FLAG_ADDR, APP_UPGRADE_FLAG);
	FLASH_Lock();
}


void ota_run_app(void)
{
	if((flash_read_word(FLASH_APP_ADDR + 4) & 0xFF000000) == FLASH_BASE){
		ota_load_app(FLASH_APP_ADDR);
	}
}
