#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>

#include <platform/interrupts.h>


/* Integer constant macros */
#define EMI_BASE                DCR_EM2_EMI_BASE
#define TEST_OK                 0x00000000
#define TEST_ERROR              0x00000000
#define INIT_OK                 TEST_OK
#define INIT_ERROR              TEST_ERROR
#define NUMBER_OF_BANKS         6
#define IRR_RST_ALL             0x0001FFFF
#define EMI_RFC_RP_DIV_1        0b11110011110011
#define EMI_RFC_RP_DIV_2        0b01111001111001
#define EMI_RFC_RP_MASK         0x8FFF
#define EMI_RFC_TRFC_MASK       0x0007
#define IMR_UNMASK_ALL          IRR_RST_ALL
#define IRQ_FLAGS               (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define EMI_BITS_D0_D7          EMI_WECR_BWE0_i
#define EMI_BITS_D8_D15         EMI_WECR_BWE1_i
#define EMI_BITS_D16_D23        EMI_WECR_BWE2_i
#define EMI_BITS_D24_D31        EMI_WECR_BWE3_i
#define EMI_BITS_ED0_ED7        EMI_WECR_BWE4_i
#define EMBANKS(A,B,C,D,E,F)    (!!(A) << 0) |  \
                                (!!(B) << 1) |  \
                                (!!(C) << 2) |  \
                                (!!(D) << 3) |  \
                                (!!(E) << 4) |  \
                                (!!(F) << 5)

/* Config */
/*                      ENABLED BANK -> |0|1|2|3|4|5| */
#define ENABLED_EM_BANKS        EMBANKS( 1,1,1,1,1,0 )
#define CHECK_OFFSET            0x00000100
#define CHECK_OFFSET8           (CHECK_OFFSET + ((2 * 4) + 3))
#define CHECK_OFFSET16          (CHECK_OFFSET + ((3 * 4) + 2))
#define CHECK_OFFSET32          CHECK_OFFSET
#define CHECK_OFFSET64          (CHECK_OFFSET + ((4 * 4) + 2))
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
#define BIT(N)                  (1 << (N))
#define EMIA(ADDR)              (EMI_BASE + ADDR)
#define EMI_READ(ADDR)          dcr_read (EMIA(ADDR))
#define EMI_WRITE(ADDR,DATA)    dcr_write(EMIA(ADDR),(DATA))
#define EMI_TYPE(BANK)          emi_get_bank_cfg_cached(bank)->ssx_cfg.BTYP
#define MK_RFC(RP,TRFC)         ((((RP)   & 0x8FFF) << EMI_RFC_RP_i  ) |    \
                                 (((TRFC) & 0x0007) << EMI_RFC_TRFC_i))
#define MASK_ECNT(BANK)         (0xFF >> emi_ecnt_b[BANK])
#define READ_ECNT(BANK)         ((EMI_READ(emi_ecnt[BANK])       \
                                    >> emi_ecnt_b[BANK]) & 0xFF)
#define SINGLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 0))
#define DOUBLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 1))
#define ECC_ON(BANK)            EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) |  (1 << (BANK)));
#define ECC_OFF(BANK)           EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) & ~(1 << (BANK)));
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


struct ckinfo_t
{
    uint32_t   (*test)(struct ckinfo_t*, uint32_t);
    char        *desc;
};

typedef struct ckinfo_t             ckinfo_t;
typedef struct rumboot_irq_entry    rumboot_irq_entry;
typedef volatile uint32_t           irq_flags_t;
typedef volatile uint64_t           vuint64_t;
typedef volatile uint32_t           vuint32_t;
typedef volatile uint16_t           vuint16_t;
typedef volatile uint8_t            vuint8_t;

/* Global variables */
static irq_flags_t IRQ[8] = {0,0,0,0,0,0,0,0};
static rumboot_irq_entry *irq_table = NULL;
static volatile uint32_t global_irr = 0;
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

int is_irq(void)
{
    int r, i;
    for(r = 0, i = 0; i < 8; i++) r |= IRQ[i];
    return r;
}

void clear_all_irqs(void)
{
    int i;
    for(i=0; i<8; i++) IRQ[i] = 0;
}

void emi_irq_handler( int irq, void *arg )
{
    rumboot_printf("IRQ #%d received (IRR=0x%X).\n",
            irq, (global_irr = EMI_READ(EMI_IRR)));
    IRQ[irq >> 5] = BIT(irq & 0x1F);
    /* Reset interrupts */
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
}

