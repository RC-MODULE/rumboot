/*
 * test_oi10_cpu_021_pmu_reg.c
 *
 *  Created on: Oct 26, 2018
 *      Author: a.gurov
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
#define RUMBOOT_ASSERT_WARN_ONLY
*/

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <arch/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/devices/emi.h>



/* --- Config --- */
#define PMU_VERSION                 1
#define PMU_REVISION                2
#define CHECK_BASE                  SRAM1_BASE
#define CHECK_EVENT                 L2EV_L2LwarxComplete
#define EVENTS_COUNT                3

/* --- Constants --- */
#define NOTHING                     0x00000000
#define TEST_OK                     NOTHING
#define TEST_ERROR                  !TEST_OK
#define DISABLED                    NOTHING
#define ENABLED                     !DISABLED
#define NO                          DISABLED
#define YES                         ENABLED
#define CHECK_TPL_SIZE              (4*3*2)
#define PMUC_MIN                    0
#define PMUC_MAX                    15
#define EVM                         ((1 << L2C_PMULCx_EVENT_n) - 1)
#define PMUCC0_FAC                  0x80000000
#define PMUGC0_PMCC                 (1 << L2C_PMUGC0_PMCC_i)
#define PMUGC0_LFAC                 (1 << L2C_PMUGC0_LFAC_i)
#define PMULCx_FC                   (1 << L2C_PMULCx_FC_i)
#define CMODE_INDEPENDENT           DISABLED
#define CMODE_CHAINED               ENABLED

/* --- Other --- */
#define ALIGNED(AV)                 __attribute__((aligned(AV)))
#define MAY_BE_NOT_USED             __attribute__((unused))
#define ALWAYS_INLINE               __attribute__((always_inline))


/* --- Functions --- */
#define BIT(N)                      (1 << (N))
        /* Nibbles to byte */
#define N2B(B1,B2)                  ((((B2) & 0x0F) << 4) | ((B1) & 0x0F))
        /* Byte to nibble  */
#define B2N(B,N)                    ((N)&1 ? ((B)>>4) & 0x0F : (B)& 0x0F)
#define CNT_STATE_MSK(C)            (reg_field((C), 1))
#define RANGE_ARG(BEG,END)          (range1616_t){.beg=(BEG),.end=(END)}
#define FORCE_USE(VAR)              VAR ^=0
#define PMU_WRITE(REG,VAL)          l2c_pmu_write(DCR_L2C_BASE,REG,VAL)
#define PMU_READ(REG)               l2c_pmu_read(DCR_L2C_BASE,REG)
#define PMU_CMP_EQ(REG,VAL)         (PMU_READ(REG) == (VAL))
#define PMU_CMP_NE(REG,VAL)         (PMU_READ(REG) != (VAL))
#define PMU_SET(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) |  (VAL))
#define PMU_CLR(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) & ~(VAL))
#define PMU_INV(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) ^  (VAL))
#define SPR_SET(REG,VAL)            spr_write((REG), spr_read(REG) |  (VAL))
#define SPR_CLR(REG,VAL)            spr_write((REG), spr_read(REG) & ~(VAL))
#define SPR_INV(REG,VAL)            spr_write((REG), spr_read(REG) ^  (VAL))
#define PMULCx(CMODE,FC,    \
        FCS,FCU,FCM1,FCM0,  \
        CE,EVENT)                   ((((CMODE) & 1  ) << L2C_PMULCx_CMODE_i)  | \
                                     (((FC)    & 1  ) << L2C_PMULCx_FC_i   )  | \
                                     (((FCS)   & 1  ) << L2C_PMULCx_FCS_i  )  | \
                                     (((FCU)   & 1  ) << L2C_PMULCx_FCU_i  )  | \
                                     (((FCM1)  & 1  ) << L2C_PMULCx_FCM1_i )  | \
                                     (((FCM0)  & 1  ) << L2C_PMULCx_FCM0_i )  | \
                                     (((CE)    & 1  ) << L2C_PMULCx_CE_i   )  | \
                                     (((EVENT) & EVM) << L2C_PMULCx_EVENT_i))
