#ifndef __TIMER_H__
#define __TIMER_H__

#include <at32f4xx.h>

typedef void (*Timer_Int_f)(void);

void timer5_init(uint16_t period, Timer_Int_f timer_int);
void timer5_setperiod(uint16_t period);
void timer5_deinit(void);


#endif

