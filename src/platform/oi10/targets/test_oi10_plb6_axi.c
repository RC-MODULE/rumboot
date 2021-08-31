#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>

#include <platform/interrupts.h>
#include <regs/fields/mpic128.h>
#include <platform/regs/sctl.h>
#include <devices/ugly/emi.h>
#include <devices/ugly/hscb.h>
#include <platform/test_assert.h>

#include <devices/ugly/greth.h>
#include <arch/common_macros.h>
#include <arch/ppc_476fp_mmu.h>

#include <arch/dma2plb6.h>
#include <devices/ugly/plb6mcif2.h>
#include <platform/devices/nor_1636RR4.h>

#ifndef DCR_EM2_AXIMCIF2_BASE
#define DCR_EM2_AXIMCIF2_BASE           0x800D0000
#endif

/* AXI-MCIF2 Bridge registers */
#define AXIMCIF2_BESR_RC                0x00    // Read/Clear
#define AXIMCIF2_BESR_RS                0x01    // Read/Set
#define AXIMCIF2_ERRID                  0x02    // Read
#define AXIMCIF2_AEARL                  0x03    // Read
#define AXIMCIF2_AEARU                  0x04    // Read
#define AXIMCIF2_AXIERRMSK              0x05    // Read/Write
#define AXIMCIF2_AXIASYNC               0x06    // Read/Write
#define AXIMCIF2_AXICFG                 0x07    // Read/Write
#define AXIMCIF2_AXISTS                 0x09    // Read
#define AXIMCIF2_MR0CF                  0x10    // Read/Write
#define AXIMCIF2_MR1CF                  0x11    // Read/Write
#define AXIMCIF2_MR2CF                  0x12    // Read/Write
#define AXIMCIF2_MR3CF                  0x13    // Read/Write
#define AXIMCIF2_RID                    0x20    // Read

#define GRETH_TEST_DATA_MISALIGN_IM0    0
#define GRETH_TEST_DATA_MISALIGN_IM1    0
#define GRETH_TEST_DATA_MISALIGN_IM2    0
#define GRETH_TEST_DATA_MISALIGN_EM2    0

#define GRETH_SRC_DATA_SEL              true
#define GRETH_DST_DATA_SEL              false
#define GRETH_TEST_DATA_LEN_BYTES       250
#define GRETH_EDCL_DATA_LEN_BYTES       16
#define GRETH_CKMODE_NORMAL             0
#define GRETH_CKMODE_RDFAIL             1
#define GRETH_CKMODE_WRFAIL             2
#define GRETH_CKMODES                   3
#define GRETH_RAM_PRESENT               (SRAM0_BASE + 0x00000000)
#define GRETH_RAM_ABSENT                (SRAM0_BASE + 0x08000000)
#define GRETH_FILL_SRC_EN_MASK          ((1 << GRETH_CKMODE_NORMAL)   | \
                                         (1 << GRETH_CKMODE_WRFAIL))
#define GRETH_TERR_BITS                 ((0 << GRETH_STATUS_TE)       | \
                                         (1 << GRETH_STATUS_TA))
#define GRETH_RERR_BITS                 ((0 << GRETH_STATUS_RE)       | \
                                         (1 << GRETH_STATUS_RA))

#define EDCL_TEST_ADDR_IM0              (IM0_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_IM1              (IM1_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_IM2              (IM2_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_EM2              (EM2_BANK3_BASE)

#define EVENT_CHECK_GRETH0_RX_ER        0x00001000
#define EVENT_CHECK_GRETH1_RX_ER        0x00001001
#define EVENT_CHECK_GRETH0_RX_COL       0x00001002
#define EVENT_CHECK_GRETH1_RX_COL       0x00001003
#define EVENT_CHECK_RUN_HPROT_MONITOR   0x00001004
#define EVENT_CHECK_STOP_HPROT_MONITOR  0x00001005

//setting non-default EDCLADDRL values
#define EDCLADDRL0_TEST                 0x0
#define EDCLADDRL1_TEST                 0x1
#define EDCLIP0_TEST                    (EDCLIP | EDCLADDRL0_TEST)
#define EDCLIP1_TEST                    (EDCLIP | EDCLADDRL1_TEST)

#define TST_MAC_MSB                     0x0000FFFF
#define TST_MAC_LSB                     0xFFFFFFFF

#define HCB_IRQ_F                       (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define ETH_IRQ_F                       (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)

#define TEST_DATA_SIZE                  128
#define TEST_DATA_CONTENT               0xFEDCBA9876543210ULL
#define TESTDATA_BASE                   (SRAM0_BASE + 0x1000)
#define HSCB_DESC_BASE                  (SRAM0_BASE + 0x0100)
#define PERIPH_TEST_DATA_LEN 250
#define PERIPH_TEST_DATA_LEN_BYTES      (PERIPH_TEST_DATA_LEN           \
                                            * sizeof(uint32_t))
#define SOURCE_DATA_ARR_SIZE            (PERIPH_TEST_DATA_LEN           \
                                            * sizeof(uint32_t)          \
                                            / sizeof(test_pattern_t))
#define PERIPH_TEST_PATTERN_0           0x01020304
#define PERIPH_TEST_PATTERN_1           0x05060708
#define PERIPH_TEST_PATTERN_2           0x09101112
#define PERIPH_TEST_PATTERN_3           0x13141516
#define PERIPH_TEST_PATTERN_4           0x17181920
#define PERIPH_TEST_PATTERN_5           0x21222324
#define PERIPH_TEST_PATTERN_6           0x25262728
#define PERIPH_TEST_PATTERN_7           0x29303132

