/*
 * nvic.c
 *
 *  Created on: 11 mar. 2023
 *      Author: Ludo
 */

#include "nvic.h"

#include "nvic_registers.h"
#include "scb_registers.h"
#include "types.h"

/*** NVIC linker generated symbols ***/

extern uint32_t __Vectors;

/*** NVIC local macros ***/

#define NVIC_PRIORITY_MIN   7

/*** NVIC functions ***/

/*******************************************************************/
void NVIC_init(void) {
    SCB->VTOR = (uint32_t) &__Vectors;
}

/*******************************************************************/
void NVIC_enable_interrupt(NVIC_interrupt_t irq_index) {
    // Check index.
    if (irq_index >= NVIC_INTERRUPT_LAST) goto errors;
    // Enable interrupt.
    NVIC->ISER[irq_index >> 5] = (0b1 << (irq_index & 0x1F));
errors:
    return;
}

/*******************************************************************/
void NVIC_disable_interrupt(NVIC_interrupt_t irq_index) {
    // Check index.
    if (irq_index >= NVIC_INTERRUPT_LAST) goto errors;
    // Disable interrupt.
    NVIC->ICER[irq_index >> 5] = (0b1 << (irq_index & 0x1F));
errors:
    return;
}

/*******************************************************************/
void NVIC_set_priority(NVIC_interrupt_t irq_index, uint8_t priority) {
    // Check index.
    if (irq_index >= NVIC_INTERRUPT_LAST) goto errors;
    // Clamp parameter.
    if (priority > NVIC_PRIORITY_MIN) {
        priority = NVIC_PRIORITY_MIN;
    }
    // Set priority.
    if (irq_index >= 0) {
        NVIC->IPR[irq_index] = (priority << 5);
    }
    else {
        SCB->SHPR[(irq_index & 0x0F) - 4] = (priority << 5);
    }
errors:
    return;
}

/*******************************************************************/
void NVIC_clear_pending_interrupt(NVIC_interrupt_t irq_index) {
    // Check index.
    if (irq_index >= NVIC_INTERRUPT_LAST) goto errors;
    // Clear interrupt.
    NVIC->ICPR[irq_index >> 5] = (0b1 << (irq_index & 0x1F));
errors:
    return;
}
