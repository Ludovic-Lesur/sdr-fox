/*
 * nvic.h
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#ifndef __NVIC_H__
#define __NVIC_H__

#include "types.h"

/*** NVIC interrupts vector ***/

/*!******************************************************************
 * \enum NVIC_interrupt_t
 * \brief NVIC interrupt vector.
 *******************************************************************/
typedef enum {
    NVIC_INTERRUPT_NMI = -14,
    NVIC_INTERRUPT_HARD_FAULT = -13,
    NVIC_INTERRUPT_MEMORY_MANAGEMENT = -12,
    NVIC_INTERRUPT_BUS_FAULT = -11,
    NVIC_INTERRUPT_USAGE_FAULT = -10,
    NVIC_INTERRUPT_SECURE_FAULT = -9,
    NVIC_INTERRUPT_SV_CALL = -5,
    NVIC_INTERRUPT_DEBUG_MONITOR = -4,
    NVIC_INTERRUPT_PEND_SV = -2,
    NVIC_INTERRUPT_SYSTICK = -1,
    NVIC_INTERRUPT_WDT_BOD = 0,
    NVIC_INTERRUPT_DMA0,
    NVIC_INTERRUPT_GINT0,
    NVIC_INTERRUPT_GINT1,
    NVIC_INTERRUPT_PIN_INT0,
    NVIC_INTERRUPT_PIN_INT1,
    NVIC_INTERRUPT_PIN_INT2,
    NVIC_INTERRUPT_PIN_INT3,
    NVIC_INTERRUPT_UTICK0,
    NVIC_INTERRUPT_MRT0,
    NVIC_INTERRUPT_CTIMER0,
    NVIC_INTERRUPT_CTIMER1,
    NVIC_INTERRUPT_SCT0,
    NVIC_INTERRUPT_CTIMER3,
    NVIC_INTERRUPT_FLEXCOMM0,
    NVIC_INTERRUPT_FLEXCOMM1,
    NVIC_INTERRUPT_FLEXCOMM2,
    NVIC_INTERRUPT_FLEXCOMM3,
    NVIC_INTERRUPT_FLEXCOMM4,
    NVIC_INTERRUPT_FLEXCOMM5,
    NVIC_INTERRUPT_FLEXCOMM6,
    NVIC_INTERRUPT_FLEXCOMM7,
    NVIC_INTERRUPT_ADC0,
    NVIC_INTERRUPT_RESERVED39,
    NVIC_INTERRUPT_ACMP,
    NVIC_INTERRUPT_RESERVED41,
    NVIC_INTERRUPT_RESERVED42,
    NVIC_INTERRUPT_USB0_NEEDCLK,
    NVIC_INTERRUPT_USB0,
    NVIC_INTERRUPT_RTC,
    NVIC_INTERRUPT_RESERVED46,
    NVIC_INTERRUPT_MAILBOX,
    NVIC_INTERRUPT_PIN_INT4,
    NVIC_INTERRUPT_PIN_INT5,
    NVIC_INTERRUPT_PIN_INT6,
    NVIC_INTERRUPT_PIN_INT7,
    NVIC_INTERRUPT_CTIMER2,
    NVIC_INTERRUPT_CTIMER4,
    NVIC_INTERRUPT_OS_EVENT,
    NVIC_INTERRUPT_RESERVED55,
    NVIC_INTERRUPT_RESERVED56,
    NVIC_INTERRUPT_RESERVED57,
    NVIC_INTERRUPT_SDIO,
    NVIC_INTERRUPT_RESERVED59,
    NVIC_INTERRUPT_RESERVED60,
    NVIC_INTERRUPT_RESERVED61,
    NVIC_INTERRUPT_USB1_PHY,
    NVIC_INTERRUPT_USB1,
    NVIC_INTERRUPT_USB1_NEEDCLK,
    NVIC_INTERRUPT_SEC_HYPERVISOR_CALL,
    NVIC_INTERRUPT_SEC_GPIO_INT0_IRQ0,
    NVIC_INTERRUPT_SEC_GPIO_INT0_IRQ1,
    NVIC_INTERRUPT_PLU,
    NVIC_INTERRUPT_SEC_VIO,
    NVIC_INTERRUPT_HASHCRYPT,
    NVIC_INTERRUPT_CASER,
    NVIC_INTERRUPT_PUF,
    NVIC_INTERRUPT_PQ,
    NVIC_INTERRUPT_DMA1,
    NVIC_INTERRUPT_FLEXCOMM8,
    NVIC_INTERRUPT_LAST
} NVIC_interrupt_t;

/*** NVIC functions ***/

/*!******************************************************************
 * \fn void NVIC_init(void)
 * \brief Init interrupts vector.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void NVIC_init(void);

/*!******************************************************************
 * \fn void NVIC_enable_interrupt(NVIC_interrupt_t irq_index)
 * \brief Enable interrupt.
 * \param[in]   irq_index: Interrupt to enable.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void NVIC_enable_interrupt(NVIC_interrupt_t irq_index);

/*!******************************************************************
 * \fn void NVIC_disable_interrupt(NVIC_interrupt_t irq_index)
 * \brief Disable interrupt.
 * \param[in]   irq_index: Interrupt to enable.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void NVIC_disable_interrupt(NVIC_interrupt_t irq_index);

/*!******************************************************************
 * \fn void NVIC_set_priority(NVIC_interrupt_t irq_index, uint8_t priority)
 * \brief Set interrupt priority.
 * \param[in]   irq_index: Interrupt to configure.
 * \param[in]   priority: Interrupt priority to set.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void NVIC_set_priority(NVIC_interrupt_t irq_index, uint8_t priority);

/*!******************************************************************
 * \fn void NVIC_clear_pending_interrupt(NVIC_interrupt_t irq_index);
 * \brief Clear pending interrupt.
 * \param[in]   irq_index: Interrupt to clear.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void NVIC_clear_pending_interrupt(NVIC_interrupt_t irq_index);

#endif /* __NVIC_H__ */
