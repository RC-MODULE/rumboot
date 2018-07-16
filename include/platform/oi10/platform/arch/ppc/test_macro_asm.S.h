#ifndef TEST_MACRO_ASM_H_
#define TEST_MACRO_ASM_H_

/* Define in test, if you need it.
#define DEBUG_DCR_TEST
*/

/* DCR register value checker */
.macro check_value rA,rR,rE,vAddr,vExp,sName
#ifdef CHECK_VALUE_VERBOSE
rumboot_putstring       "Check \sName ... \t"
#endif
    load_const  \rE,        \vExp
    load_const  \rA,        \vAddr
    mfdcrx      \rR,        \rA
    cmpw cr7,   \rR,        \rE
    crand       cr6*4+eq,    cr6*4+eq,      cr7*4+eq
    TEST_ASSERT(eq,cr7,"DCRE: expected \sName = \vExp (address: \vAddr) ")
    bne- cr7,   8f
#ifdef CHECK_VALUE_VERBOSE
    rumboot_putstring "ok!\n"
#endif
    8:
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif
.endm

.macro set_cr crF, crC
    crorc   \crF*4+\crC, \crF*4+\crC, \crF*4+\crC
.endm

.macro clr_cr crF, crC
    crandc   \crF*4+\crC, \crF*4+\crC, \crF*4+\crC
.endm

.macro inv_cr crF, crC
    crnor   \crF*4+\crC, \crF*4+\crC, \crF*4+\crC
.endm

.macro load_spr tmp_reg, spr_reg, spr_val
    load_const  \tmp_reg,   \spr_val
    mtspr       \spr_reg,   \tmp_reg
.endm

#define RESULT_OK       0x00
#define RESULT_ERROR    0x01

#endif /* TEST_MACRO_ASM_H_ */
