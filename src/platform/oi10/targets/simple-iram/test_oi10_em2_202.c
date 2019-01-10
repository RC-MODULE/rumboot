#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RUMBOOT_ASSERT_WARN_ONLY

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/common_macros/common_macros.h>

#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>

#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/fields/emi.h>

#include <platform/interrupts.h>

#define NOR_SELFCHECKING_DISABLE 1
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/devices/nor_1636RR4.h>


/* Integer constant macros */
#define EMI_BASE                DCR_EM2_EMI_BASE
#define TEST_OK                 0x00000000
#define TEST_ERROR              0x00000000
#define INIT_OK                 TEST_OK
#define INIT_ERROR              TEST_ERROR
#define NUMBER_OF_BANKS         6
#define EMI_TRDY_VAL            8
#define EMI_TCYC_VAL_0          17
#define EMI_TRDY_VAL_0          0x0400
#define EMI_RFC_RP_DIV_1        0b11110011110011
#define EMI_RFC_RP_DIV_2        0b01111001111001
#define EMI_RFC_RP_MASK         0x8FFF
#define EMI_RFC_TRFC_MASK       0x0007
#define EMI_TRDY_MASK           (0x03FF << EMI_SSx_T_RDY_i)
#define EMI_BITS_D0_D7          EMI_WECR_BWE0_i
#define EMI_BITS_D8_D15         EMI_WECR_BWE1_i
#define EMI_BITS_D16_D23        EMI_WECR_BWE2_i
#define EMI_BITS_D24_D31        EMI_WECR_BWE3_i
#define EMI_BITS_ED0_ED7        EMI_WECR_BWE4_i
#define IRR_RST_ALL             0x0001FFFF
#define ECC_CTRL_MASK           0x03
#define IRR_RST_SINGLE          ((1 << EMI_IMR_ME1B0_i)  |  \
                                 (1 << EMI_IMR_ME1B1_i)  |  \
                                 (1 << EMI_IMR_ME1B2_i)  |  \
                                 (1 << EMI_IMR_ME1B3_i)  |  \
                                 (1 << EMI_IMR_ME1B4_i)  |  \
                                 (1 << EMI_IMR_ME1B5_i))
#define IRR_RST_DOUBLE          ((1 << EMI_IMR_ME2B0_i)  |  \
                                 (1 << EMI_IMR_ME2B1_i)  |  \
                                 (1 << EMI_IMR_ME2B2_i)  |  \
                                 (1 << EMI_IMR_ME2B3_i)  |  \
                                 (1 << EMI_IMR_ME2B4_i)  |  \
                                 (1 << EMI_IMR_ME2B5_i))
#define IRR_RST_RDY             ((1 << EMI_IMR_MRDYR_i)  |  \
                                 (1 << EMI_IMR_MRDYW_i))
#define IRR_MASK_ALL            IRR_RST_ALL
#define IMR_UNMASK_ALL          IRR_RST_ALL
#define IMR_UNMASK_SINGLE       IRR_RST_SINGLE
#define IMR_UNMASK_DOUBLE       IRR_RST_DOUBLE
#define IRQ_CNT_SINGLE          0
#define IRQ_CNT_DOUBLE          1
#define IRQ_CNT_RDY             2
#define IRQ_CNT_OTHER           3
#define IRQ_FLAGS               (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define EMBANKS(A,B,C,D,E,F)    (!!(A) << 0) |  \
                                (!!(B) << 1) |  \
                                (!!(C) << 2) |  \
                                (!!(D) << 3) |  \
                                (!!(E) << 4) |  \
                                (!!(F) << 5)

/* Config */
/*                      ENABLED BANK -> |0|1|2|3|4|5| */
#define ENABLED_EM_BANKS        EMBANKS( 1,1,1,1,1,1 )
#define DEFAULT_OFFSET          0x00000100
#define CHECK_OFFSET            offset
#define CHECK_OFFSET8           (CHECK_OFFSET + ((2 * 4) + 3))
#define CHECK_OFFSET16          (CHECK_OFFSET + ((3 * 4) + 2))
#define CHECK_OFFSET32          CHECK_OFFSET
#define CHECK_OFFSET64          (CHECK_OFFSET + ((4 * 4) + 0))
#define CHECK_CONST8            0xE5
#define CHECK_CONST16           0xCAFE
#define CHECK_CONST32           0xF00D4BEE
#define CHECK_CONST64           0xDEADCAFE4BADBABEULL
#define CHECK_CONST             CHECK_CONST32
#define CHECK_ECC_CODE          0x10
#define MK_ERR_1                0x0001
#define MK_ERR_2                0x1100

/* Other macros */
#define CHECKLIST_TERMINATOR    {NULL,NULL}
#define CF_ARGS                 ckinfo,bank

