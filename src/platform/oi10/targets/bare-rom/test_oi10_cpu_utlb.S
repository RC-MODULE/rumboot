
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>


#define VAL_MAGIC   0xDEAD4BAD
#define BEG_ADDR0   0xFFFE0000  /* Skip last 64k page   */
#define END_ADDR0   0xFF000000
#define BEG_ADDR1   0xFEFF0000  /* 11111110_11111111    */
#define END_ADDR1   0xFE000000  /* 11111110_00000000    */
#define BEG_ADDR2   0xFDFF0000  /* 11111101_11111111    */
#define END_ADDR2   0xFD000000  /* 11111101_00000000    */
#define BEG_ADDR3   0xFBFF0000  /* 11111011_11111111    */
#define END_ADDR3   0xFB000000  /* 11111011_00000000    */
#define PHY_ADDR0   0xFFFF0000
#define PHY_ADDRE   0x00003FFF

#define WAY_0       0
#define WAY_1       1
#define WAY_2       2
#define WAY_3       3

#define CONST_CCR0    reg_field( 0, 0b0   ) /* ITE     */ \
                    | reg_field( 1, 0b1   ) /* PRE     */ \
                    | reg_field( 4, 0b1   ) /* CRPE    */ \
                    | reg_field(10, 0b0   ) /* ICS     */ \
                    | reg_field(11, 0b0   ) /* DAPUIB  */ \
                    | reg_field(15, 0b0000) /* ICWRIDX */ \
                    | reg_field(16, 0b0   ) /* DTB     */ \
                    | reg_field(23, 0b0   ) /* FLSTA   */ \
                    | reg_field(29, 0b00  ) /* DQWPM   */ \
                    | reg_field(30, 0b1   ) /* IQWPM   */

#define CONST_CCR1    reg_field( 1, 0b00  ) /* GPRPEI  */ \
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

#define CONST_CCR2    reg_field( 1, 0b00  ) /* DSTG    */ \
                    | reg_field( 2, 0b0   ) /* DLFPD   */ \
                    | reg_field( 4, 0b0   ) /* DSTI    */ \
                    | reg_field( 9, 0b0   ) /* PMUD    */ \
                    | reg_field(11, 0b0   ) /* DCSTGW  */ \
                    | reg_field(15, 0b0000) /* STGCTR  */ \
                    | reg_field(16, 0b0   ) /* DISTG   */ \
                    | reg_field(20, 0b0   ) /* SPC5C1  */ \
                    | reg_field(21, 0b0   ) /* MCDTO   */

#define CONST_MMUBE0  (0b0 << MMU_MMUBE0_VBE0_i) \
                    | (0b0 << MMU_MMUBE0_VBE1_i) \
                    | (0b0 << MMU_MMUBE0_VBE2_i)

#define CONST_MMUBE1  (0b0 << MMU_MMUBE1_VBE3_i) \
                    | (0b0 << MMU_MMUBE1_VBE4_i) \
                    | (0b0 << MMU_MMUBE1_VBE5_i)

#define CONST_SSPCR   ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) \
                    | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB  ) << MMU_SSPCR_ORD4_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD5_i)

#define CONST_ISPCR   ((                          MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((                          MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((                          MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) \
                    | ((                          MMU_XSPCR_ORD_1GB  ) << MMU_SSPCR_ORD4_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD5_i)

#define CONST_USPCR   ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                    | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) \
                    | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB  ) << MMU_SSPCR_ORD4_i) \
                    | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD5_i)

#define FILL_ATTR   MMU_TLBE_ATTR(  1, 1,               /* IL1I, IL1D       */  \
                                    0, 1, 0, 1,         /* W, I, M, G, E    */  \
                                    MMU_TLBE_E_BIG_END, /* Endianness       */  \
                                    1, 0, 1,            /* UX, UW, UR       */  \
                                    1, 0, 1,            /* SX, SW, SR       */  \
                                    0, 0)
#define FILL_STEP   (0 - 65536)

#define R_STKP      r1
#define R_ARG0      r3
#define R_ARG1      r4
#define R_ARG2      r5
#define R_ARG3      r6
#define R_MMUC      r14
#define R_TLBR      r15
#define R_ETAG      r16
#define R_MASK      r17
#define R_DUMP      r18
#define R_ADDR      r20
#define R_BEGA      R_ADDR
#define R_ENDA      r21
#define R_STEP      r22
#define R_FILL      r23
#define R_DATA      r24
#define R_SAV0      r26
#define R_SAV1      r27
#define R_SHFT      R_SAV0
#define R_XORM      R_SAV1
#define R_TEMP      r29
#define R_ZERO      r30
#define R_ONES      r31

.macro fill_utlb vWay, vAttr, aBeg, aEnd, aStep, vFill, vEFill
    rumboot_putstring       "Fill UTLB-entries for way \vWay from \aBeg to \aEnd\()...\n"
    load_const  R_MASK,     0xFFFF0000
    load_const  R_MMUC,     MMU_MMUCR(0,0,0,0,0)
    load_const  R_BEGA,     \aBeg
    load_const  R_ENDA,     \aEnd
    load_const  R_STEP,     \aStep
    load_const  R_FILL,     MMU_TLBE_DATA(\vEFill, \vFill, 0, 0)
    load_const  R_ETAG,     MMU_TLBE_TAG(0,1,0,MMU_TLBE_DSIZ_64KB,0,0,0,0)
