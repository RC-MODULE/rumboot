#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <regs/regs_mkio.h>
#include <regs/regs_spi.h>

#include <devices/mkio.h>
#include <devices/gpio_pl061.h>
#include <devices/pl022.h>
#include <devices/pl022_flash.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/regs/fields/dma2plb6.h>
#include <platform/regs/regs_dma2plb6.h>
#include <platform/devices/hscb.h>
#include <platform/devices/dma2plb6.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>
#include <platform/devices/greth.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <rumboot/hexdump.h>

#if 0
void cpu_test()
{
    rumboot_hexdump((void *) NOR_BASE, 2048);
    uint32_t start = rumboot_platform_get_uptime();
    ((void (*)())(NOR_BASE))();
    uint32_t end = rumboot_platform_get_uptime();
    rumboot_printf("Iteration took %d us\n", end - start);
}
#else 
extern void test_oi10_power();
void cpu_test() {
    uint32_t start = rumboot_platform_get_uptime();
    test_oi10_power();
    uint32_t end = rumboot_platform_get_uptime();
    rumboot_printf("Iteration took %u us\n", end - start);
}
#endif

//#define CACHE_IT
// Iteration took 122138 us

int main(void)
{
    rumboot_printf("Writing TLB entries...\n");
    
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
    { MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 ) },
#ifdef CACHE_IT
#endif
    };
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));


    rumboot_printf("Start test_oi10_power_3\n");
    cpu_test();
    rumboot_printf("We're live!\n");

    return 0;
}
