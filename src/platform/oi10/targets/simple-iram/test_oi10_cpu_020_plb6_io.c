/*
 * test_oi10_cpu_020_plb6_io.c
 *
 *  Created on: Aug 20, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>
#include <regs/fields/mpic128.h>
//#include <platform/devices/greth.h>
//#include <regs/regs_hscb.h>
//#include <devices/hscb.h>

typedef enum {
    test_io10_cpu_020_plb6_io_full_l2c_line_size = 128,
    test_io10_cpu_020_plb6_io_half_l2c_line_size = 64,
    test_io10_cpu_020_plb6_io_qword_size = 16
}test_io10_cpu_020_plb6_io_data_size;

struct greth_instance {
    uint32_t  base_addr;
    uint32_t* irq_handled;
};

#define TEST_VALUE_CLEAR                                0x0
#define TEST_VALUE_DMA0_FULL_L2C_LINE                   0x11111111
#define TEST_VALUE_DMA0_QWORD                           0x22222222
#define TEST_VALUE_GMII_FULL_L2C_LINE                   0x7E570000
#define TEST_VALUE_GMII_QWORD                           0xBABA0000
#define TEST_VALUE_HSCB_FULL_L2C_LINE                   0x37BA0000
#define TEST_VALUE_HSCB_QWORD                           0xDEDA0000

#define TLB_ENTRY_LOCAL   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define DATA_BASE_ADDR (SRAM0_BASE)

#define source_test_data_rwnitc_sram0 (DATA_BASE_ADDR)
#define target_test_data_rwnitc_sram0 (DATA_BASE_ADDR + 4 * test_io10_cpu_020_plb6_io_full_l2c_line_size)

#define source_test_data_write_sram0  (DATA_BASE_ADDR + 8 * test_io10_cpu_020_plb6_io_full_l2c_line_size)
#define target_test_data_write_sram0  (DATA_BASE_ADDR + 12 * test_io10_cpu_020_plb6_io_full_l2c_line_size)

/*
#define TST_MAC_MSB     0xFFFF
#define TST_MAC_LSB     0xFFFFFFFF

#define HSCB_UNDER_TEST_BASE        HSCB0_BASE
#define HSCB_SUPPLEMENTARY_BASE     HSCB1_BASE
#define HSCB_UNDER_TEST_INT         SW0_HSCB_INT
#define HSCB_UNDER_TEST_DMA_INT     SW0_AXI_INT
#define HSCB_SUPPLEMENTARY_INT      SW1_HSCB_INT
#define HSCB_SUPPLEMENTARY_DMA_INT  SW1_AXI_INT

#define GPIO_REG_MASK   0xFFFFFFFF
#define MAX_ATTEMPTS 2000

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;

greth_mac_t tst_greth_mac       = {TST_MAC_MSB, TST_MAC_LSB};
greth_descr_t  tx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));
greth_descr_t  rx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));
static volatile uint8_t __attribute__((section(".data"),aligned(16))) tx_desc_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"),aligned(16))) rx_desc_im[0x100] = {0} ;

static uint32_t GRETH0_IRQ_HANDLED = 0;
static uint32_t GRETH1_IRQ_HANDLED = 0;

static struct greth_instance in[ ] = {
                                        {
                                            .base_addr   = GRETH_0_BASE,
                                            .irq_handled = &GRETH0_IRQ_HANDLED
                                        },
                                        {
                                            .base_addr   = GRETH_1_BASE,
                                            .irq_handled = &GRETH1_IRQ_HANDLED
                                        }
                                     };
*/
static void cache_and_modify_data(uint32_t test_value, uint32_t source, uint32_t target)
{
    uint32_t i;
    rumboot_printf("cache_and_modify_data with value: %x\n", test_value);
    for (i = 0; i < 128; i += 4) {
        iowrite32(test_value, source + i);
        TEST_ASSERT((ioread32 (source + i) == test_value),"Data from memory do not correspond to the data sent to memory");
    }
    for (i = 0; i < 128; i += 4) {
        iowrite32(TEST_VALUE_CLEAR, target + i);
        TEST_ASSERT((ioread32(target + i) == TEST_VALUE_CLEAR),"Data from memory do not correspond to the data sent to memory");
    }
    msync();
}

