    .runtime 0x8001fd00 (NOLOAD) : {
        /* Runtime info is at the very top of iram */
        rumboot_platform_runtime_info = .;
        *(.rumboot_platform_runtime_info);
    }
