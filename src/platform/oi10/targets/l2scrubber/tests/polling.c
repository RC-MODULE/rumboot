#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <stdio.h>
#include <inttypes.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>
#include <rumboot/timer.h>

static const tlb_entry sram0_tlb_entry[] = {
//        MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 ) },
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1MB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 ) }
};

int main()
{

    dcr_write(0x80000600, 0xC10);
    dcr_write(0x80000604, 0x1);
    rumboot_platform_runtime_info->silent = 0;
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    rumboot_printf("l2c: size %d bytes, data %d elements\n", 
        layout.l2size_bytes, layout.data_array_size);


    struct l2_scrubber *scr = l2_scrubber_create(DCR_L2C_BASE, -2);
    memset(0x0, 0xa5, layout.l2size_bytes);
    l2_scrubber_enable_pattern_scrubbing(scr, 0x5a5a5a5a5a5a5a5aUL);


    for (int j=0; j < layout.l2size_bytes; j = j + 4) {
        l2_inject_data_fault(DCR_L2C_BASE, 1, 1);
        l2_inject_data_fault(DCR_L2C_BASE, 1, 2);
    }

    for (int j=0; j < layout.l2size_bytes; j = j + 4) {
        ioread32(j);
        if (l2c_l2_read(DCR_L2C_BASE, L2C_L2ARRSTAT2)) {
            l2_scrubber_scrub_once(scr);
        }
    }

    l2_scrubber_dump_stats(scr);
    return 0;
}
