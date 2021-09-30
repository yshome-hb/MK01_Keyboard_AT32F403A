/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/***************************************************************************************************************/
/*                                        FreeRTOS基础配置配置选项                                              */
/***************************************************************************************************************/

#define configUSE_PREEMPTION					1                       //1使用抢占式内核，0使用协程
#define configCPU_CLOCK_HZ			            ( SystemCoreClock )	
#define configTICK_RATE_HZ			            ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		            ( 32 )
#define configMINIMAL_STACK_SIZE	            ( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN		            ( 16 )
#define configUSE_16_BIT_TICKS					0                       //系统节拍计数器变量数据类型，
                                                                        //1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD					1                       //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务

#define configUSE_MUTEXES						1                       //为1时使用互斥信号量

#define configCHECK_FOR_STACK_OVERFLOW			0                       //大于0时启用堆栈溢出检测功能，如果使用此功能
                                                                        //用户必须提供一个栈溢出钩子函数，如果使用的话
                                                                        //此值可以为1或者2，因为有两种栈溢出检测方法。

/***************************************************************************************************************/
/*                                FreeRTOS与内存申请有关配置选项                                                */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //支持动态内存申请
#define configTOTAL_HEAP_SIZE					((size_t)(10*1024))     //系统所有总的堆大小

/***************************************************************************************************************/
/*                                FreeRTOS与钩子函数有关的配置选项                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0                       //1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK						0                       //1，使用时间片钩子；0，不使用

/***************************************************************************************************************/
/*                                FreeRTOS与运行时间和任务状态收集有关的配置选项                                 */
/***************************************************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        0                       //为1时启用运行时间统计功能
#define configUSE_TRACE_FACILITY				0                       //为1启用可视化跟踪调试
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       //与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
                                                                        //prvWriteNameToBuffer(),vTaskList(),
                                                                        //vTaskGetRunTimeStats()

/***************************************************************************************************************/
/*                                FreeRTOS与协程有关的配置选项                                                  */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 			        0                       //为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )                   //协程的有效优先级数目

/***************************************************************************************************************/
/*                                FreeRTOS与软件定时器有关的配置选项                                            */
/***************************************************************************************************************/
#define configUSE_TIMERS				        0                               //为1时启用软件定时器
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        //软件定时器优先级
#define configTIMER_QUEUE_LENGTH		        1                               //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	        (configMINIMAL_STACK_SIZE*2)    //软件定时器任务堆栈大小

/***************************************************************************************************************/
/*                                FreeRTOS可选函数配置选项                                                      */
/***************************************************************************************************************/
#define INCLUDE_vTaskPrioritySet		        0
#define INCLUDE_uxTaskPriorityGet		        0
#define INCLUDE_vTaskDelete				        0
#define INCLUDE_vTaskCleanUpResources	        0
#define INCLUDE_vTaskSuspend			        0
#define INCLUDE_vTaskDelayUntil			        0
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_xTaskGetCurrentTaskHandle       0
#define INCLUDE_xTimerPendFunctionCall	        0

/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE       256

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x0f

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	0x05

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
//#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

