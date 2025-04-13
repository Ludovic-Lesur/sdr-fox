/*
 * syscon.c
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#include "syscon.h"

#ifndef LPC55XX_DRIVERS_DISABLE_FLAGS_FILE
#include "lpc55xx_drivers_flags.h"
#endif
#include "anactrl.h"
#include "error.h"
#include "flash.h"
#include "gpio.h"
#include "pmc.h"
#include "syscon_registers.h"
#include "types.h"

/*** SYSCON local macros ***/

#define SYSCON_TIMEOUT_COUNT                    1000000

#define SYSCON_FRO_32K_FREQUENCY_TYPICAL_HZ     32768
#define SYSCON_FRO_1M_FREQUENCY_TYPICAL_HZ      1000000
#define SYSCON_FRO_12M_FREQUENCY_TYPICAL_HZ     12000000
#define SYSCON_FRO_HF_FREQUENCY_TYPICAL_HZ      96000000

#define SYSCON_WAIT_STATES_MAX                  12

#define SYSCON_PLL_N_MIN                        1
#define SYSCON_PLL_INPUT_CLOCK_HZ_MIN           2000
#define SYSCON_PLL_INPUT_CLOCK_HZ_MAX           150000000
#define SYSCON_PLL_M_MIN                        1
#define SYSCON_PLL_CCO_CLOCK_HZ_MIN             275000000
#define SYSCON_PLL_CCO_CLOCK_HZ_MAX             550000000
#define SYSCON_PLL_P_MIN                        1
#define SYSCON_PLL_P_MAX                        62
#define SYSCON_PLL_OUTPUT_CLOCK_MIN_HZ          4300000
#define SYSCON_PLL_OUTPUT_CLOCK_MAX_HZ          150000000
#define SYSCON_PLL_SELP_MAX_VALUE               31
#define SYSCON_PLL_SELI_MAX_VALUE               63
#define SYSCON_PLL_SELI_M_THRESHOLD_1           8000
#define SYSCON_PLL_SELI_M_THRESHOLD_2           122

#define SYSCON_CLKOUT_DIVIDER_MIN               1
#define SYSCON_CLKOUT_DIVIDER_MAX               256

/*** SYSCON local structures ***/

/*******************************************************************/
typedef struct {
    uint8_t ahb_register_index;
    uint8_t bit_index;
} SYSCON_peripheral_descriptor_t;

/*******************************************************************/
typedef struct {
    SYSCON_clock_t clock;
    PMC_block_t power_block;
    volatile uint32_t* pllclksel;
    volatile uint32_t* pllctrl;
    volatile uint32_t* pllndec;
    volatile uint32_t* pllpdec;
    volatile uint32_t* pllstat;
} SYSCON_pll_descriptor_t;

/*******************************************************************/
typedef struct {
    SYSCON_clock_t main_clock;
    SYSCON_clock_t main_clock_a;
    uint32_t clock_frequency[SYSCON_CLOCK_LAST];
} SYSCON_context_t;

/*** SYSCON local global variables ***/

static const SYSCON_peripheral_descriptor_t SYSCON_PERIPHERAL_DESCRIPTOR[SYSCON_PERIPHERAL_LAST] = {
    { 0, 1 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 }, { 0, 11 }, { 0, 13 }, { 0, 14 }, { 0, 15 },
    { 0, 16 }, { 0, 17 }, { 0, 18 }, { 0, 19 }, { 0, 20 }, { 0, 21 }, { 0, 22 }, { 0, 23 }, { 0, 26 }, { 0, 27 },
    { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 10 }, { 1, 11 }, { 1, 12 }, { 1, 13 }, { 1, 14 },
    { 1, 15 }, { 1, 16 }, { 1, 17 }, { 1, 18 }, { 1, 22 }, { 1, 25 }, { 1, 26 }, { 1, 27 },
    { 2, 1 }, { 2, 2 }, { 2, 3 }, { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 }, { 2, 13 },{ 2, 15 }, { 2, 16 }, { 2, 17 },
    { 2, 18 }, { 2, 19 }, { 2, 20 }, { 2, 21 }, { 2, 22 }, { 2, 23 }, { 2, 24 }, { 2, 27 }, { 2, 28 }, { 2, 29 }, { 2, 30 }
};

