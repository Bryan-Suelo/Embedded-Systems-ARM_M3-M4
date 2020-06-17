#include "stm32f4xx.h"

// Implement the watchdog interrupt handler
void WWDG_IRQHandler(void)
{
	unsigned long i;
	for (i=0; i<50; i++);
}

void generate_interrupt(void)
{
	// Simulate watchdog interrupt
	NVIC_EnableIRQ(WWDG_IRQn);
	NVIC_SetPendingIRQ(WWDG_IRQn);
}

int main(void)
{
	/* Lets program the PRIMASK register 0th bit as 1 */
	__set_PRIMASK(1);
	generate_interrupt();
	while(1);
	return 0;
}
