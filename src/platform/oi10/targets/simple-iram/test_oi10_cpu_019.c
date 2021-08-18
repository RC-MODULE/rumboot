/*
 * test_oi10_cpu_019.c
 *
 *  Created on: Nov 1, 2018
 *      Author: a.gurov
 */

// #define USE_HARDWARE_PART

// #define RUMBOOT_ASSERT_WARN_ONLY

#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_mmu.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <arch/dma2plb6.h>
#include <devices/mpic128.h>

#include <platform/regs/regs_dcrarb.h>
#include <platform/regs/regs_plb6plb4.h>
#include <platform/regs/regs_itrace.h>
#include <platform/regs/regs_ltrace.h>

/* Config */
#define TEST_STRING_ID              "test_oi10_cpu_019"
#define IRQ_CR_BEG                  DMA2PLB6_DMA_IRQ_0
#define IRQ_CR_END                  DMA2PLB6_DMA_IRQ_1
#define IRQ_MC_BEG                  DMA2PLB6_DMA_IRQ_2
#define IRQ_MC_END                  DMA2PLB6_DMA_IRQ_3
#define IRQ_BORDER_NC               MPIC128_PRIOR_3
#define IRQ_BORDER_CR               MPIC128_PRIOR_8
#define IRQ_BORDER_MC               MPIC128_PRIOR_12

/* Constants macros */
#define TEST_OK                     0
#define TEST_ERROR                  1
#define YES                         1
#define NO                          0
#define IRQ_WAIT_TIMEOUT            100     /* us */
#define USE_INTERNAL_INTERRUPTS     YES
#define IRQ_MIN_INTERNAL            0
#define IRQ_MAX_INTERNAL            14      /* was 31 */
#define IRQ_MIN_EXTERNAL            32
#define IRQ_MAX_EXTERNAL            77      /* was 127 */
#define IRQ_LAST_INTERNAL           LTRACE_COMPLETE_0
#define IRQ_MIN                     IRQ_MIN_INTERNAL
#define IRQ_TYPE_NC                 0
#define IRQ_TYPE_CR                 1
#define IRQ_FLAGS                   (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define IRQ_ARR_SZ_CALC             (128 / (sizeof(uint32_t) * 8))
#define IRQ_ARR_SZ                  (IRQ_ARR_SZ_CALC ? IRQ_ARR_SZ_CALC : 1)
#define IRQ_LIST_END                0xFF
#define EVERYTHING                  (~0)
#define EMPTY_DCR_ADDRESS           0xFFFFFFFF
#define DCRARB_DACR_TOCNT           IBM_BIT_INDEX(32, 27)
#define DCRARB_DAESR_EV             IBM_BIT_INDEX(32, 0)
#define EVM                         ((1 << L2C_PMULCx_EVENT_n) - 1)
#define PMUC_OVF_VAL                (~0)
#define PMUCC0_FAC                  0x80000000
#define PMUGC0_PMCC                 (1 << L2C_PMUGC0_PMCC_i)
#define PMUGC0_LFAC                 (1 << L2C_PMUGC0_LFAC_i)
#define PMULCx_FC                   (1 << L2C_PMULCx_FC_i)
#define PMULCx_RESET                0x00000000
#define CMODE_INDEPENDENT           0
#define CMODE_CHAINED               1
#define DMA2PLB6_TRANSFER_UNITS     8
#define LTRACE_TC_LTE_BIT           IBM_BIT_INDEX(32,  0)
#define LTRACE_TC_LTCEN_BIT         IBM_BIT_INDEX(32, 31)

/* Unconnected interrupts (defined for interrupt list readability) */
#define UNCONNECTED_INTERRUPT_54    54
#define UNCONNECTED_INTERRUPT_55    55
#define UNCONNECTED_INTERRUPT_61    61
#define UNCONNECTED_INTERRUPT_62    62

/* Other macros */
#define ALWAYS_INLINE               __attribute__((always_inline))
#define MAY_BE_NOT_USED             __attribute__((unused))
#define _GET_UPTIME                 rumboot_platform_get_uptime
#define RESULT_TPL                  "TEST %s!\n"
#define GENINT_ENTRY_END            {NULL,NULL,0,NULL}

