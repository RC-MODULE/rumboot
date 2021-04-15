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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <devices/mpic128.h>

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

//#define DCR_L2C_BASE               L2C0_DCR_BASE
//#define IRQ_SOURCE_L2CX_MCHKOUT     irq_source_L2C0_MCHKOUT

static struct rumboot_irq_entry *tbl;

volatile uint32_t MC_HANDLED  = 0;


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

void wait_interrupt(L2MCKEN_bits_t L2MCK_bit)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << L2MCK_bit)) && t++<TEST_MPW_CPU_025_L2C_TIMEOUT);
    TEST_ASSERT((MC_HANDLED & (1 << L2MCK_bit)), "Interrupt handle timeout!");
}

#ifdef USE_HARDWARE_PARTS
void check_l2c_mck_inj(L2C_L2MCKEN_FIELD L2MCK_bit)
{
    //irq_unmask(IRQ_SOURCE_L2CX_MCHKOUT);
    switch (L2MCK_bit)
    {
        case L2C_L2MCKEN_PLBMCK0_i:   test_event(EVENT_GENERATE_PLBMCK0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK0);
                                rumboot_printf("wait interrupt from L2MCK_PLBMCK0\n");
                                break;

        case L2C_L2MCKEN_PLBMCK1_i:   test_event(EVENT_GENERATE_PLBMCK1);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK1);
                                rumboot_printf("wait interrupt from L2MCK_PLBMCK1\n");
                                break;

        case L2C_L2MCKEN_L2AMCK0_i:   test_event(EVENT_GENERATE_L2AMCK0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK0);
                                rumboot_printf("wait interrupt from L2MCK_L2AMCK0\n");
                                break;

        case L2C_L2MCKEN_L2AMCK1_i:   test_event(EVENT_GENERATE_L2AMCK1);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK1);
                                rumboot_printf("wait interrupt from L2MCK_L2AMCK1\n");
                                break;

        case L2C_L2MCKEN_L2AMCK2_i:   test_event(EVENT_GENERATE_L2AMCK2);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK2);
                                l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRMCKEN2,(1 << 0));
                                rumboot_printf("wait interrupt from L2MCK_L2AMCK2\n");
                                break;

        case L2C_L2MCKEN_L1CMCK0_i:   test_event(EVENT_GENERATE_L1CMCK0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L1CMCK0);
                                rumboot_printf("wait interrupt from L2MCK_L1CMCK0\n");
                                break;

        case L2C_L2MCKEN_RACMCK0_i:   test_event(EVENT_GENERATE_RACMCK0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_RACMCK0);
                                rumboot_printf("wait interrupt from L2MCK_RACMCK0\n");
                                break;

        case L2C_L2MCKEN_WACMCK0_i:   test_event(EVENT_GENERATE_WACMCK0);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK0);
                                rumboot_printf("wait interrupt from L2MCK_WACMCK0\n");
                                break;

        case L2C_L2MCKEN_WACMCK1_i:   test_event(EVENT_GENERATE_WACMCK1);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK1);
                                rumboot_printf("wait interrupt from L2MCK_WACMCK1\n");
                                break;

        case L2C_L2MCKEN_WACMCK2_i:   test_event(EVENT_GENERATE_WACMCK2);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK2);
                                rumboot_printf("wait interrupt from L2MCK_WACMCK1\n");
                                break;

        case L2C_L2MCKEN_EXTMCK_i:    test_event(EVENT_GENERATE_EXTMCK);
                                l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_EXTMCK);
                                rumboot_printf("wait interrupt from L2MCK_EXTMCK\n");
                                break;

        default:                rumboot_printf("Unexpected L2MCK bit for event generation\n");
                                test_event(EVENT_ERROR);
                                break;
    }
    wait_interrupt(L2MCK_bit);
}
#endif

