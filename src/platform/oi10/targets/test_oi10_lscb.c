
//-----------------------------------------------------------------------------
//  This program is for MKIO testing
//    
//    It is designed to make few different types of test
//    It is designed to test different MKIO controllers
//    It is designed to be run on different SoC (Platforms)
//      - OI10
//      - O32T
//
//      (bad practice to unite many features in one source, never do this again)
//    
//    
//    It can be run on:
//      - rtl simulation
//      - netlist simulation
//      - post-production special hardware
//    
//    
//    CHECK_MKIO_REGS:
//      Test includes:
//      - predefine struct with registers addr, access, field mask
//      - write/read registers testing according to the struct
//
//    CHECK_MKIO_FUNC
//      - prepare data space:
//          - allocate space in requested memory for:
//              - data source
//              - data destination
//              - MKIO BC descriptors
//              - MKIO RT descriptors
//              - MKIO RT sub-address table
//      - predefine struct with transations info
//      - create IRQ handlers
//      - transmit data:
//          - check external MKIO signals are idle with GPIO
//          - create descriptors
//          - run MKIO RT
//          - check external MKIO signals with GPIO
//          - run MKIO BC
//          - check external MKIO signals with GPIO
//      - delete IRQ handlers
//      - wait MKIO IRQ
//      - check data
//      - turn MKIO controllers off
//        
//        
//      WARNING: Tx/Rx data can be placed in non-accessible memory, and test will pass
//        Now it is only "IM0" memory
//        
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------


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
#include <rumboot/timer.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
// #include <platform/test_assert.h>
#ifdef PLATFORM_O32T
#else
    #include <devices/ugly/emi.h>
#endif

#include <regs/regs_mkio.h>
#include <devices/mkio.h>
#include <regs/regs_gpio_pl061.h>
// #include <devices/gpio_pl061.h>




//-----------------------------------------------------------------------------
//  Test features control
//-----------------------------

//  Data transfer direction
//      If TEST_MODE__BC_TRANSMIT set to 0 from Cmake, read data from RT
#ifndef TEST_MODE__BC_TRANSMIT
#define TEST_MODE__BC_TRANSMIT 1
#endif


#define MKIO_TEST_LEN_IN_WORDS  32
#define MKIO_TEST_DATA_SIZE     (MKIO_TEST_LEN_IN_WORDS*sizeof(uint16_t))

//-------------------------------------------------------------



int check_mkio_txinh_idle_via_gpio();
int check_mkio_txinh_switch_via_gpio(uint8_t exp_state);


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

volatile bool mkio_irq_found = false;
volatile bool mkio_dma_error = false;
volatile bool mkio_expected_error = false;

char* heap_name; 

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

int regs_check(uint32_t base_address)
{
    if(rumboot_regpoker_check_array(mkio_check_array, base_address) !=0)
    {
        rumboot_printf("    Failed to check MKIO registers\n");
        return -1;
    }
    return 0;
}

static void handler_mkio( int irq, void *arg )
{
    uint32_t cur_status;

    struct mkio_instance volatile* mkio_inst = (struct mkio_instance volatile* ) arg;
    // rumboot_printf( "    MKIO(0x%X) IRQ arrived\n", mkio_inst->src_mkio_base_addr);
    cur_status = ioread32(mkio_inst->src_mkio_base_addr + IRQ);
    // rumboot_printf( "    IRQ = 0x%X\n", cur_status);
    
    
    //  BC Transfer-triggered event interrupt
    if (cur_status & (1<<0))
        iowrite32((1<<0), mkio_inst->src_mkio_base_addr + IRQ);
    
    
    //  BC DMA Error Enable
    if (cur_status & (1<<1))
    {
        if (heap_name == "IM0")
        {
            rumboot_printf("WARNING: Tried to access closed IM0 space. Suppose, it is test feature.\n");
            mkio_expected_error = true;
        }
        
        mkio_dma_error = true;
        iowrite32((1<<1), mkio_inst->src_mkio_base_addr + IRQ);
    }
    
    
    mkio_irq_found = true;
}


struct rumboot_irq_entry * create_irq_handlers(struct mkio_instance volatile* mkio_inst)
{
    uint32_t irq_number;
    