/* Utility and function macros */
#define CAST_ADDR(VAL)              ((void*)    (VAL))
#define CAST_SINT(VAL)              ((int)      (VAL))
#define CAST_UINT(VAL)              ((uint32_t) (VAL))
#define IRQ_IDX(IRQ_NUM)            ((IRQ_NUM) - IRQ_MIN)
#define IRQ_ARRIDX(IRQ_NUM)         (IRQ_IDX(IRQ_NUM) >> 0x05)
#define BIT(B)                      (1 << (B))
#define RESULT_STRING(ST)           (!(ST) ? "OK" : "ERROR")
#define GENINT_ENTRY(F,D,I,N)       {(F),((void*)(D)),(I),(N)}
#define PMU_WRITE(REG,VAL)          l2c_pmu_write(DCR_L2C_BASE,REG,VAL)
#define PMU_READ(REG)               l2c_pmu_read(DCR_L2C_BASE,REG)
#define PMU_SET(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) |  (VAL))
#define PMU_CLR(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) & ~(VAL))
#define PMU_TST(REG,VAL)            !!(PMU_READ(REG) & (VAL))
#define SPR_SET(REG,VAL)            spr_write((REG), spr_read(REG) |  (VAL))
#define SPR_CLR(REG,VAL)            spr_write((REG), spr_read(REG) & ~(VAL))
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
#define PMULCx_EVENT(FREEZE,EVENT)  PMULCx(CMODE_INDEPENDENT, (FREEZE),     \
                                        NO, NO, NO,                         \
                                        NO, YES, (EVENT))


/* Types, enumerations, and structures definitions */
enum tesr_bits_t
{
    TESR_P6APE   = 0,
    TESR_P6BPE   = 1,
    TESR_P4RPE   = 2,
    TESR_P6WPE   = 3,
    TESR_P4MRE   = 4,
    TESR_P4MWE   = 5,
    TESR_P6SWE   = 6,
    TESR_P4WTO   = 7,
    TESR_P4RTO   = 8,
    TESR_P6RERR  = 9,
    TESR_P6MID   = 14,
    TESR_P6MTAG  = 19,
    TESR_P4_MIRQ = 31
};

struct genint_entry_t
{
    void    (*func)(void*);
    void     *data;
    int       irqn;
    char     *name;
};

typedef enum     tesr_bits_t        tesr_bits_t;
typedef struct   genint_entry_t     genint_entry_t;
typedef struct   rumboot_irq_entry  rumboot_irq_entry;
typedef volatile uint32_t           irq_flags_t;
typedef          uint8_t            irqnum_t;

/* Global vars */
static irq_flags_t           IRQ[IRQ_ARR_SZ];
static irq_flags_t           ext_int_raised    = 0;
static irq_flags_t           irq_type_cpu      = 0;
static irq_flags_t           irq_type_mpic     = 0;
static int                   int_int_idx       = 0;
static rumboot_irq_entry    *irq_table         = NULL;
static volatile uint32_t     dma2plb6_src[DMA2PLB6_TRANSFER_UNITS] = {0};
static volatile uint32_t     dma2plb6_dst[DMA2PLB6_TRANSFER_UNITS] = {0};

static const    irqnum_t     irq_list[] =
{
    (irqnum_t) L2C0_INTERRUPTOUT,
    (irqnum_t) L2C0_MCHKOUT,
    (irqnum_t) PMU0_INTERRUPT,
    (irqnum_t) DMA2PLB6_DMA_IRQ_0,
    (irqnum_t) DMA2PLB6_DMA_IRQ_1,
    (irqnum_t) DMA2PLB6_DMA_IRQ_2,
    (irqnum_t) DMA2PLB6_DMA_IRQ_3,
#ifdef USE_HARDWARE_PART
    (irqnum_t) DMA2PLB6_SLV_ERR_INT,
    (irqnum_t) O_SYSTEM_HUNG,
#endif
    (irqnum_t) PLB6PLB40_O_0_BR6TO4_INTR,
    (irqnum_t) PLB6PLB41_O_BR6TO4_INTR,
#ifdef USE_HARDWARE_PART
    (irqnum_t) PLB4XAHB1_INTR,
#endif
    (irqnum_t) ARB_SYSDCRERR,
    (irqnum_t) ITRACE_COMPLETE,
    (irqnum_t) LTRACE_COMPLETE_0,

#ifdef USE_HARDWARE_PART
    (irqnum_t) GPIO0_INT,
    (irqnum_t) GPIO1_INT,
    (irqnum_t) GSPI0_INT,
    (irqnum_t) GSPI1_INT,
    (irqnum_t) UART0_INT,
    (irqnum_t) UART1_INT,
    (irqnum_t) MKIO0_INT,
    (irqnum_t) MKIO1_INT,
    (irqnum_t) DIT_INT0,
    (irqnum_t) DIT_INT1,
    (irqnum_t) WDT_INT,
    (irqnum_t) CRG_INT,
    (irqnum_t) SW0_HSCB_INT,
    (irqnum_t) SW0_AXI_INT,
    (irqnum_t) SW1_HSCB_INT,
    (irqnum_t) SW1_AXI_INT,
    (irqnum_t) SW2_HSCB_INT,
    (irqnum_t) SW2_AXI_INT,
    (irqnum_t) SW3_HSCB_INT,
    (irqnum_t) SW3_AXI_INT,
    (irqnum_t) ETH0_INT,
    (irqnum_t) ETH1_INT,
    (irqnum_t) UNCONNECTED_INTERRUPT_54,
    (irqnum_t) UNCONNECTED_INTERRUPT_55,
    (irqnum_t) EMI_CNTR_INT_0,
    (irqnum_t) EMI_CNTR_INT_1,
    (irqnum_t) EMI_CNTR_INT_2,
    (irqnum_t) EMI_CNTR_INT_3,
    (irqnum_t) PLB6_INT,
    (irqnum_t) UNCONNECTED_INTERRUPT_61,
    (irqnum_t) UNCONNECTED_INTERRUPT_62,
    (irqnum_t) MCLFIR_REC_INT,
    (irqnum_t) MCLFIR_UNREC_INT,
    (irqnum_t) MCLFIR_INFST_INT,
    (irqnum_t) EXT_INT0,
    (irqnum_t) EXT_INT1,
    (irqnum_t) EXT_INT2,
    (irqnum_t) EXT_INT3,
    (irqnum_t) EXT_INT4,
    (irqnum_t) EXT_INT5,
    (irqnum_t) EXT_INT6,
    (irqnum_t) EXT_INT7,
    (irqnum_t) WU_INT,
    (irqnum_t) AXI_MSIF_INT,
    (irqnum_t) SCRB_INT_0,
    (irqnum_t) SCRB_INT_1
#endif
};

//--------------------------------------------------------

ALWAYS_INLINE
inline static
void pmu_freeze_all_counters(uint32_t freeze)
{
    if(freeze)  PMU_SET(L2C_PMUGC0, PMUGC0_LFAC);
    else        PMU_CLR(L2C_PMUGC0, PMUGC0_LFAC);
}

ALWAYS_INLINE
static inline
void pmu_reset_all_counters(void)
{
    PMU_SET(L2C_PMUGC0, PMUGC0_PMCC);
}

inline static
uint32_t print_result(uint32_t test_status)
{
    rumboot_printf(RESULT_TPL, RESULT_STRING(test_status));
    return test_status;
}

inline static
void set_interrupt(irq_flags_t *irqs, int irq_num)
{
    irqs[IRQ_ARRIDX(irq_num)]
         |= BIT(IRQ_IDX(irq_num) & 0x1F);
}

inline static
uint32_t get_interrupt(irq_flags_t *irqs, int irq_num)
{
    return !!(irqs[IRQ_ARRIDX(irq_num)]
           & BIT(IRQ_IDX(irq_num) & 0x1F));
}

