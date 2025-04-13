/*
 * usb_hs_device.c
 *
 *  Created on: 15 mar. 2023
 *      Author: Ludo
 */

#include "usb_hs_device.h"

#include "error.h"
#include "maths.h"
#include "nvic.h"
#include "syscon.h"
#include "systick.h"
#include "types/usb_endpoint.h"
#include "types/usb_request.h"
#include "types/usb_types.h"
#include "usb_hs_device_registers.h"
#include "usb_hs_host_registers.h"

#include "mcu_mapping.h"

/*** USB HS DEVICE local macros ***/

#define USB_HS_DEVICE_ADDRESS_MAX                               0x7F

#define USB_HS_DEVICE_DISCONNECT_DELAY_US                       10000

#define USB_HS_DEVICE_NUMBER_OF_LOGICAL_ENDPOINTS               6
#define USB_HS_DEVICE_NUMBER_OF_PHYSICAL_ENDPOINTS              (USB_HS_DEVICE_NUMBER_OF_LOGICAL_ENDPOINTS * USB_ENDPOINT_DIRECTION_LAST)

#define USB_HS_DEVICE_NUMBER_OF_ENDPOINT_BUFFERS                2

#define USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT                      6
#define USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES                      (0b1 << USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT)

#define USB_HS_DEVICE_RAM_EP_CS_SIZE                            (USB_HS_DEVICE_NUMBER_OF_ENDPOINT_BUFFERS * USB_ENDPOINT_DIRECTION_LAST)
#define USB_HS_DEVICE_RAM_EP_CS_LIST_SIZE                       (USB_HS_DEVICE_RAM_EP_CS_SIZE * USB_HS_DEVICE_NUMBER_OF_LOGICAL_ENDPOINTS)
#define USB_HS_DEVICE_RAM_EP_CS_LIST_ALIGNMENT                  256
#define USB_HS_DEVICE_RAM_EP_CS_INDEX(ep_num, ep_dir, ep_buf)   ((USB_HS_DEVICE_RAM_EP_CS_SIZE * ep_num) + (USB_ENDPOINT_DIRECTION_LAST * ep_dir) + ep_buf)
#define USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EP0                 0x00000000
#define USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EPX                 0x40000000

#define USB_HS_DEVICE_RAM_DATA_SIZE_BYTES                       16192
#define USB_HS_DEVICE_RAM_DATA_ALIGNMENT                        USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES
#define USB_HS_DEVICE_RAM_DATA_SIZE_BLOCKS                      ((USB_HS_DEVICE_RAM_DATA_SIZE_BYTES + USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES - 1) / USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES)
#define USB_HS_DEVICE_RAM_DATA_OFFSET_BYTES                     (((uint32_t) &usb_hs_device_ram_data.data[0]) - ((uint32_t) &usb_hs_device_ram_data.ep_cs_list[0]))
#define USB_HS_DEVICE_RAM_DATA_OFFSET_BLOCKS                    (USB_HS_DEVICE_RAM_DATA_OFFSET_BYTES >> USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT)

#define USB_HS_DEVICE_RAM_MALLOC_TABLE_SIZE_BYTES               (USB_HS_DEVICE_RAM_DATA_SIZE_BLOCKS / MATH_U8_SIZE_BITS)

#define USB_HS_DEVICE_EP_PHY(ep_num, ep_dir)                    ((USB_ENDPOINT_DIRECTION_LAST * ep_num) + (ep_dir))
#define USB_HS_DEVICE_EP_NUM(ep_phy)                            (ep_phy / USB_ENDPOINT_DIRECTION_LAST)
#define USB_HS_DEVICE_EP_DIR(ep_phy)                            (ep_phy % USB_ENDPOINT_DIRECTION_LAST)

#define USB_HS_DEVICE_EPLISTSTART_MASK                          0x000FFF00
#define USB_HS_DEVICE_DATABUFSTART_MASK                         0xFFC00000

/*** USB HS DEVICE local structures ***/