static void check_values(uint32_t check_value, test_io10_cpu_020_plb6_io_data_size data_size, uint32_t target) {
    uint32_t i;
    rumboot_printf("check_values with value: %x\n", check_value);
    rumboot_printf("size: %x\n", data_size);
    for (i = 0; i < data_size; i += 4)
        COMPARE_VALUES(ioread32(target + i), check_value, "Incorrect data were read!");
}

/*
static void test_procedure_with_gmii(uint32_t base_addr_src_eth, test_io10_cpu_020_plb6_io_data_size data_size, uint32_t source, uint32_t target)
{
    rumboot_printf("test_procedure_with_gmii\n");
    uint32_t base_addr_dst_eth;
    uint32_t * eth_hadled_flag_ptr;

    TEST_ASSERT((base_addr_src_eth==GRETH_1_BASE)||(base_addr_src_eth==GRETH_0_BASE), "Wrong GRETH base address\n");

    if (base_addr_src_eth==GRETH_1_BASE)
    {
        base_addr_dst_eth = GRETH_0_BASE;
        GRETH0_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH0_IRQ_HANDLED;
    }
    else
    {
        base_addr_dst_eth = GRETH_1_BASE;
        GRETH1_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH1_IRQ_HANDLED;
    }
    rumboot_printf("\nChecking transfer from 0x%X to 0x%x\n", (uint32_t)source, (uint32_t) target);

    greth_configure_for_receive( base_addr_dst_eth, (uint32_t *) target, data_size, rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, (uint32_t *) source, data_size, tx_descriptor_data_, &tst_greth_mac);
    msync();
    greth_start_receive( base_addr_dst_eth, true );
    greth_start_transmit( base_addr_src_eth );
    msync();
    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_hadled_flag_ptr), "Receiving is failed\n");

    rumboot_printf("GMII copy completed.\n");
}
*/
static void test_procedure_with_dma(uint32_t dma_base, test_io10_cpu_020_plb6_io_data_size data_size, uint32_t source, uint32_t target)
{
    rumboot_printf("test_procedure_with_dma\n");
    uint64_t src = 0;
    uint64_t dst = 0;

    static struct dma2plb6_setup_info dma_info;
    static struct channel_status dma_status = { };
    transfer_width curent_width = tr_width_quadword;

    src = rumboot_virt_to_dma( (volatile void *) source);
    dst = rumboot_virt_to_dma( (volatile void *) target);

    dma_info.base_addr = dma_base;
    dma_info.source_adr = src;
    dma_info.dest_adr = dst;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    dma_info.transfer_width = curent_width;
    dma_info.rw_transfer_size = rw_tr_size_8q;
    dma_info.snp_mode = snp_mode_src_dst;

    dma_info.count = (data_size >> curent_width) + 1;

    rumboot_printf("DMA prepared, starting copy.\n");
    if (dma2plb6_single_copy_coherency_required(&dma_info, &dma_status) == false)
    {
        dma2plb6_trace_error_status(&dma_status);
        TEST_ASSERT(0, "DMA copy failed.\n");
    }
    rumboot_printf("DMA copy completed.\n");
}

static void  clear_test_fields (uint32_t clear_value, uint32_t source, uint32_t target)
{
    uint32_t i;
    rumboot_printf("clear_test_fields with value: %x\n", clear_value);
    for (i = 0; i < 128; i += 4)
    {
        iowrite32(clear_value, (source + i));
        TEST_ASSERT((ioread32(source + i) == clear_value),"Data from memory do not correspond to the data sent to memory");
    }
    for (i = 0; i < 128; i += 4)
    {
        iowrite32(clear_value, target + i);
        TEST_ASSERT((ioread32(target + i) == clear_value),"Data from memory do not correspond to the data sent to memory");
    }
    msync();
}

