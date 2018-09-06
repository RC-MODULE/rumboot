
#define RUMBOOT_ASSERT_WARN_ONLY

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_event_codes.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_fpu_const.h>
#include <platform/trace.S.h>
#include <platform/devices.h>


/* Config */
#define DUMP_ADDR   save_data

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

/* Integer register aliases */
#define R_STAK          r1      /* Stack pointer            */
#define R_ARG0          r3      /* Argument parameter #0    */
#define R_ARG1          r4      /* Argument parameter #1    */
#define R_ARG2          r5      /* Argument parameter #2    */
#define R_ARG3          r6      /* Argument parameter #3    */
#define R_RETV          R_ARG0  /* Return value             */
#define R_DMPA          r15     /* Dump memory address      */
#define R_DMPB          r16     /* Dump bytes count         */
#define R_DMPC          r17     /* Dump temp data           */
#define R_MSRC          r18     /* MSR copy                 */
#define R_BACK          r19     /* Unified backup           */
#define R_ADDR          r20     /* Unified address          */
#define R_CONI          R_ADDR  /* Integer constants ptr    */
#define R_CONF          R_ADDR  /* FP constants ptr         */
#define R_VARS          R_ADDR  /* Variables address        */
#define R_SIZE          r21     /* Size                     */
#define R_CNTR          R_SIZE  /* Counter                  */
#define R_DATA          r22     /* Unified data             */
#define R_OFFT          r23     /* Offset (Target)          */
#define R_OFFS          r24     /* Offset (Source)          */
#define R_EXPV          r25     /* Expecting value          */
#define R_OPR1          r26     /* Unified first operand    */
#define R_OPR2          R_OFFS  /* Unified second operand   */
#define R_MASK          R_OFFS  /* Unified mask value       */
#define R_TEMP          R_DMPC  /* Temporary data           */
#define R_STKB          r27     /* Stack backup             */
#define R_SAV0          r28     /* Copy of SRR0             */
#define R_SAV1          r29     /* Copy of SRR1             */
#define R_ZERO          r30     /* Holds 0x00000000         */
#define R_ONES          r31     /* Holds 0xFFFFFFFF         */

/* Floating point register aliases */
#define FR_NUL          f0      /* Holds 0x0000000000000000 */
#define FR_ONE          f1      /* Holds 0xFFFFFFFFFFFFFFFF */
#define FR_SCR          f8      /* Holds FPSCR              */

#define CONST32_1       0xA5A55A5A
#define CONST32_2       0x3C3CC3C3
#define CONST16_1       0x0000FFA5
#define CONST16_2       0x00005A3C
#define CONST8_1        0x0000005A
#define CONST8_2        0x000000C3

#define FOURCC(A,B,C,D) (((A) << 24) | ((B) <<16) | ((C) << 8) | (D))

/* Dump address pointer must 64 bit aligned */
.macro dump_write_uint32 sRhi, sRlo
    stw     \sRhi,  0(R_DMPA)
    stw     \sRlo,  4(R_DMPA)
    addi    R_DMPA, R_DMPA, SIZEOF_DWORD
    addi    R_DMPB, R_DMPB, SIZEOF_DWORD
.endm

.macro dump_write_single sFR
    /* Dump address pointer must 64 bit aligned */
    stw     R_ZERO, 0(R_DMPA)
    stfs    \sFR,   4(R_DMPA)
    addi    R_DMPA, R_DMPA, SIZEOF_DOUBLE
    addi    R_DMPB, R_DMPB, SIZEOF_DOUBLE
.endm

.macro dump_write_double sFR
    stfd    \sFR,   0(R_DMPA)
    addi    R_DMPA, R_DMPA, SIZEOF_DOUBLE
    addi    R_DMPB, R_DMPB, SIZEOF_DOUBLE
.endm

.macro dump_write_fpscr
    mffs    FR_SCR
    stfd    FR_SCR, 0(R_DMPA)
    addi    R_DMPA, R_DMPA, SIZEOF_DOUBLE
    addi    R_DMPB, R_DMPB, SIZEOF_DOUBLE
