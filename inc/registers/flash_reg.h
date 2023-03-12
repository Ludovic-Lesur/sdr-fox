/*
 * flash_reg.h
 *
 *  Created on: 12 mar. 2023
 *      Author: Ludo
 */

#ifndef __FLASH_REG_H__
#define __FLASH_REG_H__

#include "types.h"

/*** FLASH registers ***/

typedef struct {
	volatile uint32_t CMD;				// Command register, offset: 0x0.
	volatile uint32_t EVENT;			// Event register, offset: 0x4.
	uint8_t RESERVED0[8];
	volatile uint32_t STARTA;			// Start (or only) address for next flash command, offset: 0x10.
	volatile uint32_t STOPA;			// End address for next flash command, if command operates on address ranges, offset: 0x14.
	uint8_t RESERVED1[104];
	volatile uint32_t DATAW[4];			// Data register, word 0-7; Memory data, or command parameter, or command result., array offset: 0x80, array step: 0x4.
	uint8_t RESERVED2[3912];
	volatile uint32_t INT_CLR_ENABLE;	// Clear interrupt enable bits, offset: 0xFD8.
	volatile uint32_t INT_SET_ENABLE;	// Set interrupt enable bits, offset: 0xFDC.
	volatile uint32_t INT_STATUS;		// Interrupt status bits, offset: 0xFE0.
	volatile uint32_t INT_ENABLE;		// Interrupt enable bits, offset: 0xFE4.
	volatile uint32_t INT_CLR_STATUS;	// Clear interrupt status bits, offset: 0xFE8.
	volatile uint32_t INT_SET_STATUS;	// Set interrupt status bits, offset: 0xFEC.
	uint8_t RESERVED3[12];
	volatile uint32_t MODULE_ID;		// Controller+Memory module identification, offset: 0xFFC.
} FLASH_registers_t;

/*** FLASH base address ***/

#define FLASH	((FLASH_registers_t*) ((uint32_t) 0x40034000))

#endif /* __FLASH_REG_H__ */
