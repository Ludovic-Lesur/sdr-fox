/*
 * usb_hs_phy.c
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#include "usb_hs_phy.h"

#include "anactrl_reg.h"
#include "pmc_reg.h"
#include "syscon_reg.h"
#include "systick.h"
#include "usb_hs_phy_reg.h"

/*** USB HS PHY functions ***/

/* INIT USB HS PHY.
 * @param:			None.
 * @return status:	Function execution status.
 */
USB_HS_PHY_status_t USB_HS_PHY_init(void) {
	// Local variables.
	USB_HS_PHY_status_t status = USB_HS_PHY_SUCCESS;
	SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
	// Power.
	PMC -> PDRUNCFGCLR0 = (0b1 << 12) | (0b1 << 18);
	// Enable clock.
	ANACTRL -> XO32M_CTRL |= (0b1 << 23);
	SYSCON -> AHBCLKCTRLSET[2] |= (0b1 << 7);
	// Reset peripheral.
	SYSCON -> PRESETCTRLSET[2] |= (0b1 << 7);
	SYSCON -> PRESETCTRLCLR[2] |= (0b1 << 7);
	USB_HS_PHY -> CTRL_CLR = (0b1 << 31);
	// Set PLL for 16MHz crystal.
	USB_HS_PHY -> PLL_SIC &= ~(0b111 << 22);
	USB_HS_PHY -> PLL_SIC |= (0b110 << 22);
	USB_HS_PHY -> PLL_SIC_SET = (0b1 << 21);
	USB_HS_PHY -> PLL_SIC_CLR = (0b1 << 16) | (0b11 << 25); // Reserved bits to be cleared.
	USB_HS_PHY -> PLL_SIC_CLR = (0b1 << 13);
	// PLL delay.
	systick_status = SYSTICK_delay_milliseconds(10);
	SYSTICK_status_check(USB_HS_PHY_ERROR_BASE_SYSTICK);
	USB_HS_PHY -> PLL_SIC_SET = (0b1 << 12);
	USB_HS_PHY -> PLL_SIC_SET = (0b1 << 6);
	// Enable auto power-down.
	USB_HS_PHY -> CTRL_SET = (0b1 << 30);
	USB_HS_PHY -> PWD_SET = 0;
	USB_HS_PHY -> CTRL_SET = (0b1 << 15);
	USB_HS_PHY -> CTRL_SET = (0b1 << 14);
	USB_HS_PHY -> CTRL_SET = (0b1 << 19);
	// Enable auto-resume.
	USB_HS_PHY -> CTRL_SET = (0b1 << 18);
	USB_HS_PHY -> CTRL_SET = (0b1 << 20);
	USB_HS_PHY -> CTRL_SET = (0b1 << 1);
errors:
	return status;
}

