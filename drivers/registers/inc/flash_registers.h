/*
 * flash_registers.h
 *
 *  Created on: 12 mar. 2023
 *      Author: Ludo
 */

#ifndef __FLASH_REGISTERS_H__
#define __FLASH_REGISTERS_H__

#include "types.h"

/*** FLASH REGISTERS macros ***/

#define FLASH   ((FLASH_registers_t*) ((uint32_t) 0x40034000))

/*** FLASH REGISTERS structures ***/

/*!******************************************************************
 * \struct FLASH_registers_t
 * \brief FLASH registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CMD;
    volatile uint32_t EVENT;
    uint8_t RESERVED0[8];
    volatile uint32_t STARTA;
    volatile uint32_t STOPA;
    uint8_t RESERVED1[104];
    volatile uint32_t DATAW[4];
    uint8_t RESERVED2[3912];
    volatile uint32_t INT_CLR_ENABLE;
    volatile uint32_t INT_SET_ENABLE;
    volatile uint32_t INT_STATUS;
    volatile uint32_t INT_ENABLE;
    volatile uint32_t INT_CLR_STATUS;
    volatile uint32_t INT_SET_STATUS;
    uint8_t RESERVED3[12];
    volatile uint32_t MODULE_ID;
} FLASH_registers_t;

#endif /* __FLASH_REGISTERS_H__ */
