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
#include <platform/interrupts.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <arch/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/devices/emi.h>

#include <platform/regs/sctl.h>


/* Configuration section*/
#define CHECK_RDWR_BASE         SRAM1_BASE /* Bank for R/W operations  */
#define CHECK_EXEC_BASE         0xFFF80000 /* Mirror virtual address   */
#define EXEC_EM2_BANK           SRAM1_BASE /* Bank for EXEC operations */
#define CHECK_EVENT             L2EV_L2LwarxComplete
#define EVENTS_COUNT            3
#define IRQ_FLAGS               (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define IRQ_TIMEOUT             50
#define BAD_VALUE               0xDEADFACE
#define WRITE_TPL               0x12345678
#define EVICTION_ADDR0          (SSRAM_BASE + 0x00000000)
#define EVICTION_ADDR1          (SSRAM_BASE + 0x00010000)
#define EVICTION_ADDR2          (SRAM1_BASE + 0x00000000)
#define EVICTION_ADDR3          (SRAM1_BASE + 0x00010000)
#define EVICTION_ADDR4          (SDRAM_BASE + 0x00000000)
#define CACHE_LINE_SZ           128

/* --- Constants --- */
#define NOTHING                 0x00000000
#define TEST_OK                 NOTHING
#define TEST_ERROR              !TEST_OK
#define NO                      0
#define YES                     1
#define NO_COUNTER              (~0)
#define SHARED_WINDOW           MEM_WINDOW_SHARED
#define BLR_OPCODE              0x4E800020
#define PMUC_MIN                0
#define PMUC_MAX                15
#define PMUC_OVF_MASK           0x80000000
#define PMUC_OVF_VAL            (PMUC_OVF_MASK - 1)
#define EVM                     ((1 << L2C_PMULCx_EVENT_n) - 1)
#define PMUCC0_FAC              0x80000000
#define PMUGC0_PMCC             (1 << L2C_PMUGC0_PMCC_i)
#define PMUGC0_LFAC             (1 << L2C_PMUGC0_LFAC_i)
#define PMULCx_FC               (1 << L2C_PMULCx_FC_i)
#define PMU_EVENTS_MASK         (32 - 1)
#define PMULCx_RESET            0x00000000
#define EVEN_BITS               0xAAAAAAAA
#define ODD_BITS                0x55555555
#define CMODE_INDEPENDENT       0
#define CMODE_CHAINED           1
#define PRI_CTR                 1
#define SEC_CTR                 0
#define PMU_0_VALID             (1 << 0)
#define PMU_1_VALID             (1 << 1)
#define PMU_2_VALID             (1 << 2)
#define PMU_3_VALID             (1 << 3)
#define PMU_4_VALID             (1 << 4)
#define INC_STAGE1              0x19 /* Boolean array -> 0b011001 */
#define PMU_ALL_COUNTERS        ((1 << (PMUC_MAX + 1)) - 1)
#define PMU_EVEN_COUNTERS       ((PMU_ALL_COUNTERS ^ EVEN_BITS) \
                                    & PMU_ALL_COUNTERS)
#define PMU_ODD_COUNTERS        ((PMU_ALL_COUNTERS ^ ODD_BITS)  \
                                    & PMU_ALL_COUNTERS)
#define PMU_NO_INTERRUPT        0x00000000
        /* Interrupts enable mask */
#define PMU_ALL_INTERRUPTS_E    ((1 << L2C_PMUIE0_IE0_i )     | \
                                 (1 << L2C_PMUIE0_IE0_i )     | \
                                 (1 << L2C_PMUIE0_IE1_i )     | \
                                 (1 << L2C_PMUIE0_IE2_i )     | \
                                 (1 << L2C_PMUIE0_IE3_i )     | \
                                 (1 << L2C_PMUIE0_IE4_i )     | \
                                 (1 << L2C_PMUIE0_IE5_i )     | \
                                 (1 << L2C_PMUIE0_IE6_i )     | \
                                 (1 << L2C_PMUIE0_IE7_i )     | \
                                 (1 << L2C_PMUIE0_IE8_i )     | \
                                 (1 << L2C_PMUIE0_IE9_i )     | \
                                 (1 << L2C_PMUIE0_IE10_i)     | \
                                 (1 << L2C_PMUIE0_IE11_i)     | \
                                 (1 << L2C_PMUIE0_IE12_i)     | \
                                 (1 << L2C_PMUIE0_IE13_i)     | \
                                 (1 << L2C_PMUIE0_IE14_i)     | \
                                 (1 << L2C_PMUIE0_IE15_i))
        /* Interrupts state mask */
