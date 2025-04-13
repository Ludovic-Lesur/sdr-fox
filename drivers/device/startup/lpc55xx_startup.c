/*
 * lpc55xx_startup.c
 *
 *  Created on: 08 mar. 2023
 *      Author: Ludo
 */

#ifndef LPC55XX_DEVICE_DISABLE_FLAGS_FILE
#include "lpc55xx_device_flags.h"
#endif
#include "types.h"

/*** LPC55XX DEVICE structures ***/

typedef void (*pFunc)(void);

/*** LPC55XX DEVICE local functions declaration ***/

// Default interrupt handler.
void Default_Handler(void);
// Weak definition of main function.
void main(void)                             __attribute__ ((weak, alias("Default_Handler")));
// Cortex-M33 interrupts.
void Reset_Handler(void);
void NMI_Handler(void)                      __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)                __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)                __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void SecureFault_Handler(void)              __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void)                      __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)                  __attribute__ ((weak, alias("Default_Handler")));
// Device interrupts.
void WDT_BOD_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void DMA0_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void GINT0_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void GINT1_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT0_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT1_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT2_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT3_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void UTICK0_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void MRT0_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void CTIMER0_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CTIMER1_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void SCT0_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void CTIMER3_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM0_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM1_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM2_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM3_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM4_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM5_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM6_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM7_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void Reserved39_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void ACMP_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void Reserved41_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void Reserved42_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void USB0_NEEDCLK_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void USB0_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void Reserved46_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void MAILBOX_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT4_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT5_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT6_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PIN_INT7_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void CTIMER2_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CTIMER4_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void OS_EVENT_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void Reserved55_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void Reserved56_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void Reserved57_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void SDIO_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void Reserved59_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void Reserved60_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void Reserved61_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void USB1_PHY_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void USB1_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void USB1_NEEDCLK_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void SEC_HYPERVISOR_CALL_IRQHandler(void)   __attribute__ ((weak, alias("Default_Handler")));
void SEC_GPIO_INT0_IRQ0_IRQHandler(void)    __attribute__ ((weak, alias("Default_Handler")));
void SEC_GPIO_INT0_IRQ1_IRQHandler(void)    __attribute__ ((weak, alias("Default_Handler")));
void PLU_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void SEC_VIO_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void HASHCRYPT_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void CASER_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void PUF_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void PQ_IRQHandler(void)                    __attribute__ ((weak, alias("Default_Handler")));
void DMA1_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void FLEXCOMM8_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));

/*** LPC55XX DEVICE linker variables ***/

extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;
extern uint32_t __stack_limit__;
extern uint32_t __stack_top__;

/*** LPC55XX DEVICE local global variables ***/

static uint8_t stack[LPC55XX_DEVICE_STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

#if (LPC55XX_DEVICE_HEAP_SIZE > 0)
static uint8_t heap[LPC55XX_DEVICE_HEAP_SIZE] __attribute__ ((aligned(8), used, section(".heap")));
#endif

const pFunc __Vectors[] __attribute__ ((section(".vectors"))) = {
    // Cortex-M33 interrupts.
    (pFunc) ((uint32_t) &__stack_top__),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    SecureFault_Handler,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    // Device interrupts.
    WDT_BOD_IRQHandler,
    DMA0_IRQHandler,
    GINT0_IRQHandler,
    GINT1_IRQHandler,
    PIN_INT0_IRQHandler,
    PIN_INT1_IRQHandler,
    PIN_INT2_IRQHandler,
    PIN_INT3_IRQHandler,
    UTICK0_IRQHandler,
    MRT0_IRQHandler,
    CTIMER0_IRQHandler,
    CTIMER1_IRQHandler,
    SCT0_IRQHandler,
    CTIMER3_IRQHandler,
    FLEXCOMM0_IRQHandler,
    FLEXCOMM1_IRQHandler,
    FLEXCOMM2_IRQHandler,
    FLEXCOMM3_IRQHandler,
    FLEXCOMM4_IRQHandler,
    FLEXCOMM5_IRQHandler,
    FLEXCOMM6_IRQHandler,
    FLEXCOMM7_IRQHandler,
    ADC0_IRQHandler,
    Reserved39_IRQHandler,
    ACMP_IRQHandler,
    Reserved41_IRQHandler,
    Reserved42_IRQHandler,
    USB0_NEEDCLK_IRQHandler,
    USB0_IRQHandler,
    RTC_IRQHandler,
    Reserved46_IRQHandler,
    MAILBOX_IRQHandler,
    PIN_INT4_IRQHandler,
    PIN_INT5_IRQHandler,
    PIN_INT6_IRQHandler,
    PIN_INT7_IRQHandler,
    CTIMER2_IRQHandler,
    CTIMER4_IRQHandler,
    OS_EVENT_IRQHandler,
    Reserved55_IRQHandler,
    Reserved56_IRQHandler,
    Reserved57_IRQHandler,
    SDIO_IRQHandler,
    Reserved59_IRQHandler,
    Reserved60_IRQHandler,
    Reserved61_IRQHandler,
    USB1_PHY_IRQHandler,
    USB1_IRQHandler,
    USB1_NEEDCLK_IRQHandler,
    SEC_HYPERVISOR_CALL_IRQHandler,
    SEC_GPIO_INT0_IRQ0_IRQHandler,
    SEC_GPIO_INT0_IRQ1_IRQHandler,
    PLU_IRQHandler,
    SEC_VIO_IRQHandler,
    HASHCRYPT_IRQHandler,
    CASER_IRQHandler,
    PUF_IRQHandler,
    PQ_IRQHandler,
    DMA1_IRQHandler,
    FLEXCOMM8_IRQHandler,
};

/*** LPC55XX DEVICE functions ***/

/*******************************************************************/
void Reset_Handler(void) {
    // Local variables.
    uint32_t* pSrc;
    uint32_t* pDest;
    uint32_t *pTable;
    // Disable interrupts
    __asm volatile ("cpsid i");
    // Config VTOR & MSPLIM register
    __asm volatile ("LDR R0, =0xE000ED08  \n"
                    "STR %0, [R0]         \n"
                    "LDR R1, [%0]         \n"
                    "MSR MSP, R1          \n"
                    "MSR MSPLIM, %1       \n"
                    :
                    : "r"(__Vectors), "r"(&__stack_limit__)
                    : "r0", "r1");
    // Init RAMs.
    pTable = &__copy_table_start__;
    for (; pTable < &__copy_table_end__; pTable = pTable + 3) {
        pSrc  = (uint32_t*) *(pTable + 0);
        pDest = (uint32_t*) *(pTable + 1);
        for (; pDest < (uint32_t*) (*(pTable + 1) + *(pTable + 2)); ) {
            *pDest++ = *pSrc++;
        }
    }
    // Clear BSS sections.
    pTable = &__zero_table_start__;
    for (; pTable < &__zero_table_end__; pTable = pTable + 2) {
        pDest = (uint32_t*) *(pTable + 0);
        for (; pDest < (uint32_t*) (*(pTable + 0) + *(pTable + 1)); ) {
            *pDest++ = 0;
        }
    }
    // Enable interrupts
    __asm volatile ("cpsie i");
    // Start application.
    main();
}

/*******************************************************************/
void Default_Handler(void) {
    // Enter sleep mode.
    while (1) {
        __asm volatile ("wfi");
    }
}
