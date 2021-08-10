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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
#include <devices/ugly/emi.h>
#include <devices/mpic128.h>
#include <regs/fields/emi.h>
#include <platform/interrupts.h>
#include <arch/dma2plb6.h>

#define TEC_START_CHECK_LWARX_L1_MISS_L2_MISS   TEST_EVENT_CODE_MIN
#define TEC_START_CHECK_LWARX_L1_MISS_L2_HIT    (TEST_EVENT_CODE_MIN + 0x1)
#define TEC_START_CHECK_LWARX_L1_HIT_L2_MISS    (TEST_EVENT_CODE_MIN + 0x2)
#define TEC_START_CHECK_LWARX_L1_HIT_L2_HIT     (TEST_EVENT_CODE_MIN + 0x3)
#define TEC_START_CHECK_STWCX_L1_MISS_L2_MISS   (TEST_EVENT_CODE_MIN + 0x4)
#define TEC_START_CHECK_STWCX_L1_MISS_L2_HIT    (TEST_EVENT_CODE_MIN + 0x5)
#define TEC_START_CHECK_STWCX_L1_HIT_L2_MISS    (TEST_EVENT_CODE_MIN + 0x6)
#define TEC_START_CHECK_STWCX_L1_HIT_L2_HIT     (TEST_EVENT_CODE_MIN + 0x7)
#define TEC_START_CHECK_MBAR                    (TEST_EVENT_CODE_MIN + 0x8)
#define TEC_START_CHECK_MSYNC                   (TEST_EVENT_CODE_MIN + 0x9)
#define TEC_START_CHECK_LWSYNC                  (TEST_EVENT_CODE_MIN + 0xA)

BEGIN_ENUM( SYNC_INSTR )
DECLARE_ENUM_VAL( s_nop,    0x00 )
DECLARE_ENUM_VAL( s_msync,  0x01 )
DECLARE_ENUM_VAL( s_lwsync, 0x02 )
DECLARE_ENUM_VAL( s_mbar,   0x03 )
END_ENUM( SYNC_INSTR )
//                               MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_EM2_CACHE_WT   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define WRITE_SYNCX_READ(X)\
    iowrite32 (0x00, addr1);\
    iowrite32 (test_data, addr2);\
    X;\
    rdata1 = ioread32(addr1);\
    rdata2 = ioread32(addr2);

static void check_block (SYNC_INSTR sync_instr)
{
    uint32_t test_data = 0xBABADEDA;

    uint32_t addr1 = SRAM0_BASE,
             addr2 = SRAM0_BASE + SRAM0_SIZE; //this address corresponds by EMI to the addr1 (phys_addr1 == phys_addr2)

    uint32_t rdata1 = 0,
             rdata2 = 0;

    memset((void*)SRAM0_BASE, 0x00, 0x80);

    switch (sync_instr) {
        case s_nop:
            iowrite32 (0x00, addr1);
            iowrite32 (test_data, addr2);
            rdata1 = ioread32(addr1);
            rdata2 = ioread32(addr2);
            break;
        case s_msync:  test_event(TEC_START_CHECK_MSYNC);
            WRITE_SYNCX_READ(msync());                      //s_nop:* + sync command
            break;
        case s_lwsync: test_event(TEC_START_CHECK_LWSYNC);
            WRITE_SYNCX_READ(lwsync());
            break;
        case s_mbar:   test_event(TEC_START_CHECK_MBAR);
            WRITE_SYNCX_READ(mbar());
            break;
    }

    rumboot_printf("if the transactions is completed, the data will be == %x\n", test_data);
    rumboot_printf("rdata1 = %x\n", rdata1);
    rumboot_printf("rdata2 = %x\n", rdata2);
    if ((rdata1 == test_data) && (rdata2 == test_data))
        rumboot_printf("transactions completed\n");
    else
        rumboot_printf("transactions not completed\n");

    if (sync_instr != s_nop)
        TEST_ASSERT ( ((rdata1 == test_data) && (rdata2 == test_data)), "ERROR: not expected read data");
    else
        TEST_ASSERT ( ((rdata1 != test_data) || (rdata2 == test_data)), "ERROR: not expected read data");
}

