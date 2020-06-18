#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/ppc_476fp_lib_c.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>

#include <platform/arch/ppc/test_macro.h>
#include <arch/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>


const uint32_t EVENT_OI10_NRST_PON  = TEST_EVENT_CODE_MIN + 1;
const uint32_t EVENT_OI10_NRST_SYS  = TEST_EVENT_CODE_MIN + 2;
const uint32_t EVENT_OI10_CHECK     = TEST_EVENT_CODE_MIN + 3;


enum TEST_CRG_STATE
{
    TEST_CRG_STATE_NRST_PON = 1,
    TEST_CRG_STATE_NRST_SYS
};



int main(void)
{
    if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_NRST_PON)
        goto label_NRST_PON;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_NRST_SYS)
        goto label_NRST_SYS;


    test_event_send_test_id("test_oi10_ctrl_002_2");

//    rumboot_putstring("Generating NRST_PON reset signal ...\n");
//    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_NRST_PON;
//    rumboot_platform_perf("reset_system by NRST_PON");
//    test_event(EVENT_OI10_NRST_PON);
//    udelay(10);

label_NRST_PON:
    test_event(EVENT_OI10_CHECK);
    udelay(1);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating NRST_SYS reset signal ...\n");
    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_NRST_SYS;
//    rumboot_platform_perf("reset_system by NRST_SYS"); // OI10-305
    test_event(EVENT_OI10_NRST_SYS);
    udelay(10);

label_NRST_SYS:
    test_event(EVENT_OI10_CHECK);
    udelay(1);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 10)) != 0x00 , "Value in RST_MON register is incorrect!");


    udelay(10);
    return 0;
}