void check_target_is_invalidated_in_l2c (uint32_t l2c_base, uint32_t target, bool assert)
{
    uint64_t physical_addr;
    uint32_t tag_data;
    int  way;
    rumboot_printf("check_target_is_invalidated_in_l2c\n");
    physical_addr = get_physical_addr (target, 0);
    rumboot_printf("physical_addr low == %x\n", (uint32_t)(physical_addr & 0xFFFFFFFF));
    rumboot_printf("physical_addr high == %x\n", (uint32_t)((physical_addr >> 32) & 0xFFFFFFFF));
    msync();
    if( l2c_arracc_get_way_by_address(l2c_base, (uint32_t)(physical_addr >> 32), (uint32_t)(physical_addr & 0xFFFFFFFF), &way) )
    {
        if( l2c_arracc_tag_info_read_by_way(
                l2c_base,
                (uint32_t)(physical_addr >> 32),
                (uint32_t)(physical_addr & 0xFFFFFFFF),
                way,
                &tag_data) )
        {
            rumboot_printf("tag_data == %x\n", tag_data);
            if (assert)
            {
                TEST_ASSERT((L2C_L2ARRACCDX0_TAG_CACHE_STATE_mask & tag_data) == (L2C_TAG_CacheState_I << L2C_L2ARRACCDX0_TAG_CACHE_STATE_i),"Target area was not invalidated!\n");
            }
            else
                if ((L2C_L2ARRACCDX0_TAG_CACHE_STATE_mask & tag_data) != (L2C_TAG_CacheState_I << L2C_L2ARRACCDX0_TAG_CACHE_STATE_i))
                {
                    rumboot_printf("Target data is not invalid for address: %x\n", target);
                }
        }
        else
            rumboot_printf("Target tag was not found in L2C\n");
    }
    else
        rumboot_printf("Target tag way was not found in L2C\n");
}

