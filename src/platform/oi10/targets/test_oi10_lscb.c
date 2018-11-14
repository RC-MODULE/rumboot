
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/regpoker.h>
#include <rumboot/macros.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/devices/emi.h>

#include <regs/regs_mkio.h>
#include <devices/mkio.h>
#include <regs/regs_gpio_pl061.h>
#include <devices/gpio_pl061.h>

void check_mkio_txinh_idle_via_gpio();
void check_mkio_txinh_switch_via_gpio(uint8_t exp_state);

#define MKIO_TEST_LEN_IN_WORDS  32
#define MKIO_TEST_DATA_SIZE     (MKIO_TEST_LEN_IN_WORDS*sizeof(uint16_t))

uint16_t test_mkio_data_im0_src[MKIO_TEST_LEN_IN_WORDS] = {
                                                            0x0001,
                                                            0x0002,
                                                            0x0004,
                                                            0x0008,
                                                            0x0010,
                                                            0x0020,
                                                            0x0040,
                                                            0x0080,
                                                            0x0100,
                                                            0x0200,
                                                            0x0400,
                                                            0x0800,
                                                            0x1000,
                                                            0x2000,
                                                            0x4000,
                                                            0x8000,
                                                            0xFFFE,
                                                            0xFFFD,
                                                            0xFFFB,
                                                            0xFFF7,
                                                            0xFFEF,
                                                            0xFFDF,
                                                            0xFFBF,
                                                            0xFF7F,
                                                            0xFEFF,
                                                            0xFDFF,
                                                            0xFBFF,
                                                            0xF7FF,
                                                            0xEFFF,
                                                            0xDFFF,
                                                            0xBFFF,
                                                            0x7FFF
                                                    };

uint16_t* test_mkio_data_src;//   = (uint16_t *)( IM1_BASE );
uint16_t* test_mkio_data_dst;//   = (uint16_t *)( IM1_BASE + MKIO_TEST_DATA_SIZE);

struct mkio_bc_descriptor volatile* mkio_bc_desr;//  = (mkio_bc_descriptor*)(IM1_BASE + 2 * MKIO_TEST_DATA_SIZE);
struct mkio_rt_descriptor volatile* mkio_rt_descr;// = (mkio_rt_descriptor*)(IM1_BASE + 2 * MKIO_TEST_DATA_SIZE + sizeof(mkio_bc_descriptor));

struct mkio_rt_sa_table volatile*   rt_sa_tbl;// = (mkio_rt_sa_table_t *)rumboot_malloc_from_heap_aligned(1, sizeof(mkio_rt_sa_table_t)*2, 512);

/*
 * Registers access checks
 */