    rumboot_printf( "    Create irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    
    switch (mkio_inst->src_mkio_base_addr) {
#ifdef PLATFORM_O32T
        case MKIO0_BASE : {irq_number = MKIO0_INT; break;}
        case MKIO1_BASE : {irq_number = MKIO1_INT; break;}
        case MKIO2_BASE : {irq_number = MKIO2_INT; break;}
        case MKIO3_BASE : {irq_number = MKIO3_INT; break;}
#else
        case MKIO0_BASE : {irq_number = MKIO0_INT; break;}
        case MKIO1_BASE : {irq_number = MKIO1_INT; break;}
#endif
    }
    
    rumboot_irq_set_handler( tbl, irq_number, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_mkio, (void*)mkio_inst );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( irq_number );
    
    rumboot_irq_sei();
    return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

void prepare_test_data(char* heap_name)
{
    rumboot_printf("    Preparing source test data\n");

#ifdef PLATFORM_O32T
    //  TODO: add emi controller init for O32T. It is not done yet.
#else
    emi_init(DCR_EM2_EMI_BASE);
#endif
    
#ifndef DATA_DIRECT_ADDRESS
    test_mkio_data_src = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);

    test_mkio_data_dst = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);
#else
    test_mkio_data_src = DATA_DIRECT_ADDRESS + 0x0000;
    test_mkio_data_dst = DATA_DIRECT_ADDRESS + 0x1000;
#endif

    mkio_bc_desr  = (struct mkio_bc_descriptor volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_bc_descriptor), 128);
    memset((uint32_t*)mkio_bc_desr, 0, sizeof(struct mkio_bc_descriptor));

    mkio_rt_descr = (struct mkio_rt_descriptor volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_rt_descriptor), 128);
    memset((uint32_t*)mkio_rt_descr, 0, sizeof(struct mkio_rt_descriptor));

    rt_sa_tbl = (struct mkio_rt_sa_table volatile*)rumboot_malloc_from_named_heap_aligned(heap_name, sizeof(struct mkio_rt_sa_table)*2, 512);
    memset((uint32_t*)rt_sa_tbl, 0, sizeof(struct mkio_rt_sa_table)*2);

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

void configure_mkio(struct mkio_instance * mkio_cfg, uint32_t write_data)
{
    uint32_t* mkio_irq_ring_buffer;
    mkio_irq_ring_buffer = (uint32_t *)rumboot_malloc_from_heap_aligned(1, 16 * sizeof(uint32_t), 64);
    
    if (write_data)
    {
        rumboot_printf("    MKIO BC transmit data\n");
        mkio_prepare_bc_descr(mkio_cfg->src_mkio_base_addr, mkio_cfg->src_addr, mkio_cfg->size, mkio_cfg->bc_desr, mkio_cfg->bus, 0);
        mkio_prepare_rt_descr(mkio_cfg->dst_mkio_base_addr, mkio_cfg->dst_addr, mkio_cfg->size, mkio_cfg->rt_descr, mkio_cfg->rt_sa_tbl);
    }
    else
    {
        rumboot_printf("    MKIO BC receive data\n");
        mkio_prepare_bc_descr(mkio_cfg->src_mkio_base_addr, mkio_cfg->dst_addr, mkio_cfg->size, mkio_cfg->bc_desr, mkio_cfg->bus, 1);
        mkio_prepare_rt_descr(mkio_cfg->dst_mkio_base_addr, mkio_cfg->src_addr, mkio_cfg->size, mkio_cfg->rt_descr, mkio_cfg->rt_sa_tbl);
    }
        
    mkio_enable_all_irq(mkio_cfg->src_mkio_base_addr);
    mkio_set_bcrd(mkio_cfg->src_mkio_base_addr, rumboot_virt_to_dma(mkio_irq_ring_buffer));
}

int run_mkio_transfers_via_external_loopback(struct mkio_instance * mkio_cfg, uint32_t write_data)
{
#if !defined (CMAKE_BUILD_TYPE_POSTPRODUCTION)
    if (check_mkio_txinh_idle_via_gpio() != 0)
        return -1;
#endif


    configure_mkio(mkio_cfg, write_data);
    mkio_rt_run_schedule(mkio_cfg->dst_mkio_base_addr);
    
    
#if !defined (CMAKE_BUILD_TYPE_POSTPRODUCTION)
    uint8_t gpio_state;
 #ifdef PLATFORM_O32T
    //In tb GPIO1[7:0] = {MK3_TXINHB, MK3_TXINHA, MK2_TXINHB, MK2_TXINHA, MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    switch (mkio_cfg->dst_mkio_base_addr) {
        case MKIO0_BASE : {gpio_state = 0b11111100; break;}
        case MKIO1_BASE : {gpio_state = 0b11110011; break;}
        case MKIO2_BASE : {gpio_state = 0b11001111; break;}
        case MKIO3_BASE : {gpio_state = 0b00111111; break;}
    }
 #else
    //In tb GPIO1[3:0] = {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    switch (mkio_cfg->dst_mkio_base_addr) {
        case MKIO0_BASE : {gpio_state = 0b00001100; break;}
        case MKIO1_BASE : {gpio_state = 0b00000011; break;}
    }
 #endif
    if (check_mkio_txinh_switch_via_gpio(gpio_state) != 0)
        return -1;
