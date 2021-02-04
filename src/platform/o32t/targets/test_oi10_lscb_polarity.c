
//-----------------------------------------------------------------------------
//  This program is for MKIO polarity testing
//    
//    It is designed to test different MKIO controllers
//    
//    It can be run on:
//      - rtl simulation
//      - (TODO) netlist simulation
//      - (TODO) post-production special hardware
//    
//    It includes:
//      - prepare data space:
//          - allocate space in requested memory for:
//              - data source
//              - data destination
//              - MKIO BC descriptors
//              - MKIO RT descriptors
//              - MKIO RT sub-address table
//      - predefine struct with transations info
//      - create BC IRQ handler
//      - transmit data in usual polarity mode:
//          - create descriptors
//          - run MKIO RT
//          - run MKIO BC
//          - wait MKIO IRQ
//          - check data
//      - transmit data in BUS A polarity inversion mode:
//          - ...
//      - transmit data in BUS B polarity inversion mode:
//          - ...
//      - delete IRQ handlers
//      - turn MKIO controllers off
//        
//        
//    Test duration (RTL): ~ 6.9 ms
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

#include <regs/regs_mkio.h>
#include <devices/mkio.h>
#include <regs/regs_gpio_pl061.h>




//-----------------------------------------------------------------------------
//  Test features control
//-----------------------------

//  Data transfer direction
//      If TEST_MODE__BC_TRANSMIT set to 0 from Cmake, read data from RT
#ifndef TEST_MODE__BC_TRANSMIT
#define TEST_MODE__BC_TRANSMIT 1
#endif


#define MKIO_TEST_LEN_IN_WORDS  2
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
    
    mkio_irq_found = true;
}


struct rumboot_irq_entry * create_irq_handlers(struct mkio_instance volatile* mkio_inst)
{
    uint32_t irq_number;
    
