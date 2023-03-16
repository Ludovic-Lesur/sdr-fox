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
#include "types.h"

/*** SYSCON local macros ***/

#define SYSCON_TIMEOUT_COUNT					1000000

#define SYSCON_FRO12M_FREQUENCY_HZ				12000000
#define SYSCON_XTAL32M_FREQUENCY_HZ				16000000

#define SYSCON_SYSTEM_CLOCK_FREQUENCY_HZ_MAX	150000000

#define SYSCON_PLL1_M							25
#define SYSCON_PLL1_N							4

#define SYSCON_SELP_MAX_VALUE					31

#define SYSCON_SELI_MAX_VALUE					63
#define SYSCON_SELI_M_THRESHOLD_1				8000
#define SYSCON_SELI_M_THRESHOLD_2				122

#define SYSCON_NUMBER_OF_WAIT_STATES_THRESHOLDS	12

/*** SYSCON local structures ***/

typedef enum {
	SYSCON_MAIN_CLOCK_A_SOURCE_FRO12M = 0,
	SYSCON_MAIN_CLOCK_A_SOURCE_CLKIN,
	SYSCON_MAIN_CLOCK_A_SOURCE_FRO1M,
	SYSCON_MAIN_CLOCK_A_SOURCE_FRO96M,
	SYSCON_MAIN_CLOCK_A_SOURCE_LAST
} SYSCON_main_clock_a_source;

typedef enum {
	SYSCON_MAIN_CLOCK_B_SOURCE_MAIN_CLOCK_A = 0,
	SYSCON_MAIN_CLOCK_B_SOURCE_PLL0,
	SYSCON_MAIN_CLOCK_B_SOURCE_PLL1,
	SYSCON_MAIN_CLOCK_B_SOURCE_32K_OSC,
	SYSCON_MAIN_CLOCK_B_SOURCE_LAST
} SYSCON_main_clock_b_source;

typedef struct {
	uint8_t ahb_register_index;
	uint8_t bit_index;
} SYSCON_peripheral_bit_t;

typedef struct {
	uint32_t system_clock_hz;
} SYSCON_context_t;

/*** SYSCON local global variables ***/

static const SYSCON_peripheral_bit_t SYSCON_PERIPHERAL_CLOCK_BIT[SYSCON_PERIPHERAL_LAST] = {
	{0, 1}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 11}, {0, 13}, {0, 14}, {0, 15}, {0, 16}, {0, 17}, {0, 18}, {0, 19}, {0, 20}, {0, 21}, {0, 22}, {0, 23}, {0, 26}, {0, 27},
	{1, 0}, {1, 1}, {1, 2}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}, {1, 16}, {1, 17}, {1, 18}, {1, 22}, {1, 25}, {1, 26}, {1, 27},
	{2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 13}, {2, 15}, {2, 16}, {2, 17}, {2, 18}, {2, 19}, {2, 20}, {2, 21}, {2, 22}, {2, 23}, {2, 24}, {2, 27}, {2, 28}, {2, 29}, {2, 30}
};
static const uint32_t SYSCON_WAIT_STATES_THRESHOLDS[SYSCON_NUMBER_OF_WAIT_STATES_THRESHOLDS] = {
	11000000, 22000000, 33000000, 44000000, 55000000, 66000000, 77000000, 88000000, 100000000, 115000000, 130000000, 150000000
};
static SYSCON_context_t syscon_ctx;

/*** SYSCON local functions ***/

/* CHECK PERIPHERAL PARAMETER.
 * @param:	None.
 * @return:	None.
 */
#define _SYSCON_check_peripheral(void) { \
	if (peripheral >= SYSCON_PERIPHERAL_LAST) { \
		status = SYSCON_ERROR_PERIPHERAL; \
		goto errors; \
	} \
}

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

/* SET FLASH LATENCY ACCORDING TO CURRENT SYSTEM CLOCK.
 * @param:			None.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_update_latency(void) {
	// Local variables.
	SYSCON_status_t status = FLASH_SUCCESS;
	FLASH_status_t flash_status = FLASH_SUCCESS;
	uint8_t wait_states = 0;
	// Compute number of wait states.
	for (wait_states=0 ; wait_states<SYSCON_NUMBER_OF_WAIT_STATES_THRESHOLDS ; wait_states++) {
		if (syscon_ctx.system_clock_hz <= SYSCON_WAIT_STATES_THRESHOLDS[wait_states]) {
			break;
		}
	}
	// Set flash latency.
	flash_status = FLASH_set_latency(wait_states);
	FLASH_status_check(SYSCON_ERROR_BASE_FLASH);
	SYSCON -> FMCCR &= ~(0b1111 << 12);
	SYSCON -> FMCCR |= ((wait_states & 0b1111) << 12);
errors:
	return status;
}

/* SWITCH SYSTEM CLOCK.
 * @param source_a:	Main clock A source.
 * @param source_b:	Main clock B source.
 * @return status:	Function execution status.
 */
