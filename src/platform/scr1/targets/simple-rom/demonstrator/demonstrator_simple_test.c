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
  #define dbg(x, ...) rumboot_printf("test_dem: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


int main() {
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