52:
    load_const  R_TLBR,     ((\vWay) << MMU_TLBWE_RA_WAY_i)
    load_const  R_SHFT,     24
    load_const  R_XORM,     0x000F
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
    load_const  R_TLBR,     ((\vWay) << MMU_TLBWE_RA_WAY_i)
    mtspr       SPR_MMUCR,  R_MMUC
    tlbwe       R_FILL,     R_TLBR,     MMU_TLB_ENTRY_DATA
    load_const  R_TLBR,     ((\vWay) << MMU_TLBWE_RA_WAY_i)
    mtspr       SPR_MMUCR,  R_MMUC
    load_const  R_DATA,     \vAttr
    tlbwe       R_DATA,     R_TLBR,     MMU_TLB_ENTRY_ATTR
    cmpw  cr7,  R_BEGA,     R_ENDA
    add         R_BEGA,     R_BEGA,     R_STEP
    bne+  cr7,  52b
    rumboot_putstring       "Filled.\n"
.endm

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

rumboot_entry_point:
    gtube_init

    /* Clear listed GP registers */
    .irp RN,    2,  3,  4,  5,  6,  7,  8,  9,  10, 11, \
                12, 13, 14, 15, 16, 17, 18, 19, 20, 21, \
                22, 23, 24, 25, 26, 27, 28, 29, 30
        load_const  \RN,    0x00000000
    .endr



    rumboot_putstring "Init SPRs...\n"
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0, R_ZERO   /* DBCR0 clear */

/* Setup DBSR */
    mfspr       R_TEMP, SPR_DBSR_RC     /* read current DBSR    */
    mtspr       SPR_DBSR_RC, R_TEMP     /* clear all valid bits */

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

    .irp        nSPR,       lr,cr,ctr,xer
        mt\nSPR             R_ZERO
    .endr


    mtspr       SPR_MCSR_C, R_ONES
    mtspr       SPR_TSR_RC, R_ONES

    b           setup_tlb

.align 2
tlb_entries:
/*          MMU_TLB_ENTRY(ERPN,  RPN,     EPN,     DSIZ,               IL1I,IL1D, W,I,M,G, E,                  UX, UW, UR,  SX, SW, SR   DULXE,IULXE, TS,  TID,               WAY,             BID,              V   ) */
/* Invalidate initial TLB entry */
    .long   MMU_TLB_ENTRY(0x000, 0x00000, 0xFFFFF, MMU_TLBE_DSIZ_4KB,  0b0, 0b0,  0,0,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b0,0b0,0b0, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0 )
/* TLB entry to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff */
    .long   MMU_TLB_ENTRY(0x3FF, 0xFFFF0, 0xFFFF0, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b1 )


utlb_entry_tpl:
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b0 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_1, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_2, MMU_TLBWE_BE_UND, 0b1 )
    .long   MMU_TLB_ENTRY(0x3BE, 0xDEAD7, 0xBEAF6, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,1, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b1 )
    .long   0xAA55AA55
    .long   0x55AA55AA



setup_tlb:
    rumboot_putstring   "Setup TLB...\n"
    load_addr   R_ARG0, tlb_entries
    load_const  R_ARG1, 0x0002
    mfspr       R_TEMP, SPR_RSTCFG
    addi        R_ARG0, R_ARG0, -4

write_tlb_entries_loop:
    cmpwi cr7,  R_ARG1, 0x0000
    beq-  cr7,  write_tlb_entries_end

    /* entry[0] -> MMUCR */
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    mtspr       SPR_MMUCR, R_DATA

    /* entry[1] -> WTF? */
    lwzu        R_TLBR, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)

    /* entry[2] -> ETAG    (WS=0) */
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    tlbwe       R_DATA, R_TLBR, MMU_TLB_ENTRY_TAG

    /* entry[3] -> EDATA   (WS=1) */
    lwzu        R_DATA, MMU_TLB_ENTRY_FIELD_SIZE(R_ARG0)
    tlbwe       R_DATA, R_TLBR, MMU_TLB_ENTRY_DATA

    /* entry[4] ->EATTR    (WS=2) */
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

    rumboot_putstring       "Filling UTLB-entries...\n"

    fill_utlb   WAY_0,      /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR0,  /*  EPN BEGIN   */ \
                END_ADDR0,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   WAY_1,      /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR1,  /*  EPN BEGIN   */ \
                END_ADDR1,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   WAY_2,      /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR2,  /*  EPN BEGIN   */ \
                END_ADDR2,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   WAY_3,      /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR3,  /*  EPN BEGIN   */ \
                END_ADDR3,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

fill_end:
    rumboot_putstring       "Filling UTLB-entries complete.\n"

    b           test_start

    mfspr       R_SAV0,     SPR_SRR0
    mfspr       R_SAV1,     SPR_SRR1

    mfmsr       R_DATA
    mtspr       SPR_SRR1,   R_DATA
    load_addr   R_ADDR,     test_start
    mtspr       SPR_SRR0,   R_ADDR

    rfi

test_end:
    bne- cr6,   test_error

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

.section ".text","ax",@progbits

magic_data:
    .long VAL_MAGIC

test_start:

    rumboot_putstring   "TEST IS NOT READY!!!\n"
    clr_cr      cr6,    eq
    b test_end





