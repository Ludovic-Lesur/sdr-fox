/*
 * systick.h
 *
 *  Created on: 14 mar 2023
 *      Author: Ludo
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "types.h"

/*** SYSTICK structures ***/

typedef enum {
	SYSTICK_SUCCESS = 0,
	SYSTICK_ERROR_DELAY_OVERFLOW,
	SYSTICK_ERROR_DELAY_UNDERFLOW,
	SYSTICK_ERROR_BASE_LAST = 0x100
} SYSTICK_status_t;

/*** SYSTICK functions ***/

void SYSTICK_init(void);
SYSTICK_status_t SYSTICK_delay_milliseconds(uint32_t delay_ms);

#define SYSTICK_status_check(error_base) { if (systick_status != SYSTICK_SUCCESS) { status = error_base + systick_status; goto errors; }}
#define SYSTICK_error_check() { ERROR_status_check(systick_status, SYSTICK_SUCCESS, ERROR_BASE_SYSTICK); }
#define SYSTICK_error_check_print() { ERROR_status_check_print(systick_status, SYSTICK_SUCCESS, ERROR_BASE_SYSTICK); }

#endif /* __SYSTICK_H__ */
