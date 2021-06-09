MEMORY
{
    sram_wb (rwx) : ORIGIN = (0x800000), LENGTH = 64K
}


SECTIONS
{
    .header :
    {
        KEEP(*(.header))
    } > sram_wb = 0xff

    .text : ALIGN(8)
    {
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
    } > sram_wb = 0xff

    .rodata : ALIGN(8)
    {
        *(.rodata)
        *(.rodata.*)
        *(.fprinit)
        CONSTRUCTORS
    } > sram_wb = 0xff

    __data_rom = LOADADDR(.data);

    .data : ALIGN(8)
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        . = ALIGN(8);
        __data_end = .;
    } > sram_wb AT>sram_wb = 0xff

    .sbss      :
    {
        __sbss_start = .;
        ___sbss_start = .;
        *(.sbss)
        *(.sbss.*)
        *(.gnu.linkonce.sb.*)
        *(.scommon)
        *(.dynsbss)
        __sbss_end = .;
        ___sbss_end = .;
    } > sram_wb = 0xff

    .bss       :
    {
    	_bss_start = .;
        __bss_start = .;
        ___bss_start = .;
        *(.dynbss)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
        _bss_end = .;
    } > sram_wb = 0xff
}
