/**
  ******************************************************************************
  * File   : main.c 
  * Version: V1.2.8
  * Date   : 2020-11-27
  * Brief  : Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

#include "arch_init.h"

#include "usb_config.h"

#include "at32f4xx.h"

/* Private define ------------------------------------------------------------*/
#define TEST_TASK_PRIO		4
/* test task */
#define TEST_STK_SIZE 		256  

/* Extern variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
TaskHandle_t TestTask_Handler;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* test task */
void test_task(void *pvParameters)
{
uint8_t usb_sendBuffer[32] = {0};
uint16_t recvLen, sendLen;
	
#if 0
  GPIO_InitType GPIO_InitStructure;  	

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOA, GPIO_Pins_0, Bit_SET);  

    // CAPSLOCK 阳极
		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOA, GPIO_Pins_10, Bit_SET);

    // NUMLOCK 阳极
		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOA, GPIO_Pins_9, Bit_SET);

    // FN 阳极
		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOA, GPIO_Pins_8, Bit_SET);

    //白灯阴极
		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_WriteBit(GPIOB, GPIO_Pins_10, Bit_SET);

		// GPIO_InitStructure.GPIO_Pins = GPIO_Pins_14;
		// GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		// GPIO_Init(GPIOA, &GPIO_InitStructure);
		// GPIO_WriteBit(GPIOA, GPIO_Pins_14, Bit_SET);  
#endif

    while(1)
    {
        //printf("hello world\n");
//				usb_sendBuffer[2] = 0x04;
//				CDC_Send_DATA(usb_sendBuffer, 8);
//				vTaskDelay(1);
//				usb_sendBuffer[2] = 0x00;
//				CDC_Send_DATA(usb_sendBuffer, 8);
        vTaskDelay(2000);
    }
}


/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
  arch_mcu_init();
  //vRegisterControlCLICommands();
  //vUARTCommandConsoleStart(1024, 1);

		USB_GPIO_init();
	
    /*Enable USB Interrut*/
 	  USB_Interrupts_Config();  

    /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
    Set_USBClock(USBCLK_FROM_HSI);

    /* USB protocol and register initialize*/
 	 USB_Init();

  xTaskCreate((TaskFunction_t )test_task,     	
              (const char*    )"test_task",   	
              (uint16_t       )TEST_STK_SIZE, 
              (void*          )NULL,				
              (UBaseType_t    )TEST_TASK_PRIO,	
              (TaskHandle_t*  )&TestTask_Handler);   
           
  vTaskStartScheduler();
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */



