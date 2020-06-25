ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x080000000,LENGTH =1024K
    SRAM(rwk):ORIGIN =0x020000000,LENGTH =128K
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.rodata)
        _etext = .; /* Store the value of location counter into symbol */
    }> FLASH

    .data :
    {
        _sdata = .;
        *(.data)
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    }> SRAM
}

