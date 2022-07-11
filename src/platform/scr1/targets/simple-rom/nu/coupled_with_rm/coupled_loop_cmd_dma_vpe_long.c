#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 
#include <regs/regs_na.h>
#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h>
 
#include <devices/rcm_na/nu_test_macro.h> 

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data
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
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  cfg->depend_mask=0; // Does Not Depend On Any Other Channel Run
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
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

int main() {
  int heap_id;
  int i;
 // int res=0;
  uint32_t * dump_ptr_adr;
  uint32_t * dump_ptr_dst_wdma_adr;
  uint32_t * dump_ptr_src_rdma_adr;  
  uint32_t * dump_ptr_op0_rdma_adr;
  uint32_t * dump_ptr_op1_rdma_adr;
  uint32_t * dump_ptr_op2_rdma_adr;
  uint32_t * dump_ptr_op2_lut1_access_adr;
  uint32_t * dump_ptr_op2_lut2_access_adr; 
  uint32_t dump_size =  416;
  uint32_t dump_size_dma_vpe = 256;
  uint32_t dump_size_lut1_access = 2064; //(NU_VPE_LUT<i>_SIZE)*8 +8;
  uint32_t dump_size_lut2_access = 528;
  int iterations;
  
  uint8_t  axi_len;
  

#ifdef AXI_LEN
  axi_len = AxiLen;
#else 
  axi_len = 15;
#endif

 
  rumboot_printf("Hello\n");
 // rumboot_printf("dump_lut= %d\n",((NU_VPE_LUT1_SIZE + NU_VPE_LUT2_SIZE)*8));
   heap_id = nu_get_heap_id();
  dump_ptr_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size/4) * sizeof(uint32_t),8);
  if (dump_ptr_adr == NULL) return -1;  

  dump_ptr_dst_wdma_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_dma_vpe/4) * sizeof(uint32_t),8);
  if (dump_ptr_dst_wdma_adr == NULL) return -1;  
 
  dump_ptr_src_rdma_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_dma_vpe/4) * sizeof(uint32_t),8);
  if (dump_ptr_src_rdma_adr == NULL) return -1;
  
  dump_ptr_op0_rdma_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_dma_vpe/4) * sizeof(uint32_t),8);
  if (dump_ptr_op0_rdma_adr == NULL) return -1;    
 
  dump_ptr_op1_rdma_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_dma_vpe/4) * sizeof(uint32_t),8);
  if (dump_ptr_op1_rdma_adr == NULL) return -1;

  dump_ptr_op2_rdma_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_dma_vpe/4) * sizeof(uint32_t),8);
  if (dump_ptr_op2_rdma_adr == NULL) return -1;
  
  dump_ptr_op2_lut1_access_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_lut1_access/4) * sizeof(uint32_t),8);
  if (dump_ptr_op2_lut1_access_adr == NULL) return -1;
  
  dump_ptr_op2_lut2_access_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size_lut2_access/4) * sizeof(uint32_t),8);
  if (dump_ptr_op2_lut2_access_adr == NULL) return -1;
  
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
  nu_vpe_init_test_desc(&test_desc);
  if(nu_vpe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc); 
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif

  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
    nu_vpe_iteration_start(&iteration_desc);
    nu_vpe_print_iteration_desc(&iteration_desc);
    
      // Load LUTs If Needed
	  	  
 // if(iteration_desc.cfg->op2_config.lut_en == Enable_En) {
 //   nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
