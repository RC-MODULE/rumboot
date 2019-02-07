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
#include <rumboot/irq.h>

#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>


typedef enum {
    RESET_REQ_TYPE_NONE =  0b00,
    RESET_REQ_TYPE_CORE =  0b01,
    RESET_REQ_TYPE_CHIP =  0b10,
    RESET_REQ_TYPE_SYSTEM = 0b11,
    RESET_REQ_TYPE_UNKNOWN = 0xF14158ED,

} RST_REQ_TYPE;

enum {
       TEC_CHECK_DEBUG_MACHINECHECK = TEST_EVENT_CODE_MIN,
} test_event_code;

enum {
    TEST_DATA_UNDEF     = 0x8E591794
} test_data;


#define EVENT_RESET_CORE                        TEST_EVENT_CODE_MIN + 0
#define EVENT_RESET_CHIP                        TEST_EVENT_CODE_MIN + 1
#define EVENT_RESET_SYSTEM                      TEST_EVENT_CODE_MIN + 2
#define EVENT_FINISHED                          TEST_EVENT_CODE_MIN + 3

static volatile bool MC_HANDLED;


enum {
    NXT_RESET_REQ_TYPE = 1,
};

void update_reset_req_type ()
{
    rumboot_printf("Update reset type\n");
    switch(rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE])
    {
    case RESET_REQ_TYPE_NONE:
        rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE] = RESET_REQ_TYPE_CORE;
        rumboot_printf("Initial configuration\n");
        test_event(EVENT_RESET_CORE);
        break;
    case RESET_REQ_TYPE_CORE:
        rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE] = RESET_REQ_TYPE_CHIP;
        test_event(EVENT_RESET_CHIP);
        break;
    case RESET_REQ_TYPE_CHIP:
        rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE] = RESET_REQ_TYPE_SYSTEM;
        test_event(EVENT_RESET_SYSTEM);
        break;
    case RESET_REQ_TYPE_SYSTEM:
        rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE] = RESET_REQ_TYPE_UNKNOWN;
        break;
    case RESET_REQ_TYPE_UNKNOWN:
         break;
    }
    rumboot_printf("Next reset type = %x\n", rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE]);
}

static void reset_system (RST_REQ_TYPE dbcr_rst_type)
{
    rumboot_platform_perf("reset_system");
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
        spr_write(SPR_MCSR_C,0xFFFFFFFF);

        reset_system(rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE]);
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


    return 1;
}

int main ()
{
    test_event_send_test_id( "test_oi10_cpu_025_rst");

    MC_HANDLED = false;
    update_reset_req_type ();
    if((rumboot_platform_runtime_info->persistent[NXT_RESET_REQ_TYPE] == RESET_REQ_TYPE_UNKNOWN))
    {
        test_event(EVENT_FINISHED);
        return 0;
    }

    check_machinecheck ();


   return 1;
}

