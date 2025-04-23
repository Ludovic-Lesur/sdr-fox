/*
 * terminal_hw.c
 *
 *  Created on: 23 apr. 2025
 *      Author: Ludo
 */

#include "terminal_hw.h"

#ifndef EMBEDDED_UTILS_DISABLE_FLAGS_FILE
#include "embedded_utils_flags.h"
#endif
#include "error.h"
#include "error_base.h"
#include "types.h"
#include "device/class/usbd_cdc.h"

#if (!(defined EMBEDDED_UTILS_TERMINAL_DRIVER_DISABLE) && (EMBEDDED_UTILS_TERMINAL_INSTANCES_NUMBER > 0))

/*** TERMINAL HW local structures ***/

/*******************************************************************/
typedef struct {
    TERMINAL_rx_irq_cb_t rx_irq_callback;
} TERMINAL_HW_context_t;

/*** TERMINAL HW local functions declaration ***/

static USB_status_t _TERMINAL_HW_set_serial_port_configuration(USB_CDC_serial_port_configuration_t* configuration);
static USB_status_t _TERMINAL_HW_get_serial_port_configuration(USB_CDC_serial_port_configuration_t* configuration);
static USB_status_t _TERMINAL_HW_set_serial_port_state(uint8_t rts, uint8_t dtr);
static USB_status_t _TERMINAL_HW_send_break(void);
static USB_status_t _TERMINAL_HW_rx_completion(uint8_t data);
static USB_status_t _TERMINAL_HW_tx_completion(void);

/*** TERMINAL HW local global variables ***/

static const USBD_CDC_callbacks_t TERMINAL_HW_CDC_CALLBACKS = {
    .set_serial_port_configuration_request = &_TERMINAL_HW_set_serial_port_configuration,
    .get_serial_port_configuration_request = &_TERMINAL_HW_get_serial_port_configuration,
    .set_serial_port_state = &_TERMINAL_HW_set_serial_port_state,
    .send_break = &_TERMINAL_HW_send_break,
    .rx_completion = &_TERMINAL_HW_rx_completion,
    .tx_completion = &_TERMINAL_HW_tx_completion
};

static TERMINAL_HW_context_t terminal_hw_ctx = {
    .rx_irq_callback = NULL
};

/*** TERMINAL HW local functions ***/

/*******************************************************************/
static USB_status_t _TERMINAL_HW_set_serial_port_configuration(USB_CDC_serial_port_configuration_t* configuration) {
    // Nothing to do as there is no physical UART connected to the CDC interface.
    UNUSED(configuration);
    return USB_SUCCESS;
}

/*******************************************************************/
static USB_status_t _TERMINAL_HW_get_serial_port_configuration(USB_CDC_serial_port_configuration_t* configuration) {
    // Nothing to do as there is no physical UART connected to the CDC interface.
    UNUSED(configuration);
    return USB_SUCCESS;
}

/*******************************************************************/
static USB_status_t _TERMINAL_HW_set_serial_port_state(uint8_t rts, uint8_t dtr) {
    // Nothing to do as there is no physical UART connected to the CDC interface.
    UNUSED(rts);
    UNUSED(dtr);
    return USB_SUCCESS;
}

/*******************************************************************/
static USB_status_t _TERMINAL_HW_send_break(void) {
    // Nothing to do as there is no physical UART connected to the CDC interface.
    return USB_SUCCESS;
}

/*******************************************************************/
static USB_status_t _TERMINAL_HW_rx_completion(uint8_t data) {
    // Call terminal callback.
    if (terminal_hw_ctx.rx_irq_callback != NULL) {
        terminal_hw_ctx.rx_irq_callback(data);
    }
    return USB_SUCCESS;
}

/*******************************************************************/
static USB_status_t _TERMINAL_HW_tx_completion(void) {
    return USB_SUCCESS;
}

/*** TERMINAL HW functions ***/

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_init(uint8_t instance, uint32_t baud_rate, TERMINAL_rx_irq_cb_t rx_irq_callback) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Unused parameters.
    UNUSED(instance);
    UNUSED(baud_rate);
    // Register callback.
    terminal_hw_ctx.rx_irq_callback = rx_irq_callback;
    // Init CDC class.
    usb_status = USBD_CDC_init((USBD_CDC_callbacks_t*) &TERMINAL_HW_CDC_CALLBACKS);
    USB_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_de_init(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
    // Reset callback.
    terminal_hw_ctx.rx_irq_callback = NULL;
    // Release CDC class.
    usb_status = USBD_CDC_de_init();
    USB_stack_error(ERROR_BASE_TERMINAL + TERMINAL_ERROR_BASE_HW_INTERFACE);
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_enable_rx(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
    // Nothing to do.
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_disable_rx(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
    // Nothing to do.
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_write(uint8_t instance, uint8_t* data, uint32_t data_size_bytes) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    USB_status_t usb_status = USB_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
    // Write data over USART.
    usb_status = USBD_CDC_write(data, data_size_bytes);
    USB_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
    return status;
}

#endif /* EMBEDDED_UTILS_TERMINAL_DRIVER_DISABLE */
