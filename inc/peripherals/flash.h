/*
 * flash.h
 *
 *  Created on: 12 mar. 2023
 *      Author: Ludo
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include "types.h"

/*** FLASH structures ***/

typedef enum {
	FLASH_SUCCESS = 0,
	FLASH_ERROR_LATENCY,
	FLASH_ERROR_TIMEOUT,
	FLASH_ERROR_BASE_LAST = 0x0100
} FLASH_status_t;

/*** FLASH functions ***/

FLASH_status_t FLASH_set_latency(uint8_t wait_states);

#define FLASH_status_check(error_base) { if (flash_status != FLASH_SUCCESS) { status = error_base + flash_status; goto errors; }}
#define FLASH_error_check() { ERROR_status_check(flash_status, FLASH_SUCCESS, ERROR_BASE_FLASH); }
#define FLASH_error_check_print() { ERROR_status_check_print(flash_status, FLASH_SUCCESS, ERROR_BASE_FLASH); }

#endif /* __FLASH_H__ */
