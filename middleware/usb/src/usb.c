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
#include "strings.h"
#include "types.h"
#include "usb_device_sdr_fox.h"

#ifndef __USB_C__
#define __USB_C__

/*** USB local macros ***/

#define USB_FULL_CONFIGURATION_DESCRIPTOR_SIZE_BYTES                1024
#define USB_FULL_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH_FIELD_INDEX  2

#define USB_STRING_DESCRIPTOR_SIZE_BYTES                            256

/*** USB local functions declaration ***/

static USBD_CONTROL_status_t _USB_set_configuration(uint8_t index);

/*** USB local global variables ***/

static const USBD_CONTROL_requests_callbacks_t USBD_CONTROL_REQUESTS_CALLBACKS = {
    .set_configuration = &_USB_set_configuration
};

/*** USB local functions ***/

/*******************************************************************/
static USBD_CONTROL_status_t _USB_set_configuration(uint8_t index) {
    // Unused parameter.
    UNUSED(index);
    // Nothing to so since the device has only one configuration.
    return USBD_CONTROL_SUCCESS;
}

/*** USB functions ***/

/*******************************************************************/
USB_status_t USB_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    USBD_CONTROL_status_t usbd_control_status = USBD_CONTROL_SUCCESS;
    USBD_CDC_status_t usbd_cdc_status = USBD_CDC_SUCCESS;
    // Init USB device library.
    usbd_status = USBD_init();
    USBD_exit_error(USB_ERROR_BASE_USBD);
    // Init control interface.
    usbd_control_status = USBD_CONTROL_init(&USB_DEVICE_SDR_FOX, (USBD_CONTROL_requests_callbacks_t*) &USBD_CONTROL_REQUESTS_CALLBACKS);
    USBD_CONTROL_exit_error(USB_ERROR_BASE_USBD_CONTROL);
    // Init CDC class.
    usbd_cdc_status = USBD_CDC_init();
    USBD_CDC_exit_error(USB_ERROR_BASE_USBD_CDC);
errors:
    return status;
}

/*******************************************************************/
USB_status_t USB_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    USBD_CONTROL_status_t usbd_control_status = USBD_CONTROL_SUCCESS;
    USBD_CDC_status_t usbd_cdc_status = USBD_CDC_SUCCESS;
    // Release CDC class.
    usbd_cdc_status = USBD_CDC_de_init();
    USBD_CDC_exit_error(USB_ERROR_BASE_USBD_CDC);
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