struct regpoker_checker mkio_check_array[] =
{
    { "IRQ    ", REGPOKER_READ32,  IRQ   , 0x00000000, 0xFFFFFFFF},
    { "IRQE   ", REGPOKER_READ32,  IRQE  , 0x00000000, 0xFFFFFFFF},
    { "HWCFG  ", REGPOKER_READ32,  HWCFG , 0x00000000, 0xFFFFFFFF},
    { "BCSL   ", REGPOKER_READ32,  BCSL  , 0xF0000000, 0xFFFFFFFF},
    { "BCA    ", REGPOKER_READ32,  BCA   , 0x00000000, 0xFFFFFFFF},
    { "BCTNP  ", REGPOKER_READ32,  BCTNP , 0x00000000, 0xFFFFFFFF},
    { "BCANP  ", REGPOKER_READ32,  BCANP , 0x00000000, 0xFFFFFFFF},
    { "BCT    ", REGPOKER_READ32,  BCT   , 0x00000000, 0xFFFFFFFF},
    { "BCTW   ", REGPOKER_READ32,  BCTW  , 0x00000000, 0xFFFFFFFF},
    { "BCRD   ", REGPOKER_READ32,  BCRD  , 0x00000000, 0xFFFFFFFF},
    { "BCBS   ", REGPOKER_READ32,  BCBS  , 0x00000000, 0xFFFFFFFF},
    { "BCTCP  ", REGPOKER_READ32,  BCTCP , 0x00000000, 0xFFFFFFFF},
    { "BCACP  ", REGPOKER_READ32,  BCACP , 0x00000000, 0xFFFFFFFF},
    { "RTS    ", REGPOKER_READ32,  RTS   , 0x80000000, 0xFFFFFFFF},
    { "RTC    ", REGPOKER_READ32,  RTC   , 0x0000E03E, 0xFFFFFFFF},
    { "RTBS   ", REGPOKER_READ32,  RTBS  , 0x00000000, 0xFFFFFFFF},
    { "RTSW   ", REGPOKER_READ32,  RTSW  , 0x00000000, 0xFFFFFFFF},
    { "RTSY   ", REGPOKER_READ32,  RTSY  , 0x00000000, 0xFFFFFFFF},
    { "RTSTBA ", REGPOKER_READ32,  RTSTBA, 0x00000000, 0xFFFFFFFF},
    { "RTMCC  ", REGPOKER_READ32,  RTMCC , 0x00000555, 0xFFFFFFFF},
    { "RTTTC  ", REGPOKER_READ32,  RTTTC , 0x00000000, 0xFFFFFFFF},
    { "RTELM  ", REGPOKER_READ32,  RTELM , 0xFFFFFFFC, 0xFFFFFFFF},
    { "RTELP  ", REGPOKER_READ32,  RTELP , 0x00000000, 0xFFFFFFFF},
    { "RTELIP ", REGPOKER_READ32,  RTELIP, 0x00000000, 0xFFFFFFFF},
    { "BMS    ", REGPOKER_READ32,  BMS   , 0x80000000, 0xFFFFFFFF},
    { "BMC    ", REGPOKER_READ32,  BMC   , 0x00000000, 0xFFFFFFFF},
    { "BMRTAF ", REGPOKER_READ32,  BMRTAF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTSF ", REGPOKER_READ32,  BMRTSF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTMC ", REGPOKER_READ32,  BMRTMC, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMLBS  ", REGPOKER_READ32,  BMLBS , 0x00000000, 0xFFFFFFFF},
    { "BMLBE  ", REGPOKER_READ32,  BMLBE , 0x00000007, 0xFFFFFFFF},
    { "BMLBP  ", REGPOKER_READ32,  BMLBP , 0x00000000, 0xFFFFFFFF},
    { "BMTTC  ", REGPOKER_READ32,  BMTTC , 0x00000000, 0xFFFFFFFF},
    { "BCRD   ", REGPOKER_WRITE32, BCRD  , 0x00000000, 0xFFFFFFFC},
    { "BCBS   ", REGPOKER_WRITE32, BCBS  , 0x00000000, 0xFFFFFFFF},
    { "RTSTBA ", REGPOKER_WRITE32, RTSTBA, 0x00000000, 0xFFFFFE00},
    { "RTTTC  ", REGPOKER_WRITE32, RTTTC , 0x00000000, 0xFFFFFFFF},
    { "RTELP  ", REGPOKER_WRITE32, RTELP , 0x00000000, 0xFFFFFFFC},
    { "BMRTAF ", REGPOKER_WRITE32, BMRTAF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTSF ", REGPOKER_WRITE32, BMRTSF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMLBS  ", REGPOKER_WRITE32, BMLBS , 0x00000000, 0xFFFFFFF8},
    { "BMLBE  ", REGPOKER_WRITE32, BMLBE , 0x00000007, 0x003FFFF8},
    { "BMLBP  ", REGPOKER_WRITE32, BMLBP , 0x00000000, 0x003FFFF8},
    { "BMTTC  ", REGPOKER_WRITE32, BMTTC , 0x00000000, 0xFFFFFFFF},
};

void regs_check(uint32_t base_address)
{
    TEST_ASSERT(rumboot_regpoker_check_array(mkio_check_array, base_address)==0, "Failed to check MKIO registers\n");
}

volatile bool mkio_handler = false;

static void handler_mkio( int irq, void *arg )
{
    uint32_t cur_status;
    struct mkio_instance volatile* mkio_inst = (struct mkio_instance volatile* ) arg;
    rumboot_printf( "MKIO(0x%X) IRQ arrived\n", mkio_inst->src_mkio_base_addr);
    cur_status = ioread32(mkio_inst->src_mkio_base_addr + IRQ);
    rumboot_printf( "IRQ = 0x%X\n", cur_status);
    iowrite32(1, mkio_inst->src_mkio_base_addr + IRQ);
    mkio_handler = true;
}


struct rumboot_irq_entry * create_irq_handlers(struct mkio_instance volatile* mkio_inst)
{
    rumboot_printf( "Create irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    if (mkio_inst->src_mkio_base_addr==MKIO0_BASE)
    {
        rumboot_irq_set_handler( tbl, MKIO0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_mkio, (void*)mkio_inst );
        rumboot_irq_table_activate( tbl );
        rumboot_irq_enable( MKIO0_INT );
    }
    else
    {
        rumboot_irq_set_handler( tbl, MKIO1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_mkio, (void*)mkio_inst );
        rumboot_irq_table_activate( tbl );
        rumboot_irq_enable( MKIO1_INT );
    }
    rumboot_irq_sei();
    return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

void prepare_test_data(int heap_id)
{
    char* heap_name;
    rumboot_putstring("Preparing source test data\n");

    switch (heap_id)
    {
        case 1:
            heap_name = "IM1";
            break;
        case 2:
            heap_name = "IM2";
            break;
        case 3:
            heap_name = "SSRAM";
            emi_init(DCR_EM2_EMI_BASE);
            break;
        default:
            TEST_ASSERT(0, "Wrong heap_id value int mkio func test");
    }

    test_mkio_data_src = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);
//    rumboot_printf("test_mkio_data_src from addr 0x%X\n", (uint32_t) test_mkio_data_src);

    test_mkio_data_dst = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);
//    rumboot_printf("test_mkio_data_dst from addr 0x%X\n", (uint32_t) test_mkio_data_dst);

