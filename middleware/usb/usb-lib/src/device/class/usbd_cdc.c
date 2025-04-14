/*
 * usb_cdc.c
 *
 *  Created on: 04 may 2023
 *      Author: Ludo
 */

#include "device/class/usbd_cdc.h"

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "device/usbd_hw.h"
#include "types.h"
#include "types/usb_device.h"
#include "types/usb_endpoint.h"
#include "types/usb_interface.h"
#include "types/usb_types.h"

/*** USB CDC local functions declaration ***/

static void _USBD_CDC_endpoint_callback_out(void);
static void _USBD_CDC_endpoint_callback_in(void);

/*** USBD CONTROL local structures ***/

/*******************************************************************/
typedef enum {
    USBD_CDC_ENDPOINT_INDEX_OUT = 0,
    USBD_CDC_ENDPOINT_INDEX_IN,
    USBD_CDC_ENDPOINT_INDEX_LAST
} USBD_CDC_endpoint_index_t;

/*** USB CDC local global variables ***/

static const USB_physical_endpoint_t USBD_CDC_PHY_EP_OUT = {
    .number = USBD_CDC_ENDPOINT_OUT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_OUT,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_BULK,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = 1024,
    .callback = &_USBD_CDC_endpoint_callback_out
};

static const USB_physical_endpoint_t USBD_CDC_PHY_EP_IN = {
    .number = USBD_CDC_ENDPOINT_IN_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_BULK,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = 1024,
    .callback = &_USBD_CDC_endpoint_callback_in
};

static const USB_endpoint_descriptor_t USBD_CDC_PHY_EP_OUT_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CDC_PHY_EP_OUT.number,
    .bEndpointAddress.direction = USBD_CDC_PHY_EP_OUT.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CDC_PHY_EP_OUT.transfer_type,
    .bmAttributes.synchronization_type = USBD_CDC_PHY_EP_OUT.synchronization_type,
    .bmAttributes.usage_type = USBD_CDC_PHY_EP_OUT.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CDC_PHY_EP_OUT.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_descriptor_t USBD_CDC_PHY_EP_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CDC_PHY_EP_IN.number,
    .bEndpointAddress.direction = USBD_CDC_PHY_EP_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CDC_PHY_EP_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_CDC_PHY_EP_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_CDC_PHY_EP_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CDC_PHY_EP_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_t USBD_CDC_EP_OUT = {
    .physical_endpoint = &USBD_CDC_PHY_EP_OUT,
    .descriptor = &USBD_CDC_PHY_EP_OUT_DESCRIPTOR
};

static const USB_endpoint_t USBD_CDC_EP_IN = {
    .physical_endpoint = &USBD_CDC_PHY_EP_IN,
    .descriptor = &USBD_CDC_PHY_EP_IN_DESCRIPTOR
};

static const USB_endpoint_t* const USBD_CDC_INTERFACE_EP_LIST[USBD_CDC_ENDPOINT_INDEX_LAST] = {
    &USBD_CDC_EP_OUT,
    &USBD_CDC_EP_IN
};

static const USB_interface_descriptor_t USB_DESCRIPTOR_INTERFACE_CDC = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_CDC_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_CDC_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_CDC_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = USB_DESCRIPTOR_PROTOCOL_CUSTOM,
    .iInterface = USBD_CDC_INTERFACE_STRING_DESCRIPTOR_INDEX
};

/*** USB CDC global variables ***/

const USB_interface_t USBD_CDC_INTERFACE = {
    .descriptor = &USB_DESCRIPTOR_INTERFACE_CDC,
    .endpoint_list = (const USB_endpoint_t**) &USBD_CDC_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_CDC_ENDPOINT_INDEX_LAST
};

/*** USB CDC local functions ***/

/*******************************************************************/
static void _USBD_CDC_endpoint_callback_out(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_CDC_endpoint_callback_in(void) {
    // TODO
}

/*** USB CDC functions ***/

/*******************************************************************/
USBD_CDC_status_t USBD_CDC_init(void) {
    // Local variables.
    USBD_CDC_status_t status = USBD_CDC_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    // Registers endpoints.
    uint8_t idx = 0;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_CDC_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        usbd_status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        USBD_exit_error(USBD_CDC_ERROR_BASE_HW_INTERFACE);
    }
errors:
    return status;
}

/*******************************************************************/
USBD_CDC_status_t USBD_CDC_de_init(void) {
    // Local variables.
    USBD_CDC_status_t status = USBD_CDC_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    // Registers endpoints.
    uint8_t idx = 0;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_CDC_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        usbd_status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        USBD_exit_error(USBD_CDC_ERROR_BASE_HW_INTERFACE);
    }
errors:
    return status;
}
