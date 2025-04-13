/*
 * usb_device_descriptor.c
 *
 *  Created on: 22 apr. 2023
 *      Author: Ludo
 */

#include "usb_device_descriptor.h"

#include "types/usb_configuration.h"
#include "types/usb_device.h"
#include "types/usb_types.h"
#include "usb_device_configuration.h"

/*** USB DEVICE DESCRIPTOR local macros ***/

#define USB_DESCRIPTOR_ID_VENDOR    0x2CC1
#define USB_DESCRIPTOR_ID_PRODUCT   0x0000

/*** USB DEVICE DESCRIPTOR global variables ***/

const USB_device_descriptor_t USB_DESCRIPTOR_DEVICE = {
    .bLength = sizeof(USB_device_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB = USB_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_VENDOR_SPECIFIC,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = USB_DESCRIPTOR_PROTOCOL_CUSTOM,
    .bMaxPacketSize0 = 64,
    .idVendor = USB_DESCRIPTOR_ID_VENDOR,
    .idProduct = USB_DESCRIPTOR_ID_PRODUCT,
    .bcdDevice = 0x0000,
    .iManufacturer = USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER,
    .iProduct = USB_STRING_DESCRIPTOR_INDEX_PRODUCT,
    .iSerialNumber = USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUMBER,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST
};

const USB_device_qualifier_descriptor_t USB_DESCRIPTOR_DEVICE_QUALIFIER = {
    .bLength = sizeof(USB_device_qualifier_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    .bcdUSB = USB_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_VENDOR_SPECIFIC,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = USB_DESCRIPTOR_PROTOCOL_CUSTOM,
    .bMaxPacketSize0 = 64,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST,
    .bReserved = 0
};

const USB_configuration_descriptor_t USB_DESCRIPTOR_CONFIGURATION = {
    .bLength = sizeof(USB_configuration_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CONFIGURATION,
    .wTotalLength = sizeof(USB_configuration_descriptor_t) + (USB_INTERFACE_INDEX_LAST - 1) * sizeof(USB_interface_descriptor_t) + (2 * sizeof(USB_endpoint_descriptor_t)),
    .bNumInterfaces = (USB_INTERFACE_INDEX_LAST - 1), // Control interface not taken into account.
    .bConfigurationValue = USB_CONFIGURATION_INDEX_SDR_FOX,
    .iConfiguration = USB_STRING_DESCRIPTOR_INDEX_CONFIGURATION,
    .bmAttributes.reserved_4_0 = 0,
    .bmAttributes.self_powered = 0,
    .bmAttributes.remote_wakeup = 0,
    .bmAttributes.reserved_7 = 0,
    .bMaxPower = (uint8_t) (USB_CONFIGURATION_MAX_POWER_MA >> 1)
};
