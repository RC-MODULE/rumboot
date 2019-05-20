#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/ppc/arch/ppc_476fp_config.h>
#include <arch/ppc/arch/ppc_476fp_lib_c.h>

#include <devices/sp805.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>

#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>

#include <regs/regs_sp805.h>


const uint32_t EVENT_OI10_NRST_PON  = TEST_EVENT_CODE_MIN + 1;
const uint32_t EVENT_OI10_CORERESET = TEST_EVENT_CODE_MIN + 2;
const uint32_t EVENT_OI10_CHIPRESET = TEST_EVENT_CODE_MIN + 3;
const uint32_t EVENT_OI10_SYSRESET  = TEST_EVENT_CODE_MIN + 4;
const uint32_t EVENT_OI10_WD        = TEST_EVENT_CODE_MIN + 5;
const uint32_t EVENT_OI10_CHECK     = TEST_EVENT_CODE_MIN + 6;

const uint32_t CRG_REG_WRITE_ENABLE = 0x1ACCE551;

enum TEST_CRG_STATE
{
    TEST_CRG_STATE_NRST_PON = 1,
    TEST_CRG_STATE_CORERESET,
    TEST_CRG_STATE_CHIPRESET,
    TEST_CRG_STATE_SYSRESET,
    TEST_CRG_STATE_WD
};


static volatile uint32_t wd_int_occured = 0;

static struct sp805_conf wd_conf = { .mode              = FREERUN,
                                     .interrupt_enable  = 0x1,
                                     .clock_division    = 0x1,
                                     .width             = 0x20,
                                     .load              = 0x1000
                                   };


static void irpt_handler(int irq, void *arg )
{
    switch (irq) {

        case CRG_INT:

            rumboot_putstring("CRG irq handler invoked.");

            if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_NRST_PON)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");
            }
            else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_CORERESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                //TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 10)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_CHIPRESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 0)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_SYSRESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 1)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_WD)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 2)) != 0x00, "Value in RST_MON register is incorrect!");
            }

            break;

        case WDT_INT:

            rumboot_putstring("Watchdog irq handler invoked.");
            //sp805_clrint(DCR_WATCHDOG_BASE);
            //dcr_write(DCR_WATCHDOG_BASE + WD_REG_CONTROL, dcr_read(DCR_WATCHDOG_BASE + WD_REG_CONTROL) & ~WD_CTRL_RESEN);
            //sp805_stop(DCR_WATCHDOG_BASE);

            wd_int_occured++;

            break;

        default:
            break;
    }
}



int main(void)
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, CRG_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irpt_handler, 0 );
    rumboot_irq_set_handler( tbl, WDT_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irpt_handler, 0 );

    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( CRG_INT );
    rumboot_irq_enable( WDT_INT );

    rumboot_irq_sei();

    //Enabling CRG interrupt
    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, CRG_REG_WRITE_ENABLE);
    dcr_write(DCR_CRG_BASE + CRG_SYS_INTMASK, 0x01);
    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, ~CRG_REG_WRITE_ENABLE);


    if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_NRST_PON)
        goto label_NRST_PON;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_CORERESET)
        goto label_CORERESET;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_CHIPRESET)
        goto label_CHIPRESET;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_SYSRESET)
        goto label_SYSRESET;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_WD)
        goto label_WD;

    test_event_send_test_id("test_oi10_ctrl_002_3");



//    rumboot_putstring("EVENT_OI10_NRST_PON asserted!\n");
//    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_NRST_PON;
//    rumboot_platform_perf("reset_system by NRST_PON");
//    test_event(EVENT_OI10_NRST_PON);
//    udelay(10);

label_NRST_PON:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating CORERESET reset signal ...\n");
    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_CORERESET;

    rumboot_platform_perf("reset_system by CORERESET");
    test_event(EVENT_OI10_CORERESET);
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x1 << IBM_BIT_INDEX(64, 35)));
    udelay(10);

label_CORERESET:
    test_event(EVENT_OI10_CHECK);
    //TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating CHIPRESET reset signal ...\n");
    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_CHIPRESET;
    rumboot_platform_perf("reset_system by CHIPRESET");
    test_event(EVENT_OI10_CHIPRESET);
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x2 << IBM_BIT_INDEX(64, 35)));
    udelay(10);

label_CHIPRESET:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 0)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating SYSRESET reset signal ...\n");
    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_SYSRESET;
    rumboot_platform_perf("reset_system by SYSRESET");
    test_event(EVENT_OI10_SYSRESET);
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x3 << IBM_BIT_INDEX(64, 35)));
    udelay(10);

label_SYSRESET:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 1)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_putstring("Generating WD_RESET reset signal ...\n");
    rumboot_platform_runtime.persistent[0] = TEST_CRG_STATE_WD;
    rumboot_platform_perf("reset_system by WD");
    sp805_unlock_access(DCR_WATCHDOG_BASE);
    if (dcr_read(DCR_WATCHDOG_BASE + WD_REG_LOCK) != 0x00)
    {
        rumboot_putstring("Error: watchdog registers write unlock failed!\n");
        return 1;
    }
    sp805_clrint(DCR_WATCHDOG_BASE);
    sp805_config(DCR_WATCHDOG_BASE, &wd_conf);
    dcr_write(DCR_WATCHDOG_BASE + WD_REG_CONTROL, dcr_read(DCR_WATCHDOG_BASE + WD_REG_CONTROL) | WD_CTRL_RESEN);
    sp805_enable(DCR_WATCHDOG_BASE);

    rumboot_putstring("Waiting for watchdog interrupt ... ");
    while (wd_int_occured != 2)
        ;

    test_event(EVENT_OI10_WD);
    udelay(10);

label_WD:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 2)) != 0x00 , "Value in RST_MON register is incorrect!");


    udelay(10);
    return 0;
}
