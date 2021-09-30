#ifndef __SYS_H__
#define __SYS_H__

#include "at32f4xx.h"

void WFI_SET(void);		//執行WFI指令
void INTX_DISABLE(void);//關閉所有中斷
void INTX_ENABLE(void);	//開啟所有中斷
void MSR_MSP(uint32_t addr);	//設置堆棧地址

#endif
