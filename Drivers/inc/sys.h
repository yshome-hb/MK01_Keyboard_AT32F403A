#ifndef __SYS_H__
#define __SYS_H__

#include "at32f4xx.h"

void WFI_SET(void);		//����WFI���O
void INTX_DISABLE(void);//�����Ҧ����_
void INTX_ENABLE(void);	//�}�ҩҦ����_
void MSR_MSP(uint32_t addr);	//�]�m��̦a�}

#endif
