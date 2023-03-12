/*
 * syscon.h
 *
 *  Created on: 11 mar 2023
 *      Author: Ludo
 */

#ifndef __SYSCON_H__
#define __SYSCON_H__

#include "flash.h"

/*** SYSCON structures ***/

typedef enum {
	SYSCON_SUCCESS = 0,
	SYSCON_ERROR_PLL1_LOCK_TIMEOUT,
	SYSCON_ERROR_BASE_FLASH = 0x100,
	SYSCON_ERROR_BASE_LAST = (SYSCON_ERROR_BASE_FLASH + FLASH_ERROR_BASE_LAST)
} SYSCON_status_t;

/*** SYSCON functions ***/

SYSCON_status_t SYSCON_init_clock(void);

#endif /* __SYSCON_H__ */
