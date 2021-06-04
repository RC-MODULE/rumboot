MEMORY
{
    sram_wt (rwx) : ORIGIN = (0x400000), LENGTH = 64K
}


SECTIONS
{
    .header :
    {
        KEEP(*(.header))
    } > sram_wt = 0xff

    .text : ALIGN(8)
    {
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
    } > sram_wt = 0xff

    .rodata : ALIGN(8)
    {
        *(.rodata)
        *(.rodata.*)
        *(.fprinit)
        CONSTRUCTORS
    } > sram_wt = 0xff

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
    } > sram_wt AT>sram_wt = 0xff

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
    } > sram_wt = 0xff

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
    } > sram_wt = 0xff
}