inline static
void clr_interrupt(irq_flags_t *irqs, int irq_num)
{
    irqs[IRQ_ARRIDX(irq_num)] &= ~BIT(IRQ_IDX(irq_num) & 0x1F);
}

static void clear_all_interrupts(irq_flags_t *irqs)
{
    int i;
    for(i = 0; i < IRQ_ARR_SZ; i++)
        irqs[i] = 0;
}

static void ex_handler(int exid, const char *exname)
{
    if(exid == RUMBOOT_IRQ_MACHINE_CHECK)
    {
        rumboot_printf(
                "\n >>> EXCEPTION #%d (%s) at 0x%X with MCSR=0x%X! <<<\n",
                exid, exname, spr_read(SPR_MCSRR0), spr_read(SPR_MCSR_RW));
        spr_write(SPR_MCSR_C, ~0);
    } else
    {
        rumboot_printf(
                "\n >>> EXCEPTION #%d (%s) at 0x%X <<<\n",
                exid, exname, spr_read(SPR_SRR0));
    }
    l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT1, ~0);
}

uint32_t wait_irq(uint32_t usecs, int irq)
{
    volatile
    uint32_t    tmp;

    tmp = _GET_UPTIME();
    rumboot_printf("Wait for interrupt #%d...\n", irq);
    while(((_GET_UPTIME() - tmp) < (usecs)) && ! get_interrupt(IRQ, irq))
        ;
    rumboot_printf((_GET_UPTIME() - tmp) < (usecs)
            ?"HAVE WAITED IRQ!\n":"IRQ WAIT TIMEOUT!\n");

    return usecs - (_GET_UPTIME() - tmp);
}

/* Call with negated IRQ number, to process an interruption fail */
void irq_handler( int irq_num, void *arg )
{
    volatile
    uint32_t  temp = 0, srr1, csrr1;
    uint32_t  fail = (irq_num < 0) ? YES : NO;

    if(fail)
    {
        irq_num = -irq_num;
        rumboot_printf("Process fail on IRQ #%d...\n",
                irq_num);
    }
    else
    {
        srr1  = spr_read(SPR_SRR1);
        csrr1 = spr_read(SPR_CSRR1);
        rumboot_printf("IRQ #%d received"
                " (SRR0=0x%X, SRR1=0x%X, CSRR0=0x%X, CSRR1=0x%X).\n",
                irq_num,
                spr_read(SPR_SRR0),  srr1,
                spr_read(SPR_CSRR0), csrr1);
        irq_type_cpu  = (!!csrr1 << IRQ_TYPE_CR) | (!!srr1 << IRQ_TYPE_NC);
        irq_type_mpic = rumboot_irq_current_type();
        set_interrupt(IRQ, irq_num);
    }

#if IRQ_MIN_INTERNAL > 0
    if((irq_num <= IRQ_MAX_INTERNAL) && (irq_num >= IRQ_MIN_INTERNAL))
#else
    if (irq_num <= IRQ_MAX_INTERNAL)
#endif
    {   /* Process internal interrupt */
        /* rumboot_printf("Process internal interrupt.\n"); */
        switch(irq_num)
        {
        case L2C0_INTERRUPTOUT:         /*  0 */
        case L2C0_MCHKOUT:              /*  1 */
            temp = l2c_l2_read(DCR_L2C_BASE, L2C_L2PLBFERR0);
            l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBFRC0, 0);
            l2c_global_mck_disable(DCR_L2C_BASE, EVERYTHING);
            break;
        case PMU0_INTERRUPT:            /*  2 */
            pmu_freeze_all_counters(YES);
            PMU_WRITE(L2C_PMULC0, PMULCx_RESET);
            PMU_CLR(L2C_PMUIE0, BIT(L2C_PMUIE0_IE0_i));
            if(PMU_TST(L2C_PMUIS0, BIT(L2C_PMUIS0_ISTAT0_i)))
                PMU_WRITE(L2C_PMUIS0, BIT(L2C_PMUIS0_ISTAT0_i));
            pmu_reset_all_counters();
            break;
        case DMA2PLB6_DMA_IRQ_0:        /*  3 */
            dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel0);
            break;
        case DMA2PLB6_DMA_IRQ_1:        /*  4 */
            dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel1);
            break;
        case DMA2PLB6_DMA_IRQ_2:        /*  5 */
            dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel2);
            break;
        case DMA2PLB6_DMA_IRQ_3:        /*  6 */
            dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel3);
            break;
        case DMA2PLB6_SLV_ERR_INT:      /*  7 */
        case O_SYSTEM_HUNG:             /*  8 */