static const SYSCON_pll_descriptor_t SYSCON_PLL_DESCRIPTOR[SYSCON_PLL_LAST] = {
    { SYSCON_CLOCK_PLL0, PMC_BLOCK_PLL0, &(SYSCON->PLL0CLKSEL), &(SYSCON->PLL0CTRL), &(SYSCON->PLL0NDEC), &(SYSCON->PLL0PDEC), &(SYSCON->PLL0STAT) },
    { SYSCON_CLOCK_PLL1, PMC_BLOCK_PLL1, &(SYSCON->PLL1CLKSEL), &(SYSCON->PLL1CTRL), &(SYSCON->PLL1NDEC), &(SYSCON->PLL1PDEC), &(SYSCON->PLL1STAT) }
};

static const uint32_t SYSCON_WAIT_STATES_THRESHOLDS[SYSCON_WAIT_STATES_MAX] = {
    11000000, 22000000, 33000000, 44000000, 55000000, 66000000, 77000000, 88000000, 100000000, 115000000, 130000000, 150000000
};

static const uint8_t SYSCON_MAINCLKSELA[SYSCON_CLOCK_LAST] = { 0b111, 0b111, 0b111, 0b011, 0b111, 0b000, 0b010, 0b001, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111 };

static const uint8_t SYSCON_MAINCLKSELB[SYSCON_CLOCK_LAST] = { 0b111, 0b111, 0b000, 0b111, 0b111, 0b111, 0b111, 0b111, 0b001, 0b111, 0b010, 0b111, 0b111, 0b011 };

static const uint8_t SYSCON_CLKOUTSEL[SYSCON_CLOCK_LAST] = { 0b111, 0b000, 0b111, 0b011, 0b111, 0b111, 0b100, 0b010, 0b001, 0b111, 0b101, 0b111, 0b111, 0b110 };

static SYSCON_context_t syscon_ctx;

/*** SYSCON local functions ***/

/*******************************************************************/
#define _SYSCON_check_peripheral(void) { \
    if (peripheral >= SYSCON_PERIPHERAL_LAST) { \
        status = SYSCON_ERROR_PERIPHERAL; \
        goto errors; \
    } \
}

/*******************************************************************/
static SYSCON_status_t _SYSCON_wait_for_clock_ready(SYSCON_clock_t clock, SYSCON_status_t timeout_error) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    uint8_t clock_is_ready = 0;
    uint32_t loop_count = 0;
    // Wait for clock to be stable.
    do {
        // Read status.
        status = SYSCON_get_status(clock, &clock_is_ready);
        if (status != SYSCON_SUCCESS) goto errors;
        // Exit if timeout.
        loop_count++;
        if (loop_count > SYSCON_TIMEOUT_COUNT) {
            status = timeout_error;
            goto errors;
        }
    }
    while (clock_is_ready == 0);
errors:
    return status;
}


