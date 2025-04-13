/*
 * anactrl_registers.h
 *
 *  Created on: 11 mar 2023
 *      Author: Ludo
 */

#ifndef __ANACTRL_REGISTERS_H__
#define __ANACTRL_REGISTERS_H__

#include "types.h"

/*** ANACTRL REGISTERS macros ***/

#define ANACTRL     ((ANACTRL_registers_t*) ((uint32_t) 0x40013000))

/*** ANACTRL REGISTERS structures ***/

/*!******************************************************************
 * \struct ANACTRL_registers_t
 * \brief ANACTRL registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t ANALOG_CTRL_CFG;
    volatile uint32_t ANALOG_CTRL_STATUS;
    uint8_t RESERVED_0[4];
    volatile uint32_t FREQ_ME_CTRL;
    volatile uint32_t FRO192M_CTRL;
    volatile uint32_t FRO192M_STATUS;
    volatile uint32_t ADC_CTRL;
    uint8_t RESERVED_1[4];
    volatile uint32_t XO32M_CTRL;
    volatile uint32_t XO32M_STATUS;
    uint8_t RESERVED_2[8];
    volatile uint32_t BOD_DCDC_INT_CTRL;
    volatile uint32_t BOD_DCDC_INT_STATUS;
    uint8_t RESERVED_3[8];
    volatile uint32_t RINGO0_CTRL;
    volatile uint32_t RINGO1_CTRL;
    volatile uint32_t RINGO2_CTRL;
    uint8_t RESERVED_4[100];
    volatile uint32_t LDO_XO32M;
    volatile uint32_t AUX_BIAS;
    uint8_t RESERVED_5[72];
    volatile uint32_t USBHS_PHY_CTRL;
    volatile uint32_t USBHS_PHY_TRIM;
} ANACTRL_registers_t;

#endif /* __ANACTRL_REGISTERS_H__ */
