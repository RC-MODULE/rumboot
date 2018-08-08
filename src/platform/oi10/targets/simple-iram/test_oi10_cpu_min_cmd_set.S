

#include <platform/test_event_codes.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_fpu_const.h>
#include <platform/trace.S.h>
#include <platform/devices.h>

.section ".text","ax",@progbits

.global main

main:
	save_stack r29
	b	image_start

.align 2
test_mpw_cpu_min_cmd_set_data:
.long 0xA5A55A5A
.long 0x3C3CC3C3
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000

.align 3
test_mpw_cpu_min_cmd_set_fp_data:
.long   0xC3C3C3C3
.long   0x3C3C3C3C
.long   C_S_POS_ZERO
.single M_PI
.quad   0x85A5A5A5A5A5A5A5
.quad   0x8A5A5A5A5A5A5A5A
.quad   C_D_POS_ZERO
.quad   C_D_POS_ZERO
.quad   C_D_POS_ZERO
.quad   C_D_POS_ZERO
.double M_PI
.quad   C_D_NEG_ZERO

image_start:
    rumboot_putstring   "Test started.\n"
    set_cr      cr6,    eq

test_mpw_cpu_min_cmd_set:
    b b_pipe

func_call:
    load_const  r23,    0x0010
loop:
    subi        r23,    r23,    0x0001
    cmpwi cr0,  r23,    0x0000
    bne+  cr0,  loop
    blr
    b           test_error

b_pipe:
    rumboot_putstring   "B-pipe... "
    bl          func_call
    rumboot_putstring   "done.\n"

l_pipe:
    rumboot_putstring   "L-pipe... "
    load_addr   r23,    test_mpw_cpu_min_cmd_set_data
    load_const  r24,    0x0004

    load_const  r25,    12
    lbz         r26,    0(r23)
    lbzx        r27,    r24,    r23
    stb         r26,    8(r23)
    stbx        r27,    r25,    r23

    load_const  r25,    20
    lhz         r26,    0(r23)
    lhzx        r27,    r24,    r23
    sth         r26,    16(r23)
    sthx        r27,    r25,    r23

    load_const  r25,    28
    lwz         r26,    0(r23)
    lwzx        r27,    r14,    r23
    stw         r26,    24(r3)
    stwx        r27,    r25,    r23
    rumboot_putstring   "done.\n"

j_i_pipe:
    rumboot_putstring   "J_I-pipe... "
    load_const  r23,    0x00000001
    load_const  r24,    0xAAAA5555
    subi        r23,    r23,    0x00000001
    cmpwi cr0,  r23,    0x00000000
    TEST_ASSERT(eq, cr0, "Expected value 0x00000000 after 'subi' instruction.")
    add         r23,    r23,    r24
    cmpw  cr0,  r23,    r24
    TEST_ASSERT(eq, cr0, "Expected value 0xAAAA5555 after 'add' instruction.")
    rotlwi      r25,    r23,    8
    rlwimi      r25,    r23,    24, 0,  7
    rlwimi      r25,    r23,    24, 16, 23
    mr          r23,    r25
    load_const  r26,    0x5555AAAA
    cmpw  cr0,  r23,    r26
    TEST_ASSERT(eq, cr0, "Expected value 0x5555AAAA after rotate instructions.")
    or          r23,    r23,    r24
    load_const  r26,    0xFFFFFFFF
    cmpw  cr0,  r23,    r26
    TEST_ASSERT(eq, cr0, "Expected value 0xFFFFFFFF after 'or' instruction.")
    andi.       r23,    r23,    0xAAAA
    load_const  r26,    0x0000AAAA
    cmpw  cr0,  r23,    r26
    TEST_ASSERT(eq, cr0, "Expected value 0x0000AAAA after 'andi' instruction.")
    load_const  r25,    0x0000AAAA
    xor         r23,    r23,    r25
    cmpwi cr0,  r23,    0x0000
    TEST_ASSERT(eq, cr0, "Expected value 0x00000000 after 'xor' instruction.")
    rumboot_putstring   "done.\n"

m_pipe:
    rumboot_putstring   "M-pipe... "
    load_addr   r23,    test_mpw_cpu_min_cmd_set_data
    lwz         r24,    4(r23)
    load_const  r25,    0xAAAA5555
    mulli       r26,    r24,    0x5555
    mulhw       r27,    r24,    r25
    mullw       r28,    r24,    r25
    divwu       r29,    r26,    r24
    rumboot_putstring   "done.\n"

fp_load_store:
    rumboot_putstring   "FP_load_store... "

    /* FPU enable */
    mfmsr       r23
    ori         r23,    r23,    reg_field(50, 0b1)
    mtmsr       r23
    msync

    load_addr   r23,    test_mpw_cpu_min_cmd_set_fp_data
    lfs         f1,     0(r23)
    lfs         f2,     4(r23)
    load_const  r24,    0x0008
    lfdx        f3,     r23,    r24
    load_const  r24,    16
    lfdx        f4,     r23,    r24

    stfd        f1,     24(r23)
    load_const  r24,    32
    stfdx       f2,     r23,    r24
    stfs        f3,     40(r3)
    load_const  r24,    48
    stfsx       f4,     r23,    r24
    rumboot_putstring   "done.\n"

fp_exec:
    rumboot_putstring   "FP_exec-pipe... "
    fmadd.		f3, f3, f4, f4
    fmsub.		f4, f4, f3, f3
    fdivs.		f5, f1, f2
    fmuls.		f6, f5, f1
    fadd		f5, f1, f2
    fsub		f6, f5, f1
    fcmpo cr3,  f6, f2
    fcmpu cr4,  f6, f2
    TEST_ASSERT(eq, cr3, "Unexpected FP ordered compare instruction result.")
    TEST_ASSERT(eq, cr4, "Unexpected FP unordered compare instruction result.")
    fsqrts		f6, f6
    rumboot_putstring   "done.\n"


move_spr_dcr_mcr:
    rumboot_putstring   "SPR_DCR_MC... "
    load_const  r23, 0x3333CCCC
    mtspr       SPR_SPRG6,  r23
    mfspr       r24,    SPR_SPRG6
    cmpw  cr0,  r24,    r23
    TEST_ASSERT(eq, cr0, "Expected value 0x3333CCCC after 'mtspr/mfspr' instructions.")
    load_const  r23,    DCR_ITRACE_BASE
    load_const  r24,    0x60000000
    mtdcrx      r23,    r24
    mfdcrx      r25,    r23
    cmpw  cr0,  r25,    r24
    TEST_ASSERT(eq, cr0, "Expected value 0x60000000 after 'mtdcrx/mfdcrx' instructions.")
    load_const  r23,    0x22222222
    mfcr        r26     /* save CR */
    mtcr        r23
    mfcr        r24
    mtcr        r26     /* rest CR */
    load_const  r25,    0x22222222
    cmpw  cr0,  r24,    r25
    TEST_ASSERT(eq, cr0, "Expected value 0x22222222 after 'mtcr/mfcr' instructions.")
    rumboot_putstring "done.\n"

    bne-  cr6,  test_error

test_ok:
    rumboot_putstring   "TEST OK\n"
    load_const  r3,     RESULT_OK
    b           finish

test_error:
    rumboot_putstring   "TEST ERROR\n"
    load_const  r3,     RESULT_ERROR
    b                   finish

finish:
    rest_stack          r29
    blr

