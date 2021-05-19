#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

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

#include <devices/ugly/greth.h>

#define EVENT_CHECK_RUN_HPROT_MONITOR     0x00001004
#define EVENT_CHECK_STOP_HPROT_MONITOR    0x00001005
/*
 * Functions and variables for interrupt handling
 */
static uint32_t volatile GRETH0_IRQ_HANDLED = 0;
static uint32_t volatile GRETH1_IRQ_HANDLED = 0;

struct greth_instance {
    uint32_t  base_addr;
    uint32_t volatile*  irq_handled;
};

static void handler_eth( int irq, void *arg )
{
    uint32_t cur_status;
    uint32_t mask;
    struct greth_instance* gr_inst = (struct greth_instance* ) arg;

    rumboot_printf( "GRETH%d(0x%X) IRQ arrived  \n", GET_GRETH_IDX(gr_inst->base_addr), gr_inst->base_addr );
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RI) |
            (1 << GRETH_STATUS_RE));

    cur_status = greth_get_status(gr_inst->base_addr);
    if (cur_status & (1 << GRETH_STATUS_RI))
    {
        *(gr_inst->irq_handled) = 1;
        rumboot_printf("Receive interrupt (0x%x)\n", cur_status );
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
    rumboot_printf("Exit from handler\n");
}

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


struct rumboot_irq_entry * create_greth01_irq_handlers()
{
    rumboot_printf( "Enable GRETH irqs\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    GRETH0_IRQ_HANDLED = 0;
    GRETH1_IRQ_HANDLED = 0;

    rumboot_irq_set_handler( tbl, ETH0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[0] );
    rumboot_irq_set_handler( tbl, ETH1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[1] );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( ETH0_INT );
    rumboot_irq_enable( ETH1_INT );
    rumboot_irq_sei();

    return tbl;
}

void delete_greth01_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

greth_descr_t*  tx0_descriptor_data_;
greth_descr_t*  rx0_descriptor_data_;
greth_descr_t*  tx1_descriptor_data_;
greth_descr_t*  rx1_descriptor_data_;
uint8_t* test_data_src;
uint8_t* test_data_dst_0[N_DESC];
uint8_t* test_data_dst_1[N_DESC];

void prepare_test_data()
{
    uint32_t i=0;

    #ifndef GRETH_CMAKE_O32T
        #ifdef INIT_EMI
            {
                rumboot_putstring("Init EMI");
                emi_init_sram40ns(DCR_EM2_EMI_BASE);
            }
        #endif
    #endif
    
    rumboot_putstring("Preparing data");
    tx0_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_named_heap_aligned(DESC_HEAP_NAME, N_DESC * sizeof(greth_descr_t), 1024);
    rx0_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_named_heap_aligned(DESC_HEAP_NAME, N_DESC * sizeof(greth_descr_t), 1024);
    
    tx1_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_named_heap_aligned(DESC_HEAP_NAME, N_DESC * sizeof(greth_descr_t), 1024);
    rx1_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_named_heap_aligned(DESC_HEAP_NAME, N_DESC * sizeof(greth_descr_t), 1024);

    test_data_src = (uint8_t* )rumboot_malloc_from_named_heap_aligned(SRC_HEAP_NAME, GRETH_TEST_DATA_LEN_BYTES, 64);
    
    rumboot_putstring("Fill src array");
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;
    test_data_src[i++] = (uint8_t)0xff;

    test_data_src[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0x00FF) >> 0);
    test_data_src[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0xFF00) >> 8);

    while(i<GRETH_TEST_DATA_LEN_BYTES)
    {
        test_data_src[i] = i;
        i++;
    }
    
    for (i = 0; i < N_DESC; i++){
        test_data_dst_0[i] = (uint8_t* )rumboot_malloc_from_named_heap_aligned(DST_HEAP_NAME, GRETH_TEST_DATA_LEN_BYTES, 64);
        test_data_dst_1[i] = (uint8_t* )rumboot_malloc_from_named_heap_aligned(DST_HEAP_NAME, GRETH_TEST_DATA_LEN_BYTES, 64);
    }
    rumboot_putstring("Preparing data finished");

// rx desc config
    uint32_t last=0;
    rumboot_printf("N_DESC = %x\n", N_DESC);
    for (i = 0; i < N_DESC; i++){
        rumboot_printf("desc_rx i = %x\n", i);
        if ( i == N_DESC-1) {
            last = 1;
        }
        else {
            last = 0;
        }
        iowrite32((last<<GRETH_RX_DESCR_IE) |
                  (last<<GRETH_RX_DESCR_WR) |
                  (1   <<GRETH_RX_DESCR_EN) |
                  GRETH_TEST_DATA_LEN_BYTES ,rumboot_virt_to_dma(rx0_descriptor_data_) + i*8);
                
        iowrite32(rumboot_virt_to_dma(test_data_dst_0[i]),rumboot_virt_to_dma(rx0_descriptor_data_) + 4 + i*8);
        
        //---
        iowrite32((last<<GRETH_RX_DESCR_IE) |
                  (last<<GRETH_RX_DESCR_WR) |
                  (1   <<GRETH_RX_DESCR_EN) |
                  GRETH_TEST_DATA_LEN_BYTES ,rumboot_virt_to_dma(rx1_descriptor_data_) + i*8);
                
        iowrite32(rumboot_virt_to_dma(test_data_dst_1[i]),rumboot_virt_to_dma(rx1_descriptor_data_) + 4 + i*8);
    }    

    rumboot_printf("----------------\n");
