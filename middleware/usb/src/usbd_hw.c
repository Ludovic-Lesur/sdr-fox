/*
 * usbd_hw.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd_hw.h"

#include "device/usbd.h"
#include "error_base.h"
#include "nvic_priority.h"
#include "types/usb_endpoint.h"
#include "types.h"
#include "usb_hs_device.h"

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
USBD_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Register end-point.
    usb_hs_device_status = USB_HS_DEVICE_register_endpoint(endpoint);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Register end-point.
    usb_hs_device_status = USB_HS_DEVICE_register_setup_callback(setup_callback);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Unregister end-point.
    usb_hs_device_status = USB_HS_DEVICE_unregister_endpoint(endpoint);
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
    // Stop high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_write(endpoint, usb_data_in);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
USBD_status_t USBD_HW_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    USB_HS_DEVICE_status_t usb_hs_device_status = USB_HS_DEVICE_SUCCESS;
    // Stop high-speed device controller.
    usb_hs_device_status = USB_HS_DEVICE_read(endpoint, usb_data_out);
    USB_HS_DEVICE_exit_error(USBD_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}
