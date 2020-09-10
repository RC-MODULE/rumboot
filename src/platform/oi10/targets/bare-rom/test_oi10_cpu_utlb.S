/* -*- rumboot-test-labels: broken -*- */
// did, 09.09.20
// Replaced write_tlb_entries_loop filling UTLB entries
// on write_tlb_entrie from rumboot/src/platform/oi10/ppc_mmu_impl.S
#define DEBUG_CONTINUE_ON_ASSERT

#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
/* CCR0, CCR1, CCR2 fields */
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
/* MSR fields */
#include <arch/ppc_476fp_itrpt_fields.h>


#define reg_field32 reg_field

#define VAL_MAGIC   0xBED4DEAD
#define BEG_ADDRX   0xFFFE0000  /*      Last 64k page   */
#define BEG_ADDR0   0xFFFE0000  /* Skip last 64k page   */
#define END_ADDR0   0xFF000000
#define BEG_ADDR1   0xFEFF0000  /* 11111110_11111111    */
#define END_ADDR1   0xFE000000  /* 11111110_00000000    */
#define BEG_ADDR2   0xFDFF0000  /* 11111101_11111111    */
#define END_ADDR2   0xFD000000  /* 11111101_00000000    */
#define BEG_ADDR3   0xFBFF0000  /* 11111011_11111111    */
#define END_ADDR3   0xFB000000  /* 11111011_00000000    */
#define PHY_ADDR0   0xFFFF0000
#define PHY_ADDRE   0x000003FF
#define CNT_INIT0   0x00000000
#define CNT_INIT1   0x00000001

#define MMU_WAY_0       MMU_TLBWE_WAY_0
#define MMU_WAY_1       MMU_TLBWE_WAY_1
#define MMU_WAY_2       MMU_TLBWE_WAY_2
#define MMU_WAY_3       MMU_TLBWE_WAY_3

#define FILL_STEP   (0 - 65536)
#define READ_STEP   (0 + 65536)

#define R_STKP      r1          /* Stack pointer (don't work here)  */
#define R_ARG0      r3          /* Standart argument #0             */
#define R_ARG1      r4          /* Standart argument #1             */
#define R_ARG2      r5          /* Standart argument #2             */
#define R_ARG3      r6          /* Standart argument #3             */
#define R_ATTR      r13         /* UTLB Attributes                  */
#define R_MMUC      r14         /* Holds MMURC                      */
#define R_TLBR      r15         /* Holds tlbwe second parameter     */
#define R_ETAG      r16         /* Holds UTLB Entry TAG             */
#define R_MASK      r17         /* Address mask                     */
#define R_DUMP      r18         /* Dump value for debug purposes    */
#define R_CNTR      r19         /* Counter (for UTLB index compare) */
#define R_AIDX      R_CNTR      /* Offset in page table             */
#define R_ADDR      r20         /* Uniform address pointer          */
#define R_BEGA      R_ADDR      /* Begin address                    */
#define R_ENDA      r21         /* End address                      */
#define R_STEP      r22         /* Step                             */
#define R_FILL      r23         /* Fill value                       */
#define R_EXPV      R_FILL      /* Expected value                   */
#define R_DATA      r24         /* Readed or output data            */
#define R_SAV0      r26         /* Copy of SRR0                     */
#define R_SAV1      r27         /* Copy of SRR1                     */
#define R_SHFT      R_SAV0      /* Shift value                      */
#define R_XORM      R_SAV1      /* XOR mask                         */
#define R_TPL0      r28         /* Template for tlbwe second arg    */
#define R_TEMP      r29         /* Temporary data                   */
#define R_ZERO      r30         /* 0x00000000                       */
#define R_ONES      r31         /* 0xFFFFFFFF                       */

#define CONST_MSR     reg_field64(ITRPT_XSR_WE_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_CE_e,  0b1   )  \
                    | reg_field64(ITRPT_XSR_EE_e,  0b1   )  \
                    | reg_field64(ITRPT_XSR_PR_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_FP_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_ME_e,  0b1   )  \
                    | reg_field64(ITRPT_XSR_FE0_e, 0b0   )  \
                    | reg_field64(ITRPT_XSR_DWE_e, 0b0   )  \
                    | reg_field64(ITRPT_XSR_DE_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_FE1_e, 0b0   )  \
                    | reg_field64(ITRPT_XSR_IS_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_DS_e,  0b0   )  \
                    | reg_field64(ITRPT_XSR_PMM_e, 0b0   )

