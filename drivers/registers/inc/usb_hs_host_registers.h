/*
 * usb_hs_host_registers.h
 *
 *  Created on: 18 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_HOST_REGISTERS_H__
#define __USB_HS_HOST_REGISTERS_H__

#include "types.h"

/*** USB_HS_HOST REGISTERS macros ***/

#define USB_HS_HOST     ((USB_HS_HOST_registers_t*) ((uint32_t) 0x400A3000))

/*** USB_HS_HOST REGISTERS structures ***/

/*!******************************************************************
 * \struct USB_HS_HOST_registers_t
 * \brief USB HS HOST registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CAPLENGTH_CHIPID;
    volatile uint32_t HCSPARAMS;
    uint8_t RESERVED_0[4];
    volatile uint32_t FLADJ_FRINDEX;
    volatile uint32_t ATLPTD;
    volatile uint32_t ISOPTD;
    volatile uint32_t INTPTD;
    volatile uint32_t DATAPAYLOAD;
    volatile uint32_t USBCMD;
    volatile uint32_t USBSTS;
    volatile uint32_t USBINTR;
    volatile uint32_t PORTSC1;
    volatile uint32_t ATLPTDD;
    volatile uint32_t ATLPTDS;
    volatile uint32_t ISOPTDD;
    volatile uint32_t ISOPTDS;
    volatile uint32_t INTPTDD;
    volatile uint32_t INTPTDS;
    volatile uint32_t LASTPTD;
    uint8_t RESERVED_1[4];
    volatile uint32_t PORTMODE;
} USB_HS_HOST_registers_t;

#endif /* __USB_HS_HOST_REGISTERS_H__ */
