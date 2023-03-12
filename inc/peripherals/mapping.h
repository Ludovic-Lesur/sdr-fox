/*
 * mapping.h
 *
 *  Created on: 10 mar. 2023
 *      Author: Ludo
 */

#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "gpio.h"
#include "gpio_reg.h"

// RGB LED.
static const GPIO_pin_t GPIO_LED_RED =		(GPIO_pin_t) {1, 6, 0};
static const GPIO_pin_t GPIO_LED_GREEN =	(GPIO_pin_t) {1, 7, 0};
static const GPIO_pin_t GPIO_LED_BLUE =		(GPIO_pin_t) {1, 4, 0};
// Clock output.
static const GPIO_pin_t GPIO_CLKOUT =		(GPIO_pin_t) {0, 16, 2};
// Programming.
static const GPIO_pin_t GPIO_SWCLK =		(GPIO_pin_t) {0, 11, 6};
static const GPIO_pin_t GPIO_SWDIO =		(GPIO_pin_t) {0, 12, 6};

#endif /* __MAPPING_H__ */
