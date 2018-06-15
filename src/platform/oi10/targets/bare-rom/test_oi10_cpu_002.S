#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/trace.S.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/ppc470s/mmu.S.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/regs/regs_plb6plb4.h>

#define TEST_DATA_WORD_0    0x12345678
#define P64CR_VAL			0x00000100


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init

    load_const r0,  0
    load_const r1,  0
    load_const r2,  0
    load_const r3,  0
    load_const r4,  0
    load_const r5,  0
    load_const r6,  0
    load_const r7,  0
    load_const r8,  0
    load_const r9,  0
    load_const r10, 0
    load_const r11, 0
    load_const r12, 0
    load_const r13, 0
    load_const r14, 0
    load_const r15, 0
    load_const r16, 0
    load_const r17, 0
    load_const r18, 0
    load_const r19, 0
    load_const r20, 0
    load_const r21, 0
    load_const r22, 0
    load_const r23, 0
    load_const r24, 0
    load_const r25, 0
    load_const r26, 0
    load_const r27, 0
    load_const r28, 0
    load_const r29, 0
    load_const r30, 0
    load_const r31, 0

init_spr:
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    load_const r0, 0x00000000	/* DBCR0 value          */
    mtspr   SPR_DBCR0, r0		/* DBCR0 clear          */

/* Setup DBSR */
    mfspr   r0, SPR_DBSR_RC		/* read current DBSR    */
    mtspr   SPR_DBSR_RC, r0		/* clear all valid bits */

    load_const r0,	reg_field( 0, 0b0   ) /* ITE        */ \
				  | reg_field( 1, 0b1   ) /* PRE        */ \
				  | reg_field( 4, 0b0   ) /* CRPE       */ \
				  | reg_field(10, 0b0   ) /* ICS        */ \
				  | reg_field(11, 0b0   ) /* DAPUIB     */ \
				  | reg_field(15, 0b0000) /* ICWRIDX    */ \
				  | reg_field(16, 0b0   ) /* DTB        */ \
				  | reg_field(23, 0b0   ) /* FLSTA      */ \
				  | reg_field(29, 0b00  ) /* DQWPM      */ \
				  | reg_field(30, 0b1   ) /* IQWPM      */
    mtspr   SPR_CCR0, r0

    load_const r0,	reg_field( 1, 0b00  ) /* GPRPEI     */ \
				  | reg_field( 3, 0b00  ) /* FPRPEI     */ \
				  | reg_field( 5, 0b00  ) /* ICDPEI     */ \
				  | reg_field( 7, 0b00  ) /* ICLPEI     */ \
				  | reg_field( 9, 0b00  ) /* ICTPEI     */ \
				  | reg_field(11, 0b00  ) /* DCDPEI     */ \
				  | reg_field(13, 0b00  ) /* DCLPEI     */ \
				  | reg_field(15, 0b00  ) /* DCTPEI     */ \
				  | reg_field(16, 0b0   ) /* MMUTPEI    */ \
				  | reg_field(17, 0b0   ) /* MMUDPEI    */ \
				  | reg_field(19, 0b0   ) /* TSS        */ \
				  | reg_field(21, 0b0   ) /* DPC        */ \
				  | reg_field(23, 0b00  ) /* TCS        */
    mtspr   SPR_CCR1, r0

    load_const r0,	reg_field( 1, 0b00  ) /* DSTG       */ \
				  | reg_field( 2, 0b0   ) /* DLFPD      */ \
				  | reg_field( 4, 0b0   ) /* DSTI       */ \
				  | reg_field( 9, 0b0   ) /* PMUD       */ \
				  | reg_field(11, 0b0   ) /* DCSTGW     */ \
				  | reg_field(15, 0b0000) /* STGCTR     */ \
				  | reg_field(16, 0b0   ) /* DISTG      */ \
				  | reg_field(20, 0b0   ) /* SPC5C1     */ \
				  | reg_field(21, 0b0   ) /* MCDTO      */
    mtspr   SPR_CCR2, r0

    load_const r0,	reg_field(29, 0b0   ) /* VBE0       */ \
    			  | reg_field(30, 0b0   ) /* VBE1       */ \
    			  | reg_field(31, 0b0   ) /* VBE2       */
    mtspr   SPR_MMUBE0, r0

    load_const r0,	reg_field(29, 0b0   ) /* VBE3       */ \
    			  | reg_field(30, 0b0   ) /* VBE4       */ \
    			  | reg_field(31, 0b0   ) /* VBE5       */
    mtspr   SPR_MMUBE1, r0

    load_const r0,	reg_field( 3, 0b0001) /* ORD1       */ \
    			  | reg_field( 7, 0b0010) /* ORD2       */ \
    			  | reg_field(11, 0b0011) /* ORD3       */ \
    			  | reg_field(15, 0b0100) /* ORD4       */ \
    			  | reg_field(19, 0b0101) /* ORD5       */ \
    			  | reg_field(23, 0b0110) /* ORD6       */ \
    			  | reg_field(27, 0b0111) /* ORD7       */
    mtspr   SPR_SSPCR, r0

    load_const r0,	reg_field( 3, 0b0001) /* ORD1       */ \
    			  | reg_field( 7, 0b0010) /* ORD2       */ \
    			  | reg_field(11, 0b0011) /* ORD3       */ \
    			  | reg_field(15, 0b0100) /* ORD4       */ \
    			  | reg_field(19, 0b0101) /* ORD5       */ \
    			  | reg_field(23, 0b0110) /* ORD6       */ \
    			  | reg_field(27, 0b0111) /* ORD7       */
    mtspr   SPR_ISPCR, r0

    load_const r0,	reg_field( 3, 0b0001) /* ORD1       */ \
    			  | reg_field( 7, 0b0010) /* ORD2       */ \
    			  | reg_field(11, 0b0011) /* ORD3       */ \
    			  | reg_field(15, 0b0100) /* ORD4       */ \
    			  | reg_field(19, 0b0101) /* ORD5       */ \
    			  | reg_field(23, 0b0110) /* ORD6       */ \
    			  | reg_field(27, 0b0111) /* ORD7       */
    mtspr   SPR_USPCR, r0

