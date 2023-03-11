/*
 * main.c
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
 */

#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "types.h"

/* MAIN FUNCTION.
 * @param:	None.
 * @return:	None.
 */
int main(void) {
	// Local variavles.
	uint32_t i = 0;
	// Init vector.
	NVIC_init();
	// Init peripherals.
	GPIO_init();
	// Configure LED pin.
	GPIO_configure(&GPIO_LED_RED, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_GREEN, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_BLUE, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	// Main loop.
	while (1) {
		GPIO_toggle(&GPIO_LED_RED);
		for (i=0 ; i<1000000 ; i++);
		GPIO_toggle(&GPIO_LED_GREEN);
		for (i=0 ; i<1000000 ; i++);
		GPIO_toggle(&GPIO_LED_BLUE);
		for (i=0 ; i<1000000 ; i++);
	}
	return 0;
}


