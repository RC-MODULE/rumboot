/*
 * test_oi10_cpu_025_rst.c
 *
 *  Created on: Dec 21, 2018
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

BEGIN_ENUM( RST_REQ_TYPE )
DECLARE_ENUM_VAL (RESET_REQ_TYPE_NONE,   0b00)
DECLARE_ENUM_VAL (RESET_REQ_TYPE_CORE,   0b01)
DECLARE_ENUM_VAL (RESET_REQ_TYPE_CHIP,   0b10)
DECLARE_ENUM_VAL (RESET_REQ_TYPE_SYSTEM, 0b11)
END_ENUM( RST_REQ_TYPE );


enum {
       TEC_CHECK_DEBUG_MACHINECHECK = TEST_EVENT_CODE_MIN,
} test_event_code;

enum {
    TEST_DATA_UNDEF     = 0x8E591794
} test_data;


#define EVENT_RESET                        TEST_EVENT_CODE_MIN + 0
#define EVENT_FINISHED                     TEST_EVENT_CODE_MIN + 1

static volatile uint32_t RESET_REQ_TYPE __attribute__((section(".data"))) = RESET_REQ_TYPE_CORE;
static volatile uint32_t NXT_RESET_REQ_TYPE __attribute__((section(".data"))) = RESET_REQ_TYPE_CORE;
static volatile bool MC_HANDLED;

//enum {
//DECLARE_ENUM_VAL( MCSR_L2_e,         42 )
//DECLARE_ENUM_VAL( MCSR_L2_n,         1 )
//DECLARE_ENUM_VAL( MCSR_L2_i,         IBM_BIT_INDEX( 64, MCSR_L2_e ) )
//} MCSR_FIELD;

volatile static uint32_t mc_cnt = 0;

void update_reset_req_type ()
{
    rumboot_printf("Update reset type\n");
    switch(RESET_REQ_TYPE)
    {
    case RESET_REQ_TYPE_CORE:
        NXT_RESET_REQ_TYPE = RESET_REQ_TYPE_CHIP;
        break;
    case RESET_REQ_TYPE_CHIP:
        NXT_RESET_REQ_TYPE = RESET_REQ_TYPE_SYSTEM;
        break;
    case RESET_REQ_TYPE_SYSTEM:
        NXT_RESET_REQ_TYPE = RESET_REQ_TYPE_NONE;
        break;
    case RESET_REQ_TYPE_NONE:
        break;
    }
    rumboot_printf("Curent reset type = %x\n", RESET_REQ_TYPE);
    rumboot_printf("Next reset type = %x\n", NXT_RESET_REQ_TYPE);
//    trace_msg("Curent reset type = ");
//    trace_hex(RESET_REQ_TYPE);
//    trace_msg("Next reset type = ");
//    trace_hex(NXT_RESET_REQ_TYPE);
}

static void reset_system (RST_REQ_TYPE dbcr_rst_type)
{
    uint32_t dbcr0 =  spr_read (SPR_DBCR0);
    CLEAR_BITS_BY_MASK(dbcr0, 0b11 << DEBUG_DBCR0_RST_i);
    spr_write(SPR_DBCR0, SET_BITS_BY_MASK(dbcr0, dbcr_rst_type << DEBUG_DBCR0_RST_i));
}

static void exception_handler(int id, const char *name)
{
    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
    if(id == RUMBOOT_IRQ_MACHINE_CHECK)
    {
        rumboot_printf("It is OK!\n");
        test_event(EVENT_RESET);
        update_reset_req_type ();
        reset_system(RESET_REQ_TYPE);
	    mc_cnt += 1;
	    spr_write(SPR_MCSR_C,0xFFFFFFFF);
    }
    else
    {
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
    }
}

long for_test = TEST_DATA_UNDEF;

int check_machinecheck ()
{
	uint32_t const msr_old_value = msr_read();
	uint32_t const ccr1_old_value = spr_read(SPR_CCR1);
	uint32_t addr = (uint32_t)&for_test;

	rumboot_printf("DEBUG_MACHINECHECK check start!\n");

	spr_write(SPR_MCSR_C,0xFFFFFFFF);
	msr_write(msr_old_value | (0b1 << ITRPT_XSR_FP_i)   /* MSR[FP] - Floating point available. */
                            | (0b1 << ITRPT_XSR_ME_i)); /* MSR[ME] - Machine check enable.     */

	/* Set our own handler */
    rumboot_irq_set_exception_handler(exception_handler);

	rumboot_printf("send TEC_CHECK_DEBUG_MACHINECHECK\n");
	//test_event( );

	//Check C470S_DBGMACHINECHECK via ESR[ISMC]
	spr_write(SPR_ESR,0x80000000);
	spr_write(SPR_ESR,0x00000000);
	spr_write(SPR_MCSR_C,0xFFFFFFFF);

    //Other check C470S_DBGMACHINECHECK
	asm volatile (
			"lfs  1, 0(%0)\n\t"
			"lfs  2, 0(%0)\n\t"
			::"r"(addr)
			 : "memory"
	);
	msync();

	spr_write( SPR_CCR1, ccr1_old_value | (0b11 << CTRL_CCR1_FPRPEI_i)); //Floating-Point Register (FPR) parity error insert.
	isync();

	asm volatile (
			"lfs  1, 0(%0)\n\t"
			"lfs  2, 0(%0)\n\t"
			::"r"(addr)
			: "memory"
	);
	msync();

	spr_write( SPR_CCR1, ccr1_old_value);
	isync();

	asm volatile ("fcmpu 0, 1, 2\n\t");

	spr_write(SPR_MCSR_C,0xFFFFFFFF);
	msr_write(msr_old_value);

	if(mc_cnt >= 1)
	{


	    return 1;
	}

	rumboot_printf("ERROR! The reset did not happen! \n");

	return 0;
}






int main ()
{
    test_event_send_test_id( "test_oi10_cpu_025_rst");

    rumboot_printf("TEST START\n");

    MC_HANDLED = false;
    RESET_REQ_TYPE = NXT_RESET_REQ_TYPE;
    if(RESET_REQ_TYPE == RESET_REQ_TYPE_NONE)
    {
        rumboot_printf("Reset sequence done\n");
        return 0;
    }
    check_machinecheck ();



    rumboot_printf("TEST ERROR\n");
    return 0;
}

