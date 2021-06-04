#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <devices/rcm_cp.h>
#include <platform/devices.h>
#include <rumboot/rumboot.h>
#include <rumboot/irq.h>

#define BUF_SIZE 1024
#define TIMEOUT (BUF_SIZE*50)
#define WORD_SIZE 8
#ifndef BASE_FREQ
    #define BASE_FREQ 50000
#endif

struct rcm_cp_instance c0; 
struct rcm_cp_instance c1; 

typedef enum cp_irq_handler_status{ 
  CP_IRQ_HANDLER_STATUS_NOT_STARTED,
  CP_IRQ_HANDLER_STATUS_OK,
  CP_IRQ_HANDLER_STATUS_ABORTED
} cp_irq_handler_status;

volatile static cp_irq_handler_status handler_status_tx;
volatile static cp_irq_handler_status handler_status_rx;

volatile static int cp_tx_irq_handler_count;
volatile static int cp_rx_irq_handler_count;


static void handler_tx(int id, void * c_) {
  uintptr_t base;
  int r;
  
  struct rcm_cp_instance* c = (struct rcm_cp_instance *) c_;
  base = c->base;
  
//  rumboot_printf("handler_tx(%x)\n",base);
  
  r = cp_wait_tx(base,0);
  
  if(r != 0) {
    cp_elaborate_tx_exception(base);
    handler_status_tx = CP_IRQ_HANDLER_STATUS_ABORTED;
  }
  else {
    handler_status_tx = CP_IRQ_HANDLER_STATUS_OK;
  }
  
  cp_tx_irq_handler_count++;
}

static void handler_rx(int id, void * c_) {
  uintptr_t base;
  int r;
  
  struct rcm_cp_instance* c = (struct rcm_cp_instance *) c_;
  
  base = c->base;
  
//  rumboot_printf("handler_rx(%x)\n",base);
  
  r = cp_wait_rx(base,0);
  
  if(r != 0) {
    r = cp_elaborate_rx_exception(base,r,0);  // This Will Wait For Full Packet Invlaidation
                                              //  So, It Is Not A Good Idea To Use This Code In A Real Interrupt Handler
    handler_status_rx = CP_IRQ_HANDLER_STATUS_ABORTED;
  }
  else {
    handler_status_rx = CP_IRQ_HANDLER_STATUS_OK;
  }
  
  cp_rx_irq_handler_count++;
}

int test_iteration(
  struct rcm_cp_instance *txer, 
  struct rcm_cp_instance *rxer, 
  int break_txer, 
  int break_rxer,
  char* heap_name,
  int size
    ) {
  int r=0;
  
  int expected_tx_handler_count;
  int expected_rx_handler_count;
  
  void* tx_buf;
  void* rx_buf;
  void* invalid_ptr = (void*) COM0_BASE;// This Is Just Some Address That Should Not Be Accessible By Commport
  
  rumboot_printf("Start Iteration: break_txer=%d, break_rxer=%d, size=%d\n",break_txer,break_rxer,size);
  
  if(break_txer)
    tx_buf = invalid_ptr; 
  else
    tx_buf = rumboot_malloc_from_named_heap_aligned(heap_name,size,8);
  
  if(break_rxer)
    rx_buf = invalid_ptr;
  else
    rx_buf = rumboot_malloc_from_named_heap_aligned(heap_name,size,8);
  
  if(tx_buf==NULL || rx_buf==NULL) {
    rumboot_printf("ERROR: Buffer Allocation Failed, tx_buf=%x rx_buf=%x\n",(uint32_t)tx_buf,(uint32_t)rx_buf);
    r = -1;goto finalize_test_iteration;
  }
  
  rumboot_printf("Allocated tx_buf=0x%x, rx_buf=0x%x\n",(uint32_t) tx_buf,(uint32_t) rx_buf);
  
  if(!break_txer) {
    rumboot_memfill32(tx_buf,size/4,0x00010203,0x04040404);
  }
  
  if(!break_rxer) {
    memset(rx_buf,0xA5,size); // Clean The Place For Rx Data
  }
  
  handler_status_rx = CP_IRQ_HANDLER_STATUS_NOT_STARTED;
  handler_status_tx = CP_IRQ_HANDLER_STATUS_NOT_STARTED;
  expected_rx_handler_count = cp_rx_irq_handler_count+1;
  expected_tx_handler_count = cp_tx_irq_handler_count+1;
  
  cp_start_tx(txer, tx_buf, size);
  cp_start_rx(rxer, rx_buf, size);
  
  while(!(
      // Both Complete
    (handler_status_rx != CP_IRQ_HANDLER_STATUS_NOT_STARTED && handler_status_tx != CP_IRQ_HANDLER_STATUS_NOT_STARTED) ||
      // Tx Broken
    (break_txer && handler_status_tx == CP_IRQ_HANDLER_STATUS_ABORTED)
  ))
  { // Wait The Handler To Touch This Variables
  }
  
  if(break_txer) {
    if(cp_tx_irq_handler_count != expected_tx_handler_count) {
      rumboot_printf("ERROR: expected_tx_handler_count=%d, cp_tx_irq_handler_count=%d (When break_txer)\n",
                             expected_tx_handler_count,    cp_tx_irq_handler_count);
      r = -1;goto finalize_test_iteration;
    }
    
    if(handler_status_rx != CP_IRQ_HANDLER_STATUS_OK) { // Exclude The Case When The Wrong Word Was Single And Transmitted
      cp_abort_rx(rxer->base); // Shut The Receiver Off
    }
    
    r = 0; goto finalize_test_iteration;
  }
  
  if(expected_rx_handler_count != cp_rx_irq_handler_count) {
    rumboot_printf("ERROR: expected_rx_handler_count=%d, cp_rx_irq_handler_count=%d (Mismatch)\n",
                           expected_rx_handler_count,    cp_rx_irq_handler_count);
    r = -1;goto finalize_test_iteration;
  }
  if(expected_tx_handler_count != cp_tx_irq_handler_count) {
    rumboot_printf("ERROR: expected_tx_handler_count=%d, cp_tx_irq_handler_count=%d (Mismatch)\n",
                           expected_tx_handler_count,    cp_tx_irq_handler_count);
    r = -1;goto finalize_test_iteration;
  }
  
  if(break_rxer && handler_status_rx != CP_IRQ_HANDLER_STATUS_ABORTED) {
    rumboot_printf("ERROR: Wrong Address Set For Rx But Handler Does Not Reported ABORT\n");
    r = -1;goto finalize_test_iteration;
  }
  
  if(!break_rxer && handler_status_rx != CP_IRQ_HANDLER_STATUS_OK) {
    rumboot_printf("ERROR: Normal Rx Expected But Handler Reported ABORT\n");
    r = -1;goto finalize_test_iteration;
  }
  
  if(!break_txer && handler_status_tx != CP_IRQ_HANDLER_STATUS_OK) {
    rumboot_printf("ERROR: Normal Tx Expected But Handler Reported ABORT\n");
    r = -1; goto finalize_test_iteration;
  }
  
  
  if(!break_txer && !break_rxer) {
    if(memcmp(tx_buf,rx_buf,size) == 0) {
      rumboot_printf("Iteration Passed\n");
      r = 0;
    }
    else {
      rumboot_printf("ERROR: Data Mismatch\n");
      r = -1;
    }
  }
  
finalize_test_iteration:
  if(!break_txer)
    rumboot_free(tx_buf);
  
  if(!break_rxer)
    rumboot_free(rx_buf);
  
  return r;
}

