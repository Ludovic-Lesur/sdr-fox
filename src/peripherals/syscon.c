/*
 * syscon.c
 *
 *  Created on: 11 mar 2023.
 *      Author: Ludo
 */

#include "syscon.h"

#include "anactrl.h"
#include "flash.h"
#include "gpio.h"
#include "mapping.h"
#include "pmc.h"
#include "syscon_reg.h"

/*** SYSCON local macros ***/

#define SYSCON_TIMEOUT_COUNT		1000000

#define SYSCON_SELP_MAX_VALUE		31

#define SYSCON_SELI_MAX_VALUE		63
#define SYSCON_SELI_M_THRESHOLD_1	8000
#define SYSCON_SELI_M_THRESHOLD_2	122

/*** SYSCON local functions ***/

/* ENABLE INTERNAL OSCILLATOR.
 * @param:	None.
 * @return:	None.
 */
SYSCON_status_t _SYSCON_enable_fro(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	PMC_status_t pmc_status = PMC_SUCCESS;
	ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
	// Power FRO 12MHz.
	pmc_status = PMC_power_on(PMC_BLOCK_FRO12M);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Enable 12MHz clock.
	anactrl_status = ANACTRL_enable_fro(ANACTRL_FRO_TYPE_12MHZ);
	ANACTRL_status_check(SYSCON_ERROR_BASE_ANACTRL);
errors:
	return status;
}

/* ENABLE 32KHZ CLOCK.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_enable_32k_osc(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	PMC_status_t pmc_status = PMC_SUCCESS;
	uint32_t loop_count = 0;
	// Power external low-speed crystal.
	pmc_status = PMC_power_on(PMC_BLOCK_XTAL32K);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	pmc_status = PMC_set_32k_osc_source(PMC_32K_OSC_SOURCE_XTAL32K);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Wait for crystal to be stable.
	while (PMC_xtal32k_is_ready() == 0) {
		loop_count++;
		if (loop_count > SYSCON_TIMEOUT_COUNT) {
			status = SYSCON_ERROR_32K_OSC_TIMEOUT;
			goto errors;
		}
	}
errors:
	return status;
}

/* ENABLE EXTERNAL CRYSTAL OSCILLATOR.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_enable_clk_in(void) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	PMC_status_t pmc_status = PMC_SUCCESS;
	uint32_t loop_count = 0;
	// Power external high-speed crystal.
	pmc_status = PMC_power_on(PMC_BLOCK_LDO_XO32M);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	pmc_status = PMC_power_on(PMC_BLOCK_XTAL32M);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Enable CLK-IN for system clock.
	SYSCON -> CLOCK_CTRL |= (0b1 << 5);
	// Wait for crystal to be stable.
	while (ANACTRL_xo32m_is_ready() == 0) {
		loop_count++;
		if (loop_count > SYSCON_TIMEOUT_COUNT) {
			status = SYSCON_ERROR_CLK_IN_TIMEOUT;
			goto errors;
		}
	}
errors:
	return status;
}

/* SWTICH TO FRO 12MHZ INTERNAL OSCILLATOR.
 * @param:	None.
 * @return:	None.
 */
void _SYSCON_switch_to_fro_12m(void) {
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
	PMC_status_t pmc_status = PMC_SUCCESS;
	ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
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
	pmc_status = PMC_power_off(PMC_BLOCK_PLL1);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Configure PLL1.
	anactrl_status = ANACTRL_enable_xo32m_output(ANACTRL_XO32M_OUTPUT_SYSTEM_CLK);
	ANACTRL_status_check(SYSCON_ERROR_BASE_ANACTRL);
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
	pmc_status = PMC_power_on(PMC_BLOCK_PLL1);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Wait for PLL0 to be locked.
	while (((SYSCON -> PLL1STAT) & (0b1 << 0)) == 0) {
		loop_count++;
		if (loop_count > SYSCON_TIMEOUT_COUNT) {
			status = SYSCON_ERROR_PLL1_LOCK_TIMEOUT;
			goto errors;
		}
	}
	// Switch main clock to PLL1.
	SYSCON -> MAINCLKSELA = 0x01;
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
	status = _SYSCON_enable_fro();
	if (status != SYSCON_SUCCESS) goto errors;
	_SYSCON_switch_to_fro_12m();
	// Output (main clock / 100) on CLKOUT pin.
	SYSCON -> CLKOUTDIV = 0x64;
	SYSCON -> CLKOUTSEL = 0x00;
	GPIO_configure(&GPIO_CLKOUT, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_FAST, GPIO_PULL_NONE);
	// Enable low speed crystal.
	status = _SYSCON_enable_32k_osc();
	if (status != SYSCON_SUCCESS) goto errors;
	// Enable high speed crystal.
	status = _SYSCON_enable_clk_in();
	if (status != SYSCON_SUCCESS) goto errors;
	// Set latency.
	flash_status = FLASH_set_latency(8);
	FLASH_status_check(SYSCON_ERROR_BASE_FLASH);
	// Switch to PLL1.
	status = _SYSCON_switch_to_pll1();
errors:
	return status;
}
