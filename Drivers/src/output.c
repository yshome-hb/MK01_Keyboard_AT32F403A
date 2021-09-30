#include "at32f4xx.h"
#include "output.h"


typedef struct
{
	GPIO_Type* gpiox; 
	uint16_t pin;
	GPIOMode_Type type;
	uint8_t dvalue;
	
}IO_Output_t;


static IO_Output_t io_output[OUTPUT_MAX] = {0};

void io_output_init(void)
{	
	GPIO_InitType GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_AFIO, ENABLE);
	GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	io_output[OUTPUT_POWER_3V].gpiox = GPIOA;
	io_output[OUTPUT_POWER_3V].pin = GPIO_Pins_5;
	io_output[OUTPUT_POWER_3V].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_POWER_3V].dvalue = 0;	

	io_output[OUTPUT_LED_1].gpiox = GPIOA;
	io_output[OUTPUT_LED_1].pin = GPIO_Pins_6;
	io_output[OUTPUT_LED_1].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_1].dvalue = 1;		

	io_output[OUTPUT_LED_2].gpiox = GPIOA;
	io_output[OUTPUT_LED_2].pin = GPIO_Pins_7;
	io_output[OUTPUT_LED_2].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_2].dvalue = 1;	

	io_output[OUTPUT_LED_3].gpiox = GPIOB;
	io_output[OUTPUT_LED_3].pin = GPIO_Pins_0;
	io_output[OUTPUT_LED_3].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_3].dvalue = 1;	

	io_output[OUTPUT_LED_4].gpiox = GPIOB;
	io_output[OUTPUT_LED_4].pin = GPIO_Pins_1;
	io_output[OUTPUT_LED_4].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_4].dvalue = 1;

	io_output[OUTPUT_LED_BL].gpiox = GPIOB;
	io_output[OUTPUT_LED_BL].pin = GPIO_Pins_13;
	io_output[OUTPUT_LED_BL].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_BL].dvalue = 0;

	io_output[OUTPUT_LED_DBG].gpiox = GPIOA;
	io_output[OUTPUT_LED_DBG].pin = GPIO_Pins_15;
	io_output[OUTPUT_LED_DBG].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_LED_DBG].dvalue = 0;

	io_output[OUTPUT_ESP_RST].gpiox = GPIOB;
	io_output[OUTPUT_ESP_RST].pin = GPIO_Pins_15;
	io_output[OUTPUT_ESP_RST].type = GPIO_Mode_OUT_PP;	
	io_output[OUTPUT_ESP_RST].dvalue = 0;	

	for(uint16_t i = 0; i < OUTPUT_MAX; i++)
	{
		GPIO_InitStructure.GPIO_Pins = io_output[i].pin;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = io_output[i].type;
		GPIO_Init(io_output[i].gpiox, &GPIO_InitStructure);
		GPIO_WriteBit(io_output[i].gpiox, io_output[i].pin, (BitState)io_output[i].dvalue);
	}	
}


void io_output_set(uint8_t idx, uint8_t value)
{
 	if(idx >= OUTPUT_MAX)
		return;

	io_output[idx].dvalue = (value > 0);
	GPIO_WriteBit(io_output[idx].gpiox, io_output[idx].pin, (BitState)io_output[idx].dvalue);
}


void io_output_toggle(uint8_t idx)
{
 	if(idx >= OUTPUT_MAX)
		return;

	io_output[idx].dvalue = (io_output[idx].dvalue == 0);
	GPIO_WriteBit(io_output[idx].gpiox, io_output[idx].pin, (BitState)io_output[idx].dvalue);
}


uint8_t io_output_get(uint8_t idx)
{
 	if(idx >= OUTPUT_MAX)
		return 0;

	return io_output[idx].dvalue;
}
