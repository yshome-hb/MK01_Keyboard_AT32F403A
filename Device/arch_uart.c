#include "miio_define.h"
#include "at32f4xx.h"
#include "arch_dbg.h"
#include "miio_uart.h"
#include "arch_uart.h"

#undef	MIIO_LOG_TAG
#define MIIO_LOG_TAG			"arch_uart"

#define MIIO_UART_DEV            ( USART2 )
#define MIIO_UART_RCC            ( RCC_APB1PERIPH_USART2 )
#define MIIO_UART_IRQ_CH         ( USART2_IRQn )
#define MIIO_UART_IRQ_HANDLER    ( USART2_IRQHandler )
#define MIIO_IRQ_PRIORITY        ( 8 )
#define MIIO_UART_BAUDRATE       ( 115200 )

uart_error_t _uart_init(miio_uart_t *uart)
{
	/* the following is an example for linux platform */
	/* user should change below for each mcu platform */
	/* adjust start */
	do {
		GPIO_InitType GPIO_InitStructure;
		USART_InitType USART_InitStructure;
		NVIC_InitType NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(MIIO_UART_RCC,ENABLE);

		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2;
		GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pins = GPIO_Pins_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = MIIO_UART_IRQ_CH;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MIIO_IRQ_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		USART_InitStructure.USART_BaudRate = MIIO_UART_BAUDRATE;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; /* 8 data bits */
		USART_InitStructure.USART_StopBits = USART_StopBits_1; /* 1 stop bit */
		USART_InitStructure.USART_Parity = USART_Parity_No; /* no parity */
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* no hardware flow control */
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; /* enable input & output */

		USART_Init(MIIO_UART_DEV, &USART_InitStructure);
		USART_INTConfig(MIIO_UART_DEV, USART_INT_RDNE, ENABLE);
		USART_Cmd(MIIO_UART_DEV, ENABLE);
	}while(false);
	/* adjust end */

	uart->params.baud_rate = 115200;
	uart->params.data_bits = 8;
	uart->params.parity = 0;
	uart->params.stop_bits = 1;

	return UART_OK;
}

int _uart_send_str(miio_uart_t *uart, const char* str)
{
	int len = strlen(str);
	int n_send = 0;
	int t = 0;
	if (len <= 0) { return UART_OK; }

	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	
	for(t = 0; t < len; t++) {
		while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);      
		USART_SendData(MIIO_UART_DEV, str[t]);
		n_send++;
	}
	while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);   
	/* adjust end*/
	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < len) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string failed");
		return UART_SEND_ERROR;
	}

#if PRINT_SEND_BUFF
	LOG_INFO_TAG(MIIO_LOG_TAG, "send string : %s", str);
#endif

	return n_send;
}

int _uart_send_byte(miio_uart_t *uart, const char c)
{
	int n_send = 0;
	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */

	/* adjust start */
	         
	while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);      
	USART_SendData(MIIO_UART_DEV, c);
	n_send++;
	while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);   
	/* adjust end */

	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < 1) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send byte failed : %x[hex]", c);
		return UART_SEND_ERROR;
	}

	LOG_INFO_TAG(MIIO_LOG_TAG, "send byte : %x[hex]", c);
	return n_send;
}

int _uart_send_str_wait_ack(miio_uart_t *uart, const char* str)
{
	int len = strlen(str);
    int n_send = 0;
	int t = 0;
	uint8_t ack_buf[ACK_BUF_SIZE] = { 0 };
	if (len <= 0) { return UART_OK; }


	
	memset(ack_buf, 0, ACK_BUF_SIZE);
	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */

	/* adjust start */
	
	for(t = 0; t < len; t++) {
		while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);     
		USART_SendData(MIIO_UART_DEV, str[t]);
		n_send++;
	}
	while(USART_GetFlagStatus(MIIO_UART_DEV, USART_FLAG_TRAC) == RESET);
	/* adjust end */

	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < len) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string wait ack failed 1");
		return UART_SEND_ERROR;
	}
		