#ifdef USE_HARDWARE_PART
            test_event(EVENT_CLR_INT_INT);
#endif
            break;
        case PLB6PLB40_O_0_BR6TO4_INTR: /*  9 */
            dcr_write(DCR_PLB6PLB4_0_BASE + TESR, 0x00000000);
            break;
        case PLB6PLB41_O_BR6TO4_INTR:   /* 10 */
            dcr_write(DCR_PLB6PLB4_1_BASE + TESR, 0x00000000);
            break;
        case PLB4XAHB1_INTR:            /* 11 */
#ifdef USE_HARDWARE_PART
            test_event(EVENT_CLR_INT_INT);
#endif
            break;
        case ARB_SYSDCRERR:             /* 12 */
            temp = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
            if(temp & BIT(DCRARB_DAESR_EV))
                dcr_write(DCR_ARB_BASE + DCRARB_DAESR, temp);
            break;
        case ITRACE_COMPLETE:           /* 13 */
            dcr_write(DCR_ITRACE_BASE + ITC0_TS,
                    dcr_read(DCR_ITRACE_BASE + ITC0_TS));
            break;
        case LTRACE_COMPLETE_0:         /* 14 */
            dcr_write(DCR_LTRACE_BASE + LTR_TS,
                    dcr_read(DCR_LTRACE_BASE + LTR_TS));
            break;
        default:
#ifdef USE_HARDWARE_PART
            test_event(EVENT_CLR_INT_INT);
#endif
            break;
        }
    }
    else
    {   /* Process external interrupt */
        /* rumboot_printf("Process external interrupt.\n"); */
        /* Clear external interrupt signal
           and increment interrupt number   */
        test_event(EVENT_CLR_EXT_INT);
        ext_int_raised = fail ? NO : YES;
    }
}

/* -------------------------------------- */
void genint__l2c(void *data)
{
    uint32_t temp;

    l2c_enable_interrupt(CAST_UINT(data),
            L2C_L2PLBINTEN0, L2PLBINTEN0_IntvnDataPE0);
    l2c_global_enable_interrupt(CAST_UINT(data), L2INTEN_PLBINT0);
    temp = l2c_l2_read(CAST_UINT(data), L2C_L2PLBFRC0);
    l2c_l2_write(CAST_UINT(data), L2C_L2PLBFRC0,
            temp | L2PLBFRC0_IntvnDataPE0);
}

void genint__l2c_mchk(void *data)
{
    uint32_t temp;

    l2c_enable_interrupt(CAST_UINT(data),
            L2C_L2PLBMCKEN0, L2PLBMCKEN0_IntvnDataPE0);
    l2c_global_mck_enable(CAST_UINT(data), L2MCKEN_PLBMCK0);
    temp = l2c_l2_read(CAST_UINT(data), L2C_L2PLBFRC0);
    l2c_l2_write(CAST_UINT(data), L2C_L2PLBFRC0,
            temp | L2PLBFRC0_IntvnDataPE0);
}

void genint__pmu(void *data)
{
    pmu_freeze_all_counters(YES);
    pmu_reset_all_counters();
    PMU_SET(L2C_PMUIE0, BIT(L2C_PMUIE0_IE0_i));
    PMU_WRITE(L2C_PMULC0, PMULCx_EVENT(NO, L2EV_PlbMasterCmd));
    PMU_WRITE(L2C_PMUC0, PMUC_OVF_VAL);
    pmu_freeze_all_counters(NO);
}