/*
	Before editing TLB let's initialize all SPR registers to default values by the spec
	Most of them have X-state after reset and therefore CPU model cannot work properly
*/
    load_const  r0, 0
    load_const  r1, 0xFFFFFFFF

    mtspr   SPR_CTR,	r0
    mtcr    r0
    mtspr   SPR_LR,		r0
    mtspr   SPR_DCESR,	r0
    mtspr   SPR_CSRR0,	r0
    mtspr   SPR_CSRR1,	r0
    mtspr   SPR_MCSRR0,	r0
    mtspr   SPR_MCSRR1,	r0
    mtspr   SPR_RMPD,	r0

    mtspr   SPR_IAC1,	r0
    mtspr   SPR_IAC2,	r0
    mtspr   SPR_IAC3,	r0
    mtspr   SPR_IAC4,	r0
    mtspr   SPR_DAC1,	r0
    mtspr   SPR_DAC2,	r0
    mtspr   SPR_DVC1,	r0
    mtspr   SPR_DVC2,	r0
    mtspr   SPR_DBCR1,	r0
    mtspr   SPR_DBCR2,	r0
    mtspr   SPR_DBDR,	r0
    mtspr   SPR_MCSR_C,	r1
    mtspr   SPR_DEAR,	r0
    mtspr   SPR_TCR,	r0
    mtspr   SPR_DEC,	r0
    mtspr   SPR_DECAR,	r0
    mtspr   SPR_TSR_RC,	r1
    mtspr   SPR_SRR0,	r0
    mtspr   SPR_SRR1,	r0
    mtspr   SPR_TBL_W,	r0
    mtspr   SPR_TBU_W,	r0
    mtspr   SPR_XER,	r0

    ppc470s_set_mem_window  r3, MEM_WINDOW_SHARED
    b       setup_tlb

.section ".init.rodata","a",@progbits

tlb_entries:
/* TLB entries to remap 64KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff */
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
/* TLB entry to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entry to remap 64KB page mirror-BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0x00000000-0x0000ffff*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entries to remap 128KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0xc0010000-0xc002ffff*/
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0xC0010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00020,    0xC0020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
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


.section ".init.text","ax",@progbits

/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_tlb:
    load_addr   r3, tlb_entries
    load_addr   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */

plb6plb4_init:
	load_const	r9,  0x10
	load_const	r10, DCR_PLB6PLB4_0_BASE + ADDRL1
	mtdcrx		r10, r9
	load_const	r10, DCR_PLB6PLB4_0_BASE + ADDRH1
	mtdcrx		r10, r9
	load_const	r10, DCR_PLB6PLB4_0_BASE + P64CR
	mfdcrx		r9,  r10
	ori			r9,  r9, P64CR_VAL
	mtdcrx		r10, r9

	b	    test_oi10_cpu_002

.section ".text","ax",@progbits

some_code0:
    /* r5++ */
    add r5,r5,r4
    blr

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
    load_const r2, 0x00000001 /* cmp1 */
    load_const r3, 0x00000002 /* cmp2 */
    load_const r4, 0x00000001
    load_const r5, 0x00000000 /* counter */

   /* branch to 0xFFFF0000 (.BOOT_ROM.text) */
    bla 0xFFFF0000
    cmp     cr7,0, r2, r5
    TEST_ASSERT(eq,cr7,"mmu check 1: value(1) mismatch")
    /* branch to 0x00000000 (.BOOT_ROM.text), virtual */
    bla 0x00000000
    cmp     cr7,0, r3, r5
    TEST_ASSERT(eq,cr7,"mmu check 1: value(2) mismatch")

    test_event EVENT_CHECK_CPU_PPC_MMU_1

    b ok

error:
    putchar 'E'
    putchar 'R'
    putchar 'R'
    putchar 'O'
    putchar 'R'
    putchar '\n'
    exit RESULT_ERROR
    b stop

ok:
    putchar 'O'
    putchar 'K'
    putchar '\n'
    exit RESULT_OK

stop:
    b stop

