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

/*
 * 1. Create 4 tasks
 * 2. Reserve stack areas for all the task and scheduler
 * 3. Scheduling policy section
 * 		- We will be using round robin pre-emptive scheduling
 * 		- No task priority
 * 		- Use systick to generate exception for every 1ms to run the scheduler code
 * 	4. Configure the systick timer to produce exception for every 1ms
 * 	5. Initialize scheduler stack pointer (MSP)
 * 	6. Init task stack memory
 * */

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "led.h"

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
void init_tasks_stack(void);
void enable_processor_faults(void);
__attribute__((naked)) void switch_sp_to_psp(void);
uint32_t get_psp_value(void);

void task_delay(uint32_t tick_count);

uint32_t g_tick_count = 0;
uint8_t current_task = 1;
typedef struct
{
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t current_state;
	void (*task_handler) (void);
}TCB_t;

TCB_t user_task[MAX_TASK];

/* Semi-hosting init function */
extern void initialise_monitor_handles(void);

int main(void)
{
	enable_processor_faults();

	/* Initialize semihosting library */
	initialise_monitor_handles();

	init_scheduler_stack(SCHED_STACK_START);

	//printf("Implementation of simple stack scheduler\r\n");

	init_tasks_stack();

	led_init_all();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

void idle_task(void)
{
	while(1);
}

void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		task_delay(1000);
		led_off(LED_GREEN);
		task_delay(1000);
	}
}

void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		task_delay(500);
		led_off(LED_ORANGE);
		task_delay(500);
	}
}

void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		task_delay(250);
		led_off(LED_BLUE);
		task_delay(250);
	}
}

void task4_handler(void)
{
	while(1)
	{
		led_on(LED_RED);
		task_delay(100);
		led_off(LED_RED);
		task_delay(100);
	}
}

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t *)0xE000E014; /* SysTick Reload Value Register */
	uint32_t *pSCSR = (uint32_t *)0xE000E010; /* SysTick Control and Status Register */

	uint32_t count_value = (SYSTICK_TIM_CLK / tick_hz)-1; /* RELOAD value of N-1  16000 - 1 */

	/* Clear SysTick Reload Value Register */
	*pSRVR &= ~(0x00FFFFFF);
	/* Load the value*/
	*pSRVR |= count_value;

	/* Settings in SCSR */
	*pSCSR |= (1 << 1); /* Enables SysTick exception request TICKINT */
	*pSCSR |= (1 << 2); /* Indicates the clock source CLKSOURCE */
	*pSCSR |= (1 << 0);	/* Enables SysTick counter*/
}

