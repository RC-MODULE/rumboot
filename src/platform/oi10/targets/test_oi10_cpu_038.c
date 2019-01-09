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
#include <platform/devices/emi.h>

typedef uint8_t func();

#ifdef TEST_OI10_CPU_038_IM0
                       //           MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
/* TLB entries to remap 128KB pages IM0 from 0x00000010_80000000-0x00000010_8001FFFF to 0x80020000-0x8003FFFF*/
#define TLB_ENTRY_IM0_NEW_VADDR0   {MMU_TLB_ENTRY(  0x010,  0x80000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )}
#define TLB_ENTRY_IM0_NEW_VADDR1   {MMU_TLB_ENTRY(  0x010,  0x80010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )}
/* TLB entries to remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0x80000000-0x8001FFFF*/
#define TLB_ENTRY_IM1_NEW_VADDR0   {MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )}
#define TLB_ENTRY_IM1_NEW_VADDR1   {MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )}

static const tlb_entry im_switch_tlb_entries[] =   {TLB_ENTRY_IM0_NEW_VADDR0,
                                                    TLB_ENTRY_IM0_NEW_VADDR1,
                                                    TLB_ENTRY_IM1_NEW_VADDR0,
                                                    TLB_ENTRY_IM1_NEW_VADDR1};
#endif

int main(void)
{
    uint32_t result = 1;

    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00);

#ifdef TEST_OI10_CPU_038_IM0
    //copy IM0 to IM1
    memcpy((void *)IM1_BASE,(void *)IM0_BASE,0x20000);
    rumboot_putstring("copy IM0 to IM1 finished");
    //switch physical addresses IM0<->IM1
    write_tlb_entries(im_switch_tlb_entries,ARRAY_SIZE(im_switch_tlb_entries));

    rumboot_printf("TLB entries switching finished\n");
#endif

#ifdef EMI_INIT
    emi_init(DCR_EM2_EMI_BASE);
#endif
    rumboot_platform_request_file("MBIN", M_BASE);

    rumboot_printf("Starting test code from another memory\n");
    func* f = (func*)(M_BASE);
    result = f();

    return result;
}
