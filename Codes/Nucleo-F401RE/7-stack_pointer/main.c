#include "stdint.h"
#include "Board_LED.h"
#include "cmsis_armcc.h"
#include "stm32f401xe.h"

extern void __main(void);

void delay(void)
{
	uint32_t i = 0;
	for(i = 0; i< 500000; i++);
}
void WWDG_IRQHandler(void)
{
	uint32_t i=0;
	for (i=0; i<50000; i++);
}
void generate_interrupt(void)
{
	NVIC_EnableIRQ(WWDG_IRQn);
	NVIC_SetPendingIRQ(WWDG_IRQn);
}

int main (void)
{
	/* Let's move the top of the stack to end of SRAM1 */
	/* Let's program 0x20017FFF + 1 into MSP */
	__set_MSP(0x20017FFF+1);
	
	/* Let's change the current SP to PSP*/
	uint32_t value = __get_CONTROL();
	value |= (1 << 1);
	__set_CONTROL(value);
	
	/* Let's initialize PSP first */
	__set_PSP(0x20017FFF+1);
	
	/* Simulate an interrupt */
	generate_interrupt();
	
	LED_Initialize();
	while (1)
	{		
			LED_On(0);
			delay();
			LED_Off(0);
			delay();
		
	}	
}
