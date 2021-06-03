_STACK_SIZE = 0x400;

MEMORY
{
    sram_wb            (rwx) : ORIGIN = (0x0), LENGTH = 256K
}

ENTRY(_boot_entry)
SECTIONS
{
    .header :
    {
        KEEP(*(.header))
    } > sram_wb = 0xff

    .loader_text :
    {
        KEEP(*(.boot_entry))
        KEEP(*(.loader_text))
        . = ORIGIN(sram_wb) + (1024);
    } > sram_wb = 0xff

   .text.export : ALIGN(8)
    {
    	flash_img_start = .;
        KEEP(*(.text.export.system))
        . = ALIGN(128);
    }>sram_wb = 0xff

    .text.export.drv : ALIGN(128)
    {
        KEEP(SORT(*)(.text.export.drv))
        . = ALIGN(128);
    }>sram_wb = 0xff

    .vectors : { KEEP(*(.vectors)) } > sram_wb = 0xff

    .ppcinit : { KEEP(*(.ppcinit)) } > sram_wb = 0xff

    .text : ALIGN(8)
    {
        KEEP(*(.text_start))
        *(.text)
        *(.text.*)
        . = 0x86b4;
        *(.memcpy_copyed)
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

    flash_img_end = .;

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

    .testdata 0x20000 :
    {
        . = 0xbdc;
        *(.testdata.monitor_version)
        . = 0x1fec;
        *(.testdata.tx_buf)
    }

    __data_size = __data_end - __data_start;
    __data_size_in_words = __data_size / 4;

    flash_img_size = flash_img_end - flash_img_start + __data_size;
    flash_img_size_ = flash_img_end - flash_img_start;

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

    .stack (NOLOAD) : ALIGN(16)
    {
        _stack_start = .;
        . = . + _STACK_SIZE;
        . = ALIGN(16);
        __stack = .;
        _stack_base = .;
        _stack_end = _stack_base;
    }  > sram_wb = 0xff
/*
    .heap  : ALIGN(16)
    {
        _heap_start = .;
        . = . + _HEAP_SIZE;
        . = ALIGN(16);
        _heap_end = .;
    } > SRAM_EXT
*/
}