/* Function and block definition macros */
#define ALIGN(AV)               __attribute__ ((aligned (AV)))
#define BIT(N)                  (1 << (N))
#define EMIA(ADDR)              (EMI_BASE + ADDR)
#define EMI_READ(ADDR)          dcr_read (EMIA(ADDR))
#define EMI_WRITE(ADDR,DATA)    dcr_write(EMIA(ADDR),(DATA))
#define EMI_TYPE(BANK)          emi_get_bank_cfg_cached(bank)->ssx_cfg.BTYP
#define GEN_ECC(VAL)            calc_hamming_ecc(reverse_bytes32(VAL))
#define GET_BANK_SS(BANK)       ((EMI_REG)(emi_ss[(BANK)]))
#define GET_TRDY(SSI)           (((SSI) & EMI_TRDY_MASK) >> EMI_SSx_T_RDY_i)
#define SET_TRDY(SSI,TRDY)      ((((SSI) | BIT(EMI_SSx_SRDY_i)) & ~EMI_TRDY_MASK)  | \
                                    (((EMI_TRDY_VAL_0 - (TRDY)) \
                                        << EMI_SSx_T_RDY_i) & EMI_TRDY_MASK))
#define GET_TCYC(SSI)           (EMI_TCYC_VAL_0 - \
                                    (((SSI) >> EMI_SSx_T_CYC_i) & 0x0F))
#define MK_RFC(RP,TRFC)         ((((RP)   & 0x8FFF) << EMI_RFC_RP_i  ) |             \
                                 (((TRFC) & 0x0007) << EMI_RFC_TRFC_i))
#define MASK_ECNT(BANK)         (0xFF >> emi_ecnt_b[BANK])
#define READ_ECNT(BANK)         ((uint8_t)((EMI_READ(emi_ecnt[BANK])    \
                                    >> emi_ecnt_b[BANK]) & 0xFF))
#define SINGLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 0))
#define DOUBLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 1))
#define ECC_ON(BANK)            EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) |  (1 << (BANK)));
#define ECC_OFF(BANK)           EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) & ~(1 << (BANK)));
#define IS_NOR(BANK)            (EMI_TYPE(BANK) == BTYP_NOR)
#define IS_SDRAM(BANK)          (EMI_TYPE(BANK) == BTYP_SDRAM)
#define IS_SSRAM(BANK)          (EMI_TYPE(BANK) == BTYP_SSRAM)
#define IS_SRAM(BANK)           (EMI_TYPE(BANK) == BTYP_SSRAM)
#define IS_NOT_NOR(BANK)        (EMI_TYPE(BANK) != BTYP_NOR)
#define IS_NOT_SDRAM(BANK)      (EMI_TYPE(BANK) != BTYP_SDRAM)
#define IS_NOT_SSRAM(BANK)      (EMI_TYPE(BANK) != BTYP_SSRAM)
#define IS_NOT_SRAM(BANK)       (EMI_TYPE(BANK) != BTYP_SRAM)
#define CHECK_ADDR8(BANK)       (emi_bank_bases[BANK] + CHECK_OFFSET8)
#define CHECK_ADDR16(BANK)      (emi_bank_bases[BANK] + CHECK_OFFSET16)
#define CHECK_ADDR32(BANK)      (emi_bank_bases[BANK] + CHECK_OFFSET32)
#define CHECK_ADDR64(BANK)      (emi_bank_bases[BANK] + CHECK_OFFSET64)
#define CHECK_ADDR(BANK)        CHECK_ADDR32(BANK)
#define CHECK_FUNC(NAME)        check__ ## NAME
#define INIT_FUNC(NAME)         init__ ## NAME
#define CHECK_ITEM(NAME,DESC)   {CHECK_FUNC(NAME),DESC}
#define DEFINE_CHECK(NAME)      uint32_t CHECK_FUNC(NAME)       \
                                (ckinfo_t* ckinfo, uint32_t bank)
#define CAST_ADDR(ADDR)         ((void*)(ADDR))
#define DEFINE_INIT(NAME)       uint32_t INIT_FUNC(NAME) (void)
#define BEGIN_TESTS(VAR)        ckinfo_t VAR[]={
#define END_TESTS               CHECKLIST_TERMINATOR}
#define FOREACH_TEST(ITEM,LIST) for(ITEM = LIST; ITEM->test; ITEM++)
#define FOREACH_IRQ(ITEM,LIST)  for(ITEM = LIST; ~*ITEM; ITEM++)
/* N - Banks number;  I - Bank index;
   A - Bank activity; L - Active banks list (as bits). */
#define FOREACH_BANK(I,A,N,L)   for(I = 0, A = (L) & 1; I < N;   \
                                    I++, A = (((L) >> I) & 1))
#define DO_INIT(RES,FUNC)       if(!!(RES = init__ ## FUNC ())) \
                                return test_result



struct irq_sp_t /* IRQ Sub-Process descriptor */
{
    uint8_t       bank;
    uint8_t       cmpv;
    uint8_t       newv; /* not used */
    uint8_t       flag;
};


