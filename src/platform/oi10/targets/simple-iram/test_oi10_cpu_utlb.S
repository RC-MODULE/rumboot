
#define DEBUG_CONTINUE_ON_ASSERT

#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
/* CCR0, CCR1, CCR2 fields */
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
/* MSR fields */
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>

/* Data sizes */
#define SIZEOF_BYTE     0x0001
#define SIZEOF_CHAR     SIZEOF_BYTE
#define SIZEOF_HWORD    0x0002
#define SIZEOF_WCHAR    SIZEOF_HWORD
#define SIZEOF_WORD     0x0004
#define SIZEOF_LONG     SIZEOF_WORD
#define SIZEOF_DWORD    0x0008
#define SIZEOF_FLOAT    SIZEOF_WORD
#define SIZEOF_DOUBLE   SIZEOF_DWORD
#define SIZEOF_SINGLE   SIZEOF_FLOAT
#define SIZEOF_ADDR     SIZEOF_WORD

#define reg_field32     reg_field

#define VAL_MAGIC       0xBED4DEAD

#define BEG_ADDRA       0xFFFF0000  /* 11111111_11111111        */
#define END_ADDRA       0xFF800000  /* 11111111_10000000        */
/* Skip 2 entries for addresses 0x80000000-0x8001FFFF (way 0)   */
#define BEG_ADDRB       0xFF7D0000  /* 11111111_01111101        */
#define END_ADDRB       0xFF000000  /* 11111111_00000000        */

#define BEG_ADDR1       0xFEFF0000  /* 11111110_11111111        */
#define END_ADDR1       0xFE000000  /* 11111110_00000000        */

#define BEG_ADDR2       0xFDFF0000  /* 11111101_11111111        */
#define END_ADDR2       0xFD000000  /* 11111101_00000000        */

#define BEG_ADDR3       0xFBFF0000  /* 11111011_11111111        */
#define END_ADDR3       0xFB000000  /* 11111011_00000000        */

#define PHY_ADDR0       0x80000000
#define PHY_ADDRE       0x00000010

#define CNT_INI00       0x00000000
#define CNT_INI01       0x00000001
#define CNT_INI82       0x00000082

#define MMU_WAY_0       MMU_TLBWE_WAY_0
#define MMU_WAY_1       MMU_TLBWE_WAY_1
#define MMU_WAY_2       MMU_TLBWE_WAY_2
#define MMU_WAY_3       MMU_TLBWE_WAY_3

#define FILL_STEP       (0 - 65536)
#define READ_STEP       (0 + 65536)

#define R_STKP          r1          /* Stack pointer                    */
#define R_ARG0          r3          /* Standart argument #0             */
#define R_ARG1          r4          /* Standart argument #1             */
#define R_ARG2          r5          /* Standart argument #2             */
#define R_ARG3          r6          /* Standart argument #3             */
#define R_RETV          R_ARG0      /* Standart argument #3             */
#define R_ATTR          r13         /* UTLB Attributes                  */
#define R_MMUC          r14         /* Holds MMURC                      */
#define R_TLBR          r15         /* Holds tlbwe second parameter     */
#define R_ETAG          r16         /* Holds UTLB Entry TAG             */
#define R_MASK          r17         /* Address mask                     */
#define R_DUMP          r18         /* Dump value for debug purposes    */
#define R_CNTR          r19         /* Counter (for UTLB index compare) */
#define R_AIDX          R_CNTR      /* Offset in page table             */
#define R_ADDR          r20         /* Uniform address pointer          */
#define R_BEGA          R_ADDR      /* Begin address                    */
#define R_ENDA          r21         /* End address                      */
#define R_STEP          r22         /* Step                             */
#define R_FILL          r23         /* Fill value                       */
#define R_EXPV          R_FILL      /* Expected value                   */
#define R_DATA          r24         /* Readed or output data            */
#define R_SAV0          r26         /* Copy of SRR0                     */
#define R_SAV1          r27         /* Copy of SRR1                     */
#define R_SHFT          R_SAV0      /* Shift value                      */
#define R_XORM          R_SAV1      /* XOR mask                         */
#define R_TPL0          r28         /* Template for tlbwe second arg    */
#define R_TEMP          r29         /* Temporary data                   */
#define R_ZERO          r30         /* 0x00000000                       */
#define R_ONES          r31         /* 0xFFFFFFFF                       */


