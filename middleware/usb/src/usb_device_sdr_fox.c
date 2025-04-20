/*
 * usb_device_configuration.c
 *
 *  Created on: 03 nov. 2024
 *      Author: Ludo
 */

#include "usb_device_sdr_fox.h"

#include "common/usb_configuration.h"
#include "common/usb_device.h"
#include "common/usb_descriptor.h"
#include "common/usb_interface.h"
#include "device/class/usbd_cdc.h"
#include "device/standard/usbd_control.h"
#include "device/usbd.h"
#include "version.h"

/*** USB DEVICE CONFIGURATION local macros ***/

#define USB_DEVICE_SDR_FOX_ID_VENDOR        0x2CC1
#define USB_DEVICE_SDR_FOX_ID_PRODUCT       0x0000

#define USB_DEVICE_SDR_FOX_MAX_POWER_MA     500

/*** USB local functions declaration ***/

static USB_status_t _USB_DEVICE_SDR_FOX_set_configuration(uint8_t index);

/*** USB DEVICE DESCRIPTOR local global variables ***/

static const char_t USB_DESCRIPTOR_LANGUAGE_ID[] = { 0x09, 0x04 };
static const char_t USB_DESCRIPTOR_MANUFACTURER[] = "Ludovic Lesur";
static const char_t USB_DESCRIPTOR_PRODUCT[] = "SDR-FOX";
static const char_t USB_DESCRIPTOR_SERIAL_NUMBER[] = "0";
static const char_t USB_DESCRIPTOR_CONFIGURATION[] = "SDR platfom";
static const char_t USB_DESCRIPTOR_INTERFACE_CONTROL[] = "USB control interface";
static const char_t USB_DESCRIPTOR_INTERFACE_CDC_COMM[] = "Radio control interface (COMM)";
static const char_t USB_DESCRIPTOR_INTERFACE_CDC_DATA[] = "Radio control interface (DATA)";

/*** USB DEVICE DESCRIPTOR global variables ***/

static const USB_device_descriptor_t USB_DEVICE_DESCRIPTOR = {
    .bLength = sizeof(USB_device_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB = USB_DEVICE_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_USE_INTERFACE,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = USB_HS_CONTROL_PACKET_SIZE_MAX,
    .idVendor = USB_DEVICE_SDR_FOX_ID_VENDOR,
    .idProduct = USB_DEVICE_SDR_FOX_ID_PRODUCT,
    .bcdDevice = ((GIT_MAJOR_VERSION << 16) + GIT_MINOR_VERSION),
    .iManufacturer = USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER,
    .iProduct = USB_STRING_DESCRIPTOR_INDEX_PRODUCT,
    .iSerialNumber = USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUMBER,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST
};

static const USB_device_qualifier_descriptor_t USB_DEVICE_QUALIFIER_DESCRIPTOR = {
    .bLength = sizeof(USB_device_qualifier_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    .bcdUSB = USB_DEVICE_DESCRIPTOR_USB_VERSION,
    .bDeviceClass = USB_CLASS_CODE_VENDOR_SPECIFIC,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = USB_HS_CONTROL_PACKET_SIZE_MAX,
    .bNumConfigurations = USB_CONFIGURATION_INDEX_LAST,
    .bReserved = 0
};

static const USB_configuration_descriptor_t USB_CONFIGURATION_DESCRIPTOR = {
    .bLength = sizeof(USB_configuration_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CONFIGURATION,
    .wTotalLength = 0, // Dynamically computed by the USBD control driver.
    .bNumInterfaces = (USB_INTERFACE_INDEX_LAST - 1), // Control interface not taken into account.
    .bConfigurationValue = (USB_CONFIGURATION_INDEX_SDR_FOX + 1), // Non-zero value should be returned.
    .iConfiguration = USB_STRING_DESCRIPTOR_INDEX_CONFIGURATION,
    .bmAttributes.reserved_4_0 = 0,
    .bmAttributes.self_powered = 0,
    .bmAttributes.remote_wakeup = 0,
    .bmAttributes.reserved_7 = 1,
    .bMaxPower = (uint8_t) (USB_DEVICE_SDR_FOX_MAX_POWER_MA >> 1)
};

static const char_t* const USB_STRING_DESCRIPTOR[USB_STRING_DESCRIPTOR_INDEX_LAST] = {
    USB_DESCRIPTOR_LANGUAGE_ID,
    USB_DESCRIPTOR_MANUFACTURER,
    USB_DESCRIPTOR_PRODUCT,
    USB_DESCRIPTOR_SERIAL_NUMBER,
    USB_DESCRIPTOR_CONFIGURATION,
    USB_DESCRIPTOR_INTERFACE_CONTROL,
    USB_DESCRIPTOR_INTERFACE_CDC_COMM,
    USB_DESCRIPTOR_INTERFACE_CDC_DATA
};

static const USB_interface_t* const USB_CONFIGURATION_SDR_FOX_INTERFACE_LIST[USB_INTERFACE_INDEX_LAST] = {
    &USBD_CONTROL_INTERFACE,
    &USBD_CDC_COMM_INTERFACE,
    &USBD_CDC_DATA_INTERFACE
};

static const USB_configuration_t USB_CONFIGURATION_SDR_FOX = {
    .descriptor = &USB_CONFIGURATION_DESCRIPTOR,
    .interface_list = (const USB_interface_t**) &USB_CONFIGURATION_SDR_FOX_INTERFACE_LIST,
    .number_of_interfaces = USB_INTERFACE_INDEX_LAST,
    .max_power_ma = USB_DEVICE_SDR_FOX_MAX_POWER_MA,
};


static const USB_configuration_t* USB_CONFIGURATION_LIST[USB_CONFIGURATION_INDEX_LAST] = {
    &USB_CONFIGURATION_SDR_FOX
};

static const USB_device_t USB_DEVICE_SDR_FOX = {
    .descriptor = &USB_DEVICE_DESCRIPTOR,
    .qualifier_descriptor = &USB_DEVICE_QUALIFIER_DESCRIPTOR,
    .configuration_list = (const USB_configuration_t**) &USB_CONFIGURATION_LIST,
    .number_of_configurations = USB_CONFIGURATION_INDEX_LAST,
    .string_descriptor_list = (const char_t**) &USB_STRING_DESCRIPTOR,
    .number_of_string_descriptors = USB_STRING_DESCRIPTOR_INDEX_LAST
};

static const USBD_CONTROL_callbacks_t USB_DEVICE_SDR_FOX_CONTROL_CALLBACKS = {
    .set_configuration_request = &_USB_DEVICE_SDR_FOX_set_configuration,
    .vendor_request = NULL
};

/*** USB DEVICE SDR FOX local functions ***/

/*******************************************************************/
static USB_status_t _USB_DEVICE_SDR_FOX_set_configuration(uint8_t index) {
    // Unused parameter.
    UNUSED(index);
    // Nothing to so since the device has only one configuration.
    return USB_SUCCESS;
}

/*** USB DEVICE SDR FOX functions ***/

/*******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_init(void) {
    // Local variables.
    USB_DEVICE_SDR_FOX_status_t status = USB_DEVICE_SDR_FOX_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Init USB device library.
    usb_status = USBD_init();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
    // Init control interface.
    usb_status = USBD_CONTROL_init(&USB_DEVICE_SDR_FOX, (USBD_CONTROL_callbacks_t*) &USB_DEVICE_SDR_FOX_CONTROL_CALLBACKS);
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
    // Init CDC class.
    usb_status = USBD_CDC_init();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
errors:
    return status;
}

/*******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_de_init(void) {
    // Local variables.
    USB_DEVICE_SDR_FOX_status_t status = USB_DEVICE_SDR_FOX_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Release CDC class.
    usb_status = USBD_CDC_de_init();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
    // Release control interface.
    usb_status = USBD_CONTROL_de_init();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
    // Release USB device library.
    usb_status = USBD_de_init();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
errors:
    return status;
}

/*******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_start(void) {
    // Local variables.
    USB_DEVICE_SDR_FOX_status_t status = USB_DEVICE_SDR_FOX_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Start device controller.
    usb_status = USBD_start();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
errors:
    return status;
}

/*******************************************************************/
USB_DEVICE_SDR_FOX_status_t USB_DEVICE_SDR_FOX_stop(void) {
    // Local variables.
    USB_DEVICE_SDR_FOX_status_t status = USB_DEVICE_SDR_FOX_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Start device controller.
    usb_status = USBD_stop();
    USB_exit_error(USB_DEVICE_SDR_FOX_ERROR_BASE_USB);
errors:
    return status;
}
