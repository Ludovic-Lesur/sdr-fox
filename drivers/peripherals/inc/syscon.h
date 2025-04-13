/*
 * syscon.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __SYSCON_H__
#define __SYSCON_H__

#include "anactrl.h"
#include "flash.h"
#include "gpio.h"
#include "pmc.h"
#include "error.h"
#include "types.h"

/*** SYSCON structures ***/

/*!******************************************************************
 * \enum SYSCON_status_t
 * \brief SYSCON driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    SYSCON_SUCCESS = 0,
    SYSCON_ERROR_NULL_PARAMETER,
    SYSCON_ERROR_CLOCK,
    SYSCON_ERROR_PERIPHERAL,
    SYSCON_ERROR_MAIN_CLOCK_A_SOURCE,
    SYSCON_ERROR_MAIN_CLOCK_B_SOURCE,
    SYSCON_ERROR_MAIN_CLOCK_READY,
    SYSCON_ERROR_SYSTEM_FREQUENCY_OVERFLOW,
    SYSCON_ERROR_XTAL_32K_TIMEOUT,
    SYSCON_ERROR_FRO_32K_TIMEOUT,
    SYSCON_ERROR_32K_OSC_TIMEOUT,
    SYSCON_ERROR_CLK_IN_TIMEOUT,
    SYSCON_ERROR_PLL,
    SYSCON_ERROR_PLL_BUSY,
    SYSCON_ERROR_PLL_CLOCK_SOURCE,
    SYSCON_ERROR_PLL_CLOCK_SOURCE_READY,
    SYSCON_ERROR_PLL_N,
    SYSCON_ERROR_PLL_INPUT_CLOCK_FREQUENCY_UNDERFLOW,
    SYSCON_ERROR_PLL_INPUT_CLOCK_FREQUENCY_OVERFLOW,
    SYSCON_ERROR_PLL_M,
    SYSCON_ERROR_PLL_CCO_CLOCK_FREQUENCY_UNDERFLOW,
    SYSCON_ERROR_PLL_CCO_CLOCK_FREQUENCY_OVERFLOW,
    SYSCON_ERROR_PLL_P,
    SYSCON_ERROR_PLL_OUTPUT_CLOCK_UNDERFLOW,
    SYSCON_ERROR_PLL_OUTPUT_CLOCK_OVERFLOW,
    SYSCON_ERROR_PLL_LOCK_TIMEOUT,
    SYSCON_ERROR_CLKOUT_DIVIDER,
    // Low level drivers errors.
    SYSCON_ERROR_BASE_ANACTRL = ERROR_BASE_STEP,
    SYSCON_ERROR_BASE_PMC = (SYSCON_ERROR_BASE_ANACTRL + ANACTRL_ERROR_BASE_LAST),
    SYSCON_ERROR_BASE_FLASH = (SYSCON_ERROR_BASE_PMC + PMC_ERROR_BASE_LAST),
    // Last base value.
    SYSCON_ERROR_BASE_LAST = (SYSCON_ERROR_BASE_FLASH + FLASH_ERROR_BASE_LAST)
} SYSCON_status_t;

/*!******************************************************************
 * \enum SYSCON_clock_t
 * \brief MCU clocks list.
 *******************************************************************/
typedef enum {
    SYSCON_CLOCK_NONE = 0,
    SYSCON_CLOCK_MAIN,
    SYSCON_CLOCK_MAIN_A,
    SYSCON_CLOCK_FRO_HF,
    SYSCON_CLOCK_FRO_HF_DIV,
    SYSCON_CLOCK_FRO_12M,
    SYSCON_CLOCK_FRO_1M,
    SYSCON_CLOCK_CLK_IN_XO32M,
    SYSCON_CLOCK_PLL0,
    SYSCON_CLOCK_PLL0_DIV,
    SYSCON_CLOCK_PLL1,
    SYSCON_CLOCK_FRO_32K,
    SYSCON_CLOCK_XTAL_32K,
    SYSCON_CLOCK_32K_OSC,
    SYSCON_CLOCK_LAST
} SYSCON_clock_t;

