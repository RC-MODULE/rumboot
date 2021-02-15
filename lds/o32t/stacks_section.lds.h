    .stacks 0x8001bd00 (NOLOAD) : { /* TODO: always update address with rtl/testbench/gtube_inst.sv RUMBOOT_RUNTIME_ADDR-0x4000 */
        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x4000; /* 16k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0
