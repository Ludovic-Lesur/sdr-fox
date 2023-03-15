/*
 * error.h
 *
 *  Created on: 25 oct. 2022
 *      Author: Ludo
 */

#ifndef __ERROR_H__
#define __ERROR_H__

// Peripherals.
#include "flash.h"
#include "syscon.h"
#include "systick.h"
#include "usb_hs_phy.h"
// Components.
// Utils.
#include "types.h"

/*** ERROR structures ***/

typedef enum {
	SUCCESS = 0,
	// Peripherals.
	ERROR_BASE_FLASH = 0x100,
	ERROR_BASE_SYSCON = (ERROR_BASE_FLASH + FLASH_ERROR_BASE_LAST),
	ERROR_BASE_SYSTICK = (ERROR_BASE_SYSCON + SYSCON_ERROR_BASE_LAST),
	ERROR_BASE_USB_HS_PHY = (ERROR_BASE_SYSTICK + SYSTICK_ERROR_BASE_LAST),
	// Components.
	// Utils.
	// Last index.
	ERROR_BASE_LAST = (ERROR_BASE_USB_HS_PHY + USB_HS_PHY_ERROR_BASE_LAST)
} ERROR_t;

/*** ERROR functions ***/

void ERROR_stack_init(void);
void ERROR_stack_add(ERROR_t code);
ERROR_t ERROR_stack_read(void);
uint8_t ERROR_stack_is_empty(void);

#define ERROR_status_check(status, success, error_base) { \
	if (status != success) { \
		ERROR_stack_add(error_base + status); \
	} \
}

#define ERROR_status_check_print(status, success, error_base) { \
	if (status != success) { \
		_AT_print_error(error_base + status); \
		goto errors; \
	} \
}

#endif /* __ERROR_H__ */
