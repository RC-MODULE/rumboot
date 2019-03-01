/*
 * test_oi10_cpu_025_io.c
 *
 *  Created on: Feb 19, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/ppc470s/mmu/mem_window.h>

#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/devices/dma2plb6.h>

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


#define EVENT_GENERATE_INT_DMA_O_S_SLV_ERR_INT       (TEST_EVENT_CODE_MIN + 0)
#define EVENT_CLEAR_INT_DMA_O_S_SLV_ERR_INT          (TEST_EVENT_CODE_MIN + 1)

#define EVENT_GENERATE_INT_P6BC_O_SYSTEM_HUNG        (TEST_EVENT_CODE_MIN + 2)
#define EVENT_CLEAR_INT_P6BC_O_SYSTEM_HUNG           (TEST_EVENT_CODE_MIN + 3)

#define EVENT_GENERATE_INT_P6P4_0_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 4)
#define EVENT_CLEAR_INT_P6P4_0_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 5)

#define EVENT_GENERATE_INT_P6P4_1_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 6)
#define EVENT_CLEAR_INT_P6P4_1_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 7)

#define EVENT_GENERATE_INT_ARB_SYSDCRERR             (TEST_EVENT_CODE_MIN + 8)
#define EVENT_CLEAR_INT_ARB_SYSDCRERR                (TEST_EVENT_CODE_MIN + 9)

#define TEST_OI10_CPU_025_IO_TIMEOUT    400

#define UNEXIST_DCR_ADDR                0x00


struct rumboot_irq_entry *tbl;
volatile uint32_t MC_HANDLED = 0;

static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );
    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i) )),
                 "Unexpected exception" );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
    isync();
    msync();
}


void generate_P6BC_O_SYSTEM_HUNG(uint32_t l2c_dcr_base)
{
    rumboot_printf("Check O_SYSTEM_HUNG generation\n");
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
}

void generate_BR6TO4_INTR(uint32_t base_address)
{
    rumboot_printf("Check BR6TO4_INTR generation\n");
    dcr_write(base_address + P64_TESR, 0b1 << ESR_TESR_P6WPE_i);
}

void generate_ARB_SYSDCRERR()
{
    dcr_write(DCR_ARB_BASE + DCRARB_DACR, (0x1 << DCRARB_DACR_TOCNT_i));
    dcr_read(UNEXIST_DCR_ADDR);
}

void wait_interrupt(uint32_t interrupt_number)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << interrupt_number)) && t++<TEST_OI10_CPU_025_IO_TIMEOUT) msync();
    TEST_ASSERT((MC_HANDLED & (1 << interrupt_number)), "Interrupt handle timeout!");
}

void DMA_O_S_SLV_ERR_INT_inj_int_handler()
{
    rumboot_printf("DMA1_O_S_SLV_ERR_INT_inj_int_handler\n");
    dma2plb6_disable_o_slv_err_interrupt(DCR_DMAPLB6_BASE);
    dcr_write(DCR_DMAPLB6_BASE + PLB6_DMA_PSE, 1 << DMA_PSE_PE_i);
    SET_BIT(MC_HANDLED, DMA2PLB6_SLV_ERR_INT);
    test_event(EVENT_CLEAR_INT_DMA_O_S_SLV_ERR_INT);
}

void P6BC_O_SYSTEM_HUNG_inj_int_handler()
{
    rumboot_printf("P6BC_O_SYSTEM_HUNG_inj_int_handler\n");
    SET_BIT(MC_HANDLED, O_SYSTEM_HUNG);
    test_event(EVENT_CLEAR_INT_P6BC_O_SYSTEM_HUNG);
}

void P6P4_0_BR6TO4_INTR_inj_int_handler()
{
    rumboot_printf("P6P4_0_BR6TO4_INTR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_0_BASE + P64_ESR) & (0b1 << ESR_TESR_P4MWE_i), "Wrong value in P6P4_0 ESR reg. Expected ESR[P4MWE]=1\n");
    test_event(EVENT_CLEAR_INT_P6P4_0_BR6TO4_INTR);
    dcr_write(DCR_PLB6PLB4_0_BASE + P64_ESR, ((0b1 << ESR_TESR_P4MWE_i)));
}

void P6P4_1_BR6TO4_INTR_inj_int_handler()
{
    rumboot_printf("P6P4_1_BR6TO4_INTR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_1_BASE + P64_ESR) & (0b1 << ESR_TESR_P4MWE_i), "Wrong value in P6P4_1 ESR reg. Expected ESR[P4MWE]=1\n");
    test_event(EVENT_CLEAR_INT_P6P4_1_BR6TO4_INTR);
    dcr_write(DCR_PLB6PLB4_1_BASE + P64_ESR, ((0b1 << ESR_TESR_P4MWE_i)));
}

void ARB_SYSDCRERR_inj_int_handler()
{
    uint32_t daesr_reg;
    rumboot_printf("ARB_SYSDCRERR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
    daesr_reg = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
    TEST_ASSERT((daesr_reg & (0b1 << DCRARB_DAESR_TE_i)) && (daesr_reg & (0b1 << DCRARB_DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
    test_event(EVENT_CLEAR_INT_ARB_SYSDCRERR);
    dcr_write(DCR_ARB_BASE + DCRARB_DAESR, 0b1<<DCRARB_DAESR_EV_i); //dcrarb_clear_daesr
}

void P6BC_O_SYSTEM_HUNG_L2C_reg_int_handler()
{
    rumboot_printf("P6BC_O_SYSTEM_HUNG_L2C1_reg_int_handler\n");
    SET_BIT(MC_HANDLED, O_SYSTEM_HUNG);
    l2c_l2_write(DCR_L2C_BASE, L2C_L2RACCFG, l2c_l2_read(DCR_L2C_BASE, L2C_L2RACCFG) & ~(1 << L2C_L2RACCFG_DisablePlbErrScrub_i));
    l2c_l2_write(DCR_L2C_BASE, L2C_L2WACCFG, l2c_l2_read(DCR_L2C_BASE, L2C_L2WACCFG) & ~(1 << L2C_L2WACCFG_DisablePlbErrScrub_i));
    l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBFRC0,(1 << L2C_L2PLBFRC0_IntvnDataPE0_i) );//generate interrupt for reset slave core
}

void P6P4_0_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_0_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_0_BASE + P64_ESR) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_0 ESR reg. Expected ESR[P6BPE]=1\n");
    dcr_write(DCR_PLB6PLB4_0_BASE + P64_ESR, (0b1 << ESR_TESR_P6WPE_i));
}

void P6P4_1_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_1_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_1_BASE + P64_ESR) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_1 ESR reg. Expected ESR[P6BPE]=1\n");
    dcr_write(DCR_PLB6PLB4_1_BASE + P64_ESR, (0b1 << ESR_TESR_P6WPE_i));
}

void ARB_SYSDCRERR_reg_int_handler()
{
    uint32_t daesr_reg;
    rumboot_printf("ARB_SYSDCRERR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
    daesr_reg = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
    TEST_ASSERT((daesr_reg & (0b1 << DCRARB_DAESR_TE_i)) && (daesr_reg & (0b1 << DCRARB_DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
    dcr_write(DCR_ARB_BASE + DCRARB_DAESR, 0b1 << DCRARB_DAESR_EV_i); //dcrarb_clear_daesr
}

void test_setup( uint32_t interrupt_number )
{

    switch (interrupt_number)
    {
    case O_SYSTEM_HUNG:
                        rumboot_printf("Set handler for O_SYSTEM_HUNG\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, O_SYSTEM_HUNG, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6BC_O_SYSTEM_HUNG_L2C_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( O_SYSTEM_HUNG );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB40_O_0_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB40_O_0_BR6TO4_INTR \n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB40_O_0_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_0_BR6TO4_INTR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB40_O_0_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB41_O_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB41_O_BR6TO4_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_1_BR6TO4_INTR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case ARB_SYSDCRERR:
                        rumboot_printf("Set handler for ARB_SYSDCRERR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, ARB_SYSDCRERR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ARB_SYSDCRERR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( ARB_SYSDCRERR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    }
}

void test_setup_inj( uint32_t interrupt_number )
{

    switch (interrupt_number)
    {
    case O_SYSTEM_HUNG:
                        rumboot_printf("Set handler for O_SYSTEM_HUNG\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, O_SYSTEM_HUNG, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, P6BC_O_SYSTEM_HUNG_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( O_SYSTEM_HUNG );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB40_O_0_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB40_O_0_BR6TO4_INTR \n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB40_O_0_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_0_BR6TO4_INTR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB40_O_0_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB41_O_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB41_O_BR6TO4_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_1_BR6TO4_INTR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case ARB_SYSDCRERR:
                        rumboot_printf("Set handler for ARB_SYSDCRERR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, ARB_SYSDCRERR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ARB_SYSDCRERR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( ARB_SYSDCRERR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case DMA2PLB6_SLV_ERR_INT:
                        rumboot_printf("Set handler for DMA2PLB6_SLV_ERR_INT\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, DMA2PLB6_SLV_ERR_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, DMA_O_S_SLV_ERR_INT_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( DMA2PLB6_SLV_ERR_INT );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    }
}

void check_interrupt_from_io_dev_reg(uint32_t interrupt_number)
{
    test_setup( interrupt_number );

    switch (interrupt_number)
    {
        case O_SYSTEM_HUNG:
                                            rumboot_printf("Check software generation P6BC_O_SYSTEM_HUNG\n");
                                            //setup_interrupt(interrupt_number, P6BC_O_SYSTEM_HUNG_L2C_reg_int_handler);
                                            generate_P6BC_O_SYSTEM_HUNG(DCR_L2C_BASE);
                                            //START_SLAVE_CORE_TEST_PART = 1;

                                            break;
        case PLB6PLB40_O_0_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_0_BR6TO4_INTR\n");
                                            generate_BR6TO4_INTR(DCR_PLB6PLB4_0_BASE);
                                            break;
        case PLB6PLB41_O_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_1_BR6TO4_INTR\n");
                                            generate_BR6TO4_INTR(DCR_PLB6PLB4_1_BASE);
                                            break;

        case ARB_SYSDCRERR:
                                            rumboot_printf("Check ARB_SYSDCRERR generation\n");
                                            generate_ARB_SYSDCRERR();
                                            break;

        default: rumboot_printf("Get wrong interrupt id for injection\n");
                 test_event(EVENT_ERROR);
                 break;
    }

    wait_interrupt(interrupt_number);
}

void check_interrupt_from_io_dev_inj(uint32_t interrupt_number)
{
    test_setup_inj( interrupt_number );

    switch (interrupt_number)
    {
        case DMA2PLB6_SLV_ERR_INT:
                                            test_event(EVENT_GENERATE_INT_DMA_O_S_SLV_ERR_INT);
                                            break;

        case O_SYSTEM_HUNG:
                                            test_event(EVENT_GENERATE_INT_P6BC_O_SYSTEM_HUNG);
                                            break;

        case PLB6PLB40_O_0_BR6TO4_INTR:
                                            test_event(EVENT_GENERATE_INT_P6P4_0_BR6TO4_INTR);
                                            break;

        case PLB6PLB41_O_BR6TO4_INTR:
                                            test_event(EVENT_GENERATE_INT_P6P4_1_BR6TO4_INTR);
                                            break;


        case ARB_SYSDCRERR:
                                            dcr_write(DCR_ARB_BASE + DCRARB_DACR, (0b1 << DCRARB_DACR_TOCNT_i));
                                            test_event(EVENT_GENERATE_INT_ARB_SYSDCRERR);
                                            break;

        default: rumboot_printf("Get wrong interrupt id for injection\n");
                 test_event(EVENT_ERROR);
                 break;
    }

    wait_interrupt(interrupt_number);
}

int main()
{
    test_event_send_test_id( "test_oi10_cpu_025_io");

    rumboot_irq_set_exception_handler(exception_handler);

    MC_HANDLED = 0;

    check_interrupt_from_io_dev_inj(DMA2PLB6_SLV_ERR_INT);
    check_interrupt_from_io_dev_inj(O_SYSTEM_HUNG);
    check_interrupt_from_io_dev_inj(PLB6PLB40_O_0_BR6TO4_INTR);
    check_interrupt_from_io_dev_inj(PLB6PLB41_O_BR6TO4_INTR);
    check_interrupt_from_io_dev_inj(ARB_SYSDCRERR);


    check_interrupt_from_io_dev_reg(PLB6PLB40_O_0_BR6TO4_INTR);
    check_interrupt_from_io_dev_reg(PLB6PLB41_O_BR6TO4_INTR);
    check_interrupt_from_io_dev_reg(ARB_SYSDCRERR);
    check_interrupt_from_io_dev_reg(O_SYSTEM_HUNG);


    return 0;
}
