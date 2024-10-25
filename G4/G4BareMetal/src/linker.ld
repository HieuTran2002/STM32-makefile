/* Entry Point */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 32K
}

/* Sections */
SECTIONS
{
    /* .data section in SRAM, initialized from Flash */
    .data : {
        _sdata = .;
        *(.data)
        _edata = .;
    } >RAM AT>FLASH

    /* Initialization values in Flash */
    .rodata : {
        _sidata = .;
        *(.rodata)
    } >FLASH

    /* .bss section in SRAM, zero-initialized */
    .bss : {
        _sbss = .;
        *(.bss)
        _ebss = .;
    } >RAM

    /* Stack pointer initialization */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