/*******************************************************************/
typedef union {
    struct {
        unsigned address_offset :11;
        unsigned n_bytes :15;
        unsigned type :1;
        unsigned rate_feedback_toggle_value :1;
        unsigned toggle_reset :1;
        unsigned stall :1;
        unsigned disable :1;
        unsigned active :1;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
    uint32_t value;
} USB_HS_DEVICE_endpoint_command_status_t;

/*******************************************************************/
typedef enum {
    USB_HS_DEVICE_ENDPOINT_BUFFER_0 = 0,
    USB_HS_DEVICE_ENDPOINT_BUFFER_1,
    USB_HS_DEVICE_ENDPOINT_BUFFER_LAST
} USB_HS_DEVICE_endpoint_buffer_t;

/*******************************************************************/
typedef struct {
    USB_HS_DEVICE_endpoint_command_status_t ep_cs_list[USB_HS_DEVICE_RAM_EP_CS_LIST_SIZE] __attribute__((aligned(USB_HS_DEVICE_RAM_EP_CS_LIST_ALIGNMENT)));
    uint8_t zlp[USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES] __attribute__((aligned(USB_HS_DEVICE_RAM_DATA_ALIGNMENT)));
    uint8_t data[USB_HS_DEVICE_RAM_DATA_SIZE_BYTES] __attribute__((aligned(USB_HS_DEVICE_RAM_DATA_ALIGNMENT)));
} USB_HS_DEVICE_ram_data_t;

/*******************************************************************/
typedef void (*USB_HS_DEVICE_endpoint_cb_t)(void);

/*******************************************************************/
typedef struct {
    uint8_t registered;
    uint32_t irq_mask;
    uint32_t max_packet_size_bytes;
    uint32_t address_offset_buffer0;
    uint32_t address_offset_buffer1;
    USB_endpoint_cb_t callback;
} USB_HS_DEVICE_physical_endpoint_t;

/*******************************************************************/
typedef union {
    struct {
        unsigned in_data_phase_ongoing;
        unsigned out_data_phase_ongoing;
        unsigned setup :1;
        unsigned init :1;
    };
    uint8_t all;
} USB_HS_DEVICE_flags_t;

/*******************************************************************/
typedef struct {
    USB_setup_cb_t setup_callback;
    volatile uint32_t intstat;
    volatile USB_HS_DEVICE_flags_t flags;
    uint8_t ram_malloc_table[USB_HS_DEVICE_RAM_MALLOC_TABLE_SIZE_BYTES];
    USB_HS_DEVICE_physical_endpoint_t physical_endpoints[USB_HS_DEVICE_NUMBER_OF_PHYSICAL_ENDPOINTS];
} USB_HS_DEVICE_context_t;

/*** USB HS DEVICE local global variables ***/

static volatile USB_HS_DEVICE_ram_data_t usb_hs_device_ram_data __attribute__((section (".bss_sram_usb")));

static USB_HS_DEVICE_context_t usb_hs_device_ctx = {
    .setup_callback = NULL,
    .intstat = 0,
    .flags.all = 0,
    .ram_malloc_table = { [0 ... (USB_HS_DEVICE_RAM_MALLOC_TABLE_SIZE_BYTES - 1)] = 0x00 }
};

/*** USB HS DEVICE local functions declarations ***/

static void _USB_HS_DEVICE_status_irq_callback(void);
static void _USB_HS_DEVICE_control_endpoint_out_irq_callback(uint8_t ep_phy);
static void _USB_HS_DEVICE_control_endpoint_in_irq_callback(uint8_t ep_phy);
static void _USB_HS_DEVICE_data_endpoint_irq_callback(uint8_t ep_phy);

/*** USB HS DEVICE local functions ***/

/*******************************************************************/
void USB1_IRQHandler(void) {
    // Local variables.
    uint8_t ep_phy = 0;
    // Capture interrupts flags.
    usb_hs_device_ctx.intstat |= (USB_HS_DEVICE->INTSTAT);
    // Clear all flags.
    USB_HS_DEVICE->INTSTAT |= 0xC0000FFF;
    // Check device interrupt.
    _USB_HS_DEVICE_status_irq_callback();
    // Endpoints loop.
    for (ep_phy = 0; ep_phy < USB_HS_DEVICE_NUMBER_OF_PHYSICAL_ENDPOINTS; ep_phy++) {
        // Check software flag.
        if (((usb_hs_device_ctx.intstat) & (usb_hs_device_ctx.physical_endpoints[ep_phy].irq_mask)) != 0) {
            // Clear software flag.
            usb_hs_device_ctx.intstat &= ~(usb_hs_device_ctx.physical_endpoints[ep_phy].irq_mask);
            // Check if endpoint is registered.
            if (usb_hs_device_ctx.physical_endpoints[ep_phy].registered != 0) {
                // Specific control endpoint handling.
                switch (ep_phy) {
                case (USB_HS_DEVICE_EP_PHY(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_OUT)):
                    _USB_HS_DEVICE_control_endpoint_out_irq_callback(ep_phy);
                    break;
                case (USB_HS_DEVICE_EP_PHY(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_IN)):
                    _USB_HS_DEVICE_control_endpoint_in_irq_callback(ep_phy);
                    break;
                default:
                    _USB_HS_DEVICE_data_endpoint_irq_callback(ep_phy);
                    break;
                }
            }
        }
    }
}

/*******************************************************************/
static void _USB_HS_DEVICE_status_irq_callback(void) {
    // Check flag.
    if ((usb_hs_device_ctx.intstat & (0b1 << 31)) != 0) {
        // Clear flag.
        usb_hs_device_ctx.intstat &= ~(0b1 << 31);
        // Clear status flags.
        USB_HS_DEVICE->DEVCMDSTAT |= (0b111 << 24);
        // TODO
    }
}

/*******************************************************************/
static void _USB_HS_DEVICE_control_endpoint_out_irq_callback(uint8_t ep_phy) {
    // Local variables.
    uint8_t ep0_out_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_OUT, USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ep0_in_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_IN, USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    USB_request_operation_t request_operation = USB_REQUEST_OPERATION_NOT_SUPPORTED;
    // Check setup flag.
    if (((USB_HS_DEVICE->DEVCMDSTAT) & (0b1 << 8)) != 0) {
        // Set endpoint state.
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 0;
        // Update local flags.
        usb_hs_device_ctx.flags.setup = 1;
        usb_hs_device_ctx.flags.out_data_phase_ongoing = 0;
        usb_hs_device_ctx.flags.in_data_phase_ongoing = 0;
        // Clear setup flag.
        USB_HS_DEVICE->DEVCMDSTAT |= (0b1 << 8);
        // Call setup callback.
        if (usb_hs_device_ctx.setup_callback != NULL) {
            usb_hs_device_ctx.setup_callback(&request_operation);
        }
        switch (request_operation) {
        case USB_REQUEST_OPERATION_READ:
            // Start IN data phase.
            // Note: NBytes and active bit are set by the setup callback which just called the write function.
            usb_hs_device_ctx.flags.in_data_phase_ongoing = 1;
            break;
        case USB_REQUEST_OPERATION_WRITE:
            // Start OUT data phase.
            usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 1;
            usb_hs_device_ctx.flags.out_data_phase_ongoing = 1;
            break;
        case USB_REQUEST_OPERATION_WRITE_NO_DATA:
            // Activate IN endpoint.
            usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 1;
            usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].n_bytes = 0;
            break;
        default:
            // Stall control endpoint.
            usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
            usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
            break;
        }
    }
    else if (usb_hs_device_ctx.flags.out_data_phase_ongoing != 0) {
        // Activate IN endpoint.
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].n_bytes = 0;
        // Reset OUT endpoint for next reception.
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].n_bytes = (usb_hs_device_ctx.physical_endpoints[ep_phy].max_packet_size_bytes);
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].address_offset = (usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer0);
        // Call applicative callback.
        if (usb_hs_device_ctx.physical_endpoints[ep_phy].callback != NULL) {
            usb_hs_device_ctx.physical_endpoints[ep_phy].callback();
        }
    }
    else if (usb_hs_device_ctx.flags.in_data_phase_ongoing != 0) {
        // Case of OUT ZLP acknowledge.
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].n_bytes = (usb_hs_device_ctx.physical_endpoints[ep_phy].max_packet_size_bytes + 1);
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
        // Reset flag.
        usb_hs_device_ctx.flags.in_data_phase_ongoing = 0;
    }
    else {
        // Stall endpoint.
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
    }
}