/*******************************************************************/
static SYSCON_status_t _SYSCON_enable_32k_osc(void) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    PMC_oscillator_t low_speed_oscillator = PMC_OSCILLATOR_XTAL32K;
    // First try using external low-speed crystal.
    pmc_status = PMC_set_power(PMC_BLOCK_XTAL_32K, 1);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Wait for external crystal to be stable.
    status = _SYSCON_wait_for_clock_ready(SYSCON_CLOCK_XTAL_32K, SYSCON_ERROR_XTAL_32K_TIMEOUT);
    // Check result.
    switch (status) {
    case SYSCON_SUCCESS:
        // Turn internal oscillator off.
        pmc_status = PMC_set_power(PMC_BLOCK_FRO_32K, 0);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        break;
    case SYSCON_ERROR_XTAL_32K_TIMEOUT:
        // Try with internal oscillator.
        pmc_status = PMC_set_power(PMC_BLOCK_XTAL_32K, 0);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        pmc_status = PMC_set_power(PMC_BLOCK_FRO_32K, 1);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        // Wait for external crystal to be stable.
        status = _SYSCON_wait_for_clock_ready(SYSCON_CLOCK_FRO_32K, SYSCON_ERROR_FRO_32K_TIMEOUT);
        if (status != SYSCON_SUCCESS) goto errors;
        // Update source.
        low_speed_oscillator = PMC_OSCILLATOR_FRO32K;
        break;
    default:
        goto errors;
    }
    pmc_status = PMC_set_32k_osc_source(low_speed_oscillator);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Double check.
    status = _SYSCON_wait_for_clock_ready(SYSCON_CLOCK_32K_OSC, SYSCON_ERROR_32K_OSC_TIMEOUT);
    if (status != SYSCON_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
static SYSCON_status_t _SYSCON_enable_fro_1m(void) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    // Enable 1MHz clock.
    SYSCON->CLOCK_CTRL |= (0b1 << 6);
    return status;
}

/*******************************************************************/
static SYSCON_status_t _SYSCON_enable_fro_12m(void) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
    // Power FRO 12MHz.
    pmc_status = PMC_set_power(PMC_BLOCK_FRO_12M, 1);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    anactrl_status = ANACTRL_set_fro_state(ANACTRL_FRO_12M, 1);
    ANACTRL_exit_error(SYSCON_ERROR_BASE_ANACTRL);
errors:
    return status;
}

/*******************************************************************/
static SYSCON_status_t _SYSCON_enable_clk_in(void) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
    // Power external high-speed crystal.
    pmc_status = PMC_set_power(PMC_BLOCK_LDO_XO_32M, 1);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    pmc_status = PMC_set_power(PMC_BLOCK_XTAL_32M, 1);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Enable CLK-IN for system clock.
    SYSCON->CLOCK_CTRL |= (0b1 << 5);
    anactrl_status = ANACTRL_set_xo32m_output(ANACTRL_XO32M_OUTPUT_SYSTEM_CLOCK, 1);
    ANACTRL_exit_error(SYSCON_ERROR_BASE_ANACTRL);
errors:
    return status;
}

/*******************************************************************/
static uint8_t _SYSCON_compute_latency(uint32_t system_clock_frequency_hz) {
    // Local variables.
    uint8_t wait_states = 0;
    // Compute number of wait states.
    for (wait_states = 0; wait_states < SYSCON_WAIT_STATES_MAX; wait_states++) {
        if (system_clock_frequency_hz <= SYSCON_WAIT_STATES_THRESHOLDS[wait_states]) {
            break;
        }
    }
    return wait_states;
}

/*******************************************************************/
static uint8_t _SYSCON_get_latency(void) {
    // Local variables.
    uint8_t wait_states = 0;
    // Read register.
    wait_states = ((SYSCON->FMCCR) >> 12) & 0x0F;
    // Return.
    return wait_states;
}

/*******************************************************************/
static SYSCON_status_t _SYSCON_set_latency(uint8_t wait_states) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    FLASH_status_t flash_status = FLASH_SUCCESS;
    uint32_t fmccr_reg_value = 0;
    // Set flash latency.
    flash_status = FLASH_set_latency(wait_states);
    FLASH_exit_error(SYSCON_ERROR_BASE_FLASH);
    // Set local latency.
    fmccr_reg_value = (SYSCON->FMCCR);
    fmccr_reg_value &= ~(0b1111 << 12);
    fmccr_reg_value |= ((wait_states & 0b1111) << 12);
    SYSCON->FMCCR = fmccr_reg_value;
errors:
    return status;
}

/*** SYSCON functions ***/

