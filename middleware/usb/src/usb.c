/*
 * usb.c
 *
 *  Created on: 01 may 2023
 *      Author: Ludo
 */

#include "usb.h"

#include "device/class/usbd_cdc.h"
#include "device/standard/usbd_control.h"
#include "device/usbd.h"
#include "error.h"
#include "types.h"
#include "types/usb_types.h"
#include "usb_device_configuration.h"
#include "usb_device_descriptor.h"

#ifndef __USB_C__
#define __USB_C__

/*** USB local functions declaration ***/

static USBD_CONTROL_status_t _USB_get_descriptor(USB_descriptor_type_t type, uint8_t** descriptor_ptr, uint32_t* descriptor_size_bytes);

/*** USB local global variables ***/

static const USBD_CONTROL_data_callbacks_t USBD_CONTROL_CALLBACKS = {
    .get_descriptor = &_USB_get_descriptor
};

/*** USB local functions ***/

/*******************************************************************/
static USBD_CONTROL_status_t _USB_get_descriptor(USB_descriptor_type_t type, uint8_t** descriptor_ptr, uint32_t* descriptor_size_bytes) {
    // Local variables.
    USBD_CONTROL_status_t status = USBD_CONTROL_SUCCESS;
    // Reset output.
    (*descriptor_ptr) = NULL;
    (*descriptor_size_bytes) = 0;
    // Check type.
    switch (type) {
    case USB_DESCRIPTOR_TYPE_DEVICE:
        (*descriptor_ptr) = (uint8_t*) &USB_DESCRIPTOR_DEVICE;
        (*descriptor_size_bytes) = USB_DESCRIPTOR_DEVICE.bLength;
        break;
    case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
        (*descriptor_ptr) = (uint8_t*) &USB_DESCRIPTOR_DEVICE_QUALIFIER;
        (*descriptor_size_bytes) = USB_DESCRIPTOR_DEVICE_QUALIFIER.bLength;
        break;
    case USB_DESCRIPTOR_TYPE_CONFIGURATION:
    case USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
        (*descriptor_ptr) = (uint8_t*) &USB_DESCRIPTOR_CONFIGURATION;
        (*descriptor_size_bytes) = USB_DESCRIPTOR_CONFIGURATION.bLength;
        break;
    case USB_DESCRIPTOR_TYPE_INTERFACE:
        (*descriptor_ptr) = (uint8_t*) &(USB_CONFIGURATION.interface_list[0].descriptor); // TODO dynamic index from request?
        (*descriptor_size_bytes) = (USB_CONFIGURATION.interface_list[0].descriptor)->bLength;
        break;
    case USB_DESCRIPTOR_TYPE_ENDPOINT:
        (*descriptor_ptr) = (uint8_t*) &(USB_CONFIGURATION.interface_list[0].endpoint_list[0].descriptor); // TODO dynamic index from request?
        (*descriptor_size_bytes) = (USB_CONFIGURATION.interface_list[0].endpoint_list[0].descriptor)->bLength;
        break;
    default:
        status = USBD_CONTROL_ERROR_DESCRIPTOR_TYPE;
        goto errors;
    }
errors:
    return status;
}

/*** USB functions ***/

/*******************************************************************/
USB_status_t USB_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    USBD_CONTROL_status_t usbd_control_status = USBD_CONTROL_SUCCESS;
    // Init USB device library.
    usbd_status = USBD_init();
    USBD_exit_error(USB_ERROR_BASE_USBD);
    // Init control interface.
    usbd_control_status = USBD_CONTROL_init((USBD_CONTROL_data_callbacks_t*) &USBD_CONTROL_CALLBACKS);
    USBD_CONTROL_exit_error(USB_ERROR_BASE_USBD_CONTROL);
errors:
    return status;
}

/*******************************************************************/
USB_status_t USB_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    USBD_CONTROL_status_t usbd_control_status = USBD_CONTROL_SUCCESS;
    // Release control interface.
    usbd_control_status = USBD_CONTROL_de_init();
    USBD_CONTROL_exit_error(USB_ERROR_BASE_USBD_CONTROL);
    // Release USB device library.
    usbd_status = USBD_de_init();
    USBD_exit_error(USB_ERROR_BASE_USBD);
errors:
    return status;
}

/*******************************************************************/
USB_status_t USB_start(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    // Start device controller.
    usbd_status = USBD_start();
    USBD_exit_error(USB_ERROR_BASE_USBD);
errors:
    return status;
}

/*******************************************************************/
USB_status_t USB_stop(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    // Start device controller.
    usbd_status = USBD_stop();
    USBD_exit_error(USB_ERROR_BASE_USBD);
errors:
    return status;
}

#endif /* __USB_C__ */
