#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>
#include <platform/devices/dma2plb6.h>

#include <devices/crg.h>
#include <devices/sp805.h>
#include <regs/regs_crg.h>
#include <regs/regs_sp805.h>


#include <platform/regs/regs_dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <platform/regs/regs_plb6bc.h>
#include <platform/regs/regs_dcrarb.h>
#include <platform/regs/regs_p64.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>
#include <platform/regs/regs_l2c_pmu.h>

//                                 MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_EM2_CACHE_WT_0   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_EM2_CACHE_WT_1   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define SIZE_4KB    0x1000
#define SIZE_16KB   0x4000

#define DATA_SIZE   SIZE_4KB
#define DATA_OFFSET (DATA_SIZE + DATA_SIZE)

uint32_t src_address_array[] =
{
    SRAM0_BASE+0x20000,
    SDRAM_BASE,
    SSRAM_BASE,
    PIPELINED_BASE,
    SRAM1_BASE,
    IM1_BASE,
    IM2_BASE
};

static volatile bool dma2plb6_handler = false;

static void handler_dma2plb6_ch0()
{
    rumboot_putstring ("irq handler (DMA, ch0)\n");
    dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel0);
    dma2plb6_handler = true;
}

static void handler_system_hung()
{
    dcr_write(DCR_CRG_BASE + CRG_WR_LOCK, CRG_UNLOCK_CODE);
    dcr_write(DCR_CRG_BASE + CRG_RST_GFG2, (dcr_read(DCR_CRG_BASE + CRG_RST_GFG2) & 0x103000F) + 0x40020);

    //rumboot_platform_perf("reset_system");
    struct sp805_conf config_FREE_RUN =
    {
           .mode = FREERUN,
           .interrupt_enable = 1,
           .clock_division = 1,
           .width = 32,
           .load = 5,
    };
    dcr_write(DCR_WATCHDOG_BASE + WD_REG_CONTROL, dcr_read(DCR_WATCHDOG_BASE + WD_REG_CONTROL) | WD_CTRL_RESEN);
    sp805_unlock_access(DCR_WATCHDOG_BASE);
    sp805_write_to_itcr(DCR_WATCHDOG_BASE, 0b0); //set up normal mode
    sp805_config(DCR_WATCHDOG_BASE, &config_FREE_RUN);
    sp805_enable(DCR_WATCHDOG_BASE);

    while(1);
}

void init_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, DMA2PLB6_DMA_IRQ_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_dma2plb6_ch0, ( void* ) 0);
    rumboot_irq_set_handler( tbl, O_SYSTEM_HUNG, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_system_hung, ( void* ) 0);
    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( DMA2PLB6_DMA_IRQ_0 );
    rumboot_irq_enable( O_SYSTEM_HUNG );
    rumboot_irq_sei();
}

void configure_dma(uint32_t base_addr, DmaChannel channel, uint32_t addr_src, uint32_t addr_dst, uint32_t count)
{
    //default
    dma2plb6_setup_info dma_info = {
            .base_addr = base_addr,
            .source_adr = rumboot_virt_to_phys((void*)addr_src),
            .dest_adr = rumboot_virt_to_phys((void*)addr_dst),
            .priority = priority_medium_low,
            .striding_info.striding = striding_none,
            .tc_int_enable = true,
            .err_int_enable = true,
            .int_mode = int_mode_level_wait_clr,
            .channel = channel,
            .rw_transfer_size = rw_tr_size_1q,
            .transfer_width = tr_width_byte,
            .count = count,
            .snp_mode = snp_mode_off
    };
    dma2plb6_mcpy_init(&dma_info);
}