void rwnitc_check (uint32_t source, uint32_t target, uint32_t xor_invertor)
{
    rumboot_printf("\n\nrwnitc_check\n");
    rumboot_printf("source == %x\n", source);
    rumboot_printf("target == %x\n", target);
    rumboot_printf("xor_invertor == %x\n", xor_invertor);

    rumboot_printf("DMA_1 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor), source, target);
    test_procedure_with_dma(DCR_DMAPLB6_BASE,
            test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_values((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor),
            test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("DMA_2 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_DMA0_QWORD ^ xor_invertor), source, target);
    test_procedure_with_dma(DCR_DMAPLB6_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_values((TEST_VALUE_DMA0_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    //commented see OI10-227 (Dryagalkin skazal test ostavit s DMA)
    /*
    rumboot_printf("GMII_1 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_GMII_FULL_L2C_LINE  ^ xor_invertor), source, target);
    test_procedure_with_gmii(GRETH_0_BASE,
            test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_values((TEST_VALUE_GMII_FULL_L2C_LINE  ^ xor_invertor),
            test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("GMII_2 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_GMII_QWORD ^ xor_invertor), source, target);
    test_procedure_with_gmii(GRETH_0_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_values((TEST_VALUE_GMII_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("HSCB_1 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_HSCB_FULL_L2C_LINE  ^ xor_invertor), source, target);
    test_procedure_with_hscb(source, target, test_io10_cpu_020_plb6_io_full_l2c_line_size);
    rumboot_printf("test_procedure hscb end\n");
    check_values((TEST_VALUE_HSCB_FULL_L2C_LINE  ^ xor_invertor), test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("HSCB_2 (rwnitc_check)\n");
    cache_and_modify_data((TEST_VALUE_HSCB_QWORD ^ xor_invertor), source, target);
    test_procedure_with_hscb(source, target, test_io10_cpu_020_plb6_io_qword_size);
    rumboot_printf("test_procedure hscb end\n");
    check_values((TEST_VALUE_HSCB_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);
    */
}

void write_check(uint32_t source, uint32_t target, uint32_t xor_invertor)
{
    uint32_t l2c_base = DCR_L2C_BASE;
    rumboot_printf("\n\nwrite_check\n");
    rumboot_printf("source == %x\n", source);
    rumboot_printf("target == %x\n", target);
    rumboot_printf("xor_invertor == %x\n", xor_invertor);
    rumboot_printf("l2c_base == %x\n", l2c_base);

    rumboot_printf("DMA_1 (write_check)\n");
    cache_and_modify_data((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_dma(DCR_DMAPLB6_BASE, test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor), test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("DMA_2 (write_check)\n");
    cache_and_modify_data((TEST_VALUE_DMA0_QWORD ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_dma(DCR_DMAPLB6_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_DMA0_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    /*
    rumboot_printf("GMII_1 (write_check)\n");

    cache_and_modify_data((TEST_VALUE_GMII_FULL_L2C_LINE ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_gmii(GRETH_0_BASE, test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_GMII_FULL_L2C_LINE ^ xor_invertor), test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("GMII_2 (write_check)\n");
    cache_and_modify_data((TEST_VALUE_GMII_QWORD ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_gmii(GRETH_0_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_GMII_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("HSCB_1 (write_check)\n");
    cache_and_modify_data((TEST_VALUE_HSCB_FULL_L2C_LINE ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_hscb(source, target, test_io10_cpu_020_plb6_io_full_l2c_line_size);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_HSCB_FULL_L2C_LINE ^ xor_invertor), test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    rumboot_printf("HSCB_2 (write_check)\n");
    cache_and_modify_data((TEST_VALUE_HSCB_QWORD ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_hscb(source, target, test_io10_cpu_020_plb6_io_full_l2c_line_size);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_HSCB_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);
    */
}

/*
static void handler_eth( int irq, void *arg )
{
    uint32_t cur_status;
    uint32_t mask;
    struct greth_instance * gr_inst = (struct greth_instance* ) arg;

    rumboot_printf( "GRETH%d(0x%X) IRQ arrived  \n", GET_GRETH_IDX(gr_inst->base_addr), gr_inst->base_addr );
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RE));

    cur_status = greth_get_status(gr_inst->base_addr);
    if (cur_status & (1 << GRETH_STATUS_RI))
    {
        *(gr_inst->irq_handled) = 1;
        rumboot_printf("Receive interrupt (0x%x)\n", cur_status );
        greth_clear_status_bits(gr_inst->base_addr, mask );
    }
    if (cur_status & (1 << GRETH_STATUS_RE))
    {
        *(gr_inst->irq_handled) = 3;
        rumboot_printf("Receive Error interrupt (0x%x)\n", cur_status );
    }
    if (!(cur_status & (1 << GRETH_STATUS_RI)) && !(cur_status & (1 << GRETH_STATUS_RE)))
    {
        *(gr_inst->irq_handled) = 2;
        rumboot_printf( "Unexpected status (0x%x)\n", cur_status );
    }
    greth_clear_status_bits(gr_inst->base_addr, mask);
    msync();
    rumboot_printf("Exit from eth handler\n");
}

static void handler_hscb( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "IRQ arrived  \n" );
    rumboot_putstring("NON_CRITICAL int handler message\n");
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_putstring("HSCB_0_IRQ\n");
        hscb0_status = hscb_get_status(HSCB_UNDER_TEST_BASE);
        if(!hscb0_link_established)
            hscb0_link_established = hscb0_status & (1 << HSCB_STATUS_ACTIVE_LINK_i);
        rumboot_puthex (hscb0_status );
    }
    if (irq == HSCB_UNDER_TEST_DMA_INT) {
        rumboot_putstring("HSCB_0_DMA_IRQ\n");
        hscb0_dma_status = hscb_get_adma_ch_status(HSCB_UNDER_TEST_BASE );
        rumboot_puthex(hscb0_dma_status);
        if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_rdma_status(HSCB_UNDER_TEST_BASE);
        else if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_wdma_status(HSCB_UNDER_TEST_BASE);
        rumboot_puthex(hscb0_dma_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_INT) {
        rumboot_putstring("HSCB_1_IRQ\n");
        hscb1_status = hscb_get_status(HSCB_SUPPLEMENTARY_BASE);
        if(!hscb1_link_established)
            hscb1_link_established = hscb1_status & (1 << HSCB_STATUS_ACTIVE_LINK_i);
        rumboot_puthex (hscb1_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        rumboot_putstring("HSCB_1_DMA_IRQ\n");
        hscb1_dma_status = hscb_get_adma_ch_status(HSCB_SUPPLEMENTARY_BASE );
        isync();
        msync();
        rumboot_puthex(hscb1_dma_status);
        if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_rdma_status(HSCB_SUPPLEMENTARY_BASE);
        else if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_wdma_status(HSCB_SUPPLEMENTARY_BASE);
    }
}

struct rumboot_irq_entry * create_irq_handlers()
{
    rumboot_printf( "Enable GRETH irqs\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_INT,          RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_DMA_INT,      RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_INT,       RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_DMA_INT,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, ( void* )0 );
    rumboot_irq_set_handler( tbl, ETH0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[0] );
    rumboot_irq_set_handler( tbl, ETH1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[1] );

    // Activate the table
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( ETH0_INT );
    rumboot_irq_enable( ETH1_INT );
    rumboot_irq_enable( HSCB_UNDER_TEST_INT );
    rumboot_irq_enable( HSCB_UNDER_TEST_DMA_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_DMA_INT );
    rumboot_irq_sei();

    return tbl;
}

void delete_irq_handler(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}
*/
void init_data ()
{
    rumboot_printf ("Init data\n");
    uint32_t i = 0;
    for (i = 0; i < test_io10_cpu_020_plb6_io_full_l2c_line_size; i++)
    {
        iowrite32(i*i, source_test_data_write_sram0 + i * 4);
        iowrite32(2*i, source_test_data_rwnitc_sram0 + i * 4);
    }
    msync();
    dci(2);
}
/*
static void test_procedure_with_hscb(uint32_t source, uint32_t target, uint32_t size){
    rumboot_printf("test_procedure_with_hscb\n");
    int cnt = 0;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    hscb_configure_for_transmit(HSCB_UNDER_TEST_BASE, source, size, (uint32_t) tx_desc_im);
    hscb_configure_for_receive(HSCB_SUPPLEMENTARY_BASE, target, size, (uint32_t) rx_desc_im);
    rumboot_putstring( "Enable HSCB...\n" );
    // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    HSCB_UNDER_TEST_BASE + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    HSCB_SUPPLEMENTARY_BASE + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        HSCB_UNDER_TEST_BASE + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        HSCB_SUPPLEMENTARY_BASE + HSCB_SETTINGS);
    // Wait connecting
    rumboot_putstring( "Wait HSCB0 and HSCB1 enable\n" );
    while (!(hscb0_link_established & hscb1_link_established)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            TEST_ASSERT(0, "TEST_ERROR");
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_status = 0;
    hscb1_status = 0;

    rumboot_putstring( "HSCB link has enabled\n" );
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );

    hscb_run_rdma(HSCB_UNDER_TEST_BASE);
    hscb_run_wdma(HSCB_SUPPLEMENTARY_BASE);
    msync();
    rumboot_putstring( "Wait HSCB0 and HSCB1 finish work\n" );
    while (!(hscb0_dma_status || hscb1_dma_status)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            TEST_ASSERT(0, "TEST_ERROR");
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    if (hscb_adma_sw_rst(HSCB_UNDER_TEST_BASE) && hscb_adma_sw_rst(HSCB_SUPPLEMENTARY_BASE) && hscb_sw_rst(HSCB_UNDER_TEST_BASE) && hscb_sw_rst(HSCB_SUPPLEMENTARY_BASE))
      rumboot_putstring( "HSCB rst!\n" );
    else
      rumboot_putstring( "NOOOOOOOOO! HSCB dont rst!\n" );  ;
    rumboot_putstring( "Finish work!\n" );
}
*/
int main ()
{
    const uint32_t EVENT_START_MONITOR_PLB6 = 0x00001000;
    test_event_send_test_id("test_oi10_cpu_020_plb6_io");
    test_event(EVENT_START_MONITOR_PLB6);

    emi_init(DCR_EM2_EMI_BASE);

    init_data();

    rumboot_printf ("Set TLB Entry\n");
    static const tlb_entry sram0_tlb_entry_local = {TLB_ENTRY_LOCAL};
    write_tlb_entries(&sram0_tlb_entry_local,1);
/*
    struct rumboot_irq_entry *tbl;
    tbl = create_irq_handlers();
*/

    rumboot_printf("(uint32_t) source_test_data_rwnitc_sram0 == %x\n", (uint32_t) source_test_data_rwnitc_sram0);
    rumboot_printf("(uint32_t) target_test_data_rwnitc_sram0 == %x\n", (uint32_t) target_test_data_rwnitc_sram0);

    rumboot_printf("(uint32_t) source_test_data_write_sram0 == %x\n", (uint32_t) source_test_data_write_sram0);
    rumboot_printf("(uint32_t) target_test_data_write_sram0 == %x\n", (uint32_t) target_test_data_write_sram0);

    //SRAM0
    //RWNITC checks
    rumboot_printf("Checks\n");
    rwnitc_check((uint32_t) source_test_data_rwnitc_sram0, (uint32_t)target_test_data_rwnitc_sram0, 0x0);

    //WRITE checks
    write_check((uint32_t) source_test_data_write_sram0, (uint32_t)target_test_data_write_sram0, 0x0);

   // delete_irq_handler(tbl);
    rumboot_printf("TEST_OK\n");
    return 0;
}
