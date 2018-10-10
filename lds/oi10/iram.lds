INCLUDE oi10/memory.lds.h

SECTIONS {

    . = 0x00000000;

    .rwx : {
        KEEP(*(.header))
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        *(.sdata2)
        *(.sdata2.*)

        /* ROM sections copy */
        *(*.init.text)
        /* ROM sections copy end */

        rumboot_platform_bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        rumboot_platform_bss_end = .;

        . = ALIGN(32);

        rumboot_platform_spl_start = .;
        rumboot_platform_spl_end = .;

        . = ALIGN(8);

        rumboot_im0_heap_start = .;
        . = . + 0x4000; /* 16k heap */
        rumboot_im0_heap_end = .;

        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x4000; /* 16k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    .runtime 0x8001fe00 (NOLOAD) : {
        /* Runtime info is at the very top of iram */
        rumboot_platform_runtime_info = .;
        *(.rumboot_platform_runtime_info);
    }

    rumboot_im1_heap_start      = IM1_MEMORY_BASE;
    rumboot_im1_heap_end        = IM1_MEMORY_BASE + IM1_MEMORY_SIZE;

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

    /DISCARD/ : {
        *(.init.text)
        *(.init.rodata)
        *(*.init.rodata)
        *(.init.start)
    }

}
