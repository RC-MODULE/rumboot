#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/mmu.h>
#include <devices/mpic128.h>
#include <platform/devices.h>
#include <platform/test_event_c.h>
#include <rumboot/boot.h>
#include <rumboot/irq.h>



static void additional_tlb_configure(int enable)
{
    /* FixMe: Ugly fuckin' hack. */
    static const tlb_entry additional_tlb_entries_insert[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x001,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x001,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x001,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0300,    0xC0300,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0060,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0070,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0060,    0xC0060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0070,    0xC0070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )}
    };
    static const tlb_entry additional_tlb_entries_remove[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x000,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x000,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x001,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x001,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x001,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0300,    0xC0300,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0300,    0xC0300,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };  

    if (enable) {
        write_tlb_entries(additional_tlb_entries_insert, ARRAY_SIZE(additional_tlb_entries_insert));
    } else {
        write_tlb_entries(additional_tlb_entries_remove, ARRAY_SIZE(additional_tlb_entries_remove));        
    }
}
void rumboot_platform_exit(int status) {
    additional_tlb_configure(0);
    #ifdef PRODUCTION_TESTING
        if (status) {
        	rumboot_printf("\nHACK: _exit: System halted, code %d. \n", status);
        	while(1);;
        }
    #endif
}



int64_t rumboot_virt_to_phys(volatile void *addr)
{
    return get_physical_addr( (uint32_t) addr, 0);
}

uint32_t rumboot_virt_to_dma(volatile const void *addr)
{
    uint64_t ret = get_physical_addr( (uint32_t)  addr, 0);
    return (uint32_t) (ret & 0xFFFFFFFF);
}

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime() {
    #define TIMER_TICKS_PER_US  200 /* TODO check if PPC_TMR_CLK is 5ns */

    uint32_t h = spr_read( SPR_TBU_R );
    uint32_t l = spr_read( SPR_TBL_R );    
    if (h != spr_read( SPR_TBU_R ))
        return rumboot_platform_get_uptime();

    uint64_t v = h; 
    v = (v << 32) | l;
    return v / TIMER_TICKS_PER_US;
}

void rumboot_itrpt_hdr_base();
void rumboot_CI_hdr();
void rumboot_MC_hdr();
void rumboot_DS_hdr();
void rumboot_IS_hdr();
void rumboot_EI_hdr();
void rumboot_A_hdr();
void rumboot_P_hdr();
void rumboot_FPU_hdr();
void rumboot_SC_hdr();
void rumboot_APU_hdr();
void rumboot_DEC_hdr();
void rumboot_FIT_hdr();
void rumboot_WDT_hdr();
void rumboot_DTLBE_hdr();
void rumboot_ITLBE_hdr();
void rumboot_D_hdr();

static void enable_fpu()
{
    msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}

void rumboot_platform_setup() {
    /* Disable interrupts on the PPC core */
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    rumboot_irq_register_mpic128();

    spr_write( SPR_IVPR,    (uint32_t)&rumboot_itrpt_hdr_base & ITRPT_IVPR_ADDR_mask );    /* link irq handlers mirror */
    spr_write( SPR_IVOR0,   (uint32_t)&rumboot_CI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR1,   (uint32_t)&rumboot_MC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR2,   (uint32_t)&rumboot_DS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR3,   (uint32_t)&rumboot_IS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_EI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR5,   (uint32_t)&rumboot_A_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR6,   (uint32_t)&rumboot_P_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR7,   (uint32_t)&rumboot_FPU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR8,   (uint32_t)&rumboot_SC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR9,   (uint32_t)&rumboot_APU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR10,  (uint32_t)&rumboot_DEC_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR11,  (uint32_t)&rumboot_FIT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR12,  (uint32_t)&rumboot_WDT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR13,  (uint32_t)&rumboot_DTLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR14,  (uint32_t)&rumboot_ITLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR15,  (uint32_t)&rumboot_D_hdr        & ITRPT_IVORn_OFFSET_mask );

    msr_write( msr_old_value );

    /* Enable MEM_WINDOW */
    spr_write( SPR_SSPCR, ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB)   << MMU_SSPCR_ORD1_i)
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_1MB)    << MMU_SSPCR_ORD2_i)
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD3_i)
                        | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB)    << MMU_SSPCR_ORD4_i)
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_16KB)   << MMU_SSPCR_ORD5_i)
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_16MB)   << MMU_SSPCR_ORD6_i)
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD7_i) );
    set_mem_window(MEM_WINDOW_0);

#if !defined(RUMBOOT_ONLY_STACK)
    additional_tlb_configure(1);
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x10000, 0x00, 0x00); //workaround (init first 64KB in SRAM0)
#endif

#ifndef OI10_MINIMAL_INIT
    extern char rumboot_im0_heap_start;
    extern char rumboot_im0_heap_end;
    rumboot_malloc_register_heap( "IM0", &rumboot_im0_heap_start, &rumboot_im0_heap_end );

    extern char rumboot_im1_heap_start;
    extern char rumboot_im1_heap_end;
    rumboot_malloc_register_heap( "IM1", &rumboot_im1_heap_start, &rumboot_im1_heap_end );

    extern char rumboot_im2_heap_start;
    extern char rumboot_im2_heap_end;
    rumboot_malloc_register_heap( "IM2", &rumboot_im2_heap_start, &rumboot_im2_heap_end );
    
    extern char rumboot_im3_heap_start;
    extern char rumboot_im3_heap_end;
    rumboot_malloc_register_heap( "IM3", &rumboot_im3_heap_start, &rumboot_im3_heap_end );

    extern char rumboot_sram0_heap_start;
    extern char rumboot_sram0_heap_end;
    rumboot_malloc_register_heap("SRAM0", &rumboot_sram0_heap_start, &rumboot_sram0_heap_end);

    extern char rumboot_sdram_heap_start;
    extern char rumboot_sdram_heap_end;
    rumboot_malloc_register_heap("SDRAM", &rumboot_sdram_heap_start, &rumboot_sdram_heap_end);

    extern char rumboot_ssram_heap_start;
    extern char rumboot_ssram_heap_end;
    rumboot_malloc_register_heap("SSRAM", &rumboot_ssram_heap_start, &rumboot_ssram_heap_end);

    extern char rumboot_plram_heap_start;
    extern char rumboot_plram_heap_end;
    rumboot_malloc_register_heap("PLRAM", &rumboot_plram_heap_start, &rumboot_plram_heap_end);

    extern char rumboot_sram1_heap_start;
    extern char rumboot_sram1_heap_end;
    rumboot_malloc_register_heap("SRAM1", &rumboot_sram1_heap_start, &rumboot_sram1_heap_end);

    extern char rumboot_nor_heap_start;
    extern char rumboot_nor_heap_end;
    rumboot_malloc_register_heap("NOR", &rumboot_nor_heap_start, &rumboot_nor_heap_end);
    /* Set our own handler */
    rumboot_irq_set_exception_handler(rumboot_arch_exception);
#endif

    enable_fpu();

//#if !defined(RUMBOOT_ONLY_STACK)
    /* No-op on real OI10, enables l2 scrubbing on 32t */ 
    dcr_write(0x800E0010, 0x101);
//#endif

}
