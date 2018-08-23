#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/regs/regs_emi.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>


/* Integer constant macros */
#define EMI_BASE                DCR_EM2_EMI_BASE
#define TEST_OK                 0x00000000
#define TEST_ERROR              0x00000000
#define INIT_OK                 TEST_OK
#define INIT_ERROR              TEST_ERROR
#define NUMBER_OF_BANKS         6
#define IRR_RST_ALL             0x0001FFFF
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
#define ENABLED_EM_BANKS        EMBANKS( 1,0,0,0,0,0 )
#define CHECK_OFFSET            0x00000100
#define CHECK_CONST             0xF00D4BEE
#define CHECK_ECC_CODE          0x10
#define MK_ERR_1                0x0001
#define MK_ERR_2                0x1100

/* Other macros */
#define CHECKLIST_TERMINATOR    {NULL,NULL}

/* Function and block definition macros */
#define BIT(N)                  (1 << (N))
#define EMIA(ADDR)              (EMI_BASE + ADDR)
#define EMI_READ(ADDR)          dcr_read (EMIA(ADDR))
#define EMI_WRITE(ADDR,DATA)    dcr_write(EMIA(ADDR),(DATA))
#define EMI_TYPE(BANK)          emi_get_bank_cfg_cached(bank)->ssx_cfg.BTYP
#define MASK_ECNT(BANK)         (0xFF >> emi_ecnt_b[BANK])
#define READ_ECNT(BANK)         ((EMI_READ(emi_ecnt[BANK])       \
                                    >> emi_ecnt_b[BANK]) & 0xFF)
#define SINGLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 0))
#define DOUBLE_ERR_FLAG(BANK)   (BIT(((BANK) << 1) + 1))
#define ECC_ON(BANK)            EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) |  (1 << (BANK)));
#define ECC_OFF(BANK)           EMI_WRITE(EMI_HSTSR, EMI_READ(EMI_HSTSR) & ~(1 << (BANK)));
#define CHECK_ADDR(BANK)        (emi_bank_bases[BANK] + CHECK_OFFSET)
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

typedef struct
{
    uint32_t    (*r)(uint32_t);
    void        (*w)(uint32_t,uint32_t);
} emi_access_api;

typedef struct ckinfo_t             ckinfo_t;
typedef struct rumboot_irq_entry    rumboot_irq_entry;
typedef volatile uint32_t           irq_flags_t;

/* Global variables */
static irq_flags_t IRQ[8] = {0,0,0,0,0,0,0,0};
static rumboot_irq_entry *irq_table = NULL;
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
/* Memory access API definition */
static volatile const emi_access_api emi_api_list[8] =
    {
        /* 0: Async SRAM         1: Async NOR   */
        {ioread32,  iowrite32}, {ioread32,  nor_write32},
        /* 2: SSRAM               3: Reserved    */
        {ioread32,  iowrite32}, {NULL,      NULL       },
        /* 4: Pipelined/RDY       5: Reserved    */
        {ioread32,  iowrite32}, {NULL,      NULL       },
        /* 6: SDRAM               7: Reserved    */
        {ioread32,  iowrite32}, {NULL,      NULL       }
    };
/* Memory banks types descriptions */
static const char *btyp_descr[8] =
    {
/*       0           1           2           3          */
        "SRAM",     "NOR",      "SSRAM",    "UNKNOWN",
/*       4           5           6           7          */
        "PIPELINE", "UNKNOWN",  "SDRAM",    "UNKNOWN"
    };

int is_irq(irq_flags_t *irq_f)
{
    int r, i;
    for(r = 0, i = 0; i < 8; i++) r |= irq_f[i];
    return r;
}

void clear_all_irqs(irq_flags_t *irq_f)
{
    int i;
    for(i=0; i<8; i++) irq_f[i] = 0;
}

void emi_irq_handler( int irq, void *arg )
{
    rumboot_printf("IRQ #%d received.\n", irq);
    IRQ[irq >> 5] = (1 << (irq & 0x1F));
}


