
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

rumboot_entry_point:
    gtube_init
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    load_const  r1, 0xFFFFFFFF  // make r1 "all ones" source
    load_const  r2, 0x00000000
    load_const  r3, 0x00000000
    load_const  r4, 0x00000000
    load_const  r5, 0x00000000
    load_const  r6, 0x00000000
    load_const  r7, 0x00000000
    load_const  r8, 0x00000000
    load_const  r9, 0x00000000
    load_const  r10,0x00000000
    load_const  r11,0x00000000
    load_const  r12,0x00000000
    load_const  r13,0x00000000
    load_const  r14,0x00000000
    load_const  r15,0x00000000
    load_const  r16,0x00000000
    load_const  r17,0x00000000
    load_const  r18,0x00000000
    load_const  r19,0x00000000
    load_const  r20,0x00000000
    load_const  r21,0x00000000
    load_const  r22,0x00000000
    load_const  r23,0x00000000
    load_const  r24,0x00000000
    load_const  r25,0x00000000
    load_const  r26,0x00000000
    load_const  r27,0x00000000
    load_const  r28,0x00000000
    load_const  r29,0x00000000
    load_const  r30,0x00000000
    load_const  r31,0x00000000


    rumboot_putstring "Init SPRs...\n"
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0, r0       /* DBCR0 clear */

/* Setup DBSR */
    mfspr       r0, SPR_DBSR_RC     /* read current DBSR    */
    mtspr       SPR_DBSR_RC, r0     /* clear all valid bits */

    load_const  r0, reg_field( 0, 0b0   ) /* ITE     */ \
                  | reg_field( 1, 0b1   ) /* PRE     */ \
                  | reg_field( 4, 0b1   ) /* CRPE    */ \
                  | reg_field(10, 0b0   ) /* ICS     */ \
                  | reg_field(11, 0b0   ) /* DAPUIB  */ \
                  | reg_field(15, 0b0000) /* ICWRIDX */ \
                  | reg_field(16, 0b0   ) /* DTB     */ \
                  | reg_field(23, 0b0   ) /* FLSTA   */ \
                  | reg_field(29, 0b00  ) /* DQWPM   */ \
                  | reg_field(30, 0b1   ) /* IQWPM   */
    mtspr       SPR_CCR0, r0

    load_const  r0, reg_field( 1, 0b00  ) /* GPRPEI  */ \
                  | reg_field( 3, 0b00  ) /* FPRPEI  */ \
                  | reg_field( 5, 0b00  ) /* ICDPEI  */ \
                  | reg_field( 7, 0b00  ) /* ICLPEI  */ \
                  | reg_field( 9, 0b00  ) /* ICTPEI  */ \
                  | reg_field(11, 0b00  ) /* DCDPEI  */ \
                  | reg_field(13, 0b00  ) /* DCLPEI  */ \
                  | reg_field(15, 0b00  ) /* DCTPEI  */ \
                  | reg_field(16, 0b0   ) /* MMUTPEI */ \
                  | reg_field(17, 0b0   ) /* MMUDPEI */ \
                  | reg_field(19, 0b0   ) /* TSS     */ \
                  | reg_field(21, 0b0   ) /* DPC     */ \
                  | reg_field(23, 0b00  ) /* TCS     */
    mtspr       SPR_CCR1, r0

    load_const  r0, reg_field( 1, 0b00  ) /* DSTG    */ \
                  | reg_field( 2, 0b0   ) /* DLFPD   */ \
                  | reg_field( 4, 0b0   ) /* DSTI    */ \
                  | reg_field( 9, 0b0   ) /* PMUD    */ \
                  | reg_field(11, 0b0   ) /* DCSTGW  */ \
                  | reg_field(15, 0b0000) /* STGCTR  */ \
                  | reg_field(16, 0b0   ) /* DISTG   */ \
                  | reg_field(20, 0b0   ) /* SPC5C1  */ \
                  | reg_field(21, 0b0   ) /* MCDTO   */
    mtspr       SPR_CCR2, r0

    load_const  r0, (0b0 << MMU_MMUBE0_VBE0_i) \
                  | (0b0 << MMU_MMUBE0_VBE1_i) \
                  | (0b0 << MMU_MMUBE0_VBE2_i)
    mtspr       SPR_MMUBE0, r0

    load_const  r0, (0b0 << MMU_MMUBE1_VBE3_i) \
                  | (0b0 << MMU_MMUBE1_VBE4_i) \
                  | (0b0 << MMU_MMUBE1_VBE5_i)
    mtspr       SPR_MMUBE1, r0

    load_const  r0, ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                  | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                  | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) \
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB  ) << MMU_SSPCR_ORD4_i) \
                  | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD5_i)
    mtspr       SPR_SSPCR, r0

    load_const  r0, (MMU_XSPCR_ORD_64KB  << MMU_SSPCR_ORD1_i) \
                  | (MMU_XSPCR_ORD_4KB   << MMU_SSPCR_ORD2_i) \
                  | (MMU_XSPCR_ORD_256MB << MMU_SSPCR_ORD3_i) \
                  | (MMU_XSPCR_ORD_1GB   << MMU_SSPCR_ORD4_i) \
                  | (MMU_XSPCR_ORD_END   << MMU_SSPCR_ORD5_i)
    mtspr       SPR_ISPCR, r0

    load_const  r0, ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                  | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                  | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) \
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB  ) << MMU_SSPCR_ORD4_i) \
                  | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD5_i)
    mtspr       SPR_USPCR, r0

