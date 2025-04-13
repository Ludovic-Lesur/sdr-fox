/*
 * usb_hs_phy.c
 *
 *  Created on: 13 mar. 2023
 *      Author: Ludo
 */

#include "usb_hs_phy.h"

#include "anactrl.h"
#include "error.h"
#include "nvic.h"
#include "pmc.h"
#include "syscon.h"
#include "systick.h"
#include "usb_hs_phy_registers.h"

/*** USB local macros ***/

#define USB_HS_PHY_PLL_DELAY_US     10000

#define USB_HS_PHY_D_CAL            0x05
#define USB_HS_PHY_TXCAL45DM        0x0A
#define USB_HS_PHY_TXCAL45DP        0x0A

/*** USB HS PHY functions ***/

/*******************************************************************/
USB_HS_PHY_status_t USB_HS_PHY_init(void) {
    // Local variables.
    USB_HS_PHY_status_t status = USB_HS_PHY_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    ANACTRL_status_t anactrl_status = ANACTRL_SUCCESS;
    SYSCON_status_t syscon_status = SYSCON_SUCCESS;
    SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
    uint32_t reg_value = 0;
    // Turn PHY on.
    pmc_status = PMC_set_power(PMC_BLOCK_LDO_USB_HS, 1);
    PMC_exit_error(USB_HS_PHY_ERROR_BASE_PMC);
    pmc_status = PMC_set_power(PMC_BLOCK_USB_HS_PHY, 1);
    PMC_exit_error(USB_HS_PHY_ERROR_BASE_PMC);
    // Enable clock.
    anactrl_status = ANACTRL_set_xo32m_output(ANACTRL_XO32M_OUTPUT_USB_HS_PLL, 1);
    ANACTRL_exit_error(USB_HS_PHY_ERROR_BASE_ANACTRL);
    syscon_status = SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_PHY, 1);
    SYSCON_exit_error(USB_HS_PHY_ERROR_BASE_SYSCON);
    // Reset peripheral.
    syscon_status = SYSCON_reset_peripheral(SYSCON_PERIPHERAL_USB1_PHY);
    SYSCON_exit_error(USB_HS_PHY_ERROR_BASE_SYSCON);
    USB_HS_PHY->CTRL_CLR = (0b1 << 31);
    // Clear pending interrupts.
    NVIC_clear_pending_interrupt(NVIC_INTERRUPT_USB1_PHY);
    // Set PLL for 16MHz crystal.
    reg_value = (USB_HS_PHY->PLL_SIC);
    reg_value &= ~(0b111 << 22);
    reg_value |= (0b1 << 13) | (0b110 << 22);
    USB_HS_PHY->PLL_SIC = reg_value;
    USB_HS_PHY->PLL_SIC_SET = (0b1 << 21);
    USB_HS_PHY->PLL_SIC_CLR = (0b1 << 16) | (0b11 << 25); // Reserved bits to be cleared.
    // PLL delay.
    systick_status = SYSTICK_delay_microseconds(USB_HS_PHY_PLL_DELAY_US);
    SYSTICK_exit_error(USB_HS_PHY_ERROR_BASE_SYSTICK);
    USB_HS_PHY->PLL_SIC_SET = (0b1 << 12);
    USB_HS_PHY->PLL_SIC_SET = (0b1 << 6);
    // Enable auto power-down.
    USB_HS_PHY->CTRL_CLR = (0b1 << 30);
    USB_HS_PHY->PWD = 0;
    // Configure transmitter.
    reg_value = (USB_HS_PHY->TX);
    reg_value &= 0xFFD0D0F0;
    reg_value |= (USB_HS_PHY_D_CAL << 0) | (USB_HS_PHY_TXCAL45DM << 8) | (USB_HS_PHY_TXCAL45DP << 16);
    USB_HS_PHY->TX = reg_value;
    // Support LS and FS device.
    USB_HS_PHY->CTRL_SET = (0b1 << 15);
    USB_HS_PHY->CTRL_SET = (0b1 << 14);
    USB_HS_PHY->CTRL_SET = (0b1 << 19);
    // Enable auto-resume.
    USB_HS_PHY->CTRL_SET = (0b1 << 18);
    USB_HS_PHY->CTRL_SET = (0b1 << 20);
errors:
    return status;
}

/*******************************************************************/
USB_HS_PHY_status_t USB_HS_PHY_de_init(void) {
    // Local variables.
    USB_HS_PHY_status_t status = USB_HS_PHY_SUCCESS;
    SYSCON_status_t syscon_status = SYSCON_SUCCESS;
    PMC_status_t pmc_status = PMC_SUCCESS;
    // Disable peripheral clock.
    syscon_status = SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_PHY, 0);
    SYSCON_exit_error(USB_HS_PHY_ERROR_BASE_SYSCON);
    // Turn PHY off.
    pmc_status = PMC_set_power(PMC_BLOCK_USB_HS_PHY, 0);
    PMC_exit_error(USB_HS_PHY_ERROR_BASE_PMC);
    pmc_status = PMC_set_power(PMC_BLOCK_LDO_USB_HS, 0);
    PMC_exit_error(USB_HS_PHY_ERROR_BASE_PMC);
errors:
    return status;
}
