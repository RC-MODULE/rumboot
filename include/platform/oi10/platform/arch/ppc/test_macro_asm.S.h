#ifndef TEST_MACRO_ASM_H_
#define TEST_MACRO_ASM_H_

/* Define in test, if you need it.
#define DEBUG_DCR_TEST
*/
/* DCR register value checker */
.macro check_value rE,rA,rR,raddress,exp_value,reg_name
    load_const \rE, \exp_value
    load_const \rA, \raddress
    mfdcrx \rR, \rA
    cmp cr7,0, \rR,\rE
#ifdef DEBUG_PUT2USPRG0
    mtspr SPR_USPGR0, \rR
#endif
/* 	rumboot_putstring "\reg_name actual value: " */
/*	trace_hex \rR */
    TEST_ASSERT(eq,cr7,"DCRE: expected \reg_name = \exp_value (\raddress) ")
#ifdef DEBUG_DCR_TEST
    rumboot_putstring "\reg_name OK\n"
#endif
.endm


#endif /* TEST_MACRO_ASM_H_ */
