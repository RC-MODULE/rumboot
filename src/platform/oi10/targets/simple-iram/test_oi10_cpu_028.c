#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/devices.h>
#include <platform/reg_access/reg_access_l2c.h>
#include <platform/regs/regs_srammc2plb4.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <platform/devices/mpic128.h>

enum
{
    CHECK_PARITY_NO,
    CHECK_PARITY_ALL,
    CHECK_PARITY_PPC0_W,
    CHECK_PARITY_L2C0_W,
    CHECK_PARITY_P6BC_W,
    CHECK_PARITY_PLB6PLB4_W,
    CHECK_PARITY_PLB4ARB_W,
    CHECK_PARITY_SRAMMC2PLB4_W,
    CHECK_PARITY_PPC0_R,
    CHECK_PARITY_L2C0_R,
    CHECK_PARITY_P6BC_R,
    CHECK_PARITY_PLB6PLB4_R,
    CHECK_PARITY_PLB4ARB_R,
    CHECK_PARITY_SRAMMC2PLB4_R,

    CHECK_PARITY_N
};

typedef enum test_event_code
{
    TEC_CHECK_PPC0_W_PE_DETECTION   = TEST_EVENT_CODE_MIN,
    TEC_CHECK_L2C0_W_PE_DETECTION,
    TEC_CHECK_P6BC_W_PE_DETECTION,
    TEC_CHECK_PLB6PLB4_W_PE_DETECTION,
    TEC_CHECK_PLB4ARB_W_PE_DETECTION,
    TEC_CHECK_SRAMMC2PLB4_W_PE_DETECTION,
    TEC_CHECK_PPC0_R_PE_DETECTION,
    TEC_CHECK_L2C0_R_PE_DETECTION,
    TEC_CHECK_P6BC_R_PE_DETECTION,
    TEC_CHECK_PLB6PLB4_R_PE_DETECTION,
    TEC_CHECK_PLB4ARB_R_PE_DETECTION,
    TEC_CHECK_SRAMMC2PLB4_R_PE_DETECTION
} test_event_code;

enum
{
    TEST_VALUE  = 0x7E57DA7A,
    TEST_MASK   = 0x3A58C0DE,
    HANDLER_CALLED = 0x88CA11ED
};

static uint32_t volatile __attribute__((aligned(16),section(".data"))) ADDR[CHECK_PARITY_N] = { 0 };


static void disable_parity_checks() {
    register uint32_t reg;
    rumboot_putstring( "disable_parity_checks\n" );

    reg = spr_read( SPR_CCR0 );
    CLEAR_BITS_BY_MASK( reg,
                        (1 << CTRL_CCR0_PRE_i) | (1 << CTRL_CCR0_CRPE_i) );
    spr_write(SPR_CCR0, reg );

    reg = spr_read( SPR_CCR1 );
    SET_BITS_BY_MASK( reg,
                      (1 << CTRL_CCR1_DPC_i) );
    spr_write(SPR_CCR1, reg );

    reg = L2C0_L2PLBCFG_DCR_read();
    SET_BITS_BY_MASK( reg,
                      (1 << IBM_BIT_INDEX(32, 29)) | (1 << IBM_BIT_INDEX(32, 30)) | (1 << IBM_BIT_INDEX(32, 31)) );
    L2C0_L2PLBCFG_DCR_write( reg );

    reg = dcr_read( SRAMMC2PLB4_DPC + DCR_SRAMMC2PLB4_1_BASE );
    CLEAR_BIT( reg, IBM_BIT_INDEX(32, 0) );
    dcr_write( SRAMMC2PLB4_DPC + DCR_SRAMMC2PLB4_1_BASE, reg );

    //TODO add SRAM parity disable
}

static void enable_parity_checks() {
    register uint32_t reg;
    rumboot_putstring( "enable_parity_checks\n" );

    reg = spr_read( SPR_CCR0);
    SET_BITS_BY_MASK( reg,
                      (1 << CTRL_CCR0_PRE_i) | (1 << CTRL_CCR0_CRPE_i) );
    spr_write( SPR_CCR0, reg );

    reg = spr_read( SPR_CCR1 );
    CLEAR_BITS_BY_MASK( reg,
                        (1 << CTRL_CCR1_DPC_i) );
    spr_write( SPR_CCR1, reg );

    reg = L2C0_L2PLBCFG_DCR_read();
    CLEAR_BITS_BY_MASK( reg,
                        (1 << IBM_BIT_INDEX(32, 29)) | (1 << IBM_BIT_INDEX(32, 30)) | (1 << IBM_BIT_INDEX(32, 31)) );
    L2C0_L2PLBCFG_DCR_write( reg );

    reg = dcr_read( SRAMMC2PLB4_DPC + DCR_SRAMMC2PLB4_1_BASE );
    SET_BIT( reg, IBM_BIT_INDEX(32, 0) );
    dcr_write( SRAMMC2PLB4_DPC + DCR_SRAMMC2PLB4_1_BASE, reg );

    //TODO add SRAM parity enable
}