// tx desc config 
    for (i = 0; i < N_DESC; i++){
        rumboot_printf("desc_tx i = %x\n", i);
        if ( i == N_DESC-1) {
            last = 1;
        }
        else {
            last = 0;
        }
        iowrite32((last<<GRETH_TX_DESCR_WR) |
                  (1   <<GRETH_TX_DESCR_EN) |
                  GRETH_TEST_DATA_LEN_BYTES ,rumboot_virt_to_dma(tx0_descriptor_data_) + i*8);
                  
        iowrite32(rumboot_virt_to_dma(test_data_src),rumboot_virt_to_dma(tx0_descriptor_data_) + 4 + i*8);    
        
        //---
        iowrite32((last<<GRETH_TX_DESCR_WR) |
                  (1   <<GRETH_TX_DESCR_EN) |
                  GRETH_TEST_DATA_LEN_BYTES ,rumboot_virt_to_dma(tx1_descriptor_data_) + i*8);
                  
        iowrite32(rumboot_virt_to_dma(test_data_src),rumboot_virt_to_dma(tx1_descriptor_data_) + 4 + i*8);    
    } 
    rumboot_putstring("Preparing desc finished");
}

/*
 * Checks transfers which uses different memory blocks
 */
void start_transfer_wait_irq()
{
    uint32_t volatile* eth_handled_flag_ptr;
    
    GRETH0_IRQ_HANDLED = 0;
    eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
    
    GRETH1_IRQ_HANDLED = 0;
//    eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;

    // clear status register by writing '1's
    iowrite32( 0xffffffff, GRETH_0_BASE + STATUS);
    iowrite32( 0xffffffff, GRETH_1_BASE + STATUS);
    // setting descriptor pointer
    iowrite32(rumboot_virt_to_dma(rx0_descriptor_data_), GRETH_0_BASE + RECEIVER_DESCR_PTR);
    iowrite32(rumboot_virt_to_dma(tx0_descriptor_data_), GRETH_0_BASE + TRANSMIT_DESCR_PTR);
    
    iowrite32(rumboot_virt_to_dma(rx1_descriptor_data_), GRETH_1_BASE + RECEIVER_DESCR_PTR);
    iowrite32(rumboot_virt_to_dma(tx1_descriptor_data_), GRETH_1_BASE + TRANSMIT_DESCR_PTR);
    //Start greth
    iowrite32(0x9a00009a, GRETH_0_BASE);
    iowrite32(0x9a00009a, GRETH_1_BASE);
    
    iowrite32(0x9a00009b, GRETH_0_BASE);
    iowrite32(0x9a00009b, GRETH_1_BASE);
    
//    greth_start_receive( RX_GRETH_BASE, true );
//    greth_start_transmit( TX_GRETH_BASE );

    TEST_ASSERT(greth_wait_receive_irq(GRETH_0_BASE, eth_handled_flag_ptr), "Receiving is failed\n");
}

/*
 * test_o32t_2_greth_system
 */
int main(void)
{
    struct rumboot_irq_entry *tbl;
    uint32_t i=0;
    
    iowrite32( 0xffffffff, GRETH_0_BASE + STATUS);
    rumboot_printf("STATUS: 0x%x\n", ioread32(GRETH_0_BASE + STATUS));
    
    iowrite32( 0xffffffff, GRETH_1_BASE + STATUS);
    rumboot_printf("STATUS: 0x%x\n", ioread32(GRETH_1_BASE + STATUS));
            
    rumboot_printf("Start test_o32t_2_greth_system. Transmit/receive checks\n");
    test_event_send_test_id("test_o32t_2_greth_system");
    tbl = create_greth01_irq_handlers();
    prepare_test_data();
    
    #ifndef GRETH_CMAKE_O32T
        test_event(EVENT_CHECK_RUN_HPROT_MONITOR);//checking switch u_nic400_oi10_axi32.hprot_eth_1(0)_s from 0x3 to 0xF (by request from JIRA-78)
    #endif
    dcr_write(DCR_SCTL_BASE + SCTL_IFSYS_ETH_HPROT, 0x3F3F3F3F);
        
    start_transfer_wait_irq();
     
    rumboot_printf("Start checks rx_0\n");
    for (i = 0; i < N_DESC; i++){
        TEST_ASSERT(memcmp(test_data_src, test_data_dst_0[i], GRETH_TEST_DATA_LEN_BYTES)==0, "Data compare error!\n");
    }
    rumboot_printf("Start checks rx_1\n");
    for (i = 0; i < N_DESC; i++){
        TEST_ASSERT(memcmp(test_data_src, test_data_dst_1[i], GRETH_TEST_DATA_LEN_BYTES)==0, "Data compare error!\n");
    }
    rumboot_printf("Finish checks\n");

    rumboot_free(tx0_descriptor_data_);
    rumboot_free(rx0_descriptor_data_);
    rumboot_free(tx1_descriptor_data_);
    rumboot_free(rx1_descriptor_data_);
    
    #ifndef GRETH_CMAKE_O32T
        test_event(EVENT_CHECK_STOP_HPROT_MONITOR);
    #endif
    delete_greth01_irq_handlers(tbl);
    return 0;
}
