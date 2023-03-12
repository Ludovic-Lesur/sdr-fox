/*
 * iocon_reg.h
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
 */

#ifndef __IOCON_REG_H__
#define __IOCON_REG_H__

#include "types.h"

/*** IOCON registers ***/

typedef struct {
	volatile uint32_t PIO[2][32]; 	// Digital I/O control for port 0 pins PIO0_0.
									// Digital I/O control for port 1 pins PIO1_31, array offset: 0x0, array step: index*0x80, index2*0x4.
} IOCON_registers_t;

/*** IOCON base address ***/

#define IOCON	((IOCON_registers_t*) ((uint32_t) 0x40001000))

#endif /* __IOCON_REG_H__ */
