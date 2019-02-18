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

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_power_modes.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>
#include <platform/devices/dma2plb6.h>

BEGIN_ENUM( SYNC_INSTR )
DECLARE_ENUM_VAL( s_nop,    0x00 )
DECLARE_ENUM_VAL( s_msync,  0x01 )
DECLARE_ENUM_VAL( s_lwsync, 0x02 )
DECLARE_ENUM_VAL( s_mbar,   0x03 )
END_ENUM( SYNC_INSTR )

void check_block (SYNC_INSTR sync_instr)
{
    uint32_t test_data = 0xBABADEDA;

    uint32_t addr1 = SRAM0_BASE,
             addr2 = SRAM0_BASE + SRAM0_SIZE; //this address corresponds by EMI to the addr1 (phys_addr1 == phys_addr2)

    uint32_t rdata1 = 0,
             rdata2 = 0;

    dci(0);
    memset((void*)SRAM0_BASE, 0x00, 0x80);

    if (sync_instr == s_nop) {
        iowrite32 (0x00, addr1);
        iowrite32 (test_data, addr2);
        rdata1 = ioread32(addr1);
        rdata2 = ioread32(addr2);
    }
    else {
        iowrite32 (0x00, addr1);
        iowrite32 (test_data, addr2);
        switch (sync_instr) {
            case s_msync:  msync();  break;
            case s_lwsync: lwsync(); break;
            case s_mbar:   mbar();   break;
            case s_nop:    break;
        }
        rdata1 = ioread32(addr1);
        rdata2 = ioread32(addr2);
    }
    rumboot_printf("if the transactions is completed, the data will be == %x\n", test_data);
    rumboot_printf("rdata1 = %x\n", rdata1);
    rumboot_printf("rdata2 = %x\n", rdata2);
    if (rdata1 != rdata2)
        rumboot_printf("transactions not completed\n");
    else
        rumboot_printf("transactions completed\n");

    if (sync_instr != s_nop)
        TEST_ASSERT (rdata1 == rdata2, "ERROR: not expected read data (expect rdata1 == rdata2)");
    else
        TEST_ASSERT (rdata1 != rdata2, "ERROR: not expected read data (expect rdata1 != rdata2)");
}

void check_sync_instr (SYNC_INSTR sync_instr)
{
    rumboot_printf("write/read without sync instruction\n");
    check_block (s_nop);
    rumboot_printf("write/read with sync instruction\n");
    check_block (sync_instr);
    rumboot_printf("\n");
}

int main()
{
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Check msync:\n");
    check_sync_instr (s_msync);

    rumboot_printf("Check lwsync:\n");
    check_sync_instr (s_lwsync);

    rumboot_printf("Check mbar:\n");
    check_sync_instr (s_mbar);

    rumboot_printf("TEST OK\n");
    return 0;
}