static void ex_handler(int exid, const char *exname)
{
    rumboot_printf(
            "\n >>> EXCEPTION #%d (%s) at 0x%X with MCSR=0x%X! <<<\n",
            exid, exname, spr_read(SPR_MCSRR0), spr_read(SPR_MCSR_RW));
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
        ~0
    };
    rumboot_putstring ("\tStart IRQ initialization...\n");

    clear_all_irqs();
    rumboot_irq_cli();
    irq_table = rumboot_irq_create( NULL );
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_set_handler(irq_table, *irq_item,
            IRQ_FLAGS, emi_irq_handler, NULL);

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_enable(*irq_item);
    rumboot_irq_sei();
    /* Unmask all interrupts */
    EMI_WRITE(EMI_IMR, IMR_UNMASK_ALL);

    rumboot_irq_set_exception_handler(ex_handler);

    rumboot_putstring ("\tIRQ have been initialized.\n");

    return INIT_OK;
}

DEFINE_INIT(emi_hw)
{
    emi_init(DCR_EM2_EMI_BASE);
    return INIT_OK;
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
    CHECK_ITEM(rfc,                     "regeneration configure"    ),
    CHECK_ITEM(hstsr_ecc_on,            "mode control reg (ECC ON)" ),
    CHECK_ITEM(hstsr_ecc_off,           "mode control reg (ECC OFF)"),
    CHECK_ITEM(error_counter,           "single error counter"      ),
    CHECK_ITEM(single_interrupts,       "single interrupts"         ),
    CHECK_ITEM(double_interrupts,       "double interrupts"         ),
END_TESTS;

/* Checks */
DEFINE_CHECK(rfc)   /* 2.2.1 */
{
    uint32_t  rfc_saved = 0,
              cnt       = 0,
              result    = TEST_OK,
              status    = TEST_OK;
    uint32_t *rfcp      = NULL;
    vuint8_t  readed8   = 0;
    vuint16_t readed16  = 0;
    vuint32_t readed32  = 0;
    vuint64_t readed64  = 0;
    static
    uint32_t rfc[] =
    {
        4,
        MK_RFC(EMI_RFC_RP_DIV_1,TRFC_6 ),
        MK_RFC(EMI_RFC_RP_DIV_1,TRFC_13),
        MK_RFC(EMI_RFC_RP_DIV_2,TRFC_6 ),
        MK_RFC(EMI_RFC_RP_DIV_2,TRFC_13)
    };

    if(EMI_TYPE(bank) != BTYP_SDRAM)
    {
        rumboot_printf("Bank %d is not SDRAM. Check skipped.\n", bank);
        return TEST_OK;
    }

    /* Force bank switch */
    readed32 = ioread32(CHECK_ADDR(bank));

    for(rfcp = rfc+1; cnt < rfc[0]; rfcp++,cnt++)
    {
        rumboot_printf("Check SDRAM: RP=0x%X, TRFC=0x%X\n...\n",
                (*rfcp >> EMI_RFC_RP_i  ) & EMI_RFC_RP_MASK,
                (*rfcp >> EMI_RFC_TRFC_i) & EMI_RFC_TRFC_MASK);
        EMI_WRITE(EMI_RFC, *rfcp);

        /* 8 bits */
        rumboot_putstring("8 bits...\n");
        iowrite8(CHECK_CONST8, CHECK_ADDR8(bank));
        msync();
        readed8 = ioread8(CHECK_ADDR8(bank));
        result = (readed8 == CHECK_CONST8);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 8 bits failed.");
        status |= !result;
        /* ------ */

        /* 16 bits */
        rumboot_putstring("16 bits...\n");
        iowrite16(CHECK_CONST16, CHECK_ADDR16(bank));
        msync();
        readed16 = ioread16(CHECK_ADDR16(bank));
        result = (readed16 == CHECK_CONST16);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 16 bits failed.");
        status |= !result;
        /* ------- */

        /* 32 bits */
        rumboot_putstring("32 bits...\n");
        iowrite32(CHECK_CONST32, CHECK_ADDR32(bank));
        msync();
        readed32 = ioread32(CHECK_ADDR32(bank));
        result = (readed32 == CHECK_CONST32);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 32 bits failed.");
        status |= !result;
        /* ------- */

        /* 64 bits */
        rumboot_putstring("64 bits...\n");
        iowrite64(CHECK_CONST64, CHECK_ADDR64(bank));
        msync();
        readed64 = ioread64(CHECK_ADDR64(bank));
        result = (readed64 == CHECK_CONST64);
        TEST_ASSERT(result, "EMI_SDRAM: Write/read 64 bits failed.");
        status |= !result;
        /* ------- */

        rumboot_putstring("Done.\n");
    }

    rfc_saved = EMI_READ(EMI_RFC);
    rumboot_putstring("Check SDRAM complete.\n");

    EMI_WRITE(EMI_RFC, rfc_saved);
    return status;
}

DEFINE_CHECK(hstsr_ecc_on)  /* 2.2.2 */
{
    uint32_t hstsr,
             result = TEST_OK,
             status = TEST_OK;
    volatile
    uint32_t readed = 0;
    volatile
    uint32_t tmp    = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(EMI_TYPE(bank) == BTYP_NOR)
    {
        rumboot_printf("Using alternative check method for NOR RAM.\n");
        return CHECK_FUNC(hstsr_ecc_on_nor)(CF_ARGS);
    }
    /* Force bank switch */
    readed = ioread32(CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Regular write/read */
    rumboot_putstring("First stage: Regular write/read (ECC ON)...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after regular write.\n", readed);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Regular write/read error.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    /* ------------- */

    /* Make single error */
    rumboot_putstring("Second stage: Make single error...\n");
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
    rumboot_printf("READED: 0x%X after single error.\n", readed);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
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
    global_irr = 0x00000000;
    clear_all_irqs();

    /* Make double error */
    rumboot_putstring("Third stage: Make double error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after double error.\n", readed);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
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
    uint32_t hstsr,
             result = TEST_OK,
             status = TEST_OK;
    volatile
    uint32_t readed = 0;
    volatile
    uint32_t tmp    = 0;
    tmp ^= tmp; /* Prevent compiler warning */
    if(EMI_TYPE(bank) == BTYP_NOR)
    {
        rumboot_printf("Using alternative check method for NOR RAM.\n");
        return CHECK_FUNC(hstsr_ecc_off_nor)(CF_ARGS);
    }
    /* Force bank switch */
    readed = ioread32(CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(GET_BIT(hstsr, bank)) ECC_OFF(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Regular write/read */
    rumboot_putstring("First stage: Regular write/read (ECC OFF)...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = ioread32(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after regular write.\n", readed);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Regular write/read error.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
    TEST_ASSERT(result, "EMI: Wrong ECC-code.");
    status |= !result;
    result = (READ_ECNT(bank) == 0);
    TEST_ASSERT(result, "EMI: Wrong single error counter value.");
    status |= !result;
    clear_all_irqs();
    /* ------------- */

    /* Make single error */
    rumboot_putstring("Second stage: Make single error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after single error.\n", readed);
    result = (readed == (CHECK_CONST ^ MK_ERR_1));
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
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
    global_irr = 0x00000000;
    clear_all_irqs();

    /* Make double error */
    rumboot_putstring("Third stage: Make double error...\n");
    iowrite32(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    iowrite32(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    tmp = EMI_READ(EMI_WECR);
    EMI_WRITE(EMI_WECR, 0);
    readed = ioread32(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after double error.\n", readed);
    result = (readed == (CHECK_CONST ^ MK_ERR_2));
    TEST_ASSERT(result, "EMI: Wrong value after double error.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
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
    rumboot_putstring("Check of NOR RAM is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(hstsr_ecc_off_nor) /* 2.2.3 for NOR */
{
    rumboot_putstring("Check of NOR RAM is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(error_counter) /* 2.2.4 */
{
    uint32_t hstsr,
             result = TEST_OK,
             status = TEST_OK;
    volatile
    uint32_t readed = 0,
             tmp    = 0;

    if(EMI_TYPE(bank) == BTYP_NOR)
    {
        rumboot_printf("NOR RAM is not supported, skipping it.\n");
        return TEST_OK;
    }
    tmp ^= tmp; /* Prevent compiler warning */

    /* Force bank switch */
    readed = ioread32(CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, 0);
    hstsr = EMI_READ(EMI_HSTSR);
    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], 0);

    /* Make single error */
    rumboot_putstring("Make single error...\n");
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
    rumboot_printf("READED: 0x%X after single error.\n", readed);
    result = (readed == CHECK_CONST);
    TEST_ASSERT(result, "EMI: Single error repair fails.");
    status |= !result;
    result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE);
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
    global_irr = 0x00000000;
    clear_all_irqs();

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(single_interrupts) /* 2.2.5 */
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(double_interrupts) /* 2.2.6 */
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}


int main(void)
{
    uint32_t     test_result = TEST_OK,
                 test_status = TEST_OK,
                 init_result = INIT_OK;
    ckinfo_t    *test;

    DO_INIT(init_result, emi_hw);
    DO_INIT(init_result, interrupts);

    // test_event_send_test_id("test_oi10_em2_202");

    FOREACH_TEST(test,tests)
    {
        int bank, active;
        rumboot_printf("\n\n *** Check EMI: (%s) *** \n", test->desc);
        FOREACH_BANK(bank,active,NUMBER_OF_BANKS,ENABLED_EM_BANKS)
        {
            rumboot_printf("%s bank #%d (type: %d - %s)...\n",
                    active?"Check":"Skip", bank,
                    EMI_TYPE(bank), btyp_descr[EMI_TYPE(bank)]);
            if(!active) continue;
            /* Reset interrupts */
            clear_all_irqs();
            EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
            test_result = test->test(test, bank);
            rumboot_printf("%s!\n", test_result ? "Fail" : "Success");
            test_status |= test_result;
        }
        rumboot_printf(" ************************ \n");
    }

    rumboot_putstring(!test_status ? "TEST OK!\n" : "TEST ERROR!\n");

    return test_status;
}