struct ckinfo_t
{
    uint32_t   (*test)(struct ckinfo_t*, uint32_t);
    char        *desc;
};

union u8x4_t
{
    uint32_t    w;
    uint8_t     b[4];
};

typedef struct   irq_sp_t           irq_pp_t;
typedef struct   ckinfo_t           ckinfo_t;
typedef struct   rumboot_irq_entry  rumboot_irq_entry;
typedef union    u8x4_t             u8x4_t;
typedef volatile uint32_t           irq_flags_t;
typedef volatile
        union {  uint64_t u;
                 double   d;}       vuint64_t ALIGN(8);
typedef volatile uint32_t           vuint32_t;
typedef volatile uint16_t           vuint16_t;
typedef volatile uint8_t            vuint8_t;

/* Global variables */
static const uint8_t other_banks[8] = {4,2,1,2,0,5,255,255};
static const uint8_t errcnt_vals[4] = {0x01,0x04,0x08,0x10}; /* for 2.2.4 */
static irq_flags_t IRQ[8] = {0,0,0,0,0,0,0,0};
static rumboot_irq_entry   *irq_table = NULL;
static volatile uint32_t    global_irr = 0;
static volatile u8x4_t      irq_cnt;
static struct   irq_sp_t    g_sp_info  = {0,0,0,0};
/* Data tables, for register calculation, depending on bank */
static volatile const uint8_t emi_ss[8] =
    {EMI_SS0, EMI_SS1, EMI_SS2, EMI_SS3, EMI_SS4, EMI_SS5, 0,0};
static volatile const uint8_t emi_sd[8] =
    {EMI_SD0, EMI_SD1, EMI_SD2, EMI_SD3, EMI_SD4, EMI_SD5, 0,0};
static volatile const uint8_t emi_ecnt_b[8] = {0,8,16,0,8,16,0,0};
static volatile const uint8_t emi_ecnt[8] =
    {
        EMI_ECNT20, EMI_ECNT20, EMI_ECNT20,
        EMI_ECNT53, EMI_ECNT53, EMI_ECNT53, 0,0
    };
/* Memory banks addresses */
static volatile const uintptr_t emi_bank_bases[8] =
    {
        EM2_BANK0_BASE, EM2_BANK1_BASE,
        EM2_BANK2_BASE, EM2_BANK3_BASE,
        EM2_BANK4_BASE, EM2_BANK5_BASE, 0,0
    };
/* Memory banks types descriptions */
static const char *btyp_descr[8] =
    {
/*       0           1           2           3          */
        "SRAM",     "NOR",      "SSRAM",    "UNKNOWN3",
/*       4           5           6           7          */
        "PIPELINE", "UNKNOWN5", "SDRAM",    "UNKNOWN7"
    };

static void (*writer32)(uint32_t, uint32_t) = NULL;

inline static uint32_t reverse_bytes32(uint32_t data)
{
    return  ((data & 0xFF000000) >> 0x18) |
            ((data & 0x00FF0000) >> 0x08) |
            ((data & 0x0000FF00) << 0x08) |
            ((data & 0x000000FF) << 0x18) ;
}

int is_irq(void)
{
    int r, i;
    for(r = 0, i = 0; i < 8; i++) r |= IRQ[i];
    return r;
}

void clear_all_irqs(void)
{
    int i;
    global_irr = 0x00000000;
    irq_cnt.w  = 0x00000000;
    for(i=0; i<8; i++) IRQ[i] = 0;
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
}

volatile uint32_t emi_switch_bank(uint32_t bank)
{
    uint32_t offset = DEFAULT_OFFSET;
#ifdef CMAKE_BUILD_TYPE_DEBUG
    if(IS_NOT_NOR(bank))
    {
        memset(CAST_ADDR(CHECK_ADDR32(bank)),
                0, 16 * sizeof(uint32_t)); /* Anti-X */
        msync();
    }
#endif
    return ioread32(CHECK_ADDR32(bank)); /* Switch bank */
}

void emi_set_ssi(uint32_t *ssiop, uint32_t trdy_val, uint32_t bank)
{
    uint32_t    nssi; /* New SSi */
    /* ssiop - SSi Original Pointer */
    *ssiop = EMI_READ((EMI_REG)emi_ss[bank]);
    EMI_WRITE((EMI_REG)emi_ss[bank],
            (*ssiop | BIT(EMI_SSx_SRDY_i)) | SET_TRDY(*ssiop, trdy_val));
    nssi = EMI_READ(GET_BANK_SS(bank));
    rumboot_printf(
            "OLD SS%d=0x%X, OLD EMI_SS%d.T_RDY=%d,"
            " OLD EMI_SS%d.T_CYC=%dclk,\n",
            bank,   *ssiop,
            bank,   GET_TRDY(*ssiop),
            bank,   GET_TCYC(*ssiop));
    rumboot_printf(
            "NEW SS%d=0x%X, NEW EMI_SS%d.T_RDY=%d \n",
            bank,   nssi,
            bank,   GET_TRDY(nssi));
}

