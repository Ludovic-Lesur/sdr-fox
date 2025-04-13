/*
 * nvic_registers.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __NVIC_REGISTERS_H__
#define __NVIC_REGISTERS_H__

#include "types.h"

/*** NVIC REGISTERS macros ***/

#define NVIC    ((NVIC_registers_t*) ((uint32_t) 0xE000E100))

/*** NVIC REGISTERS structures ***/

/*!******************************************************************
 * \struct NVIC_registers_t
 * \brief NVIC registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t ISER[16];
    uint32_t RESERVED0[16];
    volatile uint32_t ICER[16];
    uint32_t RESERVED1[16];
    volatile uint32_t ISPR[16];
    uint32_t RESERVED2[16];
    volatile uint32_t ICPR[16];
    uint32_t RESERVED3[16];
    volatile uint32_t IABR[16];
    uint32_t RESERVED4[16];
    volatile uint32_t ITNS[16];
    uint32_t RESERVED5[16];
    volatile uint8_t IPR[496];
    uint32_t RESERVED6[580];
    volatile uint32_t STIR;
} NVIC_registers_t;

#endif /* __NVIC_REGISTERS_H__ */
