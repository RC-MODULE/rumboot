#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/irq.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>
#include <rumboot/rumboot.h>

#define  SRAM0_ADDR_OK_FUNC     SRAM0_BASE
#define  SRAM1_ADDR_TEST_FUNC   SRAM1_BASE

#define  OK_FUNC_SIZE           0x20
#define  TEST_FUNC_SIZE         0x64

typedef uint32_t func();

//                               MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_SRAM0_NOCACHE  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_CACHE    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

uint32_t test_func (void) __attribute__((aligned(128)));
uint32_t ok_func (void) __attribute__((aligned(128)));

uint32_t test_func (void)
{
    //rewrite test_func->ok_func in sram1
    for (uint32_t offset = 0; offset < OK_FUNC_SIZE; offset += 4)
    {
      iowrite32(ioread32(SRAM0_ADDR_OK_FUNC + offset), SRAM1_ADDR_TEST_FUNC + offset);
      dcbst((void*)SRAM1_ADDR_TEST_FUNC);
      msync();
    }
    //invalidate i-cache
    for (uint32_t offset = 0; offset < OK_FUNC_SIZE; offset += 4)
    {
      icbi((void*)SRAM1_ADDR_TEST_FUNC);
      isync();
    }
    return 1;
}

uint32_t ok_func (void)
{
    return 2;
}

int main()
{
    emi_init(DCR_EM2_EMI_BASE);

    //init data
    rumboot_printf("init data\n");
    memset((void*)SRAM0_ADDR_OK_FUNC, 0x00, 128);
    memset((void*)SRAM1_ADDR_TEST_FUNC, 0x00, 128);

    //copy ok func in sram0
    for (uint32_t offset = 0; offset < OK_FUNC_SIZE; offset += 4)
      iowrite32(ioread32(((uint32_t)ok_func) + offset), SRAM0_ADDR_OK_FUNC + offset);
    //copy test_func in sram1
    for (uint32_t offset = 0; offset < TEST_FUNC_SIZE; offset += 4)
      iowrite32(ioread32(((uint32_t)test_func) + offset), SRAM1_ADDR_TEST_FUNC + offset);

    //Set tlb
    rumboot_printf("Set tlb\n");
    static const tlb_entry mem_tlb_entrys[] = {{TLB_ENTRY_SRAM0_NOCACHE}, {TLB_ENTRY_SRAM1_CACHE}};
    write_tlb_entries(mem_tlb_entrys, 2);

    func* f = (func*)(SRAM1_ADDR_TEST_FUNC);
    rumboot_printf("Call test func\n");
    TEST_ASSERT( f() == 1, "invalid return value");
    rumboot_printf("Call test func (modified)\n");
    TEST_ASSERT( f() == 2, "invalid return value");

    rumboot_printf("TEST_OK\n");
    return 0;
}