DEFINE_INIT(interrupts)
{
    uint32_t    *irq_item;
    static
    uint32_t    irq_list[] =
    {
        SRAM_INT,
        NOR_INT,
        EMI_CNTR_INT_0,
        EMI_CNTR_INT_1,
        EMI_CNTR_INT_2,
        EMI_CNTR_INT_3,
        ~0
    };
    rumboot_putstring ("\tStart IRQ initialization...\n");

    clear_all_irqs(IRQ);
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

/* Checklist. Put your tests here. */
BEGIN_TESTS(tests)
    CHECK_ITEM(rfc,                     "regeneration configure"    ),
    CHECK_ITEM(hstsr_ecc_on,            "mode control reg (ECC ON)" ),
    CHECK_ITEM(hstsr_ecc_off,           "mode control reg (ECC OFF)"),
    CHECK_ITEM(error_counter,           "single error counter"      ),
    CHECK_ITEM(single_interrupts,       "single interrupts"         ),
    CHECK_ITEM(double_interrupts,       "double interrupts"         ),
END_TESTS;

DEFINE_CHECK(rfc)
{
    if(EMI_TYPE(bank) != BTYP_SDRAM)
    {
        rumboot_printf("Bank %d is not SDRAM. Check skipped.\n", bank);
        return TEST_OK;
    }
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(hstsr_ecc_on)
{
    uint32_t hstsr, wecr,
             readed = 0,
             result = TEST_OK,
             status = TEST_OK;
    volatile const emi_access_api *api = &emi_api_list[EMI_TYPE(bank)];
    if(EMI_TYPE(bank) == BTYP_NOR)
    {
        rumboot_printf("NOR is not supported. Check skipped.\n", bank);
        return TEST_OK;
    }
    hstsr = EMI_READ(EMI_HSTSR);
    wecr  = EMI_READ(EMI_WECR);
    if(!GET_BIT(hstsr, bank)) ECC_ON(bank);
    /* Reset interrupts */
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], EMI_READ(emi_ecnt[bank]) & ~MASK_ECNT(bank));

    /* Write & check */
    rumboot_putstring("First stage: Regular write/read...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after regular write.\n", readed);
    TEST_ASSERT(result = (readed == CHECK_CONST),
            "EMI: Regular write/read error.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 0),
            "EMI: Wrong single error counter value.");
    status |= !result;
    /* ------------- */

    /* Make single error */
    rumboot_putstring("Second stage: Make single error...\n");
    api->w(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after single error.\n", readed);
    TEST_ASSERT(result = (readed == CHECK_CONST),
            "EMI: Single error repair fails.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code after single error.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 1),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_IRR) & SINGLE_ERR_FLAG(bank)),
            "EMI: Single error interrupt flag not risen.");
    status |= !result;
    /* Restore EMI_WECR */
    EMI_WRITE(EMI_WECR, wecr);
    /* ----------------- */

    /* Reset interrupts */
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);

    /* Make double error */
    rumboot_putstring("Third stage: Make double error...\n");
    api->w(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after double error.\n", readed);
    TEST_ASSERT(result = (readed == (CHECK_CONST ^ MK_ERR_2)),
            "EMI: Wrong value after double error.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code after double error.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 1),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (!(EMI_READ(EMI_IRR) & SINGLE_ERR_FLAG(bank))),
            "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_IRR) & DOUBLE_ERR_FLAG(bank)),
            "EMI: Double error interrupt flag not risen.");
    status |= !result;
    /* Restore EMI_WECR */
    EMI_WRITE(EMI_WECR, wecr);
    /* ----------------- */

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(hstsr_ecc_off)
{
    uint32_t hstsr, wecr,
             readed = 0,
             result = TEST_OK,
             status = TEST_OK;
    volatile const emi_access_api *api = &emi_api_list[EMI_TYPE(bank)];
    if(EMI_TYPE(bank) == BTYP_NOR)
    {
        rumboot_printf("NOR is not supported. Check skipped.\n", bank);
        return TEST_OK;
    }
    hstsr = EMI_READ(EMI_HSTSR);
    wecr  = EMI_READ(EMI_WECR);
    if(GET_BIT(hstsr, bank)) ECC_OFF(bank);
    /* Reset interrupts */
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);
    /* Reset single error counter */
    EMI_WRITE(emi_ecnt[bank], EMI_READ(emi_ecnt[bank]) & ~MASK_ECNT(bank));

    /* Write & check */
    rumboot_putstring("First stage: Regular write/read...\n");
    rumboot_printf("Writing at 0x%X <- 0x%X...\n",
            CHECK_ADDR(bank), CHECK_CONST);
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    msync();
    rumboot_printf("Reading at 0x%X...\n", CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after regular write.\n", readed);
    TEST_ASSERT(result = (readed == CHECK_CONST),
            "EMI: Regular write/read error.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 0),
            "EMI: Wrong single error counter value.");
    status |= !result;
    /* ------------- */

    /* Make single error */
    rumboot_putstring("Second stage: Make single error...\n");
    api->w(CHECK_CONST ^ MK_ERR_1, CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D24_D31));
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after single error.\n", readed);
    TEST_ASSERT(result = (readed == (CHECK_CONST ^ MK_ERR_1)),
            "EMI: Single error repair fails.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code after single error.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 0),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (!(EMI_READ(EMI_IRR) & SINGLE_ERR_FLAG(bank))),
            "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    /* Restore EMI_WECR */
    EMI_WRITE(EMI_WECR, wecr);
    /* ----------------- */

    /* Reset interrupts */
    EMI_WRITE(EMI_IRR_RST, IRR_RST_ALL);

    /* Make double error */
    rumboot_putstring("Third stage: Make double error...\n");
    api->w(CHECK_CONST ^ MK_ERR_2, CHECK_ADDR(bank));
    EMI_WRITE(EMI_WECR, EMI_READ(EMI_WECR) | BIT(EMI_BITS_D16_D23));
    api->w(CHECK_CONST, CHECK_ADDR(bank));
    readed = api->r(CHECK_ADDR(bank));
    rumboot_printf("READED: 0x%X after double error.\n", readed);
    TEST_ASSERT(result = (readed == (CHECK_CONST ^ MK_ERR_2)),
            "EMI: Wrong value after double error.");
    status |= !result;
    TEST_ASSERT(result = (EMI_READ(EMI_ECCRDR) == CHECK_ECC_CODE),
            "EMI: Wrong ECC-code after double error.");
    status |= !result;
    TEST_ASSERT(result = (READ_ECNT(bank) == 0),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (!(EMI_READ(EMI_IRR) & SINGLE_ERR_FLAG(bank))),
            "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    TEST_ASSERT(result = (!(EMI_READ(EMI_IRR) & DOUBLE_ERR_FLAG(bank))),
            "EMI: Unexpected double error interrupt flag risen.");
    status |= !result;
    /* Restore EMI_WECR */
    EMI_WRITE(EMI_WECR, wecr);
    /* ----------------- */

    EMI_WRITE(EMI_HSTSR, hstsr);
    return status;
}

DEFINE_CHECK(error_counter)
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(single_interrupts)
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(double_interrupts)
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
        rumboot_printf("\n\nCheck EMI: (%s)...\n", test->desc);
        FOREACH_BANK(bank,active,NUMBER_OF_BANKS,ENABLED_EM_BANKS)
        {
            rumboot_printf("%s bank #%d (type: %d - %s)...\n",
                    active?"Check":"Skip", bank,
                    EMI_TYPE(bank), btyp_descr[EMI_TYPE(bank)]);
            if(!active) continue;
            test_result = test->test(test, bank);
            rumboot_printf("%s!\n", test_result ? "Fail" : "Success");
            test_status |= test_result;
        }
    }

    rumboot_putstring(!test_status ? "TEST OK!\n" : "TEST ERRER!\n");

    return test_status;
}

