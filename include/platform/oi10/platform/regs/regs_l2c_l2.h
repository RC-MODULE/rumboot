/*
 * reg_l2c_l2.h
 *
 *  Created on: Aug 8, 2018
 *      Author: r.galiulin
 */

#ifndef REGS_L2C_L2_H
#define REGS_L2C_L2_H


#include <rumboot/macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/regs/regs_l2c.h>
#include <platform/regs/fields/l2c_l2.h>


BEGIN_ENUM( L2C_L2REG )
    DECLARE_ENUM_VAL(   L2C_L2ISTAT,        0x000   )
    DECLARE_ENUM_VAL(   L2C_L2PNCR,         0x004   )
    DECLARE_ENUM_VAL(   L2C_L2REVID,        0x00C   )
    DECLARE_ENUM_VAL(   L2C_L2CNFG0,        0x010   )
    DECLARE_ENUM_VAL(   L2C_L2CNFG1,        0x014   )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL,       0x020   )
    DECLARE_ENUM_VAL(   L2C_L2DBGDATA0,     0x024   )
    DECLARE_ENUM_VAL(   L2C_L2DBGDATA1,     0x028   )
    DECLARE_ENUM_VAL(   L2C_L2SLEEPSTAT,    0x080   )
    DECLARE_ENUM_VAL(   L2C_L2SLEEPREQ,     0x084   )
    DECLARE_ENUM_VAL(   L2C_L2MCK,          0x120   )
    DECLARE_ENUM_VAL(   L2C_L2MCKEN,        0x130   )
    DECLARE_ENUM_VAL(   L2C_L2FERR,         0x140   )
    DECLARE_ENUM_VAL(   L2C_L2INT,          0x150   )
    DECLARE_ENUM_VAL(   L2C_L2INTEN,        0x160   )
    DECLARE_ENUM_VAL(   L2C_L2LOG0,         0x180   )
    DECLARE_ENUM_VAL(   L2C_L2LOG1,         0x184   )
    DECLARE_ENUM_VAL(   L2C_L2LOG2,         0x188   )
    DECLARE_ENUM_VAL(   L2C_L2LOG3,         0x18C   )
    DECLARE_ENUM_VAL(   L2C_L2LOG4,         0x190   )
    DECLARE_ENUM_VAL(   L2C_L2LOG5,         0x194   )
    DECLARE_ENUM_VAL(   L2C_L2PLBCFG,       0x210   )
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG,       0x220   )
    DECLARE_ENUM_VAL(   L2C_L2PLBERAP,      0x230   )
    DECLARE_ENUM_VAL(   L2C_L2PLBSTAT0,     0x300   )
    DECLARE_ENUM_VAL(   L2C_L2PLBSTAT1,     0x304   )
    DECLARE_ENUM_VAL(   L2C_L2PLBFRC0,      0x310   )
    DECLARE_ENUM_VAL(   L2C_L2PLBFRC1,      0x314   )
    DECLARE_ENUM_VAL(   L2C_L2PLBMCKEN0,    0x330   )
    DECLARE_ENUM_VAL(   L2C_L2PLBMCKEN1,    0x334   )
    DECLARE_ENUM_VAL(   L2C_L2PLBFERR0,     0x340   )
    DECLARE_ENUM_VAL(   L2C_L2PLBFERR1,     0x344   )
    DECLARE_ENUM_VAL(   L2C_L2PLBINTEN0,    0x360   )
    DECLARE_ENUM_VAL(   L2C_L2PLBINTEN1,    0x364   )
    DECLARE_ENUM_VAL(   L2C_L2ARRCFG,       0x410   )
    DECLARE_ENUM_VAL(   L2C_L2ARRDBG0,      0x420   )
    DECLARE_ENUM_VAL(   L2C_L2ARRDBG1,      0x424   )
    DECLARE_ENUM_VAL(   L2C_L2ARRDBG2,      0x428   )
    DECLARE_ENUM_VAL(   L2C_L2ARRDBG3,      0x42C   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL,    0x480   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCADR,    0x484   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDI0,    0x490   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDI1,    0x494   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDI2,    0x498   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDO0,    0x4A0   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDO1,    0x4A4   )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCDO2,    0x4A8   )
    DECLARE_ENUM_VAL(   L2C_L2ARRSTAT0,     0x500   )
    DECLARE_ENUM_VAL(   L2C_L2ARRSTAT1,     0x504   )
    DECLARE_ENUM_VAL(   L2C_L2ARRSTAT2,     0x508   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFRC0,      0x510   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFRC1,      0x514   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFRC2,      0x518   )
    DECLARE_ENUM_VAL(   L2C_L2ARRMCKEN0,    0x530   )
    DECLARE_ENUM_VAL(   L2C_L2ARRMCKEN1,    0x534   )
    DECLARE_ENUM_VAL(   L2C_L2ARRMCKEN2,    0x538   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFERR0,     0x540   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFERR1,     0x544   )
    DECLARE_ENUM_VAL(   L2C_L2ARRFERR2,     0x548   )
    DECLARE_ENUM_VAL(   L2C_L2ARRINTEN0,    0x560   )
    DECLARE_ENUM_VAL(   L2C_L2ARRINTEN1,    0x564   )
    DECLARE_ENUM_VAL(   L2C_L2ARRINTEN2,    0x568   )
    DECLARE_ENUM_VAL(   L2C_L2CPUCFG,       0x610   )
    DECLARE_ENUM_VAL(   L2C_L2CPUDBG,       0x620   )
    DECLARE_ENUM_VAL(   L2C_L2CPUSTAT,      0x700   )
    DECLARE_ENUM_VAL(   L2C_L2CPUFRC,       0x710   )
    DECLARE_ENUM_VAL(   L2C_L2CPUMCKEN,     0x730   )
    DECLARE_ENUM_VAL(   L2C_L2CPUFERR,      0x740   )
    DECLARE_ENUM_VAL(   L2C_L2CPUINTEN,     0x760   )
    DECLARE_ENUM_VAL(   L2C_L2RACCFG,       0x810   )
    DECLARE_ENUM_VAL(   L2C_L2RACDBG0,      0x820   )
    DECLARE_ENUM_VAL(   L2C_L2RACDBG1,      0x824   )
    DECLARE_ENUM_VAL(   L2C_L2RACSTAT0,     0x900   )
    DECLARE_ENUM_VAL(   L2C_L2RACFRC0,      0x910   )
    DECLARE_ENUM_VAL(   L2C_L2RACMCKEN0,    0x930   )
    DECLARE_ENUM_VAL(   L2C_L2RACFERR0,     0x940   )
    DECLARE_ENUM_VAL(   L2C_L2RACINTEN0,    0x960   )
    DECLARE_ENUM_VAL(   L2C_L2WACCFG,       0xC10   )
    DECLARE_ENUM_VAL(   L2C_L2WACDBG0,      0xC20   )
    DECLARE_ENUM_VAL(   L2C_L2WACDBG1,      0xC24   )
    DECLARE_ENUM_VAL(   L2C_L2WACDBG2,      0xC28   )
    DECLARE_ENUM_VAL(   L2C_L2WACSTAT0,     0xD00   )
    DECLARE_ENUM_VAL(   L2C_L2WACSTAT1,     0xD04   )
    DECLARE_ENUM_VAL(   L2C_L2WACSTAT2,     0xD08   )
    DECLARE_ENUM_VAL(   L2C_L2WACFRC0,      0xD10   )
    DECLARE_ENUM_VAL(   L2C_L2WACFRC1,      0xD14   )
    DECLARE_ENUM_VAL(   L2C_L2WACFRC2,      0xD18   )
    DECLARE_ENUM_VAL(   L2C_L2WACMCKEN0,    0xD30   )
    DECLARE_ENUM_VAL(   L2C_L2WACMCKEN1,    0xD34   )
    DECLARE_ENUM_VAL(   L2C_L2WACMCKEN2,    0xD38   )
    DECLARE_ENUM_VAL(   L2C_L2WACFERR0,     0xD40   )
    DECLARE_ENUM_VAL(   L2C_L2WACFERR1,     0xD44   )
    DECLARE_ENUM_VAL(   L2C_L2WACFERR2,     0xD48   )
    DECLARE_ENUM_VAL(   L2C_L2WACINTEN0,    0xD60   )
    DECLARE_ENUM_VAL(   L2C_L2WACINTEN1,    0xD64   )
    DECLARE_ENUM_VAL(   L2C_L2WACINTEN2,    0xD68   )