__attribute__((naked)) void init_scheduler_stack (uint32_t sched_top_of_stack)
{
	__asm volatile("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile("BX LR");
}

void init_tasks_stack(void)
{
	user_task[0].current_state = TASK_READY_STATE;
	user_task[1].current_state = TASK_READY_STATE;
	user_task[2].current_state = TASK_READY_STATE;
	user_task[3].current_state = TASK_READY_STATE;
	user_task[4].current_state = TASK_READY_STATE;

	user_task[0].psp_value = IDLE_STACK_START;
	user_task[1].psp_value = T1_STACK_START;
	user_task[2].psp_value = T2_STACK_START;
	user_task[3].psp_value = T3_STACK_START;
	user_task[4].psp_value = T4_STACK_START;

	user_task[0].task_handler = idle_task;
	user_task[1].task_handler = task1_handler;
	user_task[2].task_handler = task2_handler;
	user_task[3].task_handler = task3_handler;
	user_task[4].task_handler = task4_handler;

	uint32_t * pPSP;
	for(int i=0; i < MAX_TASK; i++)
	{
		pPSP = (uint32_t *)user_task[i].psp_value;

		pPSP--;
		*pPSP = DUMMY_XPSR; /* 0X00100000 */

		pPSP--; /* PC */
		*pPSP = (uint32_t)user_task[i].task_handler;

		pPSP--; /* LR */
		*pPSP = 0xFFFFFFFD;

		for(int j = 0; j < 13 ; j++)
		{
			pPSP--; /* LR */
			*pPSP = 0;
		}

		user_task[i].psp_value = (uint32_t)pPSP;
	}
}


void enable_processor_faults(void)
{
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	*pSHCSR |= (1 << 16);	/* mem manage */
	*pSHCSR |= (1 << 17);	/* bus fault */
	*pSHCSR |= (1 << 18);	/* usage fault */
}

uint32_t get_psp_value(void)
{
	return user_task[current_task].psp_value;
}

void save_psp_value(uint32_t current_psp_value)
{
	user_task[current_task].psp_value = current_psp_value;
}

void update_next_task()
{
	int state = TASK_BLOCKED_STATE;

	for(int i = 0; i < MAX_TASK; i++)
	{
		current_task++;
		current_task %= MAX_TASK;
		state = user_task[current_task].current_state;
		if((state == TASK_READY_STATE) && (current_task != 0))
		{
			break;
		}
	}
	if(state != TASK_READY_STATE)
	{
		current_task = 0;
	}
}

__attribute__((naked)) void switch_sp_to_psp(void)
{
	/* 1. Initialize the PSP with Task1 stack start */
	/* Get the value if psp of current task */
	__asm volatile("PUSH {LR}"); /* Preserve LR which connects back to main */
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP,R0"); /* Initialize psp */
	__asm volatile("POP {LR}"); /* Pops back LR value */

	/* 2. Change SP to PSP using CONTROL register */
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}

void schedule(void)
{
	/* PendSV exception */
	uint32_t *pICSR = (uint32_t*)0xE000ED04;
	*pICSR |= (1 << 28);
}

void task_delay(uint32_t tick_count)
{
	/* Disable interrupt */
	INTERRUPT_DISABLE();

	if(current_task)
	{
		user_task[current_task].block_count = g_tick_count + tick_count;
		user_task[current_task].current_state = TASK_BLOCKED_STATE;
		schedule();
	}

	/* Enable interrupt */
	INTERRUPT_ENABLE();
}

__attribute__((naked)) void PendSV_Handler(void)
{
	/* Save the context of current task */
	/* 1. Get current running task's PSP value */
	__asm volatile("MRS R0,PSP");
	/* 2. Using that PSP value store SF2 (R4 to R11) */
	__asm volatile("STMDB R0!,{R4-R11}");
	/* 3. Save the content of LR*/
	__asm volatile("PUSH {LR}");
	/* 4. Save the current value of PSP */
	__asm volatile("BL save_psp_value");

	/* Retrieves the context of new task */
	/* 1. Decide next task to run */
	__asm volatile("BL update_next_task");
	/* 2. Get its past PSP value */
	__asm volatile("BL get_psp_value");
	/* 3. Using PSP value retrieve SF2(R4 to R11) */
	__asm volatile("LDMIA R0!,{R4-R11}");
	/* 4. Update PSP and exit */
	__asm volatile("MSR PSP,R0");
	/* 5. Return value to LR */
	__asm volatile("POP {LR}");
	/* 6. Exit return */
	__asm volatile("BX LR");
}

void update_global_tick_count(void)
{
	g_tick_count++;
}

void unblock_task(void)
{
	for(int i = 1; i < MAX_TASK; i++)
	{
		if(user_task[i].current_state != TASK_READY_STATE)
		{
			if(user_task[i].block_count == g_tick_count)
			{
				user_task[i].current_state = TASK_READY_STATE;
			}
		}
	}
}

void SysTick_Handler(void)
{
	uint32_t *pICSR = (uint32_t*)0xE000ED04;

	update_global_tick_count();
	unblock_task();
	*pICSR |= (1 << 28);
}

/* Implement fault handlers */
void HardFault_Handler(void)
{
	//printf("Exception : Hardfault \r\n");
	while(1);
}
void MemManage_Handler(void)
{
	//printf("Exception : MemManage \r\n");
	while(1);
}
void BusFault_Handler(void)
{
	//printf("Exception : Bus Fault \r\n");
	while(1);
}