/*
 * regs_l2c_pmu.h
 *
 *  Created on: Aug 8, 2018
 *      Author: r.galiulin
 */

#ifndef REGS_REGS_L2C_PMU_H
#define REGS_REGS_L2C_PMU_H


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/regs/regs_l2c.h>

/*  <-- ./units/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C_L2CORE.v
   // Performance Monitor Outputs
   assign       L2C_EVENT[0]     = rac_L2LwarxComplete;
   assign       L2C_EVENT[1]     = wac_L2StwcxSuccess;
   assign       L2C_EVENT[2]     = wac_L2MissEviction;
   assign       L2C_EVENT[3]     = rac_L2MissDFetch;
   assign       L2C_EVENT[4]     = rac_L2MissIFetch;
   assign       L2C_EVENT[5]     = wac_L2MissStore;
   assign       L2C_EVENT[6]     = rac_L2HitDFetch;
   assign       L2C_EVENT[7]     = rac_L2HitIFetch;
   assign       L2C_EVENT[8]     = wac_L2HitStore;
   assign       L2C_EVENT[9]     = wac_L2ReadAfterWrite;
   assign       L2C_EVENT[10]    = wac_L2WriteAfterWrite;
   assign       L2C_EVENT[11]    = plb_PlbMasterCmd;
   assign       L2C_EVENT[12]    = plb_PlbMasterRead;
   assign       L2C_EVENT[13]    = plb_PlbMasterRWITM;
   assign       L2C_EVENT[14]    = plb_PlbMasterDclaim;
   assign       L2C_EVENT[15]    = plb_PlbMasterWrite;
   assign       L2C_EVENT[16:19] = rac_PlbReadOccupancy;
   assign       L2C_EVENT[20]    = rac_PlbMasterIntvnM;
   assign       L2C_EVENT[21]    = rac_PlbMasterIntvnS;
   assign       L2C_EVENT[22]    = rac_PlbMasterMemData;
   assign       L2C_EVENT[23]    = plb_PlbSnoopCmd;
   assign       L2C_EVENT[24]    = plb_PlbSnoopL2Cmd;
   assign       L2C_EVENT[25]    = wac_PlbSnoopHitIntvn;
   assign       L2C_EVENT[26]    = wac_PlbSnoopHit;
   assign       L2C_EVENT[27]    = plb_PlbSnoopRetry;
*/

BEGIN_ENUM( L2C_EVENT )
    DECLARE_ENUM_VAL(   L2EV_L2LwarxComplete,       0   )
    DECLARE_ENUM_VAL(   L2EV_L2StwcxSuccess,        1   )
    DECLARE_ENUM_VAL(   L2EV_L2MissEviction,        2   )
    DECLARE_ENUM_VAL(   L2EV_L2MissDFetch,          3   )
    DECLARE_ENUM_VAL(   L2EV_L2MissIFetch,          4   )
    DECLARE_ENUM_VAL(   L2EV_L2MissStore,           5   )
    DECLARE_ENUM_VAL(   L2EV_L2HitDFetch,           6   )
    DECLARE_ENUM_VAL(   L2EV_L2HitIFetch,           7   )
    DECLARE_ENUM_VAL(   L2EV_L2HitStore,            8   )
    DECLARE_ENUM_VAL(   L2EV_L2ReadAfterWrite,      9   )
    DECLARE_ENUM_VAL(   L2EV_L2WriteAfterWrite,    10   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterCmd,         11   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterRead,        12   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterRWITM,       13   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterDclaim,      14   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterWrite,       15   )
    DECLARE_ENUM_VAL(   L2EV_PlbReadOccupancy,     16   ) /* L2C_EVENT[16:19] */
    DECLARE_ENUM_VAL(   L2EV_PlbMasterIntvnM,      17   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterIntvnS,      18   )
    DECLARE_ENUM_VAL(   L2EV_PlbMasterMemData,     19   )
    DECLARE_ENUM_VAL(   L2EV_PlbSnoopCmd,          20   )
    DECLARE_ENUM_VAL(   L2EV_PlbSnoopL2Cmd,        21   )
    DECLARE_ENUM_VAL(   L2EV_PlbSnoopHitIntvn,     22   )
    DECLARE_ENUM_VAL(   L2EV_PlbSnoopHit,          23   )
    DECLARE_ENUM_VAL(   L2EV_PlbSnoopRetry,        24   )
END_ENUM( L2C_PMUREG )

BEGIN_ENUM( L2C_PMUREG )
    DECLARE_ENUM_VAL(   L2C_PMUGS0,             0x000   )
    DECLARE_ENUM_VAL(   L2C_PMUGC0,             0x004   )
    DECLARE_ENUM_VAL(   L2C_PMUIS0,             0x010   )
    DECLARE_ENUM_VAL(   L2C_PMUIE0,             0x014   )
    DECLARE_ENUM_VAL(   L2C_PMUREVID,           0xC00   )
    DECLARE_ENUM_VAL(   L2C_PMULC0,             0x800   )
    DECLARE_ENUM_VAL(   L2C_PMUC0,              0x808   )
    DECLARE_ENUM_VAL(   L2C_PMULC1,             0x810   )
    DECLARE_ENUM_VAL(   L2C_PMUC1,              0x818   )
    DECLARE_ENUM_VAL(   L2C_PMULC2,             0x820   )
    DECLARE_ENUM_VAL(   L2C_PMUC2,              0x828   )
    DECLARE_ENUM_VAL(   L2C_PMULC3,             0x830   )
    DECLARE_ENUM_VAL(   L2C_PMUC3,              0x838   )
    DECLARE_ENUM_VAL(   L2C_PMULC4,             0x900   )
    DECLARE_ENUM_VAL(   L2C_PMUC4,              0x908   )
    DECLARE_ENUM_VAL(   L2C_PMULC5,             0x910   )
    DECLARE_ENUM_VAL(   L2C_PMUC5,              0x918   )
    DECLARE_ENUM_VAL(   L2C_PMULC6,             0x920   )
    DECLARE_ENUM_VAL(   L2C_PMUC6,              0x928   )
    DECLARE_ENUM_VAL(   L2C_PMULC7,             0x930   )
    DECLARE_ENUM_VAL(   L2C_PMUC7,              0x938   )
    DECLARE_ENUM_VAL(   L2C_PMULC8,             0xA00   )
    DECLARE_ENUM_VAL(   L2C_PMUC8,              0xA08   )
    DECLARE_ENUM_VAL(   L2C_PMULC9,             0xA10   )
    DECLARE_ENUM_VAL(   L2C_PMUC9,              0xA18   )
    DECLARE_ENUM_VAL(   L2C_PMULC10,            0xA20   )
    DECLARE_ENUM_VAL(   L2C_PMUC10,             0xA28   )
    DECLARE_ENUM_VAL(   L2C_PMULC11,            0xA30   )
    DECLARE_ENUM_VAL(   L2C_PMUC11,             0xA38   )
    DECLARE_ENUM_VAL(   L2C_PMULC12,            0xB00   )
    DECLARE_ENUM_VAL(   L2C_PMUC12,             0xB08   )
    DECLARE_ENUM_VAL(   L2C_PMULC13,            0xB10   )
    DECLARE_ENUM_VAL(   L2C_PMUC13,             0xB18   )
    DECLARE_ENUM_VAL(   L2C_PMULC14,            0xB20   )
    DECLARE_ENUM_VAL(   L2C_PMUC14,             0xB28   )
    DECLARE_ENUM_VAL(   L2C_PMULC15,            0xB30   )
    DECLARE_ENUM_VAL(   L2C_PMUC15,             0xB38   )
END_ENUM( L2C_PMUREG )


#ifdef __ASSEMBLER__
.macro l2c_pmu_write    reg_base, reg_l2reg, reg_value, reg_tmp
    addi    \reg_tmp, \reg_base, L2C_PMUDCRAI
    mtdcrx  \reg_tmp, \reg_l2reg
    addi    \reg_tmp, \reg_base, L2C_PMUDCRDI
    mtdcrx  \reg_tmp, \reg_value
.endm

.macro l2c_pmu_read     reg_result, reg_base, reg_l2reg
    addi    \reg_result, \reg_base, L2C_PMUDCRAI
    mtdcrx  \reg_result, \reg_l2reg
    addi    \reg_result, \reg_base, L2C_PMUDCRDI
    mfdcrx  \reg_result, \reg_result
.endm
#else
#include <arch/ppc_476fp_lib_c.h>
static inline void l2c_pmu_write( uint32_t const base_addr, L2C_PMUREG const reg, uint32_t const value ) {
    dcr_write( base_addr+L2C_PMUDCRAI, (uint32_t const)reg );
    dcr_write( base_addr+L2C_PMUDCRDI, value );
}

static inline uint32_t l2c_pmu_read( uint32_t const base_addr, L2C_PMUREG const reg ) {
    dcr_write( base_addr+L2C_PMUDCRAI, (uint32_t const)reg );
    return dcr_read( base_addr+L2C_PMUDCRDI );
}
#endif


#endif /* REGS_REGS_L2C_PMU_H */
