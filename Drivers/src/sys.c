#include "sys.h"

//THUMB���O������J�s���p
//�ĥΦp�U��k��{����J�s���OWFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�����Ҧ����_
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//�}�ҩҦ����_
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//�]�m�̳��a�}
//addr:�̳��a�}
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
