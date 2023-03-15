/*
 * pmc.c
 *
 *  Created on: Mar 15, 2023
 *      Author: ludo
 */

#include "pmc.h"

#include "pmc_reg.h"
#include "types.h"

/*** PMC local macros ***/

static const uint8_t PMC_PDEN_BIT_INDEX[PMC_BLOCK_LAST] = {3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 18, 19, 20, 22, 23};

/*** PMC functions ***/

/* TURN BLOCK ON.
 * @param block:	Bloc to power up.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_power_on(PMC_block_t block) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check parameter.
	if (block >= PMC_BLOCK_LAST) {
		status = PMC_ERROR_BLOCK;
		goto errors;
	}
	// Clear power down bit.
	PMC -> PDRUNCFGCLR0 = (0b1 << PMC_PDEN_BIT_INDEX[block]);
errors:
	return status;
}

/* TURN BLOCK OFF.
 * @param block:	Bloc to power down.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_power_off(PMC_block_t block) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check parameter.
	if (block >= PMC_BLOCK_LAST) {
		status = PMC_ERROR_BLOCK;
		goto errors;
	}
	// Set power down bit.
	PMC -> PDRUNCFGSET0 = (0b1 << PMC_PDEN_BIT_INDEX[block]);
errors:
	return status;
}

/* SELECT 32K-OSC CLOCK SOURCE.
 * @param source:	Clock source.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_set_32k_osc_source(PMC_32k_osc_source_t source) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check source.
	switch (source) {
	case PMC_32K_OSC_SOURCE_FRO32K:
		PMC -> RTCOSC32K &= ~(0b1 << 0);
		break;
	case PMC_32K_OSC_SOURCE_XTAL32K:
		PMC -> RTCOSC32K |= (0b1 << 0);
		break;
	default:
		status = PMC_ERROR_32K_OSC_SOURCE;
		goto errors;
	}
errors:
	return status;
}

/* READ XTAL32K STATUS.
 * @param:	None.
 * @return:	'1' if the XTAL32K is running, '0' otherwise.
 */
uint8_t PMC_xtal32k_is_ready(void) {
	// Read XTAL32KOK bit.
	return ((PMC -> STATUSCLK) & (0b1 << 0));
}
