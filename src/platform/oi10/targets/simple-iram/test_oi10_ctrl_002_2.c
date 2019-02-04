#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>

#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>


const uint32_t EVENT_OI10_NRST_POR  = TEST_EVENT_CODE_MIN + 1;
const uint32_t EVENT_OI10_NRST_SYS  = TEST_EVENT_CODE_MIN + 2;
const uint32_t EVENT_OI10_CHECK     = TEST_EVENT_CODE_MIN + 3;


enum TEST_CRG_STATE
{
    TEST_CRG_STATE_NRST_POR = 1,
    TEST_CRG_STATE_NRST_SYS
};

static volatile enum TEST_CRG_STATE state = 0;

//rumboot_platform_runtime_info ri;


int main(void)
{
    test_event_send_test_id("test_oi10_ctrl_002_2");

    if (state == TEST_CRG_STATE_NRST_POR)
        goto label_NRST_POR;
    else if (state == TEST_CRG_STATE_NRST_SYS)
        goto label_NRST_SYS;


    rumboot_putstring("Generating NRST_PON reset signal ...\n");
    state = TEST_CRG_STATE_NRST_POR;
    test_event(EVENT_OI10_NRST_POR);

label_NRST_POR:
    test_event(EVENT_OI10_CHECK);
    udelay(10);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating NRST_SYS reset signal ...\n");
    state = TEST_CRG_STATE_NRST_SYS;
    test_event(EVENT_OI10_NRST_SYS);

label_NRST_SYS:
    test_event(EVENT_OI10_CHECK);
    udelay(10);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_printf("TEST_OK\n");
    return 0;
}