/*!******************************************************************
 * \enum SYSCON_peripheral_t
 * \brief MCU peripherals list.
 *******************************************************************/
typedef enum {
    SYSCON_PERIPHERAL_ROM = 0,
    SYSCON_PERIPHERAL_SRAM_CTRL1,
    SYSCON_PERIPHERAL_SRAM_CTRL2,
    SYSCON_PERIPHERAL_SRAM_CTRL3,
    SYSCON_PERIPHERAL_SRAM_CTRL4,
    SYSCON_PERIPHERAL_FLASH,
    SYSCON_PERIPHERAL_FMC,
    SYSCON_PERIPHERAL_MUX,
    SYSCON_PERIPHERAL_IOCON,
    SYSCON_PERIPHERAL_GPIO0,
    SYSCON_PERIPHERAL_GPIO1,
    SYSCON_PERIPHERAL_GPIO2,
    SYSCON_PERIPHERAL_GPIO3,
    SYSCON_PERIPHERAL_PINT,
    SYSCON_PERIPHERAL_GINT,
    SYSCON_PERIPHERAL_DMA0,
    SYSCON_PERIPHERAL_CRCGEN,
    SYSCON_PERIPHERAL_WWDT,
    SYSCON_PERIPHERAL_RTC,
    SYSCON_PERIPHERAL_MAILBOX,
    SYSCON_PERIPHERAL_SYSCON,
    SYSCON_PERIPHERAL_MRT,
    SYSCON_PERIPHERAL_OSTIMER,
    SYSCON_PERIPHERAL_SCT,
    SYSCON_PERIPHERAL_UTICK,
    SYSCON_PERIPHERAL_FLEXCOMM0,
    SYSCON_PERIPHERAL_FLEXCOMM1,
    SYSCON_PERIPHERAL_FLEXCOMM2,
    SYSCON_PERIPHERAL_FLEXCOMM3,
    SYSCON_PERIPHERAL_FLEXCOMM4,
    SYSCON_PERIPHERAL_FLEXCOMM5,
    SYSCON_PERIPHERAL_FLEXCOMM6,
    SYSCON_PERIPHERAL_FLEXCOMM7,
    SYSCON_PERIPHERAL_TIMER2,
    SYSCON_PERIPHERAL_USB0_DEV,
    SYSCON_PERIPHERAL_TIMER0,
    SYSCON_PERIPHERAL_TIMER1,
    SYSCON_PERIPHERAL_DMA1,
    SYSCON_PERIPHERAL_COMP,
    SYSCON_PERIPHERAL_SDIO,
    SYSCON_PERIPHERAL_USB1_HOST,
    SYSCON_PERIPHERAL_USB1_DEV,
    SYSCON_PERIPHERAL_USB1_RAM,
    SYSCON_PERIPHERAL_USB1_PHY,
    SYSCON_PERIPHERAL_FREQME,
    SYSCON_PERIPHERAL_RNG,
    SYSCON_PERIPHERAL_SYSCTL,
    SYSCON_PERIPHERAL_USB0_HOSTM,
    SYSCON_PERIPHERAL_USB0_HOSTS,
    SYSCON_PERIPHERAL_HASH_AES,
    SYSCON_PERIPHERAL_POWER_QUAD,
    SYSCON_PERIPHERAL_PLULUT,
    SYSCON_PERIPHERAL_TIMER3,
    SYSCON_PERIPHERAL_TIMER4,
    SYSCON_PERIPHERAL_PUF,
    SYSCON_PERIPHERAL_CASPER,
    SYSCON_PERIPHERAL_ANACTRL,
    SYSCON_PERIPHERAL_HS_LSPI,
    SYSCON_PERIPHERAL_GPIO_SEC,
    SYSCON_PERIPHERAL_GPIO_SEC_INT,
    SYSCON_PERIPHERAL_LAST
} SYSCON_peripheral_t;

