#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 


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

  cfg->dma_d_config.depend_mask = 0; // Does Not Depend On Any Other Channel Run
  cfg->dma_w_config.depend_mask = 0;

  // Search For The MPE DMA Configuration (That Is Predefined In cfg_lut Table)
  if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1;

  // Fill Other Fields
  if(nu_mpe_decide_dma_config_trivial(cfg, cube_metrics, warr_metrics)!=0) return -1;

  return 0;
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  void*res_data,
  Enable MPE_ENABLED,
  Enable PPE_ENABLED
) {
  int axi_len = 15; // 15 - Most Performance-Friendly Option
  
  // Define The trace_mode FIRST!!!! Because nu_vpe_decide_dma_config_trivial depends On It
  if(MPE_ENABLED == Enable_En)
    cfg->trace_mode = TraceMode_MPE;  // MPE Trace Mode Always When Work With MPE
  else
    if(PPE_ENABLED == Enable_En)      // PPE Trace Mode Always When Work With PPE
      cfg->trace_mode = TraceMode_PPE;
    else
      cfg->trace_mode = TraceMode_Linear; // Simple Linear Trace Mode When Standalone
  
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
  cfg->depend_mask=0; // Does Not Depend On Any Other Channel Run // if Depends - This Parameter Set Later
  
  if(MPE_ENABLED == Enable_En)
    cfg->src_flying = Enable_En;  // Make VPE To Get Data From MPE
  else
    cfg->src_flying = Enable_NotEn; // VPE Takes Data By Its Own
  
  if(PPE_ENABLED==Enable_En)    //  And Send Them To PPE If PPE_ENABLED
    cfg->dst_flying = Enable_En;
  else
    cfg->dst_flying = Enable_NotEn;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics); // Fill Other cfg Fields
  
    // Data Base Addresses
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

void nu_ppe_decide_dma_config(
  ConfigPPE* cfg,
  ConfigREGPPE* cfg_reg,
  CubeMetrics* res_metrics,
  void*in_data,
  void*res_data,
  Enable MPE_ENABLED,
  Enable VPE_ENABLED
){
  cfg_reg->rBALi = nu_virt_to_dma(in_data); // Data Base Addresses
  cfg_reg->wBALo = nu_virt_to_dma(res_data);

  if(MPE_ENABLED == Enable_En)
    cfg_reg->wOpM = 3 << 8; // FLYING_BOXED Every Time MPE Works
  else
    if(VPE_ENABLED == Enable_En)
      cfg_reg->wOpM = 1 << 8; // FLYING_LINEAR - The Simpliest PPE Mode
    else
      cfg_reg->wOpM = 0 << 8; // OFFFLY_LINEAR - Simpliest PPE Mode And Take Data By Itself
                         // Other Fields Of wOpM Will Be Appended By nu_ppe_decide_dma_config_trivial
  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
  cfg_reg->wOpEn  = 0x0; // This Is Needed Because Some Other Setup Functions Write Into DMA Start Reg
}


NPETestDescriptor test_desc;
NPEIterationDescriptor iteration_desc;
NAHeapMap heap_map;

