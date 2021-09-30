/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/*
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Library includes. */
#include <string.h>
#include <at32f4xx.h>

/* application includes. */
#include "cli_serial.h"
/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serINVALID_SEM					( ( SemaphoreHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serTX_BLOCK_TIME				( 40 / portTICK_PERIOD_MS )

/* UART defines */
#define SERIAL_UART                		USART1
#define SERIAL_UART_RCC            		RCC_APB2PERIPH_USART1
#define SERIAL_UART_IRQn                USART1_IRQn
#define SERIAL_IRQ_Handler              USART1_IRQHandler

#define SERIAL_DMA_RCC					RCC_AHBPERIPH_DMA1
#define SERIAL_DMA_TXCH					DMA1_Channel4

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static SemaphoreHandle_t xCharsForTx;
static portCHAR *uxTxBuffer;
static unsigned portBASE_TYPE uxTxLength;

/*-----------------------------------------------------------*/

/* UART interrupt handler. */
void vUARTInterruptHandler( void );

/*-----------------------------------------------------------*/

/*
 * See the serial2.h header file.
 */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength, portCHAR *uxOutputBuffer, unsigned portBASE_TYPE uxOutputLength )
{
	DMA_InitType DMA_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	/* Create the queues used to hold Rx/Tx characters. */
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xSemaphoreCreateBinary();
	uxTxBuffer = uxOutputBuffer;
	uxTxLength = uxOutputLength;

	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	if( ( xRxedChars == serINVALID_QUEUE ) 
	 || ( xCharsForTx == serINVALID_SEM )
	 || ( uxTxBuffer == NULL )
	 || ( uxTxLength == 0 ) )
	{
		return ( xComPortHandle ) 0;
	}

	RCC_AHBPeriphClockCmd( SERIAL_DMA_RCC, ENABLE );

	DMA_Reset(SERIAL_DMA_TXCH);
	DMA_DefaultInitParaConfig(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SERIAL_UART->DT);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uxTxBuffer;
	DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
	DMA_InitStructure.DMA_BufferSize = uxTxLength;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
	DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
	DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
	DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
	DMA_InitStructure.DMA_Priority = DMA_PRIORITY_MEDIUM;
	DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
	DMA_Init(SERIAL_DMA_TXCH, &DMA_InitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = SERIAL_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_LOWEST_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
		
	USART_DMACmd( SERIAL_UART, USART_DMAReq_Tx, ENABLE);
	USART_INTConfig( SERIAL_UART, USART_INT_TRAC, ENABLE );
	USART_INTConfig( SERIAL_UART, USART_INT_RDNE, ENABLE );

	xSemaphoreGive(xCharsForTx);

	/* This demo file only supports a single port but we have to return
	something to comply with the standard demo header file. */
	return ( xComPortHandle )0;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

void vSerialPrepareString( xComPortHandle pxPort, const signed char * const pcString)
{
	xSemaphoreTake(xCharsForTx, serTX_BLOCK_TIME);

	if(pcString != NULL)
		strncpy( uxTxBuffer, ( char * ) pcString, uxTxLength );
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort)
{
	DMA_ChannelEnable(SERIAL_DMA_TXCH, DISABLE);
	DMA_SetCurrDataCounter(SERIAL_DMA_TXCH, strlen(uxTxBuffer));
	DMA_ChannelEnable(SERIAL_DMA_TXCH, ENABLE);
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
	if(xSemaphoreTake(xCharsForTx, xBlockTime) != pdTRUE)
		return pdFAIL;

	memcpy(uxTxBuffer, &cOutChar, 1);
	DMA_SetCurrDataCounter(SERIAL_DMA_TXCH, 1);
	DMA_ChannelEnable(SERIAL_DMA_TXCH, ENABLE);

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
	/* Not supported as not required by the demo application. */
	vSemaphoreDelete(xCharsForTx);
	vQueueDelete(xRxedChars);

	USART_DMACmd( SERIAL_UART, USART_DMAReq_Tx, DISABLE);
	USART_INTConfig( SERIAL_UART, USART_INT_TRAC, DISABLE );
	USART_INTConfig( SERIAL_UART, USART_INT_RDNE, DISABLE );
	USART_Cmd( SERIAL_UART, DISABLE );
}
/*-----------------------------------------------------------*/

void SERIAL_IRQ_Handler( void )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
char cChar;

	if( USART_GetITStatus( SERIAL_UART, USART_INT_TRAC ) == SET )
	{		
		USART_ClearITPendingBit( SERIAL_UART, USART_INT_TRAC);
		DMA_ChannelEnable(SERIAL_DMA_TXCH, DISABLE);
		
		xSemaphoreGiveFromISR(xCharsForTx, &xHigherPriorityTaskWoken);
	}
	
	if( USART_GetITStatus( SERIAL_UART, USART_INT_RDNE ) == SET )
	{
		cChar = USART_ReceiveData( USART1 );
		xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
	}	
	
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}





	
