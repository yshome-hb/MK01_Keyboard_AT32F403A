#include "at32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"

static unsigned int fac_us = 0;

void delay_init(void)
{
	fac_us = SystemCoreClock / 1000000;
}

// Return system uptime in 1milliseconds (rollover in 49 days)
unsigned int millis(void)
{
	return (xTaskGetTickCount() / portTICK_PERIOD_MS);
}


void delay_ms(unsigned int nms)
{
	vTaskDelay(nms / portTICK_PERIOD_MS);			
}


void delay_us(unsigned int nus)
{
	unsigned int ticks = nus * fac_us;
	unsigned int reload = SysTick->LOAD;
	unsigned int told, tnow, tcnt = 0;

	told = SysTick->VAL;
	while(tcnt < ticks)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told) tcnt += (told-tnow);
			else tcnt += (reload-tnow+told);
			told = tnow;
		}
	}	
}


//------------------End of File----------------------------