#define CONST_MMUBE0      (0b0 << MMU_MMUBE0_VBE0_i) \
                        | (0b1 << MMU_MMUBE0_VBE1_i) \
                        | (0b1 << MMU_MMUBE0_VBE2_i)

#define CONST_MMUBE1      (0b0 << MMU_MMUBE1_VBE3_i) \
                        | (0b0 << MMU_MMUBE1_VBE4_i) \
                        | (0b0 << MMU_MMUBE1_VBE5_i)

#define CONST_SSPCR       ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                        | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define CONST_ISPCR       ((                          MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                        | ((                          MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                        | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define CONST_USPCR       ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_64KB ) << MMU_SSPCR_ORD1_i) \
                        | ((MMU_SUSPCR_ORD_SHARED   | MMU_XSPCR_ORD_4KB  ) << MMU_SSPCR_ORD2_i) \
                        | ((                          MMU_XSPCR_ORD_END  ) << MMU_SSPCR_ORD3_i)

#define FILL_ATTR       MMU_TLBE_ATTR(  1, 1,               /* IL1I, IL1D   */  \
                                        0, 1, 0, 0,         /* W, I, M, G,  */  \
                                        MMU_TLBE_E_BIG_END, /* Endianness   */  \
                                        1, 0, 1,            /* UX, UW, UR   */  \
                                        1, 0, 1,            /* SX, SW, SR   */  \
                                        0, 0)

.macro fill_utlb vWay, vAttr, aBeg, aEnd, aStep, vCntInit, vFill, vEFill
fill_\()\aBeg\()_\()\aEnd\():   /* For debug purpose */
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
    load_const  R_TPL0,     ((\vWay) << MMU_TLBWE_RA_WAY_i)
    load_const  R_ATTR,     \vAttr
    bl          fill_utlb_loop
.endm

.macro check_read_pages aEnd, aBeg, aStep, vExp
read_\()\aBeg\()_\()\aEnd\():   /* For debug purpose */
    rumboot_putstring       "Read from \aBeg to \aEnd\()...\n"
    load_const  R_BEGA,     \aBeg
    load_const  R_ENDA,     \aEnd
    load_const  R_STEP,     \aStep
    load_const  R_EXPV,     \vExp
//  load_const  R_AIDX,     0xF000
    li          R_AIDX,     magic_data@l
    bl          read_pages_loop
.endm

.macro push_spr spReg rTmp=R_TEMP
    mf\spReg    \rTmp
    stwu        \rTmp,  (-SIZEOF_WORD)(R_STKP)
.endm

.macro push_sprn spReg rTmp=R_TEMP
    mfspr       \rTmp,  \spReg
    stwu        \rTmp,  (-SIZEOF_WORD)(R_STKP)
.endm

.macro pop_spr spReg rTmp=R_TEMP
    lwz         \rTmp,  0(R_STKP)
    addi        R_STKP, R_STKP, SIZEOF_WORD
    mt\spReg    \rTmp
.endm

.macro pop_sprn spReg rTmp=R_TEMP
    lwz         \rTmp,  0(R_STKP)
    addi        R_STKP, R_STKP, SIZEOF_WORD
    mtspr       \spReg, \rTmp
.endm

.section ".text","ax",@progbits

.global main
main:

    load_const  R_ZERO, 0x00000000
    nand        R_ONES, R_ZERO, R_ZERO
    push_spr    lr

    /* Clear listed GP registers */
    .irp RN,    3,  4,  5,  6,  7,  8,  9,  10, 11, \
                12, 13, 14, 15, 16, 17, 18, 19, 20, \
                21, 22, 23, 24, 25, 26, 27, 28, 29
        mr      \RN,    R_ZERO
    .endr

    b           setup_tlb

.align 2
magic_data:
    .long VAL_MAGIC

