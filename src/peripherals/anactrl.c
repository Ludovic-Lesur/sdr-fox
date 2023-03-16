/*
 * anactrl.c
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#include "anactrl.h"

#include "anactrl_reg.h"
#include "syscon.h"
#include "types.h"

/*** ANACTRL functions ***/

/* ENABLE INTERNAL HIGH SPEED OSCILLATOR.
 * @param type:		Type of oscillator to enable.
 * @return status:	Function execution satus.
 */
ANACTRL_status_t ANACTRL_enable_fro(ANACTRL_fro_type_t type) {
	// Local variables.
	ANACTRL_status_t status = ANACTRL_SUCCESS;
	// Check type.
	switch (type) {
	case ANACTRL_FRO_TYPE_12MHZ:
		ANACTRL -> FRO192M_CTRL |= (0b1 << 14);
		break;
	case ANACTRL_FRO_TYPE_96MHZ:
		ANACTRL -> FRO192M_CTRL |= (0b1 << 30);
		break;
	default:
		status = ANACTRL_ERROR_FRO_TYPE;
		goto errors;
	}
errors:
	return status;
}

/* ENABLE XO32M OUTPUT.
 * @param output:	Output to enable.
 * @return status:	Function execution status.
 */
ANACTRL_status_t ANACTRL_enable_xo32m_output(ANACTRL_xo32m_output_t output) {
	// Local variables.
	ANACTRL_status_t status = ANACTRL_SUCCESS;
	// Check type.
	switch (output) {
	case ANACTRL_XO32M_OUTPUT_SYSTEM_CLK:
		ANACTRL -> XO32M_CTRL |= (0b1 << 24);
		break;
	case ANACTRL_XO32M_OUTPUT_PLL_USB:
		ANACTRL -> XO32M_CTRL |= (0b1 << 23);
		break;
	default:
		status = ANACTRL_ERROR_XO32M_OUTPUT;
		goto errors;
	}
errors:
	return status;
}

/* READ XO32M STATUS.
 * @param:	None.
 * @return:	'1' if the XO32M is running, '0' otherwise.
 */
uint8_t ANACTRL_xo32m_is_ready(void) {
	return ((ANACTRL -> XO32M_STATUS) & (0b1 << 0));
}