//  }
     

    nu_vpe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.in_metrics,
      axi_len,
      iteration_desc.in_data,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      iteration_desc.res_metrics,
      iteration_desc.res_data
    );
    // nu_vpe_print_config(iteration_desc.cfg);
    // nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
	uintptr_t ptr_adr = rumboot_virt_to_dma(dump_ptr_adr);
	nu_vpe_regs_cmd_dma_setup(MY_VPE_REGS_BASE, dump_ptr_adr, dump_size,iteration_desc.cfg);		
	if (run_na_cmd_dma(NPE_BASE,ptr_adr,dump_size) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}	
			
	uintptr_t ptr_dst_wdma_adr = rumboot_virt_to_dma(dump_ptr_dst_wdma_adr);	
	nu_vpe_dma_regs_cmd_dma_setup(MY_VPE_REGS_BASE + NU_VPE_DST_WDMA,dump_ptr_dst_wdma_adr,dump_size_dma_vpe, &(iteration_desc.cfg->wdma_config),iteration_desc.cfg->trace_mode);	   
	if (run_na_cmd_dma(NPE_BASE,ptr_dst_wdma_adr,dump_size_dma_vpe) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}

	
	uintptr_t ptr_src_rdma_adr = rumboot_virt_to_dma(dump_ptr_src_rdma_adr);		
	nu_vpe_dma_regs_cmd_dma_setup(MY_VPE_REGS_BASE + NU_VPE_SRC_RDMA,dump_ptr_src_rdma_adr, dump_size_dma_vpe, &( iteration_desc.cfg->src_rdma_config),iteration_desc.cfg->trace_mode);	   	
	if (run_na_cmd_dma(NPE_BASE,ptr_src_rdma_adr,dump_size_dma_vpe) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}

	uintptr_t ptr_op0_rdma_adr = rumboot_virt_to_dma(dump_ptr_op0_rdma_adr);	
	nu_vpe_dma_regs_cmd_dma_setup(MY_VPE_REGS_BASE + NU_VPE_OP0_RDMA,dump_ptr_op0_rdma_adr,dump_size_dma_vpe, &( iteration_desc.cfg->op0_rdma_config),iteration_desc.cfg->trace_mode);	   	
	if (run_na_cmd_dma(NPE_BASE,ptr_op0_rdma_adr,dump_size_dma_vpe) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}
	
	uintptr_t	ptr_op1_rdma_adr = rumboot_virt_to_dma(dump_ptr_op1_rdma_adr);	
	nu_vpe_dma_regs_cmd_dma_setup(MY_VPE_REGS_BASE + NU_VPE_OP1_RDMA,dump_ptr_op1_rdma_adr, dump_size_dma_vpe, &( iteration_desc.cfg->op1_rdma_config),iteration_desc.cfg->trace_mode);	   	
	if (run_na_cmd_dma(NPE_BASE,ptr_op1_rdma_adr,dump_size_dma_vpe) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}
	
	uintptr_t ptr_op2_rdma_adr = rumboot_virt_to_dma(dump_ptr_op2_rdma_adr);		
	nu_vpe_dma_regs_cmd_dma_setup(MY_VPE_REGS_BASE + NU_VPE_OP2_RDMA,dump_ptr_op2_rdma_adr, dump_size_dma_vpe,	&(	iteration_desc.cfg->op2_rdma_config),iteration_desc.cfg->trace_mode);	   	
	if (run_na_cmd_dma(NPE_BASE,ptr_op2_rdma_adr,dump_size_dma_vpe) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}
			
	if(iteration_desc.cfg->op2_config.lut_en == Enable_En){
   
	//------LUT1-------	
	uintptr_t	ptr_op2_lut1_access_adr = rumboot_virt_to_dma(dump_ptr_op2_lut1_access_adr);		
	nu_vpe_lut_access_setup(MY_VPE_REGS_BASE,((1<<17)|(0<<16)), dump_ptr_op2_lut1_access_adr, dump_size_lut1_access,iteration_desc.lut1);	   	
	if (run_na_cmd_dma(NPE_BASE,ptr_op2_lut1_access_adr,dump_size_lut1_access) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}
	
	//------LUT2-------
	uintptr_t	ptr_op2_lut2_access_adr = rumboot_virt_to_dma(dump_ptr_op2_lut2_access_adr);		
	nu_vpe_lut_access_setup(MY_VPE_REGS_BASE, ((1<<17)|(1<<16)), dump_ptr_op2_lut2_access_adr, dump_size_lut2_access,iteration_desc.lut2);	   		
	if (run_na_cmd_dma(NPE_BASE,ptr_op2_lut2_access_adr,dump_size_lut2_access) != 0 ) return -1;
	else { if ( na_dma_cmd_complete(NPE_BASE) != 0)  return -1;}
	}
	iowrite32(0x007, NPE_BASE + NA_CU_REGS_BASE   + NA_UNITS_MODE); //turn on APB bus    
	
	rumboot_printf("Running DMA..\n");
 
#if DUT_IS_VPE

      // Setup Main Channel DMAs if Required
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,iteration_desc.in_data,iteration_desc.in_metrics->s);
    if(iteration_desc.cfg->wdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,iteration_desc.res_data,iteration_desc.res_metrics->s);
    
      // Invoke Required DMA Channels
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
    if(iteration_desc.cfg->wdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
#endif
    
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
	nu_vpe_wait(MY_VPE_REGS_BASE, iteration_desc.cfg);
	
	#if DUT_IS_VPE
      // Finalize Required DMA Channels
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    if(iteration_desc.dma_config->.dma_en == Enable_NotEn)
      nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
#endif
    
    rumboot_printf("Comparing..\n");
		if(iteration_desc.cfg->op2_config.lut_en == Enable_En){
		uint32_t tmp;
 
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_CFG);	
		rumboot_printf("LUT_LUT_CFG=%x\n",tmp);	
	
	
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_XOFFSET);
		rumboot_printf("LUT_XOFFSET=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_YOFFSET);	
		rumboot_printf("LUT_YOFFSET =%x\n",tmp);	
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB0_XSHIFT);
		rumboot_printf("LUT_TAB0_XSHIFT=%x\n",tmp);	
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB1_XSHIFT);
		rumboot_printf("LUT_TAB1_XSHIFT=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB0_XMIN);
		rumboot_printf("LUT_TAB0_XMIN=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB0_XMAX);
		rumboot_printf("LUT_TAB0_XMAX%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB1_XMIN);
		rumboot_printf("LUT_TAB1_XMIN=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB1_XMAX);
		rumboot_printf("LUT_TAB1_XMAX=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB0_SLOPE_SCALE);
		rumboot_printf("LUT_TAB0_SLOPE_SCALE=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB0_SLOPE_SHIFT);
		rumboot_printf("LUT_TAB0_SLOPE_SHIFT=%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
		rumboot_printf("LUT_TAB1_SLOPE_SCALE_%x\n",tmp);
		tmp = ioread32(MY_VPE_REGS_BASE + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
		rumboot_printf("LUT_TAB1_SLOPE_SHIF+T%x\n",tmp);
		}
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
    
if(nu_vpe_check_status_regs(MY_VPE_REGS_BASE, iteration_desc.status_regs_etalon) != 0) {
   rumboot_printf("Test FAILED Due to Status Reg Check at iteration %d\n",i);
   nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
   return -1;
 }
  else {rumboot_printf("Test OK Due to Status Reg Check at iteration %d\n",i);}		
    
    
    nu_vpe_iterate_desc(&iteration_desc);
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
  
}
