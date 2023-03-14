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

#endif /* __SYSTICK_H__ */
