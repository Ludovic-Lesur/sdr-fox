/*
 * usb_lib_flags.h
 *
 *  Created on: 16 apr. 2025
 *      Author: Ludo
 */

#ifndef __USB_LIB_FLAGS_H__
#define __USB_LIB_FLAGS_H__

#include "usb_device_sdr_fox.h"
#include "usb_hs_device.h"

/*** USB library compilation flags ***/

#define USB_LIB_HW_INTERFACE_ERROR_BASE_LAST                USB_HS_DEVICE_ERROR_BASE_LAST

#define USBD_CONTROL_INTERFACE_INDEX                        USB_INTERFACE_INDEX_CONTROL
#define USBD_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX      USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CONTROL

#define USBD_CDC

#ifdef USBD_CDC
#define USBD_CDC_COM_INTERFACE_INDEX                        USB_INTERFACE_INDEX_CDC_COM
#define USBD_CDC_COM_INTERFACE_STRING_DESCRIPTOR_INDEX      USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC_DATA
#define USBD_CDC_COM_ENDPOINT_NUMBER                        1
#define USBD_CDC_COM_PACKET_SIZE_BYTES                      16

#define USBD_CDC_DATA_INTERFACE_INDEX                       USB_INTERFACE_INDEX_CDC_DATA
#define USBD_CDC_DATA_INTERFACE_STRING_DESCRIPTOR_INDEX     USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC_DATA
#define USBD_CDC_DATA_ENDPOINT_NUMBER                       2
#define USBD_CDC_DATA_PACKET_SIZE_BYTES                     1024
#endif

#endif /* __USB_LIB_FLAGS_H__ */
