/*
 * usb_configuration_sdr_fox.h
 *
 *  Created on: 20 apr. 2025
 *      Author: Ludo
 */

#ifndef __USB_CONFIGURATION_SDR_FOX_H__
#define __USB_CONFIGURATION_SDR_FOX_H__

/*** USB CONFIGURATION SDR FOX structures ***/

/*!******************************************************************
 * \enum USB_configuration_index_t
 * \brief USB device configurations index.
 *******************************************************************/
typedef enum {
    USB_CONFIGURATION_INDEX_SDR_FOX = 0,
    USB_CONFIGURATION_INDEX_LAST
} USB_configuration_index_t;

/*!******************************************************************
 * \enum USB_interface_index_t
 * \brief USB device interfaces index.
 *******************************************************************/
typedef enum {
    USB_INTERFACE_INDEX_CONTROL = 0,
    USB_INTERFACE_INDEX_CDC_COMM,
    USB_INTERFACE_INDEX_CDC_DATA,
    USB_INTERFACE_INDEX_LAST
} USB_interface_index_t;

/*!******************************************************************
 * \enum USB_string_descriptor_index_t
 * \brief USB string descriptors index.
 *******************************************************************/
typedef enum {
    USB_STRING_DESCRIPTOR_INDEX_LANGUAGE_ID = 0,
    USB_STRING_DESCRIPTOR_INDEX_MANUFACTURER,
    USB_STRING_DESCRIPTOR_INDEX_PRODUCT,
    USB_STRING_DESCRIPTOR_INDEX_SERIAL_NUMBER,
    USB_STRING_DESCRIPTOR_INDEX_CONFIGURATION,
    USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CONTROL,
    USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC_COMM,
    USB_STRING_DESCRIPTOR_INDEX_INTERFACE_CDC_DATA,
    USB_STRING_DESCRIPTOR_INDEX_LAST
} USB_string_descriptor_index_t;

#endif /* __USB_CONFIGURATION_SDR_FOX_H__ */