#define RAMPAGE(T,B,P,I)                ((T*)((B) + ((P) * (I))))
#define TD_PAGE(P,I)                    RAMPAGE(uint32_t,TESTDATA_BASE,P,I)
#define HSCB_DESC_PAGE(P,I)             RAMPAGE(uint32_t,HSCB_DESC_BASE,P,I)
#define PRETTY_HEXDUMP(V,S)             hexDump(#V,(void*)V,S)
#define PRETTY_PRINT_VAR(V)             rumboot_printf("%s = 0x%X\n",   \
                                            (#V), (uint32_t)V)
#define PRETTY_PRINT_DCR(B,R)           rumboot_printf("%s = 0x%X\n",   \
                                            (#R), (uint32_t)            \
                                            dcr_read((B) + (R)))
#define GET_NIBBLE(V,N)                 (((V) >> ((N) * 4)) & 0x0F)
#define IS_NOT_PRINTABLE(C)             (((C) < 0x20) || ((C) > 0x7E))

#define __ALIGNED(ALIGN)                __attribute__ ((aligned (ALIGN)))
#define __SECTION(NAME)                 __attribute__ ((section (NAME)))
#define __NORETURN                      __attribute__ ((noreturn))
#define __NOINLINE                      __attribute__ ((noinline))
#define __ALWAYS_INLINE                 __attribute__ ((always_inline))
#define __USED                          __attribute__ ((used))

//initial data in IM0 (will be copy to source ETH areas)
struct test_pattern_t
{
    uint32_t word0[4];
    uint32_t word1[4];
};
typedef struct test_pattern_t test_pattern_t;

struct greth_instance
{
    uint32_t    base_addr;
    volatile
    uint32_t   *irq_handled;
};
typedef struct greth_instance greth_instance;

uint32_t  edcl_seq_number   = 0;
uint8_t  *edcl_test_data_wr = NULL;

greth_descr_t*  tx_descriptor_data;
greth_descr_t*  rx_descriptor_data;

#ifdef CHECK_AXI_PLB6_SINGLE
struct pu8_src_dst_pair_t
{
    uint8_t *src;
    uint8_t *dst;
};
typedef struct pu8_src_dst_pair_t pu8_src_dst_pair_t;

static volatile
uint32_t GRETH0_IRQ_HANDLED  = 0,
         GRETH1_IRQ_HANDLED  = 0,
         GRETH0_STATUS       = 0,
         GRETH1_STATUS       = 0;

greth_mac_t tst_greth_mac    = {TST_MAC_MSB, TST_MAC_LSB};

static greth_instance in[]   =
{
        {
                .base_addr   = GRETH_0_BASE,
                .irq_handled = &GRETH0_IRQ_HANDLED
        },
        {
                .base_addr   = GRETH_1_BASE,
                .irq_handled = &GRETH1_IRQ_HANDLED
        }
};

static pu8_src_dst_pair_t greth_test_info[GRETH_CKMODES] =
{
        {
                .src = RAMPAGE(uint8_t, GRETH_RAM_PRESENT, 0x400, 1),
                .dst = RAMPAGE(uint8_t, GRETH_RAM_PRESENT, 0x400, 2)
        },
        {
                .src = RAMPAGE(uint8_t, GRETH_RAM_ABSENT,  0x400, 1),
                .dst = RAMPAGE(uint8_t, GRETH_RAM_PRESENT, 0x400, 3)
        },
        {
                .src = RAMPAGE(uint8_t, GRETH_RAM_PRESENT, 0x400, 4),
                .dst = RAMPAGE(uint8_t, GRETH_RAM_ABSENT,  0x400, 2)
        }
};

#endif

#ifdef CHECK_AXI_PLB6_BURST
static test_pattern_t test_hscbx_src_data[SOURCE_DATA_ARR_SIZE] =
    {[0 ... SOURCE_DATA_ARR_SIZE - 1] =
        {
                .word0 = {PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1,
                          PERIPH_TEST_PATTERN_2, PERIPH_TEST_PATTERN_3},
                .word1 = {PERIPH_TEST_PATTERN_4, PERIPH_TEST_PATTERN_5,
                          PERIPH_TEST_PATTERN_6, PERIPH_TEST_PATTERN_7}
        }
    };

static hscb_instance_t  hscb_cfg[4];
static volatile bool hscb_handler[4]  = {false, false, false, false};

static uint32_t *test_hscb0_data_src  = TD_PAGE(0x400, 1),
                *test_hscb0_data_dst  = TD_PAGE(0x400, 2),
                *test_hscb1_data_src  = TD_PAGE(0x400, 3),
                *test_hscb1_data_dst  = TD_PAGE(0x400, 4),
                *test_hscb2_data_src  = TD_PAGE(0x400, 5),
                *test_hscb2_data_dst  = TD_PAGE(0x400, 6),
                *test_hscb3_data_src  = TD_PAGE(0x400, 7),
                *test_hscb3_data_dst  = TD_PAGE(0x400, 8),

                *HSCB0_TX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 1),
                *HSCB0_RX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 2),
                *HSCB1_TX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 3),
                *HSCB1_RX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 4),

                *HSCB2_TX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 5),
                *HSCB2_RX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 6),
                *HSCB3_TX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 7),
                *HSCB3_RX_DESCR_ADDR  = HSCB_DESC_PAGE(0x80, 8);

#endif

#ifdef CHECK_AXI_PLB6_SINGLE
static void handler_eth(int irq, void *arg)
{
    uint32_t        cur_status  = 0,
                    greth_idx   = 0;
    volatile
    uint32_t       *pstatus     = &GRETH0_STATUS;
    greth_instance *gr_inst     = (greth_instance*)arg;
    static const
    uint32_t        mask        =
            ((1U << GRETH_STATUS_IA)    |
             (1U << GRETH_STATUS_TS)    |
             (1U << GRETH_STATUS_TA)    |
             (1U << GRETH_STATUS_RA)    |
             (1U << GRETH_STATUS_TI)    |
             (1U << GRETH_STATUS_TE)    |
             (1U << GRETH_STATUS_RI)    |
             (1U << GRETH_STATUS_RE));

    greth_idx = GET_GRETH_IDX(gr_inst->base_addr);
    if(greth_idx & 0x01) pstatus = &GRETH1_STATUS;
    rumboot_printf("GRETH%d(0x%X) IRQ arrived  \n",
            greth_idx, gr_inst->base_addr);

    *pstatus = cur_status = greth_get_status(gr_inst->base_addr);

    if(cur_status & (1 << GRETH_STATUS_RI))
    {
        *(gr_inst->irq_handled) = 1;
        rumboot_printf("Receive interrupt (0x%x)\n", cur_status);
    }

    if(cur_status & (1 << GRETH_STATUS_RE))
    {
        *(gr_inst->irq_handled) = 3;
        rumboot_printf("Receive Error interrupt (0x%x)\n", cur_status);
    }

    if(!(cur_status & (1 << GRETH_STATUS_RI)) && !(cur_status & (1 << GRETH_STATUS_RE)))
    {
        *(gr_inst->irq_handled) = 2;
        rumboot_printf("Unexpected status (0x%x)\n", cur_status);
    }

    greth_clear_status_bits(gr_inst->base_addr, mask);
    rumboot_printf("Exit from handler\n");
}