void check_l2c_mck_reg(L2C_L2MCKEN_FIELD L2MCK_bit)
{
   //irq_unmask(L2C0_MCHKOUT);//?
    switch (L2MCK_bit)
    {
        case L2C_L2MCKEN_PLBMCK0_i:   l2c_l2_write(DCR_L2C_BASE,  L2C_L2PLBFRC0 ,1 << L2C_L2PLBFRC0_IntvnDataPE0_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK0);
                                      rumboot_printf("wait interrupt from L2MCK_PLBMCK0\n");
                                      break;

        case L2C_L2MCKEN_PLBMCK1_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBFRC1,1 << L2C_L2PLBFRC1_WrtFetchDataCountError_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_PLBMCK1);
                                      rumboot_printf("wait interrupt from L2MCK_PLBMCK1\n");
                                      break;

        case L2C_L2MCKEN_L2AMCK0_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC0, 1 << L2C_L2ARRFRC0_RDFPE14_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK0);
                                      rumboot_printf("wait interrupt from L2MCKEN_L2AMCK0\n");
                                      break;

        case L2C_L2MCKEN_L2AMCK1_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC1, 1 << L2C_L2ARRFRC1_DataUE3_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK1);
                                      rumboot_printf("wait interrupt from L2MCK_L2AMCK1\n");
                                      break;

        case L2C_L2MCKEN_L2AMCK2_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRMCKEN2,1 << L2C_L2ARRFRC2_LruPE0_i);
                                      l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC2, 1 << L2C_L2ARRFRC2_LruPE0_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L2AMCK2);
                                      rumboot_printf("wait interrupt from L2MCK_L2AMCK2\n");
                                      break;

        case L2C_L2MCKEN_L1CMCK0_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2CPUMCKEN, 1 << L2C_L2CPUFRC_DwDataPE2_i);
                                      l2c_l2_write(DCR_L2C_BASE, L2C_L2CPUFRC, 1 << L2C_L2CPUFRC_DwDataPE2_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_L1CMCK0);
                                      rumboot_printf("wait interrupt from L2MCK_L1CMCK0\n");
                                      break;

        case L2C_L2MCKEN_RACMCK0_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2RACFRC0,1 << L2C_L2RACFRC0_LdBuff7PE1_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_RACMCK0);
                                      rumboot_printf("wait interrupt from L2MCK_RACMCK0\n");
                                      break;

        case L2C_L2MCKEN_WACMCK0_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2WACFRC0, 1 << L2C_L2WACFRC0_StBuff0PE0_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK0);
                                      rumboot_printf("wait interrupt from L2MCK_WACMCK0\n");
                                      break;

        case L2C_L2MCKEN_WACMCK1_i:   l2c_l2_write(DCR_L2C_BASE,  L2C_L2WACFRC1, 1 << L2C_L2WACFRC1_NcBuff3PE1_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK1);
                                      rumboot_printf("wait interrupt from L2MCK_WACMCK1\n");
                                      break;

        case L2C_L2MCKEN_WACMCK2_i:   l2c_l2_write(DCR_L2C_BASE, L2C_L2WACFRC2, 1 << L2C_L2WACFRC2_CoBuff6PE2_i);
                                      l2c_global_mck_enable(DCR_L2C_BASE, L2MCKEN_WACMCK2);
                                      rumboot_printf("wait interrupt from L2MCK_WACMCK2\n");
                                      break;

        default:                      rumboot_printf("Unexpected L2MCK bit for event generation\n");
                                      test_event(EVENT_ERROR);
                                      break;
    }
    wait_interrupt(L2MCK_bit);
}

