
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#define TEST_DATA_WORD_0    0x12345678
#define P64CR_VAL			0x00000100


.section ".text","ax",@progbits

.global main
.global some_code0

main:
rumboot_entry_point:

    load_const r3,  0x00000000
    load_const r4,  0x00000000
    load_const r5,  0x00000000
    load_const r6,  0x00000000
    load_const r7,  0x00000000
    load_const r8,  0x00000000
    load_const r9,  0x00000000
    load_const r10, 0x00000000
    load_const r11, 0x00000000
    load_const r12, 0x00000000
    load_const r13, 0x00000000
    load_const r14, 0x00000000
    load_const r15, 0x00000000
    load_const r16, 0x00000000
    load_const r17, 0x00000000
    load_const r18, 0x00000000
    load_const r19, 0x00000000
    load_const r20, 0x00000000
    load_const r21, 0x00000000
    load_const r22, 0x00000000
    load_const r23, 0x00000000
    load_const r24, 0x00000000
    load_const r25, 0x00000000
    load_const r26, 0x00000000
    load_const r27, 0x00000000
    load_const r28, 0x00000000
    load_const r29, 0x00000000
    load_const r30, 0x00000000
    load_const r31, 0x00000000

	mtspr SPR_SPRG7,	r1	/* Save stack! */
    mflr  r29				/* save LR */
    mtspr SPR_SPRG8, 	r29	/* ------- */

    ppc470s_set_mem_window  r3, MEM_WINDOW_SHARED
    b       setup_tlb

tlb_entries:
/* TLB entries to remap 64KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff */
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
/* Invalidate initial TLB entry*/
/*	.long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 ) */
/* TLB entry to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff*/
/*	.long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 ) */
/* TLB entry to remap 64KB page mirror-BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0x00000000-0x0000ffff*/
	.long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entries to remap 128KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0xc0010000-0xc002ffff*/
