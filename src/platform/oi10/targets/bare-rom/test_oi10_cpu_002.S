#undef __linux__
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/trace.S.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/ppc470s/mmu.S.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/oi10/platform/devices.h>

#define TEST_DATA_WORD_0    0x12345678


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
    load_const  r0, 0x00000000  /* make r0 "all zeros" source */

    gtube_init

    lis r0,  0
    lis r1,  0
    lis r2,  0
    lis r3,  0
    lis r4,  0
    lis r5,  0
    lis r6,  0
    lis r7,  0
    lis r8,  0
    lis r9,  0
    lis r10, 0
    lis r11, 0
    lis r12, 0
    lis r13, 0
    lis r14, 0
    lis r15, 0
    lis r16, 0
    lis r17, 0
    lis r18, 0
    lis r19, 0
    lis r20, 0
    lis r21, 0
    lis r22, 0
    lis r23, 0
    lis r24, 0
    lis r25, 0
    lis r26, 0
    lis r27, 0
    lis r28, 0
    lis r29, 0
    lis r30, 0
    lis r31, 0

init_spr:
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    load_const r0, 0x00000000 /* DBCR0 value */
    mtspr   SPR_DBCR0, r0       /* DBCR0 clear */

/* Setup DBSR */
    mfspr   r0, SPR_DBSR_RC        /* read current DBSR */
    mtspr   SPR_DBSR_RC, r0        /* clear all valid bits */

/* CCR0                 ITE                 PRE                  CRPE               ICS                  DAPUIB               ICWRIDX                 DTB                  FLSTA                DQWPM                 IQWPM */
    load_const r0, reg_field(0, 0b0) | reg_field(1, 0b1) | reg_field(4, 0b0) | reg_field(10, 0b0) | reg_field(11, 0b0) | reg_field(15, 0b0000) | reg_field(16, 0b0) | reg_field(23, 0b0) | reg_field(29, 0b00) | reg_field(30, 0b1)
    mtspr   SPR_CCR0, r0

/* CCR1                GPRPEI             FPRPEI                 ICDPEI              ICLPEI                ICTPEI                DCDPEI               DCLPEI                 DCTPEI              MMUTPEI              MMUDPEI                 TSS                 DPC                    TCS */
    load_const r0, reg_field(1, 0b00) | reg_field(3, 0b00) | reg_field(5, 0b00) | reg_field(7, 0b00) | reg_field(9, 0b00) | reg_field(11, 0b00) | reg_field(13, 0b00) | reg_field(15, 0b00) | reg_field(16, 0b0) | reg_field(17, 0b0) | reg_field(19, 0b0) | reg_field(21, 0b0) | reg_field(23, 0b00)
    mtspr   SPR_CCR1, r0

/* CCR2                 DSTG                  DLFPD            DSTI                  PMUD               DCSTGW                STGCTR                  DISTG               SPC5C1               MCDTO */
    load_const r0, reg_field(1, 0b00) | reg_field(2, 0b0) | reg_field(4, 0b0) | reg_field(9, 0b0) | reg_field(11, 0b0) | reg_field(15, 0b0000) | reg_field(16, 0b0) | reg_field(20, 0b0) | reg_field(21, 0b0)
    mtspr   SPR_CCR2, r0

/* MMUBE0                VBE0               VBE1                   VBE2 */
    load_const r0, reg_field(29, 0b0) | reg_field(30, 0b0) | reg_field(31, 0b0)
    mtspr   SPR_MMUBE0, r0

/* MMUBE1                VBE3               VBE4                   VBE5 */
    load_const r0, reg_field(29, 0b0) | reg_field(30, 0b0) | reg_field(31, 0b0)
    mtspr   SPR_MMUBE1, r0

/* SSPCR                 ORD1                   ORD2                   ORD3                    ORD4                    ORD5                    ORD6                    ORD7 */
    load_const r0, reg_field(3, 0b0001) | reg_field(7, 0b0010) | reg_field(11, 0b0011) | reg_field(15, 0b0100) | reg_field(19, 0b0101) | reg_field(23, 0b0110) | reg_field(27, 0b0111)
    mtspr   SPR_SSPCR, r0

/* ISPCR                ORD1                   ORD2                  ORD3                   ORD4                   ORD5                   ORD6                   ORD7 */
    load_const r0, reg_field(3, 0b001) | reg_field(7, 0b010) | reg_field(11, 0b011) | reg_field(15, 0b100) | reg_field(19, 0b101) | reg_field(23, 0b110) | reg_field(27, 0b111)
    mtspr   SPR_ISPCR, r0

