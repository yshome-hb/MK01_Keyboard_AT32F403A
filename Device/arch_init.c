#include "delay.h"
#include "ota.h"
#include "debug.h"
#include "at32f4xx.h"

#include "arch_init.h"

#undef	TAG
#define TAG		"arch_init"

int arch_mcu_init(void)
{
	//ota_init_app();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
#if MIIO_LOG_LEVEL <= LOG_LEVEL_FATAL	
	//debug_printf_init(115200);
#endif
	//LOG_INFO_TAG(TAG, "<****** add mcu init func here ******>");

	return 0;
}