#endif
    
    
    mkio_bc_run_schedule(mkio_cfg->src_mkio_base_addr);
    
    
#if !defined (CMAKE_BUILD_TYPE_POSTPRODUCTION)
 #ifdef PLATFORM_O32T
    //In tb GPIO1[7:0] = {MK3_TXINHB, MK3_TXINHA, MK2_TXINHB, MK2_TXINHA, MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    switch (mkio_cfg->dst_mkio_base_addr) {
        case MKIO0_BASE : {gpio_state = 0b11110000; break;}
        case MKIO1_BASE : {gpio_state = 0b11110000; break;}
        case MKIO2_BASE : {gpio_state = 0b00001111; break;}
        case MKIO3_BASE : {gpio_state = 0b00001111; break;}
    }
 #else
    //In tb GPIO1[3:0] = {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA}
    gpio_state = 0b00000000;
 #endif
    if (check_mkio_txinh_switch_via_gpio(gpio_state) != 0)
        return -1;
#endif
    if(mkio_wait_bc_schedule_state(mkio_cfg->src_mkio_base_addr, MKIO_BCSL_SCST_EXEC) != true)
    {
        rumboot_printf("    BC sched state EXEC is timed out\n");
        return -1;
    }
    
    return 0;
}

int wait_mkio_irq()
{
#if !defined (CMAKE_BUILD_TYPE_POSTPRODUCTION)
#define MKIO_TR_TIMEOUT_US 1000
#else
//  Value 5000 obtained experimantally (PCB MT150.02, full 1553 bus realization)
#define MKIO_TR_TIMEOUT_US 5000
#endif
    uint32_t _start = rumboot_platform_get_uptime();
    rumboot_printf("    Waiting MKIO interrupt...\n");
    while ((mkio_irq_found==false) && (rumboot_platform_get_uptime() - _start < MKIO_TR_TIMEOUT_US)){};
    if(rumboot_platform_get_uptime() - _start >= MKIO_TR_TIMEOUT_US)
    {
        rumboot_printf("    Failed to waiting MKIO interrupt\n");
        return -1;
    }
    rumboot_printf("    Waiting MKIO interrupt... OK\n");
    
    return 0;
}

int mem_cmp(uint16_t* src, uint16_t* dst, uint32_t size)
{
    uint32_t i = 0;
    bool err = false;
    rumboot_printf("    Start compare source and destination data\n");
    while (i<size)
    {
        if (*src!=*dst)
        {
            rumboot_printf("    %d. Data error\nsrc[0x%X] = 0x%X\ndst[0x%X] = 0x%X\n\n", (i / sizeof(uint16_t)), (uint32_t)src, *src, (uint32_t)dst, *dst);
            err = true;
        }
        src++; dst++;
        i += sizeof(uint16_t);
    }
    if(err != false)
    {
        rumboot_printf ("    Were data error(s). Please review log\n");
        return -1;
    }
    rumboot_printf("    Data OK!\n");
    
    return 0;
}

int check_mkio_txinh_idle_via_gpio()
{
    uint8_t data;
    rumboot_printf("    Checking initial state of TXINH (HIGH) via GPIO\n");
    
    //  Set GPIO as input - 0
    // gpio_set_value_by_mask(GPIO_1_BASE, GPIO_REG_MASK, gpio_pin_direction_in);
    iowrite32 (0, GPIO_1_BASE+GPIO_DIR);
    //  part of address mask read bits. So plus 0x3FC
    data = ioread32 (GPIO_1_BASE+GPIO_DATA+0x3FC);
#ifdef PLATFORM_O32T
    if (data != 0b11111111)
#else
    if ((data & 0x0F) != 0b00001111)
#endif
    {
        rumboot_printf ("    Expected all MK*_TXINH* = 0b1\n");
        return -1;
    }
    rumboot_printf("    Checking initial state of TXINH (HIGH) via GPIO ... OK\n");
    
    return 0;
}

