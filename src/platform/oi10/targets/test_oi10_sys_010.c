/* -*- rumboot-test-labels: broken -*- */
/*
 * test_oi10_sys_010.c
 *
 *  Created on: Nov 07, 2018
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/test_macro.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/regs/regs_cldcr.h>
#include <platform/regs/regs_itrace.h>



#define TLB_ENTRY_CACHE           MMU_TLB_ENTRY( 0x000, 0x40000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b0,  0b0, 0b1, 0b0, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b1,0b1, 0b0,   0b0,  0b0, MEM_WINDOW_0, MMU_TLBWE_WAY_3,   MMU_TLBWE_BE_UND, 0b1 )
#define TLB_ENTRY_INV             MMU_TLB_ENTRY( 0x000, 0x40000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b1,  0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b1,0b1, 0b0,   0b0,  0b0, MEM_WINDOW_0, MMU_TLBWE_WAY_3,   MMU_TLBWE_BE_UND, 0b0 )

typedef void func(void);

#define FUNC_ADDR1      (SSRAM_BASE + 0x0)
#define FUNC_ADDR2      (SSRAM_BASE + 0x1000)
#define FUNC_ADDR3      (SSRAM_BASE + 0x3F000)

func* trace_bt_func = (func*)(FUNC_ADDR1);
func* trace1_bt_func1 = (func*)(FUNC_ADDR2);


enum {
       TEC_CHECK_TRACE_CONFIGURED = TEST_EVENT_CODE_MIN,
       TEC_CHECK_TRACE_IAC,
	   TEC_CHECK_TRACE_DAC_READ,
	   TEC_CHECK_TRACE_DAC_WRITE,
	   TEC_CHECK_TRACE_DVC_READ,
	   TEC_CHECK_TRACE_DVC_WRITE,
	   TEC_CHECK_TRACE_TRAP,
	   TEC_CHECK_TRACE_IRPT,
	   TEC_CHECK_TRACE_UDE,
	   TEC_CHECK_TRACE_RET,
	   TEC_CHECK_TRACE_BT,
	   TEC_CHECK_TRACE_ICMP
} test_event_code;

enum {
   	TEST_DATA_OK        = 0x12345678,
	TEST_DATA_PROGRAM   = 0xC0DE1234,
	TEST_DATA_DEBUG     = 0xDBC12345,
	TEST_DATA_SC        = 0x5C123456,
   	TEST_DATA_ERROR     = 0xFFFFFFFF
} test_data;

volatile static uint32_t debug_flag = TEST_DATA_ERROR;

static void my_exception_handler(int id, const char *name)
{
	switch(id){
	case RUMBOOT_IRQ_DEBUG:
        if (debug_flag == TEST_DATA_DEBUG) debug_flag = TEST_DATA_OK;
		break;
	case RUMBOOT_IRQ_PROGRAM:
    	spr_write(SPR_ESR,0);
        if (debug_flag == TEST_DATA_PROGRAM)
        {
        	uint32_t addr = spr_read(SPR_SRR0);
        	spr_write(SPR_SRR0,addr+4);
        	debug_flag = TEST_DATA_OK;
        }
		break;
	case RUMBOOT_IRQ_SYSTEM_CALL:
        if (debug_flag == TEST_DATA_SC) debug_flag = TEST_DATA_OK;
		break;
	default:
        rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("MSR:  0x%x\n", msr_read());
        rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
        rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
        rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
        rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
        rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
        rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
        rumboot_printf("---       ---       ---\n");
      	rumboot_platform_panic("Please reset or power-cycle the board\n");
      	break;
	}
	spr_write(SPR_DBSR_RC, 0xFFFFFFFF); // clear all debug interrupts
    spr_write(SPR_DBCR0, 0);            // disable all debug events
}

int check_trace_configured()
{
	dcr_write(DCR_CLDCR_BASE+CLDCR_ITRCTRL, (CLDCR_ITRCTRL_CPU_ITEN_PPC0 << CLDCR_ITRCTRL_CPU_ITEN_i));
        dcr_write(DCR_ITRACE_BASE+ITC0_TC, (1 << ITC0_TC_ITE_i)
                                         | (1 << ITC0_TC_SS_i)); // Instraction Trace Enable (bit 0).
                                                                 // Trace information from PowerPC[0] is selected (bit 4).

    test_event(TEC_CHECK_TRACE_CONFIGURED);

    spr_write(SPR_CCR0, (1 << CTRL_CCR0_ITE_i)
                      | (1 << CTRL_CCR0_PRE_i)
                      | (0 << CTRL_CCR0_CRPE_i)
                      | (CTRL_CCR0_ICS_32byte << CTRL_CCR0_ICS_i)
                      | (0 << CTRL_CCR0_DAPUIB_i)
                      | (0 << CTRL_CCR0_ICWRIDX_i)
                      | (0 << CTRL_CCR0_DTB_i)
                      | (0 << CTRL_CCR0_FLSTA_i)
                      | (CTRL_CCR0_DQWPM_No_prediction << CTRL_CCR0_DQWPM_i)
                      | (CTRL_CCR0_IQWPM_Use_EA << CTRL_CCR0_IQWPM_i));

    return 0;
}

int test_func()
{
	if(debug_flag == TEST_DATA_OK) return 0;
	return 1;
}

int check_trace_iac()
{
	int status = 1;

	rumboot_printf("IAC check test started!\n");

	debug_flag = TEST_DATA_DEBUG;

    //according to User's Manual ch. 8.5.7 Setup order for IACs, DACs, and DVCs
    spr_write(SPR_DBCR0, 0);
    spr_write(SPR_DBSR_RC, 0xFFFFFFFF);

    spr_write(SPR_IAC1, (uint32_t)&test_func);
	spr_write(SPR_IAC2, (uint32_t)&test_func);
	spr_write(SPR_IAC3, (uint32_t)&test_func);
	spr_write(SPR_IAC4, (uint32_t)&test_func);
	spr_write(SPR_DAC1, 0);
	spr_write(SPR_DAC2, 0);
	spr_write(SPR_DVC1, 0);
	spr_write(SPR_DVC2, 0);

    spr_write(SPR_DBCR1, (DEBUG_DBCR1_IACxUS_Both << DEBUG_DBCR1_IAC1US_i)
                       | (DEBUG_DBCR1_IACxER_E_IS_Both << DEBUG_DBCR1_IAC1ER_i)
                       | (DEBUG_DBCR1_IACxUS_Both << DEBUG_DBCR1_IAC2US_i)
                       | (DEBUG_DBCR1_IACxER_E_IS_Both << DEBUG_DBCR1_IAC2ER_i)
                       | (DEBUG_DBCR1_IACxxM_Exact_match << DEBUG_DBCR1_IAC12M_i)
                       | (DEBUG_DBCR1_IACxUS_Both << DEBUG_DBCR1_IAC3US_i)
                       | (DEBUG_DBCR1_IACxER_E_IS_Both << DEBUG_DBCR1_IAC3ER_i)
                       | (DEBUG_DBCR1_IACxUS_Both << DEBUG_DBCR1_IAC4US_i)
                       | (DEBUG_DBCR1_IACxER_E_IS_Both << DEBUG_DBCR1_IAC4ER_i)
                       | (DEBUG_DBCR1_IACxxM_Exact_match << DEBUG_DBCR1_IAC34M_i));

    //spr_write(SPR_DBCR2, );
    isync();

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (1 << DEBUG_DBCR0_IAC1_i)
                       | (1 << DEBUG_DBCR0_IAC2_i)
                       | (1 << DEBUG_DBCR0_IAC3_i)
                       | (1 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_IAC);

	status = test_func();

	spr_write(SPR_IAC1, 0);
	spr_write(SPR_IAC2, 0);
	spr_write(SPR_IAC3, 0);
	spr_write(SPR_IAC4, 0);
	spr_write(SPR_DBCR1, 0);
	return status;
}

#define DAC_READ_TEST_DATA 0xDAC8EAD1
volatile uint32_t dac_read_test = DAC_READ_TEST_DATA;

int check_trace_dac_read()
{
	volatile uint32_t dac_test;
	int status = 1;

	rumboot_printf("DAC read test started!\n");
    debug_flag = TEST_DATA_DEBUG;

    //according to User's Manual ch. 8.5.7 Setup order for IACs, DACs, and DVCs
    spr_write(SPR_DBCR0, 0);
    spr_write(SPR_DBSR_RC, 0xFFFFFFFF);

    spr_write(SPR_IAC1, 0);
	spr_write(SPR_IAC2, 0);
	spr_write(SPR_IAC3, 0);
	spr_write(SPR_IAC4, 0);
	spr_write(SPR_DAC1, &dac_read_test);
	spr_write(SPR_DAC2, &dac_read_test);
	spr_write(SPR_DVC1, 0);
	spr_write(SPR_DVC2, 0);

    spr_write(SPR_DBCR1, 0);

    spr_write(SPR_DBCR2, (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC1US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC1ER_i)
                       | (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC2US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC2ER_i)
                       | (DEBUG_DBCR2_DACxxM_Exact_match << DEBUG_DBCR2_DAC12M_i));
    isync();

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (1 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (1 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_DAC_READ);

	dac_test = dac_read_test;
	if(dac_test == DAC_READ_TEST_DATA) status = test_func();

	spr_write(SPR_DAC1, 0);
	spr_write(SPR_DAC2, 0);
	spr_write(SPR_DBCR2, 0);
	return status;
}

#define DAC_WRITE_TEST_DATA 0xDAC3817E
volatile uint32_t dac_write_test = 0xDEADBEAF;

int check_trace_dac_write()
{
	int status = 1;

	rumboot_printf("DAC write test started!\n");
    debug_flag = TEST_DATA_DEBUG;

    //according to User's Manual ch. 8.5.7 Setup order for IACs, DACs, and DVCs
    spr_write(SPR_DBCR0, 0);
    spr_write(SPR_DBSR_RC, 0xFFFFFFFF);

    spr_write(SPR_IAC1, 0);
	spr_write(SPR_IAC2, 0);
	spr_write(SPR_IAC3, 0);
	spr_write(SPR_IAC4, 0);
	spr_write(SPR_DAC1, &dac_write_test);
	spr_write(SPR_DAC2, &dac_write_test);
	spr_write(SPR_DVC1, 0);
	spr_write(SPR_DVC2, 0);

    spr_write(SPR_DBCR1, 0);

    spr_write(SPR_DBCR2, (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC1US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC1ER_i)
                       | (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC2US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC2ER_i)
                       | (DEBUG_DBCR2_DACxxM_Exact_match << DEBUG_DBCR2_DAC12M_i));
    isync();

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (1 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (1 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_DAC_WRITE);

    dac_write_test = DAC_WRITE_TEST_DATA;
	if(dac_write_test == DAC_WRITE_TEST_DATA) status = test_func();

	spr_write(SPR_DAC1, 0);
	spr_write(SPR_DAC2, 0);
	spr_write(SPR_DBCR2, 0);
	return status;
}

#define DVC_READ_TEST_DATA 0xD7C8EAD1
volatile uint32_t dvc_read_test = DVC_READ_TEST_DATA;

int check_trace_dvc_read()
{
	volatile uint32_t dvc_test;
	int status = 1;

	rumboot_printf("DVC read test started!\n");
    debug_flag = TEST_DATA_DEBUG;

    //according to User's Manual ch. 8.5.7 Setup order for IACs, DACs, and DVCs
    spr_write(SPR_DBCR0, 0);
    spr_write(SPR_DBSR_RC, 0xFFFFFFFF);

    spr_write(SPR_IAC1, 0);
	spr_write(SPR_IAC2, 0);
	spr_write(SPR_IAC3, 0);
	spr_write(SPR_IAC4, 0);
	spr_write(SPR_DAC1, &dvc_read_test);
	spr_write(SPR_DAC2, &dvc_read_test);
	spr_write(SPR_DVC1, DVC_READ_TEST_DATA);
	spr_write(SPR_DVC2, ~DVC_READ_TEST_DATA);

    spr_write(SPR_DBCR1, 0);

    spr_write(SPR_DBCR2, (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC1US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC1ER_i)
                       | (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC2US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC2ER_i)
                       | (DEBUG_DBCR2_DACxxM_Exact_match << DEBUG_DBCR2_DAC12M_i)
                       | (DEBUG_DBCR2_DVCxM_AND << DEBUG_DBCR2_DVC1M_i)
                       | (DEBUG_DBCR2_DVCxM_AND << DEBUG_DBCR2_DVC2M_i)
                       | (0b1111 << DEBUG_DBCR2_DVC1BE_i)
                       | (0b1111 << DEBUG_DBCR2_DVC2BE_i));
    isync();

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (1 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (1 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_DVC_READ);

	dvc_test = dvc_read_test;
	if(dvc_test == DVC_READ_TEST_DATA) status = test_func();

	spr_write(SPR_DAC1, 0);
	spr_write(SPR_DAC2, 0);
	spr_write(SPR_DVC1, 0);
	spr_write(SPR_DVC2, 0);
	spr_write(SPR_DBCR2, 0);
	return status;
}

#define DVC_WRITE_TEST_DATA 0xD7C3817E
volatile uint32_t dvc_write_test = 0xDEADBEAF;

int check_trace_dvc_write()
{
	int status = 1;

	rumboot_printf("DVC write test started!\n");
    debug_flag = TEST_DATA_DEBUG;

    //according to User's Manual ch. 8.5.7 Setup order for IACs, DACs, and DVCs
    spr_write(SPR_DBCR0, 0);
    spr_write(SPR_DBSR_RC, 0xFFFFFFFF);

    spr_write(SPR_IAC1, 0);
	spr_write(SPR_IAC2, 0);
	spr_write(SPR_IAC3, 0);
	spr_write(SPR_IAC4, 0);
	spr_write(SPR_DAC1, &dvc_write_test);
	spr_write(SPR_DAC2, &dvc_write_test);
	spr_write(SPR_DVC1, ~DVC_WRITE_TEST_DATA);
	spr_write(SPR_DVC2, DVC_WRITE_TEST_DATA);

    spr_write(SPR_DBCR1, 0);

    spr_write(SPR_DBCR2, (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC1US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC1ER_i)
                       | (DEBUG_DBCR2_DACxUS_Both << DEBUG_DBCR2_DAC2US_i)
                       | (DEBUG_DBCR2_DACxER_E_DS_Both << DEBUG_DBCR2_DAC2ER_i)
                       | (DEBUG_DBCR2_DACxxM_Exact_match << DEBUG_DBCR2_DAC12M_i)
                       | (DEBUG_DBCR2_DVCxM_AND << DEBUG_DBCR2_DVC1M_i)
                       | (DEBUG_DBCR2_DVCxM_AND << DEBUG_DBCR2_DVC2M_i)
                       | (0b1111 << DEBUG_DBCR2_DVC1BE_i)
                       | (0b1111 << DEBUG_DBCR2_DVC2BE_i));
    isync();

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (1 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (1 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_DVC_WRITE);

	dvc_write_test = DVC_WRITE_TEST_DATA;
	if(dvc_write_test == DVC_WRITE_TEST_DATA) status = test_func();

	spr_write(SPR_DAC1, 0);
	spr_write(SPR_DAC2, 0);
	spr_write(SPR_DVC1, 0);
	spr_write(SPR_DVC2, 0);
	spr_write(SPR_DBCR2, 0);
	return status;
}

int check_trace_trap()
{
	rumboot_printf("TRAP test started!\n");
    debug_flag = TEST_DATA_PROGRAM;

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (1 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_TRAP);
    trap();

   	return test_func();
}

int check_trace_irpt()
{
	rumboot_printf("IRPT test started!\n");
    debug_flag = TEST_DATA_SC;

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (1 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_IRPT);
    sc();

   	return test_func();
}

int check_trace_ude()
{
	rumboot_printf("UDE test started!\n");
    debug_flag = TEST_DATA_DEBUG;

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (0 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();

    test_event(TEC_CHECK_TRACE_UDE);
    nop();

   	return test_func();
}

int check_trace_ret()
{
	rumboot_printf("RET test started!\n");
    debug_flag  = TEST_DATA_DEBUG;

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
					   | (1 << DEBUG_DBCR0_IDM_i)
					   | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
					   | (0 << DEBUG_DBCR0_ICMP_i)
					   | (0 << DEBUG_DBCR0_BRT_i)
					   | (0 << DEBUG_DBCR0_IRPT_i)
					   | (0 << DEBUG_DBCR0_TRAP_i)
					   | (0 << DEBUG_DBCR0_IAC1_i)
					   | (0 << DEBUG_DBCR0_IAC2_i)
					   | (0 << DEBUG_DBCR0_IAC3_i)
					   | (0 << DEBUG_DBCR0_IAC4_i)
					   | (0 << DEBUG_DBCR0_DAC1R_i)
					   | (0 << DEBUG_DBCR0_DAC1W_i)
					   | (0 << DEBUG_DBCR0_DAC2R_i)
					   | (0 << DEBUG_DBCR0_DAC2W_i)
					   | (1 << DEBUG_DBCR0_RET_i)
					   | (0 << DEBUG_DBCR0_FT_i));
    isync();

    uint32_t msrval, addr;
    asm volatile (
    	"mfmsr %0 \n\t"
    	"mtspr %2, %0 \n\t"
    	"lis %1, check_trace_exit@h \n\t"
    	"ori %1, %1, check_trace_exit@l\n\t"
    	"mtspr %3, %1 \n\t"
    	:
    		"=r" (msrval),  //0
			"=r" (addr)		//1
    	:
			"i" (SPR_SRR1), //2
			"i" (SPR_SRR0)  //3
		:	"memory"
    );

    test_event(TEC_CHECK_TRACE_RET);

    asm volatile (
    	"rfi \n\t"
		"check_trace_exit: \n\t"
    );

	return test_func();
}

int check_trace_bt()
{
	rumboot_printf("BT test started!\n");
    debug_flag  = TEST_DATA_DEBUG;

	test_event(TEC_CHECK_TRACE_BT);

    trace_bt_func();
    trace_bt_func();
    trace1_bt_func1();
    trace1_bt_func1();

    if(!test_func()) return 1;

    debug_flag  = TEST_DATA_DEBUG;

    test_event(TEC_CHECK_TRACE_BT);

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
					   | (1 << DEBUG_DBCR0_IDM_i)
					   | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
					   | (0 << DEBUG_DBCR0_ICMP_i)
					   | (1 << DEBUG_DBCR0_BRT_i)
					   | (0 << DEBUG_DBCR0_IRPT_i)
					   | (0 << DEBUG_DBCR0_TRAP_i)
					   | (0 << DEBUG_DBCR0_IAC1_i)
					   | (0 << DEBUG_DBCR0_IAC2_i)
					   | (0 << DEBUG_DBCR0_IAC3_i)
					   | (0 << DEBUG_DBCR0_IAC4_i)
					   | (0 << DEBUG_DBCR0_DAC1R_i)
					   | (0 << DEBUG_DBCR0_DAC1W_i)
					   | (0 << DEBUG_DBCR0_DAC2R_i)
					   | (0 << DEBUG_DBCR0_DAC2W_i)
					   | (0 << DEBUG_DBCR0_RET_i)
					   | (0 << DEBUG_DBCR0_FT_i));
    isync();
    nop();
    nop();

    trace_bt_func();
    trace1_bt_func1();

   	return test_func();
}

int check_trace_icmp()
{

	rumboot_printf("ICMP test started!\n");
	debug_flag  = TEST_DATA_DEBUG;

    test_event(TEC_CHECK_TRACE_ICMP);

    spr_write(SPR_DBCR0, (0 << DEBUG_DBCR0_EDM_i)
                       | (1 << DEBUG_DBCR0_IDM_i)
                       | (DEBUG_RESET_No << DEBUG_DBCR0_RST_i)
                       | (1 << DEBUG_DBCR0_ICMP_i)
                       | (0 << DEBUG_DBCR0_BRT_i)
                       | (0 << DEBUG_DBCR0_IRPT_i)
                       | (0 << DEBUG_DBCR0_TRAP_i)
                       | (0 << DEBUG_DBCR0_IAC1_i)
                       | (0 << DEBUG_DBCR0_IAC2_i)
                       | (0 << DEBUG_DBCR0_IAC3_i)
                       | (0 << DEBUG_DBCR0_IAC4_i)
                       | (0 << DEBUG_DBCR0_DAC1R_i)
                       | (0 << DEBUG_DBCR0_DAC1W_i)
                       | (0 << DEBUG_DBCR0_DAC2R_i)
                       | (0 << DEBUG_DBCR0_DAC2W_i)
                       | (0 << DEBUG_DBCR0_RET_i)
                       | (0 << DEBUG_DBCR0_FT_i));
    isync();
    nop();

	return test_func();
}

int main()
{
	test_event_send_test_id("test_oi10_sys_010");

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_platform_request_file("SBIN", SSRAM_BASE);

    static const tlb_entry tlb_entries[] =
       {
        {TLB_ENTRY_INV},
        {TLB_ENTRY_CACHE}
       };

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(tlb_entries, ARRAY_SIZE(tlb_entries));
    rumboot_printf("Done.\n");

    /* Set our own handler */
    rumboot_irq_set_exception_handler(my_exception_handler);

    uint32_t const msr_old_value = msr_read();
	msr_write(1 << ITRPT_XSR_DE_i); /* MSR[DE] - Debug interrupt. */

	if(check_trace_configured()){
		rumboot_printf("Trace configuration failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_iac()){
		rumboot_printf("IAC check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_dac_read()){
		rumboot_printf("DAC read check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_dac_write()){
		rumboot_printf("DAC write check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_dvc_read()){
		rumboot_printf("DVC read check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_dvc_write()){
		rumboot_printf("DVC write check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_trap()){
		rumboot_printf("TRAP check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_irpt()){
		rumboot_printf("IRPT check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_ude()){
		rumboot_printf("UDE check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_ret()){
		rumboot_printf("RET check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_bt()){
		rumboot_printf("BT check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}
	if(check_trace_icmp()){
		rumboot_printf("ICMP check failed!\n");
		msr_write(msr_old_value);
		return 1;
	}

	msr_write(msr_old_value);

	rumboot_printf("TEST OK\n");
    return 0;
}
