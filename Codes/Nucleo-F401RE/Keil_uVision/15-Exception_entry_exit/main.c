#include "stm32f401xe.h"
#include "Board_LED.h"

void delay(void)
{
	uint32_t i;
	for (i=0; i<500000; i++);
}	

void WWDG_IRQHandler(void)
{
	/* Exception entry */
	LED_Off(0);
} /* Exception exit */
int main(void)
{
	/* Change the stack pointer to PSP */
	uint32_t value = __get_CONTROL();
	value |= (1 << 1);
	__set_CONTROL(value);
	
	/* Initialize PSP to SRAM2_BASE */
	__set_PSP(SRAM1_BASE);

	LED_Initialize();
	
	while(1)
	{
		LED_On(0);
		
		/* Enable and Pend the Watchdog interrupt here */
		NVIC_EnableIRQ(WWDG_IRQn);
		NVIC_SetPendingIRQ(WWDG_IRQn);
		
		delay();
		
		LED_On(0);
		
		delay();
	}
}
