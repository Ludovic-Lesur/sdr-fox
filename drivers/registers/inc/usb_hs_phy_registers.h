/*
 * usb_hs_phy_registers.h
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_PHY_REGISTERS_H__
#define __USB_HS_PHY_REGISTERS_H__

/*** USB_HS_PHY REGISTERS macros ***/

#define USB_HS_PHY      ((USB_HS_PHY_registers_t*) ((uint32_t) 0x40038000))

/*** USB_HS_PHY REGISTERS structures ***/

/*!******************************************************************
 * \struct USB_HS_PHY_registers_t
 * \brief USB HS PHY registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t PWD;
    volatile uint32_t PWD_SET;
    volatile uint32_t PWD_CLR;
    volatile uint32_t PWD_TOG;
    volatile uint32_t TX;
    volatile uint32_t TX_SET;
    volatile uint32_t TX_CLR;
    volatile uint32_t TX_TOG;
    volatile uint32_t RX;
    volatile uint32_t RX_SET;
    volatile uint32_t RX_CLR;
    volatile uint32_t RX_TOG;
    volatile uint32_t CTRL;
    volatile uint32_t CTRL_SET;
    volatile uint32_t CTRL_CLR;
    volatile uint32_t CTRL_TOG;
    volatile uint32_t STATUS;
    uint8_t RESERVED0[92];
    volatile uint32_t PLL_SIC;
    volatile uint32_t PLL_SIC_SET;
    volatile uint32_t PLL_SIC_CLR;
    volatile uint32_t PLL_SIC_TOG;
    uint8_t RESERVED1[16];
    volatile uint32_t USB1_VBUS_DETECT;
    volatile uint32_t USB1_VBUS_DETECT_SET;
    volatile uint32_t USB1_VBUS_DETECT_CLR;
    volatile uint32_t USB1_VBUS_DETECT_TOG;
    uint8_t RESERVED2[48];
    volatile uint32_t ANACTRL_REG;
    volatile uint32_t ANACTRL_SET;
    volatile uint32_t ANACTRL_CLR;
    volatile uint32_t ANACTRL_TOG;
} USB_HS_PHY_registers_t;

#endif /* __USB_HS_PHY_REGISTERS_H__ */
