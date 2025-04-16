/*
 * usb_device_configuration.h
 *
 *  Created on: 23 mar. 2023
 *      Author: Ludo
 */

#ifndef __USB_DEVICE_CONFIGURATION_H__
#define __USB_DEVICE_CONFIGURATION_H__

#include "types.h"
#include "types/usb_configuration.h"
#include "types/usb_interface.h"
#include "types/usb_types.h"

/*** USB CONFIGURATION macros ***/

#define USB_CONFIGURATION_MAX_POWER_MA  500

/*** USB CONFIGURATION structures ***/

/*!******************************************************************
 * \enum USB_configuration_index_t
 * \brief USB device configurations index.
 *******************************************************************/
typedef enum {
    USB_CONFIGURATION_INDEX_SDR_FOX = 0,
    USB_CONFIGURATION_INDEX_LAST
} USB_configuration_index_t;

/*!******************************************************************
 * \enum USB_interface_index_t
 * \brief USB device interfaces index.
 *******************************************************************/
typedef enum {
    USB_INTERFACE_INDEX_CONTROL = 0,
    USB_INTERFACE_INDEX_CDC_COM,
    USB_INTERFACE_INDEX_CDC_DATA,
    USB_INTERFACE_INDEX_LAST
} USB_interface_index_t;

/*** USB CONFIGURATION global variables ***/

extern const USB_interface_t* const USB_CONFIGURATION_SDR_FOX_INTERFACE_LIST[USB_INTERFACE_INDEX_LAST];

extern const USB_configuration_t USB_CONFIGURATION;

#endif /* __USB_DEVICE_CONFIGURATION_H__ */
