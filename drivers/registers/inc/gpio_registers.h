/*
 * gpio_registers.h
 *
 *  Created on: 08 mar. 2023
 *      Author: ludo
 */

#ifndef __GPIO_REGISTERS_H__
#define __GPIO_REGISTERS_H__

#include "types.h"

/*** GPIO REGISTERS macros ***/

#define GPIO    ((GPIO_registers_t*) ((uint32_t) 0x4008C000))

/*** GPIO REGISTERS structures ***/

/*!******************************************************************
 * \struct GPIO_registers_t
 * \brief GPIO registers map.
 *******************************************************************/
typedef struct {
    volatile uint8_t B[2][32];
    uint8_t RESERVED_0[4032];
    volatile uint32_t W[2][32];
    uint8_t RESERVED_1[3840];
    volatile uint32_t DIR[2];
    uint8_t RESERVED_2[120];
    volatile uint32_t MASK[2];
    uint8_t RESERVED_3[120];
    volatile uint32_t PIN[2];
    uint8_t RESERVED_4[120];
    volatile uint32_t MPIN[2];
    uint8_t RESERVED_5[120];
    volatile uint32_t SET[2];
    uint8_t RESERVED_6[120];
    volatile uint32_t CLR[2];
    uint8_t RESERVED_7[120];
    volatile uint32_t NOT[2];
    uint8_t RESERVED_8[120];
    volatile uint32_t DIRSET[2];
    uint8_t RESERVED_9[120];
    volatile uint32_t DIRCLR[2];
    uint8_t RESERVED_10[120];
    volatile uint32_t DIRNOT[2];
} GPIO_registers_t;

#endif /* __GPIO_REGISTERS_H__ */
