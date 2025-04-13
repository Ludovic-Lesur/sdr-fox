/*
 * anactrl.h
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#ifndef __ANACTRL_H__
#define __ANACTRL_H__

#include "error.h"
#include "types.h"

/*** ANACTRL structures ***/

/*!******************************************************************
 * \enum ANACTRL_status_t
 * \brief ANACTRL driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    ANACTRL_SUCCESS = 0,
    ANACTRL_ERROR_NULL_PARAMETER,
    ANACTRL_ERROR_FRO_TYPE,
    ANACTRL_ERROR_XO32M_MODE,
    ANACTRL_ERROR_XO32M_OUTPUT,
    // Last base value.
    ANACTRL_ERROR_BASE_LAST = ERROR_BASE_STEP
} ANACTRL_status_t;

/*!******************************************************************
 * \enum ANACTRL_fro_t
 * \brief Free running oscillators list.
 *******************************************************************/
typedef enum {
    ANACTRL_FRO_12M = 0,
    ANACTRL_FRO_HF,
    ANACTRL_FRO_LAST
} ANACTRL_fro_t;

/*!******************************************************************
 * \enum ANACTRL_xo32m_mode_t
 * \brief Crystal oscillator modes.
 *******************************************************************/
typedef enum {
    ANACTRL_XO32M_MODE_OSCILLATOR = 0,
    ANACTRL_XO32M_MODE_BYPASS,
    ANACTRL_XO32M_MODE_LAST
} ANACTRL_xo32m_mode_t;

/*!******************************************************************
 * \enum ANACTRL_xo32m_output_t
 * \brief Crystal oscillator output signals list.
 *******************************************************************/
typedef enum {
    ANACTRL_XO32M_OUTPUT_SYSTEM_CLOCK = 0,
    ANACTRL_XO32M_OUTPUT_USB_HS_PLL,
    ANACTRL_XO32M_OUTPUT_LAST
} ANACTRL_xo32m_output_t;

/*** ANACTRL functions ***/

/*!******************************************************************
 * \fn ANACTRL_status_t ANACTRL_set_fro_state(ANACTRL_fro_t fro, uint8_t state)
 * \brief Set free running oscillator state.
 * \param[in]   fro: Oscillator to control.
 * \param[in]   state: Disable (0) or enable (otherwise) the oscillator.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
ANACTRL_status_t ANACTRL_set_fro_state(ANACTRL_fro_t fro, uint8_t state);

/*!******************************************************************
 * \fn ANACTRL_status_t ANACTRL_get_fro_status(ANACTRL_fro_t fro, uint8_t* fro_is_ready)
 * \brief Get free running oscillator status.
 * \param[in]   fro: Oscillator to check.
 * \param[out]  fro_is_ready: Pointer to the oscillator status.
 * \retval      Function execution status.
 *******************************************************************/
ANACTRL_status_t ANACTRL_get_fro_status(ANACTRL_fro_t fro, uint8_t* fro_is_ready);

/*!******************************************************************
 * \fn ANACTRL_status_t ANACTRL_set_xo32m_mode(ANACTRL_xo32m_mode_t xo32m_mode)
 * \brief Configure the external crystal oscillator.
 * \param[in]   xo32m_mode: Oscillator mode.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
ANACTRL_status_t ANACTRL_set_xo32m_mode(ANACTRL_xo32m_mode_t xo32m_mode);

/*!******************************************************************
 * \fn ANACTRL_status_t ANACTRL_set_xo32m_mode(ANACTRL_xo32m_mode_t xo32m_mode)
 * \brief Configure the external crystal oscillator.
 * \param[in]   xo32m_mode: Oscillator mode.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
ANACTRL_status_t ANACTRL_set_xo32m_output(ANACTRL_xo32m_output_t xo32m_output, uint8_t state);

/*!******************************************************************
 * \fn ANACTRL_status_t ANACTRL_get_xo32m_status(uint8_t* xo32m_is_ready)
 * \brief Get high speed crystal oscillator status.
 * \param[in]   none
 * \param[out]  xo32m_is_ready: Pointer to the oscillator status.
 * \retval      Function execution status.
 *******************************************************************/
ANACTRL_status_t ANACTRL_get_xo32m_status(uint8_t* xo32m_is_ready);

/*******************************************************************/
#define ANACTRL_exit_error(base) { ERROR_check_exit(anactrl_status, ANACTRL_SUCCESS, base) }

/*******************************************************************/
#define ANACTRL_stack_error(base) { ERROR_check_stack(anactrl_status, ANACTRL_SUCCESS, base) }

/*******************************************************************/
#define ANACTRL_stack_exit_error(base, code) { ERROR_check_stack_exit(anactrl_status, ANACTRL_SUCCESS, base, code) }

#endif /* __ANACTRL_H__ */