/*******************************************************************/
static void _USB_HS_DEVICE_control_endpoint_in_irq_callback(uint8_t ep_phy) {
    // Local variables.
    uint8_t ep0_out_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_OUT, USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ep0_in_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_IN, USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ep0_out_phy = USB_HS_DEVICE_EP_PHY(USB_ENDPOINT_NUMBER_EP0, USB_ENDPOINT_DIRECTION_OUT);
    // Check data phase.
    if (usb_hs_device_ctx.flags.in_data_phase_ongoing != 0) {
        // Activate OUT endpoint.
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].n_bytes = (usb_hs_device_ctx.physical_endpoints[ep0_out_phy].max_packet_size_bytes + 1);
        // Reset IN endpoint for next transmission.
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].n_bytes = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].address_offset = (usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer0);
        // Call applicative callback.
        if (usb_hs_device_ctx.physical_endpoints[ep_phy].callback != NULL) {
            usb_hs_device_ctx.physical_endpoints[ep_phy].callback();
        }
    }
    else if (usb_hs_device_ctx.flags.out_data_phase_ongoing != 0) {
        // Case of IN ZLP acknowledge.
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].n_bytes = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 1;
        // Reset flag.
        usb_hs_device_ctx.flags.out_data_phase_ongoing = 0;
    }
    else {
        // Stall endpoint.
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_in_cs_index].stall = 1;
    }
}

/*******************************************************************/
static void _USB_HS_DEVICE_data_endpoint_irq_callback(uint8_t ep_phy) {
    // Call applicative callback if not NULL.
    if (usb_hs_device_ctx.physical_endpoints[ep_phy].callback != NULL) {
        usb_hs_device_ctx.physical_endpoints[ep_phy].callback();
    }
}