/*******************************************************************/
SYSCON_status_t SYSCON_init(void) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    uint8_t idx = 0;
    // Set boot configuration.
    syscon_ctx.main_clock = SYSCON_CLOCK_MAIN_A;
    syscon_ctx.main_clock_a = SYSCON_CLOCK_FRO_12M;
    // Set default frequencies.
    for (idx = 0; idx < SYSCON_CLOCK_LAST; idx++) {
        syscon_ctx.clock_frequency[idx] = 0;
    }
    syscon_ctx.clock_frequency[SYSCON_CLOCK_FRO_HF] = SYSCON_FRO_HF_FREQUENCY_TYPICAL_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_FRO_HF_DIV] = SYSCON_FRO_HF_FREQUENCY_TYPICAL_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_FRO_12M] = SYSCON_FRO_12M_FREQUENCY_TYPICAL_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_FRO_1M] = SYSCON_FRO_1M_FREQUENCY_TYPICAL_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_CLK_IN_XO32M] = LPC55XX_DRIVERS_SYSCON_XO32M_FREQUENCY_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_FRO_32K] = SYSCON_FRO_32K_FREQUENCY_TYPICAL_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_32K_OSC] = LPC55XX_DRIVERS_SYSCON_XTAL32K_FREQUENCY_HZ;
    syscon_ctx.clock_frequency[SYSCON_CLOCK_MAIN_A] = syscon_ctx.clock_frequency[syscon_ctx.main_clock_a];
    syscon_ctx.clock_frequency[SYSCON_CLOCK_MAIN] = syscon_ctx.clock_frequency[syscon_ctx.main_clock];
    // Enable memories clock.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_ROM, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_SRAM_CTRL1, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_SRAM_CTRL2, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_SRAM_CTRL3, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_SRAM_CTRL4, 1);
    // Enable FLASH and ANACTRL blocks.
    status = SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_FLASH, 1);
    if (status != SYSCON_SUCCESS) goto errors;
    status = SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_ANACTRL, 1);
    if (status != SYSCON_SUCCESS) goto errors;
    // Enable low speed crystal.
    status = _SYSCON_enable_32k_osc();
    if (status != SYSCON_SUCCESS) goto errors;
    // Switch on internal 12MHz clock by default.
    status = _SYSCON_enable_fro_12m();
    if (status != SYSCON_SUCCESS) goto errors;
    status = SYSCON_switch_system_clock(SYSCON_CLOCK_FRO_12M, SYSCON_CLOCK_MAIN_A);
    if (status != SYSCON_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_set_peripheral_clock(SYSCON_peripheral_t peripheral, uint8_t state) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    // Check parameter.
    _SYSCON_check_peripheral();
    // Check state.
    if (state == 0) {
        SYSCON->AHBCLKCTRLCLR[SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].bit_index);
    }
    else {
        SYSCON->AHBCLKCTRLSET[SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].bit_index);
    }
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_reset_peripheral(SYSCON_peripheral_t peripheral) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    // Check parameter.
    _SYSCON_check_peripheral();
    // Never reset ANACTRL block.
    if (peripheral == SYSCON_PERIPHERAL_ANACTRL) {
        status = SYSCON_ERROR_PERIPHERAL;
        goto errors;
    }
    // Reset peripheral.
    SYSCON->PRESETCTRLSET[SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].bit_index);
    SYSCON->PRESETCTRLCLR[SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].ahb_register_index] = (0b1 << SYSCON_PERIPHERAL_DESCRIPTOR[peripheral].bit_index);
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_configure_pll(SYSCON_pll_t pll, SYSCON_pll_configuration_t* pll_configuration) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    uint32_t selp = 0;
    uint32_t seli = 0;
    uint8_t pllsel = 0b111;
    uint32_t pll_in_hz = 0;
    uint32_t pll_vco_hz = 0;
    uint64_t pll_out_hz = 0;
    uint32_t loop_count = 0;
    // Check parameters.
    if (pll >= SYSCON_PLL_LAST) {
        status = SYSCON_ERROR_PLL;
        goto errors;
    }
    if (pll_configuration == NULL) {
        status = SYSCON_ERROR_NULL_PARAMETER;
        goto errors;
    }
    if ((pll_configuration->m) < SYSCON_PLL_M_MIN) {
        status = SYSCON_ERROR_PLL_M;
        goto errors;
    }
    if ((pll_configuration->n) < SYSCON_PLL_N_MIN) {
        status = SYSCON_ERROR_PLL_N;
        goto errors;
    }
    if (((pll_configuration->p) < SYSCON_PLL_P_MIN) || ((pll_configuration->p) > SYSCON_PLL_P_MAX)) {
        status = SYSCON_ERROR_PLL_P;
        goto errors;
    }
    if (((pll_configuration->p) > SYSCON_PLL_P_MIN) && (((pll_configuration->p) % 2) != 0)) {
        status = SYSCON_ERROR_PLL_P;
        goto errors;
    }
    // PLL should be configured while it is used as system clock.
    if (syscon_ctx.main_clock == SYSCON_PLL_DESCRIPTOR[pll].clock) {
        status = SYSCON_ERROR_PLL_BUSY;
        goto errors;
    }
    // Check clock source.
    switch (pll_configuration->source) {
    case SYSCON_CLOCK_FRO_12M:
        status = _SYSCON_enable_fro_12m();
        if (status != SYSCON_SUCCESS) goto errors;
        pllsel = 0b000;
        break;
    case SYSCON_CLOCK_CLK_IN_XO32M:
        status = _SYSCON_enable_clk_in();
        if (status != SYSCON_SUCCESS) goto errors;
        pllsel = 0b001;
        break;
    case SYSCON_CLOCK_FRO_1M:
        status = _SYSCON_enable_fro_1m();
        if (status != SYSCON_SUCCESS) goto errors;
        pllsel = 0b010;
        break;
    case SYSCON_CLOCK_32K_OSC:
        status = _SYSCON_enable_32k_osc();
        if (status != SYSCON_SUCCESS) goto errors;
        pllsel = 0b011;
        break;
    default:
        status = SYSCON_ERROR_PLL_CLOCK_SOURCE;
        goto errors;
    }
    // Wait for clock source to be ready.
    status = _SYSCON_wait_for_clock_ready(pll_configuration->source, SYSCON_ERROR_PLL_CLOCK_SOURCE_READY);
    if (status != SYSCON_SUCCESS) goto errors;
    // Power down PLL during configuration.
    pmc_status = PMC_set_power(SYSCON_PLL_DESCRIPTOR[pll].power_block, 0);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Enable output.
    (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) |= (0b1 << 21);
    // Configure source.
    (*SYSCON_PLL_DESCRIPTOR[pll].pllclksel) &= 0xFFFFFFF8;
    (*SYSCON_PLL_DESCRIPTOR[pll].pllclksel) |= (pllsel & 0x07);
    // Input clock divider.
    pll_in_hz = (syscon_ctx.clock_frequency[pll_configuration->source] / (pll_configuration->n));
    if (pll_in_hz < SYSCON_PLL_INPUT_CLOCK_HZ_MIN) {
        status = SYSCON_ERROR_PLL_INPUT_CLOCK_FREQUENCY_UNDERFLOW;
        goto errors;
    }
    if (pll_in_hz > SYSCON_PLL_INPUT_CLOCK_HZ_MAX) {
        status = SYSCON_ERROR_PLL_INPUT_CLOCK_FREQUENCY_OVERFLOW;
        goto errors;
    }
    if ((pll_configuration->n) == 1) {
        (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) |= (0b1 << 19);
    }
    else {
        (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) &= ~(0b1 << 19);
        (*SYSCON_PLL_DESCRIPTOR[pll].pllndec) &= 0xFFFFFF00;
        (*SYSCON_PLL_DESCRIPTOR[pll].pllndec) |= ((pll_configuration->n) & 0xFF);
        (*SYSCON_PLL_DESCRIPTOR[pll].pllndec) |= (0b1 << 8);
    }
    // VCO.
    pll_vco_hz = (pll_in_hz * (pll_configuration->m));
    if (pll_vco_hz < SYSCON_PLL_CCO_CLOCK_HZ_MIN) {
        status = SYSCON_ERROR_PLL_CCO_CLOCK_FREQUENCY_UNDERFLOW;
        goto errors;
    }
    if (pll_vco_hz > SYSCON_PLL_CCO_CLOCK_HZ_MAX) {
        status = SYSCON_ERROR_PLL_CCO_CLOCK_FREQUENCY_OVERFLOW;
        goto errors;
    }
    switch (pll) {
    case SYSCON_PLL0:
        SYSCON->PLL0SSCG1 &= 0xEC0003FF;
        SYSCON->PLL0SSCG1 |= (((pll_configuration->m) & 0xFFFF) << 10);
        SYSCON->PLL0SSCG1 |= (0b1 << 26);
        break;
    case SYSCON_PLL1:
        SYSCON->PLL1MDEC &= 0xFFFF0000;
        SYSCON->PLL1MDEC |= ((pll_configuration->m) & 0xFFFF);
        SYSCON->PLL1MDEC |= (0b1 << 16);
        break;
    default:
        status = SYSCON_ERROR_PLL;
        goto errors;
    }
    // Output prescaler.
    pll_out_hz = (pll_vco_hz / (pll_configuration->p));
    if (pll_out_hz < SYSCON_PLL_OUTPUT_CLOCK_MIN_HZ) {
        status = SYSCON_ERROR_PLL_OUTPUT_CLOCK_UNDERFLOW;
        goto errors;
    }
    if (pll_out_hz > SYSCON_PLL_OUTPUT_CLOCK_MAX_HZ) {
        status = SYSCON_ERROR_PLL_OUTPUT_CLOCK_OVERFLOW;
        goto errors;
    }
    if ((pll_configuration->p) == 1) {
        (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) |= (0b1 << 20);
    }
    else {
        (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) &= ~(0b1 << 20);
        (*SYSCON_PLL_DESCRIPTOR[pll].pllpdec) &= 0xFFFFFFE0;
        (*SYSCON_PLL_DESCRIPTOR[pll].pllpdec) |= (((pll_configuration->p) >> 1) & 0x1F);
        (*SYSCON_PLL_DESCRIPTOR[pll].pllpdec) |= (0b1 << 5);
    }
    // Compute bandwidth values.
    selp = (((pll_configuration->m) >> 2) + 1);
    if (selp > SYSCON_PLL_SELP_MAX_VALUE) {
        selp = SYSCON_PLL_SELP_MAX_VALUE;
    }
    if ((pll_configuration->m) >= SYSCON_PLL_SELI_M_THRESHOLD_1) {
        seli = 1;
    }
    else {
        if ((pll_configuration->m) >= SYSCON_PLL_SELI_M_THRESHOLD_2) {
            seli = (SYSCON_PLL_SELI_M_THRESHOLD_1 / (pll_configuration->m));
        }
        else {
            seli = (((pll_configuration->m) >> 2) << 1) + 3;
        }
    }
    if (seli > SYSCON_PLL_SELI_MAX_VALUE) {
        seli = SYSCON_PLL_SELI_MAX_VALUE;
    }
    (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) &= 0xFFFFE000;
    (*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) |= ((selp & 0x1F) << 10) | ((seli & 0x3F) << 4);
    // Enable output.
    //(*SYSCON_PLL_DESCRIPTOR[pll].pllctrl) |= (0b1 << 21);
    // Turn PLL1 on.
    pmc_status = PMC_set_power(SYSCON_PLL_DESCRIPTOR[pll].power_block, 1);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Wait for PLL1 to be locked.
    while (((*SYSCON_PLL_DESCRIPTOR[pll].pllstat) & (0b1 << 0)) == 0) {
        loop_count++;
        if (loop_count > SYSCON_TIMEOUT_COUNT) {
            status = SYSCON_ERROR_PLL_LOCK_TIMEOUT;
            goto errors;
        }
    }
    // Update clock frequency.
    syscon_ctx.clock_frequency[SYSCON_PLL_DESCRIPTOR[pll].clock] = pll_out_hz;
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_switch_system_clock(SYSCON_clock_t main_clock_a, SYSCON_clock_t main_clock) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    uint8_t current_latency = 0;
    uint8_t new_latency = 0;
    // Check parameters.
    switch (main_clock_a) {
    case SYSCON_CLOCK_FRO_12M:
    case SYSCON_CLOCK_CLK_IN_XO32M:
    case SYSCON_CLOCK_FRO_1M:
    case SYSCON_CLOCK_FRO_HF:
        break;
    default:
        status = SYSCON_ERROR_MAIN_CLOCK_A_SOURCE;
        goto errors;
    }
    switch (main_clock) {
    case SYSCON_CLOCK_MAIN_A:
    case SYSCON_CLOCK_PLL0:
    case SYSCON_CLOCK_PLL1:
    case SYSCON_CLOCK_32K_OSC:
        break;
    default:
        status = SYSCON_ERROR_MAIN_CLOCK_B_SOURCE;
        goto errors;
    }
    // Wait for clock sources to be ready.
    status = _SYSCON_wait_for_clock_ready(main_clock, SYSCON_ERROR_MAIN_CLOCK_READY);
    if (status != SYSCON_SUCCESS) goto errors;
    // Update DC-DC.
    pmc_status = PMC_configure_dc_dc(syscon_ctx.clock_frequency[main_clock]);
    PMC_exit_error(SYSCON_ERROR_BASE_PMC);
    // Computes latencies.
    current_latency = _SYSCON_get_latency();
    new_latency = _SYSCON_compute_latency(syscon_ctx.clock_frequency[main_clock]);
    // Set latency before switching if it has to be increased.
    if (new_latency > current_latency) {
        status = _SYSCON_set_latency(new_latency);
        if (status != SYSCON_SUCCESS) goto errors;
    }
    // Update context.
    syscon_ctx.main_clock_a = main_clock_a;
    syscon_ctx.main_clock = main_clock;
    // Switch clock.
    SYSCON->MAINCLKSELA = SYSCON_MAINCLKSELA[main_clock_a];
    SYSCON->MAINCLKSELB = SYSCON_MAINCLKSELB[main_clock];
    // Set latency after switching if it has to be reduced.
    if (new_latency < current_latency) {
        status = _SYSCON_set_latency(new_latency);
        if (status != SYSCON_SUCCESS) goto errors;
    }
