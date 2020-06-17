#include "stm32f401xe.h"


void USART1_IRQHandler(void)
{
	//while(1);
	uint16_t i;
	i++;
}

int main(void)
{
	/* Enable the USART interrupt or USART IRQ number */ 
	NVIC_EnableIRQ(USART1_IRQn);
	
	/* Disable IRQ */
	NVIC_DisableIRQ(USART1_IRQn);
	
	/* Enable the USART1 interrupt or USART3 IRQ number */
	NVIC_SetPendingIRQ(USART1_IRQn);
	
	while(1);
}
