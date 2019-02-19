/*
 * test_oi10_cpu_025_l2c.c
 *
 *  Created on: Feb 19, 2019
 *      Author: m.smolina
 */

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
#include <platform/interrupts.h>
#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_power_modes.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>

#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>
#include <platform/regs/sctl.h>
#include <platform/regs/fields/emi.h>

#include <devices/sp804.h>

#define EVENT_GENERATE_EXTMCK                    (TEST_EVENT_CODE_MIN + 0)
#define EVENT_CLEAR_EXTMCK                       (TEST_EVENT_CODE_MIN + 1)

#define EVENT_GENERATE_PLBMCK0                   (TEST_EVENT_CODE_MIN + 2)
#define EVENT_CLEAR_PLBMCK0                      (TEST_EVENT_CODE_MIN + 3)

#define EVENT_GENERATE_PLBMCK1                   (TEST_EVENT_CODE_MIN + 4)
#define EVENT_CLEAR_PLBMCK1                      (TEST_EVENT_CODE_MIN + 5)

#define EVENT_GENERATE_L2AMCK0                   (TEST_EVENT_CODE_MIN + 6)
#define EVENT_CLEAR_L2AMCK0                      (TEST_EVENT_CODE_MIN + 7)

#define EVENT_GENERATE_L2AMCK1                   (TEST_EVENT_CODE_MIN + 8)
#define EVENT_CLEAR_L2AMCK1                      (TEST_EVENT_CODE_MIN + 9)

#define EVENT_GENERATE_L2AMCK2                   (TEST_EVENT_CODE_MIN + 10)
#define EVENT_CLEAR_L2AMCK2                      (TEST_EVENT_CODE_MIN + 11)

#define EVENT_GENERATE_L1CMCK0                   (TEST_EVENT_CODE_MIN + 12)
#define EVENT_CLEAR_L1CMCK0                      (TEST_EVENT_CODE_MIN + 13)

#define EVENT_GENERATE_RACMCK0                   (TEST_EVENT_CODE_MIN + 14)
#define EVENT_CLEAR_RACMCK0                      (TEST_EVENT_CODE_MIN + 15)

#define EVENT_GENERATE_WACMCK0                   (TEST_EVENT_CODE_MIN + 16)
#define EVENT_CLEAR_WACMCK0                      (TEST_EVENT_CODE_MIN + 17)

#define EVENT_GENERATE_WACMCK1                   (TEST_EVENT_CODE_MIN + 18)
#define EVENT_CLEAR_WACMCK1                      (TEST_EVENT_CODE_MIN + 19)

#define EVENT_GENERATE_WACMCK2                   (TEST_EVENT_CODE_MIN + 20)
#define EVENT_CLEAR_WACMCK2                      (TEST_EVENT_CODE_MIN + 21)

#define TEST_MPW_CPU_025_L2C_TIMEOUT    100

//#define L2CX_DCR_BASE               L2C0_DCR_BASE
//#define IRQ_SOURCE_L2CX_MCHKOUT     irq_source_L2C0_MCHKOUT

volatile uint32_t MC_HANDLED __attribute__((section(".data")))   = 0;

static void exception_handler(int const id, char const * const name ) {
    //rumboot_printf( "Exception: %s\n", name );
    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i)
                                           | (0b1 << ITRPT_MCSR_L2_i) )),
                 "Unexpected exception" );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
}



void wait_interrupt(L2MCKEN_bits_t L2MCK_bit)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << L2MCK_bit)) && t++<TEST_MPW_CPU_025_L2C_TIMEOUT);
    TEST_ASSERT((MC_HANDLED & (1 << L2MCK_bit)), "Interrupt handle timeout!");
}