#define CONST_CCR0    reg_field32( 0, 0b0   ) /* ITE     */ \
                    | reg_field32( 1, 0b1   ) /* PRE     */ \
                    | reg_field32( 4, 0b1   ) /* CRPE    */ \
                    | reg_field32(10, 0b0   ) /* ICS     */ \
                    | reg_field32(11, 0b0   ) /* DAPUIB  */ \
                    | reg_field32(15, 0b0000) /* ICWRIDX */ \
                    | reg_field32(16, 0b0   ) /* DTB     */ \
                    | reg_field32(23, 0b0   ) /* FLSTA   */ \
                    | reg_field32(29, 0b00  ) /* DQWPM   */ \
                    | reg_field32(30, 0b1   ) /* IQWPM   */

#define CONST_CCR1    reg_field32( 1, 0b00  ) /* GPRPEI  */ \
                    | reg_field32( 3, 0b00  ) /* FPRPEI  */ \
                    | reg_field32( 5, 0b00  ) /* ICDPEI  */ \
                    | reg_field32( 7, 0b00  ) /* ICLPEI  */ \
                    | reg_field32( 9, 0b00  ) /* ICTPEI  */ \
                    | reg_field32(11, 0b00  ) /* DCDPEI  */ \
                    | reg_field32(13, 0b00  ) /* DCLPEI  */ \
                    | reg_field32(15, 0b00  ) /* DCTPEI  */ \
                    | reg_field32(16, 0b0   ) /* MMUTPEI */ \
                    | reg_field32(17, 0b0   ) /* MMUDPEI */ \
                    | reg_field32(19, 0b0   ) /* TSS     */ \
                    | reg_field32(21, 0b0   ) /* DPC     */ \
                    | reg_field32(23, 0b00  ) /* TCS     */

#define CONST_CCR2    reg_field32( 1, 0b00  ) /* DSTG    */ \
                    | reg_field32( 2, 0b0   ) /* DLFPD   */ \
                    | reg_field32( 4, 0b0   ) /* DSTI    */ \
                    | reg_field32( 9, 0b0   ) /* PMUD    */ \
                    | reg_field32(11, 0b0   ) /* DCSTGW  */ \
                    | reg_field32(15, 0b0000) /* STGCTR  */ \
                    | reg_field32(16, 0b0   ) /* DISTG   */ \
                    | reg_field32(20, 0b0   ) /* SPC5C1  */ \
                    | reg_field32(21, 0b0   ) /* MCDTO   */

#define CONST_MMUBE0  (0b0 << MMU_MMUBE0_VBE0_i) \
                    | (0b0 << MMU_MMUBE0_VBE1_i) \
                    | (0b0 << MMU_MMUBE0_VBE2_i)

#define CONST_MMUBE1  (0b0 << MMU_MMUBE1_VBE3_i) \
                    | (0b0 << MMU_MMUBE1_VBE4_i) \
                    | (0b0 << MMU_MMUBE1_VBE5_i)

#define CONST_SSPCR   ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define CONST_ISPCR   ((                          MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((                          MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define CONST_USPCR   ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define FILL_ATTR   MMU_TLBE_ATTR(  1, 1,               /* IL1I, IL1D   */  \
                                    0, 1, 0, 1,         /* W, I, M, G,  */  \
                                    MMU_TLBE_E_BIG_END, /* Endianness   */  \
                                    1, 0, 1,            /* UX, UW, UR   */  \
                                    1, 0, 1,            /* SX, SW, SR   */  \
                                    0, 0)

.macro fill_utlb vWay, vAttr, aBeg, aEnd, aStep, vCntInit, vFill, vEFill
    rumboot_putstring       "Fill UTLB-entries for way \vWay from \aBeg to \aEnd\()...\n"
    load_const  R_CNTR,     \vCntInit
    load_const  R_MASK,     0xFFFF0000
    load_const  R_XORM,     0x00FF
    load_const  R_MMUC,     MMU_MMUCR(0,0,0,0,MEM_WINDOW_SHARED)
    load_const  R_BEGA,     \aBeg
    load_const  R_ENDA,     \aEnd
    load_const  R_STEP,     \aStep
    load_const  R_FILL,     MMU_TLBE_DATA(\vEFill, (\vFill) >> 12, 0, 0)
    load_const  R_ETAG,     MMU_TLBE_TAG(0,1,0,MMU_TLBE_DSIZ_64KB,0,0,0,0)
    load_const  R_TPL0,     ((\vWay) << MMU_TLBE_RA_WAY_i)
    load_const  R_ATTR,     \vAttr
    bl          fill_utlb_loop
.endm