void ppc0_w_pe_handler(int irq, void *args) {
    rumboot_putstring( "ppc0_w_pe_handler\n" );

    TEST_ASSERT( (ADDR[CHECK_PARITY_PPC0_W] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );
    ADDR[CHECK_PARITY_PPC0_W] = HANDLER_CALLED;

    L2C0_L2CPUSTAT_DCR_write( (0b1 << IBM_BIT_INDEX(32, 28))
                            | (0b1 << IBM_BIT_INDEX(32, 29))
                            | (0b1 << IBM_BIT_INDEX(32, 30))
                            | (0b1 << IBM_BIT_INDEX(32, 31)) );

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
}

void l2c0_w_pe_handler(int irq, void *args) {
    rumboot_putstring( "l2c0_w_pe_handler\n" );

    TEST_ASSERT( (ADDR[CHECK_PARITY_L2C0_W] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );
    ADDR[CHECK_PARITY_L2C0_W] = HANDLER_CALLED;
//    msync();
//    trace_dump( &ADDR[CHECK_PARITY_L2C0_W], 4 );

    dcr_write( DCR_PLB6PLB4_1_BASE+0x1, 0xFFFFFFFF );
    dcr_read(MPIC128_NCIAR0 +  DCR_MPIC128_BASE );
    dcr_write(MPIC128_NCEOI0 +  DCR_MPIC128_BASE, 0x00000000 );

    spr_write( SPR_ESR, 0x00000000 );
}

void p6bc_w_pe_handler(int irq, void *args) {
    rumboot_putstring( "p6bc_w_pe_handler\n" );

    TEST_ASSERT( (ADDR[CHECK_PARITY_P6BC_W] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );
    ADDR[CHECK_PARITY_P6BC_W] = HANDLER_CALLED;

    dcr_write( DCR_PLB6PLB4_1_BASE+0x1, 0xFFFFFFFF );
    dcr_read(MPIC128_NCIAR0 +  DCR_MPIC128_BASE );
    dcr_write(MPIC128_NCEOI0 +  DCR_MPIC128_BASE, 0x00000000 );

    spr_write( SPR_ESR, 0x00000000 );
}

void plb6plb4_w_pe_handler(int irq, void *args) {
    rumboot_putstring( "plb6plb4_w_pe_handler\n" );

    TEST_ASSERT( (ADDR[CHECK_PARITY_PLB6PLB4_W] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );
    ADDR[CHECK_PARITY_PLB6PLB4_W] = HANDLER_CALLED;

    dcr_write( DCR_PLB6PLB4_1_BASE+0x1, 0xFFFFFFFF );
    dcr_read(MPIC128_NCIAR0 +  DCR_MPIC128_BASE );
    dcr_write(MPIC128_NCEOI0 +  DCR_MPIC128_BASE, 0x00000000 );

    spr_write( SPR_ESR, 0x00000000 );
}

void ppc0_r_pe_handler(int irq, void *args) {
    rumboot_putstring( "ppc0_r_pe_handler\n" );

    TEST_ASSERT( ADDR[CHECK_PARITY_PPC0_R] == TEST_VALUE, "Test value read error" );
    ADDR[CHECK_PARITY_PPC0_R] = HANDLER_CALLED;

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
}

void l2c0_r_pe_handler(int irq, void *args) {
    rumboot_putstring( "l2c0_r_pe_handler\n" );

    TEST_ASSERT( ADDR[CHECK_PARITY_L2C0_R] == TEST_VALUE, "Test value read error" );
    ADDR[CHECK_PARITY_L2C0_R] = HANDLER_CALLED;

    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
}

void p6bc_r_pe_handler(int irq, void *args) {
    rumboot_putstring( "p6bc_r_pe_handler\n" );

    TEST_ASSERT( ADDR[CHECK_PARITY_P6BC_R] == TEST_VALUE, "Test value read error" );
    ADDR[CHECK_PARITY_P6BC_R] = HANDLER_CALLED;

    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
}

void plb6plb4_r_pe_handler(int irq, void *args) {
    rumboot_putstring( "plb6plb4_r_pe_handler\n" );

    TEST_ASSERT( ADDR[CHECK_PARITY_PLB6PLB4_R] == TEST_VALUE, "Test value read error" );
    ADDR[CHECK_PARITY_PLB6PLB4_R] = HANDLER_CALLED;

    dcr_write( DCR_PLB6PLB4_1_BASE+0x1, 0xFFFFFFFF );
    dcr_read(MPIC128_NCIAR0 +  DCR_MPIC128_BASE );
    dcr_write(MPIC128_NCEOI0 +  DCR_MPIC128_BASE, 0x00000000 );

    spr_write( SPR_ESR, 0x00000000 );
}

void srammc2plb4_r_pe_handler(int irq, void *args) {
    rumboot_putstring( "srammc2plb4_r_pe_handler\n" );

    TEST_ASSERT( ADDR[CHECK_PARITY_SRAMMC2PLB4_R] == TEST_VALUE, "Test value read error" );
    ADDR[CHECK_PARITY_SRAMMC2PLB4_R] = HANDLER_CALLED;

    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
}


static void check_parity_disabled() {
    ADDR[CHECK_PARITY_NO] = TEST_VALUE;
    TEST_ASSERT( ADDR[CHECK_PARITY_NO] == TEST_VALUE, "Read/Write with parity check disabled is failed" );
}

static void check_parity_enabled() {
    ADDR[CHECK_PARITY_ALL] = TEST_VALUE;
    TEST_ASSERT( ADDR[CHECK_PARITY_ALL] == TEST_VALUE, "Read/Write with parity check enabled is failed" );
}

static void check_ppc0_w_pe_detection() {
    const uint32_t l2cpumcken = L2C0_L2CPUMCKEN_DCR_read();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_putstring( "check_ppc0_w_pe_detection\n" );

    rumboot_irq_set_handler(tbl, L2C0_MCHKOUT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), ppc0_w_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    L2C0_L2CPUSTAT_DCR_write( 0xFFFFFFFF );
    L2C0_L2CPUMCKEN_DCR_write( (0b1 << IBM_BIT_INDEX(32, 28))
                             | (0b1 << IBM_BIT_INDEX(32, 29))
                             | (0b1 << IBM_BIT_INDEX(32, 30))
                             | (0b1 << IBM_BIT_INDEX(32, 31)) );

    test_event( TEC_CHECK_PPC0_W_PE_DETECTION );
    ADDR[CHECK_PARITY_PPC0_W] = TEST_VALUE;

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_PPC0_W] == HANDLER_CALLED, "PPC0 write parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_PPC0_W] == HANDLER_CALLED, 2 );

    L2C0_L2CPUMCKEN_DCR_write( l2cpumcken );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

static void check_l2c0_w_pe_detection() {
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_putstring( "check_l2c0_w_pe_detection\n" );

    rumboot_irq_set_handler(tbl,PLB6PLB40_O_0_BR6TO4_INTR, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), l2c0_w_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    test_event( TEC_CHECK_L2C0_W_PE_DETECTION );
    ADDR[CHECK_PARITY_L2C0_W] = TEST_VALUE;
    TEST_WAIT( ADDR[CHECK_PARITY_L2C0_W] == HANDLER_CALLED, 2 );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

static void check_p6bc_w_pe_detection() {
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_putstring( "check_p6bc_w_pe_detection\n" );

    rumboot_irq_set_handler(tbl,PLB6PLB40_O_0_BR6TO4_INTR, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), p6bc_w_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    test_event( TEC_CHECK_P6BC_W_PE_DETECTION );
    ADDR[CHECK_PARITY_P6BC_W] = TEST_VALUE;

    TEST_WAIT( ADDR[CHECK_PARITY_P6BC_W] == HANDLER_CALLED, 2 );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

}

/*static void check_plb6plb4_w_pe_detection() {
    rumboot_putstring( "check_plb6plb4_w_pe_detection\n" );

    irq_set_handler(irq_source_External_input, plb6plb4_w_pe_handler);

    mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, 18, MPIC128_PRIOR_1, int_sense_level, int_pol_pos, Processor0 );
    spr_write( SPR_ESR, 0x00000000 );
    msr_write( (0b0 << ITRPT_XSR_WE_i)
             | (0b0 << ITRPT_XSR_CE_i)
             | (0b1 << ITRPT_XSR_EE_i)
             | (0b0 << ITRPT_XSR_PR_i)
             | (0b0 << ITRPT_XSR_FP_i)
             | (0b0 << ITRPT_XSR_ME_i)
             | (0b0 << ITRPT_XSR_FE0_i)
             | (0b0 << ITRPT_XSR_DWE_i)
             | (0b0 << ITRPT_XSR_DE_i)
             | (0b0 << ITRPT_XSR_FE1_i)
             | (0b0 << ITRPT_XSR_IS_i)
             | (0b0 << ITRPT_XSR_DS_i)
             | (0b0 << ITRPT_XSR_PMM_i) );

    test_event( TEC_CHECK_PLB6PLB4_W_PE_DETECTION );
    ADDR[CHECK_PARITY_PLB6PLB4_W] = TEST_VALUE;

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_PLB6PLB4_W] == HANDLER_CALLED, "PLB6PLB4 write parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_PLB6PLB4_W] == HANDLER_CALLED, 2 );

    irq_set_handler(irq_source_External_input, irq_handler__default);
}*/

/*static void check_ppc0_r_pe_detection() {
    rumboot_putstring( "check_ppc0_r_pe_detection\n" );

    irq_set_handler(irq_source_Machine_check, ppc0_r_pe_handler);

    spr_write( SPR_MCSR_C, 0xFFFFFFFF );
    msr_write( (0b0 << ITRPT_XSR_WE_i)
             | (0b1 << ITRPT_XSR_CE_i)
             | (0b1 << ITRPT_XSR_EE_i)
             | (0b0 << ITRPT_XSR_PR_i)
             | (0b0 << ITRPT_XSR_FP_i)
             | (0b1 << ITRPT_XSR_ME_i)
             | (0b0 << ITRPT_XSR_FE0_i)
             | (0b0 << ITRPT_XSR_DWE_i)
             | (0b0 << ITRPT_XSR_DE_i)
             | (0b0 << ITRPT_XSR_FE1_i)
             | (0b0 << ITRPT_XSR_IS_i)
             | (0b0 << ITRPT_XSR_DS_i)
             | (0b0 << ITRPT_XSR_PMM_i) );

    ADDR[CHECK_PARITY_PPC0_R] = TEST_VALUE;
    msync();
    test_event( TEC_CHECK_PPC0_R_PE_DETECTION );
    TEST_ASSERT( (ADDR[CHECK_PARITY_PPC0_R] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_PPC0_R] == HANDLER_CALLED, "PPC0 read parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_PPC0_R] == HANDLER_CALLED, 2 );

    irq_set_handler(irq_source_Machine_check, irq_handler__default);
}*/

static void check_l2c0_r_pe_detection() {
    uint32_t l2plbmcken1 = L2C0_L2PLBMCKEN1_DCR_read();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_putstring( "check_l2c0_r_pe_detection\n" );

    rumboot_irq_set_handler(tbl, L2C0_MCHKOUT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), l2c0_r_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    L2C0_L2PLBSTAT0_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBMCKEN1_DCR_write( 0xFFFF << IBM_BIT_INDEX(32, 31) );

    ADDR[CHECK_PARITY_L2C0_R] = TEST_VALUE;
    msync();
    test_event( TEC_CHECK_L2C0_R_PE_DETECTION );
    TEST_ASSERT( (ADDR[CHECK_PARITY_L2C0_R] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_L2C0_R] == HANDLER_CALLED, "L2C0 read parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_L2C0_R] == HANDLER_CALLED, 2 );

    L2C0_L2PLBMCKEN1_DCR_write( l2plbmcken1 );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

static void check_p6bc_r_pe_detection() {
    uint32_t l2plbmcken1 = L2C0_L2PLBMCKEN1_DCR_read();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_putstring( "check_p6bc_r_pe_detection\n" );

    rumboot_irq_set_handler(tbl, L2C0_MCHKOUT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), p6bc_r_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    L2C0_L2PLBSTAT0_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBMCKEN1_DCR_write( 0xFFFF << IBM_BIT_INDEX(32, 31) );

    ADDR[CHECK_PARITY_P6BC_R] = TEST_VALUE;
    msync();
    test_event( TEC_CHECK_P6BC_R_PE_DETECTION );
    TEST_ASSERT( (ADDR[CHECK_PARITY_P6BC_R] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );

    TEST_WAIT( ADDR[CHECK_PARITY_P6BC_R] == HANDLER_CALLED, 2 );

    L2C0_L2PLBMCKEN1_DCR_write( l2plbmcken1 );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

/*static void check_plb6plb4_r_pe_detection() {
    rumboot_putstring( "check_plb6plb4_r_pe_detection\n" );

    irq_set_handler(irq_source_External_input, plb6plb4_r_pe_handler);

    mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, 18, MPIC128_PRIOR_1, int_sense_level, int_pol_pos, Processor0 );
    spr_write(  SPR_ESR, 0x00000000 );
    msr_write( (0b0 << ITRPT_XSR_WE_i)
             | (0b0 << ITRPT_XSR_CE_i)
             | (0b1 << ITRPT_XSR_EE_i)
             | (0b0 << ITRPT_XSR_PR_i)
             | (0b0 << ITRPT_XSR_FP_i)
             | (0b0 << ITRPT_XSR_ME_i)
             | (0b0 << ITRPT_XSR_FE0_i)
             | (0b0 << ITRPT_XSR_DWE_i)
             | (0b0 << ITRPT_XSR_DE_i)
             | (0b0 << ITRPT_XSR_FE1_i)
             | (0b0 << ITRPT_XSR_IS_i)
             | (0b0 << ITRPT_XSR_DS_i)
             | (0b0 << ITRPT_XSR_PMM_i) );

    test_event( TEC_CHECK_PLB6PLB4_R_PE_DETECTION );
    ADDR[CHECK_PARITY_PLB6PLB4_R] = TEST_VALUE;
    msync();
    TEST_ASSERT( (ADDR[CHECK_PARITY_PLB6PLB4_R] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_PLB6PLB4_R] == HANDLER_CALLED, "PLB6PLB4 read parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_PLB6PLB4_R] == HANDLER_CALLED, 2 );

    irq_set_handler(irq_source_External_input, irq_handler__default);
}*/

static void check_srammc2plb4_r_pe_detection() {
    uint32_t l2plbmcken1 = L2C0_L2PLBMCKEN1_DCR_read();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_putstring( "check_srammc2plb4_r_pe_detection\n" );

    rumboot_irq_set_handler(tbl, L2C0_MCHKOUT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), srammc2plb4_r_pe_handler, NULL);

    rumboot_irq_table_activate(tbl);

    L2C0_L2PLBSTAT0_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBSTAT1_DCR_write( 0xFFFFFFFF );
    L2C0_L2PLBMCKEN1_DCR_write( 0x3 << IBM_BIT_INDEX(32, 13) );

    ADDR[CHECK_PARITY_SRAMMC2PLB4_R] = TEST_VALUE;
    msync();
    test_event( TEC_CHECK_SRAMMC2PLB4_R_PE_DETECTION );
    TEST_ASSERT( (ADDR[CHECK_PARITY_SRAMMC2PLB4_R] ^ TEST_MASK) == TEST_VALUE, "Masked test value read error" );

//    msync();
//    TEST_ASSERT( ADDR[CHECK_PARITY_SRAMMC2PLB4_R] == HANDLER_CALLED, "SRAMMC2PLB4 read parity error handler isn't called" );
    TEST_WAIT( ADDR[CHECK_PARITY_SRAMMC2PLB4_R] == HANDLER_CALLED, 2 );

    L2C0_L2PLBMCKEN1_DCR_write( l2plbmcken1 );

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}


int main() {
    test_event_send_test_id("test_oi10_cpu_028");
    disable_parity_checks();

    check_parity_disabled();

    enable_parity_checks();

    check_parity_enabled();

//    irq_init_cpu_int_vec();
    check_ppc0_w_pe_detection();

    check_l2c0_w_pe_detection();
    check_p6bc_w_pe_detection();
//    check_plb6plb4_w_pe_detection();
//    check_ppc0_r_pe_detection();
    check_l2c0_r_pe_detection();
    check_p6bc_r_pe_detection();
//    check_plb6plb4_r_pe_detection();
    check_srammc2plb4_r_pe_detection();

    return 0;
}


