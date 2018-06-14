
#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>

#define SAVE_MEM_ADDR	0xC002e000

.macro check_reg_value rN
	rumboot_putstring "\rN "
	lwzu		r5, 4(r3)
	lwzu		r6, 4(r4)
	cmpw		cr7, r5, r6
    TEST_ASSERT(eq, cr7, "Saved register \rN content differs from expected!\n")
    bne cr7, test_error
.endm


.section ".text","ax",@progbits

.global main

main:
	save_stack r29
	b rumboot_entry_point

origin_values:
    .long	0x0000FF00
    .long	0x0000FE01
    .long	0x0000FD02
    .long	0x0000FC03
    .long	0x0000FB04
    .long	0x0000FA05
    .long	0x0000F906
    .long	0x0000F807
    .long	0x0000F708
    .long	0x0000F609
    .long	0x0000F50A
    .long	0x0000F40B
    .long	0x0000F30C
    .long	0x0000F20D
    .long	0x0000F10E
    .long	0x0000F00F
    .long	0x00000F10
    .long	0x00000E11
    .long	0x00000D12
    .long	0x00000C13
    .long	0x00000B14
    .long	0x00000A15
    .long	0x00000916
    .long	0x00000817
    .long	0x00000718
    .long	0x00000619
    .long	0x0000051A
    .long	0x0000041B
    .long	0x0000031C
    .long	0x0000021D
    .long	0x0000011E
    .long	0x0000001F


rumboot_entry_point:
    load_const	r0,  0xFF00
    load_const	r1,  0xFE01
    load_const	r2,  0xFD02
    load_const	r3,  0xFC03
    load_const	r4,  0xFB04
    load_const	r5,  0xFA05
    load_const	r6,  0xF906
    load_const	r7,  0xF807
    load_const	r8,  0xF708
    load_const	r9,  0xF609
    load_const	r10, 0xF50A
    load_const	r11, 0xF40B
    load_const	r12, 0xF30C
    load_const	r13, 0xF20D
    load_const	r14, 0xF10E
    load_const	r15, 0xF00F
    load_const	r16, 0x0F10
    load_const	r17, 0x0E11
    load_const	r18, 0x0D12
    load_const	r19, 0x0C13
    load_const	r20, 0x0B14
    load_const	r21, 0x0A15
    load_const	r22, 0x0916
    load_const	r23, 0x0817
    load_const	r24, 0x0718
    load_const	r25, 0x0619
    load_const	r26, 0x051A
    load_const	r27, 0x041B
    load_const	r28, 0x031C
    load_const	r29, 0x021D
    load_const	r30, 0x011E
    load_const	r31, 0x001F

	mtsprg6		r3				/* Save r3 */
	load_const	r3,  SAVE_MEM_ADDR
	stw			r0,  0*4(r3)
	stw			r1,  1*4(r3)
	mr			r1,  r3
	mfsprg6		r3				/* Restore r3 */
	stmw		r2,  2*4(r1)	/* Store r2-r31 to memory */

	rumboot_putstring "Check... "

	load_const	r3,  SAVE_MEM_ADDR - 4	/* for lwzu */
	load_addr	r4,  origin_values - 4	/* for lwzu */

	check_reg_value	r0
	check_reg_value	r1
	check_reg_value	r2
	check_reg_value	r3
	check_reg_value	r4
	check_reg_value	r5
	check_reg_value	r6
	check_reg_value	r7
	check_reg_value	r8
	check_reg_value	r9
	check_reg_value	r10
	check_reg_value	r11
	check_reg_value	r12
	check_reg_value	r13
	check_reg_value	r14
	check_reg_value	r15
	check_reg_value	r16
	check_reg_value	r17
	check_reg_value	r18
	check_reg_value	r19
	check_reg_value	r20
	check_reg_value	r21
	check_reg_value	r22
	check_reg_value	r23
	check_reg_value	r24
	check_reg_value	r25
	check_reg_value	r26
	check_reg_value	r27
	check_reg_value	r28
	check_reg_value	r29
	check_reg_value	r30
	check_reg_value	r31
	rumboot_putstring "Done!\n"

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
	rest_stack r29
    blr

