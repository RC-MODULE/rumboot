#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>

#include <platform/devices.h> 

#include <rumboot/irq.h>
#include <platform/interrupts.h>


#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 


#include <regs/regs_na.h>

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data,
  bool last_turn
) {
  
  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif

#ifdef VPE_TraceMode_PPE
  cfg->trace_mode = TraceMode_PPE;
#else
  cfg->trace_mode = TraceMode_MPE;
#endif

  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  if(last_turn) 
    cfg->mark = Enable_En;
  
  cfg->src_rdma_config.dma_baddr = nu_virt_to_dma(in_data);
  cfg->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
  cfg->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
  cfg->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
  cfg->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;
  
}

VPETestDescriptor test_desc;
VPEIterationDescriptor iteration_desc;

//volatile unsigned VPE_HANDLED;

static volatile uint32_t  Int_VPE_CNTX_APPLY;
static volatile uint32_t  Int_CNTX_FINISH =0;
static volatile uint32_t  Int_MARKED_FINISH =0;
static volatile uint32_t  VPE_IRQ = IRQ_VPE_CNTX_APPLY;

static void handler1() {

	rumboot_printf( "Int_irq= %x\n",(ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) )) ;
		uint32_t NA_clear = 0;
		uint32_t const NA_Status = ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS ); //Read interrupt status
	/*	 if  (((NA_Status >> 19) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x80000;
			  Int_VPE_CNTX_APPLY = 0;
			
		 }
		 if  (((NA_Status >> 11) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x800;
			  Int_VPE_CNTX_APPLY = 0;
		 }
		 if  (((NA_Status >> 13) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x2000;
			  Int_VPE_CNTX_APPLY = 0;
		 }
		 if  (((NA_Status >> 14) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x4000;
			  Int_VPE_CNTX_APPLY = 0;
		 } 
		 if  (((NA_Status >> 15) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear |  0x8000;
			  Int_VPE_CNTX_APPLY = 0;
		 }
		 if  (((NA_Status >> 17) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x20000;
			  Int_VPE_CNTX_APPLY = 0;
			  Int_CNTX_FINISH ==1;
		 }
		 if  (((NA_Status >> 18) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x40000;
			  Int_VPE_CNTX_APPLY = 0;
		 } 
		if  (((NA_Status >> 16) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x10000;
			  Int_VPE_CNTX_APPLY = 0;
		 }
		 if  (((NA_Status >> 12) & 1)== 1){
			  Int_VPE_CNTX_APPLY = 1;
			  NA_clear = NA_clear | 0x1000;
			  Int_VPE_CNTX_APPLY = 0;
			  Int_CNTX_MARKED_FINISH = 1;
		 }
		iowrite32(NA_clear,(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);

	}*/		
		 
		if (((ioread32((NPE_BASE + NA_CU_REGS_BASE)  + NA_INT_UNITS_STATUS) >> 19) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<19),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);		
		Int_VPE_CNTX_APPLY = 0;
		}
		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 11) & 1)== 1)  {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<11),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);
		Int_VPE_CNTX_APPLY = 0;
		}
	
		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 13) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<13),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);	
		Int_VPE_CNTX_APPLY = 0;
		}
  	  	if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 14) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<14),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);	
		Int_VPE_CNTX_APPLY = 0;
		}
		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 15) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<15),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);
		Int_VPE_CNTX_APPLY = 0;
		}

		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 17) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<17),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);
		Int_VPE_CNTX_APPLY = 0;
		Int_CNTX_FINISH = 1;
		}

		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 18) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;
		rumboot_printf( "Int_Marked cube\n");
		iowrite32((1<<18),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);
		rumboot_printf( "Int_VPE_CNTX_APPLY= %d\n",Int_VPE_CNTX_APPLY );
		Int_VPE_CNTX_APPLY = 0;

		}

		if (((ioread32(NPE_BASE + NA_CU_REGS_BASE  + NA_INT_UNITS_STATUS) >> 16) & 1)== 1) {
		Int_VPE_CNTX_APPLY = 1;	
		iowrite32((1<<16),(NPE_BASE + NA_CU_REGS_BASE) + NA_INT_UNITS_RESET);
		rumboot_printf( "Int_VPE_CNTX_APPLY= %d\n",Int_VPE_CNTX_APPLY );	
		Int_VPE_CNTX_APPLY = 0;
		}
      		
  if (((ioread32((NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) ) >> 12) & 1) == 1) {
		Int_VPE_CNTX_APPLY = 1;
		iowrite32((1<<12),(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET));
		Int_VPE_CNTX_APPLY = 0;
		Int_MARKED_FINISH = 1;
		rumboot_printf( "VPE_MARKED_FINISH= %d\n",Int_MARKED_FINISH );
		
		}  
}
	  
static struct rumboot_irq_entry * init_irq() {
    rumboot_irq_cli();
    struct rumboot_irq_entry * const tbl = rumboot_irq_create( NULL );
	VPE_IRQ = IRQ_VPE_CNTX_APPLY;

	
    rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	
    rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	
	rumboot_irq_set_handler( tbl,IRQ_VPE_CNTX_APPLY, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL ); 
	
	
	rumboot_irq_table_activate( tbl );
	rumboot_irq_enable(VPE_IRQ);
    rumboot_irq_sei();
    return tbl;
}	
	
static void deinit_irq( struct rumboot_irq_entry * const tbl ) {
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}
uint32_t wait_na_int_handled( uint32_t timeout, volatile uint32_t * const  flag ) {
   do {     

			while (( (ioread32(NPE_BASE + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {}
					 iowrite32((1<<12),NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
					// rumboot_printf(" NA_VPE wait is completed\n");
					
			
			if( * flag) {	
			
			//rumboot_printf( "Int_12345\n");
           * flag = 0;
            return 1;
       }
    } while ( --timeout );

    return 0;
}


int main() {
  int heap_id;
  int i;
  int iterations;
  LUTLoadDecision* lut_decision;
  void* lut1_prev;
  void* lut2_prev;

  struct rumboot_irq_entry * tbl = init_irq();
  uint8_t  axi_len;


#ifdef AXI_LEN
  axi_len = AxiLen;
#else 
  axi_len = 15;
#endif

 
  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  nu_vpe_init_test_desc(&test_desc);
  if(nu_vpe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  
#if DUT_IS_NPE
 na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE,0x00000000);;
#endif
 #ifdef VPE_CUBE_CMPL
  nu_vpe_set_int_mask(MY_VPE_REGS_BASE, iteration_desc.cfg);  
#else 
{ rumboot_printf("NPE_BASE= %x\n",NPE_BASE);
nu_npe_vpe_set_int_mask(NPE_BASE);
}
#endif      
  lut_decision = rumboot_malloc_from_heap(heap_id,sizeof(LUTLoadDecision)*iterations);
  lut1_prev=NULL;lut2_prev=NULL;
  //~ cfg_prev=NULL;
  
    // Fill The Recent cfg Fields Now Because We Can
  for(i=0;i<iterations;i++) {
    	// iterations =15;
	rumboot_printf("Deciding DMA Configuration, Iteration %d\n",i);
    nu_vpe_iteration_start(&iteration_desc);
    // nu_vpe_print_iteration_desc(&iteration_desc);
    nu_vpe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.in_metrics,
      axi_len,
      iteration_desc.in_data,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      iteration_desc.res_metrics,
      iteration_desc.res_data,
      i==iterations-1
    );
    // nu_vpe_print_config(iteration_desc.cfg);
    // nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
    
      // Prepare The Decision What To Do With LUT On Each Iteration
    if(iteration_desc.cfg->op2_config.lut_en==Enable_En) {
      if(lut1_prev==NULL) {
        lut_decision[i]=LUTLoadDecision_LoadNow;
        lut1_prev = iteration_desc.lut1; lut2_prev = iteration_desc.lut2;
      }
      else {
        if(nu_vpe_compare_luts(iteration_desc.lut1, iteration_desc.lut2, lut1_prev, lut2_prev) == 0) {
          lut_decision[i]=LUTLoadDecision_DontLoad;
        }
        else {
          lut_decision[i]=LUTLoadDecision_BlockThenLoad;
          lut1_prev = iteration_desc.lut1; lut2_prev = iteration_desc.lut2;
        }
      }
    }
    else {
      lut_decision[i]=LUTLoadDecision_DontLoad;
      lut1_prev = NULL; lut2_prev = NULL;
    }
    
    //~ cfg_prev = iteration_desc.cfg;
    nu_vpe_iterate_desc(&iteration_desc);
  }
  
    // Once More Init - This Time For The Main Working Loop
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  	// iterations =15;
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
    nu_vpe_iteration_start(&iteration_desc);
    
      // Load LUTs If Needed
    if(lut_decision[i]==LUTLoadDecision_BlockThenLoad) {
      rumboot_printf("Blocked To Load LUT\n");
      while (nu_vpe_busy(MY_VPE_REGS_BASE)) {};
      rumboot_printf("OK\n");
    }
    if(lut_decision[i]!=LUTLoadDecision_DontLoad) {
      nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
    }
 
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg);
	//rumboot_printf("OK_1\n");
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg);
	//rumboot_printf("OK_2\n");
 
    if(i!=iterations-1)
      nu_vpe_iterate_desc(&iteration_desc);
  }

	//rumboot_printf("VPE_STATUS =%x\n ",(ioread32(MY_VPE_REGS_BASE + NU_VPE +NU_VPE_INT_STATUS)));
   // nu_vpe_wait(MY_VPE_REGS_BASE, iteration_desc.cfg);
	
	
	if( !wait_na_int_handled( 1000, &Int_MARKED_FINISH ) ) {
			rumboot_printf( "NA_VPE interrupt timeout_2\n" );
			return 1;
	}
		
    // And Third Turn - Check The Data
  rumboot_printf("Comparing..\n");
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  
  for(i=0;i<iterations;i++) {
    nu_vpe_iteration_start(&iteration_desc);
    
    if(nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      rumboot_printf("Addresses: res_data=0x%x, etalon=0x%x\n", (uintptr_t) iteration_desc.res_data, (uintptr_t) iteration_desc.etalon);
      nu_vpe_print_config(iteration_desc.cfg);
      nu_vpe_interpret_mismatch(
        iteration_desc.cfg,
        iteration_desc.res_data,
        iteration_desc.etalon,
        iteration_desc.in_data,
        iteration_desc.op0,
        iteration_desc.op1,
        iteration_desc.op2,
        iteration_desc.res_metrics->s,
        iteration_desc.res_metrics->C
      );
      return 1;
    }
    
    nu_vpe_iterate_desc(&iteration_desc);
  }
   deinit_irq(tbl);
  rumboot_printf("Test Passed\n");
  return 0;
  
}

