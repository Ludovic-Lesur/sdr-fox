/*
 * anactrl_reg.h
 *
 *  Created on: 11 mar 2023
 *      Author: Ludo
 */

#ifndef __ANACTRL_REG_H__
#define __ANACTRL_REG_H__

#include "types.h"

/*** ANACTRL registers ***/

typedef struct {
	volatile uint32_t ANALOG_CTRL_CFG;		// Various Analog blocks configuration (like FRO 192MHz trimmings source ...), offset: 0x0.
	volatile uint32_t ANALOG_CTRL_STATUS;	// Analog Macroblock Identity registers, Flash Status registers, offset: 0x4.
	uint8_t RESERVED_0[4];
	volatile uint32_t FREQ_ME_CTRL;			// Frequency Measure function control register, offset: 0xC.
	volatile uint32_t FRO192M_CTRL;			// 192MHz Free Running OScillator (FRO) Control register, offset: 0x10.
	volatile uint32_t FRO192M_STATUS;		// 192MHz Free Running OScillator (FRO) Status register, offset: 0x14.
	volatile uint32_t ADC_CTRL;				// General Purpose ADC VBAT Divider branch control, offset: 0x18.
	uint8_t RESERVED_1[4];
	volatile uint32_t XO32M_CTRL;			// High speed Crystal Oscillator Control register, offset: 0x20.
	volatile uint32_t XO32M_STATUS;			// High speed Crystal Oscillator Status register, offset: 0x24.
	uint8_t RESERVED_2[8];
	volatile uint32_t BOD_DCDC_INT_CTRL;	// Brown Out Detectors (BoDs) & DCDC interrupts generation control register, offset: 0x30.
	volatile uint32_t BOD_DCDC_INT_STATUS;	// BoDs & DCDC interrupts status register, offset: 0x34.
	uint8_t RESERVED_3[8];
	volatile uint32_t RINGO0_CTRL;			// First Ring Oscillator module control register., offset: 0x40.
	volatile uint32_t RINGO1_CTRL;			// Second Ring Oscillator module control register., offset: 0x44.
	volatile uint32_t RINGO2_CTRL;			// Third Ring Oscillator module control register., offset: 0x48.
	uint8_t RESERVED_4[100];
	volatile uint32_t LDO_XO32M;			// High Speed Crystal Oscillator (12 MHz - 32 MHz) Voltage Source Supply Control register, offset: 0xB0.
	volatile uint32_t AUX_BIAS;				// AUX_BIAS, offset: 0xB4.
	uint8_t RESERVED_5[72];
	volatile uint32_t USBHS_PHY_CTRL;		// USB High Speed Phy Control, offset: 0x100.
	volatile uint32_t USBHS_PHY_TRIM;		// USB High Speed Phy Trim values, offset: 0x104.
} ANACTRL_registers_t;

/*** ANACTRL base address ***/

#define ANACTRL		((ANACTRL_registers_t*) ((uint32_t) 0x40013000))

#endif /* __ANACTRL_REG_H__ */
