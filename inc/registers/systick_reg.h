/*
 * systick_reg.h
 *
 *  Created on: 14 mar. 2023
 *      Author: Ludo
 */

#ifndef __SYSTICK_REG_H__
#define __SYSTICK_REG_H__

/*** SYSTICK registers ***/

typedef struct {
	volatile uint32_t CTRL;		// Offset: 0x000 (R/W)  SysTick Control and Status Register.
	volatile uint32_t LOAD;		// Offset: 0x004 (R/W)  SysTick Reload Value Register.
	volatile uint32_t VAL;		// Offset: 0x008 (R/W)  SysTick Current Value Register.
	volatile uint32_t CALIB;	// Offset: 0x00C (R/ )  SysTick Calibration Register.
} SYSTICK_registers_t;

/*** SYSTICK base address ***/

#define SYSTICK		((SYSTICK_registers_t*) ((uint32_t) 0xE000E010))

#endif /* __SYSTICK_REG_H__ */
