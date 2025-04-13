/*
 * systick.c
 *
 *  Created on: Mar 14, 2023
 *      Author: ludo
 */

#include "systick.h"

#ifndef LPC55XX_DRIVERS_DISABLE_FLAGS_FILE
#include "lpc55xx_drivers_flags.h"
#endif
#include "error.h"
#include "maths.h"
#include "nvic.h"
#include "syscon.h"
#include "syscon_registers.h"
#include "systick_registers.h"
#include "types.h"

/*** SYSTICK local macros ***/

#define SYSTICK_TIMER_WIDTH_BITS        24

#define SYSTICK_RELOAD_VALUE_MIN        1
#define SYSTICK_RELOAD_VALUE_MAX        ((1 << SYSTICK_TIMER_WIDTH_BITS) - 1)

#define SYSTICK_RELOAD_VALUE_MASK       SYSTICK_RELOAD_VALUE_MAX

#define SYSTICK_MAIN_CLOCK_DIVIDER_MIN  1
#define SYSTICK_MAIN_CLOCK_DIVIDER_MAX  256

/*** SYSTICK local structures ***/

typedef struct {
    uint8_t clksel;
    SYSCON_clock_t clock;
} SYSTICK_clock_descriptor_t;

/*******************************************************************/
typedef struct {
    volatile uint8_t irq_flag;
    uint8_t init_flag;
    uint32_t clock_frequency_hz;
} SYSTICK_context_t;

/*** SYSTICK local global variables ***/

static const SYSTICK_clock_descriptor_t SYSTICK_CLOCK_DESCRIPTOR[SYSTICK_CLOCK_SOURCE_LAST] = {
    { 0b111, SYSCON_CLOCK_MAIN },
    { 0b000, SYSCON_CLOCK_MAIN },
    { 0b001, SYSCON_CLOCK_FRO_1M },
    { 0b010, SYSCON_CLOCK_32K_OSC}
};

static SYSTICK_context_t systick_ctx = {
    .irq_flag = 0,
    .init_flag = 0,
    .clock_frequency_hz = 0
};

/*** SYSTICK local functions ***/

/*******************************************************************/
void SysTick_Handler(void) {
    systick_ctx.irq_flag = 1;
}

/*******************************************************************/
#define _SYSTICK_check_core(void) { \
    if (core >= SYSTICK_CORE_LAST) { \
        status = SYSTICK_ERROR_CORE; \
        goto errors; \
    } \
}

/*** SYSTICK functions ***/

/*******************************************************************/
SYSTICK_status_t SYSTICK_init(SYSTICK_core_t core, SYSTICK_clock_source_t clock_source, uint16_t main_clock_divider, uint8_t nvic_priority) {
    // Local variables.
    SYSTICK_status_t status = SYSTICK_SUCCESS;
    SYSCON_status_t syscon_status = SYSCON_SUCCESS;
    uint32_t clksel = 0;
    uint32_t clkoutdiv = 0;
    uint8_t clock_status = 0;
    uint32_t clock_frequency_hz = 0;
    uint32_t clock_divider = 1;
    // Check core.
    _SYSTICK_check_core();
    // Check parameters.
    if (clock_source >= SYSTICK_CLOCK_SOURCE_LAST) {
        status = SYSTICK_ERROR_CLOCK_SOURCE;
        goto errors;
    }
    if ((main_clock_divider < SYSTICK_MAIN_CLOCK_DIVIDER_MIN) || (main_clock_divider > SYSTICK_MAIN_CLOCK_DIVIDER_MAX)) {
        status = SYSTICK_ERROR_MAIN_CLOCK_DIVIDER;
        goto errors;
    }
    // Check state.
    if (systick_ctx.init_flag != 0) {
        status = SYSTICK_ERROR_ALREADY_INITIALIZED;
        goto errors;
    }
    // Stop timer.
    SYSTICK->CTRL &= ~(0b11 << 0);
    SYSCON->SYSTICKCLKDIVX[core] |= (0b1 << 30);
    // Get clock source status.
    syscon_status = SYSCON_get_status(SYSTICK_CLOCK_DESCRIPTOR[clock_source].clock, &clock_status);
    SYSCON_exit_error(SYSTICK_ERROR_BASE_SYSCON);
    // Check is clock source is ready.
    if (clock_status == 0) {
        status = SYSTICK_ERROR_CLOCK_SOURCE_READY;
        goto errors;
    }
    // Check source.
    if (clock_source == SYSTICK_CLOCK_SOURCE_MAIN_CLK_DIV) {
        // Configure divider.
        clock_divider = main_clock_divider;
        clkoutdiv = (SYSCON->SYSTICKCLKDIVX[core] & 0xFFFFFF00);
        clkoutdiv |= ((main_clock_divider - 1) & 0xFF);
        SYSCON->SYSTICKCLKDIVX[core] = clkoutdiv;
        SYSCON->SYSTICKCLKDIVX[core] &= ~(0b11 << 29);
    }
    // Select clock source.
    if (clock_source == SYSTICK_CLOCK_SOURCE_CPU) {
        SYSTICK->CTRL |= (0b1 << 2);
    }
    else {
        clksel = (SYSCON->SYSTICKCLKSELX[core] & 0xFFFFFFFE);
        clksel |= (SYSTICK_CLOCK_DESCRIPTOR[clock_source].clksel & 0b111);
        SYSCON->SYSTICKCLKSELX[core] = clksel;
        SYSTICK->CTRL &= ~(0b1 << 2);
    }
    // Get input clock frequency.
    syscon_status = SYSCON_get_frequency_hz(SYSTICK_CLOCK_DESCRIPTOR[clock_source].clock, &clock_frequency_hz);
    SYSCON_exit_error(SYSTICK_ERROR_BASE_SYSCON);
    // Update timer input clock.
    systick_ctx.clock_frequency_hz = (clock_frequency_hz / clock_divider);
    // Configure interrupt.
    NVIC_set_priority(NVIC_INTERRUPT_SYSTICK, nvic_priority);
    // Update initialization flag.
    systick_ctx.init_flag = 1;
errors:
    return status;
}

