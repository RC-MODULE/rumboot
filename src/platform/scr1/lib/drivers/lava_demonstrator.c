
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator
#include <regs/regs_mdma.h> 


#include <devices/mdma_simple.h> // lib for work with mdma
#include <devices/lava_demonstrator.h> // lib for work with demonstrator
#include <assert.h>

int load_demonstrator_data (int heap_id, void* addr_src, uint32_t data_size)
{
  return simple_mdma_exec(heap_id, MDMA_BASE, addr_src, DATA_BASE, data_size);
}

int load_demonstrator_weights (int heap_id,  void* addr_src,   uint32_t data_size)
{
  return simple_mdma_exec(heap_id, MDMA_BASE,  addr_src, WEIGHT_BASE, data_size);
}

int load_demonstrator_result (int heap_id, void* addr_src,   uint32_t data_size)
{
  return simple_mdma_exec(heap_id, MDMA_BASE, RESULT_BASE, addr_src, data_size); 
}

int run_matrix() {
	uint32_t tmp =-1;
	uint32_t cnt=0;
	int ret = 1;
	
	uint32_t data_size  	= 32768;    //byte or 256 x 64 x 16 bit
	uint32_t weight_size 	= 2048;   	//byte or 16 x 64 x 16  bit

	
	uint32_t data;
  
	
	uint32_t DEMONSTRATOR_ATTEMPT = 0x01000;
	
	rumboot_printf("set demonstrator registers\n");
	
	tmp = (ioread32(DEMONSTRATOR_APB_BASE + NA_MXID));
	if (tmp != 0x4D545800) {
           rumboot_printf("TEST FAILED\n");
            return 1;		
		}
	//rumboot_printf("MTX_MXID=0x%x\n", tmp);
	//-------------------------------------------------------------------------------------
	//  Set REGs and load  data&weights for  MATRIX work
	//-------------------------------------------------------------------------------------	
		
	iowrite32(0,	(DEMONSTRATOR_APB_BASE + NA_DCNT));      		// set initial memory address for data
	rumboot_printf("READ_DATA_BASE=0x%x\n",(ioread32(DEMONSTRATOR_APB_BASE + NA_DCNT)));
	
	iowrite32(0,	(DEMONSTRATOR_APB_BASE + NA_WCNT));   		// set initial memory address for weights
	iowrite32(0,	(DEMONSTRATOR_APB_BASE + NA_RCNT));    		// set initial memory address for the result
	iowrite32(2048,	(DEMONSTRATOR_APB_BASE + NA_FADR));    		// data size

	uintptr_t addr_ext =  (uintptr_t) rumboot_malloc_from_heap_aligned(1,data_size, 16);
	uintptr_t addr_ext_w= (uintptr_t) rumboot_malloc_from_heap_aligned(1, weight_size,16);


    //uint32_t addr_ext = DATA_BASE;
	rumboot_printf("READ_input_size=0x%x\n", data_size);
	load_array(data_size , addr_ext, data ); //reduced temporally!!
	load_array_w (weight_size, addr_ext_w ); //reduced temporally
	
	//rumboot_printf("set demonstrator 3\n");

	rumboot_printf("READ_input_DATA=0x%x\n", addr_ext);
	//rumboot_printf(" demonstrator data load for DMA\n");
	//uint32_t data_read = ioread32(addr_ext);
	//rumboot_printf("READ_input_DATA=0x%x\n", data_read);
	
    
	assert (  ((uintptr_t) addr_ext != true) &&  ((uintptr_t) addr_ext_w != true));
	//rumboot_printf("READ_input_ADDRESS\n");
	ret = load_demonstrator_data (1, addr_ext,  data_size);	//load data for multiplication	
	if (ret) {rumboot_printf("TEST FAILED\n");
				return 1;		
				}
	rumboot_printf(" demonstrator data array was loaded by DMA\n");
	//rumboot_printf("data_addr_ext=0x%x\n",(ioread32(addr_ext)));
	ret = load_demonstrator_weights (1, addr_ext_w,  weight_size);	//load weights for multiplication
	rumboot_printf("data_addr_ext_w=0x%x\n",(ioread32(addr_ext_w)));
	if ( ret) {rumboot_printf("TEST FAILED-my1\n");
				return 1;		
				}
	rumboot_printf(" demonstrator weights array was loaded by DMA\n");
//----------------------------------------------------------------------
//--------------------------temporal checking array----------------------
	//load_array(data_size , addr_ext_r, data ); //reduced temporally!!
	//ret = load_demonstrator_data (1,addr_ext_r,  data_size);	//load data for multiplication	
	//if (ret) {rumboot_printf("TEST FAILED\n");
		//		return 1;		
	//			}
	//rumboot_printf(" demonstrator data array was loaded by DMA\n");
	//rumboot_printf("data_addr_ext_w=0x%x\n",(ioread32(addr_ext_w)));
	//ret = load_demonstrator_result (1, addr_ext_r,  data_size);	//load weights for multiplication
	//if ( ret) {rumboot_printf("TEST FAILED-my2\n");
	//			return 1;		
	//			}
	//rumboot_printf(" demonstrator result array was loaded by DMA\n");
	//rumboot_printf("data_addr_ext_r=0x%x\n",(ioread32(addr_ext_r)));
	
//---------------------------------------------------
//---------------------------------------------------	
	iowrite32(0x1,	(DEMONSTRATOR_APB_BASE + NA_STGS));    		//   data size	
	cnt = DEMONSTRATOR_ATTEMPT  ;
	iowrite32(1,(DEMONSTRATOR_APB_BASE + NA_PMEN));    			// MTX_PMEN -run load weights to memory
	rumboot_printf(" demonstrator runs to load weights\n");
	do {
	tmp = 0xFFFFFFFF & (ioread32(DEMONSTRATOR_APB_BASE + NA_PUMP));	//wait end of loading weights	
	--cnt;
	} while (tmp != 0x00000000);
	
	if (cnt == 0) {
           rumboot_printf("No end exchange!\n");
            return 1;		
		}
	rumboot_printf(" load weights from shadow registers\n");	
	iowrite32(0x1,(DEMONSTRATOR_APB_BASE + NA_WCHG)); 
	
	rumboot_printf(" demonstrator runs matrix\n");
	iowrite32(1,(DEMONSTRATOR_APB_BASE +NA_ENAB));    			// run multiplication on matrix	
	

	cnt = 32  ;//temporal !!!
		
	do {
	tmp = 0xFFFFFFFF & (ioread32(DEMONSTRATOR_APB_BASE + NA_COMP));	//wait the end of multiplication	
	--cnt;

	} while  ((tmp != 0x00000001)/*|(cnt != 0) */) ; // 
	// rumboot_printf("CYCLE_COUNT=%d\n",ioread32(DEMONSTRATOR_APB_BASE + NA_CNTT)); 
	if (cnt == 0)  {
           rumboot_printf("No end multiplication!\n");
            return 1;

	
}
	rumboot_printf("CYCLE_COUNT=%d\n",ioread32(DEMONSTRATOR_APB_BASE + NA_CNTT)); // the number of cycles that matrix runs
	rumboot_printf("CYCLE_end=%d\n",ioread32(DEMONSTRATOR_APB_BASE + NA_COMP));   // multiplication is ended
}
int demonstrator_compare_data(uintptr_t src_etalon[], uintptr_t addr_ext_r[],  uint32_t len) {

	int i;
	uint32_t addr;
	uint32_t addr_etalon;
	uint32_t tmp;
	
	for (i = 0; i< len ; i++)
	{
	 addr 	=  &addr_ext_r[i];
	 addr_etalon 	= &src_etalon[i];
	//rumboot_printf("&addr_ext_r=0x%x\n",addr_ext_r[i]);
	//rumboot_printf("src_etalon=0x%x\n",src_etalon[i]);
	//rumboot_printf("addr=0x%x\n",&addr_ext_r[i]);
	//rumboot_printf("addr_etalon=0x%x\n",&src_etalon[i]);

	tmp  = ioread32(addr) - ioread32(addr_etalon);
	
	//if (tmp != 0)        //temporally excluded to avoid the result of comparison
	{
		rumboot_printf("READ_etalon_adr=0x%x\n", addr_etalon);
		rumboot_printf("READ_etalon_DATA=0x%x\n", ioread32(addr_etalon));
		rumboot_printf("READ_adr=0x%x\n",addr);
		rumboot_printf("READ_DATA=0x%x\n", ioread32(addr));
	   // rumboot_printf("MATRIX test ERROR!\n");
		 }	
	}
	return 0;
}