void genint__dma2plb6(void *data)
{
    static
    struct dma2plb6_setup_info dma_info =
    {
        .base_addr              = DCR_DMAPLB6_BASE,
        .priority               = priority_medium_low,
        .striding_info.striding = striding_none,
        .tc_int_enable          = YES,
        .err_int_enable         = NO,
        .int_mode               = int_mode_level_wait_clr,
        .rw_transfer_size       = rw_tr_size_1q,
        .transfer_width         = tr_width_word,
        .count                  = DMA2PLB6_TRANSFER_UNITS
    };

        dma_info.source_adr     = get_physical_addr(CAST_UINT(dma2plb6_src), 0);
        dma_info.dest_adr       = get_physical_addr(CAST_UINT(dma2plb6_dst), 0);
        dma_info.channel        = (DmaChannel)data;

        dma2plb6_mcpy(&dma_info);
}

void genint__dcr_arb(void *data)
{
    uint32_t    dcra = CAST_UINT(data) + DCRARB_DACR;
    uint32_t    temp = dcr_read(dcra);
    dcr_write(dcra, temp | BIT(DCRARB_DACR_TOCNT));
    /* DCR transaction */
    dcr_read(EMPTY_DCR_ADDRESS);
    dcr_write(dcra, temp);
}

void genint__p6p4(void *data)
{
    uint32_t    dcra = CAST_UINT(data) + TESR;
    uint32_t    temp = dcr_read(dcra);
    /* Write TESR register, plb6 address parity error bit = 1. */
    dcr_write(dcra, temp | BIT(IBM_BIT_INDEX(32, TESR_P6WPE)));
}

void genint__itrace(void *data)
{
    uint32_t dcra = CAST_UINT(data);

    dcr_write(dcra + ITC0_TC,   BIT(ITC0_TC_ITE_i)          |
                                BIT(ITC0_TC_ITIM_i));
    dcr_write(dcra + ITC0_TC,   BIT(ITC0_TC_ITIM_i));

}

void genint__ltrace(void *data)
{
    uint32_t dcra = CAST_UINT(data);

    dcr_write(dcra + LTR_TC,    BIT(LTRACE_TC_LTE_BIT)      |
                                BIT(LTRACE_TC_LTCEN_BIT));
    dcr_write(dcra + LTR_TC,    BIT(LTRACE_TC_LTCEN_BIT));
}

#ifdef USE_HARDWARE_PART
void genint__hw_event(void *data)
{
    if(int_int_idx > CAST_SINT(data))
    {
        int_int_idx = 0;
        test_event(EVENT_RST_INT_INT);
    }
    if(int_int_idx != CAST_SINT(data))
        for(;int_int_idx < CAST_SINT(data); int_int_idx++)
            test_event(EVENT_INC_INT_INT);
    test_event(EVENT_SET_INT_INT);
}
#endif

struct genint_entry_t intgen_list[] =
{
    GENINT_ENTRY(genint__l2c,      DCR_L2C_BASE,            L2C0_INTERRUPTOUT,
            "signal <L2C0_INTERRUPTOUT>"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__l2c_mchk, DCR_L2C_BASE,            L2C0_MCHKOUT,
            "signal <L2C0_MCHKOUT>"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__pmu,      DCR_L2C_BASE,            PMU0_INTERRUPT,
            "L2C PMU #0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel0,                DMA2PLB6_DMA_IRQ_0,
            "DMA2PLB6 Channel #0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel1,                DMA2PLB6_DMA_IRQ_1,
            "DMA2PLB6 Channel #1"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel2,                DMA2PLB6_DMA_IRQ_2,
            "DMA2PLB6 Channel #2"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel3,                DMA2PLB6_DMA_IRQ_3,
            "DMA2PLB6 Channel #3"),
/*  -----------------------------------------------------------------------  */
#ifdef USE_HARDWARE_PART
    GENINT_ENTRY(genint__hw_event, DMA2PLB6_SLV_ERR_INT,    DMA2PLB6_SLV_ERR_INT,
            "signal <DMA2PLB6_SLV_ERR_INT>"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__hw_event, O_SYSTEM_HUNG,           O_SYSTEM_HUNG,
            "SYSTEM HUNG"),
#endif
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__p6p4,     DCR_PLB6PLB4_0_BASE,     PLB6PLB40_O_0_BR6TO4_INTR,
            "PLB6PLB4 Bridge #0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__p6p4,     DCR_PLB6PLB4_1_BASE,     PLB6PLB41_O_BR6TO4_INTR,
            "PLB6PLB4 Bridge #1"),
