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
#include "../src/platform/oi10/targets/l2scrubber/glue.h"
#include <devices/ugly/emi.h>

/* This file is the OI10 glue for scrubber framework */

static const tlb_entry sram0_tlb_entry[] = {
//        MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 ) },
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 ) }
};


static emi_bank_cfg bank_cfg =
{
    //SS0
    {
        BTYP_SRAM,
        PTYP_NO_PAGES,
        SRDY_EXT_RDY_NOT_USE,
        TWR_0,
        SST_Flow_Through,
        TSSOE_1,
        TSOE_1,
        TCYC_3,
        0, //T_RDY
        TDEL_1
    },
    //SD0
    {
        CSP_256,
        SDS_2M,
        CL_3,
        TRDL_1,
        SI_EXT_INIT,
        TRCD_5,
        TRAS_9
    }
};

int do_memfill32(void *addr, size_t sz, int val, int incr)
{
    int i;
    uint32_t *dst = addr;
    for(i=0; i < sz; i++) {
      dst[i] = val; 
      val += incr;
    }
    return 0;
}


int main()
{
    dcr_write(0x80000600, 0xC10);
    dcr_write(0x80000604, 0x1);
    rumboot_platform_runtime_info->silent = 0;
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, &bank_cfg);
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));
    struct l2_scrubber *scr = l2_scrubber_create(DCR_L2C_BASE, L2C0_INTERRUPTOUT);
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    do_memfill32(l2scrubber_tests_get_cachable_area(), 128, 0, 1);
    asm("msync");
    asm("isync"); 

    for (int j=0; j < 512; j = j + 8) {
        ioread64(j);
    }
    rumboot_printf("l2c: Init complete\n");
    rumboot_irq_sei();

    l2_inject_data_fault(DCR_L2C_BASE, 0, 1);
    l2_inject_data_fault(DCR_L2C_BASE, 1, 1);

    for (int j=0; j < 512; j = j + 8) {
        ioread64(j);
    }

    return 0;
}