/*	.long   MMU_TLB_ENTRY(  0x010,  0x00010,    0xC0010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 ) */
/*	.long   MMU_TLB_ENTRY(  0x010,  0x00020,    0xC0020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 ) */
/* mirror from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff */
	.long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
	.long   MMU_TLB_ENTRY(  0x010,  0x00020,    0x00020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* mirror from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff */
	.long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
	.long   MMU_TLB_ENTRY(  0x010,  0x00020,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* mirror from 0x00000010_00010000-0x00000010_0002ffff to 0x00060000-0x0007ffff */
	.long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
	.long   MMU_TLB_ENTRY(  0x010,  0x00020,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE


/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_tlb:
    load_addr   r3, tlb_entries
    load_addr   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */

	b		    test_oi10_cpu_002


test_oi10_cpu_002:

/* test simple load, store commands
   lwa, lwax, lwaux not supported */

    rumboot_putstring "start checking data addressing (IM0)\n"
    /* D-FORM:
       d-form: comm REG, D(ADDR)
       r1 - input register
       r2 - address register
       r3 - output register */

    load_const          r2, IM0_BASE

    /* stb-lbz, stbu-lbzu */
	rumboot_putstring "\nstb-lbz, stbu-lbzu\n"
    load_const          r1, 0xFF
    stb                 r1, 0(r2)
    lbz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAA
    stb                 r1, 0(r2)
    lbz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55
    stb                 r1, 0(r2)
    lbz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFF
    stbu                r1, 0(r2)
    lbzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAA
    stbu                r1, 0(r2)
    lbzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55
    stbu                r1, 0(r2)
    lbzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* sth-lhz, sthu-lhzu */
	rumboot_putstring "\nsth-lhz, sthu-lhzu\n"
    load_const          r1, 0xFFFF
    sth                 r1, 0(r2)
    lhz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAA
    sth                 r1, 0(r2)
    lhz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sth                 r1, 0(r2)
    lhz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFFFF
    sthu                r1, 0(r2)
    lhzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAA
    sthu                r1, 0(r2)
    lhzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthu                r1, 0(r2)
    lhzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* stw-lwz, stwu-lwzu */
	rumboot_putstring "\nstw-lwz, stwu-lwzu\n"
    load_const          r1, 0xFFFFFFFF
    stw                 r1, 0(r2)
    lwz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAAAAAA
    stw                 r1, 0(r2)
    lwz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55555555
    stw                 r1, 0(r2)
    lwz                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFFFFFFFF
    stwu                r1, 0(r2)
    lwzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAAAAAA
    stwu                r1, 0(r2)
    lwzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55555555
    stwu                r1, 0(r2)
    lwzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* sth-lha, sthu-lhau */
	rumboot_putstring "\nsth-lha, sthu-lhau\n"
    load_const          r1, 0x7FFF
    sth                 r1, 0(r2)
    lha                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x6AAA
    sth                 r1, 0(r2)
    lha                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sth                 r1, 0(r2)
    lha                 r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x7FFF
    sthu                r1, 0(r2)
    lhau                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x6AAA
    sthu                r1, 0(r2)
    lhau                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthu                r1, 0(r2)
    lhau                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* X-FORM:

       x-form: comm REG, REGA, REGB
       r1 - input register
       r2 - first address register
       r3 - second address register
       r4 - output register */

    load_const          r2, IM0_BASE
    load_const          r3, 0

    /* stbx-lbzx, stbux-lbzux */
	rumboot_putstring "\nstbx-lbzx, stbux-lbzux\n"
    load_const          r1, 0xFF
    stbx                r1, r2,r3
    lbzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAA
    stbx                r1, r2,r3
    lbzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55
    stbx                r1, r2,r3
    lbzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFF
    stbux               r1, r2,r3
    lbzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAA
    stbux               r1, r2,r3
    lbzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55
    stbux               r1, r2,r3
    lbzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* sthx-lhzx, sthux-lhzux */
	rumboot_putstring "\nsthx-lhzx, sthux-lhzux\n"
    load_const          r1, 0xFFFF
    sthx                r1, r2,r3
    lhzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAA
    sthx                r1, r2,r3
    lhzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthx                r1, r2,r3
    lhzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFFFF
    sthux               r1, r2,r3
    lhzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAA
    sthux               r1, r2,r3
    lhzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthux               r1, r2,r3
    lhzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* stwx-lwzx, stwux-lwzux */
	rumboot_putstring "\nstwx-lwzx, stwux-lwzux\n"
    load_const          r1, 0xFFFFFFFF
    stwx                r1, r2,r3
    lwzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAAAAAA
    stwx                r1, r2,r3
    lwzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55555555
    stwx                r1, r2,r3
    lwzx                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xFFFFFFFF
    stwux               r1, r2,r3
    lwzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0xAAAAAAAA
    stwux               r1, r2,r3
    lwzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x55555555
    stwux               r1, r2,r3
    lwzux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    /* sthx-lhax,  sthux-lhaux */
	rumboot_putstring "\nsthx-lhax, sthux-lhaux\n"
    load_const          r1, 0x7FFF
    sthx                r1, r2,r3
    lhax                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x6AAA
    sthx                r1, r2,r3
    lhax                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthx                r1, r2,r3
    lhax                r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x7FFF
    sthux               r1, r2,r3
    lhaux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x6AAA
    sthux               r1, r2,r3
    lhaux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    load_const          r1, 0x5555
    sthux               r1, r2,r3
    lhaux               r4, r2,r3
    cmp                 cr7,0, r4, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch")

    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_23

mmu_check_0:
    load_const r5, 0x123
    load_const r4, 0x321

    /* store data in IM0. */
	rumboot_putstring "\nStart MMU ckeck stage 0. Store to IM0_BASE VA\n"
    load_const          r2, IM0_BASE
    load_const          r1, TEST_DATA_WORD_0
    stwu                r1, 0(r2)
    /* check */
    lwzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at IM0_BASE")

    /* read data from virtual address 0x00040000 */
	rumboot_putstring   "\nRead from VA 0x00040000...\n"
    load_const          r2,	          0x00040000
    lwzu                r4, 0(r2)
    cmp                 cr7,0, r3, r4
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00040000")

    /* read data from virtual address 0x00060000 */
	rumboot_putstring   "\nRead from VA 0x00060000...\n"
    load_const          r2,           0x00060000
    lwzu                r5, 0(r2)
    cmp                 cr7,0, r3, r5
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00060000")

    test_event EVENT_CHECK_CPU_PPC_MMU_0

mmu_check_1:

	rumboot_putstring "\nStart MMU ckeck stage 1.\n"
    mflr r29		/* save LR */
    load_const r2, 0x00000001 /* cmp1 */
    load_const r3, 0x00000002 /* cmp2 */
    load_const r4, 0x00000001
    load_const r5, 0x00000000 /* counter */

	rumboot_putstring "Branch to 0xFFFFxxxx... "
    bla some_code0
    cmp     cr7,0, r2, r5
    TEST_ASSERT(eq,cr7,"mmu check 1: value(1) mismatch")
    rumboot_putstring "Done!\n"
    lis r6, 0x0000
    ori r6, r6, some_code0@l
    mtctr r6	/* Address to CTR */
	rumboot_putstring "Branch to 0x0000xxxx... "
    bctrl		/* Do branch */
    cmp     cr7,0, r3, r5
    TEST_ASSERT(eq,cr7,"mmu check 1: value(2) mismatch")
    rumboot_putstring "Done!\n"

    test_event EVENT_CHECK_CPU_PPC_MMU_1

    b test_ok


test_ok:
    rumboot_putstring "\nTEST OK\n"
    test_event		EVENT_OK
    load_const r3,	RESULT_OK
    b finish

test_error:
    rumboot_putstring "\nTEST ERROR\n"
    test_event		EVENT_ERROR
    load_const r3,	RESULT_ERROR

finish:
	mfspr r29, SPR_SPRG8	/* Restore LR */
    mtlr  r29				/* ---------- */
	mfspr r1,  SPR_SPRG7	/* Restore stack! */
    blr


some_code0:
    add r5,r5,r4
	rumboot_putstring "ok "
    blr
	.long 0xBAD4DEAD