static bool _greth_wait_transmit(uint32_t base_addr)
{
    uint32_t    t           = 0;
    uint32_t    cur_status  = 0;
    uint32_t    greth_idx   = 0;
    volatile
    uint32_t   *pstatus     = &GRETH0_STATUS;

    static const
    uint32_t    mask        = ((1 << GRETH_STATUS_IA) |
                               (1 << GRETH_STATUS_TS) |
                               (1 << GRETH_STATUS_TA) |
                               (1 << GRETH_STATUS_RA) |
                               (1 << GRETH_STATUS_TI) |
                               (1 << GRETH_STATUS_TE) |
                               (1 << GRETH_STATUS_RE));

    rumboot_printf("Waiting transmit..\n");

    greth_idx = GET_GRETH_IDX(base_addr);
    if(greth_idx & 0x01) pstatus = &GRETH1_STATUS;

    do {cur_status = ioread32(base_addr + STATUS);}
        while(!(cur_status & mask) && (t++ < GRETH_TIMEOUT));

    *pstatus = cur_status;

    if((t == GRETH_TIMEOUT) || !(cur_status & (1 << GRETH_STATUS_TI)))
    {
        rumboot_printf("Transmit is timed out (0x%X)\n", cur_status);
        greth_clear_status_bits(base_addr, mask);
        return false;
    } else
    {
        rumboot_printf("Transmit is OK! (0x%X)\n", cur_status);
        greth_clear_status_bits(base_addr, (1 << GRETH_STATUS_TI));
        return true;
    }
}
#endif

#ifdef CHECK_AXI_PLB6_BURST
static void handler_hscb(int irq, void *arg)
{
    uint32_t adma_ch_status_src;
    hscb_instance_t* hscb_inst = (hscb_instance_t*)arg;
    uint32_t idx;
    adma_ch_status_src = hscb_get_adma_ch_status(hscb_inst->src_hscb_base_addr);
    idx = GET_HSCB_IDX_BY_ADDR(hscb_inst->src_hscb_base_addr);
    rumboot_printf("HSCB%d(0x%X) ADMA_IRQ status: 0x%X\n",
            idx, hscb_inst->src_hscb_base_addr, adma_ch_status_src);
    if(adma_ch_status_src & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
    {
        rumboot_printf("Detected WDMA_IRQ status: 0x%X\n",
                hscb_get_wdma_status(hscb_inst->src_hscb_base_addr));
    } else
    {
        TEST_ASSERT(0, "Unexpected IRQ status");
    }

    hscb_handler[idx] = true;
}

void init_hscb_cfg(hscb_instance_t *hscb_inst)
{
    rumboot_printf("[%s]: Init hscb instances...\n", __FUNCTION__);
    hscb_inst[0].src_hscb_base_addr   = HSCB0_BASE;
    hscb_inst[0].dst_hscb_base_addr   = HSCB1_BASE;
    hscb_inst[0].src_addr             = test_hscb0_data_src;
    hscb_inst[0].dst_addr             = test_hscb0_data_dst;
    hscb_inst[0].src_size             = sizeof(test_hscbx_src_data);
    hscb_inst[0].dst_size             = sizeof(test_hscbx_src_data);
    hscb_inst[0].tx_descr_addr        = (uint32_t)HSCB0_TX_DESCR_ADDR;
    hscb_inst[0].rx_descr_addr        = (uint32_t)HSCB0_RX_DESCR_ADDR;

    hscb_inst[1].src_hscb_base_addr   = HSCB1_BASE;
    hscb_inst[1].dst_hscb_base_addr   = HSCB0_BASE;
    hscb_inst[1].src_addr             = test_hscb1_data_src;
    hscb_inst[1].dst_addr             = test_hscb1_data_dst;
    hscb_inst[1].src_size             = sizeof(test_hscbx_src_data);
    hscb_inst[1].dst_size             = sizeof(test_hscbx_src_data);
    hscb_inst[1].tx_descr_addr        = (uint32_t)HSCB1_TX_DESCR_ADDR;
    hscb_inst[1].rx_descr_addr        = (uint32_t)HSCB1_RX_DESCR_ADDR;

    hscb_inst[2].src_hscb_base_addr   = HSCB2_BASE;
    hscb_inst[2].dst_hscb_base_addr   = HSCB3_BASE;
    hscb_inst[2].src_addr             = test_hscb2_data_src;
    hscb_inst[2].dst_addr             = test_hscb2_data_dst;
    hscb_inst[2].src_size             = sizeof(test_hscbx_src_data);
    hscb_inst[2].dst_size             = sizeof(test_hscbx_src_data);
    hscb_inst[2].tx_descr_addr        = (uint32_t)HSCB2_TX_DESCR_ADDR;
    hscb_inst[2].rx_descr_addr        = (uint32_t)HSCB2_RX_DESCR_ADDR;

    hscb_inst[3].src_hscb_base_addr   = HSCB3_BASE;
    hscb_inst[3].dst_hscb_base_addr   = HSCB2_BASE;
    hscb_inst[3].src_addr             = test_hscb3_data_src;
    hscb_inst[3].dst_addr             = test_hscb3_data_dst;
    hscb_inst[3].src_size             = sizeof(test_hscbx_src_data);
    hscb_inst[3].dst_size             = sizeof(test_hscbx_src_data);
    hscb_inst[3].tx_descr_addr        = (uint32_t)HSCB3_TX_DESCR_ADDR;
    hscb_inst[3].rx_descr_addr        = (uint32_t)HSCB3_RX_DESCR_ADDR;
}

struct
rumboot_irq_entry *create_hscb_irq_handlers(hscb_instance_t* hscb_inst)
{
    int i, il[4] = {SW0_AXI_INT, SW1_AXI_INT, SW2_AXI_INT, SW3_AXI_INT};

    rumboot_printf("Create hscb irq handlers...\n");
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);


    init_hscb_cfg(hscb_inst);

    for(i = 0; i < 4; i++)
        rumboot_irq_set_handler(tbl, il[i], HCB_IRQ_F,
                handler_hscb, hscb_inst + i);

    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    for(i = 0; i < 4; i++)
        rumboot_irq_enable(il[i]);

    rumboot_irq_sei();

    return tbl;
}
#endif

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

