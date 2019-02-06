/*
 * test_oi10_cpu_019.c
 *
 *  Created on: Nov 1, 2018
 *      Author: a.gurov
 */

#define RUMBOOT_ASSERT_WARN_ONLY

#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/interrupts.h>

#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/devices/dma2plb6.h>

#include <platform/regs/regs_dcrarb.h>
#include <platform/regs/regs_plb6plb4.h>

#define TEST_STRING_ID              "test_oi10_cpu_019"

/* Constants macros */
#define TEST_OK                     0
#define TEST_ERROR                  1
#define YES                         1
#define NO                          0
#define IRQ_WAIT_TIMEOUT            100         /* us */
#define USE_INTERNAL_INTERRUPTS     YES
#define IRQ_MIN_INTERNAL            0
#define IRQ_MAX_INTERNAL            31
#define IRQ_MIN_EXTERNAL            32
#define IRQ_MAX_EXTERNAL            95
#define IRQ_MIN                     (USE_INTERNAL_INTERRUPTS ? 0 : 32)
#define IRQ_MAX                     ((IRQ_MAX_EXTERNAL > IRQ_MAX_INTERNAL) ? \
                                      IRQ_MAX_EXTERNAL : IRQ_MAX_INTERNAL)
#define IRQ_CNT                     (IRQ_MAX - IRQ_MIN + 1)
#define IRQ_FLAGS                   (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define IRQ_ARR_SZ_CALC             (IRQ_CNT / (sizeof(uint32_t) * 8))
#define IRQ_ARR_SZ                  (IRQ_ARR_SZ_CALC ? IRQ_ARR_SZ_CALC : 1)
#define IRQ_LIST_END                0xFF
#define EVERYTHING                  (~0)
#define EMPTY_DCR_ADDRESS           0xFFFFFFFF
#define DCRARB_DACR_TOCNT           IBM_BIT_INDEX(32, 27)
#define DCRARB_DAESR_EV             IBM_BIT_INDEX(32, 0)
#define EVM                         ((1 << PMULCx_EVENT_n) - 1)
#define PMUC_OVF_VAL                (~0)
#define PMUCC0_FAC                  0x80000000
#define PMUGC0_PMCC                 (1 << PMUGC0_PMCC_i)
#define PMUGC0_LFAC                 (1 << PMUGC0_LFAC_i)
#define PMULCx_FC                   (1 << PMULCx_FC_i)
#define PMULCx_RESET                0x00000000
#define CMODE_INDEPENDENT           0
#define CMODE_CHAINED               1
#define DMA2PLB6_TRANSFER_UNITS     8


/* Other macros */
#define ALWAYS_INLINE               __attribute__((always_inline))
#define MAY_BE_NOT_USED             __attribute__((unused))
#define _GET_UPTIME                 rumboot_platform_get_uptime
#define RESULT_TPL                  "TEST %s!\n"
#define EVENT_CLR_EXT_INT           EVENT_CLEAR_EXT_INT
#define GENINT_ENTRY_END            {NULL,NULL,0,NULL}

/* Utility and function macros */
#define CAST_ADDR(VAL)              ((void*)    (VAL))
#define CAST_UINT(VAL)              ((uint32_t) (VAL))
#define IRQ_IDX(IRQ_NUM)            ((IRQ_NUM) - IRQ_MIN)
#define IRQ_ARRIDX(IRQ_NUM)           (IRQ_IDX(IRQ_NUM) >> 0x05)
#define BIT(B)                      (1 << (B))
#define RESULT_STRING(ST)           (!(ST) ? "OK" : "ERROR")
#define GENINT_ENTRY(F,D,I,N)       {(F),((void*)(D)),(I),(N)}
#define PMU_WRITE(REG,VAL)          l2c_pmu_write(DCR_L2C_BASE,REG,VAL)
#define PMU_READ(REG)               l2c_pmu_read(DCR_L2C_BASE,REG)
#define PMU_SET(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) |  (VAL))
#define PMU_CLR(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) & ~(VAL))
#define PMU_INV(REG,VAL)            PMU_WRITE((REG), PMU_READ(REG) ^  (VAL))
#define PMU_TST(REG,VAL)            !!(PMU_READ(REG) & (VAL))
#define SPR_SET(REG,VAL)            spr_write((REG), spr_read(REG) |  (VAL))
#define SPR_CLR(REG,VAL)            spr_write((REG), spr_read(REG) & ~(VAL))
#define SPR_INV(REG,VAL)            spr_write((REG), spr_read(REG) ^  (VAL))
#define PMULCx(CMODE,FC,    \
        FCS,FCU,FCM1,FCM0,  \
        CE,EVENT)                   ((((CMODE) & 1  ) << PMULCx_CMODE_i)  | \
                                     (((FC)    & 1  ) << PMULCx_FC_i   )  | \
                                     (((FCS)   & 1  ) << PMULCx_FCS_i  )  | \
                                     (((FCU)   & 1  ) << PMULCx_FCU_i  )  | \
                                     (((FCM1)  & 1  ) << PMULCx_FCM1_i )  | \
                                     (((FCM0)  & 1  ) << PMULCx_FCM0_i )  | \
                                     (((CE)    & 1  ) << PMULCx_CE_i   )  | \
                                     (((EVENT) & EVM) << PMULCx_EVENT_i))
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

