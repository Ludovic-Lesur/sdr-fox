/*
 * pmc.h
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#ifndef __PMC_H__
#define __PMC_H__

#include "error.h"
#include "types.h"

/*** PMC structures ***/

/*!******************************************************************
 * \enum PMC_status_t
 * \brief PMC driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    PMC_SUCCESS = 0,
    PMC_ERROR_NULL_PARAMETER,
    PMC_ERROR_BLOCK,
    PMC_ERROR_OSCILLATOR,
    PMC_ERROR_DCDC_POWER_PROFILE,
    // Last base value.
    PMC_ERROR_BASE_LAST = ERROR_BASE_STEP
} PMC_status_t;

/*!******************************************************************
 * \enum PMC_block_t
 * \brief PMC power blocks list.
 *******************************************************************/
typedef enum {
    PMC_BLOCK_BODVBAT = 0,
    PMC_BLOCK_FRO_12M,
    PMC_BLOCK_FRO_32K,
    PMC_BLOCK_XTAL_32K,
    PMC_BLOCK_XTAL_32M,
    PMC_BLOCK_PLL0,
    PMC_BLOCK_PLL1,
    PMC_BLOCK_USB_FS_PHY,
    PMC_BLOCK_USB_HS_PHY,
    PMC_BLOCK_COMP,
    PMC_BLOCK_LDO_USB_HS,
    PMC_BLOCK_AUX_BIAS,
    PMC_BLOCK_LDO_XO_32M,
    PMC_BLOCK_RNG,
    PMC_BLOCK_PLL0_SSCG,
    PMC_BLOCK_LAST
} PMC_block_t;

/*!******************************************************************
 * \enum PMC_32k_osc_t
 * \brief Low speed clocks list.
 *******************************************************************/
typedef enum {
    PMC_OSCILLATOR_FRO32K = 0,
    PMC_OSCILLATOR_XTAL32K,
    PMC_OSCILLATOR_LAST
} PMC_oscillator_t;

/*** PMC functions ***/

/*!******************************************************************
 * \fn PMC_status_t PMC_configure_dc_dc(uint32_t system_clock_frequency_hz)
 * \brief Configure internal DC-DC converter according to the system clock frequency.
 * \param[in]   system_clock_frequency_hz: System clock frequency in Hz.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
PMC_status_t PMC_configure_dc_dc(uint32_t system_clock_frequency_hz);

/*!******************************************************************
 * \fn PMC_status_t PMC_set_power(PMC_block_t block, uint8_t state)
 * \brief Set power block state.
 * \param[in]   block: Power block to control.
 * \param[in]   state: Disable (0) or enable (otherwise) the power block.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
PMC_status_t PMC_set_power(PMC_block_t block, uint8_t state);

/*!******************************************************************
 * \fn PMC_status_t PMC_set_32k_osc_source(PMC_oscillator_t source)
 * \brief Set low speed oscillator source.
 * \param[in]   source: Clock source to select.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
PMC_status_t PMC_set_32k_osc_source(PMC_oscillator_t source);

/*!******************************************************************
 * \fn PMC_status_t PMC_get_32k_osc_status(PMC_oscillator_t oscillator, uint8_t* oscillator_is_ready)
 * \brief Get low speed crystal oscillator status.
 * \param[in]   oscillator: Clock source to check.
 * \param[out]  oscillator_is_ready: Pointer to the oscillator status.
 * \retval      Function execution status.
 *******************************************************************/
PMC_status_t PMC_get_oscillator_status(PMC_oscillator_t oscillator, uint8_t* oscillator_is_ready);

/*!******************************************************************
 * \fn PMC_status_t PMC_get_32k_osc_status(uint8_t* oscillator_is_ready)
 * \brief Get low speed crystal oscillator status.
 * \param[in]   none
 * \param[out]  oscillator_is_ready: Pointer to the oscillator status.
 * \retval      Function execution status.
 *******************************************************************/
PMC_status_t PMC_get_32k_osc_status(uint8_t* oscillator_is_ready);

/*******************************************************************/
#define PMC_exit_error(base) { ERROR_check_exit(pmc_status, PMC_SUCCESS, base) }

/*******************************************************************/
#define PMC_stack_error(base) { ERROR_check_stack(pmc_status, PMC_SUCCESS, base) }

/*******************************************************************/
#define PMC_stack_exit_error(base, code) { ERROR_check_stack_exit(pmc_status, PMC_SUCCESS, base, code) }

#endif /* __PMC_H__ */
