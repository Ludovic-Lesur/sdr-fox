/*
 * pmc.h
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#ifndef __PMC_H__
#define __PMC_H__

#include "types.h"

/*** PMC structures ***/

typedef enum {
	PMC_SUCCESS = 0,
	PMC_ERROR_BLOCK,
	PMC_ERROR_32K_OSC_SOURCE,
	PMC_ERROR_BASE_LAST = 0x0100
} PMC_status_t;

typedef enum {
	PMC_BLOCK_BODVBAT = 0,
	PMC_BLOCK_FRO12M,
	PMC_BLOCK_FRO32K,
	PMC_BLOCK_XTAL32K,
	PMC_BLOCK_XTAL32M,
	PMC_BLOCK_PLL0,
	PMC_BLOCK_PLL1,
	PMC_BLOCK_USB_FS_PHY,
	PMC_BLOCK_USB_HS_PHY,
	PMC_BLOCK_COMP,
	PMC_BLOCK_LDO_USB_HS,
	PMC_BLOCK_AUX_BIAS,
	PMC_BLOCK_LDO_XO32M,
	PMC_BLOCK_RNG,
	PMC_BLOCK_PLL0_SSCG,
	PMC_BLOCK_LAST
} PMC_block_t;

typedef enum {
	PMC_32K_OSC_SOURCE_FRO32K = 0,
	PMC_32K_OSC_SOURCE_XTAL32K,
	PMC_32K_OSC_SOURCE_LAST
} PMC_32k_osc_source_t;

/*** PMC functions ***/

PMC_status_t PMC_power_on(PMC_block_t block);
PMC_status_t PMC_power_off(PMC_block_t block);

PMC_status_t PMC_set_32k_osc_source(PMC_32k_osc_source_t source);
uint8_t PMC_xtal32k_is_ready(void);

#define PMC_status_check(error_base) { if (pmc_status != PMC_SUCCESS) { status = error_base + pmc_status; goto errors; }}
#define PMC_error_check() { ERROR_status_check(pmc_status, PMC_SUCCESS, ERROR_BASE_PMC); }
#define PMC_error_check_print() { ERROR_status_check_print(pmc_status, PMC_SUCCESS, ERROR_BASE_PMC); }

#endif /* __PMC_H__ */
