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

int load_data_for_pooling_array (  uint32_t len, int16_t* dump_ptr[]) {

	uint16_t i;
	uint16_t data;
	
	for (i = 0; i< len ; i++){
	data = i; //rand;	
	dump_ptr[i] =  data ;	
	
	//rumboot_printf(" address_data=0x%x\n", &dump_ptr[i]);
	rumboot_printf(" input data=0x%x\n", (dump_ptr[i]));
	
	
	}
	return 0;		
}

int load_etalon_array (  uint32_t len, int16_t* dump_ptr[]) {  

	uint32_t i;
	uint16_t etalon;
	uint16_t data[len-1];
    uint32_t pooling_mode;
	
	pooling_mode = 0x00000100 & ioread32(DEMONSTRATOR_APB_BASE + NA_ACTS);

  if (pooling_mode ==0){
	for (i = 0; i< len ; i++){
		
		data[i] =i;	//rand;
		data[i+1] =i+1;	//rand;
		data[i+2] =i+2;	//rand;
		data[i+3] =i+3;	//rand;
		
		if ((i % 4) ==0) 
		etalon = (data[i] + data[i+1] +  data[i+2] + data[i+3]) /2;
		if	((i % 4) !=0)		
		etalon = 0;	
		dump_ptr[i] = etalon;
		rumboot_printf(" i=0x%x\n", i);
		rumboot_printf(" etalon=0x%x\n", etalon);
	}
  }	
 else if	(pooling_mode !=0) {
	for (i = 0; i< len ; i++){
	
		data[i] = i;	//rand;
		data[i+1] =i+1;	//rand;
		data[i+2] =i+2; //rand;
		data[i+3] =i+3; //rand;
				
		if  (((i % 4) ==0)&(data[i]>= data[i+1]) & (data[i] >= data[i+2]) & (data[i] >= data[i+3]))
		etalon = i;		
		if (((i % 4) ==0)&(data[i+1] >=data[i]) & (data[i+1] >= data[i+2]) & (data[i+1] >= data[i+3]))
		etalon = i+1;		
		if ( ((i % 4) ==0)&(data[i+2] >=data[i+1]) & (data[i+2] >=data[i+3]) & (data[i+2] >=data[i]))		
		etalon = i+2;
		if (( (i % 4) ==0) &(data[i+3] >=data[i]) & (data[i+3] >= data[i+2]) & (data[i+3] >= data[i+1]))
	    etalon =i+3;
		
if	((i % 4) !=0)		
	etalon = 0;	

	dump_ptr[i] = etalon;	//data[i];

	rumboot_printf(" i=0x%x\n", i);
	rumboot_printf(" etalon=0x%x\n", etalon);
		}
	}
	return 0;	
} 


int pooling_compare_data(int16_t* res, int16_t* src_etalon, uint32_t size)
{
  uint32_t pooling_mode;
  pooling_mode = 0x00000100 & ioread32(DEMONSTRATOR_APB_BASE + NA_ACTS);
  
  for (int i=0; i<size; i++)
  {
  if	(((res[i] != (src_etalon[2*i] >> 3))& ( pooling_mode ==0)) |
		((res[i] != (src_etalon[2*i] >> 1))& ( pooling_mode !=0)))
    {
    rumboot_printf("read_in_data 0x%x\n",   res[i]);
	rumboot_printf("read_in_etal 0x%x\n",   src_etalon[i]);
	
	rumboot_printf("read_etalon1 0x%x\n", (src_etalon[2*i] >> 1)); //to fix
	rumboot_printf("read_etalon2 0x%x\n", (src_etalon[2*i] >> 3) ); //to fix
	
	rumboot_printf("adr_data 0x%x\n",  &res[2*i]);
	rumboot_printf("adr_etal 0x%x\n",  &src_etalon[2*i]);
	//rumboot_printf("adr_etal 0x%x\n",  &src_etalon[i]);
    rumboot_printf("i=0x%x\n", i);
    rumboot_printf("POOLING test ERROR!\n");
 return 1;
    }

  }
  return 0;
}




int main() {
  uint32_t src_vectors = 2;//32;
  uint32_t src_data_array_size = src_vectors * 64 * 2; // Each Vector Has 64 int16
  //uint32_t dst_data_array_size = src_vectors * 64 * 2; // Each Vector Has 16 int64
  int ret;
  uint32_t pooling_mode =0x1;
  int16_t* src_data;
  int16_t* src_etalon;
  int16_t* dst_data;

 
  rumboot_printf("TEST pooling start\n");

  src_data = rumboot_malloc_from_heap_aligned(1, src_data_array_size, 16);
  dst_data = rumboot_malloc_from_heap_aligned(1, src_data_array_size, 16);
  src_etalon   = rumboot_malloc_from_heap_aligned(1, src_data_array_size, 16);


   rumboot_printf("src address 0x%x\n",(src_data));
   rumboot_printf("dst address 0x%x\n",(dst_data));
   rumboot_printf("etalon address 0x%x\n",(src_etalon));
   rumboot_printf("pooling_mode= 0x%x\n",pooling_mode);
  if(src_data == NULL ||  dst_data == NULL || src_etalon == NULL)
  {
    rumboot_printf("Malloc failed\n");
    return 1;
  } 
  
  ret=load_data_for_pooling_array(src_data_array_size/2,  src_data);
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
   
	ret = load_etalon_array(src_data_array_size/2, src_etalon);
	if(ret!=0){
    rumboot_printf("ERROR loading etalon\n");
		return 1;
  }
 
	rumboot_printf(" compare demonstrator results\n");	
	ret = pooling_compare_data((int16_t*)dst_data,(int16_t*)src_etalon,(src_data_array_size/2));	
	if (ret) {
	rumboot_printf(" compare pooling result ERROR\n");
	rumboot_printf("TEST pooling operation FAILED\n");
	return 1;
	}
	else
	if (ret==0){ 
  rumboot_printf("TEST pooling operation PASSED\n");
  return 0;
	}
}