#define PMU_ALL_INTERRUPTS_S    ((1 << L2C_PMUIS0_ISTAT0_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT0_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT1_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT2_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT3_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT4_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT5_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT6_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT7_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT8_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT9_i )  | \
                                 (1 << L2C_PMUIS0_ISTAT10_i)  | \
                                 (1 << L2C_PMUIS0_ISTAT11_i)  | \
                                 (1 << L2C_PMUIS0_ISTAT12_i)  | \
                                 (1 << L2C_PMUIS0_ISTAT13_i)  | \
                                 (1 << L2C_PMUIS0_ISTAT14_i)  | \
                                 (1 << L2C_PMUIS0_ISTAT15_i))

/* --- Other --- */
#define ALIGNED(AV)             __attribute__((aligned(AV)))
#define MAY_BE_NOT_USED         __attribute__((unused))
#define ALWAYS_INLINE           __attribute__((always_inline))
#define NEVER_INLINE            __attribute__((noinline))
#define _GET_UPTIME             rumboot_platform_get_uptime

#define IM0_BOLTED_ENTRIES          { /* Enable execute in user mode */ \
                /*            ERPN,             RPN, */                 \
                MMU_TLB_ENTRY(0x010,            0x80000,                \
                /*  EPN,             DSIZ  */                           \
                    0x80000,         MMU_TLBE_DSIZ_64KB,                \
                /*  IL1I,IL1D,W,I,M,G,E */                              \
                    1,   1,   0,1,0,0,MMU_TLBE_E_BIG_END,               \
                /*  UX,UW,UR, SX,SW,SR, DULXE,IULXE,TS,TID  */          \
                    1, 1, 1,  1, 1, 1 , 0,    0,    0, SHARED_WINDOW,   \
                /*  WAY,             BID,            V      */          \
                    MMU_TLBWE_WAY_0, MMU_TLBWE_BE_1, 1)                 \
                                    },                                  \
                                    {                                   \
                /*            ERPN,           RPN, */                   \
                MMU_TLB_ENTRY(0x010,            0x80010,                \
                /*  EPN,             DSIZ  */                           \
                    0x80010,         MMU_TLBE_DSIZ_64KB,                \
                /*  IL1I,IL1D,W,I,M,G,E */                              \
                    1,   1,   0,1,0,0,MMU_TLBE_E_BIG_END,               \
                /*  UX,UW,UR, SX,SW,SR, DULXE,IULXE,TS,TID  */          \
                    1, 1, 1,  1, 1, 1,  0,    0,    0, SHARED_WINDOW,   \
                /*  WAY,             BID,            V      */          \
                    MMU_TLBWE_WAY_0, MMU_TLBWE_BE_2, 1)                 \
                                    }
#define EM2_ENTRIES                 { /* Enable L2 caching  */          \
                /*            ERPN, RPN,    EPN,    DSIZ    */          \
                MMU_TLB_ENTRY(0x000,0x00000,0x00000,MMU_TLBE_DSIZ_1GB,  \
                /*  IL1I,IL1D,W,I,M,G,E */                              \
                    0,   0,   0,0,1,0,MMU_TLBE_E_BIG_END,               \
                /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */           \
                    1, 1, 1,  1, 1, 1, 0,    0,    0, MEM_WINDOW_0,     \
                /*  WAY,            BID,             V     */           \
                    MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)                 \
                                    },                                  \
                                    {                                   \
                /*            ERPN, RPN,    EPN,    DSIZ    */          \
                MMU_TLB_ENTRY(0x000,0x40000,0x40000,MMU_TLBE_DSIZ_1GB,  \
                /*  IL1I,IL1D,W,I,M,G,E */                              \
                    0,   0,   0,0,1,0,MMU_TLBE_E_BIG_END,               \
                /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */           \
                    1, 1, 1,  1, 1, 1, 0,    0,    0, MEM_WINDOW_0,     \
                /*  WAY,            BID,             V     */           \
                    MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)                 \
                                    }
