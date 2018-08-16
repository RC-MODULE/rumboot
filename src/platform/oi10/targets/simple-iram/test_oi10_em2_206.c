
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RUMBOOT_ASSERT_WARN_ONLY

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

#define TEST_OK                     0x00000000
#define TEST_ERROR                  0x00000001

void _nor_wr8  (uint8_t,  uint32_t);
void _nor_wr16 (uint16_t, uint32_t);

#define RAM_WRITERS                 iowrite8,   iowrite16,  iowrite32
#define RAM_READERS                 ioread8,    ioread16,   ioread32
#define NOR_WRITERS                 _nor_wr8,   NULL,       nor_write32
#define NOR_READERS                 RAM_READERS
#define CKLIST_END                  {NULL,0,0,NULL}

#define CMP_EQ(A1,A2,TR,SR)         ((SR) |= (TR = !((A1) == (A2))))
#define CMP_NE(A1,A2,TR,SR)         ((SR) |= (TR = !((A1) != (A2))))

/* Structures definitions */
typedef struct
{
    void        (*wrb)(uint8_t, uint32_t);
    void        (*wrh)(uint16_t,uint32_t);
    void        (*wrw)(uint32_t,uint32_t);
    uint8_t     (*rdb)(uint32_t);
    uint16_t    (*rdh)(uint32_t);
    uint32_t    (*rdw)(uint32_t);
} memio_api_t;

typedef struct
{
    memio_api_t *api;
    uint32_t     base;
    uint32_t     event;
    char        *comment;
} ckinfo_t;

/* Global vars */
static  /* Memory access APIs */
memio_api_t ram_api = {RAM_WRITERS, RAM_READERS},
            nor_api = {NOR_WRITERS, NOR_READERS};

static  /* Main check list. Add RAM regions here. */
ckinfo_t    check_list[] =
{ /*  API    | Base address (offset)   | Event |  Comment  */
//    {&ram_api,  SRAM0_BASE + 0x00000100, 0,       "SRAM0"},
//    {&ram_api,  SRAM1_BASE + 0x00000100, 0,       "SRAM1"},
    {&nor_api,  NOR_BASE   + 0x00000100, 0,       "NOR"  },
    CKLIST_END, /* List terminator */
    /* Fake entries */
    {&ram_api,  SRAM0_BASE + 0x00000800, 0,       "SRAM0"},
    {&ram_api,  SRAM1_BASE + 0x00000800, 0,       "SRAM1"},
    {&nor_api,  0xFFFFFFFC,     0x00000000,       "FAKE ENTRY"      }
};
static  /* Check templates 32 bits */
uint32_t    long_tpl[] = {0xDEBA0001, 0xDEDA0001, 0xDEDA0101, 0xDEDA0100};
static  /* Check templates 8 bits */
uint8_t     byte_tpl[] = {0xDE, 0xDA, 0x01, 0x00};

/* Write byte to NOR */
void _nor_wr8(uint8_t data, uint32_t addr)
{
    uint32_t    new_data,
                old_data = ioread32(addr);
    switch(addr & 0x03)
    {
        case 0:
            new_data = (old_data & 0x00FFFFFF) | (data << 0x18);
            break;
        case 1:
            new_data = (old_data & 0xFF00FFFF) | (data << 0x10);
            break;
        case 2:
            new_data = (old_data & 0xFFFF00FF) | (data << 0x08);
            break;
        case 3:
            new_data = (old_data & 0xFFFFFF00) | (data << 0x00);
            break;
    }
    nor_write32(new_data, addr);
}

uint32_t check_emi(ckinfo_t *ck_entry)
{
    int          i;
    uint32_t     s_result   = TEST_OK,
                 t_result   = TEST_OK,
                 data32;

    rumboot_printf("CHECK %s\n", ck_entry->comment);

    (ck_entry->api->wrw)(CONST_1, ck_entry->base);
    rumboot_printf("READED DATA = 0x%X\n", (ck_entry->api->rdw)(ck_entry->base));

    for(i = 0; i < 4; i++)
    {
        (ck_entry->api->wrb)(byte_tpl[i], ck_entry->base + i);
        data32 = (ck_entry->api->rdw)(ck_entry->base);
        rumboot_printf("writing byte: 0x%X, data = 0x%X\n", byte_tpl[i], data32);
        CMP_EQ(data32, long_tpl[i], t_result, s_result);
        rumboot_printf("%d: At 0x%X, Expected: 0x%X, Actual: 0x%X -> %s\n",
                i, ck_entry->base, long_tpl[i], data32,
                !t_result ? "ok" : "error");
        TEST_ASSERT(data32 == long_tpl[i], "READ ERROR");
    }

    rumboot_putstring("done.\n");

    rumboot_putstring("Inject error by h/w event...");
    // test_event(ev_code);

    rumboot_putstring("Check data\n");
    data32 = (ck_entry->api->rdw)(ck_entry->base);
    rumboot_printf("Expected = 0x%X, Actual = 0x%X\n",
            long_tpl[3], data32);
    CMP_EQ(data32, long_tpl[3], t_result, s_result);
    TEST_ASSERT(data32 == long_tpl[3], "ECC ERROR");
    rumboot_putstring("Success!\n");
    return s_result;
}

uint32_t main(void)
{
    uint32_t     test_result    = TEST_OK;
    ckinfo_t    *ckitem;

    rumboot_putstring("Init EMI...\n");
    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
    emi_set_ecc(SRAM0_BASE, emi_b0_sram0, emi_ecc_on);
    emi_set_ecc(SRAM1_BASE, emi_b4_sram1, emi_ecc_on);
    emi_set_ecc(NOR_BASE,   emi_b5_nor,   emi_ecc_on);
    rumboot_putstring("done.\n");

    for(ckitem = check_list; ckitem->api; ckitem++)
        test_result |= check_emi(ckitem);

    rumboot_putstring(!test_result ? "TEST OK!\n":"TEST_ERROR!\n");

    return test_result;
}

