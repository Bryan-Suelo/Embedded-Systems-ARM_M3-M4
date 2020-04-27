#include "stm32f401xe.h"

void WWDG_IRQHandler(void)
{
	NVIC_SetPendingIRQ(USART1_IRQn);
	while(1);
}
void USART1_IRQHandler(void)
{
	NVIC_SetPendingIRQ(TIM3_IRQn);
	while(1);
}
void TIM3_IRQHandler(void)
{ 
	while(1);
}

int main(void)
{
	/* Enable watchdog interrupt */
	NVIC_EnableIRQ(WWDG_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	
	/* Set priority */
	NVIC_SetPriority(WWDG_IRQn,5);
	NVIC_SetPriority(USART1_IRQn,4);
	NVIC_SetPriority(TIM3_IRQn,3);
	
	/* Pend Watchdog IRQ and Usart1 IRQ */
	NVIC_SetPendingIRQ(WWDG_IRQn);
	//NVIC_SetPendingIRQ(USART1_IRQn);
	//NVIC_SetPendingIRQ(TIM3_IRQn);
	
	while(1);
	//return 0;
}
