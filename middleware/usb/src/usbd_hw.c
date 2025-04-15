/*
 * usbd_hw.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd_hw.h"

#include "device/usbd.h"
#include "error.h"
#include "error_base.h"
#include "nvic_priority.h"
#include "types/usb_endpoint.h"
#include "types.h"
#include "usb_hs_device.h"

/*** USBD HW local functions ***/

/*******************************************************************/
#define _USBD_HW_create_endpoint(void) { \
    usb_hs_device_endpoint.number = (endpoint->number); \
    usb_hs_device_endpoint.direction = (USB_HS_DEVICE_endpoint_direction_t) (endpoint->direction); \
    usb_hs_device_endpoint.transfer_type = (USB_HS_DEVICE_endpoint_transfer_type_t) (endpoint->transfer_type); \
    usb_hs_device_endpoint.max_packet_size_bytes = (endpoint->max_packet_size_bytes); \
    usb_hs_device_endpoint.callback = (USB_HS_DEVICE_endpoint_cb_t) (endpoint->callback); \
}

/*** USBD HW functions ***/

/*******************************************************************/
USBD_status_t USBD_HW_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Init high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_init(NVIC_PRIORITY_USB_HS_DEVICE);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_de_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Release high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_de_init();
    USB_HS_DEVICE_stack_error(ERROR_BASE_USBD + USBD_ERROR_BASE_HW_INTERFACE);
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Register end-point.
    usb_hs_device_status = USB_HS_DEVICE_register_setup_callback((USB_HS_DEVICE_setup_cb_t) setup_callback);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_t usb_hs_device_endpoint;
    // Create endpoint structure.
    _USBD_HW_create_endpoint();
    // Register end-point.
    usb_hs_device_status = USB_HS_DEVICE_register_endpoint(&usb_hs_device_endpoint);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_t usb_hs_device_endpoint;
    // Create endpoint structure.
    _USBD_HW_create_endpoint();
    // Unregister end-point.
    usb_hs_device_status = USB_HS_DEVICE_unregister_endpoint(&usb_hs_device_endpoint);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_set_address(uint8_t device_address) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Unregister end-point.
    usb_hs_device_status = USB_HS_DEVICE_set_address(device_address);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_start(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Start high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_start();
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_stop(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Stop high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_stop();
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_write(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_t usb_hs_device_endpoint;
    // Create endpoint structure.
    _USBD_HW_create_endpoint();
    // Stop high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_write(&usb_hs_device_endpoint, (usb_data_in->data), (usb_data_in->size_bytes));
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_t usb_hs_device_endpoint;
    // Create endpoint structure.
    _USBD_HW_create_endpoint();
    // Stop high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_read(&usb_hs_device_endpoint, &(usb_data_out->data), &(usb_data_out->size_bytes));
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}