#ifdef CHECK_AXI_PLB6_SINGLE
struct rumboot_irq_entry *create_greth01_irq_handlers()
{
    rumboot_printf("Enable GRETH irqs\n");
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

    GRETH0_IRQ_HANDLED = 0;
    GRETH1_IRQ_HANDLED = 0;

    rumboot_irq_set_handler(tbl, ETH0_INT, ETH_IRQ_F, handler_eth, in + 0);
    rumboot_irq_set_handler(tbl, ETH1_INT, ETH_IRQ_F, handler_eth, in + 1);

    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(ETH0_INT);
    rumboot_irq_enable(ETH1_INT);
    rumboot_irq_sei();

    return tbl;
}

void prepare_test_data_greth(pu8_src_dst_pair_t *inf,
                             bool                need_alloc,
                             bool                need_fill)
{
    uint32_t    i     = 0;
    uint8_t    *psrc  = inf->src;

    rumboot_putstring("Preparing data...");

    if(!tx_descriptor_data)
    {
        tx_descriptor_data = (greth_descr_t*)
            rumboot_malloc_from_heap_aligned(
            1, 3 * sizeof(greth_descr_t), 1024);
        rumboot_printf("%s memory for %s descriptor: 0x%X\n",
                "Allocated", "tx", (uint32_t)tx_descriptor_data);
    } else
    {
        rumboot_printf("%s memory for %s descriptor: 0x%X\n",
                "Reused", "tx", (uint32_t)tx_descriptor_data);
    }

    if(!rx_descriptor_data)
    {
        rx_descriptor_data = (greth_descr_t*)
            rumboot_malloc_from_heap_aligned(
            1, 3 * sizeof(greth_descr_t), 1024);
        rumboot_printf("%s memory for %s descriptor: 0x%X\n",
                "Allocated", "rx", (uint32_t)rx_descriptor_data);
    } else
    {
        rumboot_printf("%s memory for %s descriptor: 0x%X\n",
                "Used", "rx", (uint32_t)rx_descriptor_data);
    }

    if(need_alloc)
    {
        inf->src = (uint8_t*)
                rumboot_malloc_from_heap_misaligned(
                    0, GRETH_TEST_DATA_LEN_BYTES, 16,
                    GRETH_TEST_DATA_MISALIGN_IM0);
        rumboot_printf("Allocated from im0 for src (0x%X)\n",
                (uint32_t)inf->src);
        inf->dst = (uint8_t*)
                rumboot_malloc_from_heap_misaligned(
                    0, GRETH_TEST_DATA_LEN_BYTES, 16,
                    GRETH_TEST_DATA_MISALIGN_IM0);
        rumboot_printf("Allocated from im0 for dst (0x%X)\n",
                (uint32_t)inf->src);
    } else
    {
        rumboot_printf(
                "Skip src and dst allocation "
                "and use predefined addresses\n");
    }

    if(need_fill)
    {
        rumboot_putstring("Fill source array...\n");
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);

        psrc[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);

        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
        psrc[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);

        psrc[i++] = (uint8_t)
                (((GRETH_TEST_DATA_LEN_BYTES - 14) & 0x00FF) >> 0);
        psrc[i++] = (uint8_t)
                (((GRETH_TEST_DATA_LEN_BYTES - 14) & 0xFF00) >> 8);
        psrc[i++] = (uint8_t)(0);
        psrc[i++] = (uint8_t)(0);

        while(i < GRETH_TEST_DATA_LEN_BYTES)
        {
            psrc[i] = i;
            i++;
        }
    }

    rumboot_putstring("Preparing data finished");
}
#endif

#ifdef CHECK_AXI_PLB6_BURST
void prepare_test_data_hscb(void *psrc_data, size_t dsz)
{
    rumboot_putstring("Preparing data...");

    PRETTY_PRINT_VAR(test_hscb0_data_src);
    PRETTY_PRINT_VAR(test_hscb0_data_dst);
    PRETTY_PRINT_VAR(test_hscb1_data_src);
    PRETTY_PRINT_VAR(test_hscb1_data_dst);
    PRETTY_PRINT_VAR(test_hscb2_data_src);
    PRETTY_PRINT_VAR(test_hscb2_data_dst);
    PRETTY_PRINT_VAR(test_hscb3_data_src);
    PRETTY_PRINT_VAR(test_hscb3_data_dst);

    memcpy((void*)test_hscb0_data_src, psrc_data, dsz);
    memcpy((void*)test_hscb1_data_src, psrc_data, dsz);
    memcpy((void*)test_hscb2_data_src, psrc_data, dsz);
    memcpy((void*)test_hscb3_data_src, psrc_data, dsz);

    rumboot_putstring("Preparing data finished");
}
#endif

#ifdef CHECK_AXI_PLB6_SINGLE

#define TE_MSG     "Transmit error mismatch!"
#define RE_MSG     "Receive error mismatch!"