/*
    Before editing TLB let's initialize all SPR registers to default values by the spec
    Most of them have X-state after reset and therefore CPU model cannot work properly
*/
    load_const  r0, 0x00000000  // make r0 "all zeros" source

    mtspr       SPR_LR,     r0
    mtspr       SPR_CTR,    r0
    mtcr        r0
    mtspr       SPR_XER,    r0

    mtspr       SPR_SRR0,   r0
    mtspr       SPR_SRR1,   r0
    mtspr       SPR_CSRR0,  r0
    mtspr       SPR_CSRR1,  r0
    mtspr       SPR_MCSRR0, r0
    mtspr       SPR_MCSRR1, r0
    mtspr       SPR_MCSR_C, r1
    mtspr       SPR_ESR,    r0
    mtspr       SPR_DCESR,  r0
    mtspr       SPR_DEAR,   r0

    mtspr       SPR_TCR,    r0
    mtspr       SPR_TBL_W,  r0
    mtspr       SPR_TBU_W,  r0
    mtspr       SPR_DEC,    r0
    mtspr       SPR_DECAR,  r0
    mtspr       SPR_TSR_RC, r1

    mtspr       SPR_IAC1,   r0
    mtspr       SPR_IAC2,   r0
    mtspr       SPR_IAC3,   r0
    mtspr       SPR_IAC4,   r0
    mtspr       SPR_DAC1,   r0
    mtspr       SPR_DAC2,   r0
    mtspr       SPR_DVC1,   r0
    mtspr       SPR_DVC2,   r0
    mtspr       SPR_DBCR1,  r0
    mtspr       SPR_DBCR2,  r0
    mtspr       SPR_DBDR,   r0
    mtspr       SPR_RMPD,   r0

    b           setup_tlb

.align 2
tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
/* Invalidate initial TLB entry */
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
/* TLB entry to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff */
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

setup_tlb:
    rumboot_putstring "Setup TLB...\n"
    load_addr   r3, tlb_entries
    load_addr   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */


    rumboot_putstring "Run tests...\n"

    rumboot_putstring   "TEST IS NOT READY!!!\n"
    b           test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event EVENT_OK
    exit RESULT_OK

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    exit RESULT_ERROR

stop:
    b stop