#define PMULCx_EVENT(EN,EVENT)      PMULCx(0,0,0,0,0,0,EN,EVENT)
#define ARRAY_END(ARRAY)            (ARRAY_SIZE(ARRAY) - 1)
#define MK_MASK(SIZE)               ((1 << (SIZE)) - 1)
#define MK_REVID(VER,REV)           ((((VER)                                    \
                                           & MK_MASK(L2C_PMUREVID_VERSION_n))   \
                                          << L2C_PMUREVID_VERSION_i)          | \
                                     (((REV)                                    \
                                           & MK_MASK(L2C_PMUREVID_REVISION_n))  \
                                          << L2C_PMUREVID_REVISION_i))
#define CK_VAL(S,R,V,E)              S |= (R = ((V) != (E)))

/* --- Registers defaults values --- */
#define PMUGS0_DFLT                 (BIT(L2C_PMUGS0_CSTAT0_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT1_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT2_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT3_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT4_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT5_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT6_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT7_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT8_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT9_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT10_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT11_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT12_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT13_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT14_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT15_i ) |   \
                                     BIT(L2C_PMUGS0_EXTSTAT_i))
#define PMUGC0_DFLT                 0x00000000
#define PMUIS0_DFLT                 0x00000000
#define PMUIE0_DFLT                 0x00000000
#define PMUREVID_DFLT               MK_REVID(PMU_VERSION,PMU_REVISION)
#define PMUCx_DFLT                  0x00000000
#define PMULCx_DFLT                 0x00000000

/* --- Registers masks --- */
#define PMUGS0_MASK                 (BIT(L2C_PMUGS0_CSTAT0_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT1_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT2_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT3_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT4_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT5_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT6_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT7_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT8_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT9_i  ) |   \
                                     BIT(L2C_PMUGS0_CSTAT10_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT11_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT12_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT13_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT14_i ) |   \
                                     BIT(L2C_PMUGS0_CSTAT15_i ) |   \
                                     BIT(L2C_PMUGS0_ORSTAT_i  ) |   \
                                     BIT(L2C_PMUGS0_SRSTAT_i  ) |   \
                                     BIT(L2C_PMUGS0_EXTSTAT_i ) |   \
                                     BIT(L2C_PMUGS0_CPUFAC_i  ) |   \
                                     BIT(L2C_PMUGS0_CPUPR_i   ) |   \
                                     BIT(L2C_PMUGS0_CPUPMM_i  ))
#define PMUGC0_MASK                 (BIT(L2C_PMUGC0_MSRME_i   ) |   \
                                     BIT(L2C_PMUGC0_MSRCE_i   ) |   \
                                     BIT(L2C_PMUGC0_MORFCE_i  ) |   \
                                     BIT(L2C_PMUGC0_MORAE_i   ) |   \
                                     BIT(L2C_PMUGC0_LFAC_i    ) |   \
                                     BIT(L2C_PMUGC0_FCEC_i    ))
#define PMUIS0_MASK                 (BIT(L2C_PMUIS0_ISTAT0_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT1_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT2_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT3_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT4_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT5_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT6_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT7_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT8_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT9_i  ) |   \
                                     BIT(L2C_PMUIS0_ISTAT10_i ) |   \
                                     BIT(L2C_PMUIS0_ISTAT11_i ) |   \
                                     BIT(L2C_PMUIS0_ISTAT12_i ) |   \
                                     BIT(L2C_PMUIS0_ISTAT13_i ) |   \
                                     BIT(L2C_PMUIS0_ISTAT14_i ) |   \
                                     BIT(L2C_PMUIS0_ISTAT15_i ))
