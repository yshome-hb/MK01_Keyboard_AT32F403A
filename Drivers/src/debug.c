#include <at32f4xx.h>
#include <stdio.h>
#include "debug.h"

/**************** DEBUG defines ****************/
#define DEBUG_UART_DEV                  USART1
#define DEBUG_UART_RCC                  RCC_APB2PERIPH_USART1
#define DEBUG_UART_IRQn                 USART1_IRQn
#define DEBUG_IRQ_Handler               USART1_IRQHandler

/*Tx*/
#define DEBUG_UARTTX_GPIO_RCC           RCC_APB2PERIPH_GPIOA
#define DEBUG_UARTTX_GPIO               GPIOA
#define DEBUG_UARTTX_PIN                GPIO_Pins_9 

/*Rx*/
#define DEBUG_UARTRX_GPIO_RCC           RCC_APB2PERIPH_GPIOA
#define DEBUG_UARTRX_GPIO               GPIOA
#define DEBUG_UARTRX_PIN                GPIO_Pins_10

log_level_t g_log_level = LOG_LEVEL_INFO;

/* Suport printf function, useMicroLib is unnecessary */
#ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  
  FILE __stdout;
  
  void _sys_exit(int x)
  {
    x = x;
  }
#endif

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{  
  USART_SendData(DEBUG_UART_DEV, ch);
  while ( USART_GetFlagStatus(DEBUG_UART_DEV, USART_FLAG_TRAC) == RESET );    
  return ch;
}

/**
  * @brief  initialize DEBUG   
  * @param  bound: UART BaudRate
  * @retval None
  */
void debug_printf_init(unsigned int bound)
{
  GPIO_InitType GPIO_InitStructure;
  USART_InitType USART_InitStructure;

  RCC_APB2PeriphClockCmd(DEBUG_UARTTX_GPIO_RCC | DEBUG_UARTRX_GPIO_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(DEBUG_UART_RCC, ENABLE);

  /* Configure the UART1 TX pin */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = DEBUG_UARTTX_PIN; 
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(DEBUG_UARTTX_GPIO, &GPIO_InitStructure);

  /* Configure the UART1 RX pin */
  GPIO_InitStructure.GPIO_Pins = DEBUG_UARTRX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PD;
  GPIO_Init(DEBUG_UARTRX_GPIO, &GPIO_InitStructure);

  /*Configure UART param*/
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

  USART_Init(DEBUG_UART_DEV, &USART_InitStructure); 
  //USART_INTConfig(DEBUG_PRINT_UART, USART_INT_RDNE, ENABLE);
  USART_Cmd(DEBUG_UART_DEV, ENABLE);   
}


void debug_printf_begin(void)
{

#if 0	/* add log time header here */
    long current = time(NULL);
	struct tm* ptm = localtime(&current);

	struct timeval tv;
	gettimeofday(&tv, NULL);

	printf("%02d:%02d:%02d.%03d ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)tv.tv_usec / 1000);
#endif
}


void debug_printf_end(void)
{
	printf("\r\n");
}

