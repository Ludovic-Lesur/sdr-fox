/*
 * scb_reg.h
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
*/

#ifndef __SCB_REG_H__
#define __SCB_REG_H__

#include "types.h"

/*** SCB registers ***/

typedef struct {
	volatile uint32_t CPUID;			// CPUID Base Register.
	volatile uint32_t ICSR;				// Interrupt Control and State Register.
	volatile uint32_t VTOR;				// Vector Table Offset Register.
	volatile uint32_t AIRCR;			// Application Interrupt and Reset Control Register.
	volatile uint32_t SCR;				// System Control Register.
	volatile uint32_t CCR;				// Configuration Control Register.
	volatile uint8_t  SHPR[12];			// System Handlers Priority Registers (4-7, 8-11, 12-15).
	volatile uint32_t SHCSR;			// System Handler Control and State Register.
	volatile uint32_t CFSR;				// Configurable Fault Status Register.
	volatile uint32_t HFSR;				// HardFault Status Register.
	volatile uint32_t DFSR;				// Debug Fault Status Register.
	volatile uint32_t MMFAR;			// MemManage Fault Address Register.
	volatile uint32_t BFAR;				// BusFault Address Register.
	volatile uint32_t AFSR;				// Auxiliary Fault Status Register.
	volatile uint32_t ID_PFR[2];		// Processor Feature Register.
	volatile uint32_t ID_DFR;			// Debug Feature Register.
	volatile uint32_t ID_ADR;			// Auxiliary Feature Register.
	volatile uint32_t ID_MMFR[4];		// Memory Model Feature Register.
	volatile uint32_t ID_ISAR[6];		// Instruction Set Attributes Register.
	volatile uint32_t CLIDR;			// Cache Level ID register.
	volatile uint32_t CTR;				// Cache Type register.
	volatile uint32_t CCSIDR;			// Cache Size ID Register.
	volatile uint32_t CSSELR;			// Cache Size Selection Register.
	volatile uint32_t CPACR;			// Coprocessor Access Control Register.
	volatile uint32_t NSACR;			// Non-Secure Access Control Register.
	uint32_t RESERVED3[92];
	volatile uint32_t STIR;				// Software Triggered Interrupt Register.
	uint32_t RESERVED4[15];
	volatile uint32_t MVFR0;			// Media and VFP Feature Register 0.
	volatile uint32_t MVFR1;			// Media and VFP Feature Register 1.
	volatile uint32_t MVFR2;			// Media and VFP Feature Register 2.
	uint32_t RESERVED5;
	volatile uint32_t ICIALLU;			// I-Cache Invalidate All to PoU.
	uint32_t RESERVED6;
	volatile uint32_t ICIMVAU;			// I-Cache Invalidate by MVA to PoU.
	volatile uint32_t DCIMVAC;			// D-Cache Invalidate by MVA to PoC.
	volatile uint32_t DCISW;			// D-Cache Invalidate by Set-way.
	volatile uint32_t DCCMVAU;			// D-Cache Clean by MVA to PoU.
	volatile uint32_t DCCMVAC;			// D-Cache Clean by MVA to PoC.
	volatile uint32_t DCCSW;			// D-Cache Clean by Set-way.
	volatile uint32_t DCCIMVAC;			// D-Cache Clean and Invalidate by MVA to PoC.
	volatile uint32_t DCCISW;			// D-Cache Clean and Invalidate by Set-way.
	volatile uint32_t BPIALL;			// Branch Predictor Invalidate All.
} SCB_base_address_t;

/*** SYSCON base address ***/

#define SCB		((SCB_base_address_t*) ((uint32_t) 0xE000ED00))

#endif /* __SCB_REG_H__ */
