#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/regs/regs_p64.h>

.section ".text.rom","a",@progbits

.global p64_set_split_range_address
p64_set_split_range_address:
    addi        r9,r3,P64_ADDRL1
    mtdcrx      r9,r4
    addi        r9,r3,P64_ADDRH1
    mtdcrx      r9,r5
    mfdcrx      r9,r3
    ori         r9,r9,( 1 << P64CR_SPEN )
    mtdcrx      r3,r9
    blr