.macro check_read_pages aEnd, aBeg, aStep, vExp
    rumboot_putstring       "Read from \aBeg to \aEnd\()...\n"
    load_const  R_BEGA,     \aBeg
    load_const  R_ENDA,     \aEnd
    load_const  R_STEP,     \aStep
    load_const  R_EXPV,     \vExp
    load_const  R_AIDX,     0xF000
    bl          read_pages_loop
.endm

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

magic_data:
    .long VAL_MAGIC

rumboot_entry_point:
    gtube_init

    /* Clear listed GP registers */
    .irp RN,    2,  3,  4,  5,  6,  7,  8,  9,  10, 11, \
                12, 13, 14, 15, 16, 17, 18, 19, 20, 21, \
                22, 23, 24, 25, 26, 27, 28, 29, 30
        load_const  \RN,    0x00000000
    .endr

    nand        R_ONES, R_ZERO, R_ZERO

    rumboot_putstring "Init SPRs...\n"

/* Setup interrupt/exception handler */
    lis         R_TEMP,     exception_handler@h
    li          R_ADDR,     exception_handler@l
    mtspr       SPR_IVPR,   R_TEMP
    .irp        r_ivor,                                         \
                SPR_IVOR0,  SPR_IVOR1,  SPR_IVOR2,  SPR_IVOR3,  \
                SPR_IVOR4,  SPR_IVOR5,  SPR_IVOR6,  SPR_IVOR7,  \
                SPR_IVOR8,  SPR_IVOR9,  SPR_IVOR10, SPR_IVOR11, \
                SPR_IVOR12, SPR_IVOR13, SPR_IVOR14, SPR_IVOR15
        mtspr   \r_ivor,    R_ADDR
    .endr

/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0, R_ZERO   /* DBCR0 clear */

/* Setup DBSR */
    mfspr       R_TEMP, SPR_DBSR_RC     /* read current DBSR    */
    mtspr       SPR_DBSR_RC, R_TEMP     /* clear all valid bits */

/* Setup MSR  */
    load_const  R_TEMP, CONST_MSR
    mtmsr       R_TEMP

    load_spr    R_TEMP, SPR_CCR0,   CONST_CCR0,
    load_spr    R_TEMP, SPR_CCR1,   CONST_CCR1,
    load_spr    R_TEMP, SPR_CCR2,   CONST_CCR2,
    load_spr    R_TEMP, SPR_MMUBE0, CONST_MMUBE0,
    load_spr    R_TEMP, SPR_MMUBE1, CONST_MMUBE1,
    load_spr    R_TEMP, SPR_SSPCR,  CONST_SSPCR,
    load_spr    R_TEMP, SPR_ISPCR,  CONST_ISPCR,
    load_spr    R_TEMP, SPR_USPCR,  CONST_USPCR,


/*
    Before editing TLB let's initialize all SPR registers to default values by the spec
    Most of them have X-state after reset and therefore CPU model cannot work properly
*/

    .irp        nSPR,                   \
                SPR_SRR0,   SPR_SRR1,   \
                SPR_CSRR0,  SPR_CSRR1,  \
                SPR_MCSRR0, SPR_MCSRR1, \
                SPR_ESR,                \
                SPR_DCESR,              \
                SPR_DEAR,               \
                SPR_TCR,                \
                SPR_TBL_W,  SPR_TBU_W,  \
                SPR_DEC,    SPR_DECAR,  \
                SPR_IAC1,   SPR_IAC2,   \
                SPR_IAC3,   SPR_IAC4,   \
                SPR_DAC1,   SPR_DAC2,   \
                SPR_DVC1,   SPR_DVC2,   \
                SPR_DBCR1,  SPR_DBCR2,  \
                SPR_DBDR,   SPR_RMPD
        mtspr   \nSPR,      R_ZERO
    .endr

    .irp        nSPR,       lr,cr,ctr,xer,          \
                            sprg4,sprg5,sprg6,sprg7
        mt\nSPR             R_ZERO
    .endr


    mtspr       SPR_MCSR_C, R_ONES
    mtspr       SPR_TSR_RC, R_ONES

    b           setup_tlb

.align 4
exception_handler:
    rumboot_putstring   "Unexpected exception!\n"
    rumboot_putstring   "MCSR is:\n"
    mfspr       R_TEMP, SPR_MCSR_RW
    rumboot_puthex      R_TEMP
    rumboot_putstring   "MCSRR0 is:\n"
    mfspr       R_TEMP, SPR_MCSRR0
    rumboot_puthex      R_TEMP
    clr_cr  cr6,        eq
    b       test_end