/* Global vars */
static irq_flags_t        IRQ[IRQ_ARR_SZ];
static irq_flags_t        ext_int_raised    = 0;
static rumboot_irq_entry *irq_table         = NULL;
static volatile void     *g_ptr             = NULL;
static volatile uint32_t  dma2plb6_src[DMA2PLB6_TRANSFER_UNITS] = {0};
static volatile uint32_t  dma2plb6_dst[DMA2PLB6_TRANSFER_UNITS] = {0};


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
    rumboot_printf(
            "\n >>> EXCEPTION #%d (%s) at 0x%X with MCSR=0x%X! <<<\n",
            exid, exname, spr_read(SPR_MCSRR0), spr_read(SPR_MCSR_RW));
    spr_write(SPR_MCSR_C, ~0);
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
    uint32_t  temp = 0;
    uint32_t  fail = (irq_num < 0) ? YES : NO;

    if(fail)
    {
        irq_num = -irq_num;
        rumboot_printf("Process fail on IRQ #%d...\n",
                irq_num);
    }
    else
    {
        rumboot_printf("IRQ #%d received (Return address: 0x%X).\n",
                irq_num, spr_read(SPR_SRR0));
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
            PMU_CLR(L2C_PMUIE0, BIT(PMUIE0_IE0_i));
            if(PMU_TST(L2C_PMUIS0, BIT(PMUIS0_ISTAT0_i)))
                PMU_WRITE(L2C_PMUIS0, BIT(PMUIS0_ISTAT0_i));
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
            test_event(EVENT_CLEAR_SLVERR_INT_0);
            break;
        case O_SYSTEM_HUNG:             /*  8 */
            test_event(EVENT_CLEAR_SYSTEM_HANG);
            break;
        case PLB6PLB40_O_0_BR6TO4_INTR: /*  9 */
            dcr_write(DCR_PLB6PLB4_0_BASE + TESR, 0x00000000);
            break;
        case PLB6PLB41_O_BR6TO4_INTR:   /* 10 */
            dcr_write(DCR_PLB6PLB4_1_BASE + TESR, 0x00000000);
            break;
        case PLB4XAHB1_INTR:            /* 11 */
            test_event(EVENT_CLEAR_P4XAHB_0_INT);
            break;
        case ARB_SYSDCRERR:             /* 12 */
            temp = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
            if(temp & BIT(DCRARB_DAESR_EV))
                dcr_write(DCR_ARB_BASE + DCRARB_DAESR, temp);
            break;
        case ITRACE_COMPLETE:           /* 13 */
            break;
        case LTRACE_COMPLETE_0:         /* 14 */
            break;
        default:
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
    l2c_global_mck_enable(CAST_UINT(data), L2MCKEN_PLBINT0);
    temp = l2c_l2_read(CAST_UINT(data), L2C_L2PLBFRC0);
    l2c_l2_write(CAST_UINT(data), L2C_L2PLBFRC0,
            temp | L2PLBFRC0_IntvnDataPE0);
}

MAY_BE_NOT_USED
void genint__pmu(void *data)
{
    pmu_freeze_all_counters(YES);
    pmu_reset_all_counters();
    PMU_SET(L2C_PMUIE0, BIT(PMUIE0_IE0_i));
    PMU_WRITE(L2C_PMULC0, PMULCx_EVENT(NO, L2EV_PlbMasterCmd));
    PMU_WRITE(L2C_PMUC0, PMUC_OVF_VAL);
    pmu_freeze_all_counters(NO);
}

void genint__dma2plb6(void *data)
{
    static
    dma2plb6_setup_info dma_info =
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

void genint__hw_event(void *data)
{
    test_event(CAST_UINT(data));
}

void genint__itrace(void *data)
{
    /* Temporary stub */
    irq_handler(ITRACE_COMPLETE, CAST_ADDR(&g_ptr));
}

void genint__ltrace(void *data)
{
    /* Temporary stub */
    irq_handler(LTRACE_COMPLETE_0, CAST_ADDR(&g_ptr));
}


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
            "DMA2PLB6 Channel #0 IRQ"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel1,                DMA2PLB6_DMA_IRQ_1,
            "DMA2PLB6 Channel #1 IRQ"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel2,                DMA2PLB6_DMA_IRQ_2,
            "DMA2PLB6 Channel #2 IRQ"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dma2plb6, channel3,                DMA2PLB6_DMA_IRQ_3,
            "DMA2PLB6 Channel #3 IRQ"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__hw_event, EVENT_SET_SLVERR_INT_0,  DMA2PLB6_SLV_ERR_INT,
            "signal <DMA2PLB6_SLV_ERR_INT>"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__hw_event, EVENT_SET_SYSTEM_HANG,   O_SYSTEM_HUNG,
            "SYSTEM HUNG"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__p6p4,     DCR_PLB6PLB4_0_BASE,     PLB6PLB40_O_0_BR6TO4_INTR,
            "PLB6PLB4 Bridge #0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__p6p4,     DCR_PLB6PLB4_1_BASE,     PLB6PLB41_O_BR6TO4_INTR,
            "PLB6PLB4 Bridge #1"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__hw_event, EVENT_SET_P4XAHB_0_INT,  PLB4XAHB1_INTR,
            "signal <PLB4XAHB_INTR>"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__dcr_arb,  DCR_ARB_BASE,            ARB_SYSDCRERR,
            "DCR Arbiter"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY(genint__itrace,   0,                       ITRACE_COMPLETE,
            "ITRACE COMPLETE"),
    GENINT_ENTRY(genint__ltrace,   0,                       LTRACE_COMPLETE_0,
            "LTRACE COMPLETE 0"),
/*  -----------------------------------------------------------------------  */
    GENINT_ENTRY_END
};

uint32_t check_internal_interrupts(void)
{
    static
    char        *title  = "Check internal interrupts";
    uint32_t     status = TEST_OK;
    uint32_t     result = 0;
    int          idx    = 0;
    int          irq    = 0;

    rumboot_printf("%s...\n", title);
    for(idx = 0; intgen_list[idx].func; idx++)
    {
        irq = intgen_list[idx].irqn;
        rumboot_printf("[%d]: Generating '%s' interrupt (IRQ #%d)...\n",
                idx, intgen_list[idx].name, irq);
        intgen_list[idx].func(intgen_list[idx].data);
        wait_irq(IRQ_WAIT_TIMEOUT, irq);
        status |= !(result = get_interrupt(IRQ, irq));
        if(!result)
        {
            rumboot_printf("While waiting internal interrupt #%d...\n", irq);
            irq_handler(-irq, CAST_ADDR(&g_ptr));
        }
        TEST_ASSERT(result, "INTERNAL INTERRUPT NOT RECEIVED!\n");
    }
    rumboot_printf("%s %s!\n", title, !status ? "success" : "failed");
    return status;
}

uint32_t check_external_interrupts(void)
{
    static
    char            *title  = "Check external interrupts";
    uint32_t         status = TEST_OK,
                     result = 0;
    int              irq;

    rumboot_printf("%s...\n", title);
    for(irq = IRQ_MIN_EXTERNAL; irq < IRQ_MAX_EXTERNAL; irq++)
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

void init_interrupts(void)
{
    uint32_t    irq;
    rumboot_printf("Start IRQ initialization...\n");

    /* Turn on bits 'CE' and 'EE' in MSR (enable interrupts) */
    msr_write(msr_read() | BIT(ITRPT_XSR_CE_i) | BIT(ITRPT_XSR_EE_i));

    clear_all_interrupts(IRQ);
    rumboot_irq_cli();
    ext_int_raised = NO;
    irq_table = rumboot_irq_create(NULL);
    for(irq = IRQ_MIN; irq < IRQ_MAX; irq++)
        rumboot_irq_set_handler(irq_table, irq,
            IRQ_FLAGS, irq_handler, CAST_ADDR(&g_ptr));

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    for(irq = IRQ_MIN; irq < IRQ_MAX; irq++)
        rumboot_irq_enable(irq);
    rumboot_irq_sei();
    rumboot_irq_set_exception_handler(ex_handler);

    rumboot_printf("IRQ have been initialized.\n");
}

uint32_t main(void)
{
    test_event_send_test_id(TEST_STRING_ID);
    init_interrupts();

    SPR_CLR(SPR_CCR2, 1 << CTRL_CCR2_PMUD_i);
    SPR_CLR(SPR_PMUCC0, PMUCC0_FAC);

    return print_result(
            check_internal_interrupts()
            ||
            check_external_interrupts()
            );
}




