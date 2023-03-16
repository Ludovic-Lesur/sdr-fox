/*
 * flash.c
 *
 *  Created on: 12 mar. 2023
 *      Author: Ludo
 */

#include "flash.h"

#include "flash_reg.h"
#include "types.h"

/*** FLASH local macros ***/

#define FLASH_WAIT_STATES_MAX	15
#define FLASH_TIMEOUT_COUNT		100000

/*** FLASH functions ***/

/* SET FLASH LATENCY.
 * @param wait_states:	Number of wait states.
 * @return status:		Function execution status.
 */
FLASH_status_t FLASH_set_latency(uint8_t wait_states) {
	// Local variables.
	FLASH_status_t status = FLASH_SUCCESS;
	uint32_t loop_count = 0;
	// Check parameter.
	if (wait_states > FLASH_WAIT_STATES_MAX) {
		status = FLASH_ERROR_LATENCY;
		goto errors;
	}
	// Clear flags.
	FLASH -> INT_CLR_STATUS = 0x1F;
	// Configure number of wait states.
	FLASH -> DATAW[0] &= ~(0b1111 << 0); // Reset bits 0-3.
	FLASH -> DATAW[0] |= ((wait_states & 0b1111) << 0); // Set latency.
	// Write command.
	FLASH -> CMD = 0x02;
	// Wait until configuration is done.
	while (((FLASH -> INT_STATUS) & (0b1 << 2)) == 0) {
		loop_count++;
		if (loop_count > FLASH_TIMEOUT_COUNT) {
			status = FLASH_ERROR_TIMEOUT;
			goto errors;
		}
	}
errors:
	return status;
}
