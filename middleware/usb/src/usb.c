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

/*** USB local macros ***/

#define USB_FULL_CONFIGURATION_DESCRIPTOR_SIZE_BYTES                1024
#define USB_FULL_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH_FIELD_INDEX  2

/*** USB local structures ***/

/*******************************************************************/
typedef struct {
    uint8_t full_configuration_descriptor[USB_FULL_CONFIGURATION_DESCRIPTOR_SIZE_BYTES];
} USB_context_t;

/*** USB local functions declaration ***/

static USBD_CONTROL_status_t _USB_get_descriptor(USB_descriptor_type_t type, uint8_t** descriptor_ptr, uint32_t* descriptor_size_bytes);

/*** USB local global variables ***/

static const USBD_CONTROL_data_callbacks_t USBD_CONTROL_CALLBACKS = {
    .get_descriptor = &_USB_get_descriptor
};

static USB_context_t usb_ctx;

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
        (*descriptor_ptr) = (uint8_t*) &(usb_ctx.full_configuration_descriptor);
        (*descriptor_size_bytes) = usb_ctx.full_configuration_descriptor[USB_FULL_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH_FIELD_INDEX];
        break;
    case USB_DESCRIPTOR_TYPE_INTERFACE:
        (*descriptor_ptr) = (uint8_t*) &(USB_CONFIGURATION.interface_list[0]->descriptor); // TODO dynamic index from request?
        (*descriptor_size_bytes) = (USB_CONFIGURATION.interface_list[0]->descriptor)->bLength;
        break;
    case USB_DESCRIPTOR_TYPE_ENDPOINT:
        (*descriptor_ptr) = (uint8_t*) &(USB_CONFIGURATION.interface_list[0]->endpoint_list[0]->descriptor); // TODO dynamic index from request?
        (*descriptor_size_bytes) = (USB_CONFIGURATION.interface_list[0]->endpoint_list[0]->descriptor)->bLength;
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
    const USB_interface_t* interface_ptr = NULL;
    const USB_endpoint_t* endpoint_ptr = NULL;
    uint8_t* descriptor_ptr = NULL;
    uint8_t interface_idx = 0;
    uint8_t endpoint_idx = 0;
    uint32_t full_idx = 0;
    uint32_t idx = 0;
    // Build full configuration descriptor.
    descriptor_ptr = (uint8_t*) &USB_DESCRIPTOR_CONFIGURATION;
    for (idx = 0; idx < USB_DESCRIPTOR_CONFIGURATION.bLength; idx++) {
        usb_ctx.full_configuration_descriptor[full_idx++] = descriptor_ptr[idx];
    }
    // Interfaces loop.
    for (interface_idx = (USB_INTERFACE_INDEX_CONTROL + 1); interface_idx < (USB_CONFIGURATION.number_of_interfaces); interface_idx++) {
        // Update pointer.
        interface_ptr = USB_CONFIGURATION.interface_list[interface_idx];
        descriptor_ptr = (uint8_t*) (interface_ptr->descriptor);
        // Append interface descriptor.
        for (idx = 0; idx < (interface_ptr->descriptor)->bLength; idx++) {
            usb_ctx.full_configuration_descriptor[full_idx++] = descriptor_ptr[idx];
        }
        // Endpoints loop.
        for (endpoint_idx = 0; endpoint_idx < (interface_ptr->number_of_endpoints); endpoint_idx++) {
            // Update pointer.
            endpoint_ptr = interface_ptr->endpoint_list[endpoint_idx];
            descriptor_ptr = (uint8_t*) (endpoint_ptr->descriptor);
            // Append endpoint descriptor.
            for (idx = 0; idx < (endpoint_ptr->descriptor)->bLength; idx++) {
                usb_ctx.full_configuration_descriptor[full_idx++] = descriptor_ptr[idx];
            }
        }
    }
    // Update total length field.
    usb_ctx.full_configuration_descriptor[USB_FULL_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH_FIELD_INDEX] = full_idx;
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