/*****************************************usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].active = 0;
        usb_hs_device_ram_data.ep_cs_list[ep0_out_cs_index].stall = 0;**************************/
static USB_HS_DEVICE_status_t _USB_HS_DEVICE_ram_malloc(uint32_t ram_size_bytes, uint32_t* ram_address_offset_blocks) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint32_t ram_size_blocks = 0;
    uint32_t free_size_blocks = 0;
    uint32_t free_start_block = 0;
    uint32_t free_end_block = 0;
    uint32_t block_idx = 0;
    uint8_t byte_idx = 0;
    uint8_t bit_idx = 0;
    // Check parameters.
    if ((ram_size_bytes == 0) || (ram_size_bytes > USB_HS_DEVICE_RAM_DATA_SIZE_BYTES)) {
        status = USB_HS_DEVICE_ERROR_RAM_MALLOC_SIZE;
        goto errors;
    }
    if (ram_address_offset_blocks == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Compute number of blocks.
    ram_size_blocks = ((ram_size_bytes + USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES - 1) / (USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES));
    // Search first free location with required amount of bytes.
    for (block_idx = 0; block_idx < USB_HS_DEVICE_RAM_DATA_SIZE_BLOCKS; block_idx++) {
        // Compute indexes.
        byte_idx = (block_idx / MATH_U8_SIZE_BITS);
        bit_idx = (block_idx % MATH_U8_SIZE_BITS);
        // Check if block is free.
        if ((usb_hs_device_ctx.ram_malloc_table[byte_idx] & (0b1 << bit_idx)) != 0) {
            // Reset size since we search a contiguous RAM space.
            free_size_blocks = 0;
            free_start_block = (block_idx + 1);
        }
        else {
            // Increment free size.
            free_size_blocks++;
        }
        // Exit if required size has been reached.
        if (free_size_blocks >= ram_size_blocks) break;
    }
    free_end_block = (block_idx + 1);
    // Check if memory space has been found.
    if (free_size_blocks < ram_size_blocks) {
        status = USB_HS_DEVICE_ERROR_RAM_MALLOC;
        goto errors;
    }
    // Update allocation status.
    for (block_idx = free_start_block; block_idx < free_end_block; block_idx++) {
        // Compute indexes.
        byte_idx = (block_idx / MATH_U8_SIZE_BITS);
        bit_idx = (block_idx % MATH_U8_SIZE_BITS);
        // Set bit.
        usb_hs_device_ctx.ram_malloc_table[byte_idx] |= (0b1 << bit_idx);
    }
    // Update allocated address.
    (*ram_address_offset_blocks) = (USB_HS_DEVICE_RAM_DATA_OFFSET_BLOCKS + free_start_block);
errors:
    return status;
}

/*******************************************************************/
static USB_HS_DEVICE_status_t _USB_HS_DEVICE_ram_free(uint32_t ram_address_offset_blocks, uint32_t ram_size_bytes) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint32_t ram_size_blocks = 0;
    uint32_t block_idx = 0;
    uint8_t byte_idx = 0;
    uint8_t bit_idx = 0;
    uint8_t idx = 0;
    // Check parameters.
    if (ram_address_offset_blocks >= USB_HS_DEVICE_RAM_DATA_SIZE_BLOCKS) {
        status = USB_HS_DEVICE_ERROR_RAM_FREE_ADDRESS;
        goto errors;
    }
    if ((ram_size_bytes == 0) || (ram_size_bytes > USB_HS_DEVICE_RAM_DATA_SIZE_BYTES)) {
        status = USB_HS_DEVICE_ERROR_RAM_FREE_SIZE;
        goto errors;
    }
    // Compute number of blocks.
    ram_size_blocks = ((ram_size_bytes + USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES - 1) / (USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES));
    // Update allocation status.
    for (block_idx = ram_address_offset_blocks; block_idx < (ram_address_offset_blocks + ram_size_blocks); block_idx++) {
        // Compute indexes.
        byte_idx = (block_idx / MATH_U8_SIZE_BITS);
        bit_idx = (block_idx % MATH_U8_SIZE_BITS);
        // Set bit.
        usb_hs_device_ctx.ram_malloc_table[byte_idx] &= ~(0b1 << bit_idx);
        // Clear RAM.
        for (idx = 0; idx < USB_HS_DEVICE_RAM_BLOCK_SIZE_BYTES; idx++) {
            usb_hs_device_ram_data.data[(block_idx << USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT) + idx] = 0x00;
        }
    }
errors:
    return status;
}

