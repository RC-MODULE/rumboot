#include <arch/ppc_476fp_asm.h>
#include <platform/devices.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/regs/plb6bc.h>
#include <platform/regs/sctl.h>
#include <platform/bootm.h>
#include <rumboot/irq.h>

.section ".init.rodata","a",@progbits

.align 4
rom_tlb_entries:
nor_tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* TLB entries to remap 64KB page BOOTROM from 0x000003FF_FFFF0000-0x000003FF_FFFFFFFF to 0xFFFF0000-0xFFFFFFFF (bolted entries must be defined first)*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x01F,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
DECLARE_CONST( ROM_TLB_ENTRIES_N, (.-rom_tlb_entries)/MMU_TLB_ENTRY_SIZE )
    .long   MMU_TLB_ENTRY(  0x000,  0x7FFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )
DECLARE_CONST( NOR_TLB_ENTRIES_N, (.-nor_tlb_entries)/MMU_TLB_ENTRY_SIZE )

.section ".init.text","ax",@progbits

/* Low-level setup. No FPU, no interrupts yet */
.global rumboot_ppc_platform_setup_low
rumboot_ppc_platform_setup_low:
    load_const  r3, DCR_SCTL_BASE+SCTL_SYS_BOOT_CFG
    mfdcrx      r3, r3
    load_const  r4, BOOTM_NOR_BOOT
    and.        r3, r4, r3
    bne         write_rom_tlb_entries

write_nor_tlb_entries:
    load_addr   r3, nor_tlb_entries
    load_size   r4, NOR_TLB_ENTRIES_N
    b           write_xxx_tlb_entries

write_rom_tlb_entries:
    load_addr   r3, rom_tlb_entries
    load_size   r4, ROM_TLB_ENTRIES_N

write_xxx_tlb_entries:
    bl          write_tlb_entries
    blr


/* FPU and interrupts are setup already */
.global rumboot_ppc_platform_setup_high
rumboot_ppc_platform_setup_high:
setup_boot_rom_im0_p64:
    load_const  r3, DCR_PLB6PLB4_0_BASE
    /* enable requests to BOOT_ROM mirror (0x0000001F_FFFF0000:0x0000001F_FFFFFFFF) and IM0 (0x00000010_80000000:0x00000010_8001FFFF) */
    load_const  r4, 0x00000010
    load_const  r5, 0x0000001F
    bl          p64_set_split_range_address

setup_plb6bc:
    load_const  r6, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1MB)    << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)    << MMU_SSPCR_ORD4_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16KB)   << MMU_SSPCR_ORD5_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16MB)   << MMU_SSPCR_ORD6_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD7_i)

    load_const  r7, MEM_WINDOW_SHARED

    load_const  r3, DCR_PLB6_BC_BASE

    addi        r4, r3, PLB6BC_SGD1
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_SGD2
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_SGD3
    load_const  r5, 0x00000020
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_HCPP
    load_const  r5, PLB6BC_HCPP_4K
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_CR0
    load_const  r5, (0b1 << PLB6BC_CR0_BC_DSBS_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_MSRSP_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_SRSNZ_i)
.align 4
    mtdcrx      r4, r5          /* enable segmentation on P6BC */
    mtspr       SPR_SSPCR, r6   /* must be set explicitly after segmentation on P6BC has been enabled */
    mtspr       SPR_PID, r7     /* ppc470s_set_mem_window  r7, MEM_WINDOW_0 */
    isync

setup_tlb:
    load_addr   r3, tlb_entries
    load_size   r4, TLB_ENTRIES_N
    bl          write_tlb_entries /*(r3, r4) - are parameters*/

setup_stack: /*stack is growing down*/
    load_addr   r1, rumboot_platform_stack_area_end
    stwu        r0, -16(r1) /*allocate LR and save SP of first stack frame according to ABI rules*/
    blr


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
