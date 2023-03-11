/*
 * gpio.h
 *
 *  Created on: 9 mar 2023
 *      Author: Ludo
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "gpio_reg.h"
#include "types.h"

/*** GPIO structures ***/

typedef struct {
	uint8_t port;
	uint8_t pin;
	uint8_t function;
} GPIO_pin_t;

typedef enum {
	GPIO_MODE_ANALOG_INPUT,
	GPIO_MODE_DIGITAL_INPUT,
	GPIO_MODE_DIGITAL_INPUT_INVERTED,
	GPIO_MODE_DIGITAL_OUTPUT,
	GPIO_MODE_LAST
} GPIO_mode_t;

typedef enum {
	GPIO_TYPE_PUSH_PULL = 0,
	GPIO_TYPE_OPEN_DRAIN,
	GPIO_TYPE_LAST
} GPIO_output_type_t;

typedef enum {
	GPIO_SLEW_RATE_STANDARD = 0,
	GPIO_SLEW_RATE_FAST,
	GPIO_SLEW_RATE_SPEED_LAST
} GPIO_slew_rate_t;

typedef enum {
	GPIO_PULL_NONE = 0,
	GPIO_PULL_DOWN,
	GPIO_PULL_UP,
	GPIO_PULL_REPEATER,
	GPIO_PULL_LAST
} GPIO_pull_resistor_t;

/*** GPIO functions ***/

void GPIO_init(void);
void GPIO_configure(const GPIO_pin_t* gpio, GPIO_mode_t mode, GPIO_output_type_t output_type, GPIO_slew_rate_t slew_rate, GPIO_pull_resistor_t pull_resistor);
void GPIO_write(const GPIO_pin_t* gpio, uint8_t state);
uint8_t GPIO_read(const GPIO_pin_t* gpio);
void GPIO_toggle(const GPIO_pin_t* gpio);

#endif /* __GPIO_H__ */
