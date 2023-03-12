/*
 * gpio_reg.h
 *
 *  Created on: Mar 8, 2023
 *      Author: ludo
*/

#ifndef __GPIO_REG_H__
#define __GPIO_REG_H__

#include "types.h"

/*** GPIO registers ***/

typedef struct {
	volatile uint8_t B[2][32];		// Byte pin registers for all port GPIO pins, array offset: 0x0, array step: index*0x20, index2*0x1.
	uint8_t RESERVED_0[4032];
	volatile uint32_t W[2][32];		// Word pin registers for all port GPIO pins, array offset: 0x1000, array step: index*0x80, index2*0x4.
	uint8_t RESERVED_1[3840];
	volatile uint32_t DIR[2];		// Direction registers for all port GPIO pins, array offset: 0x2000, array step: 0x4.
	uint8_t RESERVED_2[120];
	volatile uint32_t MASK[2];		// Mask register for all port GPIO pins, array offset: 0x2080, array step: 0x4.
	uint8_t RESERVED_3[120];
	volatile uint32_t PIN[2];		// Port pin register for all port GPIO pins, array offset: 0x2100, array step: 0x4.
	uint8_t RESERVED_4[120];
	volatile uint32_t MPIN[2];		// Masked port register for all port GPIO pins, array offset: 0x2180, array step: 0x4.
	uint8_t RESERVED_5[120];
	volatile uint32_t SET[2];		// Write: Set register for port. Read: output bits for port, array offset: 0x2200, array step: 0x4.
	uint8_t RESERVED_6[120];
	volatile uint32_t CLR[2];		// Clear port for all port GPIO pins, array offset: 0x2280, array step: 0x4.
	uint8_t RESERVED_7[120];
	volatile  uint32_t NOT[2];		// Toggle port for all port GPIO pins, array offset: 0x2300, array step: 0x4.
	uint8_t RESERVED_8[120];
	volatile  uint32_t DIRSET[2];	// Set pin direction bits for port, array offset: 0x2380, array step: 0x4.
	uint8_t RESERVED_9[120];
	volatile  uint32_t DIRCLR[2];	// Clear pin direction bits for port, array offset: 0x2400, array step: 0x4.
	uint8_t RESERVED_10[120];
	volatile  uint32_t DIRNOT[2];	// Toggle pin direction bits for port, array offset: 0x2480, array step: 0x4.
} GPIO_registers_t;

/*** GPIO base address ***/

#define GPIO	((GPIO_registers_t*) ((uint32_t) 0x4008C000))

#endif /* __GPIO_REG_H__ */