/*  -----------------------------------------------------------------------  */
#ifdef USE_HARDWARE_PART
    GENINT_ENTRY(genint__hw_event, PLB4XAHB1_INTR,          PLB4XAHB1_INTR,
            "signal <PLB4XAHB_INTR>"),
#endif
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dcr_arb,  DCR_ARB_BASE,            ARB_SYSDCRERR,
            "DCR Arbiter"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__itrace,   DCR_ITRACE_BASE,         ITRACE_COMPLETE,
            "ITRACE COMPLETE"),
    GENINT_ENTRY(genint__ltrace,   DCR_LTRACE_BASE,         LTRACE_COMPLETE_0,
            "LTRACE COMPLETE 0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY_END
};

uint32_t check_internal_interrupts(void)
{
    static
    char         *title  = "Check internal interrupts";
    uint32_t      status = TEST_OK,
                  result = 0;
    int           spri   = 0,   /* SPR index */
                  idx    = 0,
                  irq    = 0;
    static const  /* SPR list */
    uint32_t      sprl[] = {SPR_SRR0, SPR_CSRR0, SPR_SRR1, SPR_CSRR1};

    rumboot_printf("%s...\n", title);
    int_int_idx = 0;
    test_event(EVENT_RST_INT_INT);
    for(idx = 0; intgen_list[idx].func; idx++)
    {
        irq = intgen_list[idx].irqn;
        rumboot_printf("[%d]: Generating '%s' interrupt (IRQ #%d)...\n",
                idx, intgen_list[idx].name, irq);
        /* Clear xSRRn */
        for(spri = 0; spri < ARRAY_SIZE(sprl); spri++)
            spr_write(sprl[spri],  0x00000000);
        irq_type_cpu  = 0;
        irq_type_mpic = 0;
        intgen_list[idx].func(intgen_list[idx].data);
        wait_irq(IRQ_WAIT_TIMEOUT, irq);

        /* Check interrupt receiving */
        status |= !(result = get_interrupt(IRQ, irq));
        if(!result)
        {
            rumboot_printf("While waiting internal interrupt #%d...\n", irq);
            irq_handler(-irq, NULL);
        }
        TEST_ASSERT(result, "INTERNAL INTERRUPT NOT RECEIVED!\n");

        /* Check interrupt input line of CPU */
        if((irq >= IRQ_MC_BEG) && (irq <= IRQ_MC_END))
            result = !!(irq_type_cpu & BIT(IRQ_TYPE_CR));
        else if((irq >= IRQ_CR_BEG) && (irq <= IRQ_CR_END))
                result = !!(irq_type_cpu & BIT(IRQ_TYPE_CR));
        else result = !!(irq_type_cpu & BIT(IRQ_TYPE_NC));
        status |= !result;
        TEST_ASSERT(result, "CPU INTERRUPT INPUT LINE MISMATCH!\n");

        /* Check interrupt type in MPIC */
        if((irq >= IRQ_MC_BEG) && (irq <= IRQ_MC_END))
            result = (irq_type_mpic == RUMBOOT_IRQ_TYPE_MACHINECHECK);
        else if((irq >= IRQ_CR_BEG) && (irq <= IRQ_CR_END))
                result = (irq_type_mpic == RUMBOOT_IRQ_TYPE_CRITICAL);
        else result = (irq_type_mpic == RUMBOOT_IRQ_TYPE_NORMAL);
        status |= !result;
        TEST_ASSERT(result, "MPIC INTERRUPT TYPE MISMATCH!\n");
    }
    rumboot_printf("%s %s!\n", title, !status ? "success" : "failed");
    return status;
}

