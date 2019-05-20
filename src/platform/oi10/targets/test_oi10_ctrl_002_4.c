#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>

#include <arch/ppc/arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>


const uint32_t EVENT_OI10_NRST_PON      = TEST_EVENT_CODE_MIN + 1;
const uint32_t EVENT_OI10_CHECK_DEFAULT = TEST_EVENT_CODE_MIN + 2;
const uint32_t EVENT_OI10_CHECK_OSC     = TEST_EVENT_CODE_MIN + 3;

const uint32_t CRG_REG_WRITE_ENABLE = 0x1ACCE551;

enum TEST_CRG_STATE
{
    TEST_CRG_STATE_NRST_PON = 1,
    TEST_CRG_STATE_PLL_USE_OSC,
};


static void handler_crg(int irq, void *arg )
{
    rumboot_putstring("CRG irq handler invoked.");
//
//    if (!NRST_POR)
//    {
//        TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
//        TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");
//
//        test_event(EVENT_OI10_CRG_003_CHECK);
//
//        NRST_POR = 1;
//    }
//    else if (!NRST_SYS)
//    {
//        TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_INTCLR) & (1 << 0)) != 0x00 , "Value in INTCLR register is incorrect!");
//        TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 10)) != 0x00, "Value in RST_MON register is incorrect!");
//
//        test_event(EVENT_OI10_CRG_003_CHECK);
//
//        NRST_SYS = 1;
//    }
}


int main(void)
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, CRG_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_crg, 0 );

    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( CRG_INT );

    rumboot_irq_sei();

    //Initial settings

    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, CRG_REG_WRITE_ENABLE);

    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_PRDIV, 0x01);
    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_FBDIV, 0x20);
    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_PSDIV, 0x04);

    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, ~CRG_REG_WRITE_ENABLE);


    if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_NRST_PON)
        goto label_NRST_PON;
    else if (rumboot_platform_runtime.persistent[0] == TEST_CRG_STATE_PLL_USE_OSC)
        goto label_PLL_USE_OSC;

    test_event_send_test_id("test_oi10_ctrl_002_4");


//    while ((dcr_read(DCR_CRG_BASE + CRG_SYS_PLL_STAT) & (1 << 0)) != (1 << 0))
//        ;

    //Enabling CRG interrupt
//    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, CRG_REG_WRITE_ENABLE);
//    dcr_write(DCR_CRG_BASE + CRG_SYS_INTMASK, 0x01);
//    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, ~CRG_REG_WRITE_ENABLE);


//    test_event(EVENT_OI10_NRST_PON);

label_NRST_PON:
    test_event(EVENT_OI10_CHECK_DEFAULT);
    TEST_ASSERT((dcr_read(DCR_CRG_BASE + CRG_SYS_RST_MON) & (1 << 11)) != 0x00 , "Value in RST_MON register is incorrect!");


    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, CRG_REG_WRITE_ENABLE);

    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_CTRL, 0x03);

    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_PRDIV, 0x08);
    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_FBDIV, 0x40);
    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_PSDIV, 0x02);

    dcr_write(DCR_CRG_BASE + CRG_SYS_PLL_CTRL, 0x00);

    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, ~CRG_REG_WRITE_ENABLE);

    udelay(100);


label_PLL_USE_OSC:
    test_event(EVENT_OI10_CHECK_OSC);


    udelay(10);
    return 0;
}