static void check_sync_instr (SYNC_INSTR sync_instr)
{
    rumboot_printf("write/read without sync instruction\n");
    check_block (s_nop);
    dci(0); dci(2);
    rumboot_printf("write/read with sync instruction\n");
    check_block (sync_instr);
    dci(0); dci(2);
    rumboot_printf("\n");
}

static inline __attribute__((always_inline))
uint32_t stwcx_(uint32_t wval, uintptr_t wptr) {
    uint32_t rval = 0;
    asm volatile (
            "stwcx. %0, 0, %1       \n\t"
        :
        :   "r"(wval), "r"(wptr)
        :   "memory"
    );
    return rval;
}

static void check_lwarx_stwcx (uint32_t addr, uint32_t event_lwarx, uint32_t event_stwcx)
{
    volatile uint32_t tmp;
    (void) dcread (addr);
    test_event (event_lwarx);
    tmp = lwarx(addr);
    (void) dcread (addr);
    test_event (event_stwcx);
    stwcx_(tmp, addr);
    (void) dcread (addr);
}

int main()
{
    test_event_send_test_id("test_oi10_cpu_020_sync");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Check msync:\n");
    check_sync_instr (s_msync);

    rumboot_printf("Check lwsync:\n");
    check_sync_instr (s_lwsync);

    rumboot_printf("Check mbar:\n");
    check_sync_instr (s_mbar);

    rumboot_printf("Checking lwarx and stwcx.\n");
    uint32_t test_line1 = SRAM0_BASE + 128 * 1;
    uint32_t test_line2 = SRAM0_BASE + 128 * 2;
    uint32_t test_line3 = SRAM0_BASE + 128 * 3;
    uint32_t test_line4 = SRAM0_BASE + 4096;

    memset((void*)SRAM0_BASE, 0x00, 4096 + 128);
    iowrite32(0xAAAABBBB ,test_line1);
    iowrite32(0xBBBBAAAA ,test_line2);
    iowrite32(0xDDDDEEEE ,test_line3);
    iowrite32(0xDDDDAAAA ,test_line4);

    uint32_t addr = 0;

    static const tlb_entry sram0_tlb_entry_wt = {TLB_ENTRY_EM2_CACHE_WT};
    write_tlb_entries(&sram0_tlb_entry_wt,1);

    rumboot_printf("l1 - miss, l2 - miss\n");
    addr = test_line1;
    (void) ioread32(addr);
    dci(0); dci(2);
    check_lwarx_stwcx(addr, TEC_START_CHECK_LWARX_L1_MISS_L2_MISS, TEC_START_CHECK_STWCX_L1_MISS_L2_MISS);

    rumboot_printf("l1 - hit, l2 - hit\n");
    dci(0); dci(2);
    addr = test_line2;
    (void) ioread32(addr);
    check_lwarx_stwcx(addr, TEC_START_CHECK_LWARX_L1_HIT_L2_HIT, TEC_START_CHECK_STWCX_L1_HIT_L2_HIT);

    rumboot_printf("l1 - hit, l2 - miss\n");
    dci(0); dci(2);
    addr = test_line3;
    (void) ioread32(addr);
    msync();
    dci(2);
    check_lwarx_stwcx(addr, TEC_START_CHECK_LWARX_L1_HIT_L2_MISS, TEC_START_CHECK_STWCX_L1_HIT_L2_MISS);

    rumboot_printf("l1 - miss, l2 - hit\n");
    dci(0); dci(2);
    addr = test_line4;
    (void) ioread32(addr);
    dci(0);
    check_lwarx_stwcx(addr, TEC_START_CHECK_LWARX_L1_MISS_L2_HIT, TEC_START_CHECK_STWCX_L1_MISS_L2_HIT);

    rumboot_printf("Check lwarx and stwcx. : OK\n");

    rumboot_printf("TEST OK\n");
    return 0;
}
