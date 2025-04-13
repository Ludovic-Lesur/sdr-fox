/*
 * anactrl.c
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#include "anactrl.h"

#include "anactrl_registers.h"
#include "syscon.h"
#include "types.h"

/*** ANACTRL functions ***/

/*******************************************************************/
ANACTRL_status_t ANACTRL_set_fro_state(ANACTRL_fro_t fro, uint8_t state) {
    // Local variables.
    ANACTRL_status_t status = ANACTRL_SUCCESS;
    // Check oscillator.
    switch (fro) {
    case ANACTRL_FRO_12M:
        // Check state.
        if (state == 0) {
            ANACTRL->FRO192M_CTRL &= ~(0b1 << 14);
        }
        else {
            ANACTRL->FRO192M_CTRL |= (0b1 << 14);
        }
        break;
    case ANACTRL_FRO_HF:
        // Check state.
        if (state == 0) {
            ANACTRL->FRO192M_CTRL &= ~(0b1 << 30);
        }
        else {
            ANACTRL->FRO192M_CTRL |= (0b1 << 30);
        }
        break;
    default:
        status = ANACTRL_ERROR_FRO_TYPE;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
ANACTRL_status_t ANACTRL_get_fro_status(ANACTRL_fro_t fro, uint8_t* fro_is_ready) {
    // Local variables.
    ANACTRL_status_t status = ANACTRL_SUCCESS;
    uint8_t fro_192m_status = (((ANACTRL->FRO192M_STATUS) >> 0) & 0b1);
    uint8_t fro_enabled = 0;
    // Check parameter.
    if (fro_is_ready == NULL) {
        status = ANACTRL_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Check oscillator.
    switch (fro) {
    case ANACTRL_FRO_12M:
        fro_enabled = (((ANACTRL->FRO192M_CTRL) >> 14) & 0b1);
        break;
    case ANACTRL_FRO_HF:
        fro_enabled = (((ANACTRL->FRO192M_CTRL) >> 30) & 0b1);
        break;
    default:
        status = ANACTRL_ERROR_FRO_TYPE;
        goto errors;
    }
    (*fro_is_ready) = (fro_enabled & fro_192m_status);
errors:
    return status;
}

/*******************************************************************/
ANACTRL_status_t ANACTRL_set_xo32m_mode(ANACTRL_xo32m_mode_t xo32m_mode) {
    // Local variables.
    ANACTRL_status_t status = ANACTRL_SUCCESS;
    // Check mode.
    switch (xo32m_mode) {
    case ANACTRL_XO32M_MODE_OSCILLATOR:
        // Disable bypass.
        ANACTRL->XO32M_CTRL &= ~(0b1 << 22);
        break;
    case ANACTRL_XO32M_MODE_BYPASS:
        // Enable bypass.
        ANACTRL->XO32M_CTRL |= (0b1 << 22);
        break;
    default:
        status = ANACTRL_ERROR_XO32M_MODE;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
ANACTRL_status_t ANACTRL_set_xo32m_output(ANACTRL_xo32m_output_t xo32m_output, uint8_t state) {
    // Local variables.
    ANACTRL_status_t status = ANACTRL_SUCCESS;
    // Check output.
    switch (xo32m_output) {
    case ANACTRL_XO32M_OUTPUT_SYSTEM_CLOCK:
        // Check state.
        if (state == 0) {
            ANACTRL->XO32M_CTRL &= ~(0b1 << 24);
        }
        else {
            ANACTRL->XO32M_CTRL |= (0b1 << 24);
        }
        break;
    case ANACTRL_XO32M_OUTPUT_USB_HS_PLL:
        // Check state.
        if (state == 0) {
            ANACTRL->XO32M_CTRL &= ~(0b1 << 23);
        }
        else {
            ANACTRL->XO32M_CTRL |= (0b1 << 23);
        }
        break;
    default:
        status = ANACTRL_ERROR_XO32M_OUTPUT;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
ANACTRL_status_t ANACTRL_get_xo32m_status(uint8_t* xo32m_is_ready) {
    // Local variables.
    ANACTRL_status_t status = ANACTRL_SUCCESS;
    // Check parameter.
    if (xo32m_is_ready == NULL) {
        status = ANACTRL_ERROR_NULL_PARAMETER;
        goto errors;
    }
    (*xo32m_is_ready) = (((ANACTRL->XO32M_STATUS) >> 0) & 0b1);
errors:
    return status;
}
