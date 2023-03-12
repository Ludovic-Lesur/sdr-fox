/*
 * syscon_reg.h
 *
 *  Created on: 8 mar 2023
 *      Author: Ludo
*/

#ifndef __SYSCON_REG_H__
#define __SYSCON_REG_H__

#include "types.h"

typedef struct {
	volatile uint32_t MEMORYREMAP;					// Memory Remap control register, offset: 0x0.
	uint8_t RESERVED_0[12];
	volatile uint32_t AHBMATPRIO;					// AHB Matrix priority control register Priority values are 3 = highest, 0 = lowest, offset: 0x10.
	uint8_t RESERVED_1[36];
	volatile uint32_t CPU0STCKCAL;					// System tick calibration for secure part of CPU0, offset: 0x38.
	volatile uint32_t CPU0NSTCKCAL;					// System tick calibration for non-secure part of CPU0, offset: 0x3C.
	volatile uint32_t CPU1STCKCAL;					// System tick calibration for CPU1, offset: 0x40.
	uint8_t RESERVED_2[4];
	volatile uint32_t NMISRC;						// NMI Source Select, offset: 0x48.
	uint8_t RESERVED_3[180];
	union {											// offset: 0x100.
		struct {									// offset: 0x100.
			volatile uint32_t PRESETCTRL0;			// Peripheral reset control 0, offset: 0x100.
			volatile uint32_t PRESETCTRL1;			// Peripheral reset control 1, offset: 0x104.
			volatile uint32_t PRESETCTRL2;			// Peripheral reset control 2, offset: 0x108.
		} PRESETCTRL;
		volatile uint32_t PRESETCTRLX[3];			// Peripheral reset control register, array offset: 0x100, array step: 0x4.
	};
	uint8_t RESERVED_4[20];
	volatile uint32_t PRESETCTRLSET[3];				// Peripheral reset control set register, array offset: 0x120, array step: 0x4.
	uint8_t RESERVED_5[20];
	volatile uint32_t PRESETCTRLCLR[3];				// Peripheral reset control clear register, array offset: 0x140, array step: 0x4.
	uint8_t RESERVED_6[20];
	volatile  uint32_t SWR_RESET;					// generate a software_reset, offset: 0x160.
	uint8_t RESERVED_7[156];
union {												// offset: 0x200.
		struct {									// offset: 0x200.
			volatile uint32_t AHBCLKCTRL0;			// AHB Clock control 0, offset: 0x200.
			volatile uint32_t AHBCLKCTRL1;			// AHB Clock control 1, offset: 0x204.
			volatile uint32_t AHBCLKCTRL2;			// AHB Clock control 2, offset: 0x208.
		} AHBCLKCTRL;
		volatile uint32_t AHBCLKCTRLX[3];			// Peripheral reset control register, array offset: 0x200, array step: 0x4.
	};
	uint8_t RESERVED_8[20];
	volatile uint32_t AHBCLKCTRLSET[3];				// Peripheral reset control register, array offset: 0x220, array step: 0x4.
	uint8_t RESERVED_9[20];
	volatile uint32_t AHBCLKCTRLCLR[3];				// Peripheral reset control register, array offset: 0x240, array step: 0x4.
	uint8_t RESERVED_10[20];
	union {											// offset: 0x260.
		struct {									// offset: 0x260.
			volatile uint32_t SYSTICKCLKSEL0;		// System Tick Timer for CPU0 source select, offset: 0x260.
			volatile uint32_t SYSTICKCLKSEL1;		// System Tick Timer for CPU1 source select, offset: 0x264.
		} SYSTICKCLKSEL;
		volatile uint32_t SYSTICKCLKSELX[2];		// Peripheral reset control register, array offset: 0x260, array step: 0x4.
	};
	volatile uint32_t TRACECLKSEL;					// Trace clock source select, offset: 0x268.
	union {											// offset: 0x26C.
		struct {									// offset: 0x26C.
			volatile uint32_t CTIMERCLKSEL0;		// CTimer 0 clock source select, offset: 0x26C.
			volatile uint32_t CTIMERCLKSEL1;		// CTimer 1 clock source select, offset: 0x270.
			volatile uint32_t CTIMERCLKSEL2;		// CTimer 2 clock source select, offset: 0x274.
			volatile uint32_t CTIMERCLKSEL3;		// CTimer 3 clock source select, offset: 0x278.
			volatile uint32_t CTIMERCLKSEL4;		// CTimer 4 clock source select, offset: 0x27C.
		} CTIMERCLKSEL;
		volatile uint32_t CTIMERCLKSELX[5];			// Peripheral reset control register, array offset: 0x26C, array step: 0x4.
	};
	volatile uint32_t MAINCLKSELA;					// Main clock A source select, offset: 0x280.
	volatile uint32_t MAINCLKSELB;					// Main clock source select, offset: 0x284.
	volatile uint32_t CLKOUTSEL;					// CLKOUT clock source select, offset: 0x288.
	uint8_t RESERVED_11[4];
	volatile uint32_t PLL0CLKSEL;					// PLL0 clock source select, offset: 0x290.
	volatile uint32_t PLL1CLKSEL;					// PLL1 clock source select, offset: 0x294.
	uint8_t RESERVED_12[12];
	volatile uint32_t ADCCLKSEL;					// ADC clock source select, offset: 0x2A4.
	volatile uint32_t USB0CLKSEL;					// FS USB clock source select, offset: 0x2A8.
	uint8_t RESERVED_13[4];
	union {											// offset: 0x2B0.
		struct {									// offset: 0x2B0.
			volatile uint32_t FCCLKSEL0;			// Flexcomm Interface 0 clock source select for Fractional Rate Divider, offset: 0x2B0.
			volatile uint32_t FCCLKSEL1;			// Flexcomm Interface 1 clock source select for Fractional Rate Divider, offset: 0x2B4.
			volatile uint32_t FCCLKSEL2;			// Flexcomm Interface 2 clock source select for Fractional Rate Divider, offset: 0x2B8.
			volatile uint32_t FCCLKSEL3;			// Flexcomm Interface 3 clock source select for Fractional Rate Divider, offset: 0x2BC.
			volatile uint32_t FCCLKSEL4;			// Flexcomm Interface 4 clock source select for Fractional Rate Divider, offset: 0x2C0.
			volatile uint32_t FCCLKSEL5;			// Flexcomm Interface 5 clock source select for Fractional Rate Divider, offset: 0x2C4.
			volatile uint32_t FCCLKSEL6;			// Flexcomm Interface 6 clock source select for Fractional Rate Divider, offset: 0x2C8.
			volatile uint32_t FCCLKSEL7;			// Flexcomm Interface 7 clock source select for Fractional Rate Divider, offset: 0x2CC.
		} FCCLKSEL;
		volatile uint32_t FCCLKSELX[8];				// Peripheral reset control register, array offset: 0x2B0, array step: 0x4.
	};
	volatile uint32_t HSLSPICLKSEL;					// HS LSPI clock source select, offset: 0x2D0.
	uint8_t RESERVED_14[12];
	volatile uint32_t MCLKCLKSEL;					// MCLK clock source select, offset: 0x2E0.
	uint8_t RESERVED_15[12];
	volatile uint32_t SCTCLKSEL;					// SCTimer/PWM clock source select, offset: 0x2F0.
	uint8_t RESERVED_16[4];
	volatile uint32_t SDIOCLKSEL;					// SDIO clock source select, offset: 0x2F8.
	uint8_t RESERVED_17[4];
	volatile uint32_t SYSTICKCLKDIV0;				// System Tick Timer divider for CPU0, offset: 0x300.
	volatile uint32_t SYSTICKCLKDIV1;				// System Tick Timer divider for CPU1, offset: 0x304.
	volatile uint32_t TRACECLKDIV;					// TRACE clock divider, offset: 0x308.
	uint8_t RESERVED_18[20];
	union {											// offset: 0x320.
		struct {									// offset: 0x320.
			volatile uint32_t FLEXFRG0CTRL;			// Fractional rate divider for flexcomm 0, offset: 0x320.
			volatile uint32_t FLEXFRG1CTRL;			// Fractional rate divider for flexcomm 1, offset: 0x324.
			volatile uint32_t FLEXFRG2CTRL;			// Fractional rate divider for flexcomm 2, offset: 0x328.
			volatile uint32_t FLEXFRG3CTRL;			// Fractional rate divider for flexcomm 3, offset: 0x32C.
			volatile uint32_t FLEXFRG4CTRL;			// Fractional rate divider for flexcomm 4, offset: 0x330.
			volatile uint32_t FLEXFRG5CTRL;			// Fractional rate divider for flexcomm 5, offset: 0x334.
			volatile uint32_t FLEXFRG6CTRL;			// Fractional rate divider for flexcomm 6, offset: 0x338.
			volatile uint32_t FLEXFRG7CTRL;			// Fractional rate divider for flexcomm 7, offset: 0x33C.
		} FLEXFRGCTRL;
		volatile uint32_t FLEXFRGXCTRL[8];			// Peripheral reset control register, array offset: 0x320, array step: 0x4.
	};
	uint8_t RESERVED_19[64];
	volatile uint32_t AHBCLKDIV;					// System clock divider, offset: 0x380.
	volatile uint32_t CLKOUTDIV;					// CLKOUT clock divider, offset: 0x384.
	volatile uint32_t FROHFDIV;						// FRO_HF (96MHz) clock divider, offset: 0x388.
	volatile uint32_t WDTCLKDIV;					// WDT clock divider, offset: 0x38C.
	uint8_t RESERVED_20[4];
	volatile uint32_t ADCCLKDIV;					// ADC clock divider, offset: 0x394.
	volatile uint32_t USB0CLKDIV;					// USB0 Clock divider, offset: 0x398.
	uint8_t RESERVED_21[16];
	volatile uint32_t MCLKDIV;						// I2S MCLK clock divider, offset: 0x3AC.
	uint8_t RESERVED_22[4];
	volatile uint32_t SCTCLKDIV;					// SCT/PWM clock divider, offset: 0x3B4.
	uint8_t RESERVED_23[4];
	volatile uint32_t SDIOCLKDIV;					// SDIO clock divider, offset: 0x3BC.
	uint8_t RESERVED_24[4];
	volatile uint32_t PLL0CLKDIV;					// PLL0 clock divider, offset: 0x3C4.
	uint8_t RESERVED_25[52];
	volatile uint32_t CLOCKGENUPDATELOCKOUT;		// Control clock configuration registers access (like xxxDIV, xxxSEL), offset: 0x3FC.
	volatile uint32_t FMCCR;						// FMC configuration register, offset: 0x400.
	uint8_t RESERVED_26[8];
	volatile uint32_t USB0NEEDCLKCTRL;				// USB0 need clock control, offset: 0x40C.
	volatile  uint32_t USB0NEEDCLKSTAT;				// USB0 need clock status, offset: 0x410.
	uint8_t RESERVED_27[8];
	volatile  uint32_t FMCFLUSH;					// FMCflush control, offset: 0x41C.
	volatile uint32_t MCLKIO;						// MCLK control, offset: 0x420.
	volatile uint32_t USB1NEEDCLKCTRL;				// USB1 need clock control, offset: 0x424.
	volatile  uint32_t USB1NEEDCLKSTAT;				// USB1 need clock status, offset: 0x428.
	uint8_t RESERVED_28[52];
	volatile uint32_t SDIOCLKCTRL;					// SDIO CCLKIN phase and delay control, offset: 0x460.
	uint8_t RESERVED_29[252];
	volatile uint32_t PLL1CTRL;						// PLL1 550m control, offset: 0x560.
	volatile  uint32_t PLL1STAT;					// PLL1 550m status, offset: 0x564.
	volatile uint32_t PLL1NDEC;						// PLL1 550m N divider, offset: 0x568.
	volatile uint32_t PLL1MDEC;						// PLL1 550m M divider, offset: 0x56C.
	volatile uint32_t PLL1PDEC;						// PLL1 550m P divider, offset: 0x570.
	uint8_t RESERVED_30[12];
	volatile uint32_t PLL0CTRL;						// PLL0 550m control, offset: 0x580.
	volatile  uint32_t PLL0STAT;					// PLL0 550m status, offset: 0x584.
	volatile uint32_t PLL0NDEC;						// PLL0 550m N divider, offset: 0x588.
	volatile uint32_t PLL0PDEC;						// PLL0 550m P divider, offset: 0x58C.
	volatile uint32_t PLL0SSCG0;					// PLL0 Spread Spectrum Wrapper control register 0, offset: 0x590.
	volatile uint32_t PLL0SSCG1;					// PLL0 Spread Spectrum Wrapper control register 1, offset: 0x594.
	uint8_t RESERVED_31[364];
	volatile uint32_t FUNCRETENTIONCTRL;			// Functional retention control register, offset: 0x704.
	uint8_t RESERVED_32[248];
	volatile uint32_t CPUCTRL;						// CPU Control for multiple processors, offset: 0x800.
	volatile uint32_t CPBOOT;						// Coprocessor Boot Address, offset: 0x804.
	uint8_t RESERVED_33[4];
	volatile  uint32_t CPSTAT;						// CPU Status, offset: 0x80C.
	uint8_t RESERVED_34[520];
	volatile uint32_t CLOCK_CTRL;					// Various system clock controls : Flash clock (48 MHz) control, clocks to Frequency Measures, offset: 0xA18.
	uint8_t RESERVED_35[244];
	volatile uint32_t COMP_INT_CTRL;				// Comparator Interrupt control, offset: 0xB10.
	volatile  uint32_t COMP_INT_STATUS;				// Comparator Interrupt status, offset: 0xB14.
	uint8_t RESERVED_36[748];
	volatile uint32_t AUTOCLKGATEOVERRIDE;			// Control automatic clock gating, offset: 0xE04.
	volatile uint32_t GPIOPSYNC;					// Enable bypass of the first stage of synchonization inside GPIO_INT module, offset: 0xE08.
	uint8_t RESERVED_37[404];
	volatile uint32_t DEBUG_LOCK_EN;				// Control write access to security registers., offset: 0xFA0.
	volatile uint32_t DEBUG_FEATURES;				// Cortex M33 (CPU0) and micro Cortex M33 (CPU1) debug features control., offset: 0xFA4.
	volatile uint32_t DEBUG_FEATURES_DP;			// Cortex M33 (CPU0) and micro Cortex M33 (CPU1) debug features control DUPLICATE register., offset: 0xFA8.
	uint8_t RESERVED_38[16];
	volatile  uint32_t KEY_BLOCK;					// block quiddikey/PUF all index., offset: 0xFBC.
	volatile uint32_t DEBUG_AUTH_BEACON;			// Debug authentication BEACON register, offset: 0xFC0.
	uint8_t RESERVED_39[16];
	volatile uint32_t CPUCFG;						// CPUs configuration register, offset: 0xFD4.
	uint8_t RESERVED_40[32];
	volatile  uint32_t DEVICE_ID0;					// Device ID, offset: 0xFF8.
	volatile  uint32_t DIEID;						// Chip revision ID and Number, offset: 0xFFC.
} SYSCON_registers_t;

/*** SYSCON base address ***/

#define SYSCON	((SYSCON_registers_t*) ((uint32_t) 0x40000000))

#endif /* __SYSCON_REG_H__ */
