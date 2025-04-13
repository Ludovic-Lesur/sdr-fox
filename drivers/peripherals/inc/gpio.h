/*
 * gpio.h
 *
 *  Created on: 09 mar. 2023
 *      Author: Ludo
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "types.h"

/*** GPIO structures ***/

/*!******************************************************************
 * \struct GPIO_pin_t
 * \brief GPIO pin descriptor.
 *******************************************************************/
typedef struct {
    uint8_t port;
    uint8_t pin;
    uint8_t function;
} GPIO_pin_t;

/*!******************************************************************
 * \enum GPIO_mode_t
 * \brief GPIO modes list.
 *******************************************************************/
typedef enum {
    GPIO_MODE_ANALOG_INPUT,
    GPIO_MODE_DIGITAL_INPUT,
    GPIO_MODE_DIGITAL_INPUT_INVERTED,
    GPIO_MODE_DIGITAL_OUTPUT,
    GPIO_MODE_LAST
} GPIO_mode_t;

/*!******************************************************************
 * \enum GPIO_output_type_t
 * \brief GPIO output types.
 *******************************************************************/
typedef enum {
    GPIO_TYPE_PUSH_PULL = 0,
    GPIO_TYPE_OPEN_DRAIN,
    GPIO_TYPE_LAST
} GPIO_output_type_t;

/*!******************************************************************
 * \enum GPIO_slew_rate_t
 * \brief GPIO slew rates.
 *******************************************************************/
typedef enum {
    GPIO_SLEW_RATE_STANDARD = 0,
    GPIO_SLEW_RATE_FAST,
    GPIO_SLEW_RATE_LAST
} GPIO_slew_rate_t;

/*!******************************************************************
 * \enum GPIO_pull_resistor_t
 * \brief GPIO pull resistor configurations.
 *******************************************************************/
typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_DOWN,
    GPIO_PULL_UP,
    GPIO_PULL_REPEATER,
    GPIO_PULL_LAST
} GPIO_pull_resistor_t;

/*** GPIO functions ***/

/*!******************************************************************
 * \fn void GPIO_init(void)
 * \brief Init GPIO driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void GPIO_init(void);

/*!******************************************************************
 * \fn void GPIO_de_init(void)
 * \brief Release GPIO driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void GPIO_de_init(void);

/*!******************************************************************
 * \fn void GPIO_configure(const GPIO_pin_t* gpio, GPIO_mode_t mode, GPIO_output_type_t output_type, GPIO_slew_rate_t slew_rate, GPIO_pull_resistor_t pull_resistor)
 * \brief Configure a GPIO.
 * \param[in]   gpio: GPIO to configure.
 * \param[in]   mode: GPIO mode.
 * \param[in]   output_type: GPIO output type.
 * \param[in]   slew_rate: GPIO slew rate.
 * \param[in]   pull_resistor: GPIO pull resistor configuration.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void GPIO_configure(const GPIO_pin_t* gpio, GPIO_mode_t mode, GPIO_output_type_t output_type, GPIO_slew_rate_t slew_rate, GPIO_pull_resistor_t pull_resistor);

/*!******************************************************************
 * \fn void GPIO_write(const GPIO_pin_t* gpio, uint8_t state)
 * \brief Set GPIO output state.
 * \param[in]   gpio: GPIO to write.
 * \param[in]   state: Output state to write (0 for low level, 1 for high level).
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void GPIO_write(const GPIO_pin_t* gpio, uint8_t state);

/*!******************************************************************
 * \fn uint8_t GPIO_read(const GPIO_pin_t* gpio)
 * \brief Read GPIO input or output state.
 * \param[in]   gpio: GPIO to read.
 * \param[out]  none
 * \retval      GPIO state (0 is low level, 1 is high level).
 *******************************************************************/
uint8_t GPIO_read(const GPIO_pin_t* gpio);

/*!******************************************************************
 * \fn void GPIO_toggle(const GPIO_pin_t* gpio)
 * \brief Toggle GPIO output state.
 * \param[in]   gpio: GPIO to toggle.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void GPIO_toggle(const GPIO_pin_t* gpio);

#endif /* __GPIO_H__ */
