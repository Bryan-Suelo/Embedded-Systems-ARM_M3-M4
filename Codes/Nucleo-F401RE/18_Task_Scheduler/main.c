/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <stdint.h>
#include <stdio.h>

void SVC_Handler_c(uint32_t *pBaseStackFrame);

int32_t add_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #36");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t sub_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #37");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t mul_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #38");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t div_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #39");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}

int main(void)
{
	int32_t res;

	res = add_numbers(40, -90);
	printf("Add result : %ld\r\n",res);

	res = sub_numbers(25, 150);
	printf("Substract result : %ld\r\n",res);

	res = mul_numbers(75, 8524);
	printf("Multiply result : %ld\r\n",res);

	res = div_numbers(6827, 541);
	printf("Divide result : %ld\r\n",res);

	for(;;);
}

__attribute__ ((naked)) void SVC_Handler(void)
{
	/* 1. Get the value of MSP */
	__asm("MRS R0,MSP");
	__asm("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseStackFrame)
{
	printf("In SVC handler ... \r\n");

	int32_t arg0, arg1, res;

	uint8_t *pReturn_addr = (uint8_t*)pBaseStackFrame[6];
	/* 2. Decrement the return address by 2 to point to
	 * opcode of the SVC instruction in the program memory */
	pReturn_addr-=2;

	/* 3. Extract the SVC number */
	uint8_t svc_number = *pReturn_addr;
	printf("SVC number : %d\r\n", svc_number);

	arg0 = pBaseStackFrame[0];
	printf("first value : %ld\r\n",arg0);
	arg1 = pBaseStackFrame[1];
	printf("second value : %ld\r\n",arg1);

	switch (svc_number)
	{
	case 36:
		res = arg0 + arg1;
		break;
	case 37:
		res = arg0 - arg1;
		break;
	case 38:
		res = arg0 * arg1;
		break;
	case 39:
		res = arg0 / arg1;
		break;
	default:
		printf("Invalid SVC code \r\n");
	}

	pBaseStackFrame[0] = res;
}