void check_l2c_mck_inj(L2MCKEN_bits_t L2MCK_bit)
{
    irq_unmask(IRQ_SOURCE_L2CX_MCHKOUT);
    switch (L2MCK_bit)
    {
        case L2MCK_PLBMCK0_i:   test_event(EVENT_GENERATE_PLBMCK0);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_PLBINT0);
                                trace_msg("wait interrupt from L2MCK_PLBMCK0\n");
                                break;

        case L2MCK_PLBMCK1_i:   test_event(EVENT_GENERATE_PLBMCK1);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_PLBINT1);
                                trace_msg("wait interrupt from L2MCK_PLBMCK1\n");
                                break;

        case L2MCK_L2AMCK0_i:   test_event(EVENT_GENERATE_L2AMCK0);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_L2AINT0);
                                trace_msg("wait interrupt from L2MCK_L2AMCK0\n");
                                break;

        case L2MCK_L2AMCK1_i:   test_event(EVENT_GENERATE_L2AMCK1);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_L2AINT1);
                                trace_msg("wait interrupt from L2MCK_L2AMCK1\n");
                                break;

        case L2MCK_L2AMCK2_i:   test_event(EVENT_GENERATE_L2AMCK2);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_L2AINT2);
                                L2C_L2ARRMCKEN2_DCR_write(L2CX_DCR_BASE, (1 << 0));
                                trace_msg("wait interrupt from L2MCK_L2AMCK2\n");
                                break;

        case L2MCK_L1CMCK0_i:   test_event(EVENT_GENERATE_L1CMCK0);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_L1CINT0);
                                trace_msg("wait interrupt from L2MCK_L1CMCK0\n");
                                break;

        case L2MCK_RACMCK0_i:   test_event(EVENT_GENERATE_RACMCK0);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_RACINT0);
                                trace_msg("wait interrupt from L2MCK_RACMCK0\n");
                                break;

        case L2MCK_WACMCK0_i:   test_event(EVENT_GENERATE_WACMCK0);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_WACINT0);
                                trace_msg("wait interrupt from L2MCK_WACMCK0\n");
                                break;

        case L2MCK_WACMCK1_i:   test_event(EVENT_GENERATE_WACMCK1);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_WACINT1);
                                trace_msg("wait interrupt from L2MCK_WACMCK1\n");
                                break;

        case L2MCK_WACMCK2_i:   test_event(EVENT_GENERATE_WACMCK2);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_WACINT2);
                                trace_msg("wait interrupt from L2MCK_WACMCK1\n");
                                break;



        case L2MCK_EXTMCK_i:    test_event(EVENT_GENERATE_EXTMCK);
                                l2c_global_mck_enable(L2CX_DCR_BASE, L2MCKEN_EXTINT);
                                trace_msg("wait interrupt from L2MCK_EXTMCK\n");
                                break;



        default:                trace_msg("Unexpected L2MCK bit for event generation\n");
                                test_event(EVENT_ERROR);
                                break;
    }
    wait_interrupt(L2MCK_bit);
}

void check_l2c_mck_reg(L2MCKEN_bits_t L2MCK_bit)
{
   // irq_unmask(L2C0_MCHKOUT);//?
    switch (L2MCK_bit)
    {
        case L2MCKEN_PLBMCK0:   dcr_write(DCR_L2C_BASE, L2C_L2PLBFRC0 , L2PLBFRC0_IntvnDataPE0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK0);
                                trace_msg("wait interrupt from L2MCK_PLBMCK0\n");
                                break;

        case L2MCKEN_PLBMCK1:   dcr_write(DCR_L2C_BASE, L2C_L2PLBFRC1,1 << L2PLBFRC1_WrtFetchDataCountError_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK1);
                                trace_msg("wait interrupt from L2MCK_PLBMCK1\n");
                                break;

        case L2MCKEN_L2AMCK0:   dcr_write(DCR_L2C_BASE, L2C_L2ARRFRC0,1 << L2ARRFRC0_RDFPE14_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK0);
                                trace_msg("wait interrupt from L2MCK_L2AMCK0\n");
                                break;

        case L2MCKEN_L2AMCK1:   dcr_write(DCR_L2C_BASE, L2C_L2ARRFRC1, 1 << L2ARRFRC1_DataUE3_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK1);
                                trace_msg("wait interrupt from L2MCK_L2AMCK1\n");
                                break;

        case L2MCKEN_L2AMCK2:   dcr_write(DCR_L2C_BASE, L2C_L2ARRMCKEN2,1 << L2ARRFRC2_LruPE0_i);
                                dcr_write(DCR_L2C_BASE, L2C_L2ARRFRC2, 1 << L2ARRFRC2_LruPE0_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK2);
                                trace_msg("wait interrupt from L2MCK_L2AMCK2\n");
                                break;

        case L2MCKEN_L1CMCK0:   dcr_write(DCR_L2C_BASE, L2C_L2CPUMCKEN, 1 << L2CPUFRC_DwDataPE2_i);
                                dcr_write(DCR_L2C_BASE, L2C_L2CPUFRC, 1 << L2CPUFRC_DwDataPE2_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L1CMCK0);
                                trace_msg("wait interrupt from L2MCK_L1CMCK0\n");
                                break;

        case L2MCKEN_RACMCK0:   dcr_write(DCR_L2C_BASE, L2C_L2RACFRC0,1 << L2RACFRC0_LdBuff7PE1_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_RACMCK0);
                                trace_msg("wait interrupt from L2MCK_RACMCK0\n");
                                break;

        case L2MCKEN_WACMCK0:   L2C_L2WACFRC0_DCR_write(DCR_L2C_BASE, 1 << L2WACFRC0_StBuff0PE0_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK0);
                                trace_msg("wait interrupt from L2MCK_WACMCK0\n");
                                break;

        case L2MCKEN_WACMCK1:   L2C_L2WACFRC1_DCR_write(DCR_L2C_BASE, 1 << L2WACFRC1_NcBuff3PE1_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK1);
                                trace_msg("wait interrupt from L2MCK_WACMCK1\n");
                                break;

        case L2MCKEN_WACMCK2:   L2C_L2WACFRC2_DCR_write(DCR_L2C_BASE, 1 << L2WACFRC2_CoBuff6PE2_i);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK2);
                                trace_msg("wait interrupt from L2MCK_WACMCK2\n");
                                break;

        default:                trace_msg("Unexpected L2MCK bit for event generation\n");
                                test_event(EVENT_ERROR);
                                break;
    }
    wait_interrupt(L2MCK_bit);
}

