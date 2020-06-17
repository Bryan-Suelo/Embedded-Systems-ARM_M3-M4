#include "stm32f401xe.h"

void HardFault_Handler(void)
{
	
}

void MemManage_Handler(void)
{
	
}

void BusFault_Handler(void)
{
	
}

void UsageFault_Handler(void)
{
	
}

void SVC_Handler(void)
{
	
}

void PendSV_Handler(void)
{
	
}

int divide_numbers(int x,int y)
{
	return x/y;
}

int main(void)
{
	SCB_Type *pSCB = SCB;
	/* Enable Exceptions*/
	pSCB->SHCSR = pSCB->SHCSR |(1 << 16); 	/* MemManage */
	pSCB->SHCSR = pSCB->SHCSR |(1 << 17); 	/* BusFault */
	pSCB->SHCSR = pSCB->SHCSR |(1 << 18); 	/* UsageFault */
	
	/* Enable DIV_0_TRP bit in CCR */
	//pSCB->CCR = pSCB->CCR |(1 << 4); 	
	
	/* Divide by 0 */
	//divide_numbers(1,0);
	
	
	/* Enable UNALIGN_TRP bit in CCR*/
	//pSCB->CCR |=(1 << 3);
	
	/* Unaligned data access */
	uint32_t volatile *p = (uint32_t*) 0x20000000;
	uint32_t volatile var = *p;
	var++;
	
	while(1);
	//return 0;
}