#ifdef USE_HARDWARE_PARTS
void critical_interrupt_handler_inj()
{
    uint32_t status;
    status = l2c_l2_read(DCR_L2C_BASE, L2C_L2MCK);
    rumboot_printf("status= %x\n", status);

    if (status & (1 << L2C_L2MCKEN_PLBMCK0_i)) {
        rumboot_printf("Detected PLBMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_PLBMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBMCK0);
        test_event(EVENT_CLEAR_PLBMCK0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT0,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_PLBMCK1_i)) {
        rumboot_printf("Detected PLBMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_PLBMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBMCK1);
        test_event(EVENT_CLEAR_PLBMCK1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT1,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK0_i)) {
        rumboot_printf("Detected L2AMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK0);
        test_event(EVENT_CLEAR_L2AMCK0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRSTAT0,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK1_i)) {
        rumboot_printf("Detected L2AMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK1);
        test_event(EVENT_CLEAR_L2AMCK1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRSTAT1,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK2_i)) {
        rumboot_printf("Detected L2AMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK2);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRMCKEN2,0x00000000);
        test_event(EVENT_CLEAR_L2AMCK2);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRSTAT2,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L1CMCK0_i)) {
        rumboot_printf("Detected L1CMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L1CMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L1CMCK0);
        test_event(EVENT_CLEAR_L1CMCK0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2CPUSTAT,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_RACMCK0_i)) {
        rumboot_printf("Detected RACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_RACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_RACMCK0);
        test_event(EVENT_CLEAR_RACMCK0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2RACSTAT0,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK0_i)) {
        rumboot_printf("Detected WACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK0);
        test_event(EVENT_CLEAR_WACMCK0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACSTAT0,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK1_i)) {
        rumboot_printf("Detected WACMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK1);
        test_event(EVENT_CLEAR_WACMCK1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACSTAT1,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK2_i)) {
        rumboot_printf("Detected WACMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK2);
        test_event(EVENT_CLEAR_WACMCK2);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACSTAT2,(1 << 0));
        return;
    }
    if (status & (1 << L2C_L2MCKEN_EXTMCK_i)) {
        rumboot_printf("Detected EXTMCK interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_EXTMCK_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_EXTMCK);
        test_event(EVENT_CLEAR_EXTMCK);
        return;
    }

    rumboot_printf("unexpected L2C machine check\n");
    rumboot_printf("L2MCK = %x\n", status);
    test_event(EVENT_ERROR);
}
#endif

void critical_interrupt_handler_reg()
{
    uint32_t status;
    status = l2c_l2_read(DCR_L2C_BASE, L2C_L2MCK);

    if (status & (1 << L2C_L2MCKEN_PLBMCK0_i)) {
        rumboot_printf("Detected PLBMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_PLBMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBMCK0);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2PLBFERR0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBFRC0,  0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_PLBMCK1_i)) {
        rumboot_printf("Detected PLBMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_PLBMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_PLBMCK1);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2PLBFERR1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBFRC1,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK0_i)) {
        rumboot_printf("Detected L2AMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK0);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2ARRFERR0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC0, 0);
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK1_i))
    {
        rumboot_printf("Detected L2AMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK1);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2ARRFERR1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC1,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L2AMCK2_i)) {
        rumboot_printf("Detected L2AMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L2AMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L2AMCK2);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2ARRFERR2);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRFRC2,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_L1CMCK0_i)) {
        rumboot_printf("Detected L1CMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_L1CMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_L1CMCK0);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2CPUFERR);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2CPUFRC,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_RACMCK0_i)) {
        rumboot_printf("Detected RACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_RACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_RACMCK0);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2RACFERR0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2RACFRC0,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK0_i)) {
        rumboot_printf("Detected WACMCK0 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK0_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK0);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2WACFERR0);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACFRC0,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK1_i)) {
        rumboot_printf("Detected WACMCK1 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK1_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK1);
        l2c_l2_read(DCR_L2C_BASE, L2C_L2WACFERR1);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACFRC1,0);
        isync();
        return;
    }
    if (status & (1 << L2C_L2MCKEN_WACMCK2_i)) {
        rumboot_printf("Detected WACMCK2 interrupt from L2C\n");
        SET_BIT(MC_HANDLED, L2C_L2MCKEN_WACMCK2_i);
        l2c_global_mck_disable(DCR_L2C_BASE, L2MCKEN_WACMCK2);
        l2c_l2_read(DCR_L2C_BASE,  L2C_L2WACFERR2);
        l2c_l2_write(DCR_L2C_BASE, L2C_L2WACFRC2,0);
        isync();
        return;
    }

    rumboot_printf("unexpected L2C machine check\n");
    rumboot_printf("L2MCK = %x\n" ,status);
    //test_event(EVENT_ERROR);
}

void test_setup()
{
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_reg, NULL );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( L2C0_MCHKOUT );
    rumboot_irq_sei();

    MC_HANDLED = 0;
}

#ifdef USE_HARDWARE_PARTS
void test_setup_inj()
{
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, critical_interrupt_handler_inj, NULL );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( L2C0_MCHKOUT );
    rumboot_irq_sei();

    MC_HANDLED = 0;
}
#endif

int main()
{
#ifdef USE_HARDWARE_PARTS
    test_event_send_test_id( "test_oi10_cpu_025_l2c");
#endif

    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB SRAM0)

#ifdef USE_HARDWARE_PARTS
    test_setup_inj();
    MC_HANDLED = 0;
    rumboot_printf("Generation machine check with injectors\n");
    check_l2c_mck_inj(L2C_L2MCKEN_EXTMCK_i);
    check_l2c_mck_inj(L2C_L2MCKEN_PLBMCK0_i);
    check_l2c_mck_inj(L2C_L2MCKEN_PLBMCK1_i);
    check_l2c_mck_inj(L2C_L2MCKEN_L2AMCK0_i);
    check_l2c_mck_inj(L2C_L2MCKEN_L2AMCK1_i);
    check_l2c_mck_inj(L2C_L2MCKEN_L2AMCK2_i);
    check_l2c_mck_inj(L2C_L2MCKEN_L1CMCK0_i);
    check_l2c_mck_inj(L2C_L2MCKEN_RACMCK0_i);
    check_l2c_mck_inj(L2C_L2MCKEN_WACMCK0_i);
    check_l2c_mck_inj(L2C_L2MCKEN_WACMCK1_i);
    check_l2c_mck_inj(L2C_L2MCKEN_WACMCK2_i);
#endif

    test_setup();
    MC_HANDLED = 0;
    rumboot_printf("Generation machine check with L2C *FRC* registers\n");
    check_l2c_mck_reg(L2C_L2MCKEN_PLBMCK0_i);
    check_l2c_mck_reg(L2C_L2MCKEN_PLBMCK1_i);
    check_l2c_mck_reg(L2C_L2MCKEN_L2AMCK0_i);
    check_l2c_mck_reg(L2C_L2MCKEN_L2AMCK1_i);
    check_l2c_mck_reg(L2C_L2MCKEN_L2AMCK2_i);
    check_l2c_mck_reg(L2C_L2MCKEN_L1CMCK0_i);
    check_l2c_mck_reg(L2C_L2MCKEN_RACMCK0_i);
    check_l2c_mck_reg(L2C_L2MCKEN_WACMCK0_i);
    check_l2c_mck_reg(L2C_L2MCKEN_WACMCK1_i);
    check_l2c_mck_reg(L2C_L2MCKEN_WACMCK2_i);

    return 0;
}