.endm

.macro fpscr_reset
    mtfsf   0xFF,   FR_NUL
.endm

.macro save_srr
    mfsrr0      R_SAV0
    mfsrr1      R_SAV1
.endm

.macro rest_srr
    mtsrr0      R_SAV0
    mtsrr1      R_SAV1
.endm

.macro load_srr rADDR rMSR
    save_srr
    mtsrr0      \rADDR
    mtsrr1      \rMSR
.endm

.macro check_loadi cmd, offset, expVal
    rumboot_putstring   "\cmd\()...\n"
    load_const  R_EXPV, \expVal
    \cmd        R_DATA, \offset(R_CONI)
    dump_write_uint32   R_EXPV, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    TEST_ASSERT(eq, cr2, "\cmd fails.\n")
.endm

.macro check_loadui cmd, offset, expVal
    rumboot_putstring   "\cmd\()...\n"
    load_const  R_EXPV, \expVal
    mr          R_BACK, R_CONI
    \cmd        R_DATA, \offset(R_CONI)
    dump_write_uint32   R_CONI, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    addi        R_EXPV, R_BACK, \offset
    cmpw  cr3,  R_CONI, R_EXPV
    mr          R_CONI, R_BACK
    TEST_ASSERT(eq, cr2, "\cmd fails.\n")
    TEST_ASSERT(eq, cr3, "\cmd autoupdate fails.\n")
.endm

.macro check_loadx cmd, offset, expVal
    rumboot_putstring   "\cmd\()...\n"
    load_const  R_OFFS, \offset
    load_const  R_EXPV, \expVal
    \cmd        R_DATA, R_CONI, R_OFFS
    dump_write_uint32   R_EXPV, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    TEST_ASSERT(eq, cr2, "\cmd fails.\n")
.endm

.macro check_loadux cmd, offset, expVal
    rumboot_putstring   "\cmd\()...\n"
    load_const  R_EXPV, \expVal
    load_const  R_OFFS, \offset
    mr          R_BACK, R_CONI
    \cmd        R_DATA, R_CONI, R_OFFS
    dump_write_uint32   R_CONI, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    addi        R_EXPV, R_BACK, \offset
    cmpw  cr3,  R_CONI, R_EXPV
    mr          R_CONI, R_BACK
    TEST_ASSERT(eq, cr2, "\cmd fails.\n")
    TEST_ASSERT(eq, cr3, "\cmd autoupdate fails.\n")
.endm

.macro check_storei stCmd, stOffset, stVal, ldOffset, expVal
    rumboot_putstring   "\stCmd\()...\n"
    load_const  R_EXPV, \expVal
    load_const  R_TEMP, \stVal
    stw         R_ZERO, \ldOffset(R_VARS)
    \stCmd      R_TEMP, \stOffset(R_VARS)
    lwz         R_DATA, \ldOffset(R_VARS)
    dump_write_uint32   R_EXPV, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    TEST_ASSERT(eq, cr2, "\stCmd fails.\n")
.endm

.macro check_storeui stCmd, stOffset, stVal, ldOffset, expVal
    rumboot_putstring   "\stCmd\()...\n"
    load_const  R_EXPV, \expVal
    load_const  R_TEMP, \stVal
    mr          R_BACK, R_VARS
    stw         R_ZERO, \ldOffset(R_BACK)
    \stCmd      R_TEMP, \stOffset(R_VARS)
    lwz         R_DATA, \ldOffset(R_BACK)
    dump_write_uint32   R_VARS, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    addi        R_EXPV, R_BACK, \stOffset
    cmpw  cr3,  R_VARS, R_EXPV
    mr          R_VARS, R_BACK
    TEST_ASSERT(eq, cr2, "\stCmd fails.\n")
    TEST_ASSERT(eq, cr3, "\stCmd autoupdate fails.\n")
.endm

