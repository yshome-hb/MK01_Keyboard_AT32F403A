#ifndef __USART_H__
#define __USART_H__

#include <at32f4xx.h>

typedef uint8_t (*Receive_Parse8_f)(uint8_t _input);

void uart1_init(uint32_t buad, Receive_Parse8_f receive_parse);
void uart1_deinit(void);
void uart1_send(uint8_t c);

#endif


