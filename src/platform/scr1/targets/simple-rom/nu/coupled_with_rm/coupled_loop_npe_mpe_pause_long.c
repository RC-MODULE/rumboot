#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

#include <regs/regs_na.h>
#include <platform/devices.h>

int nu_mpe_decide_dma_config(
  ConfigMPE* cfg, 
  CubeMetrics* cube_metrics, 
  WarrMetrics* warr_metrics,
  void* cube,
  void* warr,
  void* cfg_lut
) {
  cfg->dma_d_config.rdma.BFCA  = nu_virt_to_dma(cube); // Data Base Address
  cfg->dma_w_config.rdma.BFCA  = nu_virt_to_dma(warr); // Weights Base Address
  if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1; // Search For The MPE DMA Configuration (That Is Predefined In cfg_lut Table)
  if(nu_mpe_decide_dma_config_trivial(cfg, cube_metrics, warr_metrics)!=0) return -1; // Fill Other Fields
  return 0;
}

void inc_if_not_zero (uint8_t* data) {
  if (*data!=0) (*data)++;
}

void rewrite_and_inc_if_not_zero (uint8_t* data0, uint8_t data1) {
  if (data1!=0) *data0 = data1+1;
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  //~ CubeMetrics*res_metrics,
  void*res_data,
  Enable PPE_ENABLED
) {
  cfg->trace_mode = TraceMode_MPE;  // Make VPE To Use MPE Trace Mode // FIRST!!!! Because nu_vpe_decide_dma_config_trivial depends On It
  
  cfg->src_rdma_config.dma_bsize=0;   // Batch Parameters Are Default In This Program (Do Not Use Batch)
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  cfg->src_flying = Enable_En;  // Make VPE To Get Data From MPE
  if(PPE_ENABLED==Enable_En)    //  And Send Them To PPE If PPE_ENABLED
    cfg->dst_flying = Enable_En;
  else
    cfg->dst_flying = Enable_NotEn;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics); // Fill Other cfg Fields
  
    // Data Base Addresses
  cfg->src_rdma_config.dma_baddr = (uint32_t) 0xDEADBEEF; // Should Be Not Used
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

void nu_ppe_decide_dma_config(
  ConfigPPE* cfg,
  ConfigREGPPE* cfg_reg,
  CubeMetrics* res_metrics,
  void*in_data,
  void*res_data
){
  cfg_reg->rBALi = (uintptr_t)0xDEADBEEF; // Data Base Addresses
  cfg_reg->wBALo = nu_virt_to_dma(res_data);

  cfg_reg->wOpM = 3 << 8; // FLYING_BOXED 
                         // Other Fields Of wOpM Will Be Appended By nu_ppe_decide_dma_config_trivial
  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
  cfg_reg->wOpEn  = 0x0; // This Is Needed Because Some Other Setup Functions Write Into DMA Start Reg
}


NPETestDescriptor test_desc;
NPEIterationDescriptor iteration_desc;


