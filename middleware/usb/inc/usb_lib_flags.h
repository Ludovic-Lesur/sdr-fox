/*
 * usb_lib_flags.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_LIB_FLAGS_H__
#define __USB_LIB_FLAGS_H__

#include "usb_device_configuration.h"
#include "usb_hs_device.h"

/*** USB library compilation flags ***/

#define USB_LIB_HW_INTERFACE_ERROR_BASE_LAST            USB_HS_DEVICE_ERROR_BASE_LAST

#define USBD_CONTROL_INTERFACE_INDEX                    USB_INTERFACE_INDEX_CONTROL
#define USBD_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX  USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CONTROL

#define USBD_CDC_INTERFACE_INDEX                        USB_INTERFACE_INDEX_CDC
#define USBD_CDC_INTERFACE_STRING_DESCRIPTOR_INDEX      USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC
#define USBD_CDC_ENDPOINT_OUT_NUMBER                    USB_ENDPOINT_NUMBER_EP1
#define USBD_CDC_ENDPOINT_IN_NUMBER                     USB_ENDPOINT_NUMBER_EP1

#endif /* __USB_LIB_FLAGS_H__ */
