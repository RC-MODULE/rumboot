#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1

#include <devices/mdma_simple.h> // lib for work with mdma
#include <platform/devices/lava_demonstrator.h> // lib for work with demonstrator
#include <assert.h>
#include <regs/regs_demonstrator.h> // regs names demonstrator
#include <regs/regs_mdma.h> // regs names demonstrator

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) rumboot_printf("test_dem: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif

typedef struct Vec_result {
  int16_t R0;
  int16_t Zero0;
  int16_t Zero1;
  int16_t Zero2;
} Vec_result;

int load_data_for_pooling_array (  uint32_t len, int16_t* dump_ptr) {

	uint32_t i;
	
	for (i = 0; i< len ; i++){
	
	dump_ptr[i] = (int16_t) i ;	
	
	//rumboot_printf(" address_data=0x%x\n", &dump_ptr[i]);
	rumboot_printf(" input data=0x%x\n", (dump_ptr[i]));
	
	}
	return 0;		
}

int load_etalon_array (  uint32_t len, Vec_result* dump_ptr, Pool_op pooling_mode) {  

	uint32_t i;
	uint32_t data;
	
  if (pooling_mode == POOL_AVE){
	for (i = 0; i< len ; i++){
		
		data = 4*i + (4*i + 1) + (4*i+2) + (4*i +3); // Summ Of 4 Words by Increment As in load_data_for_pooling_array
		data = data/4;                               // Average  (Yeh, Know! It Is 4*i+1)
		dump_ptr[i].R0 = (int16_t) data;
    dump_ptr[i].Zero0=0;
    dump_ptr[i].Zero1=0;
    dump_ptr[i].Zero2=0;
		rumboot_printf(" i=0x%x\n", i);
		rumboot_printf(" etalon=0x%x\n", dump_ptr[i].R0);
	}
  }	
 else if (pooling_mode == POOL_MAX) {
	for (i = 0; i< len ; i++){

		dump_ptr[i].R0 = 4*i+3;
    dump_ptr[i].Zero0=0;
    dump_ptr[i].Zero1=0;
    dump_ptr[i].Zero2=0;

	rumboot_printf(" i=0x%x\n", i);
	rumboot_printf(" etalon=0x%x\n", dump_ptr[i].R0);
		}
	}
	return 0;	
} 



int main() {
  uint32_t src_vectors = 2;//32;
  uint32_t src_data_array_size = src_vectors * 64 * sizeof(int16_t); // Each Vector Has 64 int16
  uint32_t dst_data_array_size = src_data_array_size;
  int ret;
  Pool_op pooling_mode =POOL_AVE;
  int16_t* src_data;
  Vec_result* src_etalon;
  Vec_result* dst_data;

 
  rumboot_printf("TEST pooling start\n");

  src_data = rumboot_malloc_from_heap_aligned(1, src_data_array_size, 16);
  dst_data = rumboot_malloc_from_heap_aligned(1, dst_data_array_size, 16);
  src_etalon   = rumboot_malloc_from_heap_aligned(1, dst_data_array_size, 16);


   rumboot_printf("src address 0x%x\n",(uint32_t)(src_data));
   rumboot_printf("dst address 0x%x\n",(uint32_t)(dst_data));
   rumboot_printf("etalon address 0x%x\n",(uint32_t)(src_etalon));
   rumboot_printf("pooling_mode= 0x%x\n",(uint32_t)pooling_mode);
  if(src_data == NULL ||  dst_data == NULL || src_etalon == NULL)
  {
    rumboot_printf("Malloc failed\n");
    return 1;
  } 
  
  ret=load_data_for_pooling_array(src_data_array_size/sizeof(int16_t),  src_data);
    if(ret!=0){
    rumboot_printf("ERROR loading  source data\n");
    return 1;
  }
  ret=load_demonstrator_data (1, src_data, src_data_array_size);
  if(ret!=0){
    rumboot_printf("ERROR loading data\n");
    return 1;
  }

  ret = demonstrator_run_pooling_flow(DEMONSTRATOR_APB_BASE, src_data_array_size, pooling_mode); 
  if (ret) {
	  rumboot_printf(" Pooling TEST ERROR\n");
	  return 1;
  }

	ret = unload_demonstrator_results (1, dst_data, src_data_array_size);
	if(ret!=0){
    rumboot_printf("ERROR loading results\n");
		return 1;
  }

	rumboot_printf(" demonstrator result array was loaded by DMA\n");
   
	ret = load_etalon_array(src_data_array_size/4, src_etalon, pooling_mode);
	if(ret!=0){
    rumboot_printf("ERROR loading etalon\n");
		return 1;
  }
 
	rumboot_printf(" compare demonstrator results\n");	
	if (!compare_demonstrator_result_64bit((int64_t*) dst_data, (int64_t*) src_etalon, dst_data_array_size/sizeof(int64_t))) {
	rumboot_printf(" compare pooling result ERROR\n");
	rumboot_printf("TEST pooling operation FAILED\n");
	return 1;
	}
  
  rumboot_printf("TEST pooling operation PASSED\n");
  return 0;
}