void emi_irq_handler( int irq, void *arg )
{
    volatile
    uint32_t tmp = 0;
    uint32_t irr = (EMI_READ(EMI_IRR) & IRR_MASK_ALL);
    struct irq_sp_t *inf = (struct irq_sp_t*)arg;
    uint8_t cnt;
    tmp ^= 0;
    rumboot_printf("IRQ #%d received at 0x%X (IRR=0x%X).\n",
            irq, spr_read(SPR_SRR0), irr);
    IRQ[irq >> 5] = BIT(irq & 0x1F);
    switch(irq)
    {
        case EMI_CNTR_INT_0:
            EMI_WRITE(EMI_IRR_RST, IRR_RST_SINGLE);
            irq_cnt.b[0]++;
            if(inf->flag & 1)
            {
                cnt = READ_ECNT(inf->bank);
                if(cnt == inf->cmpv)
                {
                    rumboot_printf(
                            "IRQ handler says>"
                            " Ooops! In bank #%d"
                            " error counter is %d"
                            " -- clear it!\n",
                            ((uint32_t)(inf->bank)),
                            ((uint32_t)(cnt)));
                    msync();
                    EMI_WRITE(emi_ecnt[inf->bank], 0);
                    tmp = EMI_READ(EMI_WECR);
                    inf->flag = 0x00;
                }
            }
            break;
        case EMI_CNTR_INT_1:
            EMI_WRITE(EMI_IRR_RST, IRR_RST_DOUBLE);
            irq_cnt.b[1]++;
            break;
        case EMI_CNTR_INT_2:
            EMI_WRITE(EMI_IRR_RST, IRR_RST_RDY);
            irq_cnt.b[2]++;
            break;
        default:
            EMI_WRITE(EMI_IRR_RST, irr);
            irq_cnt.b[3]++;
            break;
    }
    global_irr |= irr;
}

static void ex_handler(int exid, const char *exname)
{
    uintptr_t   ret_addr;
    uint32_t    syndrome;
    switch(exid)
    {
    case RUMBOOT_IRQ_MACHINE_CHECK:
        ret_addr = spr_read(SPR_MCSRR0);
        syndrome = spr_read(SPR_MCSR_RW);
        break;
    default:
        ret_addr = spr_read(SPR_SRR0);
        syndrome = spr_read(SPR_ESR);
        break;
    }
    rumboot_printf(
            "\n >>> EXCEPTION #%d (%s) at 0x%X with syndrome=0x%X! <<<\n",
            exid, exname, ret_addr, syndrome);
    spr_write(SPR_MCSR_C, ~0);
    l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT1, ~0);
}

/* Initializations */
DEFINE_INIT(interrupts)
{
    uint32_t    *irq_item;
    static
    uint32_t    irq_list[] =
    {
        EMI_CNTR_INT_0,
        EMI_CNTR_INT_1,
        EMI_CNTR_INT_2,
        EMI_CNTR_INT_3,
        PLB6_INT,
        MCLFIR_REC_INT,
        MCLFIR_UNREC_INT,
        MCLFIR_INFST_INT,
        ~0
    };
    rumboot_printf ("\tStart IRQ initialization...\n");

    clear_all_irqs();
    rumboot_irq_cli();
    irq_table = rumboot_irq_create( NULL );
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_set_handler(irq_table, *irq_item,
            IRQ_FLAGS, emi_irq_handler, CAST_ADDR(&g_sp_info));

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_enable(*irq_item);
    rumboot_irq_sei();
    /* Unmask all interrupts */
    EMI_WRITE(EMI_IMR, IMR_UNMASK_ALL);

    rumboot_irq_set_exception_handler(ex_handler);

    rumboot_printf ("\tIRQ have been initialized.\n");

    return INIT_OK;
}

DEFINE_INIT(emi_hw)
{
    uint32_t init_addr = 0x00000000;
    emi_init(DCR_EM2_EMI_BASE);
    memset(CAST_ADDR(init_addr), 0, 4096); /* Anti-X */
    msync();
    return INIT_OK;
}

DEFINE_INIT(ppc_hw)
{
    msr_write(msr_read() | BIT(ITRPT_XSR_FP_i)); // FPU enable
    return  !(msr_read() & BIT(ITRPT_XSR_FP_i));
}

DEFINE_CHECK(rfc);
DEFINE_CHECK(hstsr_ecc_on);
DEFINE_CHECK(hstsr_ecc_off);
DEFINE_CHECK(error_counter);
DEFINE_CHECK(single_interrupts);
DEFINE_CHECK(double_interrupts);

