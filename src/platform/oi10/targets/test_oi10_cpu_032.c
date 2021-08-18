#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_asm.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
#include <devices/ugly/emi.h>
#include <devices/mpic128.h>
#include <regs/fields/emi.h>
#include <platform/interrupts.h>
#include <arch/dma2plb6.h>

#include <platform/regs/regs_ltrace.h>
#include <platform/devices/ltrace.h>
#include <platform/regs/regs_itrace.h>
#include <platform/devices/itrace.h>

#include <arch/regs/dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <arch/regs/plb6bc.h>
#include <platform/regs/regs_dcrarb.h>
#include <arch/regs/regs_p64.h>


#define ITRACE_HANG_ADDR    0x00001FFF
#define NONE_HANG           0x404E8A46
#define SYSTEM_HANG         0x51578A46
#define HANG_CLEAR          0x8A46C1EA


enum {
    START_STATUS    = 0,
    STATUS_STARTED  = 0x57A47ED1
};

typedef enum {
    WDRT_system_hang        = TIMER_TCR_WRC_Core_reset,
    WDRT_no_hang_on_load    = TIMER_TCR_WRC_Chip_reset,
    WDRT_other              = TIMER_TCR_WRC_System_reset
} watchdog_reset_type;


static void init_handlers();
static void start_wd( watchdog_reset_type reset_type );
static void move_execution_to_l1();
static void generate_system_hang();
static void check_system_hang();
static void check_no_hang_on_load();

int main () {
    dcr_write( DCR_ITRACE_BASE+ITC0_ITAI, ITRACE_HANG_ADDR );
    if(rumboot_platform_runtime_info->persistent[START_STATUS] != STATUS_STARTED ) {
        rumboot_platform_runtime_info->persistent[START_STATUS] = STATUS_STARTED;

        dcr_write( DCR_ITRACE_BASE+ITC0_ITAD, NONE_HANG );
    }

    emi_init(DCR_EM2_EMI_BASE);

    init_handlers();

    uint32_t const prev_hang_status = dcr_read( DCR_ITRACE_BASE+ITC0_ITAD );
    rumboot_putstring( "Previous SYSTEM HANG value:" ); // TODO: remove or comment
    rumboot_puthex( prev_hang_status );

    switch( prev_hang_status ) {
        case NONE_HANG: {
            rumboot_putstring( "Checking SYSTEM HANG..." );
            generate_system_hang();
            break;
        }
        case SYSTEM_HANG: {
            check_system_hang();
            dcr_write( DCR_ITRACE_BASE+ITC0_ITAD, HANG_CLEAR );

            check_no_hang_on_load();
            return 0;
        }
        default:
            start_wd( WDRT_other );
    }

    return 1;
}

/* <<<<<<<<<<Implementation>>>>>>>>>> */

static void start_wd( watchdog_reset_type const reset_type ) {
    rumboot_putstring( "Start Watchdog" );

    spr_write( SPR_TBL_W, 0 );    // reset counter
    spr_write( SPR_TCR, (TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i)
                      | (reset_type                 << TIMER_TCR_WRC_i)
                      | (0b1                        << TIMER_TCR_WIE_i)
                      | (0b0                        << TIMER_TCR_DIE_i)
                      | (0b0                        << TIMER_TCR_FIE_i) );

    rumboot_platform_perf( "Start Watchdog" );
    spr_write( SPR_TBL_W, FIELD_MASK32(8, (20-8)) );    // speedup WD triggering (2^8 clocks) for TIMER_TCR_WP_2pow21_clocks
    while( !(spr_read(SPR_TBL_R) & (0b1 << 20)) );      // wait trigger
    spr_write( SPR_TBL_W, FIELD_MASK32(8, (20-8)) );    // speedup WD triggering (2^8 clocks) for TIMER_TCR_WP_2pow21_clocks
}


#define PLB6BC_SHD          0x10
#define MPIC128_IAR_PR      0x000000E0
#define MPIC128_EOI_PR      0x000000F0

asm (
ASM_TEXT(   .section ".text.rom","ax",@progbits                         )

ASM_TEXT(   .align 4                                                    )
ASM_TEXT(   rumboot_CI_hdr_save_system_hung:                            )
ASM_TEXT(       mtspr           SPR_USPRG0, r3                          )
ASM_TEXT(       mtspr           SPR_SPRG8, r4                           )
ASM_TEXT(       load_const      r3, DCR_PLB6_BC_BASE+PLB6BC_SHD         )
ASM_TEXT(       mfdcrx          r4, r3                                  )   // read SYSTEM_HANG status
ASM_TEXT(       mfcr            r3                                      )
ASM_TEXT(       cmpwi           r4, 0x00                                )
ASM_TEXT(       beq             rumboot_CI_hdr_restore_regs             )   // if( !SYSTEM_HANG ) goto  rumboot_CI_hdr_restore_regs
ASM_TEXT(       mtcr            r3                                      )
ASM_TEXT(       load_const      r3, DCR_ITRACE_BASE+ITC0_ITAD           )
ASM_TEXT(       load_const      r4, SYSTEM_HANG                         )
ASM_TEXT(       mtdcrx          r3, r4                                  )   // save SYSTEM_HANG status in itrace
ASM_TEXT(       load_const      r3, DCR_MPIC128_BASE+MPIC128_IAR_PR     )
ASM_TEXT(       mfdcrx          r4, r3                                  )   // read MPIC interrupt acknowledge
ASM_TEXT(       load_const      r3, DCR_MPIC128_BASE+MPIC128_EOI_PR     )
ASM_TEXT(       load_const      r4, 0                                   )
ASM_TEXT(       mtdcrx          r3, r4                                  )   // write MPIC interrupt end
ASM_TEXT(       mfspr           r4, SPR_SPRG8                           )
ASM_TEXT(       mfspr           r3, SPR_USPRG0                          )
ASM_TEXT(       rfci                                                    )   // exit from rumboot_CI_hdr_save_system_hung
ASM_TEXT(   rumboot_CI_hdr_restore_regs:                                )
ASM_TEXT(       mtcr            r3                                      )
ASM_TEXT(       mfspr           r4, SPR_SPRG8                           )
ASM_TEXT(       mfspr           r3, SPR_USPRG0                          )
ASM_TEXT(       b               rumboot_CI_hdr                          )   // goto default rumboot_CI_hdr

ASM_TEXT(   .align 4                                                    )
ASM_TEXT(   rumboot_WDT_hdr_speedup:                                    )
ASM_TEXT(       mtspr           SPR_USPRG0, r3                          )
ASM_TEXT(       load_const      r3, FIELD_MASK32(16, (20-16))           )   // speedup WD triggering (2^16 clocks) for TIMER_TCR_WP_2pow21_clocks
ASM_TEXT(       mtspr           SPR_TBL_W, r3                           )
ASM_TEXT(       mfspr           r3, SPR_USPRG0                          )
ASM_TEXT(       full_prologue                                           )
ASM_TEXT(       bl              wd_exception_handler                    )
ASM_TEXT(       full_epilogue                                           )
ASM_TEXT(       rfci                                                    )   // exit from rumboot_WDT_hdr_speedup
);

void rumboot_CI_hdr_save_system_hung();
void rumboot_WDT_hdr_speedup();

void wd_exception_handler( int const id, char const * const name ) {
    rumboot_putstring( "WD exception handler" );

    spr_write( SPR_TSR_RC, (0b1     << TIMER_TSR_WIS_i) ); // clear WD exception
}

static void handler_dma2plb6_ch0( int irq, void *args );

static void handler_system_hung_dummy( int irq, void *args ) {}

static void init_handlers() {
    rumboot_putstring( "Init handlers" );

    spr_write( SPR_IVOR0,   (uint32_t)&rumboot_CI_hdr_save_system_hung & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR12,  (uint32_t)&rumboot_WDT_hdr_speedup & ITRPT_IVORn_OFFSET_mask );

    rumboot_irq_cli();
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_14, MPIC128_PRIOR_14 );
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl,   DMA2PLB6_DMA_IRQ_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH,   handler_dma2plb6_ch0,       NULL );
    rumboot_irq_set_handler( tbl,   O_SYSTEM_HUNG,      RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS,     handler_system_hung_dummy,  NULL );
    rumboot_irq_priority_adjust( tbl, O_SYSTEM_HUNG, MPIC128_PRIOR_14 );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_sei();
}


extern char         IM0_MEMORY_SIZE[];
#define IM0_SIZE    ((uint32_t)IM0_MEMORY_SIZE)

static void move_execution_to_l1() {
    /*
     *  Special algorithm:
     *  1) copy im0->sram1
     *  2) tlb substitution (im0->sram1)
     *  3) cache test code and stacks (in L1)
     *
     *  because if SYSTEM HUNG detected, the code can only be called from L1I
     */

    rumboot_putstring( "Copy im0->sram1" );
    memcpy( (void *) SRAM1_BASE, (void *) IM0_BASE, IM0_SIZE );
    msync();

    rumboot_putstring( "TLB substitution (im0->sram1)" );
    static const tlb_entry im0_to_sram1_tlb[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x60000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x60010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )}
    };
    write_tlb_entries(im0_to_sram1_tlb, ARRAY_SIZE(im0_to_sram1_tlb));

    rumboot_putstring( "Cache test code (in L1I)" );
    for (uint32_t addr = IM0_BASE; addr < (IM0_BASE + L1I_SIZE/2); addr += L1C_LINE_SIZE) {
        icbtls(0, addr);
    }
}