    rumboot_printf( "    Create irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    
    switch (mkio_inst->src_mkio_base_addr) {
        case MKIO0_BASE : {irq_number = MKIO0_INT; break;}
        case MKIO1_BASE : {irq_number = MKIO1_INT; break;}
        case MKIO2_BASE : {irq_number = MKIO2_INT; break;}
        case MKIO3_BASE : {irq_number = MKIO3_INT; break;}
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

    test_mkio_data_src = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);

    test_mkio_data_dst = (uint16_t *)rumboot_malloc_from_named_heap_aligned(heap_name, MKIO_TEST_DATA_SIZE, 2);

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
    
    rumboot_printf("    MKIO BC transmit data\n");
    mkio_prepare_bc_descr(mkio_cfg->src_mkio_base_addr, mkio_cfg->src_addr, mkio_cfg->size, mkio_cfg->bc_desr, mkio_cfg->bus, 0);
    mkio_prepare_rt_descr(mkio_cfg->dst_mkio_base_addr, mkio_cfg->dst_addr, mkio_cfg->size, mkio_cfg->rt_descr, mkio_cfg->rt_sa_tbl);
        
    mkio_enable_all_irq(mkio_cfg->src_mkio_base_addr);
    mkio_set_bcrd(mkio_cfg->src_mkio_base_addr, rumboot_virt_to_dma(mkio_irq_ring_buffer));
}

int run_mkio_transfers_via_external_loopback(struct mkio_instance * mkio_cfg, uint32_t write_data)
{
    configure_mkio(mkio_cfg, write_data);
    
    mkio_rt_run_schedule(mkio_cfg->dst_mkio_base_addr);
    mkio_bc_run_schedule(mkio_cfg->src_mkio_base_addr);
    
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
    rumboot_printf("\n\ntest_oi10_lscb_polarity start\n\n");
    
    
    struct mkio_instance mkio_cfg;
    struct rumboot_irq_entry *tbl;
    uint32_t bc_base_addr;
    uint32_t rt_base_addr;
    
    uint32_t write_data;
    
    write_data = TEST_MODE__BC_TRANSMIT;
    
    heap_name = TEST_BANK;

    mkio_irq_found = false;
    
    bc_base_addr = MKIO_BASE;
    
    prepare_test_data(heap_name);
    
    switch (MKIO_BASE) {
        case MKIO0_BASE : {rt_base_addr = MKIO1_BASE; break;}
        case MKIO1_BASE : {rt_base_addr = MKIO0_BASE; break;}
        case MKIO2_BASE : {rt_base_addr = MKIO3_BASE; break;}
        case MKIO3_BASE : {rt_base_addr = MKIO2_BASE; break;}
    }
    
    
    //-------------------------------------------------------------------------
    //  This cycle is for checking our polarity scheme.
    //  Has no functional meaning for program - just for waveforms review.
    for (uint32_t i = 0; i <= 31; i++)
    {
        mkio_set_polarity ((1 << i), bc_base_addr);
        mkio_set_polarity ((1 << i), rt_base_addr);
    }
    
    
    init_mkio_cfg(&mkio_cfg, bc_base_addr, rt_base_addr, test_mkio_data_src, test_mkio_data_dst, mkio_bc_desr, mkio_rt_descr, MKIO_TEST_DATA_SIZE, MKIO_BUS_A, rt_sa_tbl);
    tbl = create_irq_handlers(&mkio_cfg);
    
    
    //-------------------------------------------------------------------------
    //  Transmit and receive data with 1553 bus A with usual polarity
    //  Not neccessary, but good for comparing with inverted mode wave forms
    if (run_mkio_transfers_via_external_loopback(&mkio_cfg, write_data) != 0)
        if (mkio_expected_error != true)
            return -2;
        
    if (wait_mkio_irq() != 0)
        if (mkio_expected_error != true)
            return -3;

    if (mem_cmp(test_mkio_data_src, test_mkio_data_dst, MKIO_TEST_DATA_SIZE) != 0)
        if (mkio_expected_error != true)
            return -4;
    rumboot_printf("  transcieve with usual polarity OK\n");
    
    
    //-------------------------------------------------------------------------
    //  Transmit and receive data with 1553 bus A with reverted polarity
    mkio_set_polarity (0x05, bc_base_addr);
    mkio_set_polarity (0x05, rt_base_addr);
    
    if (run_mkio_transfers_via_external_loopback(&mkio_cfg, write_data) != 0)
        if (mkio_expected_error != true)
            return -5;
        
    if (wait_mkio_irq() != 0)
        if (mkio_expected_error != true)
            return -6;

    if (mem_cmp(test_mkio_data_src, test_mkio_data_dst, MKIO_TEST_DATA_SIZE) != 0)
        if (mkio_expected_error != true)
            return -7;
    rumboot_printf("  transcieve with reverted BUS A polarity OK\n");
    
    
    //-------------------------------------------------------------------------
    //  Transmit and receive data with 1553 bus B with reverted polarity
    //-------------------------------------------------------------------------
    //  Change BC and RT data signals polarity for bus B
    mkio_set_polarity (0x0A, bc_base_addr);
    mkio_set_polarity (0x0A, rt_base_addr);
    
    init_mkio_cfg(&mkio_cfg, bc_base_addr, rt_base_addr, test_mkio_data_src, test_mkio_data_dst, mkio_bc_desr, mkio_rt_descr, MKIO_TEST_DATA_SIZE, MKIO_BUS_B, rt_sa_tbl);
    
    if (run_mkio_transfers_via_external_loopback(&mkio_cfg, write_data) != 0)
        if (mkio_expected_error != true)
            return -8;
        
    if (wait_mkio_irq() != 0)
        if (mkio_expected_error != true)
            return -9;

    if (mem_cmp(test_mkio_data_src, test_mkio_data_dst, MKIO_TEST_DATA_SIZE) != 0)
        if (mkio_expected_error != true)
            return -10;
    rumboot_printf("  transcieve with reverted BUS B polarity OK\n");
    
    
    delete_irq_handlers(tbl);

    turn_mkio_off (rt_base_addr);
    turn_mkio_off (bc_base_addr);
    
    
    rumboot_printf("\n\ntest_oi10_lscb finish\n\n");
    return 0;
}

