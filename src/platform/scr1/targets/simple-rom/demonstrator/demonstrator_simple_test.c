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


int main() {
  rumboot_printf("TEST simple-test start\n");

  void *addr_ext;
  void *addr_dmem;

  uint32_t data_size = 32;

  addr_ext = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  if(addr_ext == NULL)
    return 1;

  // addr_ext = EXT_MEMORY_BASE;
  addr_dmem = DATA_BASE;


  // rumboot_platform_request_file("myfile", (uint32_t)addr_src);
  
  iowrite32(0xabcd, (uint32_t)addr_ext);
  rumboot_printf("addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext));
  
  // void *addr;
  // addr = MDMA_BASE + (uint32_t)addr_ext;
  // rumboot_printf("addr=%x\n", (uint32_t)addr);

  uint32_t settings_r = ioread32(MDMA_BASE + MDMA_SETTINGS_R);
  settings_r = (settings_r | 0xffff0000) & (16 << 16);
  iowrite32(settings_r, MDMA_BASE + MDMA_SETTINGS_R);

  uint32_t settings_w = ioread32(MDMA_BASE + MDMA_SETTINGS_W);
  settings_w = (settings_w & 0x0000ffff) | (16 << 16); // так лучше
  iowrite32(settings_w, MDMA_BASE + MDMA_SETTINGS_W);


  int res;
  res = simple_mdma_exec(1, MDMA_BASE, addr_ext, addr_dmem, data_size);
  rumboot_printf("res=%d\n", res);
  
  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}