DEFINE_CHECK(hstsr_ecc_on_nor);
DEFINE_CHECK(hstsr_ecc_off_nor);


/* Checklist. Put your tests here. */
BEGIN_TESTS(tests)
    CHECK_ITEM(rfc,                 "2.2.1 - regeneration configure"    ),
    CHECK_ITEM(hstsr_ecc_on,        "2.2.2 - mode control reg (ECC ON)" ),
    CHECK_ITEM(hstsr_ecc_off,       "2.2.3 - mode control reg (ECC OFF)"),
    CHECK_ITEM(error_counter,       "2.2.4 - single error counter"      ),
    CHECK_ITEM(single_interrupts,   "2.2.5 - single interrupts"         ),
    CHECK_ITEM(double_interrupts,   "2.2.6 - double interrupts"         ),
END_TESTS;

/* Checks */
DEFINE_CHECK(rfc)   /* 2.2.1 */
{
    uint32_t  rfc_saved = 0,
              cnt       = 0,
              result    = 0,
              offset    = DEFAULT_OFFSET,
              status    = TEST_OK;
    uint32_t *rfcp      = NULL;
    vuint8_t  readed8   = 0;
    vuint16_t readed16  = 0;
    vuint32_t readed32  = 0;
    vuint64_t readed64  = {0},
              data64    = {0};
    static
    uint32_t rfc[] =
    {
        4,  /* Number of items */
        MK_RFC(EMI_RFC_RP_DIV_1, TRFC_6 ),
        MK_RFC(EMI_RFC_RP_DIV_1, TRFC_13),
        MK_RFC(EMI_RFC_RP_DIV_2, TRFC_6 ),
        MK_RFC(EMI_RFC_RP_DIV_2, TRFC_13)
    };

    if(IS_NOT_SDRAM(bank))
    {
        rumboot_printf("Bank %d is not SDRAM. Check skipped.\n", bank);
        return TEST_OK;
    }

    emi_switch_bank(bank);

#ifdef CMAKE_BUILD_TYPE_DEBUG
    memset( (void*)CHECK_ADDR32(bank), 0x00, 16*sizeof(uint32_t) ); /* Anti-X */
    msync();
#endif

    for(rfcp = rfc+1, cnt = 0; cnt < rfc[0]; rfcp++, cnt++)
    {
        rumboot_printf("Check SDRAM: RP=0x%X, TRFC=0x%X...\n",
                (*rfcp >> EMI_RFC_RP_i  ) & EMI_RFC_RP_MASK,
                (*rfcp >> EMI_RFC_TRFC_i) & EMI_RFC_TRFC_MASK);
        EMI_WRITE(EMI_RFC, *rfcp);

        /* 8 bits */
        rumboot_printf("-> 8 bits...\n");
        iowrite8(CHECK_CONST8, CHECK_ADDR8(bank));
        msync();
        readed8 = ioread8(CHECK_ADDR8(bank));
        result = (readed8 == CHECK_CONST8);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 8 bits failed.");
        status |= !result;
        /* ------ */

        /* 16 bits */
        rumboot_printf("-> 16 bits...\n");
        iowrite16(CHECK_CONST16, CHECK_ADDR16(bank));
        msync();
        readed16 = ioread16(CHECK_ADDR16(bank));
        result = (readed16 == CHECK_CONST16);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 16 bits failed.");
        status |= !result;
        /* ------- */

        /* 32 bits */
        rumboot_printf("-> 32 bits...\n");
        iowrite32(CHECK_CONST32, CHECK_ADDR32(bank));
        msync();
        readed32 = ioread32(CHECK_ADDR32(bank));
        result = (readed32 == CHECK_CONST32);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 32 bits failed.");
        status |= !result;
        /* ------- */

        /* 64 bits */
        rumboot_printf("-> 64 bits...\n");
        data64.u = CHECK_CONST64;
        iowrite64d(data64.d, CHECK_ADDR64(bank));
        msync();
        readed64.d = ioread64d(CHECK_ADDR64(bank));
        result = (readed64.u == CHECK_CONST64);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 64 bits failed.");
        status |= !result;
        /* ------- */

        rumboot_printf("Done.\n\n");
    }

    rfc_saved = EMI_READ(EMI_RFC);
    rumboot_printf("Check SDRAM complete.\n");

    EMI_WRITE(EMI_RFC, rfc_saved);
    return status;
}