/*******************************************************************/
static void _USB_HS_DEVICE_reset_endpoint(uint8_t ep_phy) {
    // Local variables.
    uint8_t ep_num = USB_HS_DEVICE_EP_NUM(ep_phy);
    uint8_t ep_dir = USB_HS_DEVICE_EP_DIR(ep_phy);
    uint8_t ram_endpoint_idx_0 = USB_HS_DEVICE_RAM_EP_CS_INDEX(ep_num, ep_dir, USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ram_endpoint_idx_1 = USB_HS_DEVICE_RAM_EP_CS_INDEX(ep_num, ep_dir, USB_HS_DEVICE_ENDPOINT_BUFFER_1);
    // Reset physical endpoint.
    usb_hs_device_ctx.physical_endpoints[ep_phy].registered = 0;
    usb_hs_device_ctx.physical_endpoints[ep_phy].irq_mask = 0;
    usb_hs_device_ctx.physical_endpoints[ep_phy].max_packet_size_bytes = 0;
    usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer0 = 0;
    usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer1 = 0;
    usb_hs_device_ctx.physical_endpoints[ep_phy].callback = NULL;
    // Reset endpoint descriptor in RAM.
    if (USB_HS_DEVICE_EP_NUM(ep_phy) == USB_ENDPOINT_NUMBER_EP0) {
        // Specific value for control endpoint.
        usb_hs_device_ram_data.ep_cs_list[ram_endpoint_idx_0].value = USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EP0;
        usb_hs_device_ram_data.ep_cs_list[ram_endpoint_idx_1].value = USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EP0;
    }
    else {
        // Default value with disable bit set.
        usb_hs_device_ram_data.ep_cs_list[ram_endpoint_idx_0].value = USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EPX;
        usb_hs_device_ram_data.ep_cs_list[ram_endpoint_idx_1].value = USB_HS_DEVICE_RAM_EP_CS_RESET_VALUE_EPX;
    }
}

/*******************************************************************/
static USB_HS_DEVICE_status_t _USB_HS_DEVICE_check_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    // Check endpoint.
    if (endpoint == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    if ((endpoint->number) >= USB_HS_DEVICE_NUMBER_OF_LOGICAL_ENDPOINTS) {
        status = USB_HS_DEVICE_ERROR_ENDPOINT_NUMBER;
        goto errors;
    }
    if ((endpoint->direction) >= USB_ENDPOINT_DIRECTION_LAST) {
        status = USB_HS_DEVICE_ERROR_ENDPOINT_DIRECTION;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
static USB_HS_DEVICE_status_t _USB_HS_DEVICE_set_address(uint8_t device_address) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint32_t reg_value = 0;
    // Check address.
    if (device_address > USB_HS_DEVICE_ADDRESS_MAX) {
        status = USB_HS_DEVICE_ERROR_ADDRESS;
        goto errors;
    }
    // Set device address.
    reg_value = ((USB_HS_DEVICE->DEVCMDSTAT) & 0xFFFFFFE0);
    reg_value |= (device_address & 0x7F);
    USB_HS_DEVICE->DEVCMDSTAT = reg_value;
errors:
    return status;
}

/*** USB HS DEVICE functions ***/

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_init(uint8_t nvic_priority) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    USB_HS_PHY_status_t usb_hs_phy_status = USB_HS_PHY_SUCCESS;
    uint32_t idx = 0;
    // Check state.
    if (usb_hs_device_ctx.flags.init != 0) {
        status = USB_HS_DEVICE_ERROR_ALREADY_INITIALIZED;
        goto errors;
    }
    // Init context.
    usb_hs_device_ctx.intstat = 0;
    usb_hs_device_ctx.flags.all = 0;
    for (idx = 0; idx < USB_HS_DEVICE_RAM_MALLOC_TABLE_SIZE_BYTES; idx++) {
        usb_hs_device_ctx.ram_malloc_table[idx] = 0x00;
    }
    // Init PHY.
    usb_hs_phy_status = USB_HS_PHY_init();
    USB_HS_PHY_exit_error(USB_HS_DEVICE_ERROR_BASE_USB_HS_PHY);
    // Enable peripheral clock.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_RAM, 1);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_DEV, 1);
    // Set device mode.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_HOST, 1);
    USB_HS_HOST->PORTMODE |= (0b1 << 16);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_HOST, 0);
    // Reset blocks.
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_USB1_RAM);
    SYSCON_reset_peripheral(SYSCON_PERIPHERAL_USB1_DEV);
    // Enable DP pull-up resistor.
    USB_HS_DEVICE->DEVCMDSTAT |= (0b1 << 16);
    // Reset registers.
    USB_HS_DEVICE->EPINUSE = 0;
    USB_HS_DEVICE->EPSKIP = 0;
    // Set endpoint command and status list and data addresses.
    USB_HS_DEVICE->EPLISTSTART = ((uint32_t) &(usb_hs_device_ram_data.ep_cs_list[0]) & USB_HS_DEVICE_EPLISTSTART_MASK);
    USB_HS_DEVICE->DATABUFSTART = (((uint32_t) &(usb_hs_device_ram_data)) & USB_HS_DEVICE_DATABUFSTART_MASK);
    // Disable LPM.
    USB_HS_DEVICE->DEVCMDSTAT &= ~(0b1 << 11);
    // Set interrupts priority.
    NVIC_set_priority(NVIC_INTERRUPT_USB1, nvic_priority);
    // Disable NAK interrupts.
    USB_HS_DEVICE->DEVCMDSTAT &= ~(0b1111 << 12);
    // Always start with address 0x00 for enumeration.
    status = _USB_HS_DEVICE_set_address(0x00);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Reset peripheral.
    status = USB_HS_DEVICE_reset();
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Update initialization flag.
    usb_hs_device_ctx.flags.init = 1;
