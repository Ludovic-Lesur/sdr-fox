/*
 * usb_types.h
 *
 *  Created on: 30 apr. 2023
 *      Author: Ludo
 */

#ifndef __USB_TYPES_H__
#define __USB_TYPES_H__

#include "types.h"

/*** USB macros ***/

#define USB_SETUP_PACKET_SIZE_BYTES     8

/*** USB structures ***/

/*!******************************************************************
 * \struct USB_data_t
 * \brief USB data structure.
 *******************************************************************/
typedef struct {
    uint8_t* data;
    uint32_t data_size_bytes;
} USB_data_t;

#endif /* __USB_TYPES_H__ */