#define MIRROR_ENTRIES              { /* For caching and execution */   \
                /*             ERPN,           RPN,             */      \
                MMU_TLB_ENTRY(0x000, ADDR4TLB(EXEC_EM2_BANK),           \
                /*          EPN                                 */      \
                    ADDR4TLB(CHECK_EXEC_BASE),                          \
                    MMU_TLBE_DSIZ_64KB, /* <- DSIZ              */      \
                /*  IL1I,IL1D,   W,I,M,G, E                     */      \
                    0,   0,      1,0,1,0, MMU_TLBE_E_BIG_END,           \
                /*  UX,UW,UR, SX,SW,SR, DULXE,IULXE,TS          */      \
                    0, 0, 0,  1, 1, 1,  0,    0,    0,                  \
                /*  TID,         WAY,              BID,             V*/ \
                    MEM_WINDOW_0,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1)}

/* --- Functions --- */
#define BIT(B)                  (1 << (B))
#define PMU_WRITE(REG,VAL)      l2c_pmu_write(DCR_L2C_BASE,REG,VAL)
#define PMU_READ(REG)           l2c_pmu_read(DCR_L2C_BASE,REG)
#define PMU_CMP_EQ(REG,VAL)     (PMU_READ(REG) == (VAL))
#define PMU_CMP_NE(REG,VAL)     (PMU_READ(REG) != (VAL))
#define PMU_SET(REG,VAL)        PMU_WRITE((REG), PMU_READ(REG) |  (VAL))
#define PMU_CLR(REG,VAL)        PMU_WRITE((REG), PMU_READ(REG) & ~(VAL))
#define PMU_INV(REG,VAL)        PMU_WRITE((REG), PMU_READ(REG) ^  (VAL))
#define PMU_TST(REG,VAL)        !!(PMU_READ(REG) & (VAL))
#define PMU_INT_ENABLE1(N)      PMU_SET(L2C_PMUIE0, pmu_ie[N])
#define PMU_INT_DISABLE1(N)     PMU_CLR(L2C_PMUIE0, pmu_ie[N])
#define PMU_INT_DISABLE_ALL()   PMU_WRITE(L2C_PMUIE0, PMU_NO_INTERRUPT)
#define PMU_INT_CLEAR(I)        PMU_WRITE(L2C_PMUIS0, pmu_is[I])
#define PMU_SEL_CTR(N)          hw->cur = (N)
#define MSR_SET(VAL)            msr_write(       msr_read(   ) |  (VAL))
#define MSR_CLR(VAL)            msr_write(       msr_read(   ) & ~(VAL))
#define SPR_SET(REG,VAL)        spr_write((REG), spr_read(REG) |  (VAL))
#define SPR_CLR(REG,VAL)        spr_write((REG), spr_read(REG) & ~(VAL))
#define SPR_INV(REG,VAL)        spr_write((REG), spr_read(REG) ^  (VAL))
#define DCR_SET(REG,VAL)        dcr_write((REG), dcr_read(REG) |  (VAL))
#define DCR_CLR(REG,VAL)        dcr_write((REG), dcr_read(REG) & ~(VAL))
#define DCR_INV(REG,VAL)        dcr_write((REG), dcr_read(REG) ^  (VAL))
#define ADDR4TLB(ADDR)          (((ADDR) & 0xFFFFF000) >> 12)
#define PMULCx(CMODE,FC,    \
        FCS,FCU,FCM1,FCM0,  \
        CE,EVENT)               ((((CMODE) & 1  ) << L2C_PMULCx_CMODE_i)  | \
                                 (((FC)    & 1  ) << L2C_PMULCx_FC_i   )  | \
                                 (((FCS)   & 1  ) << L2C_PMULCx_FCS_i  )  | \
                                 (((FCU)   & 1  ) << L2C_PMULCx_FCU_i  )  | \
                                 (((FCM1)  & 1  ) << L2C_PMULCx_FCM1_i )  | \
                                 (((FCM0)  & 1  ) << L2C_PMULCx_FCM0_i )  | \
                                 (((CE)    & 1  ) << L2C_PMULCx_CE_i   )  | \
                                 (((EVENT) & EVM) << L2C_PMULCx_EVENT_i))
#define PMULCx_EVENT(FC,EVENT)  PMULCx(CMODE_INDEPENDENT,(FC),NO,NO,        \
                                                    NO,NO,YES,EVENT)
#define PMULCx_CHAINED(FC)      PMULCx(CMODE_CHAINED,NO,NO,NO,NO,NO,NO,     \
                                    NOTHING)
#define ARRAY_END(ARRAY)        (ARRAY_SIZE(ARRAY) - 1)
#define CHECK_RANGE(BASE,                                                   \
        MINO,MAXO,VAL)          (((VAL) >= ((BASE) + (MINO)))               \
                                    &&                                      \
                                 ((VAL) <= ((BASE) + (MAXO))))
#define SYSCALL(FN)             asm volatile("\nsc\n.long %0\n"::"i"(FN))


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
    uint32_t     cur;
    const
    L2C_PMUREG  *ctr_list;
    const
    L2C_PMUREG  *ctl_list;
    uint32_t     cnt;
};

struct chk_entry_t
{
    uint32_t (*func)(struct pmu_hwconf_t*, int);
    uint32_t   mask;
    uint32_t   _pad; /* align */
    char      *desc;
};

struct event_check_entry_t
{
    L2C_PMU_EVENT    event;
    uint32_t         minof;
    uint32_t         maxof;
    char            *descr;
};

/* FC, FCS, FCU, FCM1, FCM0 */
typedef enum
{
    freeze_none,
    freeze_fc,
    freeze_fcs,
    freeze_fcu,
    freeze_fcm1,
    freeze_fcm0,
    freeze_unknown_6, /* padding to 3 bits */
    freeze_unknown_7, /* padding to 3 bits */
} freeze_num_t;

typedef union  ppc6432_t            ppc6432_t;
typedef struct pmu_hwconf_t         pmu_hwconf_t;
typedef struct pmu_counter_t        pmu_counter_t;
typedef struct chk_entry_t          chk_entry_t;
typedef struct rumboot_irq_entry    rumboot_irq_entry;
typedef struct event_check_entry_t  event_check_entry_t;

/* Function pointer types */
typedef void(*syscall_func)(void);


/* Global vars */
volatile
static uint32_t           g_tmp      = 0;
volatile
static uint32_t           g_irq      = 0;
volatile
static uint32_t           g_cnt      = NO_COUNTER;
//static rumboot_irq_entry *irq_table  = NULL;
static const tlb_entry tlb_entries[] =
{
    IM0_BOLTED_ENTRIES,
    EM2_ENTRIES,
    MIRROR_ENTRIES
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

static const uint32_t eviction_addr[5] =
{
    EVICTION_ADDR0,
    EVICTION_ADDR1,
    EVICTION_ADDR2,
    EVICTION_ADDR3,
    EVICTION_ADDR4
};

static const struct
event_check_entry_t evlist[] =
{
  
   { PPCEV_PMUCOMMITTEDCOOUNT,  4, 64, "C470S0_PMUCOMMITTEDCOUNT" }, // 25

    { PPCEV_PMUDCACHEHITCOUNT,   2, 4,  "C470S0_PMUDCACHEHITCOUNT" }, // 26
 
     { PPCEV_PMUDTLBRELOADCOUNT,  2, 64, "C470S0_PMUDTLBRELOADCOUNT"}, // 27
 
    { PPCEV_PMUICACHEHITCOUNT,   2, 64, "C470S0_PMUICACHEHITCOUNT" }, // 28

   { PPCEV_PMUITLBRELOADCOUNT,  2, 64, "C470S0_PMUITLBRELOADCOUNT"}, // 29
    {-1,                         0, 0,   NULL                      }
};


static inline void l2c_evict(void)
{
    int i;
    dci(2);
    msync();
    for(i = 0; i < ARRAY_SIZE(eviction_addr); i++)
        memset((void*)(eviction_addr[i]), 0, CACHE_LINE_SZ);
    for(i = 0; i < ARRAY_SIZE(eviction_addr); i++)
        iowrite32( WRITE_TPL, eviction_addr[i]);
    msync();
}



static
void pmu_gen_common_event(uint32_t usermode)
{
    if(usermode)
        g_tmp = ioread32(CHECK_RDWR_BASE);
    else
    {
        isync();
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE);
        stwcx(~lwarx(CHECK_RDWR_BASE), CHECK_RDWR_BASE);
        isync();
    }
}

static
void pmu_gen_special_event(L2C_PMU_EVENT ev, uint32_t ctri)
{
    int i, neg = NO;
    uint32_t base;

    /* FOR DEBUG PURPOSE */
    spr_write(SPR_SPRG8, BIT(IBM_BIT_INDEX(32, ev & PMU_EVENTS_MASK)));

    /* When event number is negated
     *  - we are not enabling PPC event signal (below) */
    if(ev & BIT(31))
    {
        neg = YES;
        ev  = -ev;
    }

    switch(ev & PMU_EVENTS_MASK)
    {
    case L2EV_L2LwarxComplete:
    case L2EV_L2StwcxSuccess:
        pmu_gen_common_event(NO);
        break;
    case L2EV_L2MissEviction:
        l2c_evict();
        break;
    case L2EV_L2MissDFetch:
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE);
        dci(2);
        msync();
        g_tmp = ioread32(CHECK_RDWR_BASE);
        break;
    case L2EV_L2MissIFetch:
        base = CHECK_EXEC_BASE + (ctri * CACHE_LINE_SZ * 2);
        memset((void*)base, 0, CACHE_LINE_SZ);
        dci(2);
        msync();
        for(i = 0; i < (CACHE_LINE_SZ / sizeof(uint32_t)); i++)
            iowrite32(BLR_OPCODE, base + (sizeof(uint32_t) * i));
        msync();
        dci(2);
        msync();
        ((void(*)())base)();
        break;
    case L2EV_L2MissStore:
        l2c_evict();
        break;
    case L2EV_L2HitDFetch:
        pmu_gen_common_event(NO);
        break;
    case L2EV_L2HitIFetch:
        base = CHECK_EXEC_BASE + (ctri * CACHE_LINE_SZ * 2)
                + CACHE_LINE_SZ;
        memset((void*)base, 0, CACHE_LINE_SZ);
        for(i = 0; i < (CACHE_LINE_SZ / sizeof(uint32_t)); i++)
            iowrite32(BLR_OPCODE, base + (sizeof(uint32_t) * i));
        ((void(*)())base)();
        break;
    case L2EV_L2HitStore:
    case L2EV_L2ReadAfterWrite:
        pmu_gen_common_event(NO);
        pmu_gen_common_event(NO);
        break;
    case L2EV_L2WriteAfterWrite:
        base = CHECK_RDWR_BASE + (ctri * CACHE_LINE_SZ);
        memset((void*)base, 0, CACHE_LINE_SZ);
        dci(2);
        msync();
        for(i = 0; i < (CACHE_LINE_SZ / sizeof(uint32_t)); i++)
            iowrite32(WRITE_TPL, base + (sizeof(uint32_t) * i));
        break;
    case L2EV_PlbMasterCmd:
        iowrite32( WRITE_TPL, CHECK_RDWR_BASE);
        break;
    case L2EV_PlbMasterRead:
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE);
        dci(2);
        msync();
        g_tmp = ioread32(CHECK_RDWR_BASE);
        break;
    case L2EV_PlbMasterRWITM:
        l2c_evict();
        break;
    /* Testing unimplemented (Reserved for future) */
    case L2EV_PlbMasterDclaim:
    case L2EV_PlbMasterWrite:
    case L2EV_PlbReadOccupancy:
    case L2EV_PlbMasterIntvnM:
    case L2EV_PlbMasterIntvnS:
    case L2EV_PlbMasterMemData:
    case L2EV_PlbSnoopCmd:
    case L2EV_PlbSnoopL2Cmd:
    case L2EV_PlbSnoopHitIntvn:
    case L2EV_PlbSnoopHit:
    case L2EV_PlbSnoopRetry:
        break;
    /* PPC events */
    /* Remark:  'neg==YES' means, that PPC events signals
     *          will not be enabled in SCTL register.
     */
    case PPCEV_PMUCOMMITTEDCOOUNT:
        memset((void*)CHECK_RDWR_BASE, 0, CACHE_LINE_SZ);
        /* Enable event count */
        if(!neg) DCR_SET(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_0_VALID);
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE);
        g_tmp = ioread32(CHECK_RDWR_BASE);
        /* Disable event count */
        if(!neg) DCR_CLR(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_0_VALID);
        break;
    case PPCEV_PMUDCACHEHITCOUNT:
        base = CHECK_RDWR_BASE;
        memset((void*)base, 0, CACHE_LINE_SZ);
        /* Enable event count */
        if(!neg) DCR_SET(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_1_VALID);
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE + 0);
        iowrite32(WRITE_TPL, CHECK_RDWR_BASE + 4);
        g_tmp = ioread32(CHECK_RDWR_BASE + 0);
        g_tmp = ioread32(CHECK_RDWR_BASE + 4);
        /* Disable event count */
        if(!neg) DCR_CLR(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_1_VALID);
        break;
    case PPCEV_PMUDTLBRELOADCOUNT:
        /* Enable event count */
        if(!neg) DCR_SET(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_2_VALID);
        for(i = 0; i < 4; i++)
        {
            isync();
            g_tmp++;
        }
        /* Disable event count */
        if(!neg) DCR_CLR(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_2_VALID);
        break;
    case PPCEV_PMUICACHEHITCOUNT:
        base = CHECK_EXEC_BASE;
        memset((void*)base, 0, CACHE_LINE_SZ);
        /* Enable event count */
        if(!neg) DCR_SET(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_3_VALID);
        for(i = 0; i < (CACHE_LINE_SZ  /  sizeof(uint32_t)); i++)
            iowrite32(BLR_OPCODE, base + (sizeof(uint32_t) * i));
        for(i = 0; i < 4; i++) ((void(*)())base)();
        /* Disable event count */
        if(!neg) DCR_CLR(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_3_VALID);
        break;
    case PPCEV_PMUITLBRELOADCOUNT:
        base = CHECK_EXEC_BASE;
        memset((void*)base, 0, CACHE_LINE_SZ);
        /* Enable event count */
        if(!neg) DCR_SET(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_4_VALID);
        for(i = 0; i < (CACHE_LINE_SZ  /  sizeof(uint32_t)); i++)
            iowrite32(BLR_OPCODE, base + (sizeof(uint32_t) * i));
        for(i = 0; i < (CACHE_LINE_SZ  /  sizeof(uint32_t)); i++)
        {
            isync();
            ((void(*)())base)();
        }
        /* Disable event count */
        if(!neg) DCR_CLR(DCR_SCTL_BASE + SCTL_PPC_PMU_CTRL, PMU_4_VALID);
        break;
    default:
        break;
    }
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
    SPR_CLR(SPR_CCR2, BIT(CTRL_CCR2_PMUD_i));
    SPR_CLR(SPR_PMUCC0, PMUCC0_FAC);
    MSR_CLR(ITRPT_XSR_PMM_i);
    pmu_freeze_all_counters(YES);
    for(i = 0; i < (hw->cnt); i++)
        PMU_WRITE(hw->ctl_list[i],
                PMULCx( CMODE_INDEPENDENT,  /* CMODE                    */
                        YES,                /* Freeze counter           */
                        NO,                 /* Freeze in system mode    */
                        NO,                 /* Freeze in user mode      */
                        NO,                 /* Freeze if PMU_C476PMM=1  */
                        NO,                 /* Freeze if PMU_C476PMM=0  */
                        YES,                /* Condition enabled        */
                        CHECK_EVENT));
}