errors:
    syscon_ctx.clock_frequency[SYSCON_CLOCK_MAIN_A] = syscon_ctx.clock_frequency[syscon_ctx.main_clock_a];
    syscon_ctx.clock_frequency[SYSCON_CLOCK_MAIN] = syscon_ctx.clock_frequency[syscon_ctx.main_clock];
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_get_frequency_hz(SYSCON_clock_t clock, uint32_t* frequency_hz) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    // Check parameters.
    if (clock >= SYSCON_CLOCK_LAST) {
        status = SYSCON_ERROR_CLOCK;
        goto errors;
    }
    if (frequency_hz == NULL) {
        status = SYSCON_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Read frequency.
    (*frequency_hz) = syscon_ctx.clock_frequency[clock];
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_get_status(SYSCON_clock_t clock, uint8_t* clock_is_ready) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    // Check parameters.
    if (clock_is_ready == NULL) {
        status = SYSCON_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Reset result.
    (*clock_is_ready) = 0;
    // Check clock.
    switch (clock) {
    case SYSCON_CLOCK_MAIN:
    case SYSCON_CLOCK_FRO_1M:
        (*clock_is_ready) = 1;
        break;
    case SYSCON_CLOCK_MAIN_A:
        status = SYSCON_get_status(syscon_ctx.main_clock_a, clock_is_ready);
        if (status != SYSCON_SUCCESS) goto errors;
        break;
    case SYSCON_CLOCK_FRO_HF_DIV:
        // Check source.
        status = SYSCON_get_status(SYSCON_CLOCK_FRO_HF, clock_is_ready);
        if ((status != SYSCON_SUCCESS) || ((*clock_is_ready) == 0)) goto errors;
        // Check output.
        (*clock_is_ready) = ((((SYSCON->FROHFDIV) & 0xE0000000) == 0) ? 1 : 0);
        break;
    case SYSCON_CLOCK_FRO_HF:
        anactrl_status = ANACTRL_get_fro_status(ANACTRL_FRO_HF, clock_is_ready);
        ANACTRL_exit_error(SYSCON_ERROR_BASE_ANACTRL);
        break;
    case SYSCON_CLOCK_FRO_12M:
        anactrl_status = ANACTRL_get_fro_status(ANACTRL_FRO_12M, clock_is_ready);
        ANACTRL_exit_error(SYSCON_ERROR_BASE_ANACTRL);
        break;
    case SYSCON_CLOCK_CLK_IN_XO32M:
        anactrl_status = ANACTRL_get_xo32m_status(clock_is_ready);
        ANACTRL_exit_error(SYSCON_ERROR_BASE_ANACTRL);
        break;
    case SYSCON_CLOCK_PLL0:
        (*clock_is_ready) = (((SYSCON->PLL0STAT) >> 0) & 0b1);
        break;
    case SYSCON_CLOCK_PLL0_DIV:
        // Check source.
        status = SYSCON_get_status(SYSCON_CLOCK_PLL0, clock_is_ready);
        if ((status != SYSCON_SUCCESS) || ((*clock_is_ready) == 0)) goto errors;
        // Check output.
        (*clock_is_ready) = ((((SYSCON->PLL0CLKDIV) & 0xE0000000) == 0) ? 1 : 0);
        break;
    case SYSCON_CLOCK_PLL1:
        (*clock_is_ready) = (((SYSCON->PLL1STAT) >> 0) & 0b1);
        break;
    case SYSCON_CLOCK_FRO_32K:
        pmc_status = PMC_get_oscillator_status(PMC_OSCILLATOR_FRO32K, clock_is_ready);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        break;
    case SYSCON_CLOCK_XTAL_32K:
        pmc_status = PMC_get_oscillator_status(PMC_OSCILLATOR_XTAL32K, clock_is_ready);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        break;
    case SYSCON_CLOCK_32K_OSC:
        pmc_status = PMC_get_32k_osc_status(clock_is_ready);
        PMC_exit_error(SYSCON_ERROR_BASE_PMC);
        break;
    default:
        status = SYSCON_ERROR_CLOCK;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
SYSCON_status_t SYSCON_set_clkout(SYSCON_clock_t clock, uint16_t divider, const GPIO_pin_t* clkout_gpio) {
    // Local variables.
    SYSCON_status_t status = SYSCON_SUCCESS;
    uint32_t clkoutdiv = (SYSCON->CLKOUTDIV);
    // Check parameters.
    if (clock >= SYSCON_CLOCK_LAST) {
        status = SYSCON_ERROR_CLOCK;
        goto errors;
    }
    if ((divider < SYSCON_CLKOUT_DIVIDER_MIN) || (divider > SYSCON_CLKOUT_DIVIDER_MAX)) {
        status = SYSCON_ERROR_CLKOUT_DIVIDER;
        goto errors;
    }
    // Stop signal.
    SYSCON->CLKOUTDIV |= (0b1 << 30);
    // Set divider.
    clkoutdiv = ((SYSCON->CLKOUTDIV) & 0xDFFFFF00);
    clkoutdiv |= ((divider - 1) & 0xFF);
    SYSCON->CLKOUTDIV = clkoutdiv;
    // Set source.
    SYSCON->CLKOUTSEL = SYSCON_CLKOUTSEL[clock];
    // Start signal.
    SYSCON->CLKOUTDIV &= ~(0b1 << 30);
    // Configure GPIO if needed.
    if (clkout_gpio != NULL) {
        // Check clock selection.
        if (clock == SYSCON_CLOCK_NONE) {
            GPIO_configure(clkout_gpio, GPIO_MODE_ANALOG_INPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
        }
        else {
            GPIO_configure(clkout_gpio, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_FAST, GPIO_PULL_NONE);
        }
    }
errors:
    return status;
}
