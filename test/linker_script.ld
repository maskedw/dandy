MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024k
  SRAM2 (rwx) : ORIGIN = 0x10000188, LENGTH = 32k - 0x188
  SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 96k
}
ENTRY(Reset_Handler)
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        *(.rodata*)
        KEEP(*(.eh_frame*))
    } > FLASH
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
    __etext = .;
    _sidata = .;
    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end__ = .;
        _edata = .;
    } > SRAM1
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
    } > SRAM1
    .heap (COPY):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        . += (ORIGIN(SRAM1) + LENGTH(SRAM1) - .);
        __HeapLimit = .;
    } > SRAM1
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));
    ASSERT((ORIGIN(SRAM1)+LENGTH(SRAM1)) >= __HeapLimit, "SRAM1 overflow")
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > SRAM2
    __StackTop = ORIGIN(SRAM2) + LENGTH(SRAM2);
    _estack = __StackTop;
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    ASSERT((ORIGIN(SRAM2)+LENGTH(SRAM2)) >= __StackLimit, "SRAM2 overflow")
}
