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
#include <platform/devices/lava_demonstrator.h> // lib for work with demonstrator

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
  uint32_t weight_size=128;

  void *addr_ext;
  void *addr_ext_w;
  addr_ext = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  addr_ext_w=rumboot_malloc_from_heap_aligned(1,weight_size,16);
  assert (addr_ext != NULL && addr_ext_w != NULL);

  // IMPORTANT!!!
  // Через input_file должен появиться input/input_rtl_data_X.bin, в кт реф-я модель что-то запишет
  // rumboot_platform_request_file("input_file", (uint32_t)addr_ext); // что-то странное происxодит
  iowrite32(0x01abcdef, (uint32_t)addr_ext);
  iowrite32(0x98765432, (uint32_t)addr_ext_w);
  dbg("addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext));
  dbg("addr_ext_w=%x, data_ext_w=%x\n", (uint32_t)addr_ext_w, ioread32((uint32_t)addr_ext_w));
  
  assert (! load_demonstrator_data    (1, addr_ext  , data_size) );
  assert (! load_demonstrator_weights (1, addr_ext_w, data_size) );
  
  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}