void check_transfer_via_ext_loop(uint32_t   base_addr_src_eth,
                                  uint8_t  *ptest_data_src,
                                  uint8_t  *ptest_data_dst,
                                  int       ckmode)
{
    volatile
    uint32_t    *eth_handled_flag_ptr   = NULL,
                *psrc_status            = NULL,
                *pdst_status            = NULL;
    uint32_t     base_addr_dst_eth      = 0x00000000;

    TEST_ASSERT(
            (base_addr_src_eth == GRETH_1_BASE) ||
            (base_addr_src_eth == GRETH_0_BASE),
                "Wrong GRETH base address\n");

    base_addr_dst_eth = (base_addr_src_eth == GRETH_0_BASE)
            ? GRETH_1_BASE : GRETH_0_BASE;

    if (base_addr_src_eth == GRETH_1_BASE)
    {
        base_addr_dst_eth    = GRETH_0_BASE;
        psrc_status          = &GRETH1_STATUS;
        pdst_status          = &GRETH0_STATUS;
        eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
        GRETH0_IRQ_HANDLED   = 0;
    } else
    {
        base_addr_dst_eth    = GRETH_1_BASE;
        psrc_status          = &GRETH0_STATUS;
        pdst_status          = &GRETH1_STATUS;
        eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;
        GRETH1_IRQ_HANDLED   = 0;
    }

    rumboot_printf("\n[%s] Checking transfer from 0x%X to 0x%x\n",
            __FUNCTION__,
            (uint32_t)ptest_data_src, (uint32_t)ptest_data_dst);

    if(ckmode != GRETH_CKMODE_NORMAL)
    {
        rumboot_putstring("Resetting GRETH hardware...\n");
        TEST_ASSERT(greth_reset(base_addr_src_eth),
                "Source GRETH reset fails!");
        TEST_ASSERT(greth_reset(base_addr_dst_eth),
                "Destination GRETH reset fails!");
    }

    greth_configure_for_receive(base_addr_dst_eth,
            ptest_data_dst,
            GRETH_TEST_DATA_LEN_BYTES,
            rx_descriptor_data,
            &tst_greth_mac);
    greth_configure_for_transmit(base_addr_src_eth,
            ptest_data_src,
            GRETH_TEST_DATA_LEN_BYTES,
            tx_descriptor_data,
            &tst_greth_mac);

    rumboot_printf("GRETH %s descriptor is at 0x%X\n", "RX",
            ioread32(base_addr_dst_eth + RECEIVER_DESCR_PTR));
    rumboot_printf("GRETH %s descriptor is at 0x%X\n", "TX",
            ioread32(base_addr_src_eth + TRANSMIT_DESCR_PTR));
    rumboot_printf("GRETH %s descriptor "
            "start address=0x%X, "
            "options=0x%X\n", "RX",
            rx_descriptor_data->start_addr,
            rx_descriptor_data->options);
    rumboot_printf("GRETH %s descriptor "
            "start address=0x%X, "
            "options=0x%X\n", "TX",
            tx_descriptor_data->start_addr,
            tx_descriptor_data->options);

    greth_start_receive(base_addr_dst_eth, true);
    greth_start_transmit(base_addr_src_eth);

    switch(ckmode)
    {
        case GRETH_CKMODE_NORMAL:
            TEST_ASSERT(_greth_wait_transmit(base_addr_src_eth),
                    "GRETH transmition error!\n");
            TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth,
                    eth_handled_flag_ptr), "Receiving IRQ is failed\n");
            TEST_ASSERT(!memcmp(ptest_data_src, ptest_data_dst,
                    GRETH_TEST_DATA_LEN_BYTES), "Data compare error!\n");
            memset(ptest_data_dst, 0, GRETH_TEST_DATA_LEN_BYTES);
            break;
        case GRETH_CKMODE_RDFAIL:
            _greth_wait_transmit(base_addr_src_eth);
            greth_wait_receive_irq(base_addr_dst_eth, eth_handled_flag_ptr);
            TEST_ASSERT(*psrc_status & GRETH_TERR_BITS, TE_MSG);
            memset(ptest_data_dst, 0, GRETH_TEST_DATA_LEN_BYTES);
            break;
        case GRETH_CKMODE_WRFAIL:
            _greth_wait_transmit(base_addr_src_eth);
            greth_wait_receive_irq(base_addr_dst_eth, eth_handled_flag_ptr);
            // Read status explicitly, because IRQ handler
            // not called. when receive fails.
            *pdst_status = greth_get_status(base_addr_dst_eth);
            TEST_ASSERT(*pdst_status & GRETH_RERR_BITS, RE_MSG);
            break;
        default:
            TEST_ASSERT(true, "Unexpected check mode!\n");
            break;
    }

    rumboot_printf("GRETH %s STATUS: 0x%X\n", "SRC", *psrc_status);
    rumboot_printf("GRETH %s STATUS: 0x%X\n", "DST", *pdst_status);

    rumboot_free(tx_descriptor_data);
    rumboot_free(rx_descriptor_data);
    //test_event(EVENT_CHECK_STOP_HPROT_MONITOR);
}

void test_oi10_greth(void)
{
	struct
	rumboot_irq_entry   *tbl;
    pu8_src_dst_pair_t   dma;
    uint32_t             oldMR0CF;
    int                  i;
    static const
    uint32_t             newMR0CF   =       // Set Rank0 base addr and size
                      ( (0      << 20)      // R_RA[31:20] Base address
                      | (0b0000 << 16)      // R_RS[19:16] Set Rank0 size
                      | (0      <<  1)      // R_RO[15:1] AXI RANK Offset
                      | (1      <<  0));    // R_RV[0] Enable Rank0
    static const char   *cmn[]      =
    {
            "NORMAL",
            "READ FAIL",
            "WRITE FAIL",
            "READ AND WRITE FAIL"
    };

    PRETTY_PRINT_DCR(DCR_EM2_AXIMCIF2_BASE, AXIMCIF2_MR0CF);
    rumboot_printf("Start test_oi10_greth. Transmit/receive checks\n");
    //test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();

    oldMR0CF = dcr_read(DCR_EM2_AXIMCIF2_BASE + AXIMCIF2_MR0CF);
    tx_descriptor_data = NULL;
    rx_descriptor_data = NULL;

    for(i = 0; i < GRETH_CKMODES; i++)
    {
        rumboot_printf("----------------------------------------\n");
        dma.src = (uint8_t*)rumboot_virt_to_dma(greth_test_info[i].src);
        dma.dst = (uint8_t*)rumboot_virt_to_dma(greth_test_info[i].dst);
        rumboot_printf(
                " --- Check transfer mode #%d (%s): "
                "0x%X(0x%X) -> 0x%X(0x%X)...\n", i, cmn[i],
                (uint32_t)greth_test_info[i].src, (uint32_t)dma.src,
                (uint32_t)greth_test_info[i].dst, (uint32_t)dma.dst);
        prepare_test_data_greth(greth_test_info + i, false,
                !!((1 << i) & GRETH_FILL_SRC_EN_MASK));
        if(i != GRETH_CKMODE_NORMAL)
        {
            dcr_write(DCR_EM2_AXIMCIF2_BASE + AXIMCIF2_MR0CF, newMR0CF);
            rumboot_printf("MR%dCF: OLD=0x%X, NEW=0x%X\n", 0,
                    oldMR0CF, newMR0CF);
        }

        check_transfer_via_ext_loop(GRETH_0_BASE, dma.src, dma.dst, i);

        if(i != GRETH_CKMODE_NORMAL)
            dcr_write(DCR_EM2_AXIMCIF2_BASE + AXIMCIF2_MR0CF, oldMR0CF);
    }

    delete_irq_handlers(tbl);
}
#endif

