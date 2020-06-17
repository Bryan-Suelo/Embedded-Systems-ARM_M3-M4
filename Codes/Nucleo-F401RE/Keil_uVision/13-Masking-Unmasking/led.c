#include "led.h"
#include "my_board.h"
#include "stm32f401xe.h"

void led_init()
{
	RCC_TypeDef *pRCC = RCC;
	
	pRCC->AHB1ENR |= (0x08);
	
	/* Configure LED */
	GPIOD->MODER |= (0x01 << (LED_2 * 2));
	GPIOD->OTYPER |= (0 << LED_2);
	GPIOD->PUPDR |= (0x00 << (LED_2 * 2));
	GPIOD->OSPEEDR |= (0x00 << (LED_2 * 2));
}	

void led_on(uint8_t led_no)
{
	GPIOD->BSRR = ( 1 << led_no );
}

void led_off(uint8_t led_no)
{
	GPIOD->BSRR = ( 1 << (led_no+16) );
}


void led_toggle(uint8_t led_no)
{
	if(GPIOD->ODR & (1 << led_no) )
	{
		led_off(led_no);
	}else
	{
		led_on(led_no);
	}
	
}
