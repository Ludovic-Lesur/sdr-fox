/*
 * pmc_reg.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __PMC_REG_H__
#define __PMC_REG_H__

#include "types.h"

/*** PMC registers ***/

typedef struct {
	uint8_t RESERVED0[4];
	volatile  uint32_t STATUS;		// Power Management Controller FSM (Finite State Machines) status, offset: 0x4.
	volatile uint32_t RESETCTRL;	// Reset Control [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x8.
	uint8_t RESERVED1[4];
	volatile uint32_t DCDC0;		// DCDC (first) control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x10.
	volatile uint32_t DCDC1;		// DCDC (second) control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x14.
	uint8_t RESERVED2[4];
	volatile uint32_t LDOPMU;		// Power Management Unit (PMU) and Always-On domains LDO control [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x1C.
	uint8_t RESERVED3[16];
	volatile uint32_t BODVBAT;		// VBAT Brown Out Dectector (BoD) control register [Reset by: PoR, Pin Reset, Software Reset], offset: 0x30.
	uint8_t RESERVED4[12];
	volatile uint32_t REFFASTWKUP;	// Analog References fast wake-up Control register [Reset by: PoR], offset: 0x40.
	uint8_t RESERVED5[8];
	volatile uint32_t XTAL32K;		// 32 KHz Crystal oscillator (XTAL) control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x4C.
	volatile uint32_t COMP;			// Analog Comparator control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x50.
	uint8_t RESERVED6[16];
	volatile uint32_t WAKEUPIOCTRL;	// Deep Power Down wake-up source [Reset by: PoR, Pin Reset, Software Reset], offset: 0x64.
	volatile uint32_t WAKEIOCAUSE;	// Allows to identify the Wake-up I/O source from Deep Power Down mode, offset: 0x68.
	uint8_t RESERVED7[8];
	volatile uint32_t STATUSCLK;	// FRO and XTAL status register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x74.
	uint8_t RESERVED8[12];
	volatile uint32_t AOREG1;		// General purpose always on domain data storage [Reset by: PoR, Brown Out Detectors Reset], offset: 0x84.
	uint8_t RESERVED9[8];
	volatile uint32_t MISCCTRL;		// Dummy Control bus to PMU [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x90.
	uint8_t RESERVED10[4];
	volatile uint32_t RTCOSC32K;	// RTC 1 KHZ and 1 Hz clocks source control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x98.
	volatile uint32_t OSTIMER;		// OS Timer control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x9C, 'r' suffix has been added to avoid a clash with peripheral base pointer macro 'OSTIMER'.
	uint8_t RESERVED11[24];
	volatile uint32_t PDRUNCFG0;	// Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xB8.
	uint8_t RESERVED12[4];
	volatile uint32_t PDRUNCFGSET0;	// Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xC0.
	uint8_t RESERVED13[4];
	volatile uint32_t PDRUNCFGCLR0;	// Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xC8.
	uint8_t RESERVED14[8];
	volatile uint32_t SRAMCTRL;		// All SRAMs common control signals [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Software Reset], offset: 0xD4.
} PMC_registers_t;

/*** PMC base address ***/

#define PMC		((PMC_registers_t*) ((uint32_t) 0x40020000))

#endif /* __PMC_REG_H__ */