#define PMUIE0_MASK                 (BIT(L2C_PMUIE0_IE0_i     ) |   \
                                     BIT(L2C_PMUIE0_IE1_i     ) |   \
                                     BIT(L2C_PMUIE0_IE2_i     ) |   \
                                     BIT(L2C_PMUIE0_IE3_i     ) |   \
                                     BIT(L2C_PMUIE0_IE4_i     ) |   \
                                     BIT(L2C_PMUIE0_IE5_i     ) |   \
                                     BIT(L2C_PMUIE0_IE6_i     ) |   \
                                     BIT(L2C_PMUIE0_IE7_i     ) |   \
                                     BIT(L2C_PMUIE0_IE8_i     ) |   \
                                     BIT(L2C_PMUIE0_IE9_i     ) |   \
                                     BIT(L2C_PMUIE0_IE10_i    ) |   \
                                     BIT(L2C_PMUIE0_IE11_i    ) |   \
                                     BIT(L2C_PMUIE0_IE12_i    ) |   \
                                     BIT(L2C_PMUIE0_IE13_i    ) |   \
                                     BIT(L2C_PMUIE0_IE14_i    ) |   \
                                     BIT(L2C_PMUIE0_IE15_i    ))
#define PMUREVID_MASK               MK_REVID(~0,~0)
#define PMUCx_MASK                  0xFFFFFFFF
#define PMULCx_MASK                 (BIT(L2C_PMULCx_CMODE_i   ) |   \
                                     BIT(L2C_PMULCx_FC_i      ) |   \
                                     BIT(L2C_PMULCx_FCS_i     ) |   \
                                     BIT(L2C_PMULCx_FCU_i     ) |   \
                                     BIT(L2C_PMULCx_FCM1_i    ) |   \
                                     BIT(L2C_PMULCx_FCM0_i    ) |   \
                                     BIT(L2C_PMULCx_CE_i      ) |   \
                                     (MK_MASK(L2C_PMULCx_EVENT_n)   \
                                           << L2C_PMULCx_EVENT_i))


/* --- Structures and types --- */
union ppc6432_t
{
    uint64_t        u64;
    struct
    {
        uint32_t    h,l;
    }               u32;
};

struct pmu_counter_t
{
    L2C_PMUREG  ctl, ctr;
};

struct pmu_hwconf_t
{
    const
    L2C_PMUREG  *cnt_list;
    const
    L2C_PMUREG  *ctl_list;
    uint32_t     cnt;
    uint32_t     not_used;
};

struct check_default_entry_t
{
    L2C_PMUREG  reg;    /* Register address */
    uint32_t    exp;    /* Expect value     */
    int         idx;    /* Index            */
    char       *nam;    /* Register name    */
};

typedef union  ppc6432_t ppc6432_t;

/* Global vars */
static const tlb_entry em2_entries[] =
{
    {   /*            ERPN,RPN,    EPN,    DSIZ    */
        MMU_TLB_ENTRY(0x00,0x00000,0x00000,MMU_TLBE_DSIZ_1GB,
        /*  IL1I,IL1D,W,I,M,G,E */
            0,   0,   0,0,1,0,MMU_TLBE_E_BIG_END,
        /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */
            0, 1, 1,  0, 1, 1, 0,    0,    0, MEM_WINDOW_0,
        /*  WAY,            BID,             V     */
            MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)
    },
    {   /*            ERPN,RPN,    EPN,    DSIZ    */
        MMU_TLB_ENTRY(0x00,0x40000,0x40000,MMU_TLBE_DSIZ_1GB,
        /*  IL1I,IL1D,W,I,M,G,E */
            0,   0,   0,0,1,0,MMU_TLBE_E_BIG_END,
        /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */
            0, 1, 1,  0, 1, 1, 0,    0,    0, MEM_WINDOW_0,
        /*  WAY,            BID,             V     */
            MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)
    }
};

static const L2C_PMUREG pmuc_dcr[] =
{
    L2C_PMUC0,      L2C_PMUC1,      L2C_PMUC2,      L2C_PMUC3,
    L2C_PMUC4,      L2C_PMUC5,      L2C_PMUC6,      L2C_PMUC7,
    L2C_PMUC8,      L2C_PMUC9,      L2C_PMUC10,     L2C_PMUC11,
    L2C_PMUC12,     L2C_PMUC13,     L2C_PMUC14,     L2C_PMUC15
};

static const L2C_PMUREG pmulc_dcr[] =
{
    L2C_PMULC0,     L2C_PMULC1,     L2C_PMULC2,     L2C_PMULC3,
    L2C_PMULC4,     L2C_PMULC5,     L2C_PMULC6,     L2C_PMULC7,
    L2C_PMULC8,     L2C_PMULC9,     L2C_PMULC10,    L2C_PMULC11,
    L2C_PMULC12,    L2C_PMULC13,    L2C_PMULC14,    L2C_PMULC15
};

static const uint32_t pairwise_independency_templates[] =
{
    0xFFFFFFFF, 0x00000000,
    0xFFFF0000, 0x0000FFFF,
    0xFF00FF00, 0x00FF00FF,
    0xF0F0F0F0, 0x0F0F0F0F,

    0xCCCCCCCC, 0x33333333,
    0xCCCC3333, 0x3333CCCC,
    0xCC33CC33, 0x33CC33CC,
    0xC3C3C3C3, 0x3C3C3C3C,

    0xAAAAAAAA, 0x55555555,
    0xAAAA5555, 0x5555AAAA,
    0xAA55AA55, 0x55AA55AA,
    0xA5A5A5A5, 0x5A5A5A5A,
};

static const uint32_t reset_check_templates[] =
{
    0x01234567, 0x12345678, 0x23456789, 0x3456789A,
    0x456789AB, 0x56789ABC, 0x6789ABCD, 0x789ABCDE,
    0x89ABCDEF, 0x9ABCDEF0, 0xABCDEF01, 0xBCDEF012,
    0xCDEF0123, 0xDEF01234, 0xEF012345, 0xF0123456,
};

static MAY_BE_NOT_USED
void pmu_dump_counters(struct pmu_hwconf_t *hw)
{
    int i;
    for(i = 0; i < (hw->cnt); i++)
        rumboot_printf("Counter #%d is 0x%X\n", i,
                PMU_READ(hw->cnt_list[i]));

}

static inline ALWAYS_INLINE
void pmu_reset_all_counters(void)
{
    PMU_SET(L2C_PMUGC0, PMUGC0_PMCC);
}

static inline ALWAYS_INLINE
void pmu_freeze_all_counters(uint32_t freeze)
{
    if(freeze)  PMU_SET(L2C_PMUGC0, PMUGC0_LFAC);
    else        PMU_CLR(L2C_PMUGC0, PMUGC0_LFAC);
}

static inline ALWAYS_INLINE
void pmu_freeze_counter(struct pmu_hwconf_t *hw,
                               uint32_t      counter,
                               uint32_t      freeze)
{
    if(freeze)  PMU_SET(hw->ctl_list[counter], PMULCx_FC);
    else        PMU_CLR(hw->ctl_list[counter], PMULCx_FC);
}

static
void pmu_init_counters(struct pmu_hwconf_t *hw)
{
    int i;
    SPR_CLR(SPR_CCR2, 1 << CTRL_CCR2_PMUD_i);
    SPR_CLR(SPR_PMUCC0, PMUCC0_FAC);
    pmu_freeze_all_counters(ENABLED);
    for(i = 0; i < (hw->cnt); i++)
        PMU_WRITE(hw->ctl_list[i],
                PMULCx( CMODE_INDEPENDENT,  /* CMODE                    */
                        YES,                /* Freeze counter           */
                        NO,                 /* Freeze in system mode    */
                        NO,                 /* Freeze in user mode      */
                        NO,                 /* Freeze if PMU_C476PMM=1  */
                        NO,                 /* Freeze if PMU_C476PMM=0  */
                        ENABLED,            /* Condition enabled        */
                        CHECK_EVENT));
}

static
uint32_t pmu_check_reg(L2C_PMUREG rega, uint32_t regm)
{
    uint32_t    result = TEST_OK,
                status = TEST_OK,
                redval = 0,
                needed = 0,
                backup = 0;

    int         i;

    backup = PMU_READ(rega);
    for(i = 0; i < CHECK_TPL_SIZE; i++)
    {
        needed = pairwise_independency_templates[i] & regm;
        PMU_WRITE(rega, needed);
        isync();
        redval = PMU_READ(rega) & regm;
        CK_VAL(status, result, redval, needed);
        if(result)
            rumboot_printf(
                    "Error in register: "
                    "EXPECTED=0x%X, ACTUAL=0x%X\n",
                    needed, redval);
        TEST_ASSERT(!result, "DCR read/write error!\n");
    }
    PMU_WRITE(rega, backup);
    return status;
}

static
uint32_t pmu_check_default(struct check_default_entry_t *info)
{
    uint32_t    status  = TEST_OK,
                result  = TEST_OK,
                regval  = 0;
    static
    char       *title0 = "Register '%s' has value 0x%X,"
                         " but expected 0x%X\n",
               *title1 = "Register '%s%d' has value 0x%X,"
                         " but expected 0x%X\n";

    regval = PMU_READ(info->reg);
    CK_VAL(status, result, regval, info->exp);
    if(result)
    {
        if(info->idx == -1)
            rumboot_printf(title0, info->nam, /* ---- */ regval, info->exp);
        else
            rumboot_printf(title1, info->nam, info->idx, regval, info->exp);
    }
    TEST_ASSERT(!result, "ASSERT: UNEXPECTED DEFAULT VALUE!\n");

    return status;
}

static
uint32_t pmu_check__defaults(struct pmu_hwconf_t *hw, void *data)
{
    uint32_t      status = TEST_OK;
    int           i;

    struct check_default_entry_t ctrinfo = {0, PMUCx_DFLT,  0, "L2C_PMUC"},
                                 ctlinfo = {0, PMULCx_DFLT, 0, "L2C_PMULC"};

    static
    struct check_default_entry_t regs[] =
    { /*     Register     | Default       |IDX|  Name            */
            {L2C_PMUGS0,    PMUGS0_DFLT,    0,  "L2C_PMUGS"   },
            {L2C_PMUGC0,    PMUGC0_DFLT,    0,  "L2C_PMUGC"   },
            {L2C_PMUIS0,    PMUIS0_DFLT,    0,  "L2C_PMUIS"   },
            {L2C_PMUIE0,    PMUIE0_DFLT,    0,  "L2C_PMUIE"   },
            {L2C_PMUREVID,  PMUREVID_DFLT, -1,  "L2C_PMUREVID"},
            {-1,            0x00000000,    -1,   NULL         }
    };


    rumboot_printf(" *** Check registers default values...\n");

    /* Global control and status registers */

    for(i = 0; regs[i].reg != -1; i++)
        status |= pmu_check_default(regs + i);

    /* Counters */
    for(ctrinfo.idx = 0; ctrinfo.idx < (hw->cnt); (ctrinfo.idx)++)
    {
        ctrinfo.reg = hw->cnt_list[ctrinfo.idx];
        status |= pmu_check_default(&ctrinfo);
    }

    /* Local control registers */
    for(ctlinfo.idx = 0; ctlinfo.idx < (hw->cnt); (ctlinfo.idx)++)
    {
        ctlinfo.reg = hw->ctl_list[ctlinfo.idx];
        status |= pmu_check_default(&ctlinfo);
    }

    rumboot_printf("%s!\n", !status ? "Success" : "Fail");
    return status;
}

static
uint32_t pmu_check__reset(struct pmu_hwconf_t *hw, void *data)
{
    uint32_t     status = TEST_OK;
    int          i;

    rumboot_printf(" *** Check reset all counters...\n");
    rumboot_printf(" +++ Filling counters...\n");
    for(i = 0; i < (hw->cnt); i++)
    {
        rumboot_printf(" +++ [%d] <-- 0x%X\n",
                i, reset_check_templates[i]);
        PMU_WRITE(hw->cnt_list[i], reset_check_templates[i]);
    }

    rumboot_printf(" +++ Doing the reset...\n");
    pmu_reset_all_counters();

    rumboot_printf(" +++ Comparing counters to zero equals...\n");
    for(i = 0; i < (hw->cnt); i++)
        status |= PMU_CMP_NE(hw->cnt_list[i], 0);

    TEST_ASSERT(!status, "Reset all counters check failed!\n");

    rumboot_printf("%s!\n", !status ? "Success" : "Fail");

    return status;
}

