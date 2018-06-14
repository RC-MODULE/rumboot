

#include <platform/test_event_codes.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
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
.long 0xC3C3C3C3
.long 0x3C3C3C3C
.quad 0x85A5A5A5A5A5A5A5
.quad 0x8A5A5A5A5A5A5A5A
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000


image_start:
    rumboot_putstring "Test started.\n"

test_mpw_cpu_min_cmd_set:
    b b_pipe

func_call:
    load_const r3, 0x10
loop:
    subi r3, r3, 0x1
    cmpwi cr0, r3, 0
    bne cr0, loop
    blr

b_pipe:
    rumboot_putstring "B-pipe... "
    bl func_call
    rumboot_putstring "ok!\n"

l_pipe:
    rumboot_putstring "L-pipe... "
    load_addr	r3, test_mpw_cpu_min_cmd_set_data
    load_const	r4, 4

    load_const	r5, 12
    lbz			r6, 0(r3)
    lbzx		r7, r4, r3
    stb			r6, 8(r3)
    stbx		r7, r5, r3

    load_const	r5, 20
    lhz			r6, 0(r3)
    lhzx		r7, r4, r3
    sth			r6, 16(r3)
    sthx		r7, r5, r3

    load_const	r5, 28
    lwz			r6, 0(r3)
    lwzx		r7, r4, r3
    stw			r6, 24(r3)
    stwx		r7, r5, r3
    rumboot_putstring "ok!\n"

j_i_pipe:
    rumboot_putstring "J_I-pipe... "
    load_const	r3,      0x00000001
    load_const	r4,      0xAAAA5555
    subi		r3,  r3, 0x00000001
    cmpwi		cr0, r3, 0x00000000
    TEST_ASSERT(eq, cr0, "Expected value 0x00000000 after 'subi' instruction.")
    add			r3,  r3, r4
    cmpw		cr0, r3, r4
    TEST_ASSERT(eq, cr0, "Expected value 0xAAAA5555 after 'add' instruction.")
    rotlwi		r5,  r3, 8
    rlwimi		r5,  r3, 24, 0, 7
    rlwimi		r5,  r3, 24, 16, 23
    mr			r3,  r5
    load_const	r6,      0x5555AAAA
    cmpw		cr0, r3, r6
    TEST_ASSERT(eq, cr0, "Expected value 0x5555AAAA after rotate instructions.")
    or			r3,  r3, r4
    load_const	r6,      0xFFFFFFFF
    cmpw		cr0, r3, r6
    TEST_ASSERT(eq, cr0, "Expected value 0xFFFFFFFF after 'or' instruction.")
    andi.		r3,  r3, 0x0000AAAA
    load_const	r6,      0x0000AAAA
    cmpw		cr0, r3, r6
    TEST_ASSERT(eq, cr0, "Expected value 0x0000AAAA after 'andi' instruction.")
    load_const	r5,      0x0000AAAA
    xor			r3,  r3, r5
    cmpwi		cr0, r3, 0x00
    TEST_ASSERT(eq, cr0, "Expected value 0x00000000 after 'xor' instruction.")
    rumboot_putstring "ok!\n"

m_pipe:
    rumboot_putstring "M-pipe... "
    load_addr	r3, test_mpw_cpu_min_cmd_set_data
    lwz			r4, 4(r3)
    load_const	r5, 	0xAAAA5555
    mulli		r6, r4, 0x5555
    mulhw		r7, r4, r5
    mullw		r8, r4, r5
    divwu		r9, r6, r4
    rumboot_putstring "ok!\n"

fp_load_store:
    rumboot_putstring "FP_load_store... "

    /* FPU enable */
    mfmsr		r3
    ori			r3, r3, reg_field(50, 0b1)
    mtmsr		r3
    msync

    load_addr	r3, test_mpw_cpu_min_cmd_set_fp_data
    lfs			f1, 0(r3)
    lfs			f2, 4(r3)
    load_const	r4, 8
    lfdx		f3, r3, r4
    load_const	r4, 16
    lfdx		f4, r3, r4

    stfd		f1, 24(r3)
    load_const	r4, 32
    stfdx		f2, r3, r4
    stfs		f3, 40(r3)
    load_const	r4, 48
    stfsx		f4, r3, r4
    rumboot_putstring "ok!\n"

fp_exec:
    rumboot_putstring "FP_exec-pipe... "
    fmadd.		f3, f3, f4, f4
    fmsub.		f4, f4, f3, f3
    fdivs.		f5, f1, f2
    fmuls.		f6, f5, f1
    fadd		f5, f1, f2
    fsub		f6, f5, f1
    fcmpo		0,  f6, f2
    TEST_ASSERT(eq, cr0, "Unexpected result after floating point compare instruction.")
    fsqrts		f6, f6
    rumboot_putstring "ok!\n"


move_spr_dcr_mcr:
    rumboot_putstring "SPR_DCR_MC... "
    load_const	r3,  0x3333CCCC
    mtspr		SPR_SPRG6,  r3
    mfspr		r4,  SPR_SPRG6
    cmpw		cr0, r4,	r3
    TEST_ASSERT(eq, cr0, "Expected value 0x3333CCCC after 'mtspr/mfspr' instructions.")
    load_const	r3,	 DCR_ITRACE_BASE
    load_const	r4,  0x60000000
    mtdcrx		r3,  r4
    mfdcrx		r5,  r3
    cmpw		cr0, r5, r4
    TEST_ASSERT(eq, cr0, "Expected value 0x60000000 after 'mtdcrx/mfdcrx' instructions.")
    load_const	r3,  0x22222222
    mtcr		r3
    mfcr		r4
    load_const	r5,  0x22222222
    cmpw		cr0, r4, r5
    TEST_ASSERT(eq, cr0, "Expected value 0x22222222 after 'mtcr/mfcr' instructions.")
    rumboot_putstring "ok!\n"

 test_ok:
    test_event EVENT_OK
    rumboot_putstring "TEST OK\n"
    load_const r3, RESULT_OK
    b finish

test_error:
    test_event EVENT_ERROR
    rumboot_putstring "TEST ERROR\n"
    load_const r3, RESULT_ERROR
    b finish

finish:
    test_event EVENT_FINISHED
    rumboot_putstring "TEST FINISHED\n"
    rest_stack r29
    blr

