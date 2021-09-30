/**
  ******************************************************************************
  * File   : main.c 
  * Version: V1.2.8
  * Date   : 2020-11-27
  * Brief  : ota bootloader body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "iap_task.h"

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
	//delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//ota_run_app();
	iap_task_create();
	
//   xTaskCreate((TaskFunction_t )test_task,     	
//               (const char*    )"test_task",   	
//               (uint16_t       )TEST_STK_SIZE, 
//               (void*          )NULL,				
//               (UBaseType_t    )TEST_TASK_PRIO,	
//               (TaskHandle_t*  )&TestTask_Handler);   
           
  vTaskStartScheduler();
}