.macro check_storex stCmd, stOffset, stVal, ldOffset, expVal
    rumboot_putstring   "\stCmd\()...\n"
    load_const  R_EXPV, \expVal
    load_const  R_TEMP, \stVal
    load_const  R_OFFT, \stOffset
    load_const  R_OFFS, \ldOffset
    stwx        R_ZERO, R_VARS, R_OFFS
    \stCmd      R_TEMP, R_VARS, R_OFFT
    lwzx        R_DATA, R_VARS, R_OFFS
    dump_write_uint32   R_EXPV, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    TEST_ASSERT(eq, cr2, "\stCmd fails.\n")
.endm

.macro check_storeux stCmd, stOffset, stVal, ldOffset, expVal
    rumboot_putstring   "\stCmd\()...\n"
    load_const  R_EXPV, \expVal
    load_const  R_TEMP, \stVal
    mr          R_BACK, R_VARS
    load_const  R_OFFT, \stOffset
    load_const  R_OFFS, \ldOffset
    stwx        R_ZERO, R_VARS, R_OFFS
    \stCmd      R_TEMP, R_VARS, R_OFFT
    lwzx        R_DATA, R_BACK, R_OFFS
    dump_write_uint32   R_VARS, R_DATA
    cmpw  cr2,  R_DATA, R_EXPV
    addi        R_EXPV, R_BACK, \stOffset
    cmpw  cr3,  R_VARS, R_EXPV
    mr          R_VARS, R_BACK
    TEST_ASSERT(eq, cr2, "\stCmd fails.\n")
    TEST_ASSERT(eq, cr3, "\stCmd autoupdate fails.\n")
.endm

.macro check_load_8 offset, expVal
    check_loadi     lbz,    \offset, \expVal
    check_loadx     lbzx,   \offset, \expVal
    check_loadui    lbzu,   \offset, \expVal
    check_loadux    lbzux,  \offset, \expVal
.endm

.macro check_load_16 offset, expVal
    check_loadi     lhz,    \offset, \expVal
    check_loadx     lhzx,   \offset, \expVal
    check_loadui    lhzu,   \offset, \expVal
    check_loadux    lhzux,  \offset, \expVal
.endm

.macro check_load_32 offset, expVal
    check_loadi     lwz,    \offset, \expVal
    check_loadx     lwzx,   \offset, \expVal
    check_loadui    lwzu,   \offset, \expVal
    check_loadux    lwzux,  \offset, \expVal
.endm

.macro check_store_8 stOffset, stVal, ldOffset, expVal
    check_storei    stb,    \stOffset + 0x00, \stVal,   \
                            \ldOffset + 0x00, \expVal
    check_storex    stbx,   \stOffset + 0x04, \stVal,   \
                            \ldOffset + 0x04, \expVal
    check_storeui   stbu,   \stOffset + 0x08, \stVal,   \
                            \ldOffset + 0x08, \expVal
    check_storeux   stbux,  \stOffset + 0x0C, \stVal,   \
                            \ldOffset + 0x0C, \expVal
.endm

.macro check_store_16 stOffset, stVal, ldOffset, expVal
    check_storei    sth,    \stOffset + 0x00, \stVal,   \
                            \ldOffset + 0x00, \expVal
    check_storex    sthx,   \stOffset + 0x04, \stVal,   \
                            \ldOffset + 0x04, \expVal
    check_storeui   sthu,   \stOffset + 0x08, \stVal,   \
                            \ldOffset + 0x08, \expVal
    check_storeux   sthux,  \stOffset + 0x0C, \stVal,   \
                            \ldOffset + 0x0C, \expVal
.endm

.macro check_store_32 stOffset, stVal, ldOffset, expVal
    check_storei    stw,    \stOffset + 0x00, \stVal,   \
                            \ldOffset + 0x00, \expVal
    check_storex    stwx,   \stOffset + 0x04, \stVal,   \
                            \ldOffset + 0x04, \expVal
    check_storeui   stwu,   \stOffset + 0x08, \stVal,   \
                            \ldOffset + 0x08, \expVal
    check_storeux   stwux,  \stOffset + 0x0C, \stVal,   \
                            \ldOffset + 0x0C, \expVal
