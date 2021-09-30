#include "sys.h"

//THUMB指令不支持彙編內聯
//採用如下方法實現執行彙編指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//關閉所有中斷
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//開啟所有中斷
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//設置棧頂地址
//addr:棧頂地址
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
