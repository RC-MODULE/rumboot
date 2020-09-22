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

int fill_data_array (  uint32_t len, int16_t* dump_ptr) {
	uint32_t i;

	for (i = 0; i< len ; i++){
    dump_ptr[i] = 0x0001;
	}
	return 0;	
} 

int fill_wght_array ( uint32_t len, int16_t* dump_ptr ) {
	uint32_t i;
	
	for (i = 0; i< len ; i++){
    dump_ptr[i] = 0x0001;//rand(); //i; //it might be assigned rand for data and <i> as weights or vice versa

	//rumboot_printf("&dump_ptr[i]=0x%x\n", &dump_ptr[i]);
	//rumboot_printf(" ioread tmp=0x%x\n", tmp);
	//rumboot_printf(" dump_ptr[i]=0x%x\n", dump_ptr[i]);
	//rumboot_printf(" len=0x%x\n", len);
	//rumboot_printf(" i=0x%x\n", i);
	
	}
	return 0;	
} 

int fill_etalon_array (  uint32_t len, int16_t* dump_ptr) {
	uint32_t i;

	for (i = 0; i< len ; i++){
    dump_ptr[i] = 64;
	}
	return 0;	
} 



int main() {
  uint32_t src_vectors = 32;
  uint32_t src_data_array_size = src_vectors * 64 * 2; // Each Vector Has 64 int16
  uint32_t src_wght_array_size = 64 * 16 * 2;          // 64x16 int16
  uint32_t dst_data_array_size = src_vectors * 16 * 8; // Each Vector Has 16 int64
  int ret;
  
  int16_t* src_data;
  int16_t* src_wght;
  int64_t* etalon;
  int64_t* dst_data;
  
 
  rumboot_printf("TEST matrix_multiplication start\n");

  src_data = rumboot_malloc_from_heap_aligned(1, src_data_array_size, 16);
  src_wght = rumboot_malloc_from_heap_aligned(1, src_wght_array_size, 16);
  dst_data = rumboot_malloc_from_heap_aligned(1, dst_data_array_size, 16);
  etalon   = rumboot_malloc_from_heap_aligned(1, dst_data_array_size, 16);
  
  rumboot_printf("src_data at 0x%x\n",(uint32_t)src_data);
  rumboot_printf("src_wght at 0x%x\n",(uint32_t)src_wght);
  rumboot_printf("dst_data at 0x%x\n",(uint32_t)dst_data);
  rumboot_printf("etalon   at 0x%x\n",(uint32_t)etalon);
  
  if(src_data == NULL || src_wght == NULL || dst_data == NULL || etalon == NULL)
  {
    rumboot_printf("Malloc failed\n");
    return 1;
  }
  
  fill_data_array(src_data_array_size/2,  src_data);
  fill_wght_array(src_wght_array_size/2,  src_wght);
  
  
  ret=load_demonstrator_data (1, src_data, src_data_array_size);
  if(ret!=0){
    rumboot_printf("ERROR loading data\n");
    return 1;
  }
  ret=load_demonstrator_weights (1, src_wght, src_wght_array_size);
  if(ret!=0){
    rumboot_printf("ERROR loading weights\n");
    return 1;
  }
  
  ret = demonstrator_run_matrix_flow(DEMONSTRATOR_APB_BASE, src_data_array_size, src_wght_array_size);
  
  if (ret) {
	  rumboot_printf(" multiplication TEST_ERROR\n");
	  return 1;
  }
 
	ret = unload_demonstrator_results (1, dst_data, dst_data_array_size);
  if(ret!=0){
    rumboot_printf("ERROR unloading results\n");
    return 1;
  }
  rumboot_printf(" demonstrator result array was loaded by DMA\n");
  
  //ret = read_demonstrator_matrix_etalon(etalon, dst_data_array_size);
  ret = fill_etalon_array(dst_data_array_size/2, etalon);
  if(ret!=0){
    rumboot_printf("ERROR loading etalon\n");
    return 1;
  }
  
  rumboot_printf(" Comparing.. at 0x%x vs at 0x%x\n",(uint32_t)dst_data,(uint32_t)etalon);
	if (!compare_demonstrator_result_64bit((int64_t*)dst_data, (int64_t*)etalon, dst_data_array_size/8)) {
	  rumboot_printf(" compare multiplication result TEST_ERROR\n");

	  return 1;
  }
 
  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}