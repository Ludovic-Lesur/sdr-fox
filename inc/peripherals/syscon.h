/*
 * syscon.h
 *
 *  Created on: 11 mar 2023
 *      Author: Ludo
 */

#ifndef __SYSCON_H__
#define __SYSCON_H__

#include "flash.h"

/*** SYSCON macros ***/

#define SYSCON_32K_OSC_FREQUENCY_HZ		32768

/*** SYSCON structures ***/

typedef enum {
	SYSCON_SUCCESS = 0,
	SYSCON_ERROR_32K_OSC_TIMEOUT,
	SYSCON_ERROR_CLK_IN_TIMEOUT,
	SYSCON_ERROR_PLL1_LOCK_TIMEOUT,
	SYSCON_ERROR_BASE_FLASH = 0x100,
	SYSCON_ERROR_BASE_LAST = (SYSCON_ERROR_BASE_FLASH + FLASH_ERROR_BASE_LAST)
} SYSCON_status_t;

/*** SYSCON functions ***/

SYSCON_status_t SYSCON_init_clock(void);

#define SYSCON_status_check(error_base) { if (syscon_status != SYSCON_SUCCESS) { status = error_base + syscon_status; goto errors; }}
#define SYSCON_error_check() { ERROR_status_check(syscon_status, SYSCON_SUCCESS, ERROR_BASE_SYSCON); }
#define SYSCON_error_check_print() { ERROR_status_check_print(syscon_status, SYSCON_SUCCESS, ERROR_BASE_SYSCON); }

#endif /* __SYSCON_H__ */