int main() {
  int i;
  int iterations;
  
  rumboot_printf("Hello, Dep\n");

    // Take The Simple Heap Map For Test Data Arrays
  nu_get_rather_fair_heap_map(&heap_map);

    // Read The Number Of Test Iterations
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
    // Zero The Test Descriptor Fields
  nu_npe_init_test_desc(&test_desc);
  
    // Place The Test Data Including Dependency Table
  if(nu_npe_place_arrays_with_dep_by_heap_map(&heap_map,&test_desc,iterations) != 0) return -1;
  
    // Allocate The Place For A Maximum Associative Register Dump
  if(nu_npe_place_associative_regs_dump(heap_map.control, &test_desc, iterations) != 0) return -1;
  
    // Allocate Two Reg Dump Buffers
  nu_npe_place_regs_dump(heap_map.control,&test_desc); 
  
    // Move The Content Of DUT Regs Into The Initial Regs Dump
    //  Then, It Will Be Compared With The First Iteration Dump
  //nu_npe_make_reg_dump(NPE_BASE, test_desc.curr_regs_dump);
  // This Is Bad On RTL Because DUT Contains Not Initialized Regs
    // Hope, That Write Reg Value Will Never Match 0xA5A5A5A5 On The First Iteration
  memset(test_desc.curr_regs_dump,0xA5,sizeof(NARegDump));
  
  nu_npe_print_test_desc(&test_desc,iterations);

    // Load The Initial Values To The Iteration Descriptor
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Deciding DMA Configuration for iteration %d\n",i);
    
      // Init The MPE_ENABLED, VPE_ENABLED, PPE_ENABLED Fields And Init The op0, op1, op2
    nu_npe_iteration_with_dep_start(&iteration_desc,&(test_desc.iteration_cfg_map),i);
    
      // Calc Secondary Configuration Fields
    if(iteration_desc.MPE_ENABLED == Enable_En) {
      if(nu_mpe_decide_dma_config(
        iteration_desc.cfg_mpe, 
        iteration_desc.in_metrics, 
        iteration_desc.warr_metrics,
        iteration_desc.in_data,
        iteration_desc.warr,
        test_desc.mpe_cfg_lut
      ) != 0) return -1;
      
      nu_calc_mpe_cube_out_metrics(iteration_desc.cfg_mpe, &iteration_desc.mpe_out_metrics);
    }
    
    if(iteration_desc.VPE_ENABLED == Enable_En) {
      nu_vpe_decide_dma_config (
        iteration_desc.cfg_vpe,
        iteration_desc.MPE_ENABLED==Enable_En ? &iteration_desc.mpe_out_metrics : iteration_desc.in_metrics,
        iteration_desc.in_data,
        iteration_desc.op0,
        iteration_desc.op1,
        iteration_desc.op2,
        iteration_desc.res_data,
        iteration_desc.MPE_ENABLED,
        iteration_desc.PPE_ENABLED
      );
    }
    
    if(iteration_desc.PPE_ENABLED == Enable_En) {
      nu_ppe_decide_dma_config(
        iteration_desc.cfg_ppe,
        iteration_desc.cfg_reg_ppe,
        iteration_desc.res_metrics,
        iteration_desc.in_data,
        iteration_desc.res_data,
        iteration_desc.MPE_ENABLED,
        iteration_desc.VPE_ENABLED
      );
    }
    
      // This Updates The Address Fields And Dependency Mask Of Configuration Structures
    nu_npe_add_depend_rd_after_wr(&test_desc, i);
    
      // CRUTCH, Because The Unused Copy Of Input Data Come From Files
      //         This Data Should Be Got In Runtime, And We Erase The Initial Copy Of This Data
      //         To Guarantee That Device Has Read Them From Runtime Copy
    nu_npe_erase_unused_in_data(&iteration_desc, & (test_desc.array_of_depend_table[i]), i);
    
    // if(iteration_desc.MPE_ENABLED == Enable_En)
    //   nu_mpe_print_config(iteration_desc.cfg_mpe);
    // if(iteration_desc.VPE_ENABLED == Enable_En)
    //   nu_vpe_print_config(iteration_desc.cfg_vpe);
    // if(iteration_desc.PPE_ENABLED == Enable_En) {
    //   nu_ppe_print_config(iteration_desc.cfg_ppe);
    //   nu_ppe_print_config_reg(iteration_desc.cfg_reg_ppe);
    // }

      // Fill The next_regs_dump Field With The Current Iteration Reg Content
    nu_npe_setup_next_regs_dump(&iteration_desc);
    
      // Make The Associative Dump Of This Iteration Regs
    nu_npe_append_associative_regs_dump(&iteration_desc);
    
      // Point At The Next Iteration Arrays (Also Swaps Reg Dumps)
    nu_npe_iterate_desc(&iteration_desc);
  }
  
  test_desc.associative_regs_dump_end_ptr = iteration_desc.associative_regs_dump_curr_ptr;

  uint32_t associative_regs_dump_byte_size = (
      test_desc.associative_regs_dump_end_ptr -
      test_desc.associative_regs_dump_start_ptr
  )*sizeof(NPEReg);

    // Actual Work
  nu_npe_cmd_dma_setup(NPE_BASE, (uint32_t)test_desc.associative_regs_dump_start_ptr, associative_regs_dump_byte_size);
  nu_npe_cmd_dma_run(NPE_BASE);
  nu_npe_cmd_dma_wait_page_complete(NPE_BASE);
  nu_npe_wait_busy(NPE_BASE);
    //

    // Checking
    // Init It Again - We Start Iterate From The Beginning
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  for(i=0;i<iterations;i++) {
    rumboot_printf("Comparing iteration %d..\n",i);
    nu_npe_iteration_start(&iteration_desc);
    
      // Result vs Etalon Comparision
    if(NU_COMPARE_FUNCTION(iteration_desc.res_data, iteration_desc.etalon, iteration_desc.res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      if(iteration_desc.MPE_ENABLED==Enable_En)
        nu_mpe_print_config(iteration_desc.cfg_mpe);
      if(iteration_desc.VPE_ENABLED==Enable_En)
        nu_vpe_print_config(iteration_desc.cfg_vpe);
      if(iteration_desc.PPE_ENABLED==Enable_En) {
        nu_ppe_print_config(iteration_desc.cfg_ppe);
        nu_ppe_print_config_reg(iteration_desc.cfg_reg_ppe);
      }

      rumboot_printf("Test FAILED at iteration %d\n",i);

      return 1;
    }
    
      // Point At The Next Iteration Data
    nu_npe_iterate_desc(&iteration_desc);
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}