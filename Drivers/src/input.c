#include "at32f4xx.h"
#include "input.h"


#define INPUT_TIMER_DEV                 TMR5
#define INPUT_TIMER_RCC                 RCC_APB1PERIPH_TMR5
#define INPUT_TIMER_IRQn                TMR5_GLOBAL_IRQn
#define INPUT_TIMER_IRQ_Handler         TMR5_GLOBAL_IRQHandler

#define INPUT_US100_FREQ            	(10000) 

typedef struct
{
	GPIO_Type* gpiox; 
	uint16_t pin;
	GPIOMode_Type type;
	uint8_t filter;
	uint8_t value;
	uint8_t prevalue;
	
}IO_Filter_t;


static IO_Filter_t input_filter[INPUT_MAX] = {0};


static void input_scan(void)
{
	for(uint16_t i = 0; i < INPUT_MAX; i++)
	{
		input_filter[i].filter = ((input_filter[i].filter << 1) | GPIO_ReadInputDataBit(input_filter[i].gpiox, input_filter[i].pin));
	}
}


void io_input_init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TMR_TimerBaseInitType TMR_TMReBaseStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(INPUT_TIMER_RCC, ENABLE);

	input_filter[INPUT_KEY_1].gpiox = GPIOA;
	input_filter[INPUT_KEY_1].pin = GPIO_Pins_0;
	input_filter[INPUT_KEY_1].type = GPIO_Mode_IN_PU;	
	input_filter[INPUT_KEY_1].filter = 0xFF;
	input_filter[INPUT_KEY_1].value = 1;	
	input_filter[INPUT_KEY_1].prevalue = 0x00;	

	input_filter[INPUT_KEY_2].gpiox = GPIOA;
	input_filter[INPUT_KEY_2].pin = GPIO_Pins_1;
	input_filter[INPUT_KEY_2].type = GPIO_Mode_IN_PU;
	input_filter[INPUT_KEY_2].filter = 0xFF;
	input_filter[INPUT_KEY_2].value = 1;	
	input_filter[INPUT_KEY_2].prevalue = 0x00;		
	
	input_filter[INPUT_CRASH].gpiox = GPIOB;
	input_filter[INPUT_CRASH].pin = GPIO_Pins_14;
	input_filter[INPUT_CRASH].type = GPIO_Mode_IN_PU;
	input_filter[INPUT_CRASH].filter = 0xFF;
	input_filter[INPUT_CRASH].value = 1;	
	input_filter[INPUT_CRASH].prevalue = 0x00;	
	
	for(uint16_t i = 0; i < INPUT_MAX; i++)
	{
		GPIO_InitStructure.GPIO_Pins = input_filter[i].pin;
		GPIO_InitStructure.GPIO_Mode = input_filter[i].type;
		GPIO_Init(input_filter[i].gpiox, &GPIO_InitStructure);	
	}

  	TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
  	TMR_TMReBaseStructure.TMR_Period = 30;
	TMR_TMReBaseStructure.TMR_DIV = (uint16_t)(SystemCoreClock/(4 *INPUT_US100_FREQ))-1;
	TMR_TMReBaseStructure.TMR_ClockDivision = TMR_CKD_DIV4;
	TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up; 
  	TMR_TimeBaseInit(INPUT_TIMER_DEV, &TMR_TMReBaseStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = INPUT_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    TMR_ClearITPendingBit(INPUT_TIMER_DEV, TMR_INT_Overflow);     
	TMR_INTConfig(INPUT_TIMER_DEV, TMR_INT_Overflow, ENABLE);
	TMR_Cmd(INPUT_TIMER_DEV, ENABLE); 	
}


uint8_t io_input_update(uint8_t idx)
{
	input_filter[idx].prevalue = (input_filter[idx].prevalue << 1) & 0x03;
	
	if((input_filter[idx].filter & 0x0F) == 0x0F)
	{
		input_filter[idx].value = 1;
	}	
	else if((input_filter[idx].filter & 0x0F) == 0x00)
	{
		input_filter[idx].value = 0;
	}
	
	input_filter[idx].prevalue |= input_filter[idx].value;
	
	return input_filter[idx].prevalue;
}


void INPUT_TIMER_IRQ_Handler(void)
{
	if(TMR_GetINTStatus(INPUT_TIMER_DEV, TMR_INT_Overflow) != RESET)
	{
		TMR_ClearITPendingBit(INPUT_TIMER_DEV, TMR_INT_Overflow);
		input_scan();
	}
}
