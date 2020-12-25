#include <rumboot/macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/ppc470s/mmu/mem_window.h>


.section ".rodata.rom","a",@progbits
.global tlb_entries
.global TLB_ENTRIES_N

.align 4
tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/

/* Invalidate BOOTROM TLB entry for MEM_WINDOW_1 */
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )

/* TLB entries to remap 128KB pages IM0 from 0x00000010_80000000-0x00000010_8001FFFF to 0x80000000-0x8001FFFF*/
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )

/* TLB entries to remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0x80020000-0x8003FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )

/* TLB entries to remap 128KB pages IM2 from 0x00000020_C0040000-0x00000020_C005FFFF to 0x80040000-0x8005FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0050,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entries to remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0xC0000000-0xC001FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0000,    0xC0000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0010,    0xC0010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entry to remap LSIF registers   from 0x00000020_C0020000-0x00000020_C003FFFF to 0xC0020000-0xC003FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0020,    0xC0020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0030,    0xC0030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entries to remap 128KB pages IM2 from 0x00000020_C0040000-0x00000020_C005FFFF to 0xC0040000-0xC005FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0040,    0xC0040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0050,    0xC0050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entries to remap 128KB pages IM3 from 0x00000020_C0060000-0x00000020_C007FFFF to 0xC0060000-0xC007FFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0060,    0xC0060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0070,    0xC0070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entry to remap HSIF commutator registers from 0x00000020_C0100000-0x00000020_C02FFFFF to 0xC0000000-0xC02FFFFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0100,    0xC0100,    MMU_TLBE_DSIZ_1MB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0200,    0xC0200,    MMU_TLBE_DSIZ_1MB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0xC0400,    0xC0400,    MMU_TLBE_DSIZ_1MB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entry to remap HSIF SW registers from 0x00000020_C0300000-0x00000020_C0303FFF to 0xC0300000-0xC0303FFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0300,    0xC0300,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entry to remap SCRB registers from 0x00000020_C0307000-0x00000020_C0307FFF to 0xC0307000-0xC0307FFF*/
    .long   MMU_TLB_ENTRY(  0x020,  0xC0307,    0xC0307,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

/* TLB entries to remap 2 1GB pages SRAM_SDRAM EM2 from 0x00000004_00000000-0x00000005_FFFFFFFF to 0x00000000-0x7FFFFFFf for MEM_WINDOW_0-MEM_WINDOW_3*/
    .long   MMU_TLB_ENTRY(  0x000,  0x70000,    0x70000,    MMU_TLBE_DSIZ_256MB,    0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

/*tlb_entries_end:*/
DECLARE_CONST( TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE )
