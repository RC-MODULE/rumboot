#ifndef TEST_MACRO_ASM_H_
#define TEST_MACRO_ASM_H_

#define DEBUG_DCR_TEST
//DCR register value checker
.macro check_value rE,rA,rR,raddress,exp_value,reg_name
    load_const \rE, \exp_value
    load_const \rA, \raddress
    mfdcrx \rR, \rA
    cmp cr7,0, \rR,\rE
    rumboot_putstring "\reg_name actual value: "
#ifdef NO_TRACE_HEX
    // trace_hex \rR
#endif
    TEST_ASSERT(eq,cr7,"DCRE: expected \reg_name = \exp_value (\raddress) ")
#ifdef DEBUG_DCR_TEST
    rumboot_putstring "\reg_name OK\n"
#endif
.endm


#endif //TEST_MACRO_ASM_H_
