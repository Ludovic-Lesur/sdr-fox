/*
 * scb_registers.h
 *
 *  Created on: 08 mar. 2023
 *      Author: Ludo
 */

#ifndef __SCB_REGISTERS_H__
#define __SCB_REGISTERS_H__

#include "types.h"

/*** SCB REGISTERS macros ***/

#define SCB     ((SCB_registers_t*) ((uint32_t) 0xE000ED00))

/*** SCB REGISTERS structures ***/

/*!******************************************************************
 * \struct SCB_registers_t
 * \brief SCB registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint8_t SHPR[12];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t DFSR;
    volatile uint32_t MMFAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
    volatile uint32_t ID_PFR[2];
    volatile uint32_t ID_DFR;
    volatile uint32_t ID_ADR;
    volatile uint32_t ID_MMFR[4];
    volatile uint32_t ID_ISAR[6];
    volatile uint32_t CLIDR;
    volatile uint32_t CTR;
    volatile uint32_t CCSIDR;
    volatile uint32_t CSSELR;
    volatile uint32_t CPACR;
    volatile uint32_t NSACR;
    uint32_t RESERVED3[92];
    volatile uint32_t STIR;
    uint32_t RESERVED4[15];
    volatile uint32_t MVFR0;
    volatile uint32_t MVFR1;
    volatile uint32_t MVFR2;
    uint32_t RESERVED5;
    volatile uint32_t ICIALLU;
    uint32_t RESERVED6;
    volatile uint32_t ICIMVAU;
    volatile uint32_t DCIMVAC;
    volatile uint32_t DCISW;
    volatile uint32_t DCCMVAU;
    volatile uint32_t DCCMVAC;
    volatile uint32_t DCCSW;
    volatile uint32_t DCCIMVAC;
    volatile uint32_t DCCISW;
    volatile uint32_t BPIALL;
} SCB_registers_t;

#endif /* __SCB_REGISTERS_H__ */
