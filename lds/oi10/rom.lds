MEMORY {
    IM0 (rwx)   : ORIGIN = 0x80000000, LENGTH = 0x20000
    IM1 (rwx)   : ORIGIN = 0x80020000, LENGTH = 0x20000
    ROM (rx)    : ORIGIN = 0xFFFF0000, LENGTH = 0x10000
}

SECTIONS {

    . = 0x00000000;

    .text : {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(32);
    } > ROM

    .init.text 0xFFFFF000 : {
        KEEP(*(.init.text))
        KEEP(*(.init.rodata))
        *(*.init.text)
        *(*.init.rodata)
    } > ROM

    .init.start 0xFFFFFFFC : {
        KEEP(*(.init.start))
    } > ROM

    .rw : {
        . = ALIGN(8);

        rumboot_platform_spl_start = .;
        . = . + 0x10000; /* spl image start */
        rumboot_platform_spl_end = .;

        . = ALIGN(8);

        rumboot_im0_heap_start = .;
        . = . + 0x8000; /* 32k heap */
        rumboot_im0_heap_end = .;

        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x7f00; /* 32k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    .runtime 0x8001ff00 (NOLOAD) : {
       /* Runtime info is at the very top of iram */
       rumboot_platform_runtime_info = .;
       *(.rumboot_platform_runtime_info);
    }

    .im1_heap 0x80020000 (NOLOAD) : {
        rumboot_im1_heap_start = .;
        . = . + 0x20000; /* 128k heap */
        rumboot_im1_heap_end = .;
    } > IM1

    /DISCARD/ : {
        *(.rela.*)
        *(.got2)
        *(.got)
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
        /* Make sure data & bss go to hell */
        *(.data)
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }

}
