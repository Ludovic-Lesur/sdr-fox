/*
 * main.c
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
 */

#include "error.h"
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
	// Local variables.
	SYSCON_status_t syscon_status = SYSCON_SUCCESS;
	SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
	// Init vector.
	NVIC_init();
	// Init peripherals.
	GPIO_init();
	SYSCON_configure_clkout(SYSCON_CLKOUT_SOURCE_MAIN, 100);
	// Init clock.
	syscon_status = SYSCON_init_clock();
	SYSCON_error_check();
	SYSTICK_init();
	// Init USB interface.
	USB_HS_PHY_init();
	// Configure LED pin.
	GPIO_configure(&GPIO_LED_RED, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_GREEN, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LED_BLUE, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
	// Main loop.
	while (1) {
		GPIO_toggle(&GPIO_LED_RED);
		systick_status = SYSTICK_delay_milliseconds(1000);
		SYSTICK_error_check();
		GPIO_toggle(&GPIO_LED_GREEN);
		systick_status = SYSTICK_delay_milliseconds(1000);
		SYSTICK_error_check();
		GPIO_toggle(&GPIO_LED_BLUE);
		systick_status = SYSTICK_delay_milliseconds(1000);
		SYSTICK_error_check();
	}
	return 0;
}


