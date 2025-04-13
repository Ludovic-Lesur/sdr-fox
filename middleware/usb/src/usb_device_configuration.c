/*
 * usb_device_configuration.c
 *
 *  Created on: 03 nov. 2024
 *      Author: Ludo
 */

#include "usb_device_configuration.h"

#include "device/class/usbd_cdc.h"
#include "device/standard/usbd_control.h"

/*** USB DEVICE CONFIGURATION global variables ***/

const USB_interface_t* USB_CONFIGURATION_SDR_FOX_INTERFACE_LIST[USB_INTERFACE_INDEX_LAST] = {
    &USBD_CONTROL_INTERFACE,
    &USBD_CDC_INTERFACE
};

const USB_configuration_t USB_CONFIGURATION = {
    .max_power_ma = USB_CONFIGURATION_MAX_POWER_MA,
    .number_of_interfaces = USB_INTERFACE_INDEX_LAST,
    .interface_list = (USB_interface_t*) &USB_CONFIGURATION_SDR_FOX_INTERFACE_LIST
};
