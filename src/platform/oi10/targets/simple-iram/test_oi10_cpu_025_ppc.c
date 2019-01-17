/*
 * test_oi10_cpu_025_ppc.c
 *
 *  Created on: Jan 16, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <platform/test_event_c.h>
#include <rumboot/irq.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>

volatile uint32_t MC_HANDLED __attribute__((section(".data")))   = 0;

static const tlb_entry em0_0_cache_off_valid    = {TLB_ENTRY_EM0_0_NOCACHE_VALID};

static void exit_from_machine_check_interrupt(uint32_t MCSR_value)
{
    //SPR_MCSRR0_write((uint32_t) exit_from_test);
    //SPR_MCSRR1_write(msr_read());
    //msr_write(0x0);

    SPR_MCSR_C_write(MCSR_value);
}

static void check_mc_status_CCR1(uint32_t mc_interrupt_status)
{
    TEST_ASSERT(mc_interrupt_status & (1 << MCSR_MCS_i), "Failed to set machine check interrupt");
    if (mc_interrupt_status & (1<<MCSR_TLB_i))
    {
        disable_mmu_parity_error_insert();
        write_tlb_entries(&bootrom_mirror,1);//fix tlb errors
        SET_BIT(MC_HANDLED, MCSR_TLB_i);
        trace_msg("detected 'UTLB parity error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<MCSR_IC_i))
    {
        disable_L1_parity_checking();
        SET_BIT(MC_HANDLED, MCSR_IC_i);
        trace_msg("detected 'I-cache asynchronous error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<MCSR_DC_i))
    {
        disable_L1_parity_checking();
        SET_BIT(MC_HANDLED, MCSR_DC_i);
        trace_msg("detected 'D-cache error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<MCSR_GPR_i))
    {
        SET_BIT(MC_HANDLED, MCSR_GPR_i);
        trace_msg("detected 'GPR parity error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<MCSR_FPR_i))
    {
        SET_BIT(MC_HANDLED, MCSR_FPR_i);
        trace_msg("detected 'FPR parity error' interrupt\n");
    }
/*
    else if (mc_interrupt_status & (1<<MCSR_IMP_i))
    {
        SET_BIT(MC_HANDLED, MCSR_IMP_i);
        trace_msg("detected 'Imprecise machine check' interrupt\n");
    }
*/
    else if (mc_interrupt_status & (1<<MCSR_L2_i))
    {
        SET_BIT(MC_HANDLED, MCSR_L2_i);
        trace_msg("detected 'Error reported through the L2 cache' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<MCSR_DCR_i))
    {
        SET_BIT(MC_HANDLED, MCSR_DCR_i);
        trace_msg("detected 'DCR timeout' interrupt\n");
    }
    else
    {
        trace_msg("unexpected machine check interrupt\n");
        trace_msg("MCSR = "); trace_hex(mc_interrupt_status);
        test_event(EVENT_ERROR);
    }
}

static void machine_check_interrupt_handler_CCR1()
{
    uint32_t mc_interrupt_status;
    mc_interrupt_status = SPR_MCSR_RW_read();
    check_mc_status_CCR1(mc_interrupt_status);
    exit_from_machine_check_interrupt(mc_interrupt_status);
}

static void setup_machine_check_interrupt(void *interrupt_handled)
{
    irq_init_cpu_int_vec();
    irq_set_handler(RUMBOOT_IRQ_MACHINE_CHECK, interrupt_handled);

}

void test_setup_CCR1()
{
    rumboot_printf("Check interrupts created by CCR1\n");
    MC_HANDLED = 0;
    enable_machine_check_on_dcr_timeout();
    setup_machine_check_interrupt(machine_check_interrupt_handler_CCR1);
    write_tlb_entries(&em0_0_cache_off_valid,1);//set em0_0 cache_off tlb_entry
/*
    ddr_init_impl(
            DdrHlbId_Default,
            DdrInitMode_Default,
            DdrEccMode_Default,
            DdrPmMode_Default,
            DdrBurstLength_4,
            DdrPartialWriteMode_ReadModifyWrite);
*/
    ddr_init_impl(DdrHlbId_Em0,
            DdrInitMode_ViolateSpecWaitRequirements,
            DdrEccMode_Off,
            DdrPmMode_DisableAllPowerSavingFeatures,
            DdrBurstLength_8,
            DdrPartialWriteMode_DataMask);

    rumboot_printf("ddr init finished");
//    write_tlb_entries(&em0_0_cache_off_invalid,1);//invalidate em0_0 cache_off tlb_entry
    write_tlb_entries(&em0_0_cache_on_valid,1);//set em0_0 cache_on tlb_entry
}

int main ()
{
    //test_event_send_test_id( "test_oi10_cpu_025_rst");

    rumboot_printf("TEST START\n");

    test_setup_CCR1();
    check_mc_with_CCR1(MCSR_TLB_i);
    check_mc_with_CCR1(MCSR_IC_i);
    check_mc_with_CCR1(MCSR_DC_i);
    check_mc_with_CCR1(MCSR_FPR_i);
//    check_mc_with_CCR1(MCSR_GPR_i);
    check_mc_with_CCR1(MCSR_DCR_i);


    rumboot_printf("TEST OK\n");
    return 0;
}
