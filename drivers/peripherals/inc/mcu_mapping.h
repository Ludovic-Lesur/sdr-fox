/*
 * mcu_mapping.h
 *
 *  Created on: 10 mar. 2023
 *      Author: Ludo
 */

#ifndef __MCU_MAPPING_H__
#define __MCU_MAPPING_H__

#include "gpio.h"
#include "types.h"

/*** MCU MAPPING global variables ***/

// RGB LED.
extern const GPIO_pin_t GPIO_LED_RED;
extern const GPIO_pin_t GPIO_LED_GREEN;
extern const GPIO_pin_t GPIO_LED_BLUE;
// Clock output.
extern const GPIO_pin_t GPIO_CLKOUT;

#endif /* __MAPPING_H__ */
