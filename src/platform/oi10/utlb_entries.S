#include <platform/oi10/platform/ppc_476fp_mmu_fields.h>
#include <platform/oi10/platform/ppc470s/mmu/mem_window.h>


.section ".init.rodata","a",@progbits
.global tlb_entries
.global TLB_ENTRIES_N

tlb_entries:
//          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
// TLB entries to remap 64KB page BOOTROM mirror from 0x00000010_00000000-0x00000010_0000ffff to 0x00000000-0x0000ffff
//    .long   MMU_TLB_ENTRY(  0x010,  0x00000,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
// TLB entries to remap 64KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00020,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
// TLB entries to remap 64KB pages IM1 from 0x00000020_00000000-0x00000020_0001ffff to 0x00060000-0x0007ffff
    .long   MMU_TLB_ENTRY(  0x020,  0x00000,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0x00000,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

// TLB entries to remap 1GB pages SRAM EM0 from 0x00000000_00000000-0x00000001_ffffffff to 0x30000000-0x6fffffff for MEM_WINDOW_0-MEM_WINDOW_7
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x000,  0x40000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x000,  0x80000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x000,  0xc0000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x001,  0x00000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_4,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x001,  0x40000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_5,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x001,  0x80000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_6,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x001,  0xc0000,    0x30000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_7,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
// TLB entries to remap 1GB pages SRAM EM1 from 0x00000002_00000000-0x00000003_ffffffff to 0x70000000-0xafffffff for MEM_WINDOW_0-MEM_WINDOW_7
    .long   MMU_TLB_ENTRY(  0x002,  0x00000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x002,  0x40000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x002,  0x80000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x002,  0xc0000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x003,  0x00000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_4,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x003,  0x40000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_5,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x003,  0x80000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_6,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x003,  0xc0000,    0x70000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_7,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

// TLB entries to remap 1GB pages SRAM_SDRAM EM2 from 0x00000004_00000000-0x00000005_ffffffff to 0xb0000000-0xefffffff for MEM_WINDOW_8-MEM_WINDOW_11
    .long   MMU_TLB_ENTRY(  0x004,  0x00000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x004,  0x40000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x004,  0x80000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x004,  0xc0000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x005,  0x00000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_4,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x005,  0x40000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_5,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x005,  0x80000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_6,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x005,  0xc0000,    0xb0000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_7,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

//tlb_entries_end:
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE
