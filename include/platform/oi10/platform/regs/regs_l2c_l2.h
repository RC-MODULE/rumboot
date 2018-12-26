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

BEGIN_ENUM( L2C_L2DBGSEL_FIELD )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB0_e,    3    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB0_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB0_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB0_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB1_e,    7    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB1_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB1_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB1_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB2_e,    11   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB2_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB2_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB2_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB3_e,    15   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB3_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB3_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB3_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB4_e,    19   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB4_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB4_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB4_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB5_e,    23   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB5_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB5_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB5_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB6_e,    27   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB6_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB6_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB6_e) )

    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB7_e,    31   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB7_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_DBGSELB7_i,    IBM_BIT_INDEX(32, L2C_L2DBGSEL_DBGSELB7_e) )
END_ENUM( L2C_L2DBGSEL_FIELD )

BEGIN_ENUM( L2C_L2DBGSEL_VALUE )
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_None,              0b0000  )   /**All '0' will be driven on external debug output bus*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_PLD6,              0b0010  )   /**See L2C_L2PLBDBG_VALUE*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_CPUL1,             0b0011  )   /**See Section 4.6.4.2 CPU L1 Cache Interface Debug Select Register (L2CPUDBG) on page 146*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_L2_Part1,          0b0100  )   /**See Section 4.6.3.2 Array Debug Select Part 1 Register (L2ARRDBG0) on page 119*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_L2_Part2,          0b0101  )   /**See Section 4.6.3.3 Array Debug Select Part 2 Register (L2ARRDBG1) on page 120*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_L2_Part3,          0b0110  )   /**See Section 4.6.3.4 Array Debug Select Part 3 Register (L2ARRDBG2) on page 121*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_L2_Part4,          0b0111  )   /**See Section 4.6.3.5 Array Debug Select Part 4 Register (L2ARRDBG3) on page 122*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_RAC_Part1,         0b1000  )   /**See Section 4.6.5.2 RAC Debug Select Part 1 Register (L2RACDBG0) on page 154*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_RAC_Part2,         0b1001  )   /**See Section 4.6.5.3 RAC Debug Select Part 2 Register (L2RACDBG1) on page 155*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_WAC_Noncachable,   0b1100  )   /**See Section 4.6.6.2 WAC Noncacheable Debug Select Registers (L2WACDBG0) on page 164 */
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_WAC_StoreGather,   0b1101  )   /**See Section 4.6.6.3 WAC Store and Gather Debug Select Register (L2WACDBG1) on page 165*/
    DECLARE_ENUM_VAL(   L2C_L2DBGSEL_WAC_Castout,       0b1110  )   /**See Section 4.6.6.4 WAC Castpout Debug Select Register (L2WACDBG2) on page 166*/
END_ENUM( L2C_L2DBGSEL_VALUE )

BEGIN_ENUM( L2C_L2PLBSEL_FIELD )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB0_e,    3    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB0_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB0_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB0_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB1_e,    7    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB1_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB1_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB1_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB2_e,    11   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB2_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB2_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB2_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB3_e,    15   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB3_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB3_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB3_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB4_e,    19   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB4_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB4_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB4_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB5_e,    23   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB5_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB5_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB5_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB6_e,    27   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB6_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB6_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB6_e) )

    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB7_e,    31   )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB7_n,    4    )
    DECLARE_ENUM_VAL(   L2C_L2PLBSEL_DBGSELB7_i,    IBM_BIT_INDEX(32, L2C_L2PLBSEL_DBGSELB7_e) )
END_ENUM( L2C_L2PLBSEL_FIELD )

BEGIN_ENUM( L2C_L2PLBDBG_VALUE )
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_Null,                              0x0  )   /**Null (default).*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterSummary,                     0x1  )   /**PLB6 master interface summary.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterReqPart1,                    0x2  )   /**PLB6 master request interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterReqPart2,                    0x3  )   /**PLB6 master request interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterResp,                        0x4  )   /**PLB6 master response interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterRd,                          0x5  )   /**PLB6 master read interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_MasterWr,                          0x6  )   /**PLB6 master write interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_InternalSnoopReq,                  0x7  )   /**Internal snoop request.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_InternalSnoopResp,                 0x8  )   /**Internal snoop response.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_PLB6SnoopReqInternalSignals,       0x9  )   /**PLB6 snoop request interface and internal signals.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_PLB6SnoopReq,                      0xA  )   /**PLB6 snoop request interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_PLB6SnoopResp,                     0xB  )   /**PLB6 snoop response interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_PLB6SnoopIntervention,             0xC  )   /**PLB6 snoop intervention interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_InternalRACtoCPU,                  0xD  )   /**Internal RAC to processor local bus (PLB) interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_InternalWACtoPLB,                  0xE  )   /**Internal WAC to PLB interface.*/
    DECLARE_ENUM_VAL(   L2C_L2PLBDBG_InternalInterventionAndSnoopResp,  0xF  )   /**Internal intervention response interface. Internal snoop response interface.*/
END_ENUM( L2C_L2PLBDBG_VALUE )

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
static inline void l2c_l2_write( uint32_t const base_addr, L2C_L2REG const reg, uint32_t const value ) {
    dcr_write( base_addr+L2C_L2CDCRAI, (uint32_t const)reg );
    dcr_write( base_addr+L2C_L2CDCRDI, value );
}

static inline uint32_t l2c_l2_read( uint32_t const base_addr, L2C_L2REG const reg ) {
    dcr_write( base_addr+L2C_L2CDCRAI, (uint32_t const)reg );
    return dcr_read( base_addr+L2C_L2CDCRDI );
}
#endif


#endif /* REGS_L2C_L2_H */
