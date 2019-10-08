/*
 * l2c_pmu.h
 *
 *  Created on: Jan 28, 2019
 *      Author: a.gurov
 */

#ifndef REGS_FIELDS_L2C_PMU_H
#define REGS_FIELDS_L2C_PMU_H

#include <rumboot/macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


BEGIN_ENUM( L2C_PMUGS0_FIELD )  /* Global Status Register 0 */
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT0_e,  0  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT1_e,  1  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT2_e,  2  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT3_e,  3  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT4_e,  4  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT5_e,  5  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT6_e,  6  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT7_e,  7  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT8_e,  8  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT9_e,  9  )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT10_e, 10 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT11_e, 11 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT12_e, 12 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT13_e, 13 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT14_e, 14 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT15_e, 15 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_ORSTAT_e,  26 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_SRSTAT_e,  27 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_EXTSTAT_e, 28 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUFAC_e,  29 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUPR_e,   30 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUPMM_e,  31 )
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT0_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT0_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT1_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT1_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT2_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT2_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT3_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT3_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT4_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT4_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT5_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT5_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT6_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT6_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT7_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT7_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT8_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT8_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT9_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT9_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT10_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT10_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT11_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT11_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT12_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT12_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT13_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT13_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT14_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT14_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CSTAT15_i, IBM_BIT_INDEX(32, L2C_PMUGS0_CSTAT15_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_ORSTAT_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_ORSTAT_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_SRSTAT_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_SRSTAT_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_EXTSTAT_i, IBM_BIT_INDEX(32, L2C_PMUGS0_EXTSTAT_e ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUFAC_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CPUFAC_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUPR_i,   IBM_BIT_INDEX(32, L2C_PMUGS0_CPUPR_e   ))
    DECLARE_ENUM_VAL( L2C_PMUGS0_CPUPMM_i,  IBM_BIT_INDEX(32, L2C_PMUGS0_CPUPMM_e  ))
END_ENUM( L2C_PMUGS0_FIELD )

BEGIN_ENUM( L2C_PMUGC0_FIELD )    /* Global Control Register 0 */
    /* PMU master counter clear                            */
    DECLARE_ENUM_VAL( L2C_PMUGC0_PMCC_e,    15 )
    /* Multiple PMU start ring master enable               */
    DECLARE_ENUM_VAL( L2C_PMUGC0_MSRME_e,   20 )
    /* Multiple PMU start ring counter enable              */
    DECLARE_ENUM_VAL( L2C_PMUGC0_MSRCE_e,   21 )
    /* Multiple PMU overflow ring freeze counter enable    */
    DECLARE_ENUM_VAL( L2C_PMUGC0_MORFCE_e,  22 )
    /* Multiple PMU overflow ring assert enable            */
    DECLARE_ENUM_VAL( L2C_PMUGC0_MORAE_e,   23 )
    /* Local freeze all counters                           */
    DECLARE_ENUM_VAL( L2C_PMUGC0_LFAC_e,    30 )
    /* Freeze counters on enabled condition                */
    DECLARE_ENUM_VAL( L2C_PMUGC0_FCEC_e,    31 )
    DECLARE_ENUM_VAL( L2C_PMUGC0_PMCC_i,    IBM_BIT_INDEX(32, L2C_PMUGC0_PMCC_e    ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_MSRME_i,   IBM_BIT_INDEX(32, L2C_PMUGC0_MSRME_e   ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_MSRCE_i,   IBM_BIT_INDEX(32, L2C_PMUGC0_MSRCE_e   ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_MORFCE_i,  IBM_BIT_INDEX(32, L2C_PMUGC0_MORFCE_e  ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_MORAE_i,   IBM_BIT_INDEX(32, L2C_PMUGC0_MORAE_e   ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_LFAC_i,    IBM_BIT_INDEX(32, L2C_PMUGC0_LFAC_e    ))
    DECLARE_ENUM_VAL( L2C_PMUGC0_FCEC_i,    IBM_BIT_INDEX(32, L2C_PMUGC0_FCEC_e    ))
END_ENUM( L2C_PMUGC0_FIELD )

BEGIN_ENUM( L2C_PMUIS0_FIELD )  /* Global Interrupt Status Register 0 */
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT0_e,  0  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT1_e,  1  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT2_e,  2  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT3_e,  3  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT4_e,  4  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT5_e,  5  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT6_e,  6  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT7_e,  7  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT8_e,  8  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT9_e,  9  )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT10_e, 10 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT11_e, 11 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT12_e, 12 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT13_e, 13 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT14_e, 14 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT15_e, 15 )
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT0_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT0_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT1_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT1_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT2_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT2_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT3_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT3_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT4_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT4_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT5_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT5_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT6_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT6_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT7_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT7_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT8_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT8_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT9_i,  IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT9_e  ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT10_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT10_e ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT11_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT11_e ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT12_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT12_e ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT13_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT13_e ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT14_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT14_e ))
    DECLARE_ENUM_VAL( L2C_PMUIS0_ISTAT15_i, IBM_BIT_INDEX(32, L2C_PMUIS0_ISTAT15_e ))
END_ENUM( L2C_PMUIS0_FIELD )

BEGIN_ENUM( L2C_PMUIE0_FIELD )  /* Global Interrupt Enable Register 0 */
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE0_e,     0  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE1_e,     1  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE2_e,     2  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE3_e,     3  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE4_e,     4  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE5_e,     5  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE6_e,     6  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE7_e,     7  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE8_e,     8  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE9_e,     9  )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE10_e,    10 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE11_e,    11 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE12_e,    12 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE13_e,    13 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE14_e,    14 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE15_e,    15 )
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE0_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE0_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE1_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE1_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE2_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE2_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE3_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE3_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE4_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE4_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE5_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE5_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE6_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE6_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE7_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE7_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE8_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE8_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE9_i,     IBM_BIT_INDEX(32, L2C_PMUIE0_IE9_e     ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE10_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE10_e    ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE11_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE11_e    ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE12_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE12_e    ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE13_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE13_e    ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE14_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE14_e    ))
    DECLARE_ENUM_VAL( L2C_PMUIE0_IE15_i,    IBM_BIT_INDEX(32, L2C_PMUIE0_IE15_e    ))
END_ENUM( L2C_PMUIE0_FIELD )

BEGIN_ENUM( L2C_PMUREVID_FIELD )
    DECLARE_ENUM_VAL( L2C_PMUREVID_VERSION_e,   23 )
    DECLARE_ENUM_VAL( L2C_PMUREVID_REVISION_e,  31 )
    DECLARE_ENUM_VAL( L2C_PMUREVID_VERSION_n,   12 )
    DECLARE_ENUM_VAL( L2C_PMUREVID_REVISION_n,  8  )
    DECLARE_ENUM_VAL( L2C_PMUREVID_VERSION_i,   IBM_BIT_INDEX(32, L2C_PMUREVID_VERSION_e    ))
    DECLARE_ENUM_VAL( L2C_PMUREVID_REVISION_i,  IBM_BIT_INDEX(32, L2C_PMUREVID_REVISION_e   ))
END_ENUM( L2C_PMUREVID_FIELD )

BEGIN_ENUM( L2C_PMULCx_FIELD ) /* Local Counter Control Register (x=0...15) */
    DECLARE_ENUM_VAL( L2C_PMULCx_CMODE_e,   7  )
    DECLARE_ENUM_VAL( L2C_PMULCx_FC_e,      10 )
    DECLARE_ENUM_VAL( L2C_PMULCx_FCS_e,     11 )
    DECLARE_ENUM_VAL( L2C_PMULCx_FCU_e,     12 )
    DECLARE_ENUM_VAL( L2C_PMULCx_FCM1_e,    13 )
    DECLARE_ENUM_VAL( L2C_PMULCx_FCM0_e,    14 )
    DECLARE_ENUM_VAL( L2C_PMULCx_CE_e,      15 )
    DECLARE_ENUM_VAL( L2C_PMULCx_EVENT_e,   31 )
    DECLARE_ENUM_VAL( L2C_PMULCx_EVENT_n,   7  )
    DECLARE_ENUM_VAL( L2C_PMULCx_CMODE_i,   IBM_BIT_INDEX(32, L2C_PMULCx_CMODE_e   ))
    DECLARE_ENUM_VAL( L2C_PMULCx_FC_i,      IBM_BIT_INDEX(32, L2C_PMULCx_FC_e      ))
    DECLARE_ENUM_VAL( L2C_PMULCx_FCS_i,     IBM_BIT_INDEX(32, L2C_PMULCx_FCS_e     ))
    DECLARE_ENUM_VAL( L2C_PMULCx_FCU_i,     IBM_BIT_INDEX(32, L2C_PMULCx_FCU_e     ))
    DECLARE_ENUM_VAL( L2C_PMULCx_FCM1_i,    IBM_BIT_INDEX(32, L2C_PMULCx_FCM1_e    ))
    DECLARE_ENUM_VAL( L2C_PMULCx_FCM0_i,    IBM_BIT_INDEX(32, L2C_PMULCx_FCM0_e    ))
    DECLARE_ENUM_VAL( L2C_PMULCx_CE_i,      IBM_BIT_INDEX(32, L2C_PMULCx_CE_e      ))
    DECLARE_ENUM_VAL( L2C_PMULCx_EVENT_i,   IBM_BIT_INDEX(32, L2C_PMULCx_EVENT_e   ))
END_ENUM( L2C_PMULCx_FIELD )
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
BEGIN_ENUM( L2C_PMU_EVENT )
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
    /* C470S0_PMUCOMMITTEDCOUNT     */
    DECLARE_ENUM_VAL(   PPCEV_PMUCOMMITTEDCOOUNT,  25   ) /* PMU0_0_EVENT   */
    /* C470S0_PMUDCACHEHITCOUNT     */
    DECLARE_ENUM_VAL(   PPCEV_PMUDCACHEHITCOUNT,   26   ) /* PMU0_1_EVENT   */
    /* C470S0_PMUDTLBRELOADCOUNT    */
    DECLARE_ENUM_VAL(   PPCEV_PMUDTLBRELOADCOUNT,  27   ) /* PMU0_2_EVENT   */
    /* C470S0_PMUICACHEHITCOUNT     */
    DECLARE_ENUM_VAL(   PPCEV_PMUICACHEHITCOUNT,   28   ) /* PMU0_3_EVENT   */
    /* C470S0_PMUITLBRELOADCOUNT    */
    DECLARE_ENUM_VAL(   PPCEV_PMUITLBRELOADCOUNT,  29   ) /* PMU0_4_EVENT   */
    /* Unconnected */
    DECLARE_ENUM_VAL(   PPCEV_NC_5,                30   ) /* PMU0_5_EVENT   */
    /* Unconnected */
    DECLARE_ENUM_VAL(   PPCEV_NC_6,                31   ) /* PMU0_6_EVENT   */
END_ENUM( L2C_PMU_EVENT )


#endif /* REGS_FIELDS_L2C_PMU_H */
