/*
 * nvic_reg.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __NVIC_REG_H__
#define __NVIC_REG_H__

#include "types.h"

/*** NVIC registers ***/

typedef struct {
	volatile uint32_t ISER[16];		// Interrupt Set Enable Register.
	uint32_t RESERVED0[16];
	volatile uint32_t ICER[16];		// Interrupt Clear Enable Register.
	uint32_t RESERVED1[16];
	volatile uint32_t ISPR[16];		// Interrupt Set Pending Register.
	uint32_t RESERVED2[16];
	volatile uint32_t ICPR[16];		// Interrupt Clear Pending Register.
	uint32_t RESERVED3[16];
	volatile uint32_t IABR[16];		// Interrupt Active bit Register.
	uint32_t RESERVED4[16];
	volatile uint32_t ITNS[16];		// Interrupt Non-Secure State Register.
	uint32_t RESERVED5[16];
	volatile uint8_t IPR[496];		// Interrupt Priority Register (8Bit wide).
	uint32_t RESERVED6[580];
	volatile  uint32_t STIR;		// Software Trigger Interrupt Register.
} NVIC_base_address_t;

/*** NVIC base address ***/

#define NVIC	((NVIC_base_address_t*) ((unsigned int) 0xE000E100))

#endif /* __NVIC_REG_H__ */
