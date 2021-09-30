#include "stdlib.h"
#include "timer.h"

#define TIMER5_DEV                  TMR5
#define TIMER5_RCC                  RCC_APB1PERIPH_TMR5
#define TIMER5_IRQn                 TMR5_GLOBAL_IRQn
#define TIMER5_IRQ_Handler          TMR5_GLOBAL_IRQHandler
#define TIMER5_US100_FREQ           (10000) // 10kHz

static Timer_Int_f timer5_int = NULL;


void timer5_init(uint16_t period, Timer_Int_f timer_int)
{
	TMR_TimerBaseInitType  TMR_TMReBaseStructure;
  	NVIC_InitType NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER5_RCC, ENABLE);
	
  	TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
  	TMR_TMReBaseStructure.TMR_Period = period - 1;
  	TMR_TMReBaseStructure.TMR_DIV = (uint16_t)(SystemCoreClock/TIMER5_US100_FREQ)-1;
  	TMR_TMReBaseStructure.TMR_ClockDivision = TMR_CKD_DIV1;
  	TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
  	TMR_TimeBaseInit(TIMER5_DEV, &TMR_TMReBaseStructure);
	
	timer5_int = timer_int;
	
 	NVIC_InitStructure.NVIC_IRQChannel = TIMER5_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	TMR_ClearFlag(TIMER5_DEV, TMR_INT_Overflow);
	TMR_INTConfig(TIMER5_DEV, TMR_INT_Overflow, ENABLE);
	TMR_ARPreloadConfig(TIMER5_DEV, ENABLE);

	TMR_Cmd(TIMER5_DEV, ENABLE);
}


void timer5_setperiod(uint16_t period)
{
	TMR_SetCounter(TIMER5_DEV, 0);
	TMR_SetAutoreload(TIMER5_DEV, period);

}


void timer5_deinit(void)
{
	TMR_INTConfig(TIMER5_DEV, TMR_INT_Overflow, DISABLE);	
	TMR_ClearITPendingBit(TIMER5_DEV, TMR_INT_Overflow);
	TMR_Cmd(TIMER5_DEV, DISABLE);
}


void TIMER5_IRQ_Handler(void)
{
	if(TMR_GetINTStatus(TIMER5_DEV, TMR_INT_Overflow) == SET)
	{
		TMR_ClearITPendingBit(TIMER5_DEV, TMR_INT_Overflow);
		if(timer5_int != NULL)
			timer5_int();
	}
}