#if PRINT_SEND_BUFF
	LOG_INFO_TAG(MIIO_LOG_TAG, "send string : %s", str);
#endif

	uart->recv_str(uart, ack_buf, ACK_BUF_SIZE, USER_UART_TIMEOUT_MS);
	if (0 != strncmp((const char*)ack_buf, "ok", strlen("ok"))) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string wait ack failed 2 str=%s\n", ack_buf);
		return UART_RECV_ACK_ERROR;
	}
	
	return n_send;
}

void MIIO_UART_IRQ_HANDLER(void)
{
	if(USART_GetITStatus(MIIO_UART_DEV, USART_INT_RDNE) != RESET)
	{
		USART_ClearITPendingBit(MIIO_UART_DEV, USART_INT_RDNE);
		_write_ringbuff(USART_ReceiveData(MIIO_UART_DEV));
	}
}


int _uart_recv_str(miio_uart_t *uart, uint8_t* buf, int buf_len, uint32_t timeout_ms)
{
	int n_read = 0, retry = 0;
	arch_os_mutex_get(&(uart->read_mutex));

	
	memset(buf,0,buf_len);
	while(retry < USER_RECEIVE_RETRY)
	{
		if (n_read >= buf_len) {
			LOG_INFO_TAG(MIIO_LOG_TAG,"out of buffer %d retry=%d\n", buf_len, retry);
			memset(buf,0,buf_len);
			n_read = 0;
			//goto error;
		}
		arch_msleep(timeout_ms);
		while(_read_ringbuff(buf+n_read))
		{
			n_read++;
			//LOG_INFO_TAG(MIIO_LOG_TAG,"+++n_read=%d\n",n_read);
			if (n_read >= buf_len) {
				LOG_INFO_TAG(MIIO_LOG_TAG,"out of buffer %d\n", buf_len);
				memset(buf,0,buf_len);
				n_read = 0;
				//goto error;
			}
			if (*(buf+n_read-1) == '\r')
			{
				buf[n_read] = '\0';
//TODO:				LOG_INFO_TAG(MIIO_LOG_TAG,"recv: %s \n", buf);
				arch_os_mutex_put(&(uart->read_mutex));
				return n_read;
			}
			
		}
		retry++;
		//LOG_INFO_TAG(MIIO_LOG_TAG,"retry receive %d n_read=%d _get_ringbuff_len=%d\n", retry, n_read, _get_ringbuff_len());
	}
error:
	n_read = 0;
	buf[n_read] = '\0';
	LOG_INFO_TAG(MIIO_LOG_TAG,"not got recv \n");
	arch_os_mutex_put(&(uart->read_mutex));
	return n_read;
}



int _Xmodem_recv_str(miio_uart_t *uart, uint8_t* buf, uint32_t timeout_ms)
{
        /* the following is an example for linux platform */
        /* user should adjust below for each mcu platform */
        /* adjust start */
				int n_read = 0;
        /* adjust end */
        arch_os_mutex_get(&(uart->read_mutex));
        /* adjust start */
        
	
	
				memset(buf,0, sizeof(buf));
				arch_msleep(timeout_ms);
				while(_read_ringbuff(buf+n_read))
				{
					n_read++;
				}
        buf[n_read] = '\0';
        /* adjust end */
        arch_os_mutex_put(&(uart->read_mutex));

        return n_read;
}

int _uart_recv_byte(miio_uart_t *uart, uint8_t* c, uint32_t timeout_ms)
{
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	int n_read = 0;
	arch_os_mutex_get(&(uart->read_mutex));
	/* adjust start */
	
	/* adjust end */
	arch_os_mutex_put(&(uart->read_mutex));

	return n_read;
}

void _uart_deinit(miio_uart_t *uart)
{
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	
	/* adjust end */
	return;
}

void _uart_interrupt(uint8_t enable) {
	if (enable == DISABLE)
		USART_INTConfig(MIIO_UART_DEV, USART_INT_RDNE, DISABLE);
	else
		USART_INTConfig(MIIO_UART_DEV, USART_INT_RDNE, ENABLE);
}
