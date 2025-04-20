/*
 * usb_device_sdr_fox.h
 *
 *  Created on: 23 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_DEVICE_SDR_FOX_H__
#define __USB_DEVICE_SDR_FOX_H__

#include "common/usb_device.h"
#include "common/usb_types.h"
#include "error.h"
#include "strings.h"
#include "types.h"

/*** USB DEVICE SDR FOX structures ***/

/*!******************************************************************
 * \enum USB_DEVICE_SDR_FOX_status_t
 * \brief Board USB driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USB_DEVICE_SDR_FOX_SUCCESS = 0,
    // Low level drivers errors.
    USB_DEVICE_SDR_FOX_ERROR_BASE_STRING = ERROR_BASE_STEP,
    USB_DEVICE_SDR_FOX_ERROR_BASE_USB = (USB_ERROR_BASE_STRING + STRING_ERROR_BASE_LAST),
    // Last base value.
    USB_DEVICE_SDR_FOX_ERROR_LAST = (USB_DEVICE_SDR_FOX_ERROR_BASE_USB + USB_ERROR_BASE_LAST)
} USB_DEVICE_SDR_FOX_status_t;

/*** USB DEVICE SDR FOX functions ***/

/*!******************************************************************
 * \fn USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_init(void)
 * \brief Init USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_init(void);

/*!******************************************************************
 * \fn USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_de_init(void)
 * \brief Release USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_de_init(void);

/*!******************************************************************
 * \fn USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_start(void)
 * \brief Start USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_start(void);

/*!******************************************************************
 * \fn USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_stop(void)
 * \brief Stop USB device.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_stop(void);

/*******************************************************************/
#define USB_DEVICE_SDR_FOX_exit_error(base) { ERROR_check_exit(usb_device_sdr_fox_status, USB_DEVICE_SDR_FOX_SUCCESS, base) }

/*******************************************************************/
#define USB_DEVICE_SDR_FOX_stack_error(base) { ERROR_check_stack(usb_device_sdr_fox_status, USB_DEVICE_SDR_FOX_SUCCESS, base) }

/*******************************************************************/
#define USB_DEVICE_SDR_FOX_stack_exit_error(base, code) { ERROR_check_stack_exit(usb_device_sdr_fox_status, USB_DEVICE_SDR_FOX_SUCCESS, base, code) }

#endif /* __USB_DEVICE_SDR_FOX_H__ */
