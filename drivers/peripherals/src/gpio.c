/*
 * gpio.c
 *
 *  Created on: 10 mar. 2023
 *      Author: Ludo
 */

#include "gpio.h"

#include "gpio_registers.h"
#include "iocon_registers.h"
#include "syscon.h"

/*** GPIO local functions ***/

/*******************************************************************/
static void _GPIO_set_mode(const GPIO_pin_t* gpio, GPIO_mode_t mode) {
    // Local variables.
    uint32_t reg_value = 0;
    // Read register.
    reg_value = IOCON->PIO[gpio->port][gpio->pin];
    // Reset ASW, DIGIMODE and INVERT bits by default.
    reg_value &= 0xFFFFFA7F;
    // Set required bits.
    switch (mode) {
    case GPIO_MODE_ANALOG_INPUT:
        // Set as input.
        GPIO->DIR[gpio->port] &= ~(0b1 << (gpio->pin));
        // Enable analog switch.
        reg_value |= (0b1 << 10);
        break;
    case GPIO_MODE_DIGITAL_INPUT:
        // Set as input.
        GPIO->DIR[gpio->port] &= ~(0b1 << (gpio->pin));
        // Enable digital mode.
        reg_value |= (0b1 << 8);
        break;
    case GPIO_MODE_DIGITAL_INPUT_INVERTED:
        // Set as input.
        GPIO->DIR[gpio->port] &= ~(0b1 << (gpio->pin));
        // Enable digital mode and inverter.
        reg_value |= (0b11 << 7);
        break;
    case GPIO_MODE_DIGITAL_OUTPUT:
        // Set as output.
        GPIO->DIR[gpio->port] |= (0b1 << (gpio->pin));
        // Enable digital mode.
        reg_value |= (0b1 << 8);
        break;
    default:
        break;
    }
    // Write register.
    IOCON->PIO[gpio->port][gpio->pin] = reg_value;
}

/*******************************************************************/
static void _GPIO_set_output_type(const GPIO_pin_t* gpio, GPIO_output_type_t output_type) {
    // Set bit.
    switch (output_type) {
    case GPIO_TYPE_PUSH_PULL:
        // OD = '0'.
        IOCON->PIO[gpio->port][gpio->pin] &= ~(0b1 << 9);
        break;
    case GPIO_TYPE_OPEN_DRAIN:
        // OD = '1'.
        IOCON->PIO[gpio->port][gpio->pin] |= (0b1 << 9);
        break;
    default:
        break;
    }
}

/*******************************************************************/
static void _GPIO_set_slew_rate(const GPIO_pin_t* gpio, GPIO_slew_rate_t slew_rate) {
    // Set bit.
    switch (slew_rate) {
    case GPIO_SLEW_RATE_STANDARD:
        // OD = '0'.
        IOCON->PIO[gpio->port][gpio->pin] &= ~(0b1 << 6);
        break;
    case GPIO_SLEW_RATE_FAST:
        // OD = '1'.
        IOCON->PIO[gpio->port][gpio->pin] |= (0b1 << 6);
        break;
    default:
        break;
    }
}

/*******************************************************************/
static void _GPIO_set_pull_resistor(const GPIO_pin_t* gpio, GPIO_pull_resistor_t pull_resistor) {
    // Local variables.
    uint32_t reg_value = 0;
    // Read register.
    reg_value = IOCON->PIO[gpio->port][gpio->pin];
    // Disable resistors by default.
    reg_value &= ~(0b11 << 4);
    // Set required bits.
    switch (pull_resistor) {
    case GPIO_PULL_NONE:
        // Nothing to do.
        break;
    case GPIO_PULL_DOWN:
        // MODE = '01'.
        reg_value |= (0b01 << 4);
        break;
    case GPIO_PULL_UP:
        // MODE = '10'.
        reg_value |= (0b10 << 4);
        break;
    case GPIO_PULL_REPEATER:
        // MODE = '10'.
        reg_value |= (0b11 << 4);
        break;
    default:
        break;
    }
    // Write register.
    IOCON->PIO[gpio->port][gpio->pin] = reg_value;
}

/*******************************************************************/
static void _GPIO_set_function(const GPIO_pin_t* gpio, uint32_t function_index) {
    // Local variables.
    uint32_t reg_value = 0;
    // Read register.
    reg_value = IOCON->PIO[gpio->port][gpio->pin];
    // Set function.
    reg_value &= ~(0b1111 << 0);
    reg_value |= (function_index &= 0x0F);
    // Write register.
    IOCON->PIO[gpio->port][gpio->pin] = reg_value;
}

/*** GPIO functions ***/

/*******************************************************************/
void GPIO_init(void) {
    // Enable IOCON and all GPIOx clocks.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_IOCON, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO0, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO1, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO2, 1);
    // Reset GPIO blocks.
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_IOCON);
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_GPIO0);
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_GPIO1);
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_GPIO2);
}

/*******************************************************************/
void GPIO_de_init(void) {
    // Enable IOCON and all GPIOx clocks.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_IOCON, 0);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO0, 0);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO1, 0);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_GPIO2, 0);
}

/*******************************************************************/
void GPIO_configure(const GPIO_pin_t* gpio, GPIO_mode_t mode, GPIO_output_type_t output_type, GPIO_slew_rate_t slew_rate, GPIO_pull_resistor_t pull_resistor) {
    // Configure GPIO.
    _GPIO_set_output_type(gpio, output_type);
    _GPIO_set_slew_rate(gpio, slew_rate);
    _GPIO_set_pull_resistor(gpio, pull_resistor);
    _GPIO_set_function(gpio, (gpio->function));
    _GPIO_set_mode(gpio, mode);
}

/*******************************************************************/
void GPIO_write(const GPIO_pin_t* gpio, uint8_t state) {
    // Write pin.
    GPIO->B[gpio->port][gpio->pin] = (state & 0x01);
}

/*******************************************************************/
uint8_t GPIO_read(const GPIO_pin_t* gpio) {
    // Read pin.
    return (GPIO->B[gpio->port][gpio->pin]);
}

/*******************************************************************/
void GPIO_toggle(const GPIO_pin_t* gpio) {
    // Write bit to toggle pin.
    GPIO->NOT[gpio->port] |= (0b1 << (gpio->pin));
}
