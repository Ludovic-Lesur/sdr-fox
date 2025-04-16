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
#include "version.h"

/*** USB DEVICE DESCRIPTOR local macros ***/

#define USB_DESCRIPTOR_ID_VENDOR    0x2CC1
#define USB_DESCRIPTOR_ID_PRODUCT   0x0000

/*** USB DEVICE DESCRIPTOR local global variables ***/

static const char_t USB_DESCRIPTOR_LANGUAGE_ID[] = { 0x09, 0x04 };
static const char_t USB_DESCRIPTOR_MANUFACTURER[] = "Ludovic Lesur";
static const char_t USB_DESCRIPTOR_PRODUCT[] = "SDR-FOX";
static const char_t USB_DESCRIPTOR_SERIAL_NUMBER[] = "0";
static const char_t USB_DESCRIPTOR_CONFIGURATION[] = "SDR platfom";
static const char_t USB_DESCRIPTOR_INTERFACE_CONTROL[] = "USB control interface";

/*** USB DEVICE DESCRIPTOR global variables ***/

const USB_device_descriptor_t USB_DEVICE_DESCRIPTOR = {
    .bLength = sizeof(USB_device_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB = USB_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_USE_INTERFACE,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = USB_HS_CONTROL_PACKET_SIZE_MAX,
    .idVendor = USB_DESCRIPTOR_ID_VENDOR,
    .idProduct = USB_DESCRIPTOR_ID_PRODUCT,
    .bcdDevice = ((GIT_MAJOR_VERSION << 16) + GIT_MINOR_VERSION),
    .iManufacturer = USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER,
    .iProduct = USB_STRING_DESCRIPTOR_INDEX_PRODUCT,
    .iSerialNumber = USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUMBER,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST
};

const USB_device_qualifier_descriptor_t USB_DEVICE_QUALIFIER_DESCRIPTOR = {
    .bLength = sizeof(USB_device_qualifier_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    .bcdUSB = USB_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_VENDOR_SPECIFIC,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = USB_DESCRIPTOR_PROTOCOL_CUSTOM,
    .bMaxPacketSize0 = USB_HS_CONTROL_PACKET_SIZE_MAX,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST,
    .bReserved = 0
};

const USB_configuration_descriptor_t USB_CONFIGURATION_DESCRIPTOR = {
    .bLength = sizeof(USB_configuration_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CONFIGURATION,
    .wTotalLength = 0, // Dynamically computed by the USB initialization function.
    .bNumInterfaces = (USB_INTERFACE_INDEX_LAST - 1), // Control interface not taken into account.
    .bConfigurationValue = USB_CONFIGURATION_INDEX_SDR_FOX,
    .iConfiguration = USB_STRING_DESCRIPTOR_INDEX_CONFIGURATION,
    .bmAttributes.reserved_4_0 = 0,
    .bmAttributes.self_powered = 0,
    .bmAttributes.remote_wakeup = 0,
    .bmAttributes.reserved_7 = 1,
    .bMaxPower = (uint8_t) (USB_CONFIGURATION_MAX_POWER_MA >> 1)
};

const char_t* const USB_STRING_DESCRIPTOR[USB_STRING_DESCRIPTOR_INDEX_LAST] = {
    USB_DESCRIPTOR_LANGUAGE_ID,
    USB_DESCRIPTOR_MANUFACTURER,
    USB_DESCRIPTOR_PRODUCT,
    USB_DESCRIPTOR_SERIAL_NUMBER,
    USB_DESCRIPTOR_CONFIGURATION,
    USB_DESCRIPTOR_INTERFACE_CONTROL
};
