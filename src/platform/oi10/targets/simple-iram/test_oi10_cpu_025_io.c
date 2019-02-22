/*
 * test_oi10_cpu_025_io.c
 *
 *  Created on: Feb 19, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>


#define EVENT_GENERATE_INT10_DMA0_O_S_SLV_ERR_INT      (TEST_EVENT_CODE_MIN + 0)
#define EVENT_CLEAR_INT10_DMA0_O_S_SLV_ERR_INT         (TEST_EVENT_CODE_MIN + 1)

#define EVENT_GENERATE_INT15_DMA1_O_S_SLV_ERR_INT      (TEST_EVENT_CODE_MIN + 2)
#define EVENT_CLEAR_INT15_DMA1_O_S_SLV_ERR_INT         (TEST_EVENT_CODE_MIN + 3)

#define EVENT_GENERATE_INT16_P6BC_O_SYSTEM_HUNG        (TEST_EVENT_CODE_MIN + 4)
#define EVENT_CLEAR_INT16_P6BC_O_SYSTEM_HUNG           (TEST_EVENT_CODE_MIN + 5)

#define EVENT_GENERATE_INT17_P6P4_0_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 6)
#define EVENT_CLEAR_INT17_P6P4_0_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 7)

#define EVENT_GENERATE_INT18_P6P4_1_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 8)
#define EVENT_CLEAR_INT18_P6P4_1_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 9)

#define EVENT_GENERATE_INT19_P4XAHB_INTR             (TEST_EVENT_CODE_MIN + 10)
#define EVENT_CLEAR_INT19_P4XAHB_INTR                (TEST_EVENT_CODE_MIN + 11)

#define EVENT_GENERATE_INT20_PLB6AHB_1_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 12)
#define EVENT_CLEAR_INT20_PLB6AHB_1_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 13)

#define EVENT_GENERATE_INT21_P4XAHB_1_INTR             (TEST_EVENT_CODE_MIN + 14)
#define EVENT_CLEAR_INT21_P4XAHB_1_INTR                (TEST_EVENT_CODE_MIN + 15)

#define EVENT_GENERATE_INT22_PLB6AHB_2_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 16)
#define EVENT_CLEAR_INT22_PLB6AHB_2_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 17)

#define EVENT_GENERATE_INT23_P4XAHB_2_INTR             (TEST_EVENT_CODE_MIN + 18)
#define EVENT_CLEAR_INT23_P4XAHB_2_INTR                (TEST_EVENT_CODE_MIN + 19)

#define EVENT_GENERATE_INT24_ARB_SYSDCRERR             (TEST_EVENT_CODE_MIN + 20)
#define EVENT_CLEAR_INT24_ARB_SYSDCRERR                (TEST_EVENT_CODE_MIN + 21)

#define TEST_MPW_CPU_025_IO_TIMEOUT    400

#define UNEXIST_DCR_ADDR                0x00

static const tlb_entry bootrom_mirror =
            //         MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
                      {MMU_TLB_ENTRY(  0x011,  0x00000,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )};

static const tlb_entry fake_tlb_entry =
            //         MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
                      {MMU_TLB_ENTRY(  0x004,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )};

#define CURRENT_CORE_PPC0   0
#define CURRENT_CORE_PPC1   1

volatile uint32_t MC_HANDLED = 0;
volatile uint32_t START_SLAVE_CORE_TEST_PART = 0;
volatile uint32_t FINISH_SLAVE_CORE_TEST_PART = 0;


//void slave_core_test_part(uint32_t l2c_dcr_base)
//{
//    if (FINISH_SLAVE_CORE_TEST_PART)
//    {
//        rumboot_printf("Slave core test part is OK\n");
//        test_event(EVENT_OK);
//        return;
//    }
//
//    irq_init_cpu_int_vec();
//
//    if (l2c_dcr_base==DCR_L2C_BASE)
//        irq_set_handler(irq_source_Machine_check, slave_core_interrupt_handler_l2c1);
//    else
//        irq_set_handler(irq_source_Machine_check, slave_core_interrupt_handler_l2c0);
//
//    msr_write(msr_read() | (1<<ITRPT_XSR_ME_i));
//
//    while (!START_SLAVE_CORE_TEST_PART);
//    rumboot_printf("Generate HANG from slave core\n");
//    generate_P6BC_O_SYSTEM_HUNG(l2c_dcr_base);
//    rumboot_printf("exit from slave_core_test_part\n");
//}
//
//void slave_core_interrupt_handler_l2c1()
//{
//    //TODO: need to remove magic
//    L2C1_L2PLBFERR0_DCR_read();
//    L2C1_L2PLBFRC0_DCR_write(0);
//    SPR_DBCR0_write(SPR_DBCR0_read() | (0b01 << (63-35)));//reset slave core
//}
//
//void slave_core_interrupt_handler_l2c0()
//{
//    //TODO: need to remove magic
//    L2C0_L2PLBFERR0_DCR_read();
//    L2C0_L2PLBFRC0_DCR_write(0);
//    SPR_DBCR0_write(SPR_DBCR0_read() | (0b01 << (63-35)));//reset slave core
//}

static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );
    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i)
                                           | (0b1 << ITRPT_MCSR_L2_i) )),
                 "Unexpected exception" );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
    isync();
    msync();
}

void test_setup( uint32_t interrupt_number )
{


    switch (interrupt_number)
    {
    case O_SYSTEM_HUNG:

                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, O_SYSTEM_HUNG, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6BC_O_SYSTEM_HUNG_L2C_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( O_SYSTEM_HUNG );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB40_O_0_BR6TO4_INTR:
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB40_O_0_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB40_O_0_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB41_O_BR6TO4_INTR:
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB4XAHB1_INTR:
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB4XAHB1_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB4XAHB1_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case ARB_SYSDCRERR:
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, ARB_SYSDCRERR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( ARB_SYSDCRERR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    }
}

//void check_interrupt_from_io_dev_inj(uint32_t interrupt_number)
//{
//    switch (interrupt_number)
//    {
//        case irq_source_DMA0_O_S_SLV_ERR_INT:
//                                             setup_interrupt(interrupt_number, DMA0_O_S_SLV_ERR_INT_inj_int_handler);
//                                             test_event(EVENT_GENERATE_INT10_DMA0_O_S_SLV_ERR_INT);
//                                             break;
//
//        case irq_source_DMA1_O_S_SLV_ERR_INT:
//                                            setup_interrupt(interrupt_number, DMA1_O_S_SLV_ERR_INT_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT15_DMA1_O_S_SLV_ERR_INT);
//                                            break;
//
//        case O_SYSTEM_HUNG:
//                                            setup_interrupt(interrupt_number, P6BC_O_SYSTEM_HUNG_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT16_P6BC_O_SYSTEM_HUNG);
//                                            break;
//
//        case PLB6PLB40_O_0_BR6TO4_INTR:
//                                            setup_interrupt(interrupt_number, P6P4_0_BR6TO4_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT17_P6P4_0_BR6TO4_INTR);
//                                            break;
//
//        case PLB6PLB41_O_BR6TO4_INTR:
//                                            setup_interrupt(interrupt_number, P6P4_1_BR6TO4_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT18_P6P4_1_BR6TO4_INTR);
//                                            break;
//
//        case irq_source_PLB4XAHB0_INTR:
//                                            setup_interrupt(interrupt_number, PLB4XAHB_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT19_P4XAHB_INTR);
//                                            break;
//
//        case irq_source_PLB6PLB4_2_O_BR6TO4_INTR:
//                                            setup_interrupt(interrupt_number, PLB6AHB_1_BR6TO4_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT20_PLB6AHB_1_BR6TO4_INTR);
//                                            break;
//
//        case PLB4XAHB1_INTR:
//                                            setup_interrupt(interrupt_number, PLB4XAHB_1_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT21_P4XAHB_1_INTR);
//                                            break;
//
//        case irq_source_PLB6PLB4_3_O_BR6TO4_INTR:
//                                            setup_interrupt(interrupt_number, PLB6AHB_2_BR6TO4_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT22_PLB6AHB_2_BR6TO4_INTR);
//                                            break;
//
//        case irq_source_PLB4XAHB2_INTR:
//                                            setup_interrupt(interrupt_number, PLB4XAHB_2_INTR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT23_P4XAHB_2_INTR);
//                                            break;
//
//        case ARB_SYSDCRERR:
//                                            dcrarb_set_timeout_counter_control(0x1);//set non-zero value to enable timeout counter
//                                            setup_interrupt(interrupt_number, ARB_SYSDCRERR_inj_int_handler);
//                                            test_event(EVENT_GENERATE_INT24_ARB_SYSDCRERR);
//                                            break;
//
//        default: rumboot_printf("Get wrong interrupt id for injection\n");
//                 test_event(EVENT_ERROR);
//                 break;
//    }
//
//    wait_interrupt(interrupt_number);
//}

void check_interrupt_from_io_dev_reg(uint32_t interrupt_number)
{
    test_setup( interrupt_number );

    switch (interrupt_number)
    {
        case O_SYSTEM_HUNG:
                                            rumboot_printf("Check software generation P6BC_O_SYSTEM_HUNG\n");
//                                            if (get_current_core()==CURRENT_CORE_PPC0)
//                                                setup_interrupt(interrupt_number, P6BC_O_SYSTEM_HUNG_L2C1_reg_int_handler);
//                                            else
//                                                setup_interrupt(interrupt_number, P6BC_O_SYSTEM_HUNG_L2C0_reg_int_handler);
                                            generate_P6BC_O_SYSTEM_HUNG(DCR_L2C_BASE);
                                            START_SLAVE_CORE_TEST_PART = 1;
                                            break;

        case PLB6PLB40_O_0_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_0_BR6TO4_INTR\n");
                                            setup_interrupt(interrupt_number, P6P4_0_BR6TO4_INTR_reg_int_handler);
                                            generate_BR6TO4_INTR(PLB6PLB4_0_BASE);
                                            break;

        case PLB6PLB41_O_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_1_BR6TO4_INTR\n");
                                            setup_interrupt(interrupt_number, P6P4_1_BR6TO4_INTR_reg_int_handler);
                                            generate_BR6TO4_INTR(PLB6PLB4_1_BASE);
                                            break;

        case PLB4XAHB1_INTR:
                                            rumboot_printf("Check software generation PLB4XAHB_1_INTR\n");
                                            setup_interrupt(interrupt_number, PLB4XAHB_1_INTR_reg_int_handler);
                                            generate_PLB4XAHB_INTR(PLB4PLB6_1_BASE);
                                            break;

        case ARB_SYSDCRERR:
                                            rumboot_printf("Check ARB_SYSDCRERR generation\n");
                                            setup_interrupt(interrupt_number, ARB_SYSDCRERR_reg_int_handler);
                                            generate_ARB_SYSDCRERR();
                                            break;

        default: rumboot_printf("Get wrong interrupt id for injection\n");
                 //test_event(EVENT_ERROR);
                 break;
    }

    wait_interrupt(interrupt_number);
}

void generate_P6BC_O_SYSTEM_HUNG(uint32_t l2c_dcr_base)
{
    l2c_l2_write(l2c_dcr_base,  L2C_L2PLBMCKEN1 , 0x0);
    l2c_l2_write(l2c_dcr_base, L2C_L2CPUMCKEN,0x0);
    //TODO: need to remove magic
    l2c_l2_write(l2c_dcr_base, L2C_L2WACCFG, L2C_L2WACCFG_DCR_read(l2c_dcr_base) | (1 << (31 - 15)));//set DisablePlbErrScrub
    dcr_write (DCR_PLB6_BC_BASE + PLB6BC_HCPP, PLB6BC_HCPP_4K);
    write_tlb_entries(&fake_tlb_entry,1);

    MEM32(0x00) = 0xBABAABBA;
    usleep(80);
}

void generate_BR6TO4_INTR(uint32_t base_address)
{
    rumboot_printf("Check BR6TO4_INTR generation\n");
    p64_dcr_write_P64_TESR(base_address, 0b1 << ESR_TESR_P6WPE_i);
}

void generate_PLB4XAHB_INTR(uint32_t plb4plb6_base)
{
    p46_dcr_write_P46_TESR(plb4plb6_base, 0b1 << ESR_TESR_P4APE_i);
}

void generate_ARB_SYSDCRERR()
{
    dcrarb_set_timeout_counter_control(0x1);//set non-zero value to enable timeout counter
    dcr_read(UNEXIST_DCR_ADDR);
}

void setup_interrupt(uint32_t interrupt_number, void *interrupt_handler)
{
    mpic128_unmask_interrupt( MPICx_BASE, interrupt_number );
    irq_set_handler(interrupt_number, interrupt_handler);
    irq_set_type(interrupt_number, irq_mpic_type_critical);
    irq_unmask(interrupt_number);
}

void wait_interrupt(uint32_t interrupt_number)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << interrupt_number)) && t++<TEST_MPW_CPU_025_IO_TIMEOUT);
    TEST_ASSERT((MC_HANDLED & (1 << interrupt_number)), "Interrupt handle timeout!");
}

//void DMA0_O_S_SLV_ERR_INT_inj_int_handler()
//{
//    rumboot_printf("DMA0_O_S_SLV_ERR_INT_inj_int_handler\n");
//    dma2plb6_disable_o_slv_err_interrupt(DMA2PLB6_0_BASE);
//    dma2plb6_dcr_write_PLB6_DMA_PSE(DMA2PLB6_0_BASE, 1 << DMA_PSE_PE_i);
//    SET_BIT(MC_HANDLED, irq_source_DMA0_O_S_SLV_ERR_INT);
//    test_event(EVENT_CLEAR_INT10_DMA0_O_S_SLV_ERR_INT);
//}
//
//void DMA1_O_S_SLV_ERR_INT_inj_int_handler()
//{
//    rumboot_printf("DMA1_O_S_SLV_ERR_INT_inj_int_handler\n");
//    dma2plb6_disable_o_slv_err_interrupt(DMA2PLB6_1_BASE);
//    dma2plb6_dcr_write_PLB6_DMA_PSE(DMA2PLB6_1_BASE, 1 << DMA_PSE_PE_i);
//    SET_BIT(MC_HANDLED, irq_source_DMA1_O_S_SLV_ERR_INT);
//    test_event(EVENT_CLEAR_INT15_DMA1_O_S_SLV_ERR_INT);
//}
//
//void P6BC_O_SYSTEM_HUNG_inj_int_handler()
//{
//    rumboot_printf("P6BC_O_SYSTEM_HUNG_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, O_SYSTEM_HUNG);
//    test_event(EVENT_CLEAR_INT16_P6BC_O_SYSTEM_HUNG);
//}
//
//void P6P4_0_BR6TO4_INTR_inj_int_handler()
//{
//    rumboot_printf("P6P4_0_BR6TO4_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
//    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_0_BASE) & (0b1 << ESR_TESR_P4MWE_i), "Wrong value in P6P4_0 ESR reg. Expected ESR[P4MWE]=1\n");
//    test_event(EVENT_CLEAR_INT17_P6P4_0_BR6TO4_INTR);
//    p64_dcr_write_P64_ESR(PLB6PLB4_0_BASE, ((0b1 << ESR_TESR_P4MWE_i)));
//}
//
//void P6P4_1_BR6TO4_INTR_inj_int_handler()
//{
//    rumboot_printf("P6P4_1_BR6TO4_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
//    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_1_BASE) & (0b1 << ESR_TESR_P4MWE_i), "Wrong value in P6P4_1 ESR reg. Expected ESR[P4MWE]=1\n");
//    test_event(EVENT_CLEAR_INT18_P6P4_1_BR6TO4_INTR);
//    p64_dcr_write_P64_ESR(PLB6PLB4_1_BASE, ((0b1 << ESR_TESR_P4MWE_i)));
//}
//
//void PLB6AHB_1_BR6TO4_INTR_inj_int_handler()
//{
//    rumboot_printf("PLB6AHB_1_BR6TO4_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, irq_source_PLB6PLB4_2_O_BR6TO4_INTR);
//    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_2_BASE) & (0b1 << ESR_TESR_P6MIRQ_i), "Wrong value in PLB6AHB_1.P6P4 ESR reg. Expected ESR[MIRQ]=1\n");
//    test_event(EVENT_CLEAR_INT20_PLB6AHB_1_BR6TO4_INTR);
//}
//
//void PLB6AHB_2_BR6TO4_INTR_inj_int_handler()
//{
//    rumboot_printf("PLB6AHB_2_BR6TO4_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, irq_source_PLB6PLB4_3_O_BR6TO4_INTR);
//    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_3_BASE) & (0b1 << ESR_TESR_P6MIRQ_i), "Wrong value in PLB6AHB_2.P6P4 ESR reg. Expected ESR[MIRQ]=1\n");
//    test_event(EVENT_CLEAR_INT22_PLB6AHB_2_BR6TO4_INTR);
//}
//
//void PLB4XAHB_INTR_inj_int_handler()
//{
//    rumboot_printf("PLB4XAHB_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, irq_source_PLB4XAHB0_INTR);
//    test_event(EVENT_CLEAR_INT19_P4XAHB_INTR);
//}
//
//void PLB4XAHB_1_INTR_inj_int_handler()
//{
//    rumboot_printf("PLB4XAHB_1_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, PLB4XAHB1_INTR);
//    test_event(EVENT_CLEAR_INT21_P4XAHB_1_INTR);
//}
//
//void PLB4XAHB_2_INTR_inj_int_handler()
//{
//    rumboot_printf("PLB4XAHB_2_INTR_inj_int_handler\n");
//    SET_BIT(MC_HANDLED, irq_source_PLB4XAHB2_INTR);
//    test_event(EVENT_CLEAR_INT23_P4XAHB_2_INTR);
//}
//
//void ARB_SYSDCRERR_inj_int_handler()
//{
//    uint32_t daesr_reg;
//    rumboot_printf("ARB_SYSDCRERR_reg_int_handler\n");
//    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
//    daesr_reg = DCRARB_dcr_read_DAESR(DCR_ARB_BASE);
//    TEST_ASSERT((daesr_reg & (0b1 << DAESR_TE_i)) && (daesr_reg & (0b1 << DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
//    test_event(EVENT_CLEAR_INT24_ARB_SYSDCRERR);
//    dcrarb_clear_daesr();
//}


void P6BC_O_SYSTEM_HUNG_L2C_reg_int_handler()
{
    rumboot_printf("P6BC_O_SYSTEM_HUNG_L2C1_reg_int_handler\n");
    SET_BIT(MC_HANDLED, O_SYSTEM_HUNG);
    msync();
    L2C_L2WACCFG_DCR_write(DCR_L2C_BASE, L2C_L2WACCFG_DCR_read(DCR_L2C_BASE) & ~(1 << (31 - 15)));//clear DisablePlbErrScrub
    FINISH_SLAVE_CORE_TEST_PART = 1;
    msync();
    L2C1_L2PLBFRC0_DCR_write( L2C1_L2PLBFRC0 ,(1 << L2C_L2PLBFRC0_IntvnDataPE0_i) );//generate interrupt for reset slave core
}

void P6P4_0_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_0_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_0_BASE) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_0 ESR reg. Expected ESR[P6BPE]=1\n");
    p64_dcr_write_P64_ESR(PLB6PLB4_0_BASE, (0b1 << ESR_TESR_P6WPE_i));
}

void P6P4_1_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_1_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
    TEST_ASSERT(p64_dcr_read_P64_ESR(PLB6PLB4_1_BASE) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_1 ESR reg. Expected ESR[P6BPE]=1\n");
    p64_dcr_write_P64_ESR(PLB6PLB4_1_BASE, (0b1 << ESR_TESR_P6WPE_i));
}

void PLB4XAHB_1_INTR_reg_int_handler()
{
    rumboot_printf("PLB4XAHB_1_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB4XAHB1_INTR);
    TEST_ASSERT(p46_dcr_read_P46_ESR(PLB4PLB6_1_BASE) & (0b1 << ESR_TESR_P4APE_i), "Wrong value in PLB6AHB_1.P4P6 ESR reg. Expected ESR[P4APE]=1\n");
    //TODO: need to resolve endian of DCR bus of PLB4XAHB_1 module
    //in PLB4XAHB documentation it is mentioned that bit numbering is [31:0] (AHB numbering),
    // == [0:31] (DCR numbering). Since in C MSB == 31 and LSB == 0, we use direct enumerating rule (*_e)
    rumboot_printf("AHB2PLB_ESR = "); trace_hex(plb4xahb_dcr_read_AHB2PLB_ESR(PLB4AHB_1_BASE));
    TEST_ASSERT(plb4xahb_dcr_read_AHB2PLB_ESR(PLB4AHB_1_BASE) & (0b1 << AHB2PLB_ESR_MIRQ_i), "Wrong value in PLB6AHB_1.PLB4XAHB AHB2PLB_ESR reg. Expected AHB2PLB_ESR[MIRQ]=1\n");
    p46_dcr_write_P46_ESR(PLB4PLB6_1_BASE, (0b1 << ESR_TESR_P4APE_e));
}

void ARB_SYSDCRERR_reg_int_handler()
{
    uint32_t daesr_reg;
    rumboot_printf("ARB_SYSDCRERR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
    daesr_reg = DCRARB_dcr_read_DAESR(DCR_ARB_BASE);
    TEST_ASSERT((daesr_reg & (0b1 << DAESR_TE_i)) && (daesr_reg & (0b1 << DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
    dcrarb_clear_daesr();
}

int main()
{

       rumboot_irq_set_exception_handler(exception_handler);
//    check_interrupt_from_io_dev_inj(irq_source_DMA0_O_S_SLV_ERR_INT);
//    check_interrupt_from_io_dev_inj(irq_source_DMA1_O_S_SLV_ERR_INT);
//    check_interrupt_from_io_dev_inj(O_SYSTEM_HUNG);
//    check_interrupt_from_io_dev_inj(PLB6PLB40_O_0_BR6TO4_INTR);
//    check_interrupt_from_io_dev_inj(PLB6PLB41_O_BR6TO4_INTR);
//    check_interrupt_from_io_dev_inj(irq_source_PLB6PLB4_2_O_BR6TO4_INTR);
//    check_interrupt_from_io_dev_inj(irq_source_PLB6PLB4_3_O_BR6TO4_INTR);
//    check_interrupt_from_io_dev_inj(irq_source_PLB4XAHB0_INTR);
//    check_interrupt_from_io_dev_inj(PLB4XAHB1_INTR);
//    check_interrupt_from_io_dev_inj(irq_source_PLB4XAHB2_INTR);
//    check_interrupt_from_io_dev_inj(ARB_SYSDCRERR);

    MC_HANDLED = 0;
    check_interrupt_from_io_dev_reg(O_SYSTEM_HUNG); //O_SYSTEM_HUNG
    check_interrupt_from_io_dev_reg(PLB6PLB40_O_0_BR6TO4_INTR); //PLB6PLB40_O_0_BR6TO4_INTR
    check_interrupt_from_io_dev_reg(PLB6PLB41_O_BR6TO4_INTR); //PLB6PLB41_O_BR6TO4_INTR
    //check_interrupt_from_io_dev_reg(irq_source_PLB6PLB4_2_O_BR6TO4_INTR);
    //check_interrupt_from_io_dev_reg(irq_source_PLB6PLB4_3_O_BR6TO4_INTR);
    //check_interrupt_from_io_dev_reg(irq_source_PLB4XAHB0_INTR);
    check_interrupt_from_io_dev_reg(PLB4XAHB1_INTR); //PLB4XAHB1_INTR
    //check_interrupt_from_io_dev_reg(irq_source_PLB4XAHB2_INTR);
    check_interrupt_from_io_dev_reg(ARB_SYSDCRERR); //ARB_SYSDCRERR

    return 0;
}