.endm


.section ".text","ax",@progbits

.global main

main:
    save_stack  R_STKB
    test_event_send_test_id "test_iss_base"

    rumboot_putstring "Init test...\n"
    set_cr      cr6,    eq
    load_const  R_ZERO, 0x00000000
    load_const  R_ONES, 0xFFFFFFFF
    load_addr   R_DMPA, save_data
    mr          R_DMPB, R_ZERO
    mfmsr       R_MSRC
    rumboot_putstring "Turning FPU ON...\n"
    load_addr   R_ADDR, main_init
    load_const  R_TEMP, (0b1 << ITRPT_XSR_FP_i)
    or          R_DATA, R_MSRC, R_TEMP
    mtxer       R_ZERO
    load_srr    R_ADDR, R_DATA
    rfi

main_init:
    rest_srr

    rumboot_putstring   "Init FPU registers...\n"
    load_addr   R_CONF, fp_data
    lfd         FR_NUL, 40(R_CONF)
    lfd         FR_ONE, 48(R_CONF)
    .irp        frN,    2,  3,  4,  5,  6,  7,  \
                        8,  9,  10, 11, 12, 13, \
                        14, 15, 16, 17, 18, 19, \
                        20, 21, 22, 23, 24, 25, \
                        26, 27, 28, 29, 30, 31, \
        fmr     \frN,   FR_NUL
    .endr
    rumboot_putstring   "Initialization complete.\n"
    b           image_start

.align 4
int_data:
    .long   0x00000000          /*  0 */
    .long   0xFFFFFFFF          /*  4 */
    .long   0xA5A55A5A          /*  8 */
    .long   0x3C3CC3C3          /* 12 */
    .long   0x00A5C300          /* 16 */
    .long   0x0A5C3000          /* 20 */
    .long   0x0000A5C3          /* 24 */
    .long   0x00000000

.align 4
fp_data:
    .long   0xC3C3C3C3          /*  0 */
    .long   0x3C3C3C3C          /*  4 */
    .long   C_S_POS_ZERO        /*  8 */
    .single M_PI                /* 12 */
    .double M_PI                /* 16 */
    .quad   0x85A5A5A5A5A5A5A5  /* 24 (8)  */
    .quad   0x8A5A5A5A5A5A5A5A  /* 32 (16) */
    .quad   C_D_POS_ZERO        /* 40 (24) */
    .quad   C_D_NEG_NAN_MAX     /* 48 (32) */

image_start:
    rumboot_putstring   "Test started.\n"
    b b_pipe

func_call:
    load_const  R_CNTR, 0x0010
loop:
    dump_write_uint32   R_ZERO, R_CNTR
    subi        R_CNTR, R_CNTR, 0x0001
    cmpw  cr0,  R_CNTR, R_ZERO
    bne+  cr0,  loop
    mr          R_DATA, R_EXPV
    blr
    b           test_error

b_pipe:
    rumboot_putstring   "B-pipe... "
    load_const  R_EXPV, 0xC0DE4BEE
    mr          R_DATA, R_ZERO
    bl          func_call
    cmpw  cr7,  R_DATA, R_EXPV
    dump_write_uint32   R_EXPV, R_DATA
    TEST_ASSERT(eq, cr7, "Function call fails.")
    bne-  cr7,  test_error
    rumboot_putstring   "done.\n"

l_pipe:
    rumboot_putstring   "L-pipe...\n"

    load_addr       R_CONI, int_data
    rumboot_putstring   "Check load intructions...\n"
    check_load_8        15, 0x000000C3
    check_load_16       10, 0x00005A5A
    check_load_32       12, 0x3C3CC3C3

    load_addr       R_VARS,     vars
    rumboot_putstring   "Check store intructions...\n"
    check_store_8     0x11, 0x000000C3, 0x10, 0x00C30000
    check_store_16    0x22, 0x0000AA55, 0x20, 0x0000AA55
    check_store_32    0x30, 0xA55AC33C, 0x30, 0xA55AC33C

    rumboot_putstring   "done.\n"

