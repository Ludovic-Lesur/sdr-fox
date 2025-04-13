/*
 * syscon_registers.h
 *
 *  Created on: 8 mar 2023
 *      Author: Ludo
 */

#ifndef __SYSCON_REGISTERS_H__
#define __SYSCON_REGISTERS_H__

#include "types.h"

/*** SYSCON REGISTERS macros ***/

#define SYSCON  ((SYSCON_registers_t*) ((uint32_t) 0x40000000))

/*** SYSCON REGISTERS structures ***/

/*!******************************************************************
 * \struct SYSCON_registers_t
 * \brief SYSCON registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t MEMORYREMAP;
    uint8_t RESERVED_0[12];
    volatile uint32_t AHBMATPRIO;
    uint8_t RESERVED_1[36];
    volatile uint32_t CPU0STCKCAL;
    volatile uint32_t CPU0NSTCKCAL;
    volatile uint32_t CPU1STCKCAL;
    uint8_t RESERVED_2[4];
    volatile uint32_t NMISRC;
    uint8_t RESERVED_3[180];
    union {
        struct {
            volatile uint32_t PRESETCTRL0;
            volatile uint32_t PRESETCTRL1;
            volatile uint32_t PRESETCTRL2;
        };
        volatile uint32_t PRESETCTRLX[3];
    };
    uint8_t RESERVED_4[20];
    volatile uint32_t PRESETCTRLSET[3];
    uint8_t RESERVED_5[20];
    volatile uint32_t PRESETCTRLCLR[3];
    uint8_t RESERVED_6[20];
    volatile uint32_t SWR_RESET;
    uint8_t RESERVED_7[156];
    union {
        struct {
            volatile uint32_t AHBCLKCTRL0;
            volatile uint32_t AHBCLKCTRL1;
            volatile uint32_t AHBCLKCTRL2;
        };
        volatile uint32_t AHBCLKCTRLX[3];
    };
    uint8_t RESERVED_8[20];
    volatile uint32_t AHBCLKCTRLSET[3];
    uint8_t RESERVED_9[20];
    volatile uint32_t AHBCLKCTRLCLR[3];
    uint8_t RESERVED_10[20];
    union {
        struct {
            volatile uint32_t SYSTICKCLKSEL0;
            volatile uint32_t SYSTICKCLKSEL1;
        };
        volatile uint32_t SYSTICKCLKSELX[2];
    };
    volatile uint32_t TRACECLKSEL;
    union {
        struct {
            volatile uint32_t CTIMERCLKSEL0;
            volatile uint32_t CTIMERCLKSEL1;
            volatile uint32_t CTIMERCLKSEL2;
            volatile uint32_t CTIMERCLKSEL3;
            volatile uint32_t CTIMERCLKSEL4;
        };
        volatile uint32_t CTIMERCLKSELX[5];
    };
    volatile uint32_t MAINCLKSELA;
    volatile uint32_t MAINCLKSELB;
    volatile uint32_t CLKOUTSEL;
    uint8_t RESERVED_11[4];
    volatile uint32_t PLL0CLKSEL;
    volatile uint32_t PLL1CLKSEL;
    uint8_t RESERVED_12[12];
    volatile uint32_t ADCCLKSEL;
    volatile uint32_t USB0CLKSEL;
    uint8_t RESERVED_13[4];
    union {
        struct {
            volatile uint32_t FCCLKSEL0;
            volatile uint32_t FCCLKSEL1;
            volatile uint32_t FCCLKSEL2;
            volatile uint32_t FCCLKSEL3;
            volatile uint32_t FCCLKSEL4;
            volatile uint32_t FCCLKSEL5;
            volatile uint32_t FCCLKSEL6;
            volatile uint32_t FCCLKSEL7;
        };
        volatile uint32_t FCCLKSELX[8];
    };
    volatile uint32_t HSLSPICLKSEL;
    uint8_t RESERVED_14[12];
    volatile uint32_t MCLKCLKSEL;
    uint8_t RESERVED_15[12];
    volatile uint32_t SCTCLKSEL;
    uint8_t RESERVED_16[4];
    volatile uint32_t SDIOCLKSEL;
    uint8_t RESERVED_17[4];
    union {
        struct {
            volatile uint32_t SYSTICKCLKDIV0;
            volatile uint32_t SYSTICKCLKDIV1;
        };
        volatile uint32_t SYSTICKCLKDIVX[2];
    };
    volatile uint32_t TRACECLKDIV;
    uint8_t RESERVED_18[20];
    union {
        struct {
            volatile uint32_t FLEXFRG0CTRL;
            volatile uint32_t FLEXFRG1CTRL;
            volatile uint32_t FLEXFRG2CTRL;
            volatile uint32_t FLEXFRG3CTRL;
            volatile uint32_t FLEXFRG4CTRL;
            volatile uint32_t FLEXFRG5CTRL;
            volatile uint32_t FLEXFRG6CTRL;
            volatile uint32_t FLEXFRG7CTRL;
        };
        volatile uint32_t FLEXFRGXCTRL[8];
    };
    uint8_t RESERVED_19[64];
    volatile uint32_t AHBCLKDIV;
    volatile uint32_t CLKOUTDIV;
    volatile uint32_t FROHFDIV;
    volatile uint32_t WDTCLKDIV;
    uint8_t RESERVED_20[4];
    volatile uint32_t ADCCLKDIV;
    volatile uint32_t USB0CLKDIV;
    uint8_t RESERVED_21[16];
    volatile uint32_t MCLKDIV;
    uint8_t RESERVED_22[4];
    volatile uint32_t SCTCLKDIV;
    uint8_t RESERVED_23[4];
    volatile uint32_t SDIOCLKDIV;
    uint8_t RESERVED_24[4];
    volatile uint32_t PLL0CLKDIV;
    uint8_t RESERVED_25[52];
    volatile uint32_t CLOCKGENUPDATELOCKOUT;
    volatile uint32_t FMCCR;
    uint8_t RESERVED_26[8];
    volatile uint32_t USB0NEEDCLKCTRL;
    volatile uint32_t USB0NEEDCLKSTAT;
    uint8_t RESERVED_27[8];
    volatile uint32_t FMCFLUSH;
    volatile uint32_t MCLKIO;
    volatile uint32_t USB1NEEDCLKCTRL;
    volatile uint32_t USB1NEEDCLKSTAT;
    uint8_t RESERVED_28[52];
    volatile uint32_t SDIOCLKCTRL;
    uint8_t RESERVED_29[252];
    volatile uint32_t PLL1CTRL;
    volatile uint32_t PLL1STAT;
    volatile uint32_t PLL1NDEC;
    volatile uint32_t PLL1MDEC;
    volatile uint32_t PLL1PDEC;
    uint8_t RESERVED_30[12];
    volatile uint32_t PLL0CTRL;
    volatile uint32_t PLL0STAT;
    volatile uint32_t PLL0NDEC;
    volatile uint32_t PLL0PDEC;
    volatile uint32_t PLL0SSCG0;
    volatile uint32_t PLL0SSCG1;
    uint8_t RESERVED_31[364];
    volatile uint32_t FUNCRETENTIONCTRL;
    uint8_t RESERVED_32[248];
    volatile uint32_t CPUCTRL;
    volatile uint32_t CPBOOT;
    uint8_t RESERVED_33[4];
    volatile uint32_t CPSTAT;
    uint8_t RESERVED_34[520];
    volatile uint32_t CLOCK_CTRL;
    uint8_t RESERVED_35[244];
    volatile uint32_t COMP_INT_CTRL;
    volatile uint32_t COMP_INT_STATUS;
    uint8_t RESERVED_36[748];
    volatile uint32_t AUTOCLKGATEOVERRIDE;
    volatile uint32_t GPIOPSYNC;
    uint8_t RESERVED_37[404];
    volatile uint32_t DEBUG_LOCK_EN;
    volatile uint32_t DEBUG_FEATURES;
    volatile uint32_t DEBUG_FEATURES_DP;
    uint8_t RESERVED_38[16];
    volatile uint32_t KEY_BLOCK;
    volatile uint32_t DEBUG_AUTH_BEACON;
    uint8_t RESERVED_39[16];
    volatile uint32_t CPUCFG;
    uint8_t RESERVED_40[32];
    volatile uint32_t DEVICE_ID0;
    volatile uint32_t DIEID;
} SYSCON_registers_t;

#endif /* __SYSCON_REGISTERS_H__ */
