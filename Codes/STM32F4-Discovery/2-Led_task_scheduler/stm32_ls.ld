ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000,LENGTH =1024K /* ROM */
    SRAM(rwk):ORIGIN =0x20000000,LENGTH =128K  /* SRAM1 116K SRAM2 16K */
}

SECTIONS
{
    .text :
    {
        /* text section of main.o led.o stm32_startup.o */
        /* first elements are: .isr_vector .text .rodata _etext */
        *(.isr_vector)
        *(.text)
        /**(.text.*)
        *(.init)
        *(.fini)*/
        *(.rodata)
       /* *(.rodata.*)*/
       /* .= ALIGN(4);*/
        _etext = .; /* Store the value of location counter into symbol _etext */
    }> FLASH

    /*_la_data = LOADADDR(.data);*/
    .data :
    {
        /* Initialize data */
        /* first elements are: .data */
        _sdata = .; /* location counter resets to start of VMA(SRAM) of this section */
        *(.data)
      /*  *(.data.*)*/
      /*  .= ALIGN(4);*/
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {        
        /* Unnitialize data */
        /* first elements are: .bss */
        _sbss = .;
       /* __bss_start__ = _sbss;*/
        *(.bss);
      /*  *(.bss.*);
        .= ALIGN(4);*/
        _ebss = .;
       /* __bss_end__ = _ebss;
        .= ALIGN(4);
        end = .;
        __end__ = .;*/
    }> SRAM
}

