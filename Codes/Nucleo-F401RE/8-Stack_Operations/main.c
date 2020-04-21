/*
	Write a program to PUSH the contents of R0, R1,R2 Registers
	Using MSP as a stack pointer, and then POP the contents back using PSP as a stack
*/

#include <stdint.h>
#include "stm32f401xe.h"

extern void do_stack_operations(void);
int main(void)
{
	/* This function is implemened in Assembly */
	do_stack_operations();
	return 0;
}

