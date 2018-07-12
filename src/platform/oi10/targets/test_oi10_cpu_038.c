#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
                       //          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
/* TLB entries to remap 128KB pages IM0 from 0x00000010_80000000-0x00000010_8001FFFF to 0x80020000-0x8003FFFF*/
#define TLB_ENTRY_IM0_NEW_VADDR0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_1,     0b1 )
#define TLB_ENTRY_IM0_NEW_VADDR1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_2,     0b1 )
/* TLB entries to remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0x80000000-0x8001FFFF*/
#define TLB_ENTRY_IM1_NEW_VADDR0   MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_IM1_NEW_VADDR1   MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entries to invalidate mapping 128KB pages IM0 from 0x00000010_80000000-0x00000010_8001FFFF to 0x80000000-0x8001FFFF*/
#define TLB_ENTRY_IM0_INVALIDATE0  MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_1,     0b0 )
#define TLB_ENTRY_IM0_INVALIDATE1  MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_2,     0b0 )
/* TLB entries to remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0x80020000-0x8003FFFF*/
#define TLB_ENTRY_IM1_INVALIDATE0  MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_IM1_INVALIDATE1  MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b0 )

static const tlb_entry im_switch_tlb_entries[] =   {TLB_ENTRY_IM0_INVALIDATE0,
                                                    TLB_ENTRY_IM0_INVALIDATE1,
                                                    TLB_ENTRY_IM1_INVALIDATE0,
                                                    TLB_ENTRY_IM1_INVALIDATE1,
                                                    TLB_ENTRY_IM0_NEW_VADDR0,
                                                    TLB_ENTRY_IM0_NEW_VADDR1,
                                                    TLB_ENTRY_IM1_NEW_VADDR0,
                                                    TLB_ENTRY_IM1_NEW_VADDR1};

int main(void) 
{
#ifdef TEST_OI10_CPU_038_IM0
    //TODO copy IM0 to IM1
    memcpy((void *)IM1_BASE,(void *)IM0_BASE,0x20000);
    rumboot_putstring("copy IM0 to IM1 finished");
    //TODO switch memory cell for eventsystem_memory to IM1
//    rumboot_platform_relocate_runtime(IM1_BASE + 0x1ff00);
//    rumboot_putstring("relocate finished");
    //TODO switch physical addresses IM0<->IM1
    msync();
    isync();
    write_tlb_entries(im_switch_tlb_entries,8);
    msync();
    isync();

    rumboot_putstring("TLB entries switching finished");
    rumboot_printf("printf working");
#endif

    rumboot_platform_request_file("IMBIN", IM_BASE);
    int (*check_function)();
    check_function = (void *)(IM_BASE);
    rumboot_putstring("starting code from IM1\n");

      return check_function();

    return 1;
}