#ifdef USE_HARDWARE_PART
uint32_t check_external_interrupts(void)
{
    static
    char            *title  = "Check external interrupts";
    uint32_t         status = TEST_OK;
    uint32_t         result = 0;
    int              irq;

    rumboot_printf("%s...\n", title);
    for(irq = IRQ_MIN_EXTERNAL; irq <= IRQ_MAX_EXTERNAL; irq++)
    {
        ext_int_raised = NO;
        /* Set external interrupt signal */
        test_event(EVENT_SET_EXT_INT);
        wait_irq(IRQ_WAIT_TIMEOUT, irq);
        if(!ext_int_raised)
            test_event(EVENT_CLR_EXT_INT);
        ext_int_raised = NO;
        status |= !(result = get_interrupt(IRQ, irq));
        clr_interrupt(IRQ, irq);
        if(!result)
        {
            rumboot_printf("While waiting external interrupt #%d...\n", irq);
        }
        TEST_ASSERT(result, "EXTERNAL INTERRUPT NOT RECEIVED!\n");
    }
    rumboot_printf("%s %s!\n", title, !status ? "success" : "failed");

    return status;
}
#endif

void init_interrupts(void)
{
    uint32_t    irq;
    rumboot_printf("Start IRQ initialization...\n");

    /* Turn on bits 'CE', and 'EE' in MSR (enable interrupts) */
    msr_write(msr_read()    | BIT(ITRPT_XSR_CE_i)
                            | BIT(ITRPT_XSR_EE_i));

    clear_all_interrupts(IRQ);
    rumboot_irq_cli();
    ext_int_raised = NO;
    mpic128_set_interrupt_borders(DCR_MPIC128_BASE,
            IRQ_BORDER_MC, IRQ_BORDER_CR);
    irq_table = rumboot_irq_create(NULL);
    for(irq = 0; irq < ARRAY_SIZE(irq_list); irq++)
        rumboot_irq_set_handler(irq_table, (int)irq_list[irq],
            IRQ_FLAGS, irq_handler, NULL);
#ifdef USE_HARDWARE_PART
    for(irq = 0; irq_list[irq] <= IRQ_LAST_INTERNAL; irq++)
#else
    for(irq = 0; irq < ARRAY_SIZE(irq_list); irq++)
#endif
    {
        int irqn = irq_list[irq];
        if((irqn >= IRQ_MC_BEG) && (irqn <= IRQ_MC_END))
            rumboot_irq_priority_adjust(irq_table, irqn, IRQ_BORDER_MC);
        else
            if((irqn >= IRQ_CR_BEG) && (irqn <= IRQ_CR_END))
                rumboot_irq_priority_adjust(irq_table, irqn, IRQ_BORDER_CR);
            else
                rumboot_irq_priority_adjust(irq_table, irqn, IRQ_BORDER_NC);
    }

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    for(irq = 0; irq < ARRAY_SIZE(irq_list); irq++)
        rumboot_irq_enable((int)irq_list[irq]);

    rumboot_irq_sei();
    rumboot_irq_set_exception_handler(ex_handler);

    rumboot_printf("IRQ have been initialized.\n");
}

uint32_t main(void)
{
#ifdef USE_HARDWARE_PART
    int use_hwp = YES;
    test_event_send_test_id(TEST_STRING_ID);
#else
    int use_hwp = NO;
#endif

    rumboot_printf("Testing %s hardware parts...\n",
            use_hwp ? "with" : "without");

    init_interrupts();

    SPR_SET(SPR_CCR0, BIT(CTRL_CCR0_ITE_i));
    SPR_CLR(SPR_CCR0, BIT(CTRL_CCR0_DTB_i));
    SPR_CLR(SPR_CCR2, BIT(CTRL_CCR2_PMUD_i));
    SPR_CLR(SPR_PMUCC0, PMUCC0_FAC);

    return print_result(
            check_internal_interrupts()
#ifdef USE_HARDWARE_PART
            ||
            check_external_interrupts()
#endif
            );
}