    mkio_bc_desr  = (struct mkio_bc_descriptor volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_bc_descriptor), 128);
    memset((uint32_t*)mkio_bc_desr, 0, sizeof(struct mkio_bc_descriptor));
//    rumboot_printf("mkio_bc_desr from addr 0x%X\n", (uint32_t) mkio_bc_desr);

    mkio_rt_descr = (struct mkio_rt_descriptor volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_rt_descriptor), 128);
    memset((uint32_t*)mkio_rt_descr, 0, sizeof(struct mkio_rt_descriptor));
//    rumboot_printf("mkio_rt_descr from addr 0x%X\n", (uint32_t) mkio_rt_descr);

    rt_sa_tbl = (struct mkio_rt_sa_table volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_rt_sa_table)*2, 512);
    memset((uint32_t*)rt_sa_tbl, 0, sizeof(struct mkio_rt_sa_table)*2);
//    rumboot_printf("rt_sa_tbl from addr 0x%X\n", (uint32_t) rt_sa_tbl);

    memcpy(test_mkio_data_src, test_mkio_data_im0_src, sizeof(test_mkio_data_im0_src));
}

void init_mkio_cfg(struct mkio_instance * mkio_inst, uint32_t bc_base_addr, uint32_t rt_base_addr, uint16_t* src_addr, uint16_t* dst_addr, struct mkio_bc_descriptor volatile* bc_descr_addr, struct mkio_rt_descriptor volatile* rt_descr_addr, uint32_t size, mkio_bus_t bus, struct mkio_rt_sa_table volatile* rt_sa_tbl)
{
    mkio_inst->src_mkio_base_addr   = bc_base_addr;
    mkio_inst->dst_mkio_base_addr   = rt_base_addr;
    mkio_inst->src_addr             = (uint32_t *) src_addr;
    mkio_inst->dst_addr             = (uint32_t *) dst_addr;
    mkio_inst->bc_desr              = bc_descr_addr;
    mkio_inst->rt_descr             = rt_descr_addr;
    mkio_inst->size                 = size;
    mkio_inst->bus                  = bus;
    mkio_inst->rt_sa_tbl            = rt_sa_tbl;
}

void configure_mkio(struct mkio_instance * mkio_cfg)
{
    uint32_t* mkio_irq_ring_buffer;
    mkio_irq_ring_buffer = (uint32_t *)rumboot_malloc_from_heap_aligned(1, 16 * sizeof(uint32_t), 64);

    mkio_prepare_bc_descr(mkio_cfg->src_mkio_base_addr, mkio_cfg->src_addr, mkio_cfg->size, mkio_cfg->bc_desr, mkio_cfg->bus);
    mkio_prepare_rt_descr(mkio_cfg->dst_mkio_base_addr, mkio_cfg->dst_addr, mkio_cfg->size, mkio_cfg->rt_descr, mkio_cfg->rt_sa_tbl);

    mkio_enable_all_irq(mkio_cfg->src_mkio_base_addr);
    mkio_set_bcrd(mkio_cfg->src_mkio_base_addr, rumboot_virt_to_dma(mkio_irq_ring_buffer));
}

