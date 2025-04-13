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

/*** USB DEVICE DESCRIPTOR global variables ***/

extern const USB_device_descriptor_t USB_DESCRIPTOR_DEVICE;

extern const USB_device_qualifier_descriptor_t USB_DESCRIPTOR_DEVICE_QUALIFIER;

extern const USB_configuration_descriptor_t USB_DESCRIPTOR_CONFIGURATION;

#endif /* __USB_DEVICE_DESCRIPTOR_H__ */
