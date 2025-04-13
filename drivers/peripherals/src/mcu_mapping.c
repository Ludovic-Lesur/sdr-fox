/*
 * mcu_mapping.c
 *
 *  Created on: 05 apr. 2025
 *      Author: Ludo
 */

#include "mcu_mapping.h"

#include "gpio.h"
#include "gpio_registers.h"

/*** MCU MAPPING global variables ***/

// RGB LED.
const GPIO_pin_t GPIO_LED_RED = { 1, 6, 0 };
const GPIO_pin_t GPIO_LED_GREEN = { 1, 7, 0 };
const GPIO_pin_t GPIO_LED_BLUE = { 1, 4, 0 };
// Clock output.
const GPIO_pin_t GPIO_CLKOUT = { 0, 16, 2 };
