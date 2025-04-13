/*
 * usb_hs_phy.h
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_HS_PHY_H__
#define __USB_HS_PHY_H__

#include "anactrl.h"
#include "error.h"
#include "pmc.h"
#include "syscon.h"
#include "systick.h"
#include "types.h"

/*** USB_HS_PHY structures ***/

/*!******************************************************************
 * \enum USB_HS_PHY_status_t
 * \brief USB HS PHY driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USB_HS_PHY_SUCCESS = 0,
    // Low level drivers errors.
    USB_HS_PHY_ERROR_BASE_ANACTRL = ERROR_BASE_STEP,
    USB_HS_PHY_ERROR_BASE_PMC = (USB_HS_PHY_ERROR_BASE_ANACTRL + ANACTRL_ERROR_BASE_LAST),
    USB_HS_PHY_ERROR_BASE_SYSCON = (USB_HS_PHY_ERROR_BASE_PMC + PMC_ERROR_BASE_LAST),
    USB_HS_PHY_ERROR_BASE_SYSTICK = (USB_HS_PHY_ERROR_BASE_SYSCON + SYSCON_ERROR_BASE_LAST),
    // Last base value.
    USB_HS_PHY_ERROR_BASE_LAST = (USB_HS_PHY_ERROR_BASE_SYSTICK + SYSTICK_ERROR_BASE_LAST)
} USB_HS_PHY_status_t;

/*** USB_HS_PHY functions ***/

/*!******************************************************************
 * \fn USB_HS_PHY_status_t USB_HS_PHY_init(void)
 * \brief Init high speed USB PHY.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_HS_PHY_status_t USB_HS_PHY_init(void);

/*!******************************************************************
 * \fn USB_HS_PHY_status_t USB_HS_PHY_de_init(void)
 * \brief Release high speed USB PHY.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_HS_PHY_status_t USB_HS_PHY_de_init(void);

/*******************************************************************/
#define USB_HS_PHY_exit_error(base) { ERROR_check_exit(usb_hs_phy_status, USB_HS_PHY_SUCCESS, base) }

/*******************************************************************/
#define USB_HS_PHY_stack_error(base) { ERROR_check_stack(usb_hs_phy_status, USB_HS_PHY_SUCCESS, base) }

/*******************************************************************/
#define USB_HS_PHY_stack_exit_error(base, code) { ERROR_check_stack_exit(usb_hs_phy_status, USB_HS_PHY_SUCCESS, base, code) }

#endif /* __USB_HS_PHY_H__ */