void run_mkio_transfers_via_external_loopback(struct mkio_instance * mkio_cfg)
{
    check_mkio_txinh_idle_via_gpio();
    configure_mkio(mkio_cfg);

    mkio_rt_run_schedule(mkio_cfg->dst_mkio_base_addr);
    //In tb GPIO1[3:0] = {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    check_mkio_txinh_switch_via_gpio((mkio_cfg->dst_mkio_base_addr==MKIO0_BASE) ? 0b00001100 : 0b00000011);

    mkio_bc_run_schedule(mkio_cfg->src_mkio_base_addr);
    //In tb GPIO1[3:0] = {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    check_mkio_txinh_switch_via_gpio(0b00000000);
    TEST_ASSERT(mkio_wait_bc_schedule_state(mkio_cfg->src_mkio_base_addr, MKIO_BCSL_SCST_EXEC)==true, "BC sched state EXEC is timed out");
}

void wait_mkio_irq()
{
#define MKIO_TR_TIMEOUT 1000
    uint32_t timeout_cnt = 0;
    rumboot_putstring("Waiting MKIO interrupt...\n");
    while ((mkio_handler==false) && (timeout_cnt++<MKIO_TR_TIMEOUT)){};
    TEST_ASSERT(timeout_cnt<MKIO_TR_TIMEOUT, "Failed to waiting MKIO interrupt");
    rumboot_putstring("Waiting MKIO interrupt... OK\n");
}
void mem_cmp(uint16_t* src, uint16_t* dst, uint32_t size)
{
    uint32_t i = 0;
    bool err = false;
    rumboot_putstring("Start compare source and destination data\n");
    while (i<size)
    {
        if (*src!=*dst)
        {
            rumboot_printf("%d. Data error\nsrc[0x%X] = 0x%X\ndst[0x%X] = 0x%X\n\n", (i / sizeof(uint16_t)), (uint32_t)src, *src, (uint32_t)dst, *dst);
            err = true;
        }
        src++; dst++;
        i += sizeof(uint16_t);
    }
    TEST_ASSERT(err==false, "Were data error(s). Please review log");
    rumboot_putstring("Data OK!\n");
}

void check_mkio_txinh_idle_via_gpio()
{
    uint8_t data;
    rumboot_putstring("Checking initial state of TXINH (HIGH) via GPIO\n");
    gpio_set_value_by_mask(GPIO_1_BASE, GPIO_REG_MASK, direction_in);
    data = gpio_get_data(GPIO_1_BASE) & 0x0F;
    TEST_ASSERT(data==0b00001111, "Expected all MK*_TXINH* = 0b1");
    rumboot_putstring("Checking initial state of TXINH (HIGH) via GPIO ... OK\n");
}

void check_mkio_txinh_switch_via_gpio(uint8_t exp_state)
{
#define MKI_TX_INH_TIMEOUT  100
    uint8_t data;
    uint32_t timeout_cnt = 0;
    rumboot_printf("Checking {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA} switch to 0x%X\n", exp_state);
    data = gpio_get_data(GPIO_1_BASE) & 0x0F;
    while ((data != exp_state) && (timeout_cnt<MKI_TX_INH_TIMEOUT))
    {
        data = gpio_get_data(GPIO_1_BASE) & 0x0F;
    }
    TEST_ASSERT(data==exp_state, "Expected all MK*_TXINH* = 0b0");
    rumboot_putstring("Checking {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA} switch ... OK\n");
}

int main(void)
{
#ifdef CHECK_MKIO_REGS
    rumboot_printf("Start test_oi10_lscb. Registers access checks for MKIO module (0x%X)\n", MKIO_BASE);
    regs_check(MKIO_BASE);
#endif

#ifdef CHECK_MKIO_FUNC
    struct mkio_instance mkio_cfg;
    struct rumboot_irq_entry *tbl;
    uint32_t bc_base_addr;
    uint32_t rt_base_addr;

    prepare_test_data(TEST_BANK);

    mkio_handler = false;
    bc_base_addr = MKIO_BASE;
    rt_base_addr = (MKIO_BASE==MKIO0_BASE) ? MKIO1_BASE : MKIO0_BASE;
    rumboot_printf("Start MKIO functional test\nBC: 0x%X\nRT: 0x%X\n", bc_base_addr, rt_base_addr);
    init_mkio_cfg(&mkio_cfg, bc_base_addr, rt_base_addr, test_mkio_data_src, test_mkio_data_dst, mkio_bc_desr, mkio_rt_descr, MKIO_TEST_DATA_SIZE, MKIO_BUS, rt_sa_tbl);
    tbl = create_irq_handlers(&mkio_cfg);

    run_mkio_transfers_via_external_loopback(&mkio_cfg);
    wait_mkio_irq();

    delete_irq_handlers(tbl);

    mem_cmp(test_mkio_data_src, test_mkio_data_dst, MKIO_TEST_DATA_SIZE);
#endif

    return 0;
}