//---------------------------------------------------------------------------



#ifdef CHECK_PLB6_AXI_SINGLE
struct regpoker_checker greth_check_array[] =
{
    {"CTRL              ", REGPOKER_READ32,  CTRL              , 0x9A000090,  0xFE007CFF},
    {"MDIO_CTRL         ", REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140,  0xFFFFFFCF},
    {"TRANSMIT_DESCR_PTR", REGPOKER_READ8,   TRANSMIT_DESCR_PTR, 0x00000000,  0x3F8},
    {"RECEIVER_DESCR_PTR", REGPOKER_READ8,   RECEIVER_DESCR_PTR, 0x00000000,  0x3F8},
    {"EDCL_IP           ", REGPOKER_READ32,  EDCL_IP           , EDCLIP,      ~0},
    {"EDCL_MAC_MSB      ", REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xFFFF},
    {"EDCL_MAC_LSB      ", REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0},
    {"EDCL_IP           ", REGPOKER_WRITE32, EDCL_IP           , 0x00000000,  ~0},
    {"EDCL_MAC_MSB      ", REGPOKER_WRITE32, EDCL_MAC_MSB      , 0x00000000,  0xFFFF},
    {"EDCL_MAC_LSB      ", REGPOKER_WRITE32, EDCL_MAC_LSB      , 0x00000000,  ~0}
};

void regs_check(uint32_t base_addr)
{

    if (base_addr == GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n",
                1, base_addr);
        greth_check_array[4].expected  = EDCLIP1;
        greth_check_array[6].expected |= EDCLADDRL1;
    } else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n",
                0, base_addr);
        greth_check_array[4].expected  = EDCLIP0;
        greth_check_array[6].expected |= EDCLADDRL0;
    }

    TEST_ASSERT(!(rumboot_regpoker_check_array(
            greth_check_array, base_addr)),
            "Failed to check GRETH registers\n");
}
#endif

#ifdef CHECK_PLB6_AXI_BURST
static void fill(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < (size_in_bytes>>3); i++)
        s[i] = pattern;
}


