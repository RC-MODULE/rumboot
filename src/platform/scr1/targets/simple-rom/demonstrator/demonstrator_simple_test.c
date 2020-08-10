// Test: simple-test on connect demonstrator in scr1-system 

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator
#include <regs/regs_mdma.h> // regs names demonstrator

#include <devices/mdma_simple.h> // lib for work with mdma

#include <assert.h>

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) rumboot_printf("qspi: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


int main() {
  rumboot_printf("TEST simple-test start\n");

  uint32_t data_size = 128;

  void *addr_ext;
  addr_ext = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  assert (addr_ext != NULL);

  // rumboot_platform_request_file("input_file", (uint32_t)addr_ext); // что-то странное происxодит
  iowrite32(0x01abcdef, (uint32_t)addr_ext);
  dbg("addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext));
  
  void *addr_dmem;
  addr_dmem = DATA_BASE;
  assert (!simple_mdma_exec(1, MDMA_BASE, addr_ext, addr_dmem, data_size));

  void *addr_weigth;
  addr_weigth = WEIGHT_BASE;
  assert (!simple_mdma_exec(1, MDMA_BASE, addr_dmem, addr_weigth, data_size));
  
  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}