j_i_pipe:
    rumboot_putstring   "J_I-pipe... "
    load_addr   R_CONI, int_data
    load_const  R_OPR1, 0x00000001
    load_const  R_OPR2, 0xAAAA5555
    dump_write_uint32   R_EXPV, R_DATA
    subi        R_OPR1, R_OPR1, 0x0001
    dump_write_uint32   R_OPR1, R_OPR2
    cmpwi cr0,  R_OPR1, 0x0000
    TEST_ASSERT(eq, cr0, "Expected value 0x00000000 after 'subi' instruction.")
    add         R_OPR1, R_OPR1, R_OPR2
    cmpw  cr0,  R_OPR1, R_OPR2
    dump_write_uint32   R_OPR1, R_OPR2
    TEST_ASSERT(eq, cr0, "Expected value 0xAAAA5555 after 'add' instruction.")
    rotlwi      R_DATA, R_OPR1, 8
    dump_write_uint32   R_OPR1, R_DATA
    rlwimi      R_DATA, R_OPR1, 24, 0,  7
    mr          R_TEMP, R_DATA
    rlwimi      R_DATA, R_OPR1, 24, 16, 23
    dump_write_uint32   R_TEMP, R_DATA
    mr          R_OPR1, R_DATA
    load_const  R_EXPV, 0x5555AAAA
    cmpw  cr7,  R_OPR1, R_EXPV
    TEST_ASSERT(eq, cr7, "Expected value 0x5555AAAA after rotate instructions.")
    or          R_OPR1, R_OPR1, R_OPR2
    load_const  R_EXPV, 0xFFFFFFFF
    dump_write_uint32   R_EXPV, R_OPR1
    cmpw  cr7,  R_OPR1, R_EXPV
    TEST_ASSERT(eq, cr7, "Expected value 0xFFFFFFFF after 'or' instruction.")
    andi.       R_OPR1, R_OPR1, 0xAAAA
    load_const  R_EXPV, 0x0000AAAA
    dump_write_uint32   R_OPR1, R_EXPV
    cmpw  cr7,  R_OPR1, R_EXPV
    TEST_ASSERT(eq, cr7, "Expected value 0x0000AAAA after 'andi.' instruction.")
    load_const  R_DATA, 0x0000AAAA
    xor         R_OPR1, R_OPR1, R_DATA
    cmpwi cr7,  R_OPR1, 0x0000
    dump_write_uint32   R_OPR1, R_DATA
    TEST_ASSERT(eq, cr7, "Expected value 0x00000000 after 'xor' instruction.")
    lwz         R_OPR1, 0x10(R_CONI)
    lwz         R_EXPV, 0x14(R_CONI)
    load_const  R_OPR2, 0x0004
    slw         R_DATA, R_OPR1, R_OPR2
    cmpw  cr7,  R_EXPV, R_DATA
    mr          R_TEMP, R_DATA
    TEST_ASSERT(eq, cr7, "Expected value 0x0A5C3000 after 'slw' instruction.")
    lwz         R_EXPV, 0x18(R_CONI)
    addi        R_OPR2, R_OPR2, 0x0008
    srw         R_DATA, R_DATA, R_OPR2
    cmpw  cr7,  R_EXPV, R_DATA
    dump_write_uint32   R_TEMP, R_DATA
    TEST_ASSERT(eq, cr7, "Expected value 0x0000A5C3 after 'srw' instruction.")
    rumboot_putstring   "done.\n"

m_pipe:
    rumboot_putstring   "M-pipe... "
    load_addr   R_CONI, int_data
    lwz         R_OPR1, 12(R_CONI)
    load_const  R_OPR2, 0xAAAA5555
    mulli       R_DATA, R_OPR1, 0x5555
    dump_write_uint32   R_OPR1, R_DATA
    mulhw       R_DATA, R_OPR1, R_OPR2
    dump_write_uint32   R_OPR1, R_DATA
    mullw       R_DATA, R_OPR1, R_OPR2
    dump_write_uint32   R_OPR1, R_DATA
    divwu       R_TEMP, R_DATA, R_OPR1
    dump_write_uint32   R_DATA, R_TEMP
    rumboot_putstring   "done.\n"