DEFINE_CHECK(hstsr_ecc_on)  /* 2.2.2 */
{
    uint32_t     hstsr,
                 offset = DEFAULT_OFFSET,
                 result = TEST_OK,
                 status = TEST_OK;
    volatile
    uint32_t     readed = 0;
    volatile
    uint32_t     tmp    = 0;
    uint8_t      eccval = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(IS_NOR(bank))
    {
        rumboot_printf("Using alternative check method for NOR RAM.\n");
        return CHECK_FUNC(hstsr_ecc_on_nor)(CF_ARGS);
    }

    emi_switch_bank(bank);
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Regular write/read */
    rumboot_printf("First stage: Regular write/read (ECC ON)...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after regular write.\n",
            readed, eccval);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Regular write/read error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    /* ------------- */

    /* Make single error */
    rumboot_printf("Second stage: Make single error...\n");
    clear_all_irqs();
    iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after single error.\n",
            readed, eccval);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after single error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = (global_irr & SINGLE_ERR_FLAG(bank));
    TEST_ASSERT(result, "EMI: Single error interrupt flag not risen.");
    status |= !result;
    /* ----------------- */

    /* Reset interrupts */
    clear_all_irqs();

    /* Make double error */
    rumboot_printf("Third stage: Make double error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after double error.\n",
            readed, eccval);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after double error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = (!(global_irr & SINGLE_ERR_FLAG(bank)));
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    result = (global_irr & DOUBLE_ERR_FLAG(bank));
    TEST_ASSERT(result, "EMI: Double error interrupt flag not risen.");
    status |= !result;
    /* ----------------- */

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(hstsr_ecc_off) /* 2.2.3 */
{
    uint32_t     hstsr,
                 offset = DEFAULT_OFFSET,
                 result = TEST_OK,
                 status = TEST_OK;
    volatile
    uint32_t     readed = 0;
    volatile
    uint32_t     tmp    = 0;
    uint8_t      eccval = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(IS_NOR(bank))
    {
        rumboot_printf("Using alternative check method for NOR RAM.\n");
        return CHECK_FUNC(hstsr_ecc_off_nor)(CF_ARGS);
    }

    emi_switch_bank(bank);
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(GET_BIT(hstsr, bank)) ECC_OFF(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Regular write/read */
    rumboot_printf("First stage: Regular write/read (ECC OFF)...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after regular write.\n",
            readed, eccval);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Regular write/read error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    clear_all_irqs();
    /* ------------- */

    /* Make single error */
    rumboot_printf("Second stage: Make single error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after single error.\n",
            readed, eccval);
    result = (readed == (CHECK_CONST ^ MK_ERR_1));
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after single error.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = (!(global_irr & SINGLE_ERR_FLAG(bank)));
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    /* ----------------- */

    /* Reset interrupts */
    clear_all_irqs();

    /* Make double error */
    rumboot_printf("Third stage: Make double error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after double error.\n",
            readed, eccval);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after double error.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = (!(global_irr & SINGLE_ERR_FLAG(bank)));
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    result = (!(global_irr & DOUBLE_ERR_FLAG(bank)));
    TEST_ASSERT(result, "EMI: Unexpected double error interrupt flag risen.");
    status |= !result;
    /* ----------------- */

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(hstsr_ecc_on_nor)  /* 2.2.2 for NOR */
{
    rumboot_printf("Check of NOR RAM is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(hstsr_ecc_off_nor) /* 2.2.3 for NOR */
{
    rumboot_printf("Check of NOR RAM is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(error_counter) /* 2.2.4 */
{
    uint32_t status = TEST_OK,
             offset = DEFAULT_OFFSET,
             eccsta = 0,
             datsta = 0,
             cntsta = 0,
             hstsr  = 0,
             idx    = 0,
             cnt    = 0;
    volatile
    uint32_t readed = 0,
             tmp    = 0;
    uint8_t  eccval = 0,
             cntval = 0,
             cntexp = 0;

    tmp ^= tmp; /* Prevent compiler warning */

    emi_switch_bank(bank);
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Make single error */
    rumboot_printf(" +++ Make single error...\n");
    clear_all_irqs();
    if(IS_NOR(bank))
    {
        rumboot_printf( "This bank type is NOR! "
                        "Using alternative write method.\n");
        EMI_WRITE(EMI_ECCWRR, CHECK_ECC_CODE);
        EMI_WRITE(EMI_FLCNTRL,
                (EMI_READ(EMI_FLCNTRL) & ~ECC_CTRL_MASK)
                | ECC_CTRL_CNT_ECCWRR);
        rumboot_printf("Writing 0x%X to NOR at 0x%X...\n",
                CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
        nor_write32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
        // msync();
    } else
    {
        rumboot_printf("Writing 0x%X to 0x%X...\n",
                CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
        iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
        // msync();
        tmp = EMI_READ(EMI_WECR);
        EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
        rumboot_printf("Writing 0x%X to 0x%X...\n",
                CHECK_CONST, CHECK_ADDR(bank));
        iowrite32(CHECK_CONST, CHECK_ADDR(bank));
        // msync();
    }
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    rumboot_printf("Reading from 0x%X in iterations...\n", CHECK_ADDR(bank));
    g_sp_info.bank = (uint8_t)(bank & 0xFF);
    for(idx = 0; idx < 4; idx++)
    {
        uint32_t cntlim = ((uint32_t)(errcnt_vals[idx]));
        datsta = eccsta = cntsta = 0;
        g_sp_info.cmpv  = errcnt_vals[idx];
        g_sp_info.flag  = 0x01;
        /* Reset single error counter */
        EMI_WRITE(emi_ecnt[bank], 0);
        rumboot_printf("Check error count limit %d...\n", cntlim);
        for(cntexp = 1, cnt = 0; cnt <= cntlim; cnt++, cntexp++)
        {
            rumboot_printf(" ### Read %d/%d\n",
                    cnt + 1, (uint32_t)(errcnt_vals[idx]) + 1);
            if((cntexp == cntlim) && (g_sp_info.flag & 1))
                cntexp = 0;
            readed = ioread32(CHECK_ADDR(bank));
            msync();
            eccval = EMI_READ(EMI_ECCRDR);
            cntval = READ_ECNT(bank);
            datsta |= !(readed == CHECK_CONST);
            eccsta |= !(eccval == CHECK_ECC_CODE);
            cntsta |= !(cntval == cntexp);
            rumboot_printf("Error count = %d (expected: %d)\n",
                    cntval, cntexp);
        }
        status |= (datsta || eccsta || cntsta);
        TEST_ASSERT(!datsta, "EMI: Single error repair fails.");
        TEST_ASSERT(!eccsta, "EMI: Wrong ECC-code after single error.");
        TEST_ASSERT(!cntsta, "EMI: Wrong single error counter value.");
    }
    g_sp_info.flag = 0x00;
    /* ----------------- */

    /* Reset interrupts */
    clear_all_irqs();

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(single_interrupts) /* 2.2.5 */
{
    uint32_t hstsr,
             ssi,
             imr,
             offset = DEFAULT_OFFSET,
             result = TEST_OK,
             status = TEST_OK;
    volatile
    uint32_t readed = 0,
             tmp    = 0;
    uint8_t  eccval = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(IS_NOT_SRAM(bank))
    {
        rumboot_printf("Bank #%d is not %s. Check skipped.\n",
                bank, btyp_descr[BTYP_SRAM]);
        return TEST_OK;
    }

    emi_switch_bank(bank);
    EMI_WRITE(EMI_WECR, 0);

    hstsr   = EMI_READ(EMI_HSTSR);
    imr     = EMI_READ(EMI_IMR);

    /* Mask double error interrupt */
    EMI_WRITE(EMI_IMR, IMR_UNMASK_ALL ^ IMR_UNMASK_DOUBLE);

    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);
    emi_set_ssi(&ssi, EMI_TRDY_VAL, bank);

    /* Make single error */
    rumboot_printf(" +++ Make single error...\n");
    clear_all_irqs();
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    emi_switch_bank(other_banks[bank]);
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after single error.\n",
            readed, eccval);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after single error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = (irq_cnt.b[IRQ_CNT_SINGLE] == 1);
    TEST_ASSERT(result, "EMI: Single error interrupt not happen.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_DOUBLE];
    TEST_ASSERT(result, "EMI: Unexpected double error interrupt happen.");
    status |= !result;
    result = !!irq_cnt.b[IRQ_CNT_RDY];
    TEST_ASSERT(result, "EMI: Read/write ready interrupt not happen.");
    status |= !result;
    rumboot_printf("IRQ Counters -> #56=%d,#57=%d,#58=%d,Other=%d\n",
            (uint32_t)irq_cnt.b[0], (uint32_t)irq_cnt.b[1],
            (uint32_t)irq_cnt.b[2], (uint32_t)irq_cnt.b[3]);
    /* ----------------- */

    /* Make double error */
    rumboot_printf(" +++ Make double error...\n");
    clear_all_irqs();
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    emi_switch_bank(other_banks[bank]);
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after double error.\n",
            readed, eccval);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after double error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_SINGLE];
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt happen.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_DOUBLE];
    TEST_ASSERT(result, "EMI: Unexpected double error interrupt happen.");
    status |= !result;
    result = !!irq_cnt.b[IRQ_CNT_RDY];
    TEST_ASSERT(result, "EMI: Read/write ready interrupt not happen.");
    status |= !result;
    rumboot_printf("IRQ Counters -> #56=%d,#57=%d,#58=%d,Other=%d\n",
            (uint32_t)irq_cnt.b[0], (uint32_t)irq_cnt.b[1],
            (uint32_t)irq_cnt.b[2], (uint32_t)irq_cnt.b[3]);
    clear_all_irqs();
    /* ----------------- */

    EMI_WRITE((EMI_REG)emi_ss[bank], ssi);
    EMI_WRITE(EMI_HSTSR, hstsr);
    EMI_WRITE(EMI_IMR,   imr);
    return status;
}

DEFINE_CHECK(double_interrupts) /* 2.2.6 */
{
    uint32_t hstsr,
             ssi,
             imr,
             offset = DEFAULT_OFFSET,
             result = TEST_OK,
             status = TEST_OK;
    volatile
    uint32_t readed = 0,
             tmp    = 0;
    uint8_t  eccval = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(IS_NOT_SRAM(bank))
    {
        rumboot_printf("Bank #%d is not %s. Check skipped.\n",
                bank, btyp_descr[BTYP_SRAM]);
        return TEST_OK;
    }

    emi_switch_bank(bank);
    EMI_WRITE(EMI_WECR, 0);

    hstsr   = EMI_READ(EMI_HSTSR);
    imr     = EMI_READ(EMI_IMR);

    /* Mask single error interrupt */
    EMI_WRITE(EMI_IMR, IMR_UNMASK_ALL ^ IMR_UNMASK_SINGLE);

    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);
    emi_set_ssi(&ssi, EMI_TRDY_VAL, bank);

    /* Make single error */
    rumboot_printf(" +++ Make single error...\n");
    clear_all_irqs();
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    emi_switch_bank(other_banks[bank]);
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after single error.\n",
            readed, eccval);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after single error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_SINGLE];
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt happen.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_DOUBLE];
    TEST_ASSERT(result, "EMI: Unexpected double error interrupt happen.");
    status |= !result;
    result = !!irq_cnt.b[IRQ_CNT_RDY];
    TEST_ASSERT(result, "EMI: Read/write ready interrupt not happen.");
    status |= !result;
    rumboot_printf("IRQ Counters -> #56=%d,#57=%d,#58=%d,Other=%d\n",
            (uint32_t)irq_cnt.b[0], (uint32_t)irq_cnt.b[1],
            (uint32_t)irq_cnt.b[2], (uint32_t)irq_cnt.b[3]);
    /* ----------------- */

    /* Make double error */
    rumboot_printf(" +++ Make double error...\n");
    clear_all_irqs();
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    rumboot_printf("Writing 0x%X to 0x%X...\n",
            CHECK_CONST, CHECK_ADDR(bank));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    emi_switch_bank(other_banks[bank]);
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    msync();
    eccval = EMI_READ(EMI_ECCRDR);
    rumboot_printf("READED: 0x%X (ECC=0x%X) after double error.\n",
            readed, eccval);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (eccval == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code after double error.");
    status |= !result;
    result = (READ_ECNT(bank) == 1);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    result = !irq_cnt.b[IRQ_CNT_SINGLE];
    TEST_ASSERT(result, "EMI: Unexpected single error interrupt happen.");
    status |= !result;
    result = !!irq_cnt.b[IRQ_CNT_DOUBLE];
    TEST_ASSERT(result, "EMI: Double error interrupt not happen.");
    status |= !result;
    result = !!irq_cnt.b[IRQ_CNT_RDY];
    TEST_ASSERT(result, "EMI: Read/write ready interrupt not happen.");
    status |= !result;
    rumboot_printf("IRQ Counters -> #56=%d,#57=%d,#58=%d,Other=%d\n",
            (uint32_t)irq_cnt.b[0], (uint32_t)irq_cnt.b[1],
            (uint32_t)irq_cnt.b[2], (uint32_t)irq_cnt.b[3]);
    clear_all_irqs();
    /* ----------------- */

    EMI_WRITE((EMI_REG)emi_ss[bank], ssi);
    EMI_WRITE(EMI_HSTSR, hstsr);
    EMI_WRITE(EMI_IMR,   imr);
    return status;
}


int main(void)
{
    uint32_t     test_result = TEST_OK,
                 test_status = TEST_OK,
                 init_result = INIT_OK;
    ckinfo_t    *test;

    DO_INIT(init_result, ppc_hw);       /* Init PPC hardware    */
    DO_INIT(init_result, emi_hw);       /* Init EMI hardware    */
    DO_INIT(init_result, interrupts);   /* Init interrupts      */

    // test_event_send_test_id("test_oi10_em2_202");

    FOREACH_TEST(test,tests)
    {
        int bank, active;
        rumboot_printf("\n\n *** Check EMI: (%s) *** \n", test->desc);
        FOREACH_BANK(bank,active,NUMBER_OF_BANKS,ENABLED_EM_BANKS)
        {
            rumboot_printf(" ---> %s bank #%d (type: %d - %s)...\n",
                    active?"Check":"Skip", bank,
                    EMI_TYPE(bank), btyp_descr[EMI_TYPE(bank)]);
            if(!active) continue;
            /* Reset interrupts */
            clear_all_irqs();
            EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
            writer32 = iowrite32;
            test_result = test->test(test, bank);
            rumboot_printf("%s!\n\n", test_result ? "Fail" : "Success");
            test_status |= test_result;
        }
        rumboot_printf(" ******************************************** \n");
    }

    rumboot_printf("TEST %s!\n", !test_status ? "OK" : "ERROR");

    return test_status;
}

