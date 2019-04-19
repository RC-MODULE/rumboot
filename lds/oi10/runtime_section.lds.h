    .runtime 0x8001fd00 (NOLOAD) : { /* TODO: always update address with rtl/testbench/gtube_inst.sv RUMBOOT_RUNTIME_ADDR */
        /* Runtime info is at the very top of iram */
        rumboot_platform_runtime_info = .;
        *(.rumboot_platform_runtime_info);
    }
