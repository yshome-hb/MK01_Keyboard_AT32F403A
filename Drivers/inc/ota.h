#ifndef __OTA_H__
#define __OTA_H__

#include "at32f4xx.h"

#define APP_VECT_TAB_OFFSET  		(FLASH_APP_ADDR)
#define APP_UPGRADE_FLAG_ADDR		(FLASH_APP_ADDR - PAGE_SIZE)
#define APP_UPGRADE_FLAG 			0x41544B38

typedef void (*otafun)(void);

void ota_init_app(void);
int ota_valid_app(void);
void ota_clear_app(void);
void ota_update_app(void);
void ota_run_app(void);

#endif