void critical_interrupt_handler_inj()
{
    uint32_t status;
    status = L2C_L2MCK_DCR_read(L2CX_DCR_BASE);
    trace_msg("status= "); trace_hex(status);
    if (status & (1 << L2MCK_PLBMCK0_i)) {
        trace_msg("Detected PLBMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_PLBMCK0_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_PLBINT0);
        test_event(EVENT_CLEAR_PLBMCK0);
        L2C_L2PLBSTAT0_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_PLBMCK1_i)) {
        trace_msg("Detected PLBMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_PLBMCK1_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_PLBINT1);
        test_event(EVENT_CLEAR_PLBMCK1);
        L2C_L2PLBSTAT1_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_L2AMCK0_i)) {
        trace_msg("Detected L2AMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK0_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_L2AINT0);
        test_event(EVENT_CLEAR_L2AMCK0);
        L2C_L2ARRSTAT0_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_L2AMCK1_i)) {
        trace_msg("Detected L2AMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK1_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_L2AINT1);
        test_event(EVENT_CLEAR_L2AMCK1);
        L2C_L2ARRSTAT1_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_L2AMCK2_i)) {
        trace_msg("Detected L2AMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK2_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_L2AINT2);
        L2C_L2ARRMCKEN2_DCR_write(L2CX_DCR_BASE, 0x00000000);
        test_event(EVENT_CLEAR_L2AMCK2);
        L2C_L2ARRSTAT2_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_L1CMCK0_i)) {
        trace_msg("Detected L1CMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L1CMCK0_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_L1CINT0);
        test_event(EVENT_CLEAR_L1CMCK0);
        L2C_L2CPUSTAT_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_RACMCK0_i)) {
        trace_msg("Detected RACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_RACMCK0_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_RACINT0);
        test_event(EVENT_CLEAR_RACMCK0);
        L2C_L2RACSTAT0_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_WACMCK0_i)) {
        trace_msg("Detected WACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK0_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_WACINT0);
        test_event(EVENT_CLEAR_WACMCK0);
        L2C_L2WACSTAT0_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_WACMCK1_i)) {
        trace_msg("Detected WACMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK1_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_WACINT1);
        test_event(EVENT_CLEAR_WACMCK1);
        L2C_L2WACSTAT1_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_WACMCK2_i)) {
        trace_msg("Detected WACMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK2_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_WACINT2);
        test_event(EVENT_CLEAR_WACMCK2);
        L2C_L2WACSTAT2_DCR_write(L2CX_DCR_BASE, (1 << 0));
        return;
    }
    if (status & (1 << L2MCK_EXTMCK_i)) {
        trace_msg("Detected EXTMCK interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_EXTMCK_i);
        l2c_global_mck_disable(L2CX_DCR_BASE, L2MCKEN_EXTINT);
        test_event(EVENT_CLEAR_EXTMCK);
        return;
    }

    trace_msg("unexpected L2C machine check\n");
    trace_msg("L2MCK = "); trace_hex(status);
    test_event(EVENT_ERROR);
}

void critical_interrupt_handler_reg()
{
    uint32_t status;
    status = dcr_read(DCR_L2C_BASE, L2C_L2MCK);

    if (status & (1 << L2MCK_PLBMCK0_i)) {
        trace_msg("Detected PLBMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_PLBMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBINT0);
        L2C_L2PLBFERR0_DCR_read(DCR_L2C_BASE);
        L2C_L2PLBFRC0_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_PLBMCK1_i)) {
        trace_msg("Detected PLBMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_PLBMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBINT1);
        L2C_L2PLBFERR1_DCR_read(DCR_L2C_BASE);
        L2C_L2PLBFRC1_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_L2AMCK0_i)) {
        trace_msg("Detected L2AMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AINT0);
        L2C_L2ARRFERR0_DCR_read(DCR_L2C_BASE);
        L2C_L2ARRFRC0_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_L2AMCK1_i))
    {
        trace_msg("Detected L2AMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AINT1);
        L2C_L2ARRFERR1_DCR_read(DCR_L2C_BASE);
        L2C_L2ARRFRC1_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_L2AMCK2_i)) {
        trace_msg("Detected L2AMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L2AMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AINT2);
        L2C_L2ARRFERR2_DCR_read(DCR_L2C_BASE);
        L2C_L2ARRFRC2_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_L1CMCK0_i)) {
        trace_msg("Detected L1CMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_L1CMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L1CINT0);
        L2C_L2CPUFERR_DCR_read(DCR_L2C_BASE);
        L2C_L2CPUFRC_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_RACMCK0_i)) {
        trace_msg("Detected RACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_RACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_RACINT0);
        L2C_L2RACFERR0_DCR_read(DCR_L2C_BASE);
        L2C_L2RACFRC0_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_WACMCK0_i)) {
        trace_msg("Detected WACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACINT0);
        L2C_L2WACFERR0_DCR_read(DCR_L2C_BASE);
        L2C_L2WACFRC0_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_WACMCK1_i)) {
        trace_msg("Detected WACMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACINT1);
        L2C_L2WACFERR1_DCR_read(DCR_L2C_BASE);
        L2C_L2WACFRC1_DCR_write(DCR_L2C_BASE, 0);
        return;
    }
    if (status & (1 << L2MCK_WACMCK2_i)) {
        trace_msg("Detected WACMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2MCK_WACMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACINT2);
        L2C_L2WACFERR2_DCR_read(DCR_L2C_BASE);
        L2C_L2WACFRC2_DCR_write(DCR_L2C_BASE, 0);
        return;
    }

    trace_msg("unexpected L2C machine check\n");
    trace_msg("L2MCK = "); trace_hex(status);
    test_event(EVENT_ERROR);
}

void test_setup()
{
//    irq_init();
//    irq_set_handler(L2C0_MCHKOUT, critical_interrupt_handler_inj);//?
//    irq_set_type(IRQ_SOURCE_L2CX_MCHKOUT, irq_mpic_type_critical);
//    mpic128_unmask_interrupt( MPICx_BASE, IRQ_SOURCE_L2CX_MCHKOUT );
//
//    l2c_global_mck_disable(DCR_L2C_BASE, 0x00000FFE);//DCR_L2C_BASE

    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( L2C0_MCHKOUT );
    L2C_InterruptEnable();
    rumboot_irq_sei();

    MC_HANDLED = 0;
}

int main()
{
      test_setup();

//    trace_msg("Generation machine check with injectors\n");
//    check_l2c_mck_inj(L2MCK_EXTMCK_i);
//    check_l2c_mck_inj(L2MCK_PLBMCK0_i);
//    check_l2c_mck_inj(L2MCK_PLBMCK1_i);
//    check_l2c_mck_inj(L2MCK_L2AMCK0_i);
//    check_l2c_mck_inj(L2MCK_L2AMCK1_i);
//    check_l2c_mck_inj(L2MCK_L2AMCK2_i);
//    check_l2c_mck_inj(L2MCK_L1CMCK0_i);
//    check_l2c_mck_inj(L2MCK_RACMCK0_i);
//    check_l2c_mck_inj(L2MCK_WACMCK0_i);
//    check_l2c_mck_inj(L2MCK_WACMCK1_i);
//    check_l2c_mck_inj(L2MCK_WACMCK2_i);

    //irq_set_handler(IRQ_SOURCE_L2CX_MCHKOUT, critical_interrupt_handler_reg);
    MC_HANDLED = 0;
    trace_msg("Generation machine check with L2C *FRC* registers\n");
    check_l2c_mck_reg(L2MCKEN_PLBMCK0);
    check_l2c_mck_reg(L2MCKEN_PLBMCK1);
    check_l2c_mck_reg(L2MCKEN_L2AMCK0);
    check_l2c_mck_reg(L2MCKEN_L2AMCK1);
    check_l2c_mck_reg(L2MCKEN_L2AMCK2);
    check_l2c_mck_reg(L2MCKEN_L1CMCK0);
    check_l2c_mck_reg(L2MCKEN_RACMCK0);
    check_l2c_mck_reg(L2MCKEN_WACMCK0);
    check_l2c_mck_reg(L2MCKEN_WACMCK1);
    check_l2c_mck_reg(L2MCKEN_WACMCK2);

    return 0;
}
