/*
 * lpc55s69.ld
 *
 *  Created on: 08 mar. 2023
 *      Author: NXP
 */
 
__flash_size__ = 630K;
__sramx_size__ = 32K;
__sram_size__ = 256K;
__sram4_power_quad_size__ = 16K;
__sram_usb_size__ = 16K;

MEMORY
{
    FLASH (rx)             : ORIGIN = 0x00000000, LENGTH = __flash_size__
    SRAMX (rwx)            : ORIGIN = 0x04000000, LENGTH = __sramx_size__
    SRAM (rwx)             : ORIGIN = 0x20000000, LENGTH = __sram_size__
    SRAM4_POWER_QUAD (rwx) : ORIGIN = 0x20040000, LENGTH = __sram4_power_quad_size__
    SRAM_USB (rwx)         : ORIGIN = 0x40100000, LENGTH = __sram_usb_size__
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        KEEP(*(.Reset_Handler))
        KEEP(*(.Default_Handler))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        KEEP(*(.eh_frame*))
    } > FLASH
    
    .ARM.extab (READONLY) :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx (READONLY) :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
    
    .copy.table (READONLY) :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG(LOADADDR(.data));
        LONG(__data_sram_start__);
        LONG(__data_sram_end__ - __data_sram_start__);
        LONG(LOADADDR(.data_sramx));
        LONG(__data_sramx_start__);
        LONG(__data_sramx_end__ - __data_sramx_start__);
        LONG(LOADADDR(.data_sram4_power_quad));
        LONG(__data_sram4_power_quad_start__);
        LONG(__data_sram4_power_quad_end__ - __data_sram4_power_quad_start__);
        LONG(LOADADDR(.data_sram_usb));
        LONG(__data_sram_usb_start__);
        LONG(__data_sram_usb_end__ - __data_sram_usb_start__);
        __copy_table_end__ = .;
    } > FLASH

    .zero.table (READONLY) :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_sram_start__)
        LONG (__bss_sram_end__ - __bss_sram_start__)
        LONG (__bss_sramx_start__)
        LONG (__bss_sramx_end__ - __bss_sramx_start__)
        LONG (__bss_sram4_power_quad_start__)
        LONG (__bss_sram4_power_quad_end__ - __bss_sram4_power_quad_start__)
        LONG (__bss_sram_usb_start__)
        LONG (__bss_sram_usb_end__ - __bss_sram_usb_start__)
        __zero_table_end__ = .;
    } > FLASH

    __flash_end__ = ALIGN(4);
    
    .data : AT(__flash_end__)
    {
        . = ALIGN(4);
        __data_sram_start__ = .;
        *(vtable)
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        __data_sram_end__ = .;
    } > SRAM
 
    __flash_end_2__ = (__flash_end__ + __data_sram_end__ - __data_sram_start__);
    
    .data_sramx : AT(__flash_end_2__)
    {
        . = ALIGN(4);
        __data_sramx_start__ = .;
        *(.data_sramx)
        *(.data_sramx.*)
        . = ALIGN(4);
        __data_sramx_end__ = .;
    } > SRAMX
    
    __flash_end_3__ = (__flash_end_2__ + __data_sramx_end__ - __data_sramx_start__);

    .data_sram4_power_quad : AT(__flash_end_3__)
    {
        . = ALIGN(4);
        __data_sram4_power_quad_start__ = .;
        *(.data_sram4_power_quad)
        *(.data_sram4_power_quad.*)
        . = ALIGN(4);
        __data_sram4_power_quad_end__ = .;
    } > SRAM4_POWER_QUAD
    
    __flash_end_4__ = (__flash_end_3__ + __data_sram4_power_quad_end__ - __data_sram4_power_quad_start__);

    .data_sram_usb : AT(__flash_end_4__)
    {
        . = ALIGN(4);
        __data_sram_usb_start__ = .;
        *(.data_sram_usb)
        *(.data_sram_usb.*)
        . = ALIGN(4);
        __data_sram_usb_end__ = .;
    } > SRAM_USB
    
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_sram_start__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_sram_end__ = .;
    } > SRAM
    
    .bss_sramx (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_sramx_start__ = .;
        *(.bss_sramx)
        *(.bss_sramx.*)
        . = ALIGN(4);
        __bss_sramx_end__ = .;
    } > SRAMX
    
    .bss_sram4_power_quad (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_sram4_power_quad_start__ = .;
        *(.bss_sram4_power_quad)
        *(.bss_sram4_power_quad.*)
        . = ALIGN(4);
        __bss_sram4_power_quad_end__ = .;
    } > SRAM4_POWER_QUAD
    
    .bss_sram_usb (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_sram_usb_start__ = .;
        *(.bss_sram_usb)
        *(.bss_sram_usb.*)
        . = ALIGN(4);
        __bss_sram_usb_end__ = .;
    } > SRAM_USB
    
    .heap (COPY) :
    {
        __heap_start__ = .;
        KEEP(*(.heap*))
        __heap_end__ = .;
    } > SRAM

    .stack (COPY) :
    {
        KEEP(*(.stack*))
    } > SRAM

    __stack_top__ = ORIGIN(SRAM) + LENGTH(SRAM);
    __stack_limit__ = __stack_top__ - SIZEOF(.stack);

    ASSERT((__stack_limit__ >= __heap_end__), "Region SRAM overflowed with stack")
}