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

#define BUF_SIZE 1024
#define TIMEOUT (BUF_SIZE*50)
#define WORD_SIZE 8
#ifndef BASE_FREQ
    #define BASE_FREQ 50000
#endif

uint32_t* tx_buf;
uint32_t* rx_buf;
struct rcm_cp_instance c0; 
struct rcm_cp_instance c1; 


int main()
{
  int r;
  int size;
  int timeout_us;
  rumboot_printf("Commport 2nd ES test\n");
  
  size = BUF_SIZE;
  timeout_us = TIMEOUT;
  
  cp_instance_init(&c0, COM0_BASE, BASE_FREQ);
  cp_instance_init(&c1, COM1_BASE, BASE_FREQ);
  r = cp_set_speed(&c0,BASE_FREQ);
  r = cp_set_speed(&c1,BASE_FREQ);

  rumboot_printf("Initialized instances 0x%x 0x%x\n",c0.base,c1.base);
  
  //////////////////////////////////////////
  // Test 1 - Simple Transmit And Receive
  tx_buf = rumboot_malloc_from_named_heap_aligned("IM1", size, 8);
  rx_buf = rumboot_malloc_from_named_heap_aligned("IM1", size, 8);
  
  if(tx_buf==NULL || rx_buf==NULL) return -1;
  
  rumboot_printf("Allocated tx_buf=0x%x, rx_buf=0x%x\n",(uint32_t) tx_buf,(uint32_t) rx_buf);
  
  memset(rx_buf,0xA5,size);
  rumboot_memfill32(tx_buf,size/4,0x00010203,0x04040404);
  
  cp_start_tx(&c0, tx_buf, size);
  cp_start_rx(&c1, rx_buf, size);
  
  r = cp_wait_rx(c1.base,timeout_us);
  if(r != 0) {
    rumboot_printf("Test 1 FAILED, r = %d\n",r);
    return 1;
  }
  r = cp_wait_tx(c0.base, timeout_us);
  
  if(r != 0) {
    rumboot_printf("Test 1 FAILED, r = %d\n",r);
    return 1;
  }
  
  
  if(memcmp(tx_buf,rx_buf,size)==0) {
    rumboot_printf("Test 1 Passed\n");
  }else{
    rumboot_printf("Test 1 FAILED\n");
    return 2;
  }
  
  rumboot_free(tx_buf);
  rumboot_free(rx_buf);
  
  
  ///////////////////////////////////////////////////////
  // Test 2 - Wrong Rx Address
  tx_buf = rumboot_malloc_from_named_heap_aligned("IM1", size, 8);
  rx_buf = (uint32_t*) COM0_BASE; // This Is Just Some Address That Should Not Be Accessible By Commport
  
  rumboot_memfill32(tx_buf,size/4,0x00010203,0x04040404);
  
  cp_start_rx(&c1,rx_buf,size);
  cp_start_tx(&c0,tx_buf,size);
  
  r = cp_wait_rx(c1.base,timeout_us);
  if(r == 0 || r < 0) {
    rumboot_printf("Test 2 FAILED, r = %d\n",r);
    return 3;
  }
  
  r = cp_elaborate_rx_exception(c1.base,r,timeout_us);
  if(r != 0) {
    rumboot_printf("Test 2 FAILED, r = %d\n",r);
    return 4;
  }
  
  r = cp_wait_tx(c0.base, timeout_us);
  if(r != 0) {
    rumboot_printf("Test 2 FAILED, r = %d\n",r);
    return 5;
  }
  
  rumboot_free(tx_buf);
  
  rumboot_printf("Test 2 Passed\n");
  
  ///////////////////////////////////////////////////////
  // Test 3 - Wrong Rx Address - Single Word
  
  tx_buf = rumboot_malloc_from_named_heap_aligned("IM1", WORD_SIZE, 8);
  rx_buf = (uint32_t*) COM0_BASE; // This Is Just Some Address That Should Not Be Accessible By Commport
  
  rumboot_memfill32(tx_buf,WORD_SIZE/4,0x03020100,0x04040404);
  
  cp_start_rx(&c1,rx_buf,WORD_SIZE);
  cp_start_tx(&c0,tx_buf,WORD_SIZE);
  
  r = cp_wait_rx(c1.base,WORD_SIZE*50);
  if(r == 0 || r < 0) {
    rumboot_printf("Test 3 FAILED, r = %d\n",r);
    return 6;
  }
  
  r = cp_elaborate_rx_exception(c1.base,r,WORD_SIZE*50);
  if(r != 0) {
    rumboot_printf("Test 3 FAILED, r = %d\n",r);
    return 7;
  }
  
  r = cp_wait_tx(c0.base, WORD_SIZE * 50);
  if(r != 0) {
    rumboot_printf("Test 3 FAILED, r = %d\n",r);
    return 8;
  }
  
  rumboot_free(tx_buf);
  
  
  
  rumboot_printf("Test 3 Passed\n");
  
  ///////////////////////////////////////////////////////
  // Test 4 - Single Word Without Errors
  
  tx_buf = rumboot_malloc_from_named_heap_aligned("IM1", WORD_SIZE, 8);
  rx_buf = rumboot_malloc_from_named_heap_aligned("IM1", WORD_SIZE, 8);
  
  rumboot_memfill32(tx_buf,WORD_SIZE/4,0x03020100,0x04040404);
  memset(rx_buf,0xA5,WORD_SIZE);
  
  cp_start_rx(&c1,rx_buf,WORD_SIZE);
  cp_start_tx(&c0,tx_buf,WORD_SIZE);
  
  r = cp_wait_rx(c1.base,0);
  if(r != 0) {
    rumboot_printf("Test 4 FAILED, Rx, r = %d\n",r);
    return 9;
  }
  
  r = cp_wait_tx(c0.base,0);
  if(r != 0) {
    rumboot_printf("Test 4 FAILED, Tx, r = %d\n",r);
    return 10;
  }
  
  if(memcmp(tx_buf,rx_buf,WORD_SIZE)==0) {
    rumboot_printf("Test 4 Passed\n");
  }else{
    rumboot_printf("Test 4 FAILED\n");
    return 11;
  }
  
  rumboot_free(tx_buf);
  rumboot_free(rx_buf);
  
  ///////////////////////////////////////////////////////
  // Test 5 - Wrong Tx Address
  
  tx_buf = (uint32_t*) COM0_BASE; // Wrong Address
  rx_buf = rumboot_malloc_from_named_heap_aligned("IM1", size, 8);
  
  memset(rx_buf,0xA5,size);
  
  cp_start_tx(&c0, tx_buf, size);
  cp_start_rx(&c1, rx_buf, size);
  
  r = cp_wait_tx(c0.base, timeout_us);
  
  if(r <= 0) {
    rumboot_printf("Test 5 FAILED, r = %d\n",r);
    return 12;
  }
  
  cp_elaborate_tx_exception(c0.base);
  cp_abort_rx(c1.base);
  
  tx_buf = rumboot_malloc_from_named_heap_aligned("IM1", size, 8); // Allocate Correct Buffer
  rumboot_memfill32(tx_buf,size/4,0x00010203,0x04040404);
  
  cp_start_tx(&c0, tx_buf, size);  // Restart 
  cp_start_rx(&c1, rx_buf, size);
  
  r = cp_wait_tx(c0.base, timeout_us);
  
  if(r != 0) {
    rumboot_printf("Test 5 FAILED (2nd stage Tx), r = %d\n",r);
    return 13;
  }
  
  r = cp_wait_rx(c1.base, timeout_us);
  
  if(r != 0) {
    rumboot_printf("Test 5 FAILED (2nd stage Rx), r = %d\n",r);
    return 15;
  }
  
  if(memcmp(tx_buf,rx_buf,size)==0) {
    rumboot_printf("Test 5 Passed\n");
  }else{
    rumboot_printf("Test 5 FAILED\n");
    return 15;
  }
  
  rumboot_free(tx_buf);
  rumboot_free(rx_buf);
  
  
  return 0;
}