errors:
    // Disable device by default.
    USB_HS_DEVICE_stop();
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_de_init(void) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    USB_HS_PHY_status_t usb_hs_phy_status = USB_HS_PHY_SUCCESS;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Disable peripheral clock.
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_RAM, 0);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_DEV, 0);
    SYSCON_set_peripheral_clock(SYSCON_PERIPHERAL_USB1_HOST, 0);
    // Release PHY.
    usb_hs_phy_status = USB_HS_PHY_de_init();
    USB_HS_PHY_exit_error(USB_HS_DEVICE_ERROR_BASE_USB_HS_PHY);
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_reset(void) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint8_t idx = 0;
    // Flush endpoints list.
    for (idx = 0; idx < USB_HS_DEVICE_NUMBER_OF_PHYSICAL_ENDPOINTS; idx++) {
        _USB_HS_DEVICE_reset_endpoint(idx);
    }
    // Free all USB RAM.
    status = _USB_HS_DEVICE_ram_free(0, USB_HS_DEVICE_RAM_DATA_SIZE_BYTES);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_register_setup_callback(USB_setup_cb_t setup_callback) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    // Check parameter.
    if (setup_callback == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Update local pointer.
    usb_hs_device_ctx.setup_callback = setup_callback;
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint8_t ep_phy = USB_HS_DEVICE_EP_PHY((endpoint->number), (endpoint->direction));
    uint8_t ep_cs_buffer0_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ep_cs_buffer1_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), USB_HS_DEVICE_ENDPOINT_BUFFER_1);
    uint32_t ram_address_offset_blocks = 0;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Check parameter.
    status = _USB_HS_DEVICE_check_endpoint(endpoint);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Check if endpoint is available.
    if (usb_hs_device_ctx.physical_endpoints[ep_phy].registered != 0) {
        status = USB_HS_DEVICE_ERROR_ENDPOINT_ALREADY_REGISTERED;
        goto errors;
    }
    // Check index.
    if ((endpoint->number) == USB_ENDPOINT_NUMBER_EP0) {
        // Check type.
        if ((endpoint->transfer_type) != USB_ENDPOINT_TRANSFER_TYPE_CONTROL) {
            status = USB_HS_DEVICE_ERROR_ENDPOINT_TRANSFER_TYPE;
            goto errors;
        }
    }
    else {
        // Set type on both buffers.
        switch (endpoint->transfer_type) {
        case USB_ENDPOINT_TRANSFER_TYPE_BULK:
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].type = 0;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].rate_feedback_toggle_value = 0;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].type = 0;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].rate_feedback_toggle_value = 0;
            break;
        case USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS:
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].type = 1;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].rate_feedback_toggle_value = 0;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].type = 1;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].rate_feedback_toggle_value = 0;
            break;
        case USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT:
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].type = 1;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].rate_feedback_toggle_value = 1;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].type = 1;
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].rate_feedback_toggle_value = 1;
            break;
        default:
            status = USB_HS_DEVICE_ERROR_ENDPOINT_TRANSFER_TYPE;
            goto errors;
        }
    }
    // Allocate RAM for buffer 0.
    status = _USB_HS_DEVICE_ram_malloc((endpoint->max_packet_size_bytes), &ram_address_offset_blocks);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Update address offset.
    usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].address_offset = ram_address_offset_blocks;
    // Enable buffer 0.
    usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].disable = 0;
    usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].stall = 0;
    // Specific exception for EP0-IN.
    if ((endpoint->number != USB_ENDPOINT_NUMBER_EP0) || (endpoint->direction != USB_ENDPOINT_DIRECTION_IN)) {
        // Allocate RAM for buffer 1.
        status = _USB_HS_DEVICE_ram_malloc((endpoint->max_packet_size_bytes), &ram_address_offset_blocks);
        if (status != USB_HS_DEVICE_SUCCESS) goto errors;
        // Update address offset.
        usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].address_offset = ram_address_offset_blocks;
        // Enable buffer 1.
        usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].disable = 0;
        usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].stall = 0;
    }
    // Use double buffer mode for data endpoints.
    if ((endpoint->number) != USB_ENDPOINT_NUMBER_EP0) {
        USB_HS_DEVICE->EPBUFCFG |= (0b1 << ((USB_ENDPOINT_DIRECTION_LAST * (endpoint->number)) + (endpoint->direction)));
    }
    // Activate out endpoint.
    if ((endpoint->direction) == USB_ENDPOINT_DIRECTION_OUT) {
        // Buffer 0.
        usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].n_bytes = (endpoint->max_packet_size_bytes);
        usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].active = 1;
        // Buffer 1.
        if ((endpoint->number) != USB_ENDPOINT_NUMBER_EP0) {
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].n_bytes = (endpoint->max_packet_size_bytes);
            usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].active = 1;
        }
    }
    // Update local list.
    usb_hs_device_ctx.physical_endpoints[ep_phy].registered = 1;
    usb_hs_device_ctx.physical_endpoints[ep_phy].irq_mask = (0b1 << ep_phy);
    usb_hs_device_ctx.physical_endpoints[ep_phy].max_packet_size_bytes = (endpoint->max_packet_size_bytes);
    usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer0 = usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].address_offset;
    usb_hs_device_ctx.physical_endpoints[ep_phy].address_offset_buffer1 = usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].address_offset;
    usb_hs_device_ctx.physical_endpoints[ep_phy].callback = (endpoint->callback);
    // Enable corresponding interrupt.
    USB_HS_DEVICE->INTEN |= (0b1 << ep_phy);
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    uint8_t ep_phy = USB_HS_DEVICE_EP_PHY((endpoint->number), (endpoint->direction));
    uint8_t ep_cs_buffer0_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), USB_HS_DEVICE_ENDPOINT_BUFFER_0);
    uint8_t ep_cs_buffer1_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), USB_HS_DEVICE_ENDPOINT_BUFFER_1);
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Check parameter.
    status = _USB_HS_DEVICE_check_endpoint(endpoint);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Check if endpoint is available.
    if (usb_hs_device_ctx.physical_endpoints[ep_phy].registered == 0) {
        status = USB_HS_DEVICE_ERROR_ENDPOINT_NOT_REGISTERED;
        goto errors;
    }
    // Free RAM for buffer 0.
    status = _USB_HS_DEVICE_ram_free(usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer0_index].address_offset, (endpoint->max_packet_size_bytes));
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Free RAM for buffer 1.
    status = _USB_HS_DEVICE_ram_free(usb_hs_device_ram_data.ep_cs_list[ep_cs_buffer1_index].address_offset, (endpoint->max_packet_size_bytes));
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Reset endpoint.
    _USB_HS_DEVICE_reset_endpoint(ep_phy);
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_start(void) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Check setup callback.
    if (usb_hs_device_ctx.setup_callback == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Delay for host to detect disconnection.
    systick_status = SYSTICK_delay_microseconds(USB_HS_DEVICE_DISCONNECT_DELAY_US);
    SYSTICK_exit_error(USB_HS_DEVICE_ERROR_BASE_SYSTICK);
    // Clear pending interrupts.
    NVIC_clear_pending_interrupt(NVIC_INTERRUPT_USB1);
    // Clear flags.
    USB_HS_DEVICE->INTSTAT |= 0xC0000FFF;
    // Enable device status interrupt
    USB_HS_DEVICE->INTEN |= (0b1 << 31);
    NVIC_enable_interrupt(NVIC_INTERRUPT_USB1);
    // Enable device.
    USB_HS_DEVICE->DEVCMDSTAT |= (0b1 << 7);
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_stop(void) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    SYSTICK_status_t systick_status = SYSTICK_SUCCESS;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Disable interrupts.
    NVIC_disable_interrupt(NVIC_INTERRUPT_USB1);
    // Disable device.
    USB_HS_DEVICE->DEVCMDSTAT &= ~(0b1 << 7);
    // Delay for host to detect disconnection.
    systick_status = SYSTICK_delay_microseconds(USB_HS_DEVICE_DISCONNECT_DELAY_US);
    SYSTICK_exit_error(USB_HS_DEVICE_ERROR_BASE_SYSTICK);
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_write(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_buffer_t ep_buf = USB_HS_DEVICE_ENDPOINT_BUFFER_0;
    uint8_t ep_phy = USB_HS_DEVICE_EP_PHY((endpoint->number), (endpoint->direction));
    uint8_t ep_cs_index = 0;
    uint32_t ep_ram_data_address = 0;
    uint32_t idx = 0;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Check endpoint.
    status = _USB_HS_DEVICE_check_endpoint(endpoint);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Check parameter.
    if (usb_data_in == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    if (((usb_data_in->data) == NULL) || ((usb_data_in->data_size_bytes) == 0)) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
    }
    if ((usb_data_in->data_size_bytes) > (endpoint->max_packet_size_bytes)) {
        status = USB_HS_DEVICE_ERROR_PACKET_SIZE_OVERFLOW;
        goto errors;
    }
    // Compute buffer index.
    if ((endpoint->number) != USB_ENDPOINT_NUMBER_EP0) {
        // Read last used buffer.
        ep_buf = (((USB_HS_DEVICE->EPINUSE) & (0b1 << ep_phy)) == 0) ? USB_HS_DEVICE_ENDPOINT_BUFFER_1 : USB_HS_DEVICE_ENDPOINT_BUFFER_0;
    }
    // Get RAM address.
    ep_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), ep_buf);
    ep_ram_data_address = (usb_hs_device_ram_data.ep_cs_list[ep_cs_index].address_offset - USB_HS_DEVICE_RAM_DATA_OFFSET_BLOCKS) << USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT;
    // Copy data in USB RAM.
    for (idx = 0; idx < (usb_data_in->data_size_bytes); idx++) {
        usb_hs_device_ram_data.data[ep_ram_data_address + idx] = (usb_data_in->data)[idx];
    }
    // Update number of bytes.
    usb_hs_device_ram_data.ep_cs_list[ep_cs_index].n_bytes = (usb_data_in->data_size_bytes);
    // Activate endpoint.
    usb_hs_device_ram_data.ep_cs_list[ep_cs_index].active = 1;
