#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/testsuite.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_muart.h>
#include <devices/muart.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>
#include <regs/regs_gp_timers.h>
#include <devices/gp_timers.h>


static uint32_t BASE[] = {UART0_BASE, UART1_BASE};

static  bool LES(uint32_t al,uint32_t ah,uint32_t bl,uint32_t bh)
{  // A < B
    if(ah < bh) return true;
    else if(ah > bh) return false;
    else if (al < bl) return true;
    else return false;
}
static  bool MORE(uint32_t al,uint32_t ah,uint32_t bl,uint32_t bh)
{  // A > B
    if(ah > bh) return true;
    else if(ah < bh) return false;
    else if (al > bl) return true;
    else return false;
}

static void set_muart(uint32_t base)
{
    struct muart_conf conf;
       conf.dum = 1;
       conf.wlen = WL_8;
       conf.stp2 = STP1;
       conf.is_even = 0;
       conf.is_parity = 1;
       conf.mode = RS_422;
       conf.is_loopback = 0;       //is_loopback
       conf.baud_rate = 12500000;  // bod/s (bdiv=4)
       conf.is_dma = 1;            // Using MDMA

    muart_soft_reset(base);
    muart_init(base, &conf);
}

uint32_t main()
{
    struct mdma_chan *chan_wr;
    struct mdma_chan *chan_rd;
    struct mdma_trans *trans;
    struct mdma_group *group;
    struct mdma_node *node;

    int size = 0x16;
    char * wr_addr = NULL, *rd_addr = NULL;
    int heap_id = 0;
    uint32_t gp_time_L_B, gp_time_H_B, gp_time_L_E, gp_time_H_E;
    uint32_t free_run_H_rd, free_run_L_rd, free_run_H_wr, free_run_L_wr;
    void * desc_rd;
    void *desc_wr;
    int desc_sz_rd, desc_sz_wr;
    bool res_b, res_e;

    rumboot_printf("Test 1.3 MUART AXI transmit and free_run_timer\n");

    if((wr_addr=(char *)rumboot_malloc_from_heap_aligned(heap_id,size+4,ALIGN4)) == NULL){
       rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
       return 2;
    }
    if((rd_addr = (char *)rumboot_malloc_from_heap_aligned(heap_id,size+4,ALIGN4)) == NULL){
         rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
         return 2;
    }
    //Set  MUART
    for (int b=0; b<= 1; b++) {
        set_muart(BASE[b]);
        muart_enable(BASE[b]);
        muart_dump(BASE[b]);
    }
    // Create DMA channel for read.  dma --> uart
    rumboot_printf("======= Create DMA channel for read.  dma --> uart\n");
    chan_rd = muart_set_chan_mdma_R(BASE[0], (void *)rd_addr, size);
    if (chan_rd == NULL) {
           rumboot_printf("UNTESTED: ERROR muart set channel rear\n");
           rumboot_free(wr_addr);
           rumboot_free(rd_addr);
           return 1;
    }
    // Create DMA channel for write.  uart --> dma
    rumboot_printf("======= Create DMA channel for write.  uart --> dma\n");
    chan_wr = muart_set_chan_mdma_W(BASE[1], (void *)wr_addr, size);
    if (chan_wr == NULL) {
        rumboot_printf("UNTESTED: ERROR muart set channel write\n");
        rumboot_free(wr_addr);
        rumboot_free(rd_addr);
        return 1;
    }

     // STAMP
//    iowrite32(0x1,(GLOBAL_TIMERS  + ENABLE));
//    iowrite32(0x1,(GLOBAL_TIMERS  + FIX_CMD));
    gp_timer_turn_on();
    // save of the beginning I/O time
    gp_timer_fix_value();
    gp_time_L_B=gp_timer_get_free_run_l();
    gp_time_H_B=gp_timer_get_free_run_h();
    mdma_chan_start(chan_wr); // uart --> dma
    mdma_chan_start(chan_rd); //dma --> uart
    if (mdma_chan_wait_trans_end(chan_wr, 1000)) {
          rumboot_printf("TEST FAIL: wait_trans_end error\n");
    }
    // save of the ending I/O time
    gp_timer_fix_value();
    gp_time_L_E=gp_timer_get_free_run_l();
    gp_time_H_E=gp_timer_get_free_run_h();

    rumboot_printf("Time of the beginning I/O   ");
    rumboot_printf("gp_time_H_B=0x%x gp_time_L_B=0x%x\n",
                   gp_time_H_B, gp_time_L_B);

    // save free_run_value of read channel
    trans = mdma_chan_get_trans(chan_rd);
    group = mdma_trans_get_group(trans);
    node = mdma_group_get_node(group);
    desc_rd = mdma_node_get_desc(node, &desc_sz_rd);
    free_run_H_rd=mdma_chan_read32((size_t)(desc_rd + sizeof(uint32_t)));
    free_run_L_rd=mdma_chan_read32((size_t)desc_rd);

    rumboot_printf("free_run of chan_rd   ");
    rumboot_printf("free_run_H_rd=0x%x free_run_L_rd=0x%x\n",
                   free_run_H_rd, free_run_L_rd);

    // save free_run_value of write channel
    trans = mdma_chan_get_trans(chan_wr);
    group = mdma_trans_get_group(trans);
    node = mdma_group_get_node(group);
    desc_wr = mdma_node_get_desc(node, &desc_sz_wr);
    free_run_H_wr=mdma_chan_read32((size_t)(desc_wr + sizeof(uint32_t)));
    free_run_L_wr=mdma_chan_read32((size_t)desc_wr);

    rumboot_printf("free_run of chan_wr   ");
    rumboot_printf("free_run_H_wr=0x%x free_run_L_wr=0x%x\n",
                   free_run_H_wr, free_run_L_wr);

    rumboot_printf("Time of the ending I/O   ");
    rumboot_printf("gp_time_H_E=0x%x gp_time_L_E=0x%x\n",
                   gp_time_H_E, gp_time_L_E);

    // Free resource
    rumboot_free(wr_addr);
    rumboot_free(rd_addr);
    mdma_chan_destroy(chan_rd);
    mdma_chan_destroy(chan_wr);
    //Checking
    //Expact: (gp_time_B < free_run_rd) and (gp_time_B < free_run_wr)
    res_b=(LES(gp_time_L_B,gp_time_H_B,free_run_L_rd,free_run_H_rd) &&
            LES(gp_time_L_B,gp_time_H_B,free_run_L_wr,free_run_H_wr));

    //Expact: (gp_time_E > free_run_rd) and (gp_time_E > free_run_wr)
    res_e=(MORE(gp_time_L_E,gp_time_H_E,free_run_L_rd,free_run_H_rd) &&
           MORE(gp_time_L_E,gp_time_H_E,free_run_L_wr,free_run_H_wr));

    rumboot_printf("res_b=%x res_e=%x\n",res_b,res_e);
    if (!res_b)
       rumboot_printf("(gp_time_B >= free_run_rd) or (gp_time_B >= free_run_wr)\n");
    if (!res_e)
        rumboot_printf("(gp_time_E <= free_run_rd) or (gp_time_E <= free_run_wr)\n");
    if ((!res_b) || (!res_e)){
        rumboot_printf("TEST FAIL\n");
        return 1;
    }else
        rumboot_printf("TEST PASS\n");
    return 0;
}