END_ENUM( L2C_L2REG )


#ifdef __ASSEMBLER__
.macro l2c_l2_write     reg_base, reg_l2reg, reg_value, reg_tmp
    addi    \reg_tmp, \reg_base, L2C_L2CDCRAI
    mtdcrx  \reg_tmp, \reg_l2reg
    addi    \reg_tmp, \reg_base, L2C_L2CDCRDI
    mtdcrx  \reg_tmp, \reg_value
.endm

.macro l2c_l2_read      reg_result, reg_base, reg_l2reg
    addi    \reg_result, \reg_base, L2C_L2CDCRAI
    mtdcrx  \reg_result, \reg_l2reg
    addi    \reg_result, \reg_base, L2C_L2CDCRDI
    mfdcrx  \reg_result, \reg_result
.endm
#else
#include <arch/ppc_476fp_lib_c.h>
static inline __attribute__((always_inline)) void l2c_l2_write( uint32_t const base_addr, L2C_L2REG const reg, uint32_t const value ) {
    dcr_write( base_addr+L2C_L2CDCRAI, (uint32_t const)reg );
    dcr_write( base_addr+L2C_L2CDCRDI, value );
}

static inline __attribute__((always_inline)) uint32_t l2c_l2_read( uint32_t const base_addr, L2C_L2REG const reg ) {
    dcr_write( base_addr+L2C_L2CDCRAI, (uint32_t const)reg );
    return dcr_read( base_addr+L2C_L2CDCRDI );
}
#endif


#endif /* REGS_L2C_L2_H */