static
uint32_t pmu_check__regs_pmuc(struct pmu_hwconf_t *hw, void *data)
{
    uint32_t    status = TEST_OK,
                reg    = 0,
                beg    = 0,
                end    = hw->cnt - 1;

    rumboot_printf(" *** Check pairwise independence...\n");
    rumboot_printf(" *** ... of registers PMUC%d-PMUC%d...\n",
            beg, end);

    for(reg = beg; reg <= end; reg++)
    {
       rumboot_printf(" +++ PMUC%d...\n", reg);
       status |= pmu_check_reg(hw->cnt_list[reg], PMUCx_MASK);
    }
    rumboot_printf("%s!\n", !status ? "Success" : "Fail");
    return status;
}

uint32_t pmu_check__regs_pmulc(struct pmu_hwconf_t *hw, void *data)
{
    uint32_t    status = TEST_OK,
                reg    = 0,
                beg    = 0,
                end    = hw->cnt - 1;

    rumboot_printf(" *** Check pairwise independence...\n");
    rumboot_printf(" *** ... of registers PMULC%d-PMULC%d...\n",
            beg, end);

    for(reg = beg; reg <= end; reg++)
    {
        rumboot_printf(" +++ PMULC%d...\n", reg);
        status |= pmu_check_reg(hw->ctl_list[reg], PMULCx_MASK);
    }
    rumboot_printf("%s!\n", !status ? "Success" : "Fail");
    return status;
}

uint32_t pmu_check__regs_globl(struct pmu_hwconf_t *hw, void *data)
{
    uint32_t    status = TEST_OK;
    static
    char       *title  = " *** Check pairwise independence of %s...\n";

    rumboot_printf(title, "global control registers");
    rumboot_printf(title, "L2C_PMUGC0");
    status |= pmu_check_reg(L2C_PMUGC0, PMUGC0_MASK);
    rumboot_printf(title, "L2C_PMUIE0");
    status |= pmu_check_reg(L2C_PMUIE0, PMUIE0_MASK);

rumboot_printf("%s!\n", !status ? "Success" : "Fail");
    return status;
}

uint32_t main(void)
{
    uint32_t         status = TEST_OK,
                     revid  = 0,
                     idx    = 0;

    struct
    pmu_hwconf_t     hw     = { pmuc_dcr,
                                pmulc_dcr,
                                ARRAY_SIZE(pmuc_dcr),
                                NOTHING
                              };

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    msync();
    /* 'lwarx' and 'stwcx.' instructions needs a cache-enabled memory */
    write_tlb_entries(em2_entries, ARRAY_SIZE(em2_entries));
    for(idx = 0; idx < (128 / sizeof(uint32_t)); idx++)
        iowrite32(0, CHECK_BASE);
    rumboot_printf("Init done.\n");

    revid = PMU_READ(L2C_PMUREVID);
    rumboot_printf("L2C Perfomance Monitor Unit revision:%d.%d\n",
            (revid >> 8) & 0x0FFF, revid & 0xFF);

    status |= pmu_check__defaults   (&hw, (void*)(NOTHING ));

    rumboot_printf("Init counters...\n");
    pmu_freeze_all_counters(ENABLED);
    pmu_init_counters(&hw);
    pmu_reset_all_counters();
    rumboot_printf("Init done.\n");

    rumboot_printf("Test started...\n");

    status |= pmu_check__reset      (&hw, (void*)(NOTHING ));
    status |= pmu_check__regs_pmuc  (&hw, (void*)(NOTHING ));
    status |= pmu_check__regs_pmulc (&hw, (void*)(NOTHING ));
    status |= pmu_check__regs_globl (&hw, (void*)(NOTHING ));
 

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}



