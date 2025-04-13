/*
 * usb_hs_device_registers.h
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_DEVICE_REGISTERS_H__
#define __USB_HS_DEVICE_REGISTERS_H__

/*** USB_HS_DEVICE REGISTERS macros ***/

#define USB_HS_DEVICE   ((USB_HS_DEVICE_registers_t*) ((uint32_t) 0x40094000))

/*** USB_HS_DEVICE REGISTERS structures ***/

/*!******************************************************************
 * \struct USB_HS_DEVICE_registers_t
 * \brief USB HS DEVICE registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t DEVCMDSTAT;
    volatile uint32_t INFO;
    volatile uint32_t EPLISTSTART;
    volatile uint32_t DATABUFSTART;
    volatile uint32_t LPM;
    volatile uint32_t EPSKIP;
    volatile uint32_t EPINUSE;
    volatile uint32_t EPBUFCFG;
    volatile uint32_t INTSTAT;
    volatile uint32_t INTEN;
    volatile uint32_t INTSETSTAT;
    uint8_t RESERVED[8];
    volatile uint32_t EPTOGGLE;
} USB_HS_DEVICE_registers_t;

#endif /* __USB_HS_DEVICE_REGISTERS_H__ */