fp_load:
    rumboot_putstring   "FP_load...\n"
    /* R_CONF, R_CONI, R_VARS are aliases of R_ADDR*/
    load_addr   R_CONF, fp_data
    load_const  R_OFFS, 0x0018
    lfs         f11,    0x00(R_CONF)
    lfs         f12,    0x04(R_CONF)
    dump_write_single   f11
    dump_write_single   f12
    lfdx        f13,    R_CONF, R_OFFS
    addi        R_OFFS, R_OFFS, 0x0008
    lfdx        f14,    R_CONF, R_OFFS
    dump_write_double   f13
    dump_write_double   f14
    mr          R_BACK, R_CONF
    lfdu        f20,    0x10(R_CONF)
    lfd         f21,    0x08(R_CONF)
    dump_write_uint32   R_BACK, R_CONF
    mr          R_CONF, R_BACK
    load_const  R_OFFS, 0x28
    lfdux       f22,    R_CONF, R_OFFS
    dump_write_uint32   R_BACK, R_CONF
    dump_write_double   f20
    dump_write_double   f21
    dump_write_double   f22
    mr          R_CONF, R_BACK

fp_store:
    rumboot_putstring   "FP_store...\n"
    load_addr   R_VARS, vars_fp

    rumboot_putstring   "stfd...\n"
    stfd        f11,    0x08(R_VARS)
    lfd         f10,    0x08(R_VARS)
    dump_write_double   f10

    rumboot_putstring   "stfdu...\n"
    mr          R_BACK, R_VARS
    stfdu       f11,    0x08(R_VARS)
    dump_write_uint32   R_BACK, R_VARS
    mr          R_VARS, R_BACK
    lfd         f10,    0x08(R_VARS)
    dump_write_double   f10

    rumboot_putstring   "stfdx...\n"
    load_const  R_OFFT, 0x10
    stfdx       f12,    R_VARS, R_OFFT
    lfdx        f10,    R_VARS, R_OFFT
    dump_write_double   f10

    rumboot_putstring   "stfdux...\n"
    mr          R_BACK, R_VARS
    stfdux      f12,    R_VARS, R_OFFT
    dump_write_uint32   R_BACK, R_VARS
    mr          R_VARS, R_BACK
    lfdx        f10,    R_VARS, R_OFFT
    dump_write_double   f10

    rumboot_putstring   "stfs...\n"
    stfs        f13,    0x1C(R_VARS)
    lfs         f9,     0x1C(R_VARS)
    dump_write_single   f9

    rumboot_putstring   "stfsu...\n"
    mr          R_BACK, R_VARS
    stfsu       f13,    0x1C(R_VARS)
    dump_write_uint32   R_BACK, R_VARS
    mr          R_VARS, R_BACK
    lfs         f9,     0x1C(R_VARS)
    dump_write_single   f9

    rumboot_putstring   "stfsx...\n"
    load_const  R_OFFT, 0x24
    stfsx       f14,    R_VARS, R_OFFT
    lfsx        f9,     R_VARS, R_OFFT
    dump_write_single   f9

    rumboot_putstring   "stfsux...\n"
    load_const  R_OFFT, 0x24
    mr          R_BACK, R_VARS
    stfsux      f14,    R_VARS, R_OFFT
    dump_write_uint32   R_BACK, R_VARS
    mr          R_VARS, R_BACK
    lfsx        f9,     R_VARS, R_OFFT
    dump_write_single   f9
    rumboot_putstring   "done.\n"