/*******************************************************************/
SYSTICK_status_t SYSTICK_de_init(SYSTICK_core_t core) {
    // Local variables.
    SYSTICK_status_t status = SYSTICK_SUCCESS;
    // Check core.
    _SYSTICK_check_core();
    // Check state.
    if (systick_ctx.init_flag == 0) {
        status = SYSTICK_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Select no clock.
    SYSCON->SYSTICKCLKSELX[core] |= 0x07;
    SYSCON->SYSTICKCLKDIVX[core] |= (0b1 << 30);
    // Disable timer and interrupt.
    SYSTICK->CTRL &= ~(0b111 << 0);
    // Update initialization flag.
    systick_ctx.init_flag = 0;
errors:
    return status;
}

/*******************************************************************/
SYSTICK_status_t SYSTICK_delay_microseconds(uint32_t delay_us) {
    // Local variables.
    SYSTICK_status_t status = SYSTICK_SUCCESS;
    uint64_t tmp_u64 = 0;
    uint32_t rvr = ((SYSTICK->LOAD) & (~SYSTICK_RELOAD_VALUE_MASK));
    // Check state.
    if (systick_ctx.init_flag == 0) {
        status = SYSTICK_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Compute minimum delay value.
    tmp_u64 = ((uint64_t) MATH_POWER_10[6] * (uint64_t) SYSTICK_RELOAD_VALUE_MIN);
    tmp_u64 /= ((uint64_t) systick_ctx.clock_frequency_hz);
    // Check delay range.
    if (delay_us < ((uint32_t) tmp_u64)) {
        status = SYSTICK_ERROR_DELAY_UNDERFLOW;
        goto errors;
    }
    // Compute maximum delay value.
    tmp_u64 = ((uint64_t) MATH_POWER_10[6] * (uint64_t) SYSTICK_RELOAD_VALUE_MAX);
    tmp_u64 /= ((uint64_t) systick_ctx.clock_frequency_hz);
    // Check delay range.
    if (delay_us > ((uint32_t) tmp_u64)) {
        status = SYSTICK_ERROR_DELAY_OVERFLOW;
        goto errors;
    }
    // Compute reload value.
    tmp_u64 = ((uint64_t) delay_us) * ((uint64_t) systick_ctx.clock_frequency_hz);
    tmp_u64 /= ((uint64_t) MATH_POWER_10[6]);
    rvr |= ((uint32_t) (tmp_u64 & SYSTICK_RELOAD_VALUE_MASK));
    SYSTICK->LOAD = rvr;
    // Clear counter.
    SYSTICK->VAL = 0;
    // Enable interrupt.
    NVIC_enable_interrupt(NVIC_INTERRUPT_SYSTICK);
    SYSTICK->CTRL |= (0b1 << 1);
    systick_ctx.irq_flag = 0;
    // Start timer.
    SYSTICK->CTRL |= (0b1 << 0);
    // Wait completion.
    while (systick_ctx.irq_flag == 0) {
        __asm volatile ("wfi");
    }
    // Stop timer.
    SYSTICK->CTRL &= ~(0b11 << 0);
    NVIC_disable_interrupt(NVIC_INTERRUPT_SYSTICK);
errors:
    return status;
}
