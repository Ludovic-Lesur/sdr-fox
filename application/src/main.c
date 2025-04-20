/*
 * main.c
 *
 *  Created on: 8 mar. 2023
 *      Author: Ludo
 */

#include "error.h"
#include "error_base.h"
#include "gpio.h"
#include "mcu_mapping.h"
#include "nvic.h"
#include "nvic_priority.h"
#include "syscon.h"
#include "systick.h"
#include "types.h"
#include "usb_device_sdr_fox.h"

/*** MAIN functions ***/

/*******************************************************************/
static void _SDR_FOX_init_hw(void) {
    // Local variables.
    SYSCON_status_t syscon_status = SYSCON_SUCCESS;
    SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
    USB_DEVICE_SDR_FOX_status_t usb_device_sdr_fox_status = USB_DEVICE_SDR_FOX_SUCCESS;
    SYSCON_pll_configuration_t pll1_config;
    // Init error stack
    ERROR_stack_init();
    // Init memory.
    NVIC_init();
    // Init GPIOs.
    GPIO_init();
    // Init clock tree.
    syscon_status = SYSCON_init();
    SYSCON_stack_error(ERROR_BASE_SYSCON);
    syscon_status = SYSCON_set_clkout(SYSCON_CLOCK_MAIN, 100, &GPIO_CLKOUT);
    SYSCON_stack_error(ERROR_BASE_SYSCON);
    // Switch to PLL.
    pll1_config.source = SYSCON_CLOCK_CLK_IN_XO32M;
    pll1_config.xo32m_mode = ANACTRL_XO32M_MODE_OSCILLATOR;
    pll1_config.m = 100;
    pll1_config.n = 4;
    pll1_config.p = 4;
    syscon_status = SYSCON_configure_pll(SYSCON_PLL1, &pll1_config);
    SYSCON_stack_error(ERROR_BASE_SYSCON);
    syscon_status = SYSCON_switch_system_clock(SYSCON_CLOCK_FRO_12M, SYSCON_CLOCK_PLL1);
    SYSCON_stack_error(ERROR_BASE_SYSCON);
    // Init delay timer.
    systick_status = SYSTICK_init(SYSTICK_CORE_CPU0, SYSTICK_CLOCK_SOURCE_32K_OSC, 1, NVIC_PRIORITY_SYSTICK);
    SYSTICK_stack_error(ERROR_BASE_SYSTICK);
    // Init USB interface.
    usb_device_sdr_fox_status = USB_DEVICE_SDR_FOX_init();
    USB_DEVICE_SDR_FOX_stack_error(ERROR_BASE_USB_DEVICE_SDR_FOX);
}

/*******************************************************************/
int main(void) {
    // Local variables.
    USB_DEVICE_SDR_FOX_status_t usb_device_sdr_fox_status = USB_DEVICE_SDR_FOX_SUCCESS;
    // Init board.
    _SDR_FOX_init_hw();
    // Configure LED pin.
    GPIO_configure(&GPIO_LED_RED, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_LED_GREEN, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_LED_BLUE, GPIO_MODE_DIGITAL_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SLEW_RATE_STANDARD, GPIO_PULL_NONE);
    GPIO_write(&GPIO_LED_RED, 1);
    GPIO_write(&GPIO_LED_GREEN, 1);
    GPIO_write(&GPIO_LED_BLUE, 1);
    // Start USB device.
    usb_device_sdr_fox_status = USB_DEVICE_SDR_FOX_start();
    USB_DEVICE_SDR_FOX_stack_error(ERROR_BASE_USB_DEVICE_SDR_FOX);
    // Main loop.
    while (1);
    return 0;
}
