/*
 * pmc_registers.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __PMC_REGISTERS_H__
#define __PMC_REGISTERS_H__

#include "types.h"

/*** PMC REGISTERS macros ***/

#define PMC     ((PMC_registers_t*) ((uint32_t) 0x40020000))

/*** PMC REGISTERS structures ***/

/*!******************************************************************
 * \struct PMC_registers_t
 * \brief PMC registers map.
 *******************************************************************/
typedef struct {
    uint8_t RESERVED0[4];
    volatile uint32_t STATUS;
    volatile uint32_t RESETCTRL;
    uint8_t RESERVED1[4];
    volatile uint32_t DCDC0;
    volatile uint32_t DCDC1;
    uint8_t RESERVED2[4];
    volatile uint32_t LDOPMU;
    uint8_t RESERVED3[16];
    volatile uint32_t BODVBAT;
    uint8_t RESERVED4[12];
    volatile uint32_t REFFASTWKUP;
    uint8_t RESERVED5[8];
    volatile uint32_t XTAL32K;
    volatile uint32_t COMP;
    uint8_t RESERVED6[16];
    volatile uint32_t WAKEUPIOCTRL;
    volatile uint32_t WAKEIOCAUSE;
    uint8_t RESERVED7[8];
    volatile uint32_t STATUSCLK;
    uint8_t RESERVED8[12];
    volatile uint32_t AOREG1;
    uint8_t RESERVED9[8];
    volatile uint32_t MISCCTRL;
    uint8_t RESERVED10[4];
    volatile uint32_t RTCOSC32K;
    volatile uint32_t OSTIMER;
    uint8_t RESERVED11[24];
    volatile uint32_t PDRUNCFG0;
    uint8_t RESERVED12[4];
    volatile uint32_t PDRUNCFGSET0;
    uint8_t RESERVED13[4];
    volatile uint32_t PDRUNCFGCLR0;
    uint8_t RESERVED14[8];
    volatile uint32_t SRAMCTRL;
} PMC_registers_t;

#endif /* __PMC_REGISTERS_H__ */
