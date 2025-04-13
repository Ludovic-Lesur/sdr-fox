/*
 * usb_configuration.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_CONFIGURATION_H__
#define __USB_CONFIGURATION_H__

#include "types.h"
#include "types/usb_interface.h"

/*** USB CONFIGURATION structures ***/

/*!******************************************************************
 * \struct USB_configuration_descriptor_attributes_t
 * \brief USB configuration descriptor attributes format.
 *******************************************************************/
typedef union {
    struct {
        unsigned reserved_4_0 :5;
        unsigned remote_wakeup :1;
        unsigned self_powered :1;
        unsigned reserved_7 :1;
    } __attribute__((scalar_storage_order("little-endian")))__attribute__((packed));
    uint8_t value;
} USB_configuration_descriptor_attributes_t;

/*!******************************************************************
 * \struct USB_configuration_descriptor_t
 * \brief USB configuration descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    USB_configuration_descriptor_attributes_t bmAttributes;
    uint8_t bMaxPower;
} USB_configuration_descriptor_t;

/*!******************************************************************
 * \struct USB_configuration_t
 * \brief USB configuration structure.
 *******************************************************************/
typedef struct {
    USB_configuration_descriptor_t* descriptor;
    uint16_t max_power_ma;
    uint8_t number_of_interfaces;
    USB_interface_t* interface_list;
} USB_configuration_t;

#endif /* __USB_CONFIGURATION_H__ */