errors:
    return status;
}

/*******************************************************************/
USB_HS_DEVICE_status_t USB_HS_DEVICE_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out) {
    // Local variables.
    USB_HS_DEVICE_status_t status = USB_HS_DEVICE_SUCCESS;
    USB_HS_DEVICE_endpoint_buffer_t ep_buf = USB_HS_DEVICE_ENDPOINT_BUFFER_0;
    uint8_t ep_phy = USB_HS_DEVICE_EP_PHY((endpoint->number), (endpoint->direction));
    uint8_t ep_cs_index = 0;
    uint32_t ep_ram_data_address = 0;
    // Check state.
    if (usb_hs_device_ctx.flags.init == 0) {
        status = USB_HS_DEVICE_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Check endpoint.
    status = _USB_HS_DEVICE_check_endpoint(endpoint);
    if (status != USB_HS_DEVICE_SUCCESS) goto errors;
    // Check parameter.
    if (usb_data_out == NULL) {
        status = USB_HS_DEVICE_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Compute buffer index.
    if ((endpoint->number) == USB_ENDPOINT_NUMBER_EP0) {
        // Check setup flag.
        ep_buf = (usb_hs_device_ctx.flags.setup == 0) ? USB_HS_DEVICE_ENDPOINT_BUFFER_0 : USB_HS_DEVICE_ENDPOINT_BUFFER_1;
    }
    else {
        // Check double buffer mode status.
        ep_buf = (((USB_HS_DEVICE->EPINUSE) & (0b1 << ep_phy)) == 0) ? USB_HS_DEVICE_ENDPOINT_BUFFER_1 : USB_HS_DEVICE_ENDPOINT_BUFFER_0;
    }
    // Get RAM address.
    ep_cs_index = USB_HS_DEVICE_RAM_EP_CS_INDEX((endpoint->number), (endpoint->direction), ep_buf);
    ep_ram_data_address = (usb_hs_device_ram_data.ep_cs_list[ep_cs_index].address_offset - USB_HS_DEVICE_RAM_DATA_OFFSET_BLOCKS) << USB_HS_DEVICE_RAM_BLOCK_SIZE_SHIFT;
    // Fill data.
    usb_data_out->data = (uint8_t*) &(usb_hs_device_ram_data.data[ep_ram_data_address]);
    // Compute data size.
    if (usb_hs_device_ctx.flags.setup != 0) {
        // NBytes field is not updated for setup packets.
        usb_data_out->data_size_bytes = USB_SETUP_PACKET_SIZE_BYTES;
    }
    else {
        // RAM buffer has been allocated with the maximum packet size.
        usb_data_out->data_size_bytes = (endpoint->max_packet_size_bytes - usb_hs_device_ram_data.ep_cs_list[ep_cs_index].n_bytes);
    }
errors:
    // Clear setup flag.
    usb_hs_device_ctx.flags.setup = 0;
    return status;
}
