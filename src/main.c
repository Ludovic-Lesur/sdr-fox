/*
 * main.c
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
 */

#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "syscon.h"
#include "systick.h"
#include "types.h"

/* MAIN FUNCTION.
 * @param:	None.
 * @return:	None.
 */
int main(void) {
	// Init vector.
	NVIC_init();
	// Init peripherals.
	GPIO_init();
	// Init clock.
	SYSCON_init_clock();
	SYSTICK_init();
	// Configure LED pin.
	GPIO_configure(&GPIO_LED_RED, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_GREEN, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_BLUE, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	// Main loop.
	while (1) {
		GPIO_toggle(&GPIO_LED_RED);
		SYSTICK_delay_milliseconds(1000);
		GPIO_toggle(&GPIO_LED_GREEN);
		SYSTICK_delay_milliseconds(1000);
		GPIO_toggle(&GPIO_LED_BLUE);
		SYSTICK_delay_milliseconds(1000);
	}
	return 0;
}