int main ()
{
    rumboot_putstring("Check CRG_RST_MON\n");
    TEST_ASSERT( (dcr_read(DCR_CRG_BASE + CRG_RST_MON) & 4) == 0, "TEST ERROR: SYSTEM HUNG detected" ); //check last reset - if WD then ERR

    emi_init(DCR_EM2_EMI_BASE);

    //init data
    rumboot_putstring("Init data\n");
    for (uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++)
    {
        rumboot_memfill8((void*)src_address_array[i], DATA_SIZE, 0x00, 0x01);
        rumboot_memfill8((void*)(src_address_array[i]+DATA_OFFSET), DATA_SIZE, 0x00, 0x01);
        rumboot_memfill8_modelling((void*)(src_address_array[i]+DATA_SIZE), DATA_SIZE, 0x00, 0x00);
        rumboot_memfill8_modelling((void*)(src_address_array[i]+DATA_OFFSET+DATA_SIZE), DATA_SIZE, 0x00, 0x00);
    }

    rumboot_putstring("Init handlers");
    init_handlers();

    /*
     *  Special algorithm:
     *  1) copy im0->sram0
     *  2) tlb substitution (im0->sram0)
     *  3) cache test code and stacks (in L1)
     *
     *  because if SYSTEM HUNG detected, the code can only be called from L1I
     */

    rumboot_putstring("copy im0->sram0");
    memcpy((void *) SRAM0_BASE, (void *) IM0_BASE, 0x20000);
    msync();

    rumboot_putstring("Set tlb (WT)");
    static const tlb_entry em0_tlb_entry_wt[] = {{TLB_ENTRY_EM2_CACHE_WT_0}, {TLB_ENTRY_EM2_CACHE_WT_1}} ;
    write_tlb_entries(em0_tlb_entry_wt,ARRAY_SIZE(em0_tlb_entry_wt));

    rumboot_putstring("tlb substitution (im0->sram0)");
    //                                       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
    #define TLB_ENTRY_IM0_TO_SRAM0_VADDR0    MMU_TLB_ENTRY(  0x000,  0x00000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )
    #define TLB_ENTRY_IM0_TO_SRAM0_VADDR1    MMU_TLB_ENTRY(  0x000,  0x00010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )
    static const tlb_entry im0_to_sram0_tlb[] = {{TLB_ENTRY_IM0_TO_SRAM0_VADDR0}, {TLB_ENTRY_IM0_TO_SRAM0_VADDR1}} ;
    write_tlb_entries(im0_to_sram0_tlb, ARRAY_SIZE(im0_to_sram0_tlb));

    rumboot_putstring("cache test code (in L1I)");
    for (uint32_t addr = IM0_BASE; addr < (IM0_BASE + SIZE_16KB); addr += 4) {
      icbtls(0, addr);
    }
    rumboot_putstring("cache stack (in L1D)\n");
    extern char rumboot_platform_stack_area_start;
    extern char rumboot_platform_stack_area_end;
    for (uint32_t addr = (uint32_t) (&rumboot_platform_stack_area_start); addr < (uint32_t) (&rumboot_platform_stack_area_end); addr += 4) {
      dcbtls(0, addr);
    }
    //end "Special algorithm"

    msr_write(msr_read() & (~0x1000));

    //generate_P6BC_O_SYSTEM_HUNG
    uint32_t l2c_dcr_base = DCR_L2C_BASE;
    rumboot_putstring("Check O_SYSTEM_HUNG generation\n");
    l2c_pmu_write(l2c_dcr_base, L2C_L2PLBMCKEN1, 0x0);
    l2c_pmu_write(l2c_dcr_base, L2C_L2CPUMCKEN, 0x0);
    l2c_l2_write(l2c_dcr_base, L2C_L2RACCFG,l2c_l2_read(l2c_dcr_base, L2C_L2RACCFG) | (1 << L2C_L2RACCFG_DisablePlbErrScrub_i));
    l2c_l2_write(l2c_dcr_base, L2C_L2WACCFG,l2c_l2_read(l2c_dcr_base, L2C_L2WACCFG) | (1 << L2C_L2WACCFG_DisablePlbErrScrub_i));
    dcr_write(DCR_PLB6_BC_BASE + PLB6BC_HCPP, PLB6BC_HCPP_4K);

    static const tlb_entry tlb_entry_fake[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x040,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( tlb_entry_fake, ARRAY_SIZE(tlb_entry_fake) );

    iowrite32 (0x00, 0x00000000);
    ioread32(0x00000000);

    while(1);
    rumboot_putstring("Data size");
    rumboot_puthex(DATA_SIZE);
    rumboot_putstring("\n");

    for (uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++)
    {
        rumboot_putstring("Start check");
        rumboot_puthex(src_address_array[i]);

        rumboot_putstring("Start DMA");
        configure_dma(DCR_DMAPLB6_BASE, channel0, src_address_array[i], src_address_array[i] + DATA_SIZE, DATA_SIZE);
        dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel0);

        rumboot_putstring("Start write-read");
        uint32_t addr = (src_address_array[i] + DATA_OFFSET);
        while (!dma2plb6_handler)
        {
            iowrite32(ioread32(addr), addr + DATA_SIZE);
            addr+=4;
        }
        dma2plb6_handler = false;
    }

    rumboot_putstring("TEST OK\n");
    return 0;
}
