/* Enable and Pend the USART1 interrupt*/

#include "stm32f401xe.h"

#define USART1_IRQ_NUM 37

void USART1_IRQHandler(void)
{
	//while(1);
	uint16_t i;
	i++;
}

int main(void)
{
	NVIC_Type *pNVIC = NVIC;
	
	/* Enable the USART1 IRQ number */
	pNVIC->ISER[1] = pNVIC->ISER[1] | (1 << (37-32) );
	
	/* Pend the interrupt */
	pNVIC->ISPR[1] = pNVIC->ISPR[1] | (1 << (37-32) );
	
	while(1);
	return 0;
}
