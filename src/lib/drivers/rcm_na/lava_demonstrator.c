
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator
#include <regs/regs_mdma.h> 


#include <devices/mdma_simple.h> // lib for work with mdma
#include <platform/devices/lava_demonstrator.h> // lib for work with demonstrator
#include <assert.h>

int load_demonstrator_data (int heap_id, void* addr_src, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, DEMONSTRATOR_MDMA_BASE, addr_src, (void*)DATA_BASE, data_size);
}

int load_demonstrator_weights (int heap_id,  void* addr_src,   uint32_t data_size)
{
  return simple_mdma_exec(heap_id, DEMONSTRATOR_MDMA_BASE,  addr_src, (void*)WEIGHT_BASE, data_size);
}

int load_demonstrator_result (int heap_id, void* addr_src,   uint32_t data_size)
{
  return simple_mdma_exec(heap_id, DEMONSTRATOR_MDMA_BASE, (void*)RESULT_BASE, addr_src, data_size); 
}

int demonstrator_run_matrix_flow(uintptr_t matrix_base, uint32_t data_size, uint32_t weight_size)
{
  int cnt; 
	int DEMONSTRATOR_ATTEMPT = 0x01000;
  uint32_t tmp;
  
	rumboot_printf("set demonstrator registers\n");
	
	tmp = (ioread32(matrix_base + NA_MXID));
	if (tmp != 0x4D545800) {
           rumboot_printf("Wrong MXID\n");
            return 1;		
		}
  
	iowrite32(0,	(matrix_base + NA_DCNT));      		// set initial memory address for data
	iowrite32(0,	(matrix_base + NA_WCNT));   		// set initial memory address for weights
	iowrite32(0,	(matrix_base + NA_RCNT));    		// set initial memory address for the result
	iowrite32(data_size,	(matrix_base + NA_FADR));    		// data size
	iowrite32(0x1,	(matrix_base + NA_STGS));    		//  Opcode

	iowrite32(1,(matrix_base + NA_PMEN));    			// MTX_PMEN -run load weights to memory
	rumboot_printf(" demonstrator runs to load weights\n");
	cnt = DEMONSTRATOR_ATTEMPT  ;
	do {
    tmp = 0x00000001 & (ioread32(matrix_base + NA_PUMP));	//wait end of loading weights	
    --cnt;
	} while (tmp != 0x00000000 && cnt != 0);  // CHECK The Doc About NA_PUMP!!!
	
	if (cnt == 0) {
           rumboot_printf("No end exchange!\n");
            return 1;		
		}
	rumboot_printf(" load weights from shadow registers\n");	
	iowrite32(0x1,(matrix_base + NA_WCHG)); 
	
	rumboot_printf(" demonstrator runs matrix\n");
	iowrite32(1,(matrix_base +NA_ENAB));    			// run multiplication on matrix	
	
	cnt = DEMONSTRATOR_ATTEMPT  ;
		
	do {
    tmp = 0x00000001 & (ioread32(matrix_base + NA_COMP));	//wait the end of multiplication	
    --cnt;
	} while  ( tmp!=0x00000001 && cnt!=0 ) ;
	if (cnt == 0)  {
           rumboot_printf("No end multiplication!\n");
            return 1;
  }
	rumboot_printf("CYCLE_COUNT=%d\n",ioread32(matrix_base + NA_CNTT)); // the number of cycles that matrix runs
	rumboot_printf("CYCLE_end=%d\n",ioread32(matrix_base + NA_COMP));   // multiplication is ended
}

int get_demonstrator_matrix_last_duration(uintptr_t matrix_base)
{
  return ioread32(matrix_base + NA_CNTT);
}

int read_demonstrator_matrix_etalon(void * etalon, uint32_t etalon_size)
{
  rumboot_platform_request_file("matrix_etalon",(uint32_t) etalon); // This Rumboot Function Does Not Use Size // And uses uint32_t!!!
  return 0;
}

bool compare_demonstrator_result_64bit(int64_t* res, int64_t* etalon, uint32_t size)
{
  rumboot_printf("--comparing at 0x%x vs at 0x%x\n", (uint32_t)res, (uint32_t)etalon);
  for (int i=0; i<size; i++)
  {
    if(res[i] != etalon[i])
    {
      rumboot_printf("READ_etalon_DATA=0x%x at 0x%x\n", etalon[i], (uint32_t)etalon);
      rumboot_printf("READ_DATA=0x%x at 0x%x\n", res[i], (uint32_t)res);
      rumboot_printf("i=0x%x\n", i);
      rumboot_printf("MATRIX test ERROR!\n");
      return false;
    }
    //res++;etalon++;
  }
  return true;
}


