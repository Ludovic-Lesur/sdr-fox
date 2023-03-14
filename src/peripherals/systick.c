/*
 * systick.c
 *
 *  Created on: Mar 14, 2023
 *      Author: ludo
 */

#include "systick.h"

#include "nvic.h"
#include "syscon.h"
#include "syscon_reg.h"
#include "systick_reg.h"
#include "types.h"

/*** SYSTICK local macros ***/

#define SYSTICK_DELAY_MS_MIN	1
#define SYSTICK_DELAY_MS_MAX	500000

/*** SYSTICK local global variables ***/

static volatile uint8_t systick_irq_flag = 0;

/*** SYSTICK local functions ***/

/* SYSTEM TICK INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void SysTick_Handler(void) {
	systick_irq_flag = 1;
}

/*** SYSTICK functions ***/

/* INIT SYSTEM TICK TIMER.
 * @param:	None.
 * @return:	None.
 */
void SYSTICK_init(void) {
	// Select 32kHz clock.
	SYSCON -> SYSTICKCLKSELX[0] = 0x02;
	SYSCON -> SYSTICKCLKDIV0 &= 0x80000000;
	// Disable timer and interrupt.
	// Select external clock source.
	SYSTICK -> CTRL &= ~(0b111 << 0);
	// Configure interrupt.
	NVIC_set_priority(NVIC_INTERRUPT_SYSTICK, NVIC_PRIORITY_MIN);
}

/* DELAY FUNCTION.
 * @param delay_ms:	Delay in milliseconds.
 * @return status:	Function execution status.
 */
SYSTICK_status_t SYSTICK_delay_milliseconds(uint32_t delay_ms) {
	// Local variables.
	SYSTICK_status_t status = SYSTICK_SUCCESS;
	uint64_t reload_value = 0;
	// Check delay.
	if (delay_ms > SYSTICK_DELAY_MS_MAX) {
		status = SYSTICK_ERROR_DELAY_OVERFLOW;
		goto errors;
	}
	if (delay_ms < SYSTICK_DELAY_MS_MIN) {
		status = SYSTICK_ERROR_DELAY_UNDERFLOW;
		goto errors;
	}
	// Compute reload value.
	reload_value = ((uint64_t) delay_ms) * ((uint32_t) SYSCON_32K_OSC_FREQUENCY_HZ);
	reload_value /= ((uint32_t) 1000);
	SYSTICK -> LOAD = reload_value;
	// Clear counter and enable interrupt.
	SYSTICK -> VAL = 0;
	SYSTICK -> CTRL |= (0b1 << 1);
	systick_irq_flag = 0;
	// Start timer.
	SYSTICK -> CTRL |= (0b1 << 0);
	// Wait completion.
	while (systick_irq_flag == 0) {
		__asm volatile ("wfi");
	}
	// Stop timer.
	SYSTICK -> CTRL &= ~(0b11 << 0);
errors:
	return status;
}