.align 2
tlb_entries:
/*          MMU_TLB_ENTRY(ERPN,  RPN,     EPN,     DSIZ,               IL1I,IL1D, W,I,M,G, E,                  UX, UW, UR,  SX, SW, SR   DULXE,IULXE, TS,  TID,               WAY,             BID,              V   ) */
/* Invalidate initial TLB entry */
    .long   MMU_TLB_ENTRY(0x000, 0x00000, 0xFFFFF, MMU_TLBE_DSIZ_4KB,  0b0, 0b0,  0,0,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b0,0b0,0b0, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0 )
/* TLB entry to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff */
    .long   MMU_TLB_ENTRY(0x3FF, 0xFFFF0, 0xFFFF0, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b1,0b0,0b1, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b1 )


/*
.align 2
utlb_entry_tpl:
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b0 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_1, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_2, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b1 )
    .long   0xAA55AA55
    .long   0x55AA55AA
*/


setup_tlb:
    rumboot_putstring   "Setup TLB...\n"
    load_addr   R_ARG0, tlb_entries
    load_const  R_ARG1, 0x0002
    mfspr       R_TEMP, SPR_RSTCFG
    addi        R_ARG0, R_ARG0, -4
/* did, 09.09.20
write_tlb_entries_loop:
    cmpwi cr7,  R_ARG1, 0x0000
    beq-  cr7,  write_tlb_entries_end

    // entry[0] -> MMUCR 
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    mtspr       SPR_MMUCR, R_DATA

    // entry[1] -> WTF? 
    lwzu        R_TLBR, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)

    // entry[2] -> ETAG    (WS=0) 
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    tlbwe       R_DATA, R_TLBR, MMU_TLB_ENTRY_TAG

    // entry[3] -> EDATA   (WS=1) 
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    tlbwe       R_DATA, R_TLBR, MMU_TLB_ENTRY_DATA

    // entry[4] ->EATTR    (WS=2) 
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    copy_field(                                             \
                R_DATA,                                     \
                MMU_TLBE_ATTR_U_e-(MMU_TLBE_ATTR_U_n-1),    \
                MMU_TLBE_ATTR_U_e,                          \
                R_TEMP,                                     \
                CTRL_RSTCFG_U_e-(CTRL_RSTCFG_U_n-1)         \
              )

    tlbwe       R_DATA, R_TLBR, MMU_TLB_ENTRY_ATTR
    subi        R_ARG1, R_ARG1, 0x0001

    b           write_tlb_entries_loop

write_tlb_entries_end:
    mfspr       R_SAV0,     SPR_SRR0
    mfspr       R_SAV1,     SPR_SRR1

    mfmsr       R_DATA
    mtspr       SPR_SRR1,   R_DATA
    load_addr   R_ADDR,     write_tlb_entries_ret
    mtspr       SPR_SRR0,   R_ADDR

    rfi

write_tlb_entries_ret:
    mtspr       SPR_SRR1,   R_SAV1
    mtspr       SPR_SRR0,   R_SAV0

    rumboot_putstring       "Run subtests...\n"
    set_cr      cr6,        eq
*/
// From rumboot/src/platform/oi10/ppc_mmu_impl.S 
//Details about trace infrastructure convention see in trace.S.h
//Caller must preserve r3,r4,r8,r9,r10,r11 before calling
//r3 - TLB entries array pointer
//r4 - TLB entries count
//r8,r9,r10,r11 - tmp registers
//returns nothing
// r29 - tlbentry[EPN, V, TS, DSIZ] <= RS[0:27]
// r30 - tlbentry[RPN]  <= RS[0 :19]
//       tlbentry[ERPN] <= RS[22:31]
// r31 - tlbentry[IL1I, IL1D, U(0:3), W, I, M, G, E] <= RS[14: 24]
//       tlbentry[UX, UW, UR, SX, SW, SR]            <= RS[26: 31]
//.global write_tlb_entries

   load_addr   r3, tlb_entries   
   load_const  r4, 0x0002

write_tlb_entries_bare_rom:
    cmplwi      r4, 0
    beqlr-

    mr          r8, r28
    mr          r9, r29
    mr          r10,r30
    mr          r11,r31

.align 4
write_next_etry:
    lmw         r28,0(r3)
    addi        r3, r3, MMU_TLB_ENTRY_SIZE

    mtspr       SPR_MMUCR,  r28
    tlbwe       r29,r31,MMU_TLB_ENTRY_TAG
    tlbwe       r30,r31,MMU_TLB_ENTRY_DATA
    tlbwe       r31,r31,MMU_TLB_ENTRY_ATTR

    subic.      r4, r4, 1
    bne+        write_next_etry

write_tlb_entries_exit:
    mfspr       r3, SPR_SRR0
    mfspr       r4, SPR_SRR1

    mfmsr       r31
    mtspr       SPR_SRR1, r31
    load_addr   r31, write_tlb_entries_return
    mtspr       SPR_SRR0, r31

    rfi