int main() {
  int heap_id;
  int i;
  int iterations;
  uint8_t  axi_len;
  
  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();
  
    
  #if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE,0x00000000);
  nu_npe_mpe_set_int_mask(NPE_BASE);
//nu_npe_vpe_set_int_mask(NPE_BASE, iteration_desc.cfg_vpe);
  #endif
  
    // Read The Number Of Test Iterations
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
    // Zero The Test Descriptor Fields
  nu_npe_init_test_desc(&test_desc);
  
    // Overwrite The PPE_ENABLED Test Property If This Program Is Compiled Not To Use The PPE
    //  If You Need A Scenario Where You Can Dynamically Enable Or Disable PPE - You Should Write Another Program
#ifdef DONT_USE_PPE
  test_desc.PPE_ENABLED=Enable_NotEn;
#endif
  axi_len =15;		 // 15 - Most Performance-Friendly Option
  
    // Load All The Test Data Into Memory
  if(nu_npe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
    // Make The Iteration Descriptor To Point At The First Test Data
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  
    // Program The CU To Enable Direct Mode For All Units // Again - If You Want Otherwise - Write Another Program
//na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_MPE_UNIT_MODE|NA_CU_VPE_UNIT_MODE|NA_CU_PPE_UNIT_MODE);
  
    // Main Loop 
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
      // Prepare Iteration Descriptor (Some Modifications Should Be Made Before Each Iteration)
    nu_npe_iteration_start(&iteration_desc);
    
      // Load (Into The VPE Internal Memory) LUTs If Needed
    if(iteration_desc.cfg_vpe->op2_config.lut_en == Enable_En) {
      nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
    }
    
      // Fill The cfg_mpe Fields That Are Not Loaded From File
    if(nu_mpe_decide_dma_config(
      iteration_desc.cfg_mpe,
      iteration_desc.in_metrics,
      iteration_desc.warr_metrics,
      iteration_desc.in_data,
      iteration_desc.warr,
      test_desc.mpe_cfg_lut )!=0) return -1;

      // mpe_out_metrics - Metrics Of VPE Input Data (Not Seen In Memory) Needed For Configuring VPE Later
    nu_calc_mpe_cube_out_metrics(iteration_desc.cfg_mpe, &iteration_desc.mpe_out_metrics); // CHECK - Calc It Before Any Iteration?
    
      // Fill The cfg_vpe Fields That Are Not Loaded From File
    nu_vpe_decide_dma_config (
      iteration_desc.cfg_vpe,
     &iteration_desc.mpe_out_metrics,
      axi_len,
      NULL,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      //~ iteration_desc.res_metrics,
      iteration_desc.res_data,
      iteration_desc.PPE_ENABLED );
    
      // Fill The cfg_reg_ppe That Is Not Loaded From File
    if(iteration_desc.PPE_ENABLED==Enable_En)
      nu_ppe_decide_dma_config (
        iteration_desc.cfg_ppe,
        iteration_desc.cfg_reg_ppe,
        iteration_desc.res_metrics,
        NULL,
        iteration_desc.res_data );
    
      // Print All The Configurations Got

#ifdef MPE_CFG_TESTPLAN_RDMA_RDCH
    if ((iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[6].BiasEn!=0) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].BiasEn!=1) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].BiasEn!=0) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[3].BiasEn!=1) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[2].BiasEn!=1) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[1].BiasEn!=1) |
        (iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[0].BiasEn!=0)) {
      rumboot_printf("ERROR: This MPE' configuration does not not match for MPE RDMA RDCH test!\n");
      nu_mpe_print_config(iteration_desc.cfg_mpe);
      return -1;
    }
      // Replace setting from Bias[5] to Bias[4]
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].BiasEn = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].BiasEn;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].BiasEn = 0;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].ThreCtrl = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].ThreCtrl;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].DecCtrl  = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].DecCtrl;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].PXBSEn   = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].PXBSEn;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].PYBSEn   = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].PYBSEn;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].Bias     = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].Bias;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].AOffset         = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].AOffset;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].CntSha          = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].CntSha;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].CntOffsetEn     = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].CntOffsetEn;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].CntOffset       = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].CntOffset;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].CntThresholdSha = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].CntThresholdSha;
    iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[4].CntCmp          = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[5].CntCmp;

      // Shift Biases
    for (int shifts=i;shifts>0;shifts--) {
      inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.rdma.ThreCtrl);

      for (int j=6;j>0;j--) {
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].BiasEn = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].BiasEn;
        rewrite_and_inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].ThreCtrl,iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].ThreCtrl);
        rewrite_and_inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].DecCtrl ,iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].DecCtrl);
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].PXBSEn = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].PXBSEn;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].PYBSEn = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].PYBSEn;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].Bias   = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].Bias;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].AOffset         = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].AOffset;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].CntSha          = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].CntSha;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].CntOffsetEn     = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].CntOffsetEn;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].CntOffset       = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].CntOffset;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].CntThresholdSha = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].CntThresholdSha;
        iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j].CntCmp          = iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[j-1].CntCmp;
      }
      iteration_desc.cfg_mpe->dma_d_config.rdma.Bias[0].BiasEn = 0;
    }
#endif

    //~ nu_mpe_print_config(iteration_desc.cfg_mpe);
    //~ nu_vpe_print_config(iteration_desc.cfg_vpe);
    // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
    // if(iteration_desc.PPE_ENABLED==Enable_En) {
      // nu_ppe_print_config(iteration_desc.cfg_ppe);
      // nu_ppe_print_config_reg(iteration_desc.cfg_reg_ppe);
    // }
    
      // Program The VPE, MPE And PPE (Single Run)
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
    nu_mpe_setup(MY_MPE_REGS_BASE, iteration_desc.cfg_mpe);
    if(iteration_desc.PPE_ENABLED==Enable_En) {
      nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg_ppe);
    
      iteration_desc.cfg_reg_ppe->wOpEn  = 0x1;  // Two Lines Needed Because nu_ppe_wdma_run Just Writes The Content Of wOpEn
      nu_ppe_wdma_run(MY_PPE_REGS_BASE, iteration_desc.cfg_reg_ppe);
    }
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
    nu_mpe_run(MY_MPE_REGS_BASE, iteration_desc.cfg_mpe);
    
  //   if ((i== (iterations-1))  |  (i == 0)){
		nu_na_mpe_pause(NPE_BASE);
		nu_na_vpe_pause(NPE_BASE);
		nu_na_mpe_dev_pause_norst_resume(NPE_BASE);
	//	nu_na_vpe_dev_pause_norst_resume(NPE_BASE);

	nu_na_wait_int(NPE_BASE); //reset NA interrupts
//	nu_na_mpe_wait_int_pause(NPE_BASE);
//	} 	

	if(i!=iterations-1)
	nu_npe_iterate_desc(&iteration_desc);

      // Wait For The Corresponding DMA Channels To Complete
    if(iteration_desc.PPE_ENABLED==Enable_En)
		
      nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    else

		{rumboot_printf("Iteration= %d \n",i);
		nu_na_vpe_wait(NPE_BASE, iteration_desc.cfg_vpe);   //???
		}
}  
	rumboot_printf("Comparing..\n");
  	  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  	  nu_npe_iterate_desc(&iteration_desc);
 for(i=1;i<(iterations-1);i++ ){     
      // Result vs Etalon Comparision
	if(nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
	else {
      nu_mpe_print_config(iteration_desc.cfg_mpe);
      nu_vpe_print_config(iteration_desc.cfg_vpe);
      if(iteration_desc.PPE_ENABLED==Enable_En)
        nu_ppe_print_config(iteration_desc.cfg_ppe);
      
      rumboot_printf("Test FAILED at iteration %d\n",i);

      return 1;
		}
	 
      // Point At The Next Iteration Data
    nu_npe_iterate_desc(&iteration_desc);

  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}
