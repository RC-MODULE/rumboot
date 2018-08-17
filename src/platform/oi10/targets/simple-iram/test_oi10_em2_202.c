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
#define IRQ_FLAGS               (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define EMBANKS(A,B,C,D,E,F)    (((A) & 1) << 0) |  \
                                (((B) & 1) << 1) |  \
                                (((C) & 1) << 2) |  \
                                (((D) & 1) << 3) |  \
                                (((E) & 1) << 4) |  \
                                (((F) & 1) << 5)

/* Config */
/*                      ENABLED BANK -> |0|1|2|3|4|5| */
#define ENABLED_EM_BANKS        EMBANKS( 1,0,0,0,1,0 )

/* Other macros */
#define CHECKLIST_TERMINATOR    {NULL,NULL}

/* Function and block definition macros */
#define EMI_READ(ADDR)          dcr_read  ((EMI_BASE) + (ADDR)        )
#define EMI_WRITE(ADDR,DATA)    dcr_write ((EMI_BASE) + (ADDR), (DATA))
#define READ_ECNT(BANK)         (EMI_READ(emi_ecnt[BANK])       \
                                    >> emi_ecnt_b[BANK]) & 0xFF
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
static volatile const uintptr_t emi_bank_bases[8] =
    {
        EM2_BANK0_BASE, EM2_BANK1_BASE,
        EM2_BANK2_BASE, EM2_BANK3_BASE,
        EM2_BANK4_BASE, EM2_BANK5_BASE, 0,0
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
    if(emi_get_bank_cfg_cached(bank)->ssx_cfg.BTYP != BTYP_SDRAM)
    {
        rumboot_printf("Bank %d is not SDRAM. Check skipped.\n", bank);
        return TEST_OK;
    }
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(hstsr_ecc_on)
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
}

DEFINE_CHECK(hstsr_ecc_off)
{
    rumboot_putstring("Check is not implemented!\n");
    return TEST_OK;
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
            rumboot_printf("%s bank #%d...\n", active?"Check":"Skip", bank);
            if(!active) continue;
            test_result = test->test(test, bank);
            rumboot_printf("%s!\n", test_result ? "Fail" : "Success");
            test_status |= test_result;
        }
    }

    rumboot_putstring(!test_status ? "TEST OK!\n" : "TEST ERRER!\n");

    return test_status;
}