SYSCON_status_t _SYSCON_switch_system_clock(SYSCON_main_clock_a_source source_a, SYSCON_main_clock_b_source source_b) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	// Check parameters.
	if (source_a >= SYSCON_MAIN_CLOCK_A_SOURCE_LAST) {
		status = SYSCON_ERROR_MAIN_CLOCK_A_SOURCE;
		goto errors;
	}
	if (source_b >= SYSCON_MAIN_CLOCK_B_SOURCE_LAST) {
		status = SYSCON_ERROR_MAIN_CLOCK_B_SOURCE;
		goto errors;
	}
	// Check targetted frequency.
	if (syscon_ctx.system_clock_hz > SYSCON_SYSTEM_CLOCK_FREQUENCY_HZ_MAX) {
		status = SYSCON_ERROR_SYSTEM_FREQUENCY_OVERFLOW;
		goto errors;
	}
	// Set latency.
	status = _SYSCON_update_latency();
	if (status != SYSCON_SUCCESS) goto errors;
	// Switch clock.
	SYSCON -> MAINCLKSELA = source_a;
	SYSCON -> MAINCLKSELB = source_b;
errors:
	return status;
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
	uint32_t selp = 0;
	uint32_t seli = 0;
	uint32_t loop_count = 0;
	uint64_t sysclk_hz = 0;
	// Compute bandwidth values.
	selp = ((SYSCON_PLL1_M / 4) + 1);
	if (selp > SYSCON_SELP_MAX_VALUE) {
		selp = SYSCON_SELP_MAX_VALUE;
	}
	if (SYSCON_PLL1_M >= SYSCON_SELI_M_THRESHOLD_1) {
		seli = 1;
	}
	else {
		if (SYSCON_PLL1_M >= SYSCON_SELI_M_THRESHOLD_2) {
			seli = (SYSCON_SELI_M_THRESHOLD_1 / SYSCON_PLL1_M);
		}
		else {
			seli = (2 * (SYSCON_PLL1_M / 4)) + 3;
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
	SYSCON -> PLL1CTRL |= (seli << 4) | (selp << 10) | (0b11 << 20); // Bypass post-divider and enable output.
	SYSCON -> PLL1MDEC = SYSCON_PLL1_M;
	SYSCON -> PLL1MDEC |= (0b1 << 16);
	SYSCON -> PLL1NDEC = SYSCON_PLL1_N;
	SYSCON -> PLL1NDEC |= (0b1 << 8);
	SYSCON -> PLL1PDEC = 0;
	SYSCON -> PLL1PDEC |= (0b1 << 5);
	// Turn PLL1 on.
	pmc_status = PMC_power_on(PMC_BLOCK_PLL1);
	PMC_status_check(SYSCON_ERROR_BASE_PMC);
	// Wait for PLL1 to be locked.
	while (((SYSCON -> PLL1STAT) & (0b1 << 0)) == 0) {
		loop_count++;
		if (loop_count > SYSCON_TIMEOUT_COUNT) {
			status = SYSCON_ERROR_PLL1_LOCK_TIMEOUT;
			goto errors;
		}
	}
	// Update system clock value.
	sysclk_hz = ((uint64_t) SYSCON_PLL1_M) * ((uint64_t) SYSCON_XTAL32M_FREQUENCY_HZ);
	sysclk_hz /= (uint64_t) SYSCON_PLL1_N;
	syscon_ctx.system_clock_hz = (uint32_t) sysclk_hz;
	// Switch to PLL.
	status = _SYSCON_switch_system_clock(SYSCON_MAIN_CLOCK_A_SOURCE_CLKIN, SYSCON_MAIN_CLOCK_B_SOURCE_PLL1);
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
	// Init context.
	syscon_ctx.system_clock_hz = SYSCON_FRO12M_FREQUENCY_HZ;
	// Enable analog control block.
	status = SYSCON_enable_peripheral(SYSCON_PERIPHERAL_ANACTRL);
	if (status != SYSCON_SUCCESS) goto errors;
	// Switch on internal clock by default.
	status = _SYSCON_enable_fro();
	if (status != SYSCON_SUCCESS) goto errors;
	status = _SYSCON_switch_system_clock(SYSCON_MAIN_CLOCK_A_SOURCE_FRO12M, SYSCON_MAIN_CLOCK_B_SOURCE_MAIN_CLOCK_A);
	if (status != SYSCON_SUCCESS) goto errors;
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
	// Switch to PLL1.
	status = _SYSCON_switch_to_pll1();
errors:
	return status;
}

/* ENABLE PERIPHERAL CLOCK.
 * @param peripheral:	Peripheral to enable.
 * @return status:		Function execution status.
 */
SYSCON_status_t SYSCON_enable_peripheral(SYSCON_peripheral_t peripheral) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	// Check parameter.
	_SYSCON_check_peripheral();
	// Enable clock.
	SYSCON -> AHBCLKCTRLSET[SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].bit_index);
errors:
	return status;
}

/* RESET PERIPHERAL.
 * @param peripheral:	Peripheral to reset.
 * @return status:		Function execution status.
 */
SYSCON_status_t SYSCON_reset_peripheral(SYSCON_peripheral_t peripheral) {
	// Local variables.
	SYSCON_status_t status = SYSCON_SUCCESS;
	// Check parameter.
	_SYSCON_check_peripheral();
	// Never reset ANACTRL block.
	if (peripheral == SYSCON_PERIPHERAL_ANACTRL) {
		status = SYSCON_ERROR_PERIPHERAL;
		goto errors; \
	}
	// Reset peripheral.
	SYSCON -> PRESETCTRLSET[SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].bit_index);
	SYSCON -> PRESETCTRLCLR[SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_CLOCK_BIT[peripheral].bit_index);
errors:
	return status;
}
