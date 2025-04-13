/*
 * usbd_hw.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd_hw.h"

#include "device/usbd.h"
#include "types/usb_endpoint.h"
#include "types.h"

/*** USBD HW functions ***/

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_de_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}