/*
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
*/

.align 2
tlb_entries:
/*          MMU_TLB_ENTRY(ERPN,  RPN,     EPN,     DSIZ,               IL1I,IL1D, W,I,M,G, E,                  UX, UW, UR,  SX, SW, SR   DULXE,IULXE, TS,  TID,               WAY,             BID,              V   ) */
/* Invalidate TLB entries of remap 128KB pages IM1 from 0x00000020_C0000000-0x00000020_C001FFFF to 0x80020000-0x8003FFFF*/
    .long   MMU_TLB_ENTRY(0x01F, 0xFFFF0, 0xFFFF0, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b0,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_0,   0b0 )
    .long   MMU_TLB_ENTRY(0x020, 0xC0000, 0x80020, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b1,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_3,   0b0 )
    .long   MMU_TLB_ENTRY(0x020, 0xC0010, 0x80030, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b1,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_4,   0b0 )
    .long   MMU_TLB_ENTRY(0x020, 0xC0040, 0x80040, MMU_TLBE_DSIZ_64KB, 0b1, 0b1,  0,1,0,0, MMU_TLBE_E_BIG_END, 0b0,0b0,0b0, 0b1,0b1,0b1, 0b0,  0b0,   0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_5,   0b0 )
.set tlb_entries_count, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE


setup_tlb:
    rumboot_putstring   "Invalidate bolted TLB entries...\n"
    load_addr   R_ARG0, tlb_entries
    load_const  R_ARG1, tlb_entries_count
    bl          write_tlb_entries
    push_sprn   SPR_MMUBE1
    push_sprn   SPR_MMUBE0
    load_spr    R_TEMP, SPR_MMUBE0,   CONST_MMUBE0,
    load_spr    R_TEMP, SPR_MMUBE1,   CONST_MMUBE1,

    rumboot_putstring       "Run subtests...\n"
    set_cr      cr6,        eq

    rumboot_putstring       "Filling UTLB-entries...\n"

    fill_utlb   MMU_WAY_0,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDRA,  /*  EPN BEGIN   */ \
                END_ADDRA,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INI00,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_0,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDRB,  /*  EPN BEGIN   */ \
                END_ADDRB,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INI82,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_1,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR1,  /*  EPN BEGIN   */ \
                END_ADDR1,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INI00,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_2,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR2,  /*  EPN BEGIN   */ \
                END_ADDR2,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INI00,  /*  CNT INIT    */ \
                PHY_ADDR0,  /*  RPN FILL    */ \
                PHY_ADDRE   /* eRPN FILL    */

    fill_utlb   MMU_WAY_3,  /*  WAY         */ \
                FILL_ATTR,  /*  TLB ATTR    */ \
                BEG_ADDR3,  /*  EPN BEGIN   */ \
                END_ADDR3,  /*  EPN END     */ \
                FILL_STEP,  /*  EPN STEP    */ \
                CNT_INI00,  /*  CNT INIT    */ \
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
    pop_sprn    SPR_MMUBE0
    pop_sprn    SPR_MMUBE1
    pop_spr     lr
    bne- cr6,   test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    load_const  R_RETV, RESULT_OK
    blr

test_error:
    rumboot_putstring "TEST ERROR\n"
    load_const  R_RETV, RESULT_ERROR
    blr


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

    /* Way 3 */
    check_read_pages    BEG_ADDR3,  END_ADDR3,  READ_STEP,  VAL_MAGIC
    /* Way 2 */
    check_read_pages    BEG_ADDR2,  END_ADDR2,  READ_STEP,  VAL_MAGIC
    /* Way 1 */
    check_read_pages    BEG_ADDR1,  END_ADDR1,  READ_STEP,  VAL_MAGIC
    /* Way 0 */
    check_read_pages    BEG_ADDRB,  END_ADDRB,  READ_STEP,  VAL_MAGIC
    /* Way 0 */
    check_read_pages    BEG_ADDRA,  END_ADDRA,  READ_STEP,  VAL_MAGIC

    rumboot_putstring   "complete.\n"

    b test_end


