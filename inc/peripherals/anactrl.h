/*
 * anactrl.h
 *
 *  Created on: 15 mar 2023
 *      Author: Ludo
 */

#ifndef __ANACTRL_H__
#define __ANACTRL_H__

#include "types.h"

/*** ANACTRL structures ***/

typedef enum {
	ANACTRL_SUCCESS = 0,
	ANACTRL_ERROR_FRO_TYPE,
	ANACTRL_ERROR_XO32M_OUTPUT,
	ANACTRL_ERROR_BASE_LAST = 0x0100
} ANACTRL_status_t;

typedef enum {
	ANACTRL_FRO_TYPE_12MHZ = 0,
	ANACTRL_FRO_TYPE_96MHZ,
	ANACTRL_FRO_TYPE_LAST
} ANACTRL_fro_type_t;

typedef enum {
	ANACTRL_XO32M_OUTPUT_SYSTEM_CLK = 0,
	ANACTRL_XO32M_OUTPUT_PLL_USB,
	ANACTRL_XO32M_OUTPUT_LAST
} ANACTRL_xo32m_output_t;

/*** ANACTRL functions ***/

ANACTRL_status_t ANACTRL_enable_fro(ANACTRL_fro_type_t type);

ANACTRL_status_t ANACTRL_enable_xo32m_output(ANACTRL_xo32m_output_t output);
uint8_t ANACTRL_xo32m_is_ready(void);

#define ANACTRL_status_check(error_base) { if (anactrl_status != ANACTRL_SUCCESS) { status = error_base + anactrl_status; goto errors; }}
#define ANACTRL_error_check() { ERROR_status_check(anactrl_status, ANACTRL_SUCCESS, ERROR_BASE_ANACTRL); }
#define ANACTRL_error_check_print() { ERROR_status_check_print(anactrl_status, ANACTRL_SUCCESS, ERROR_BASE_ANACTRL); }

#endif /* __ANACTRL_H__ */
