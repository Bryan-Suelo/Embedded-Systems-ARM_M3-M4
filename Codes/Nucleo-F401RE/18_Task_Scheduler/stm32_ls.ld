ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000, LENGTH =512K
    SRAM(rwk):ORIGIN =0x22000000, LENGTH =96K
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.rodata)
        .= ALIGN(4);
        _etext = .; /* Store the value of location counter into symbol */
    }> FLASH

    .data :
    {
        _sdata = .;
        *(.data)
        .= ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        *(.bss);
        *(COMMON)
        .= ALIGN(4);
        _ebss = .;
    }> SRAM
}