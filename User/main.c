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
  	while(1)
    {
        //printf("hello world\n");
				usb_sendBuffer[2] = 0x04;
				CDC_Send_DATA(usb_sendBuffer, 8);
				vTaskDelay(1);
				usb_sendBuffer[2] = 0x00;
				CDC_Send_DATA(usb_sendBuffer, 8);
        vTaskDelay(2000);
    }
}


#include <at32f4xx.h>
/*define usb pin*/
#define USB_DP_PIN          GPIO_Pins_12
#define USB_DM_PIN          GPIO_Pins_11

#define USB_GPIO            GPIOA
#if defined (AT32F421xx)
#define USB_GPIO_RCC_CLK    RCC_AHBPERIPH_GPIOA
#else
#define USB_GPIO_RCC_CLK    RCC_APB2PERIPH_GPIOA
#endif


/**
  * @brief  USB GPIO initialize
  *         USB use DP->PA12, DM->PA11    
  * @param  None
  * @retval None
  */
void AT32_USB_GPIO_init()
{
  GPIO_InitType GPIO_InitStructure;
  /* Enable the USB Clock*/
  RCC_APB2PeriphClockCmd(USB_GPIO_RCC_CLK, ENABLE);

  /*Configure DP, DM pin as GPIO_Mode_OUT_PP*/
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins  = USB_DP_PIN | USB_DM_PIN;
#if !defined (AT32F421xx)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
#else
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
  GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
#endif
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(USB_GPIO, &GPIO_InitStructure);
  GPIO_ResetBits(USB_GPIO, USB_DP_PIN);
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

		AT32_USB_GPIO_init();
	
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