static struct rumboot_irq_entry * init_irq(struct rcm_cp_instance *c0,struct rcm_cp_instance *c1) {
    rumboot_irq_cli();
    struct rumboot_irq_entry * const tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, CP0_TRM_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_tx, c0);
    rumboot_irq_set_handler( tbl, CP0_RCV_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_rx, c0);
    rumboot_irq_set_handler( tbl, CP1_TRM_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_tx, c1);
    rumboot_irq_set_handler( tbl, CP1_RCV_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_rx, c1);
    rumboot_irq_table_activate( tbl );
        rumboot_irq_enable(CP0_TRM_INT);
        rumboot_irq_enable(CP0_RCV_INT);
        rumboot_irq_enable(CP1_TRM_INT);
        rumboot_irq_enable(CP1_RCV_INT);
    rumboot_irq_sei();
    return tbl;
}

struct rumboot_irq_entry * tbl;

int main()
{
  int r;
  rumboot_printf("Commport 2nd ES test with interrupts\n");
  
  
  cp_instance_init(&c0, COM0_BASE, BASE_FREQ);
  cp_instance_init(&c1, COM1_BASE, BASE_FREQ);
  r = cp_set_speed(&c0,BASE_FREQ);
  r = cp_set_speed(&c1,BASE_FREQ);
  cp_tx_irq_handler_count=0;
  cp_rx_irq_handler_count=0;

  rumboot_printf("Initialized instances 0x%x 0x%x\n",c0.base,c1.base);
  
  tbl = init_irq(&c0,&c1);
  
#define ITERATION_RUN(TXER,RXER,BREAK_TXER,BREAK_RXER,HEAP_NAME,SIZE,INDEX) \
  if(test_iteration(&(TXER),&(RXER),BREAK_TXER,BREAK_RXER,HEAP_NAME,SIZE)!=0) \
    {rumboot_printf("Iteration %d FAILED\n",INDEX);return -1;}
  
  ITERATION_RUN(c0,c1,0,0,"IM1",BUF_SIZE,1) // Test 1 - Simple Transmit And Receive
  ITERATION_RUN(c1,c0,0,1,"IM1",BUF_SIZE,2) // Test 2 - Wrong Rx Address
  ITERATION_RUN(c0,c1,0,1,"IM1",WORD_SIZE,3)// Test 3 - Wrong Rx Address - Single Word
  ITERATION_RUN(c0,c1,0,0,"IM1",WORD_SIZE,4)// Test 4 - Single Word Without Errors
  ITERATION_RUN(c0,c1,1,0,"IM1",BUF_SIZE,5) // Test 5 - Wrong Tx Address
  ITERATION_RUN(c1,c0,0,0,"IM1",BUF_SIZE,6) // Test 6 - Simple Transmit And Receive
  ITERATION_RUN(c1,c0,1,0,"IM1",WORD_SIZE,7)// Test 7 - Wrong Tx Address - Single Word
  ITERATION_RUN(c1,c0,0,0,"IM1",WORD_SIZE,8)// Test 8 - Single Word Without Errors
    
  
  return 0;
}
