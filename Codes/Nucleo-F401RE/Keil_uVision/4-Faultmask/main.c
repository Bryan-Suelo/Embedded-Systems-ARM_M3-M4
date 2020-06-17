#include "stm32f4xx.h"

void WWDG_IRQHandler(void)
{
	unsigned long i;
	for(i=0; i<50; i++);
}

void generate_NMI_interrupt(void)
{
	/* Lets simulate the NMI interrupt */
	SCB_Type *pSCB;
	pSCB = SCB;
	pSCB->ICSR |= SCB_ICSR_NMIPENDSET_Msk;
}

int main(void)
{
	/* Lets program the FAULTMASK register's bit 0 as 1*/
	__set_FAULTMASK(1);
	/*
	void (*jump_addr) (void) = (void*)0x00000002;
	jump_addr();
	*/
	generate_NMI_interrupt();
	while(1);
	return 0;
}