int demonstrator_load_coefficients (int heap_id, void *addr_src, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, DEMONSTRATOR_MDMA_BASE, addr_src, COEFFICIENT_BASE, data_size);
}


int unload_demonstrator_results (int heap_id, void *addr_dst, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, DEMONSTRATOR_MDMA_BASE, RESULT_BASE, addr_dst, data_size);
}


int demonstrator_run_vec_flow (
  uintptr_t matrix_base, 
  uint32_t data_size, 
  Vec_op op,
  Relu_op relu_on,
  uint32_t data_offset,
  uint32_t result_offset
) 
{
  int cnt; 
  int DEMONSTRATOR_ATTEMPT = 0x01000;
  uint32_t tmp;
  
  rumboot_printf("set demonstrator registers\n");
  
  tmp = (ioread32(matrix_base + NA_MXID));
  if (tmp != 0x4D545800) {
    rumboot_printf("Wrong MXID\n");
    return 1;   
  }
  
  iowrite32(0x100, matrix_base + NA_STGS); // vec op
  
  uint32_t na_acts = (relu_on << 4) |
                    ((op == PLUS) 
                    ? 0x1 
                    : ((op == MIN)
                      ? 0x2
                      : 0x3));

  iowrite32(na_acts, matrix_base + NA_ACTS);

  iowrite32(data_offset, matrix_base + NA_DCNT);
  iowrite32(data_size, matrix_base + NA_FADR);
  iowrite32(result_offset, matrix_base + NA_RCNT);
  iowrite32(0x1, matrix_base + NA_ENAB);

  cnt = DEMONSTRATOR_ATTEMPT;
  do {
    tmp = 0x00000001 & (ioread32(matrix_base + NA_COMP)); //wait the end of multiplication  
    --cnt;
  } while ( tmp != 0x00000001 && cnt != 0 ) ;
  if (cnt == 0)  {
    rumboot_printf("No end multiplication!\n");
    return 1;
  }

  rumboot_printf("CYCLE_COUNT=%d\n", ioread32(matrix_base + NA_CNTT)); // the number of cycles that matrix runs
  rumboot_printf("CYCLE_end=%d\n", ioread32(matrix_base + NA_COMP));   // multiplication is ended

  iowrite32(0x1, matrix_base + NA_CRST);

  return 0;
}

int demonstrator_run_pooling_flow(uintptr_t matrix_base, uint32_t data_size, Pool_op pooling_mode)
{
  uint32_t tmp;
  int DEMONSTRATOR_ATTEMPT = 0x01000;
  int cnt = DEMONSTRATOR_ATTEMPT; 
  
  rumboot_printf("set demonstrator registers\n");
  
  tmp = (ioread32(matrix_base + NA_MXID));
  if (tmp != 0x4D545800) {
    rumboot_printf("Wrong MXID got 0x%x\n", tmp);
    return 1;   
  }
  iowrite32(0x1000, matrix_base + NA_STGS); 		// pooling op
  if (pooling_mode == POOL_MAX) {
  iowrite32(0x100, matrix_base + NA_ACTS); 			// pooling op max value
  }
  else { // pooling_mode == POOL_AVE
  iowrite32(0x0, matrix_base + NA_ACTS); 			// pooling op middle value	
  }
  iowrite32(0x0, matrix_base + NA_DCNT);  			// set initial memory address for data
  iowrite32(data_size, matrix_base + NA_FADR);		// data size
  iowrite32(0x0, matrix_base + NA_RCNT);			// set initial memory address for the result
  iowrite32(0x1, matrix_base + NA_ENAB);			// run pooling on NA_PPE	
  
  do {
    tmp = 0x00000001 & (ioread32(matrix_base + NA_COMP)); //wait the end of pooling  
    --cnt;
  } while  ( tmp!=0x00000001 && cnt!=0 ) ;
  if (cnt == 0)  {
    rumboot_printf("No end pooling!\n");
    return 1;
  }
rumboot_printf("CYCLE_COUNT=%d\n",ioread32(matrix_base + NA_CNTT)); // the number of cycles that pooling runs
rumboot_printf("CYCLE_end=%d\n",ioread32(matrix_base + NA_COMP));   // pooling is ended
 return 0;
}
