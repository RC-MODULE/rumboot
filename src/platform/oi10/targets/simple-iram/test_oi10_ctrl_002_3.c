#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <devices/sp805.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>

#include <regs/regs_sp805.h>


const uint32_t EVENT_OI10_NRST_POR  = TEST_EVENT_CODE_MIN + 1;
const uint32_t EVENT_OI10_CORERESET = TEST_EVENT_CODE_MIN + 2;
const uint32_t EVENT_OI10_CHIPRESET = TEST_EVENT_CODE_MIN + 3;
const uint32_t EVENT_OI10_SYSRESET  = TEST_EVENT_CODE_MIN + 4;
const uint32_t EVENT_OI10_WD        = TEST_EVENT_CODE_MIN + 5;
const uint32_t EVENT_OI10_CHECK     = TEST_EVENT_CODE_MIN + 6;

const uint32_t CRG_REG_WRITE_ENABLE = 0x1ACCE551;

enum TEST_CRG_STATE
{
    TEST_CRG_STATE_NRST_POR = 1,
    TEST_CRG_STATE_CORERESET,
    TEST_CRG_STATE_CHIPRESET,
    TEST_CRG_STATE_SYSRESET,
    TEST_CRG_STATE_WD
};

volatile enum TEST_CRG_STATE state = 0;

static volatile uint32_t wd_int_occured = 0;


static void irpt_handler(int irq, void *arg )
{
    switch (irq) {

        case CRG_INT:

            rumboot_putstring("CRG irq handler invoked.");

            if (state == TEST_CRG_STATE_NRST_POR)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");
            }
            else if (state == TEST_CRG_STATE_CORERESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                //TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 10)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (state == TEST_CRG_STATE_CHIPRESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 0)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (state == TEST_CRG_STATE_SYSRESET)
            {
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
                TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 1)) != 0x00, "Value in RST_MON register is incorrect!");
            }
            else if (state == TEST_CRG_STATE_WD)
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
    test_event_send_test_id("test_oi10_ctrl_002_3");

    static struct sp805_conf wd_conf = { .mode              = FREERUN,
                                         .interrupt_enable  = 0x1,
                                         .clock_division    = 0x1,
                                         .width             = 0x20,
                                         .load              = 0x1000
                                       };

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

    if (state == TEST_CRG_STATE_NRST_POR)
        goto label_NRST_POR;
    else if (state == TEST_CRG_STATE_CORERESET)
        goto label_CORERESET;
    else if (state == TEST_CRG_STATE_CHIPRESET)
        goto label_CHIPRESET;
    else if (state == TEST_CRG_STATE_SYSRESET)
        goto label_SYSRESET;
    else if (state == TEST_CRG_STATE_WD)
        goto label_WD;

goto l_test;

    rumboot_putstring("EVENT_OI10_NRST_POR asserted!\n");
    state = TEST_CRG_STATE_NRST_POR;
    test_event(EVENT_OI10_NRST_POR);
    udelay(20);

label_NRST_POR:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    state = TEST_CRG_STATE_CORERESET;
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x1 << IBM_BIT_INDEX(64, 35)));
    test_event(EVENT_OI10_CORERESET);

label_CORERESET:
    test_event(EVENT_OI10_CHECK);
    //TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    state = TEST_CRG_STATE_CHIPRESET;
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x2 << IBM_BIT_INDEX(64, 35)));
    test_event(EVENT_OI10_CHIPRESET);

label_CHIPRESET:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 0)) != 0x00 , "Value in RST_MON register is incorrect!");


    state = TEST_CRG_STATE_SYSRESET;
    spr_write(SPR_DBCR0, spr_read(SPR_DBCR0) | (0x3 << IBM_BIT_INDEX(64, 35)));
    test_event(EVENT_OI10_SYSRESET);

label_SYSRESET:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 1)) != 0x00 , "Value in RST_MON register is incorrect!");

l_test:
    state = TEST_CRG_STATE_WD;

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
    while (wd_int_occured == 2)
        ;

    test_event(EVENT_OI10_WD);

label_WD:
    test_event(EVENT_OI10_CHECK);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 2)) != 0x00 , "Value in RST_MON register is incorrect!");


    rumboot_printf("TEST_OK\n");
    return 0;
}