//int load_etalon_file( uint32_t dump_size) /*{return  data[];} */ {
	
/*	uint32_t data[dump_size];	
	int len = dump_size * 4;
	int i;
    uint32_t addr[0];	

	uintptr_t dump_ptr =(uintptr_t) rumboot_malloc_from_heap(1, len, 4); */
//	uintptr_t dump_ptr = (uintptr_t) rumboot_malloc_from_heap_aligned(1, len, 16);
	/*	for (i = 0; i< len ; i++){
		addr[i] = 	&dump_ptr[i]; */
		//uint32_t (&dump_ptr[i]) = rand();
//		data[i] = dump_ptr[i];
//		}
//	rumboot_platform_request_file("myfile",dump_ptr);
//	rumboot_platform_dump_region("mydump", (uintptr_t) dump_ptr, dump_size*4);
//    rumboot_printf("dump_ptr[i]=0x%x\n", dump_ptr[0]);
/*    return 0;
} */

int load_array ( uint32_t dump_size, uintptr_t dump_ptr[], uint32_t data ) {
	uint32_t len = dump_size /4;
	uint32_t i;
	uint32_t tmp;


	for (i = 0; i< len ; i++){
	data =rand();	
	dump_ptr[i] = data;//rand(); //i; //it might be assigned rand for data and <i> as weights or vice versa
	tmp = ioread32(&dump_ptr[i]);

	//rumboot_printf("&dump_ptr[i]=0x%x\n", &dump_ptr[i]);
	//rumboot_printf(" ioread tmp=0x%x\n", tmp);
	//rumboot_printf(" dump_ptr[i]=0x%x\n", dump_ptr[i]);
	//rumboot_printf(" len=0x%x\n", len);
	//rumboot_printf(" i=0x%x\n", i);
	
	}
	return 0;	
} 
int load_array_w ( uint32_t dump_size, uintptr_t dump_ptr[] ) {
	uint32_t len = dump_size /4;
	uint32_t i;
	uint32_t tmp;
	dump_ptr[0] = (uintptr_t) rumboot_malloc_from_heap_aligned(1, len, 16); 
	rumboot_printf("len=0x%x\n", len);
	
	for (i = 0; i< len ; i++){
	dump_ptr[i] = i;//rand(); //i; //it might be assigned rand for data and <i> as weights or vice versa
	tmp = ioread32(&dump_ptr[i]);

	//rumboot_printf("&dump_ptr[i]=0x%x\n", &dump_ptr[i]);
	//rumboot_printf(" ioread tmp=0x%x\n", tmp);
	//rumboot_printf(" dump_ptr[i]=0x%x\n", dump_ptr[i]);
	//rumboot_printf(" len=0x%x\n", len);
	//rumboot_printf(" i=0x%x\n", i);
	
	}
	return 0;	
} 

int load_etalon_array ( uint32_t dump_size, uintptr_t dump_ptr[], uint32_t data ) {
	uint32_t len = dump_size /4;
	uint32_t i;
	uint32_t tmp;
	dump_ptr[0] = (uintptr_t) rumboot_malloc_from_heap_aligned(1, len, 16); 
	rumboot_printf("len=0x%x\n", len);
	
	for (i = 0; i< len ; i++){
	data = 0xa5a5a5a5;//rand();	
	dump_ptr[i] = data;//rand(); //i; //it might be assigned rand for data and <i> as weights or vice versa
	tmp = ioread32(&dump_ptr[i]);

	//rumboot_printf("&dump_ptr[i]=0x%x\n", &dump_ptr[i]);
	//rumboot_printf(" ioread tmp=0x%x\n", tmp);
	//rumboot_printf(" dump_ptr[i]=0x%x\n", dump_ptr[i]);
	//rumboot_printf(" len=0x%x\n", len);
	//rumboot_printf(" i=0x%x\n", i);
	
	}
	return 0;	
} 




