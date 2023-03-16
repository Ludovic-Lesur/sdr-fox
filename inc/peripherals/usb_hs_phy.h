/*
 * usb_hs_phy.h
 *
 *  Created on: 13 mar 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_PHY_H__
#define __USB_HS_PHY_H__

#include "anactrl.h"
#include "pmc.h"
#include "syscon.h"
#include "systick.h"

/*** USB HS PHY structures ***/

typedef enum {
	USB_HS_PHY_SUCCESS = 0,
	USB_HS_PHY_ERROR_BASE_PMC = 0x0100,
	USB_HS_PHY_ERROR_BASE_SYSCON = (USB_HS_PHY_ERROR_BASE_PMC + PMC_ERROR_BASE_LAST),
	USB_HS_PHY_ERROR_BASE_ANACTRL = (USB_HS_PHY_ERROR_BASE_SYSCON + SYSCON_ERROR_BASE_LAST),
	USB_HS_PHY_ERROR_BASE_SYSTICK = (USB_HS_PHY_ERROR_BASE_ANACTRL + ANACTRL_ERROR_BASE_LAST),
	USB_HS_PHY_ERROR_BASE_LAST = (USB_HS_PHY_ERROR_BASE_SYSTICK + SYSTICK_ERROR_BASE_LAST)
} USB_HS_PHY_status_t;

/*** USB HS PHY functions ***/

USB_HS_PHY_status_t USB_HS_PHY_init(void);

#define USB_HS_PHY_status_check(error_base) { if (usb_hs_phy_status != USB_HS_PHY_SUCCESS) { status = error_base + usb_hs_phy_status; goto errors; }}
#define USB_HS_PHY_error_check() { ERROR_status_check(usb_hs_phy_status, USB_HS_PHY_SUCCESS, ERROR_BASE_USB_HS_PHY); }
#define USB_HS_PHY_error_check_print() { ERROR_status_check_print(usb_hs_phy_status, USB_HS_PHY_SUCCESS, ERROR_BASE_USB_HS_PHY); }

#endif /* __USB_HS_PHY_H__ */
