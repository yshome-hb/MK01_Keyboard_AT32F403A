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

  	while(1)
    {
        //printf("hello world\n");

        vTaskDelay(1000);
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