static
uint32_t pmu_check__events(struct pmu_hwconf_t *hw, int ctri)
{
    uint32_t     status = TEST_OK,
                 result = TEST_OK,
                 ctlbak = 0,
                 ctrval = 0,
                 ctrexp = 0,
                 evtidx = 0;

    L2C_PMU_EVENT evnum;

    ctlbak = PMU_READ(hw->ctl_list[ctri]);
    pmu_reset_all_counters();

    for(evtidx = 0; evlist[evtidx].event != -1; evtidx++)
    {
        evnum = evlist[evtidx].event;
        rumboot_printf(" --- Generate PMU event '%s' (%s%d)...\n",
                evlist[evtidx].descr,
                &"-"[!(evnum & BIT(31))], /* Select "-" or "" */
                (evnum & BIT(31)) ? -evnum : evnum);
        PMU_WRITE(hw->ctl_list[ctri], PMULCx_EVENT(NO, evnum));
        ctrexp = PMU_READ(hw->ctr_list[ctri]);
        pmu_freeze_all_counters(NO);
        pmu_gen_special_event(evnum, ctri);
        pmu_freeze_all_counters(YES);
        result = !CHECK_RANGE(ctrexp,
                evlist[evtidx].minof,
                evlist[evtidx].maxof,
                ctrval = PMU_READ(hw->ctr_list[ctri]));
        status |= result;
        rumboot_printf(
                "  --- Counter #%d have value 0x%X"
                " (expected range is 0x%X:0x%X)"
                " -> %s.\n",
                ctri, ctrval,
                ctrexp + evlist[evtidx].minof,
                ctrexp + evlist[evtidx].maxof,
                (evlist[evtidx].minof) ?
                        (!result ? "ok" : "fail") : "ok/skip");
        TEST_ASSERT(!result, "PMU COUNTER VALUE MISMATCH!\n");
    }

    PMU_WRITE(hw->ctl_list[ctri], ctlbak);

    return status;
}

static
uint32_t pmu_check_counters(struct pmu_hwconf_t *hw)
{
    static
    chk_entry_t  chklst[] =
    {
      
       /* {pmu_check__cond_freeze, PMU_ALL_COUNTERS,  0, "condition freeze"}, */
        {pmu_check__events,      PMU_ALL_COUNTERS,  0, "events listening"},
        {NULL,0,0,NULL}
    };
    struct
    chk_entry_t *chkent = NULL;
    uint32_t     status = TEST_OK;
    uint32_t     result = 0;
    int          idx;

    for(chkent  = chklst; chkent->func && chkent->desc; chkent++)
    {
        rumboot_printf(" +++ > Check '%s'...\n", chkent->desc);
        for(idx = 0; idx < (hw->cnt); idx++)
        {
            if(chkent->mask & BIT(idx))
            {
                pmu_freeze_all_counters(YES);
                rumboot_printf(" ### Counter [%d]:\n", idx);
                result |= (chkent->func)(hw, idx);
                rumboot_printf(" ### Counter [%d] check %s!\n", idx,
                        !result ? "success" : "failed");
            }
        }
        rumboot_printf(" +++ < Check %s!\n",
                !result ? "success" : "failed");
        status |= result;
        TEST_ASSERT(!result, "There is something wrong!\n");
    }

    return status;
}

uint32_t main(void)
{
    uint32_t         status = TEST_OK,
                     revid  = 0;

    struct
    pmu_hwconf_t     hw     = { NO_COUNTER,
                                pmuc_dcr,
                                pmulc_dcr,
                                ARRAY_SIZE(pmuc_dcr)
                              };

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    msync();
    /* 'lwarx' and 'stwcx.' instructions needs a cache-enabled memory */
    write_tlb_entries(tlb_entries, ARRAY_SIZE(tlb_entries));
    memset((void*)CHECK_RDWR_BASE, 0, CACHE_LINE_SZ);
    rumboot_printf("Init done.\n");

    revid = PMU_READ(L2C_PMUREVID);
    rumboot_printf("L2C Perfomance Monitor Unit revision:%d.%d\n",
            (revid >> 8) & 0x0FFF, revid & 0xFF);

    rumboot_printf("Init counters...\n");
    pmu_freeze_all_counters(YES);
    pmu_init_counters(&hw);
    pmu_reset_all_counters();
    rumboot_printf("Init done.\n");

    rumboot_printf("Test started...\n");

    status |= pmu_check_counters(&hw);

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}



