
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator

#include <devices/mdma_simple.h> // lib for work with mdma

int load_demonstrator_data (int heap_id, void* addr_src, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, MDMA_BASE, addr_src, DATA_BASE, data_size);
}

int load_demonstrator_weights (int heap_id, void* addr_src, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, MDMA_BASE, addr_src, WEIGHT_BASE, data_size);
}