static void generate_system_hang() {
    rumboot_putstring( "Generate SYSTEM HANG" );

    move_execution_to_l1();

    start_wd( WDRT_system_hang );

    msr_write( msr_read() & ~(0b1 << ITRPT_XSR_ME_i) );
    l2c_pmu_write( DCR_L2C_BASE, L2C_L2PLBMCKEN1,   0x0 );
    l2c_pmu_write( DCR_L2C_BASE, L2C_L2CPUMCKEN,    0x0 );
    l2c_l2_write( DCR_L2C_BASE, L2C_L2RACCFG,   l2c_l2_read(DCR_L2C_BASE, L2C_L2RACCFG) | (1 << L2C_L2RACCFG_DisablePlbErrScrub_i) );
    l2c_l2_write( DCR_L2C_BASE, L2C_L2WACCFG,   l2c_l2_read(DCR_L2C_BASE, L2C_L2WACCFG) | (1 << L2C_L2WACCFG_DisablePlbErrScrub_i) );
    dcr_write( DCR_PLB6_BC_BASE+PLB6BC_HCPP, PLB6BC_HCPP_4K );

#define FAKE_EPN    0x80040 // IM2_BASE
#define FAKE_ADDR   (FAKE_EPN << 12)
    static const tlb_entry tlb_entry_fake[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,       DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x040,  0x00000,    FAKE_EPN,  MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )}
    };
    write_tlb_entries( tlb_entry_fake, ARRAY_SIZE(tlb_entry_fake) );

    for(;;) {
        iowrite32( 0x00, FAKE_ADDR );
        ioread32( FAKE_ADDR );
    }
}

static void check_system_hang() {
    TEST_ASSERT( (spr_read(SPR_TSR_RC) & FIELD_MASK32(TIMER_TSR_WRS_i, TIMER_TSR_WRS_n)) == (TIMER_TCR_WRS_Core_reset << TIMER_TSR_WRS_i), "Wrong reset type" );

    rumboot_putstring( "Check SYSTEM HANG: OK" );
}

static void configure_dma( uint32_t const base_addr, DmaChannel const channel, uint32_t const addr_src, uint32_t const addr_dst, uint32_t const count) {
    //default
    struct dma2plb6_setup_info const dma_info = {
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
    msync();
    dma2plb6_mcpy_init( &dma_info );
}

#define SIZE_4KB    0x1000
#define SIZE_16KB   0x4000

#define DATA_SIZE   SIZE_4KB
#define DATA_OFFSET (DATA_SIZE + DATA_SIZE)

static volatile bool dma2plb6_handled = false;

static void handler_dma2plb6_ch0( int irq, void *args ) {
    rumboot_putstring( "irq handler (DMA, ch0) ");
    dma2plb6_clear_interrupt( DCR_DMAPLB6_BASE, channel0 );
    dma2plb6_handled = true;
}

static void check_no_hang_on_load() {
    rumboot_putstring( "Checking no SYSTEM HANG on load..." );

    uint32_t const src_address_array[] = {
        SRAM0_BASE,
        SDRAM_BASE,
        SSRAM_BASE,
        PIPELINED_BASE,
        SRAM1_BASE+IM0_SIZE,
        IM1_BASE,
        IM2_BASE
    };

    //init data
    rumboot_putstring( "Init data" );
    for( uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++ ) {
        rumboot_memfill8( (void*)src_address_array[i], DATA_SIZE, 0x00, 0x01 );
        rumboot_memfill8_modelling( (void*)(src_address_array[i]+DATA_SIZE), DATA_SIZE, 0x00, 0x00 );
        rumboot_memfill8( (void*)(src_address_array[i]+DATA_OFFSET), DATA_SIZE, 0x00, 0x01 );
        rumboot_memfill8_modelling( (void*)(src_address_array[i]+DATA_OFFSET+DATA_SIZE), DATA_SIZE, 0x00, 0x00 );
    }

    move_execution_to_l1();

    start_wd( WDRT_no_hang_on_load );

    rumboot_putstring( "Set tlb (WT)" );
    static const tlb_entry em0_tlb_entry_wt[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( em0_tlb_entry_wt, ARRAY_SIZE(em0_tlb_entry_wt) );

    for( uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++ ) {
        rumboot_putstring( "\tStart check" );
        rumboot_puthex( src_address_array[i] );

        rumboot_putstring( "Start DMA" );
        configure_dma( DCR_DMAPLB6_BASE, channel0, src_address_array[i], src_address_array[i]+DATA_SIZE, DATA_SIZE );
        dma2plb6_enable_channel( DCR_DMAPLB6_BASE, channel0 );

        rumboot_putstring( "Start write-read" );
        uint32_t addr = src_address_array[i] + DATA_OFFSET;
        while( !dma2plb6_handled ) {
            iowrite32( ioread32(addr), addr+DATA_SIZE );
            addr+=4;
        }
        dma2plb6_handled = false;
    }

    rumboot_putstring( "Check no SYSTEM HANG on load: OK" );
}