write_tlb_entries_return:
    mtspr       SPR_SRR1, r4
    mtspr       SPR_SRR0, r3

    mr          r28,r8
    mr          r29,r9
    mr          r30,r10
    mr          r31,r11

// did commented, 09.09.20
//    blr

    rumboot_putstring       "Run subtests...\n"
    set_cr      cr6,        eq

    rumboot_putstring       "Filling UTLB-entries...\n"

    fill_utlb   MMU_WAY_0,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR0,  /*  EPN BEGIN   */ \
                END_ADDR0,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INIT1,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_1,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR1,  /*  EPN BEGIN   */ \
                END_ADDR1,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INIT0,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_2,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR2,  /*  EPN BEGIN   */ \
                END_ADDR2,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INIT0,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_3,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR3,  /*  EPN BEGIN   */ \
                END_ADDR3,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INIT0,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

fill_end:
    rumboot_putstring       "Filling UTLB-entries complete.\n"

    mfspr       R_SAV0,     SPR_SRR0
    mfspr       R_SAV1,     SPR_SRR1

    mfmsr       R_DATA
    mtspr       SPR_SRR1,   R_DATA
    load_addr   R_ADDR,     read_pages_start
    mtspr       SPR_SRR0,   R_ADDR

    rfi

test_end:
    bne- cr6,   test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event  EVENT_OK
    exit        RESULT_OK
    b           test_finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    exit RESULT_ERROR

test_finish:
    b           test_finish

fill_utlb_loop:
    mr          R_TLBR,     R_TPL0
    load_const  R_SHFT,     24
    mtspr       SPR_MMUCR,  R_MMUC
    and         R_DATA,     R_BEGA,     R_MASK
    srw         R_TEMP,     R_DATA,     R_SHFT
    and         R_TEMP,     R_TEMP,     R_XORM
    xor         R_TEMP,     R_TEMP,     R_XORM
    addi        R_SHFT,     R_SHFT,     -8
    slw         R_TEMP,     R_TEMP,     R_SHFT
    xor         R_DATA,     R_DATA,     R_TEMP
    or          R_DATA,     R_DATA,     R_ETAG
    tlbwe       R_DATA,     R_TLBR,     MMU_TLB_ENTRY_TAG
    mfspr       R_TEMP,     SPR_MMUCR
    addi        R_SHFT,     R_SHFT,     1
    srw         R_TEMP,     R_TEMP,     R_SHFT
    and         R_TEMP,     R_TEMP,     R_XORM
    cmpw  cr7,  R_TEMP,     R_CNTR
    TEST_ASSERT(eq,cr7,"UTLB-Entry index is invalid!")

    mr          R_TLBR,     R_TPL0
    mr          R_DATA,     R_FILL
    mtspr       SPR_MMUCR,  R_MMUC
    tlbwe       R_DATA,     R_TLBR,     MMU_TLB_ENTRY_DATA
    mr          R_TLBR,     R_TPL0
    mtspr       SPR_MMUCR,  R_MMUC
    mr          R_DATA,     R_ATTR
    tlbwe       R_DATA,     R_TLBR,     MMU_TLB_ENTRY_ATTR
    cmpw  cr2,  R_BEGA,     R_ENDA
    add         R_BEGA,     R_BEGA,     R_STEP
    addi        R_CNTR,     R_CNTR,     0x0001
    bne+  cr2,  fill_utlb_loop
    rumboot_putstring       "Filled.\n"
    isync
    blr

read_pages_loop:
    lwzx        R_DATA,     R_BEGA,     R_AIDX
    cmpw  cr7,  R_EXPV,     R_DATA
    TEST_ASSERT(eq,cr7,"Read fail!")
    cmpw  cr2,  R_BEGA,     R_ENDA
    add         R_BEGA,     R_BEGA,     R_STEP
    bne+  cr2,  read_pages_loop
    rumboot_putstring       "Read done.\n"
    blr

read_pages_start:

    rumboot_putstring   "Check read memory pages...\n"

    check_read_pages    BEG_ADDR3,  END_ADDR3,  READ_STEP,  VAL_MAGIC
    check_read_pages    BEG_ADDR2,  END_ADDR2,  READ_STEP,  VAL_MAGIC
    check_read_pages    BEG_ADDR1,  END_ADDR1,  READ_STEP,  VAL_MAGIC
    check_read_pages    BEG_ADDRX,  END_ADDR0,  READ_STEP,  VAL_MAGIC

    rumboot_putstring   "complete.\n"

    b test_end


