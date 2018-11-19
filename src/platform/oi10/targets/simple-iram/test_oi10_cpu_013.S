
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0
#define DEBUG_CONTINUE_ON_ASSERT
#define RUMBOOT_ASSERT_WARN_ONLY

/*
#define UNIDIRECTIONAL_BRIDGE
*/

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb4ahb.h>

#define R_STKP          r1  /* Stack pointer        */
#define R_ARG0          r3  /* First  argument      */
#define R_ARG1          r4  /* Second argument      */
#define R_ARG2          r5  /* Third  argument      */
#define R_ARG3          r6  /* Fourth argument      */
#define R_DCRA          r20 /* DCR address          */
#define R_ADDR          r21 /* Unified address      */
#define R_DATA          r22 /* Readed data          */
#define R_EXPV          r23 /* Expected value       */
#define R_MASK          r24 /* Unified mask         */
#define R_SHFT          r25 /* Shift value          */
#define R_TERM          r26 /* Termination value    */
#define R_ZERO          r30 /* Holds 0x00000000     */
#define R_ONES          r31 /* Holds 0xFFFFFFFF     */

#define CVR3            r14,r15,r16     /* Check Value Regs triade */
#define P4A0(OFFSET)    (DCR_PLB4AHB_0_BASE + (OFFSET))

.macro check_rst_value reg, exp, regname
    check_value CVR3, P4A0(\reg), \exp, "PLB4AHB_0_\regname"
.endm

.section ".text","ax",@progbits

.global main

main:
    load_const  R_ONES, 0xFFFFFFFF
    load_const  R_ZERO, 0x00000000
    load_const  R_SHFT, 0x00000001
    /* General test status in cr6 */
    set_cr      cr6,    eq

p4ahb_0:
    rumboot_putstring "Check default values...\n"

    check_rst_value REVID           REVID_VALUE             "REVID"
    check_rst_value SEAR_ADDRUPPER  SEAR_ADDRUPPER_VALUE    "SEAR_ADDRUPPER"
    check_rst_value SEAR_ADDRLOWER  SEAR_ADDRLOWER_VALUE    "SEAR_ADDRLOWER"
    check_rst_value SESR            SESR_VALUE              "SESR"
    check_rst_value TOP_ADDR        TOP_ADDR_VALUE          "TOP_ADDR"
    check_rst_value BOT_ADDR        BOT_ADDR_VALUE          "BOT_ADDR"
    check_rst_value ATTRS           ATTRS_VALUE             "ATTRS"
    check_rst_value CONTROL         CONTROL_VALUE           "CONTROL"
    check_rst_value ERR_ST          ERR_ST_VALUE            "ERR_ST"
    check_rst_value ERR_ADDR        ERR_ADDR_VALUE          "ERR_ADDR"
    check_rst_value INT_MSK         INT_MSK_VALUE           "INT_MSK"

    rumboot_putstring "Done!\n"

running_0_1_p4ahb_0:
    load_const  R_DCRA, P4A0(CONTROL)
    mtdcrx      R_DCRA, R_ZERO
    mfdcrx      R_DATA, R_DCRA
    cmpw  cr7,  R_DATA, R_ZERO
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 1 init)")

p4ahb_0_running_1:
    load_const  R_EXPV, 0x00000001
    load_const  R_TERM, 0x00000080
p4ahb_0_loop_1:
    mtdcrx      R_DCRA, R_EXPV
    mfdcrx      R_DATA, R_DCRA
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 1)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   p4ahb_0_running_0
    slw         R_EXPV, R_EXPV, R_SHFT
    b           p4ahb_0_loop_1

p4ahb_0_running_0:
    load_const  R_EXPV, 0x000000FF
    mtdcrx      R_DCRA, R_EXPV
    mfdcrx      R_DATA, R_DCRA
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 0 init)")

    load_const  R_EXPV, 0x000000FE
    load_const  R_TERM, 0x0000007F
    load_const  R_MASK, 0x000000FF
p4ahb_0_loop_0:
    mtdcrx      R_DCRA, R_EXPV
    mfdcrx      R_DATA, R_DCRA
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 0)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   p4ahb_0_done
    slw         R_EXPV, R_EXPV, R_SHFT
    ori         R_EXPV, R_EXPV, 0x0001
    and         R_EXPV, R_EXPV, R_MASK
    b           p4ahb_0_loop_0

 p4ahb_0_done:
    rumboot_putstring "p4ahb_0 running 0/1 done!\n"

test_end:
    bne- cr6,   test_error

test_ok:
	rumboot_putstring  "TEST OK\n"
	load_const r3,     RESULT_OK
	b finish

test_error:
	rumboot_putstring  "TEST ERROR\n"
	load_const r3,     RESULT_ERROR

finish:
	blr

