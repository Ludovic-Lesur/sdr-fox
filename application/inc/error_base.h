/*
 * error_base.h
 *
 *  Created on: 25 oct. 2022
 *      Author: Ludo
 */

#ifndef __ERROR_BASE_H__
#define __ERROR_BASE_H__

#include "types.h"

/*** ERROR structures ***/

typedef enum {
    SUCCESS = 0x00000000,
    // Peripherals.
    ERROR_BASE_ANACTRL = 0x00010000,
    ERROR_BASE_FLASH = 0x00020000,
    ERROR_BASE_PMC = 0x00030000,
    ERROR_BASE_SYSCON = 0x00040000,
    ERROR_BASE_SYSTICK = 0x00050000,
    ERROR_BASE_USB_HS_PHY = 0x00060000,
    ERROR_BASE_USB_HS_DEVICE = 0x00070000,
    // Utils.
    ERROR_BASE_TERMINAL = 0x10000000,
    // Middleware.
    ERROR_BASE_CLI = 0x20000000,
    ERROR_BASE_USB = 0x20010000,
    ERROR_BASE_USB_DEVICE_SDR_FOX = 0x20020000
} ERROR_base_t;

#endif /* __ERROR_BASE_H__ */