fp_exec:
    rumboot_putstring   "FP_exec-pipe... "
    load_addr   R_CONF, fp_data
    fpscr_reset
    dump_write_fpscr
    fmadd.		f13,    f13, f14, f14
    fmsub.		f14,    f14, f13, f13
    fdivs.		f15,    f11, f12
    fmuls.		f16,    f15, f11
    fadd        f15,    f11, f12
    fsub        f16,    f15, f11
    dump_write_fpscr
    dump_write_single   f13
    dump_write_single   f14
    dump_write_single   f15
    dump_write_single   f16
    dump_write_double   f13
    dump_write_double   f14
    dump_write_double   f15
    dump_write_double   f16
    fcmpo cr3,  f16,    f12
    fcmpu cr4,  f16,    f12
    TEST_ASSERT(eq, cr3, "Unexpected FP ordered compare instruction result.")
    TEST_ASSERT(eq, cr4, "Unexpected FP unordered compare instruction result.")
    fsqrts      f16,    f16
    dump_write_single   f16
    lfs         f17,    0x0C(R_CONF)
    lfd         f18,    0x10(R_CONF)
    fpscr_reset
    frsp.       f19,    f18
    dump_write_fpscr
    dump_write_double   f18
    dump_write_single   f17
    dump_write_double   f19
    rumboot_putstring   "done.\n"


move_spr:
    rumboot_putstring   "mtspr/mfspr... "
    load_const  R_EXPV,     0x3333CCCC
    mfspr       R_BACK,     SPR_SPRG6   /* save sprg6 */
    mtspr       SPR_SPRG6,  R_EXPV
    mfspr       R_DATA,     SPR_SPRG6
    cmpw  cr7,  R_DATA,     R_EXPV
    dump_write_uint32       R_EXPV,     R_DATA
    mtspr       SPR_SPRG6,  R_BACK      /* restore sprg6 */
    TEST_ASSERT(eq, cr7, "Expected value 0x3333CCCC after 'mtspr/mfspr' instructions.")

move_dcr:
    rumboot_putstring   "mtdcrx/mfdcrx... "
    load_const  R_ADDR, DCR_ITRACE_BASE
    load_const  R_EXPV, 0x60000000
    mfdcrx      R_BACK, R_ADDR
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    mtdcrx      R_ADDR, R_BACK
    cmpw  cr7,  R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "Expected value 0x60000000 after 'mtdcrx/mfdcrx' instructions.")

move_cr:
    rumboot_putstring   "mtcr/mfcr... "
    load_const  R_EXPV, 0x22222222
    mfcr        R_BACK  /* save CR */
    mtcr        R_EXPV
    mfcr        R_DATA
    mtcr        R_BACK  /* rest CR */
    cmpw  cr7,  R_EXPV, R_DATA
    dump_write_uint32   R_EXPV, R_DATA
    TEST_ASSERT(eq, cr7, "Expected value 0x22222222 after 'mtcr/mfcr' instructions.")
    rumboot_putstring "done.\n"

    b           test_end


.align 3
test_end:
    load_const  R_OPR1, FOURCC('T','E','S','T');
    load_const  R_OPR2, FOURCC('E','N','D',0  );
    dump_write_uint32   R_ONES, R_ZERO
    dump_write_uint32   R_OPR1, R_OPR2
    load_addr   R_ADDR, test_end_cont
    load_srr    R_ADDR, R_MSRC
    rfi

test_end_cont:
    rest_srr

    load_addr   R_DMPA, DUMP_ADDR
    /*                  Addr    Bytes   Temp    */
    _rumboot_issdump    R_DMPA, R_DMPB, R_DMPC

    beq+ cr6,           test_ok


test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    load_const  R_RETV, RESULT_ERROR
    b finish

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event  EVENT_OK
    load_const  R_RETV, RESULT_OK

finish:
    rest_stack          R_STKB
    blr


.section ".data","aw",@progbits

.align 4
vars:
vars_fp:
    .rept 0x0060 / SIZEOF_DOUBLE
        .quad C_D_POS_ZERO
    .endr

save_data:
    .rept 0x0400 / SIZEOF_DWORD
        .quad C_D_POS_ZERO
    .endr

