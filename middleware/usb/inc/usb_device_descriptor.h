/*
 * usb_device_descriptor.h
 *
 *  Created on: 22 apr. 2023
 *      Author: Ludo
 */

#ifndef __USB_DEVICE_DESCRIPTOR_H__
#define __USB_DEVICE_DESCRIPTOR_H__

#include "types/usb_configuration.h"
#include "types/usb_device.h"
#include "types.h"

/*!******************************************************************
 * \enum USB_string_descriptor_index_t
 * \brief USB string descriptors index.
 *******************************************************************/
typedef enum {
    USB_STRING_DESCRIPTOR_INDEX_LANGUAGE_ID = 0,
    USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER,
    USB_STRING_DESCRIPTOR_INDEX_PRODUCT,
    USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUMBER,
    USB_STRING_DESCRIPTOR_INDEX_CONFIGURATION,
    USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CONTROL,
    USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC,
    USB_STRING_DESCRIPTOR_INDEX_LAST
} USB_string_descriptor_index_t;

/*** USB DEVICE DESCRIPTOR global variables ***/

extern const USB_device_descriptor_t USB_DEVICE_DESCRIPTOR;

extern const USB_device_qualifier_descriptor_t USB_DEVICE_QUALIFIER_DESCRIPTOR;

extern const USB_configuration_descriptor_t USB_CONFIGURATION_DESCRIPTOR;

extern const char_t* const USB_STRING_DESCRIPTOR[USB_STRING_DESCRIPTOR_INDEX_LAST];

#endif /* __USB_DEVICE_DESCRIPTOR_H__ */