/*!******************************************************************
 * \enum SYSCON_pll_t
 * \brief Embedded PLL list.
 *******************************************************************/
typedef enum {
    SYSCON_PLL0 = 0,
    SYSCON_PLL1,
    SYSCON_PLL_LAST
} SYSCON_pll_t;

/*!******************************************************************
 * \struct SYSCON_pll_configuration_t
 * \brief PLL parameters structure.
 *******************************************************************/
typedef struct {
    SYSCON_clock_t source;
    ANACTRL_xo32m_mode_t xo32m_mode;
    uint16_t m;
    uint8_t n;
    uint8_t p;
} SYSCON_pll_configuration_t;

/*** SYSCON functions ***/

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_init(void)
 * \brief Init MCU default clock tree.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_init(void);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_set_peripheral_clock(SYSCON_peripheral_t peripheral, uint8_t state)
 * \brief Set peripheral clock state.
 * \param[in]   peripheral: Peripheral to control.
 * \param[in]   state: Disable (0) or enable (otherwise) the peripheral clock.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_set_peripheral_clock(SYSCON_peripheral_t peripheral, uint8_t state);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_reset_peripheral(SYSCON_peripheral_t peripheral)
 * \brief Reset peripheral.
 * \param[in]   peripheral: Peripheral to reset.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_reset_peripheral(SYSCON_peripheral_t peripheral);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_configure_pll(SYSCON_pll_t pll, SYSCON_pll_configuration_t* pll_configuration)
 * \brief Configure PLL.
 * \param[in]   pll: PLL to configure.
 * \param[in]   pll_configuration: Pointer to the PLL parameters.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_configure_pll(SYSCON_pll_t pll, SYSCON_pll_configuration_t* pll_configuration);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_switch_system_clock(SYSCON_clock_t main_clock_a, SYSCON_clock_t main_clock)
 * \brief Switch system clock.
 * \param[in]   main_clock_a: Clock source selection for A multiplexer.
 * \param[in]   main_clock: Clock source selection for B multiplexer.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_switch_system_clock(SYSCON_clock_t main_clock_a, SYSCON_clock_t main_clock);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_get_frequency_hz(SYSCON_clock_t clock, uint32_t* frequency_hz)
 * \brief Get clock frequency.
 * \param[in]   clock: Clock to read.
 * \param[out]  frequency_hz: Pointer to the clock frequency in Hz.
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_get_frequency_hz(SYSCON_clock_t clock, uint32_t* frequency_hz);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_get_status(SYSCON_clock_t clock, uint8_t* clock_is_ready)
 * \brief Get clock status.
 * \param[in]   clock: Clock to read.
 * \param[out]  clock_is_ready: Pointer to the clock status (1 if the clock is running correctly, 0 otherwise).
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_get_status(SYSCON_clock_t clock, uint8_t* clock_is_ready);

/*!******************************************************************
 * \fn SYSCON_status_t SYSCON_set_clkout(SYSCON_clkout_source_t source, uint8_t divider, const GPIO_pin_t* clkout_gpio)
 * \brief Set clock output signal.
 * \param[in]   source: Clock to select.
 * \param[in]   divider: Clock output prescaler (1 to 256).
 * \param[in]   gpio: Optional GPIO to link to the CLKOUT signal.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSCON_status_t SYSCON_set_clkout(SYSCON_clock_t clock, uint16_t divider, const GPIO_pin_t* clkout_gpio);

/*******************************************************************/
#define SYSCON_exit_error(base) { ERROR_check_exit(syscon_status, SYSCON_SUCCESS, base) }

/*******************************************************************/
#define SYSCON_stack_error(base) { ERROR_check_stack(syscon_status, SYSCON_SUCCESS, base) }

/*******************************************************************/
#define SYSCON_stack_exit_error(base, code) { ERROR_check_stack_exit(syscon_status, SYSCON_SUCCESS, base, code) }

#endif /* __SYSCON_H__ */
