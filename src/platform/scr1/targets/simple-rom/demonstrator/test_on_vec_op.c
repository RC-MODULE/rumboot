// Test: test on vector operations in demonstrator

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
  #define dbg(x, ...) rumboot_printf("test_dem: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


int main() {
  rumboot_printf("TEST test_on_vec_op start\n");

  uint32_t data_size = 128;
  uint32_t coefficient_size = 128;

  // load coefficients in Coefficients mem 
  void *addr_ext_coef = rumboot_malloc_from_heap_aligned(1, coefficient_size, 16);
  assert (addr_ext_coef != NULL);

  rumboot_platform_request_file("input_coef", (uint32_t)addr_ext_coef);

  for(int i = 3; i >=0; --i) {
    dbg("addr_ext_coef=%x, data_ext_coef=%x\n", (uint32_t)addr_ext_coef, ioread32((uint32_t)addr_ext_coef + i*sizeof(uint32_t)));
  }

  assert (!load_demonstrator_coefficients(1, addr_ext_coef, coefficient_size));

  // load data in Data mem
  void *addr_ext = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  assert (addr_ext != NULL);

  rumboot_platform_request_file("input_data", (uint32_t)addr_ext);

  for(int i = 3; i >=0; --i) {
    dbg("addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext + i*sizeof(uint32_t)));
  }

  assert (!load_demonstrator_data(1, addr_ext, data_size));

  // execute vec_op
  assert (!demonstrator_run_vec_flow(DEMONSTRATOR_APB_BASE, data_size, coefficient_size));
  

  // pop results
  assert (!unload_demonstrator_results(1, addr_ext, data_size));
  for(int i = 3; i >=0; --i) {
    dbg("res: addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext + i*sizeof(uint32_t)));
  }

  // finish work
  iowrite32(0x1, DEMONSTRATOR_APB_BASE + NA_CRST);

  // compare results from matrix & reference model
  void *addr_etalon = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  assert (addr_etalon != NULL);

  rumboot_platform_request_file("etalon_data", (uint32_t)addr_etalon);

  assert (compare_demonstrator_result_64bit((uint32_t)addr_ext, (uint32_t)addr_etalon, data_size));

  return 0;

}
