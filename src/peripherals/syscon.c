/*
 * syscon.c
 *
 *  Created on: 11 mar 2023.
 *      Author: Ludo
 */

#include "syscon.h"

#include "anactrl_reg.h"
#include "flash.h"
#include "gpio.h"
#include "mapping.h"
#include "pmc_reg.h"
#include "syscon_reg.h"

/*** SYSCON local macros ***/

#define SYSCON_TIMEOUT_COUNT		1000000

#define SYSCON_SELP_MAX_VALUE		31

#define SYSCON_SELI_MAX_VALUE		63
#define SYSCON_SELI_M_THRESHOLD_1	8000
#define SYSCON_SELI_M_THRESHOLD_2	122

/*** SYSCON local functions ***/

/* ENABLE EXTERNAL 16MHZ CRYSTAL AND INTERNAL OSCILLATOR.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_enable_xo(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	uint32_t loop_count = 0;
	// Power external crystal.
	PMC -> PDRUNCFGCLR0 = (0b1 << 8) | (0b1 << 20);
	SYSCON -> CLOCK_CTRL |= (0b1 << 5);
	ANACTRL -> XO32M_CTRL |= (0b11 << 23);
	// Wait for crystal to be stable.
	while (((ANACTRL -> XO32M_STATUS) & (0b1 << 0)) == 0) {

	}
	// Return.
	return status;
}

/* SWTICH TO FRO 12MHZ INTERNAL OSCILLATOR.
 * @param:	None.
 * @return:	None.
 */
void _SYSCON_switch_to_fro_12mhz(void) {
	// Power FRO 12MHz.
	PMC -> PDRUNCFGCLR0 = (0b1 << 5);
	// Enable 12MHz clock.
	ANACTRL -> FRO192M_CTRL |= (0b1 << 14);
	// Switch to FRO.
	SYSCON -> MAINCLKSELA = 0x00;
	SYSCON -> MAINCLKSELB = 0x00;
}

/* CONFIGURE AND SWTICH TO PLL1 CLOCK.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_switch_to_pll1(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	uint32_t m = 0;
	uint32_t n = 0;
	uint32_t p = 0;
	uint32_t selp = 0;
	uint32_t seli = 0;
	uint32_t loop_count = 0;
	// Compute PLL coefficients.
	// Source = 16MHz crystal x (25 / (2 * 2)) = 100MHz.
	m = 25;
	n = 0;
	p = 2;
	// Compute bandwidth values.
	selp = ((m / 4) + 1);
	if (selp > SYSCON_SELP_MAX_VALUE) {
		selp = SYSCON_SELP_MAX_VALUE;
	}
	if (m >= SYSCON_SELI_M_THRESHOLD_1) {
		seli = 1;
	}
	else {
		if (m >= SYSCON_SELI_M_THRESHOLD_2) {
			seli = (SYSCON_SELI_M_THRESHOLD_1 / m);
		}
		else {
			seli = (2 * (m / 4)) + 3;
		}
	}
	if (seli > SYSCON_SELI_MAX_VALUE) {
		seli = SYSCON_SELI_MAX_VALUE;
	}
	// Power down PLL1 during configuration.
	PMC -> PDRUNCFGSET0 = (0b1 << 10);
	// Configure PLL1.
	SYSCON -> PLL1CLKSEL = 0x01; // External crystal source.
	SYSCON -> PLL1CTRL = 0;
	SYSCON -> PLL1CTRL |= (seli << 4) | (selp << 10) | (0b1 << 19) | (0b1 << 21); // Bypass pre-divider and enable output.
	SYSCON -> PLL1MDEC = m;
	SYSCON -> PLL1MDEC |= (0b1 << 16);
	SYSCON -> PLL1NDEC = n;
	SYSCON -> PLL1NDEC |= (0b1 << 8);
	SYSCON -> PLL1PDEC = p;
	SYSCON -> PLL1PDEC |= (0b1 << 5);
	// Turn PLL1 on.
	PMC -> PDRUNCFGCLR0 = (0b1 << 10);
	// Wait for PLL0 to be locked.
	while (((SYSCON -> PLL1STAT) & (0b1 << 0)) == 0) {
		loop_count++;
		if (loop_count > SYSCON_TIMEOUT_COUNT) {
			status = SYSCON_ERROR_PLL1_LOCK_TIMEOUT;
			goto errors;
		}
	}
	// Switch main clock to PLL1.
	SYSCON -> MAINCLKSELA = 0x00;
	SYSCON -> MAINCLKSELB = 0x02;
errors:
	return status;
}

/*** SYSCON functions ***/

/* INIT MCU CLOCK TREE.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t SYSCON_init_clock(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	FLASH_status_t flash_status = FLASH_SUCCESS;
	// Enable analog control block.
	SYSCON -> PRESETCTRLCLR[2] = (0b1 << 27);
	SYSCON -> AHBCLKCTRLSET[2] = (0b1 << 27);
	// Switch on internal clock by default.
	_SYSCON_switch_to_fro_12mhz();
	// Output (main clock / 100) on CLKOUT pin.
	SYSCON -> CLKOUTDIV = 0x64;
	SYSCON -> CLKOUTSEL = 0x00;
	GPIO_configure(&GPIO_CLKOUT, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_FAST, GPIO_PULL_NONE);
	// Enable high speed crystal.
	status = _SYSCON_enable_xo();
	if (status != SYSCON_SUCCESS) goto errors;
	// Set latency.
	flash_status = FLASH_set_latency(8);
	FLASH_status_check(SYSCON_ERROR_BASE_FLASH);
	// Switch to PLL1.
	status = _SYSCON_switch_to_pll1();
errors:
	return status;
}
