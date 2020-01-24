
MEMORY
{
    code_wb            (rwx) : ORIGIN = (0x0), LENGTH = 4K
    data_wb            (rw)  : ORIGIN = (0x1000), LENGTH = 4K
}

ENTRY(_boot_entry)
SECTIONS
{
    .header :
    {
        KEEP(*(.header))
    } > code_wb = 0xff

    .text : ALIGN(8)
    {
        KEEP(*(.text_start))
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
    } > code_wb = 0xff

    .rodata : ALIGN(8)
    {
        *(.rodata)
        *(.rodata.*)
        *(.fprinit)
        CONSTRUCTORS
    } > code_wb = 0xff

    .data : ALIGN(8)
    {
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        . = ALIGN(8);
    } > data_wb AT>data_wb = 0xff

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
    } > data_wb = 0xff

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
    } > data_wb = 0xff
}
