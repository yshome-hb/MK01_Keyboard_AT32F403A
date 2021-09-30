#include "stdlib.h"
#include "usart.h"	

#define PORT1_UART_DEV                  USART1
#define PORT1_UART_RCC                  RCC_APB2PERIPH_USART1
#define PORT1_UART_IRQn                 USART1_IRQn
#define PORT1_IRQ_Handler               USART1_IRQHandler

#define PORT1_DMA_RCC					RCC_AHBPERIPH_DMA1
#define PORT1_DMA_TXCH					DMA1_Channel4
#define PORT1_DMA_RXCH					DMA1_Channel5

/*Tx*/
#define PORT1_UARTTX_GPIO_RCC           RCC_APB2PERIPH_GPIOA
#define PORT1_UARTTX_GPIO               GPIOA
#define PORT1_UARTTX_PIN                GPIO_Pins_9 

/*Rx*/
#define PORT1_UARTRX_GPIO_RCC           RCC_APB2PERIPH_GPIOA
#define PORT1_UARTRX_GPIO               GPIOA
#define PORT1_UARTRX_PIN                GPIO_Pins_10


static Receive_Parse8_f uart1_parse = NULL;


void uart1_init(uint32_t buad, Receive_Parse8_f receive_parse)
{
	GPIO_InitType GPIO_InitStructure;
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(PORT1_UART_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(PORT1_UARTTX_GPIO_RCC | PORT1_UARTRX_GPIO_RCC, ENABLE);
 
  	/* Configure the UART1 TX pin */
  	GPIO_StructInit(&GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pins = PORT1_UARTTX_PIN; 
  	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(PORT1_UARTTX_GPIO, &GPIO_InitStructure);

  	/* Configure the UART1 RX pin */
  	GPIO_InitStructure.GPIO_Pins = PORT1_UARTRX_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PD;
  	GPIO_Init(PORT1_UARTRX_GPIO, &GPIO_InitStructure);
	
  	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = buad;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  	USART_Init(PORT1_UART_DEV, &USART_InitStructure);

	uart1_parse = receive_parse;		
	if(uart1_parse != NULL){

		NVIC_InitStructure.NVIC_IRQChannel = PORT1_UART_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		USART_INTConfig(PORT1_UART_DEV, USART_INT_RDNE, ENABLE);
	}

  	USART_Cmd(PORT1_UART_DEV, ENABLE);  	
}


// void uart1_setdma(uint8_t *tx_addr, uint16_t tx_size, uint8_t *rx_addr, uint16_t rx_size)
// {
// 	DMA_InitType DMA_InitStructure;

// 	if((!tx_addr) && (!tx_size) && (!rx_addr) && (!rx_size))
// 		return;

// 	RCC_AHBPeriphClockCmd(PORT1_DMA_RCC, ENABLE);	

// 	if(tx_addr != NULL && tx_size > 0 ){

// 		DMA_Reset(PORT1_DMA_TXCH);
// 		DMA_DefaultInitParaConfig(&DMA_InitStructure);
// 		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(PORT1_UART_DEV->DT);
// 		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)tx_addr;
// 		DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
// 		DMA_InitStructure.DMA_BufferSize = tx_size;
// 		DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
// 		DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
// 		DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
// 		DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
// 		DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
// 		DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
// 		DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
// 		DMA_Init(PORT1_DMA_TXCH, &DMA_InitStructure);
		
// 		USART_DMACmd( PORT1_UART_DEV, USART_DMAReq_Tx, ENABLE);
// 	}

// 	if(rx_addr != NULL && rx_size > 0 ){

// 		DMA_Reset(PORT1_DMA_RXCH);
// 		DMA_DefaultInitParaConfig(&DMA_InitStructure);
// 		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(PORT1_UART_DEV->DT);
// 		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx_addr;
// 		DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
// 		DMA_InitStructure.DMA_BufferSize = rx_size;
// 		DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
// 		DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
// 		DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
// 		DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
// 		DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
// 		DMA_InitStructure.DMA_Priority = DMA_PRIORITY_MEDIUM;
// 		DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
// 		DMA_Init(PORT1_DMA_RXCH, &DMA_InitStructure);
		
// 		USART_DMACmd( PORT1_UART_DEV, USART_DMAReq_Rx, ENABLE);
// 	}
// }


void uart1_deinit(void)
{
	USART_INTConfig(PORT1_UART_DEV, USART_INT_RDNE, DISABLE);
	USART_Cmd(PORT1_UART_DEV, DISABLE);
	USART_Reset(PORT1_UART_DEV);
}


void uart1_send(uint8_t c)
{
	while(USART_GetFlagStatus(PORT1_UART_DEV, USART_FLAG_TRAC) == RESET);
	USART_SendData(PORT1_UART_DEV, c);
}


void PORT1_IRQ_Handler( void )
{
	char cChar;	
	if( USART_GetITStatus( PORT1_UART_DEV, USART_INT_RDNE ) == SET )
	{
		cChar = USART_ReceiveData( PORT1_UART_DEV );
		if(uart1_parse != NULL)
			uart1_parse(cChar);
	}
}
