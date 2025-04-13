/*
 * systick.h
 *
 *  Created on: 14 mar. 2023
 *      Author: Ludo
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "error.h"
#include "syscon.h"
#include "types.h"

/*** SYSTICK structures ***/

/*!******************************************************************
 * \enum SYSTICK_status_t
 * \brief SYSTICK driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    SYSTICK_SUCCESS = 0,
    SYSTICK_ERROR_CORE,
    SYSTICK_ERROR_ALREADY_INITIALIZED,
    SYSTICK_ERROR_UNINITIALIZED,
    SYSTICK_ERROR_CLOCK_SOURCE,
    SYSTICK_ERROR_CLOCK_SOURCE_READY,
    SYSTICK_ERROR_MAIN_CLOCK_DIVIDER,
    SYSTICK_ERROR_DELAY_OVERFLOW,
    SYSTICK_ERROR_DELAY_UNDERFLOW,
    // Low level drivers errors.
    SYSTICK_ERROR_BASE_SYSCON = ERROR_BASE_STEP,
    // Last base value.
    SYSTICK_ERROR_BASE_LAST = (SYSTICK_ERROR_BASE_SYSCON + SYSCON_ERROR_BASE_LAST)
} SYSTICK_status_t;

/*!******************************************************************
 * \enum SYSTICK_core_t
 * \brief SYSTICK cores list.
 *******************************************************************/
typedef enum {
    SYSTICK_CORE_CPU0 = 0,
    SYSTICK_CORE_CPU1,
    SYSTICK_CORE_LAST
} SYSTICK_core_t;

/*!******************************************************************
 * \enum SYSTICK_clock_source_t
 * \brief SYSTICK cores list.
 *******************************************************************/
typedef enum {
    SYSTICK_CLOCK_SOURCE_CPU = 0,
    SYSTICK_CLOCK_SOURCE_MAIN_CLK_DIV,
    SYSTICK_CLOCK_SOURCE_FRO_1M,
    SYSTICK_CLOCK_SOURCE_32K_OSC,
    SYSTICK_CLOCK_SOURCE_LAST
} SYSTICK_clock_source_t;

/*** SYSTICK functions ***/

/*!******************************************************************
 * \fn SYSTICK_status_t SYSTICK_init(SYSTICK_core_t core, uint8_t nvic_priority)
 * \brief Init system tick peripheral for delay operation.
 * \param[in]   core: MCU core which is calling the function.
 * \param[in]   clock_source: Timer clock source.
 * \param[in]   main_clock_divider: Input prescaler in case of SYSTICK_CLOCK_SOURCE_MAIN_CLK_DIV.
 * \param[in]   nvic_priority: Interrupt priority.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSTICK_status_t SYSTICK_init(SYSTICK_core_t core, SYSTICK_clock_source_t clock_source, uint16_t main_clock_divider, uint8_t nvic_priority);

/*!******************************************************************
 * \fn SYSTICK_status_t SYSTICK_de_init(SYSTICK_core_t core)
 * \brief Release system tick peripheral.
 * \param[in]   core: MCU core which is calling the function.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSTICK_status_t SYSTICK_de_init(SYSTICK_core_t core);

/*!******************************************************************
 * \fn SYSTICK_status_t SYSTICK_delay_microseconds(uint32_t delay_us)
 * \brief Delay function.
 * \param[in]   delay_us: Delay to wait in microseconds.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
SYSTICK_status_t SYSTICK_delay_microseconds(uint32_t delay_us);

/*******************************************************************/
#define SYSTICK_exit_error(base) { ERROR_check_exit(systick_status, SYSTICK_SUCCESS, base) }

/*******************************************************************/
#define SYSTICK_stack_error(base) { ERROR_check_stack(systick_status, SYSTICK_SUCCESS, base) }

/*******************************************************************/
#define SYSTICK_stack_exit_error(base, code) { ERROR_check_stack_exit(systick_status, SYSTICK_SUCCESS, base, code) }

#endif /* __SYSTICK_H__ */
