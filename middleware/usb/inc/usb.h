/*
 * usb.h
 *
 *  Created on: 01 may 2023
 *      Author: Ludo
 */

#ifndef __USB_H__
#define __USB_H__

#include "device/usbd.h"
#include "device/standard/usbd_control.h"
#include "device/class/usbd_cdc.h"
#include "error.h"
#include "strings.h"
#include "types.h"

/*** USB structures ***/

/*!******************************************************************
 * \enum USB_status_t
 * \brief USB driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USB_SUCCESS = 0,
    // Low level drivers errors.
    USB_ERROR_BASE_STRING = ERROR_BASE_STEP,
    USB_ERROR_BASE_USBD = (USB_ERROR_BASE_STRING + STRING_ERROR_BASE_LAST),
    USB_ERROR_BASE_USBD_CONTROL = (USB_ERROR_BASE_USBD + USBD_ERROR_BASE_LAST),
    USB_ERROR_BASE_USBD_CDC = (USB_ERROR_BASE_USBD_CONTROL + USBD_CONTROL_ERROR_BASE_LAST),
    // Last base value.
    USB_ERROR_LAST = (USB_ERROR_BASE_USBD_CDC + USBD_CDC_ERROR_BASE_LAST)
} USB_status_t;

/*** USB functions ***/

/*!******************************************************************
 * \fn USB_status_t USB_init(void)
 * \brief Init USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USB_init(void);

/*!******************************************************************
 * \fn USB_status_t USB_de_init(void)
 * \brief Release USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USB_de_init(void);

/*!******************************************************************
 * \fn USB_status_t USB_start(void)
 * \brief Start USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USB_start(void);

/*!******************************************************************
 * \fn USB_status_t USB_stop(void)
 * \brief Stop USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USB_stop(void);

/*******************************************************************/
#define USB_exit_error(base) { ERROR_check_exit(usb_status, USB_SUCCESS, base) }

/*******************************************************************/
#define USB_stack_error(base) { ERROR_check_stack(usb_status, USB_SUCCESS, base) }

/*******************************************************************/
#define USB_stack_exit_error(base, code) { ERROR_check_stack_exit(usb_status, USB_SUCCESS, base, code) }

#endif /* __USB_H__ */
