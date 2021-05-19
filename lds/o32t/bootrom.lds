INCLUDE o32t/memory.lds.h

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

        rumboot_im0_heap_start = .;
        . = . + 0x8000; /* 32k heap */
        rumboot_im0_heap_end = .;
    } > IM0

    INCLUDE o32t/stacks_section.lds.h
    INCLUDE o32t/runtime_section.lds.h


    /* SPL area covers IM1 to IM3 */
    rumboot_platform_spl_start = IM1_MEMORY_BASE;
    rumboot_platform_spl_end = IM1_MEMORY_BASE + IM1_MEMORY_SIZE + IM2_MEMORY_SIZE + IM3_MEMORY_SIZE;

    rumboot_im1_heap_start      = IM1_MEMORY_BASE;
    rumboot_im1_heap_end        = IM1_MEMORY_BASE + IM1_MEMORY_SIZE;

    rumboot_im2_heap_start      = IM2_MEMORY_BASE;
    rumboot_im2_heap_end        = IM2_MEMORY_BASE + IM2_MEMORY_SIZE;

    rumboot_sram0_heap_start    = SRAM0_MEMORY_BASE;
    rumboot_sram0_heap_end      = SRAM0_MEMORY_BASE + SRAM0_MEMORY_SIZE;

    rumboot_sdram_heap_start    = SDRAM_MEMORY_BASE;
    rumboot_sdram_heap_end      = SDRAM_MEMORY_BASE + SDRAM_MEMORY_SIZE;

    rumboot_ssram_heap_start    = SSRAM_MEMORY_BASE;
    rumboot_ssram_heap_end      = SSRAM_MEMORY_BASE + SSRAM_MEMORY_SIZE;

    rumboot_plram_heap_start    = PLRAM_MEMORY_BASE;
    rumboot_plram_heap_end      = PLRAM_MEMORY_BASE + PLRAM_MEMORY_SIZE;

    rumboot_sram1_heap_start    = SRAM1_MEMORY_BASE;
    rumboot_sram1_heap_end      = SRAM1_MEMORY_BASE + SRAM1_MEMORY_SIZE;

    rumboot_nor_heap_start      = NOR_MEMORY_BASE;
    rumboot_nor_heap_end        = NOR_MEMORY_BASE + NOR_MEMORY_SIZE;

    rumboot_im3_heap_start      = IM3_MEMORY_BASE;
    rumboot_im3_heap_end        = IM3_MEMORY_BASE + IM3_MEMORY_SIZE;

    /DISCARD/ : {
        *(.rela.*)
        *(.got)
        *(.got.*)
        *(.got2)
        *(.got2.*)
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
        /* Make sure data & bss go to hell */
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        *(.sdata2)
        *(.sdata2.*)
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
    }

}
