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
#include <devices/lava_demonstrator.h> // lib for work with demonstrator
#include <assert.h>

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) rumboot_printf("qspi: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


int main() {
 // rumboot_printf("TEST simple-test start\n");
  
  
/*
  uint32_t data_size = 128;
  uint32_t weight_size=128;

  void *addr_ext;
  void *addr_ext_w;
  addr_ext = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  addr_ext_w=rumboot_malloc_from_heap_aligned(1,weight_size,16);
  assert (addr_ext != NULL && addr_ext_w != NULL);

  // rumboot_platform_request_file("input_file", (uint32_t)addr_ext); // что-то странное происxодит
  iowrite32(0x01abcdef, (uint32_t)addr_ext);
  iowrite32(0x98765432, (uint32_t)addr_ext_w);
  dbg("addr_ext=%x, data_ext=%x\n", (uint32_t)addr_ext, ioread32((uint32_t)addr_ext));
  dbg("addr_ext_w=%x, data_ext_w=%x\n", (uint32_t)addr_ext_w, ioread32((uint32_t)addr_ext_w));
  
  assert (! load_demonstrator_data    (1, addr_ext  , data_size) );
  assert (! load_demonstrator_weights (1, addr_ext_w, data_size) );
 */ 
 // IMPORTANT!!!
  // Через input_file должен появиться input/input_rtl_data_X.bin, в кт реф-я модель что-то запишет
  // rumboot_platform_request_file("input_file", (uint32_t)addr_ext); // что-то странное происxодит
 
 uint32_t res_size = 32;//65536; byte or 2 x 256 x 64 x 16 bit  //temporal
 uint32_t data = 32; //!!!!! temporal
 int ret;
 
 rumboot_printf("TEST matrix_multiplication start\n");

  ret = run_matrix(); 
  if (ret) {
	  rumboot_printf(" multiplication TEST_ERROR\n");
	  return 1;
  }
 
	uintptr_t src_etalon =(uintptr_t) rumboot_malloc_from_heap_aligned(1, sizeof(src_etalon), 16);
	uintptr_t addr_ext_r =(uintptr_t) rumboot_malloc_from_heap_aligned(1, sizeof(addr_ext_r), 16);
	
    rumboot_printf("adr_etalon=0x%x\n",src_etalon);
	
 //-----------temporal array---------------- 
	load_etalon_array(res_size, src_etalon, data ); //reduced temporally!!
	rumboot_printf("RESULT_BASE=0x%x\n",RESULT_BASE);
	rumboot_printf("MDMA_BASE=0x%x\n",MDMA_BASE);
	rumboot_printf("RESULT_ARRAY_from_source=0x%x\n",RESULT_BASE);
	rumboot_printf("addr_ext_r=0x%x\n",addr_ext_r);
	rumboot_printf("src_etalon=0x%x\n",src_etalon);
	rumboot_printf("data_etalon=0x%x\n",(ioread32(src_etalon)));
	rumboot_printf("sizeof(src_etalon)=0x%x\n",sizeof(src_etalon));
	rumboot_printf("sizeof(addr_ext_r)=0x%x\n",sizeof(addr_ext_r));
//------------------------------------------
  
	
	ret = load_demonstrator_result(1,addr_ext_r,/*res_size*/256 );  //temporal size !!!
	if (ret) {
	  rumboot_printf(" load multiplication result TEST_ERROR\n");
	  return 1;
 }
  
  
   rumboot_printf(" demonstrator result array was loaded by DMA\n");
	ret = demonstrator_compare_data(src_etalon,   addr_ext_r,(res_size/4 )); 
	if (ret) {
	  rumboot_printf(" compare multiplication result TEST_ERROR\n");

	  return 0;
  }
 
  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}