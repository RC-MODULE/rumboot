
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>


#define EVENT_INJECT_ERR_S          TEST_EVENT_CODE_MIN + 0
#define EVENT_INJECT_ERR_D          TEST_EVENT_CODE_MIN + 1
#define EVENT_CHK_IRQ		        TEST_EVENT_CODE_MIN + 2

#define CONST_1                     0xBABA0001

#define ADDR_SE_SRAM                EM2_BASE
#define ADDR_DE_SRAM                EM2_BASE + 0x100

#define TEST_OK                     0x00000000
#define TEST_ERROR                  0x00000001

#define RAM_WRITERS                 iowrite8,   iowrite16,  iowrite32
#define RAM_READERS                 ioread8,    ioread16,   ioread32
#define NOR_WRITERS                 NULL,       NULL,       nor_write32
#define NOR_READERS                 RAM_READERS
#define CKLIST_END                  {NULL,0,0,NULL}

#define CMP_EQ(ARG1,ARG2,RES)       ((RES) |= !((ARG1) == (ARG2)))
#define CMP_NE(ARG1,ARG2,RES)       ((RES) |= !((ARG1) != (ARG2)))

typedef struct
{
    void        (*wb)(uint8_t, uint32_t);
    void        (*wh)(uint16_t,uint32_t);
    void        (*ww)(uint32_t,uint32_t);
    uint8_t     (*rb)(uint32_t);
    uint16_t    (*rh)(uint32_t);
    uint32_t    (*rw)(uint32_t);
} memio_api_t;

typedef struct
{
    memio_api_t *api;
    uint32_t     base;
    uint32_t     event;
    char        *comment;
} ckinfo_t;

uint32_t check_emi(ckinfo_t *ck_entry)
{
    static
    uint32_t    long_tpl[] = {0xDEBA0001, 0xDEDA001, 0xDEDA0101, 0xDEDA0100};
    static
    uint8_t     byte_tpl[] = {0xDE, 0xDA, 0x01, 0x00};

    int          i;
    uint32_t     ck_result  = TEST_OK;

    rumboot_printf("CHECK %s\n", ck_entry->comment);

    (ck_entry->api.ww)(CONST_1, ck_entry->base);
    rumboot_printf("READED DATA = 0x%X\n", (ck_entry->api.rw)(ck_entry->base));

    for(i = 0; i < 4; i++)
    {
        (ck_entry->api.wb)(byte_tpl[i], base_addr + i);
        rumboot_printf("writing byte, data = 0x%X\n",
                (ck_entry->api.rw)(ck_entry->base));
        TEST_ASSERT(
                CMP_EQ((ck_entry->api.rw)(ck_entry->base),
                        long_tpl[i], ck_result),
                "READ ERROR");
    }

    rumboot_putstring("done.\n");

    rumboot_putstring("Inject error by h/w event...");
    // test_event(ev_code);

    rumboot_putstring("Check data\n");
    rumboot_printf("Expected = 0x%X, Actual = 0x%X\n",
            long_tpl[3], (ck_entry->api.rw)(ck_entry->base));
    TEST_ASSERT(CMP_EQ((ck_entry->api.rw)(ck_entry->base),
            long_tpl[3], ck_result),
            "ECC ERROR");
    rumboot_putstring("Success!\n");
    return ck_result;
}

uint32_t main(void)
{
    static
    memio_api_t ram_api = {RAM_WRITERS, RAM_READERS},
                nor_api = {NOR_WRITERS, NOR_READERS};
    static
    ckinfo_t    check_list[] =
    {
            {&ram_api, ADDR_SE_SRAM, EVENT_INJECT_ERR_S, "SINGLE BIT ERROR"},
            {&ram_api, ADDR_SE_SRAM, EVENT_INJECT_ERR_S, "DOUBLE BIT ERROR"},
            CKLIST_END
    };

    uint32_t     test_result    = TEST_OK;
    ckinfo_t    *ckitem;

    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
    for(ckitem = check_list; ckitem->api; ckitem++)
        test_result |= check_emi(ckitem);

    rumboot_putstring("TEST OK!\n");

    return test_result;
}