static void fill_incremental(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{

    uint32_t i;
    for (i = 0; i < (size_in_bytes>>3); i++)
        s[i] = (pattern+i);
}

static uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
{
    uint32_t ret;

    switch(transfer_width_code)
    {
        case tr_width_byte:
            ret = 1;
            break;
        case tr_width_halfword:
            ret = 2;
            break;
        case tr_width_word:
            ret = 4;
            break;
        case tr_width_doubleword:
            ret =  8;
            break;
        case tr_width_quadword:
            ret = 16;
            break;
        default:
            ret = 0;
            break;
    }

    return ret;
}

static uint64_t fill_word = TEST_DATA_CONTENT;

static uint32_t compare(uint32_t s_addr, uint32_t d_addr,
                        uint32_t size_in_bytes)
{
    int i;
    volatile uint64_t temp0, temp1;
    for (i = 0; i < size_in_bytes; i += 8)
    {

        rumboot_printf("offset: 0x%X\n", i);

        temp0 = ioread64(s_addr + i);
        temp1 = ioread64(d_addr + i);

        //msync();

        if(temp0 != temp1)
        {
            rumboot_printf("Compare failed, expected value: 0x%X:0x%X\n",
                    (uint32_t)(temp0 >> 32), (uint32_t)(temp0));
            rumboot_printf("Read value: 0x%X:0x%X\n",
                    (uint32_t)(temp1 >> 32), (uint32_t)(temp1));
            return false;
        }
    }
    return true;
}

uint32_t
check_dma2plb6_0_mem_to_mem(uint32_t source_ea, uint32_t dest_ea,
                            uint64_t source_phys, uint64_t dest_phys)
{
    struct dma2plb6_setup_info dma_info;
    struct channel_status status = {};
    dma_info.base_addr = DCR_DMAPLB6_BASE;
    dma_info.source_adr = source_phys;
    dma_info.dest_adr = dest_phys;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    for(dma_info.rw_transfer_size = rw_tr_size_1q;
        dma_info.rw_transfer_size <= rw_tr_size_8q;
        dma_info.rw_transfer_size++)
    {
        rumboot_printf("rw_transfer_size = 0x%x\n",
                dma_info.rw_transfer_size);
        for(dma_info.transfer_width = tr_width_byte;
            dma_info.transfer_width <= tr_width_quadword;
            dma_info.transfer_width++)
        {
            rumboot_printf("transfer_width = 0x%x\n",
                    dma_info.transfer_width);

            rumboot_printf("fill source area\n");
            fill_incremental((uint64_t *)source_ea,
                    fill_word, TEST_DATA_SIZE);

            rumboot_printf("fill dest area\n");
            fill((uint64_t *)dest_ea, 0x0, TEST_DATA_SIZE);

            dma_info.count = TEST_DATA_SIZE / dma2plb6_get_bytesize(
                    dma_info.transfer_width);

            rumboot_printf("run dma copy\n");
            if(!dma2plb6_single_copy(&dma_info, &status))
            {
                dma2plb6_trace_error_status(&status);
                return false;
            } else
            {
                rumboot_printf("run compare data\n");
                TEST_ASSERT(compare(source_ea, dest_ea, TEST_DATA_SIZE),
                        "Memory comparison failed");
                fill_word = ~fill_word;
                rumboot_printf("done: tr_width = 0x%x\n",
                        dma_info.transfer_width);
            }
        }
    }
    return true;
}
#endif

#ifdef CHECK_PLB6_AXI_SINGLE
static void byte_plb_axi_test(uint32_t base_addr)
{
    uint32_t i      = 0;
    uint8_t  temp   = 0;

    do
    {
        iowrite8(i, base_addr + i);
        i++;
    } while(i != 0x100);

    i=0;

    do
    {
        temp = ioread8(base_addr + i);
        if (i != temp)
        {
            rumboot_printf("[%s] Memory comparison failed "
                    "exp 0x%x act 0x%x \n", __FUNCTION__,
                    i, temp);
            TEST_ASSERT(0, "Oops!");
        }
        i++;
    } while (i != 0x100);
}

static void hword_plb_axi_test(uint32_t base_addr, uint32_t offt)
{
    uint32_t i      = 0;
    uint16_t tdata  = 0x01;
    uint16_t temp   = 0;

    do
    {
        /*
        rumboot_printf("[%s] "
                "write data 0x%x to addr 0x%x \n", __FUNCTION__,
                tdata, base_addr + i + offt); */
        iowrite16(tdata,base_addr + i + offt);
        tdata <<= 1;
        i += 2;
    } while(tdata);

    i = 0;
    tdata = 0x01;

    do
    {
        temp = ioread16(base_addr + i + offt);
        /*
        rumboot_printf("[%s] "
                "read data 0x%x in addr 0x%x \n", __FUNCTION__,
                temp, base_addr + i + offt); */
        if(tdata != temp)
        {
            rumboot_printf("[%s] "
                    "Memory comparison failed exp 0x%x act 0x%x \n",
                    __FUNCTION__, tdata, temp);
            TEST_ASSERT(0, "Oops!");
        }

        tdata <<= 1;
        i += 2;
    } while(tdata);
}

static void word_plb_axi_test(uint32_t base_addr, uint32_t offt)
{
    uint32_t i      = 0;
    uint32_t tdata  = 0x01;
    uint32_t temp   = 0;

    do
    {
        /*
        rumboot_printf("[%s] "
                "write data 0x%x to addr 0x%x \n", __FUNCTION__,
                tdata, base_addr + i + offt); */
        iowrite32(tdata, base_addr + i + offt);
        tdata <<= 1;
        i += 4;
    } while(tdata);

    i = 0;
    tdata = 0x01;

    do
    {
        temp = ioread32(base_addr + i + offt);
        /*
        rumboot_printf("[%s] "
                "read data 0x%x in addr 0x%x \n", __FUNCTION__,
                temp, base_addr + i + offc); */
        if(tdata != temp)
        {
            rumboot_printf("[%s] Memory comparison failed "
                    "exp 0x%x act 0x%x \n",
                    __FUNCTION__, tdata, temp);
            TEST_ASSERT(0, "Oops!");
        }

        tdata <<= 1;
        i += 4;
    } while(tdata);
}

static void dword_plb_axi_test(uint32_t base_addr, uint32_t offt)
{
    uint32_t i      = 0;
    uint64_t tdata  = 0x01;
    uint64_t temp   = 0;

    do
    {
        /*
        rumboot_printf("[%s] write data 0x%x%x to addr 0x%x \n",
                __FUNCTION__,
                (uint32_t)(tdata >> 32),
                (uint32_t)(tdata & 0xFFFFFFFF),
                base_addr + i + offt); */
        iowrite64(tdata, base_addr + i + offt);
        tdata <<= 1;
        i += 8;
    } while(tdata);

    i = 0;
    tdata = 0x01;
    do
    {
        temp = ioread64(base_addr + i + offt);
        /*
        rumboot_printf("[%s] "
                "read data 0x%x%x in addr 0x%x \n",
                __FUNCTION__,
                (uint32_t)(temp >> 32),
                (uint32_t)(temp & 0xFFFFFFFF),
                base_addr + i + offt); */
        if (tdata != temp)
        {
            rumboot_printf("[%s] "
                    "Memory comparison failed exp 0x%x act 0x%x \n",
                    __FUNCTION__,
                    (uint32_t)(tdata >> 32),
                    (uint32_t)(tdata & 0xFFFFFFFF),
                    (uint32_t)(temp  >> 32),
                    (uint32_t)(temp  & 0xFFFFFFFF));
            TEST_ASSERT(0, "Oops!");
        }

        tdata <<= 1;
        i += 8;
    } while(tdata);
}

void single_plb6_axi_test(uint32_t base_addr)
{
    uint32_t i;

    byte_plb_axi_test(base_addr);

    for(i = 0; i < 8; i++)
        hword_plb_axi_test(base_addr, i);

    for(i = 0; i < 8; i++)
        word_plb_axi_test(base_addr, i);

    for(i = 0; i < 8; i++)
        dword_plb_axi_test(base_addr, i);
}
#endif

#ifdef CHECK_AXI_PLB6_BURST
void run_hscb_transfers_via_ext_loop(hscb_instance_t *hscb_inst)
{
    uint32_t i;

    for(i = 0; i < 4; i++)
    {
        hscb_run_wdma(hscb_inst[i].src_hscb_base_addr); // run wdma
        hscb_run_rdma(hscb_inst[i].src_hscb_base_addr); // run rdma
    }
}

void configure_hscb(hscb_instance_t     *hscb_inst,
                    hscb_axi_arwlen_t    hscb_axi_arwlen,
                    hscb_axi_arwburst_t  hscb_axi_arwburst)
{
    int i;
    hscb_axi_params_cfg_t axi_params_cfg;

    rumboot_printf("\n---\nApply software reset HSCBs\n");
    for(i = 0; i < 4; i++)
    {
        hscb_sw_rst(hscb_inst[i].src_hscb_base_addr);
        hscb_adma_sw_rst(hscb_inst[i].src_hscb_base_addr);
    }

    // configure
    for(i = 0; i < 4; i++)
    {
        rumboot_printf("%s HSCB%d...\n", "Configure", i);
        hscb_config_for_receive_and_transmit(hscb_inst + i);
        hscb_get_axi_params(hscb_inst[i].src_hscb_base_addr,
                &axi_params_cfg);
        axi_params_cfg.arlen   = hscb_axi_arwlen;
        axi_params_cfg.awlen   = hscb_axi_arwlen;
        axi_params_cfg.arburst = hscb_axi_arwburst;
        axi_params_cfg.awburst = hscb_axi_arwburst;
        hscb_set_axi_params(hscb_inst[i].src_hscb_base_addr,
                &axi_params_cfg);
    }

    // enable
    for(i = 0; i < 4; i++)
    {
        rumboot_printf("%s HSCB%d...\n", "Enable", i);
        hscb_enable(hscb_inst[i].src_hscb_base_addr);
    }

    // wait status
    for(i = 0; i < 4; i++)
    {
        rumboot_printf("%s HSCB%d...\n", "Wait", i);
        TEST_ASSERT(hscb_wait_status(hscb_inst[i].src_hscb_base_addr,
                HSCB_STATUS_ACTIVE_LINK_mask),
                "Failed to waiting ACTIVE_LINK on HSCB");
    }
}

#define HSCB_TO_VAL                     1000
#define TEST_ASSERT_MEMCMP(CFG,I1,I2)   do                      \
    {                                                           \
            rumboot_printf(                                     \
                    "Compare %d bytes data "                    \
                    "at 0x%x and 0x%x\n",                       \
                    sizeof(test_hscbx_src_data),                \
                    (uint32_t)(CFG[I1].src_addr),               \
                    (uint32_t)(CFG[I2].dst_addr));              \
            TEST_ASSERT(memcmp(CFG[I1].src_addr,                \
                               CFG[I2].dst_addr,                \
                    sizeof(test_hscbx_src_data)) == 0,          \
                    "HSCB0->" #I2 " data compare error!\n");    \
    } while(0)
#define WAIT_HSCB_HANDLER(H,T)          (((!H[0])  ||           \
                                          (!H[1])  ||           \
                                          (!H[2])  ||           \
                                          (!H[3])) &&           \
                                          (T++ < 1000))

