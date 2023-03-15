/*
 * usb_hs_phy_reg.h
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_PHY_REG_H__
#define __USB_HS_PHY_REG_H__

/*** USB HS PHY registers ***/

typedef struct {
	volatile uint32_t PWD;						// USB PHY Power-Down Register, offset: 0x0.
	volatile uint32_t PWD_SET;					// USB PHY Power-Down Register, offset: 0x4.
	volatile uint32_t PWD_CLR;					// USB PHY Power-Down Register, offset: 0x8.
	volatile uint32_t PWD_TOG;					// USB PHY Power-Down Register, offset: 0xC.
	volatile uint32_t TX;						// USB PHY Transmitter Control Register, offset: 0x10.
	volatile uint32_t TX_SET;					// USB PHY Transmitter Control Register, offset: 0x14.
	volatile uint32_t TX_CLR;					// USB PHY Transmitter Control Register, offset: 0x18.
	volatile uint32_t TX_TOG;					// USB PHY Transmitter Control Register, offset: 0x1C.
	volatile uint32_t RX;						// USB PHY Receiver Control Register, offset: 0x20.
	volatile uint32_t RX_SET;					// USB PHY Receiver Control Register, offset: 0x24.
	volatile uint32_t RX_CLR;					// USB PHY Receiver Control Register, offset: 0x28.
	volatile uint32_t RX_TOG;					// USB PHY Receiver Control Register, offset: 0x2C.
	volatile uint32_t CTRL;						// USB PHY General Control Register, offset: 0x30.
	volatile uint32_t CTRL_SET;					// USB PHY General Control Register, offset: 0x34.
	volatile uint32_t CTRL_CLR;					// USB PHY General Control Register, offset: 0x38.
	volatile uint32_t CTRL_TOG;					// USB PHY General Control Register, offset: 0x3C.
	volatile uint32_t STATUS;					// USB PHY Status Register, offset: 0x40.
	uint8_t RESERVED0[92];
	volatile uint32_t PLL_SIC;					// USB PHY PLL Control/Status Register, offset: 0xA0.
	volatile uint32_t PLL_SIC_SET;				// USB PHY PLL Control/Status Register, offset: 0xA4.
	volatile uint32_t PLL_SIC_CLR;				// USB PHY PLL Control/Status Register, offset: 0xA8.
	volatile uint32_t PLL_SIC_TOG;				// USB PHY PLL Control/Status Register, offset: 0xAC.
	uint8_t RESERVED1[16];
	volatile uint32_t USB1_VBUS_DETECT;			// USB PHY VBUS Detect Control Register, offset: 0xC0.
	volatile uint32_t USB1_VBUS_DETECT_SET;		// USB PHY VBUS Detect Control Register, offset: 0xC4.
	volatile uint32_t USB1_VBUS_DETECT_CLR;		// USB PHY VBUS Detect Control Register, offset: 0xC8.
	volatile uint32_t USB1_VBUS_DETECT_TOG;		// USB PHY VBUS Detect Control Register, offset: 0xCC.
	uint8_t RESERVED2[48];
	volatile uint32_t ANACTRL_REG;				// USB PHY Analog Control Register, offset: 0x100.
	volatile uint32_t ANACTRL_SET;				// USB PHY Analog Control Register, offset: 0x104.
	volatile uint32_t ANACTRL_CLR;				// USB PHY Analog Control Register, offset: 0x108.
	volatile uint32_t ANACTRL_TOG;				// USB PHY Analog Control Register, offset: 0x10C.
} USB_HS_PHY_registers_t;

/*** USB HS PHY base address ***/

#define USB_HS_PHY		((USB_HS_PHY_registers_t*) ((uint32_t) 0x40038000))

#endif /* __USB_HS_PHY_REG_H__ */
