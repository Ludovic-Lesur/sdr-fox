/*
 * cli.c
 *
 *  Created on: 23 apr. 2025
 *      Author: Ludo
 */

#include "cli.h"

// Peripherals.
#include "nvic_priority.h"
// Utils.
#include "at.h"
#include "error.h"
#include "maths.h"
#include "parser.h"
#include "strings.h"
#include "types.h"
// Applicative.
#include "error_base.h"
#include "version.h"

/*** CLI local macros ***/

// Parsing.
#define CLI_CHAR_SEPARATOR  STRING_CHAR_COMMA

/*** CLI local structures ***/

/*******************************************************************/
typedef struct {
    volatile uint8_t at_process_flag;
    PARSER_context_t* at_parser_ptr;
} CLI_context_t;

/*** CLI local global variables ***/

static CLI_context_t cli_ctx = {
    .at_process_flag = 0,
    .at_parser_ptr = NULL
};

/*** CLI local functions ***/

/*******************************************************************/
#define _CLI_check_driver_status(driver_status, driver_success, driver_error_base) { \
    /* Check value */ \
    if (driver_status != driver_success) { \
        /* Stack error */ \
        ERROR_stack_add((ERROR_code_t) (driver_error_base + driver_status)); \
        /* Exit with execution error */ \
        status = AT_ERROR_COMMAND_EXECUTION; \
        goto errors; \
    } \
}

/*******************************************************************/
static void _CLI_at_process_callback(void) {
    // Set local flag.
    cli_ctx.at_process_flag = 1;
}

/*** CLI functions ***/

/*******************************************************************/
CLI_status_t CLI_init(void) {
    // Local variables.
    CLI_status_t status = CLI_SUCCESS;
    AT_status_t at_status = AT_SUCCESS;
    AT_configuration_t at_config;
    // Init context.
    cli_ctx.at_process_flag = 0;
    cli_ctx.at_parser_ptr = NULL;
    // Init AT driver.
    at_config.terminal_instance = 0;
    at_config.process_callback = &_CLI_at_process_callback;
    at_status = AT_init(&at_config, &(cli_ctx.at_parser_ptr));
    AT_exit_error(CLI_ERROR_BASE_AT);
errors:
    return status;
}

/*******************************************************************/
CLI_status_t CLI_de_init(void) {
    // Local variables.
    CLI_status_t status = CLI_SUCCESS;
    AT_status_t at_status = AT_SUCCESS;
    // Release AT driver.
    at_status = AT_de_init();
    AT_stack_error(ERROR_BASE_CLI + CLI_ERROR_BASE_AT);
    return status;
}

/*******************************************************************/
CLI_status_t CLI_process(void) {
    // Local variables.
    CLI_status_t status = CLI_SUCCESS;
    AT_status_t at_status = AT_SUCCESS;
    // Check process flag.
    if (cli_ctx.at_process_flag != 0) {
        // Clear flag.
        cli_ctx.at_process_flag = 0;
        // Process AT driver.
        at_status = AT_process();
        AT_exit_error(CLI_ERROR_BASE_AT);
    }
errors:
    return status;
}