void hscb_memcmp(hscb_instance_t* hscb_cfg)
{
    int timeout = 0, i = 0;

    while(WAIT_HSCB_HANDLER(hscb_handler, timeout));

    for (i = 0; i < 4; i++)
        rumboot_printf("hscb_handler[%d] = %d\n",
                i, (int)hscb_handler[i]);

    TEST_ASSERT(timeout < HSCB_TO_VAL, "HSCB finish is timed out");

    // PRETTY_HEXDUMP(test_hscb0_data_dst, 32);
    // PRETTY_HEXDUMP(test_hscb1_data_dst, sizeof(test_hscbx_src_data));
    // PRETTY_HEXDUMP(test_hscb2_data_dst, sizeof(test_hscbx_src_data));
    // PRETTY_HEXDUMP(test_hscb3_data_dst, sizeof(test_hscbx_src_data));

    TEST_ASSERT_MEMCMP(hscb_cfg, 0, 1);
    TEST_ASSERT_MEMCMP(hscb_cfg, 1, 0);
    TEST_ASSERT_MEMCMP(hscb_cfg, 2, 3);
    TEST_ASSERT_MEMCMP(hscb_cfg, 3, 2);

    for(i = 0; i < 4; i++)
        memset(hscb_cfg[i].dst_addr, 0, sizeof(test_hscbx_src_data));
}
#undef TEST_ASSERT_MEMCMP
#endif

int main(void)
{

#ifdef CHECK_PLB6_AXI_SINGLE
    rumboot_putstring(
            "Start test_plb6_axi_greth.\n"
            "Apply access to GRETH0 regs for checking "
            "PLB6->AXI convertion for single transactions\n");
    regs_check(GRETH_0_BASE);
    rumboot_printf("End check GRETH registers. \n");
    single_plb6_axi_test(IM1_BASE);
#endif


#ifdef CHECK_PLB6_AXI_BURST
    uint32_t src_im1, dst_im1;
    volatile uint64_t src_im1_physical, dst_im1_physical;

    src_im1 = (IM1_BASE);
    rumboot_printf("src_im1 = 0x%x\n", src_im1);

    dst_im1 = (IM1_BASE + 0x80);
    rumboot_printf("dst_im1 = 0x%x\n", dst_im1);

    set_mem_window(MEM_WINDOW_0);
    src_im1_physical = rumboot_virt_to_phys((uint32_t*)src_im1);
    rumboot_printf("src_im1_physical = 0x%x%x \n",
            src_im1_physical >> 32,
            src_im1_physical & 0xFFFFFFFF);

    dst_im1_physical = rumboot_virt_to_phys((uint32_t*)dst_im1);
    rumboot_printf("dst_im1_physical = 0x%x%x \n",
            dst_im1_physical >> 32,
            dst_im1_physical & 0xFFFFFFFF);

    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                            dst_im1,
                                            src_im1_physical,
                                            dst_im1_physical) == true, "IM1-to-IM1 failed");
#endif

#ifdef CHECK_AXI_PLB6_SINGLE
    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_printf("Init done.\n");

    test_oi10_greth();
#endif

#ifdef CHECK_AXI_PLB6_BURST

    struct rumboot_irq_entry *tbl;

    #define ARWLEN_ARR_SIZE       7

    hscb_axi_arwlen_t arwlen_codes[ARWLEN_ARR_SIZE] =
    {
            HSCB_ARWLEN_1,
            HSCB_ARWLEN_2,
            HSCB_ARWLEN_4,
            HSCB_ARWLEN_7,
            HSCB_ARWLEN_8,
            HSCB_ARWLEN_14,
            HSCB_ARWLEN_16
    };

    int arwlen_values[ARWLEN_ARR_SIZE] = {1,2,4,7,8,14,16};

    tbl = create_hscb_irq_handlers(hscb_cfg);

    rumboot_printf("Start test_oi10_hscb. Transmit/receive checks\n");
    prepare_test_data_hscb(
            (void*)test_hscbx_src_data,
            sizeof(test_hscbx_src_data));

    int i;
    for (i = 0; i < ARWLEN_ARR_SIZE; i++)
    {
        rumboot_printf("Check with ARWLEN: %d(0x%X) / ARWBURST: 0x%X\n",
                arwlen_values[i], arwlen_codes[i], HSCB_ARWBURST_INCR);
        configure_hscb(hscb_cfg,  arwlen_codes[i], HSCB_ARWBURST_INCR);
        run_hscb_transfers_via_ext_loop(hscb_cfg);
        hscb_memcmp(hscb_cfg);
    }

    delete_irq_handlers(tbl);
#endif


    rumboot_putstring("TEST OK!\n");
    return 0;
}