int check_mkio_txinh_switch_via_gpio(uint8_t exp_state)
{
#define MKI_TX_INH_TIMEOUT_US  50
    uint8_t data;
    uint32_t _start = rumboot_platform_get_uptime();
    rumboot_printf("    Checking {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA} switch to 0x%X\n", exp_state);
    //  part of address mask read bits. So plus 0x3FC
#ifdef PLATFORM_O32T
    data = ioread32 (GPIO_1_BASE+GPIO_DATA+0x3FC) & 0xFF;
#else
    data = ioread32 (GPIO_1_BASE+GPIO_DATA+0x3FC) & 0x0F;
#endif
    while ((data != exp_state) && (rumboot_platform_get_uptime() - _start<MKI_TX_INH_TIMEOUT_US))
    {
        //  part of address mask read bits. So plus 0x3FC
#ifdef PLATFORM_O32T
        data = ioread32 (GPIO_1_BASE+GPIO_DATA+0x3FC) & 0xFF;
#else
        data = ioread32 (GPIO_1_BASE+GPIO_DATA+0x3FC) & 0x0F;
#endif
    }
    if (data != exp_state)
    {
        rumboot_printf ("    Expected all MK*_TXINH* = 0b0\n");
        return -1;
    }
    rumboot_printf("    Checking {MK1_TXINHB, MK1_TXINHA, MK0_TXINHB, MK0_TXINHA} switch ... OK\n");
    
    return 0;
}

//  Use this function if You want to restart controller in another mode (BC <-> RT)
//    It also turn BM off
//    It can be used in selection tests
void turn_mkio_off (uint32_t mkio_base)
{
    iowrite32 (((0x1552 << 16)|(1 << 9)|(1 << 2)), mkio_base + BCA);
    //  Turn RT off
    iowrite32 (((0x1553 << 16)&(~(1 << 0))), mkio_base + RTC);
    //  Turn BM off
    iowrite32 (((0x1543 << 16)&(~(1 << 0))), mkio_base + BMC);
}

int main(void)
{
    rumboot_printf("test_oi10_lscb start\n\n");
    
    
#ifdef CHECK_MKIO_REGS
    rumboot_printf("  Registers access checks for MKIO module (0x%X)\n", MKIO_BASE);
    if (regs_check(MKIO_BASE) != 0)
        return -1;
#endif
    
    
#ifdef CHECK_MKIO_FUNC
    struct mkio_instance mkio_cfg;
    struct rumboot_irq_entry *tbl;
    uint32_t bc_base_addr;
    uint32_t rt_base_addr;
    
    uint32_t write_data;
    
    write_data = TEST_MODE__BC_TRANSMIT;
    
    heap_name = TEST_BANK;
    
    prepare_test_data(heap_name);

    mkio_irq_found = false;
    bc_base_addr = MKIO_BASE;
    switch (MKIO_BASE) {
#ifdef PLATFORM_O32T
        case MKIO0_BASE : {rt_base_addr = MKIO1_BASE; break;}
        case MKIO1_BASE : {rt_base_addr = MKIO0_BASE; break;}
        case MKIO2_BASE : {rt_base_addr = MKIO3_BASE; break;}
        case MKIO3_BASE : {rt_base_addr = MKIO2_BASE; break;}
#else
        case MKIO0_BASE : {rt_base_addr = MKIO1_BASE; break;}
        case MKIO1_BASE : {rt_base_addr = MKIO0_BASE; break;}
#endif
    }
    rumboot_printf("  Start MKIO functional test\nBC: 0x%X\nRT: 0x%X\n", bc_base_addr, rt_base_addr);
    init_mkio_cfg(&mkio_cfg, bc_base_addr, rt_base_addr, test_mkio_data_src, test_mkio_data_dst, mkio_bc_desr, mkio_rt_descr, MKIO_TEST_DATA_SIZE, MKIO_BUS, rt_sa_tbl);
    tbl = create_irq_handlers(&mkio_cfg);

    if (run_mkio_transfers_via_external_loopback(&mkio_cfg, write_data) != 0)
        if (mkio_expected_error != true)
            return -2;
        
    if (wait_mkio_irq() != 0)
        if (mkio_expected_error != true)
            return -3;

    delete_irq_handlers(tbl);

    if (mem_cmp(test_mkio_data_src, test_mkio_data_dst, MKIO_TEST_DATA_SIZE) != 0)
        if (mkio_expected_error != true)
            return -4;
    
    turn_mkio_off (rt_base_addr);
    turn_mkio_off (bc_base_addr);
#endif
    
    
    rumboot_printf("\n\ntest_oi10_lscb finish\n");
    return 0;
}