/* USPCR                 ORD1                   ORD2                   ORD3                    ORD4                    ORD5                    ORD6                    ORD7 */
    load_const r0, reg_field(3, 0b0001) | reg_field(7, 0b0010) | reg_field(11, 0b0011) | reg_field(15, 0b0100) | reg_field(19, 0b0101) | reg_field(23, 0b0110) | reg_field(27, 0b0111)
    mtspr   SPR_USPCR, r0

/*
Before editing TLB let's initialize all SPR registers to default values by the spec
Most of them have X-state after reset and therefore CPU model cannot work properly
*/
    load_const  r0, 0
    load_const  r1, 0xFFFFFFFF

    mtspr   SPR_CTR, r0
    mtcr    r0
    mtspr   SPR_LR, r0
    mtspr   SPR_DCESR, r0
    mtspr   SPR_CSRR0, r0
    mtspr   SPR_CSRR1, r0
    mtspr   SPR_MCSRR0, r0
    mtspr   SPR_MCSRR1, r0
    mtspr   SPR_RMPD, r0

    mtspr   SPR_IAC1, r0
    mtspr   SPR_IAC2, r0
    mtspr   SPR_IAC3, r0
    mtspr   SPR_IAC4, r0
    mtspr   SPR_DAC1, r0
    mtspr   SPR_DAC2, r0
    mtspr   SPR_DVC1, r0
    mtspr   SPR_DVC2, r0
    mtspr   SPR_DBCR1, r0
    mtspr   SPR_DBCR2, r0
    mtspr   SPR_DBDR, r0
    mtspr   SPR_MCSR_C, r1
    mtspr   SPR_DEAR, r0
    mtspr   SPR_TCR, r0
    mtspr   SPR_DEC, r0
    mtspr   SPR_DECAR, r0
    mtspr   SPR_TSR_RC, r1
    mtspr   SPR_SRR0, r0
    mtspr   SPR_SRR1, r0
    mtspr   SPR_TBL_W, r0
    mtspr   SPR_TBU_W, r0
    mtspr   SPR_XER, r0

    ppc470s_set_mem_window  r3, MEM_WINDOW_SHARED
    b       setup_tlb


.section ".init.rodata","a",@progbits
tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
/* TLB entries to remap 64KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0x00040000-0x0005ffff */
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00020,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

    .long   MMU_TLB_ENTRY(  0x010,  0x00040,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00050,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00060,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00070,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* mirror 0x00001000 -> 0x00040000,0x00002000 -> 0x00040000 */
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00001,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0x00002,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* additional entry for test ffff0000 -> 0x00000000 */
    .long   MMU_TLB_ENTRY(  0x3ff,  0xffff0,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE


.section ".init.text","ax",@progbits

setup_tlb:
    load_addr   r3, tlb_entries
    load_addr   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */

    b           test_oi10_cpu_002


.section "ROM.text","ax",@progbits

some_code0:
    /* r5++ */
    add r5,r5,r4
    blr

event_check_cpu_ppc_prog_model_23:
/* test simple load, store commands
   lwa, lwax, lwaux not supported */

    rumboot_putstring "start checking data addressing (IM0)\n"
    /* D-FORM:
       d-form: comm REG, D(ADDR)
       r1 - input register
       r2 - address register
       r3 - output register */

    load_const          r2, 0x00040000 //IM0 address

    /* stb-lbz, stbu-lbzu */

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

    load_const          r2, 0x00040000
    load_const          r3, 0

    /* stbx-lbzx, stbux-lbzux */

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
    blr


test_oi10_cpu_002:

    bl event_check_cpu_ppc_prog_model_23

mmu_check_0:
    load_const r5, 0x123
    load_const r4, 0x321

    /* store data in IM0. Addres 0x00010000 */
    load_const          r2, IM0_BASE
    load_const          r1, TEST_DATA_WORD_0
    stwu                r1, 0(r2)
    //check
    lwzu                r3, 0(r2)
    cmp                 cr7,0, r3, r1
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at IM0_BASE")

    /* read data from virtual address 0x1000 */
    load_const          r2, 0x00001000
    lwzu                r4, 0(r2)
    cmp                 cr7,0, r3, r4
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00001000")

    /* read data from virtual address 0x2000 */
    load_const          r2, 0x00002000
    lwzu                r5, 0(r2)
    cmp                 cr7,0, r3, r5
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00002000")

    test_event EVENT_CHECK_CPU_PPC_MMU_0

mmu_check_1:

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

    gtube_init
    putchar 'E'
    putchar 'R'
    putchar 'R'
    putchar 'O'
    putchar 'R'
    putchar '\n'
    exit 1
    b stop

ok:

    gtube_init
    putchar 'O'
    putchar 'K'
    putchar '\n'
    exit 0

stop:
    b stop




