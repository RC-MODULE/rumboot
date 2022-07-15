#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h>

#define DONT_USE_PPE
#define CONVERT_NEW_FEATURE
//#define MACRO //load macrocommand


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
  
  /*cfg->src_rdma_config.dma_bsize=0;   // Batch Parameters Are Default In This Program (Do Not Use Batch)
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;*/
  cfg->depend_mask=0; // Does Not Depend On Any Other Channel Run
  
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

void nu_vpe_wait_custom(uintptr_t vpe_base, ConfigVPE* cfg){
  if(! cfg->mark) {
    rumboot_printf("Wait VPE WDMA...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 8) & 1) !=1) {}
    rumboot_printf("Done while wait.\n");
    iowrite32((1<<8),vpe_base + NU_VPE + NU_VPE_INT_RESET);
  }
  else {
    rumboot_printf("Wait VPE complete...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 12) & 1) !=1) {}
    iowrite32((1<<12),vpe_base + NU_VPE + NU_VPE_INT_RESET);
  }
  rumboot_printf("Done VPE wait.\n");
}


NPETestDescriptor test_desc;
NPEIterationDescriptor iteration_desc;


int main() {
  int heap_id;
  int i;
  //int iterations;
  uint8_t  axi_len;
  int num_cubes;
  
  rumboot_printf("Hellooooooo\n");

  heap_id = nu_get_heap_id();

///////////////////////////////////////////////////////////////////////////////////////////////
  volatile unsigned int *metrics_data;
  unsigned int data_size;
  volatile unsigned int *data_ptr;

  metrics_data = rumboot_malloc_from_heap(heap_id, 1 * sizeof(int));
  rumboot_platform_request_file_ex("cmd.bin.metrics", (uintptr_t)metrics_data, 4);
  data_size = (*metrics_data)*2;

  rumboot_printf("cmd size to load: %d\n", data_size);

  data_ptr = rumboot_malloc_from_heap_aligned(heap_id, data_size * sizeof(int), 8);
  rumboot_platform_request_file_ex("cmd.bin", (uintptr_t)data_ptr, data_size * 4);
//////////////////////////////////////////////////////////////////////////////////////////////
  
    // Read The Number Of Test Iterations - Num. of cubes
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &num_cubes,sizeof(num_cubes));
  rumboot_printf("Number of cubes %d\n",num_cubes);
  
    // Zero The Test Descriptor Fields
  nu_npe_init_test_desc(&test_desc);
  
    // Overwrite The PPE_ENABLED Test Property If This Program Is Compiled Not To Use The PPE
    //  If You Need A Scenario Where You Can Dynamically Enable Or Disable PPE - You Should Write Another Program
#ifdef DONT_USE_PPE
  test_desc.PPE_ENABLED=Enable_NotEn;
#endif
  axi_len = 15; // 15 - Most Performance-Friendly Option
  
    // Load All The Test Data Into Memory
  if(nu_npe_place_arrays(heap_id,&test_desc,num_cubes) !=0) return -1;
  
  rumboot_printf("Invocations = %d\n",test_desc.invocations);
  
    // Make The Iteration Descriptor To Point At The First Test Data
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  
    // Program The CU To Enable Direct Mode For All Units // Again - If You Want Otherwise - Write Another Program
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_MPE_UNIT_MODE|NA_CU_VPE_UNIT_MODE|NA_CU_PPE_UNIT_MODE);
  
    // Main Loop 
  for(i=0;i<test_desc.invocations;i++) {
    rumboot_printf("Starting invocation %d\n",i);
	
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

#ifdef MPE_CFG_TESTPLAN_RDMA_WRCH
      // Shift Biases
    for (int shifts=i;shifts>0;shifts--) {
      inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.wdma.Thre_PLC);
      inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.wdma.Thre_VLC);
      inc_if_not_zero(&iteration_desc.cfg_mpe->dma_d_config.wdma.Dec_PLC);
      for (int j=5;j>0;j--) {
        iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[j].CntSha = iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[j-1].CntSha;
        iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[j].CntCmp = iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[j-1].CntCmp;
      }
      iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[0].CntSha = 0;
      iteration_desc.cfg_mpe->dma_d_config.wdma.Bias[0].CntSha = 0;
    }
#endif

#ifdef MPE_CFG_POWER
  iteration_desc.cfg_mpe->rnd_size = 6;
#endif

    //~ nu_mpe_print_config(iteration_desc.cfg_mpe);
    //nu_vpe_print_config(iteration_desc.cfg_vpe);
    // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
    if(iteration_desc.PPE_ENABLED==Enable_En) {
      nu_ppe_print_config(iteration_desc.cfg_ppe);
      nu_ppe_print_config_reg(iteration_desc.cfg_reg_ppe);
    }
    
      // Program The VPE, MPE And PPE (Single Run)
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
    nu_mpe_setup(MY_MPE_REGS_BASE, iteration_desc.cfg_mpe);
    if(iteration_desc.PPE_ENABLED==Enable_En) {
      nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg_ppe);
    
      iteration_desc.cfg_reg_ppe->wOpEn  = 0x1;  // Two Lines Needed Because nu_ppe_wdma_run Just Writes The Content Of wOpEn
      nu_ppe_wdma_run(MY_PPE_REGS_BASE, iteration_desc.cfg_reg_ppe);
    }
    
    //nu_mpe_print_config(iteration_desc.cfg_mpe);

/////////////////////////////////////////////////////////////////////////////////////////
  iowrite32(0x00000071 , NPE_BASE+NA_MPE_BASE+0x00000104);
  iowrite32(0x00000004 , NPE_BASE+NA_MPE_BASE+0x00000108);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000010c);
  iowrite32(0x0000000d , NPE_BASE+NA_MPE_BASE+0x00000110);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000114);
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+0x00000118);
  iowrite32(0x0000000b , NPE_BASE+NA_MPE_BASE+0x0000011c);
  iowrite32(0x00000031 , NPE_BASE+NA_MPE_BASE+0x00000120);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+0x0000012c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000130);
  iowrite32(0x00000009 , NPE_BASE+NA_MPE_BASE+0x0000013c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000140);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000148);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000014c);
  iowrite32(0x00000006 , NPE_BASE+NA_MPE_BASE+0x00000154);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000158);
  iowrite32(0x00000004 , NPE_BASE+NA_MPE_BASE+0x00000160);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000164);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000016c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000170);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000178);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000017c);
  
  iowrite32(0x00000033 , NPE_BASE+NA_MPE_BASE+0x00000204);
  iowrite32(0x00000030 , NPE_BASE+NA_MPE_BASE+0x00000208);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000020c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000210);
  iowrite32((unsigned int)iteration_desc.in_data , NPE_BASE+NA_MPE_BASE+0x00000214);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000218);
  iowrite32(0x0000000f , NPE_BASE+NA_MPE_BASE+0x00000240);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000244);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+0x00000250);
  iowrite32(0x0000000b , NPE_BASE+NA_MPE_BASE+0x00000258);
  iowrite32(0x00000106 , NPE_BASE+NA_MPE_BASE+0x0000025c);
  iowrite32(0x00000031 , NPE_BASE+NA_MPE_BASE+0x00000260);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000264);
  iowrite32(0x00001880 , NPE_BASE+NA_MPE_BASE+0x00000270);
  iowrite32(0x00000009 , NPE_BASE+NA_MPE_BASE+0x00000278);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000027c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000280);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000284);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000290);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000298);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000029c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002a0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002a4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002b0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002b8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002bc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002c0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002c4);
  iowrite32(0x00000380 , NPE_BASE+NA_MPE_BASE+0x000002d0);
  iowrite32(0x00000006 , NPE_BASE+NA_MPE_BASE+0x000002d8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002dc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002e0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002e4);
  iowrite32(0x000000c0 , NPE_BASE+NA_MPE_BASE+0x000002f0);
  iowrite32(0x00000004 , NPE_BASE+NA_MPE_BASE+0x000002f8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000002fc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000300);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000304);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000310);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000318);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000031c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000220);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000224);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000230);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000234);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000324);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000328);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000032c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000330);
  
  iowrite32(0x00000077 , NPE_BASE+NA_MPE_BASE+0x00000504);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000508);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000050c);
  iowrite32(0x0000000d , NPE_BASE+NA_MPE_BASE+0x00000510);
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+0x00000514);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000518);
  iowrite32(0x0000000f , NPE_BASE+NA_MPE_BASE+0x0000051c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000520);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+0x0000052c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000530);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000053c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000540);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000548);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000054c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000554);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000558);
  iowrite32(0x00000006 , NPE_BASE+NA_MPE_BASE+0x00000560);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000564);
  iowrite32(0x00000004 , NPE_BASE+NA_MPE_BASE+0x0000056c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000570);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000578);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000057c);
  
  iowrite32(0x00000033 , NPE_BASE+NA_MPE_BASE+0x00000604);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000608);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000060c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000610);
  iowrite32((unsigned int)iteration_desc.warr , NPE_BASE+NA_MPE_BASE+0x00000614);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000618);
  iowrite32(0x0000000f , NPE_BASE+NA_MPE_BASE+0x00000640);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000644);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+0x00000650);
  iowrite32(0x00000006 , NPE_BASE+NA_MPE_BASE+0x00000658);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000065c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000660);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000664);
  iowrite32(0x00000310 , NPE_BASE+NA_MPE_BASE+0x00000670);
  iowrite32(0x0000000f , NPE_BASE+NA_MPE_BASE+0x00000678);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000067c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000680);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000684);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000690);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000698);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000069c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006a0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006a4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006b0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006b8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006bc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006c0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006c4);
  iowrite32(0x00000070 , NPE_BASE+NA_MPE_BASE+0x000006d0);
  iowrite32(0x00000006 , NPE_BASE+NA_MPE_BASE+0x000006d8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006dc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006e0);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006e4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006f0);
  iowrite32(0x00000004 , NPE_BASE+NA_MPE_BASE+0x000006f8);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x000006fc);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000700);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000704);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000710);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000718);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000071c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000620);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000624);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000630);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000634);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000724);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000728);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x0000072c);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000730);

#ifdef MACRO
  /*
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000000*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000001*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000002*4);
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000006*4);
  //iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000007*4);
  iowrite32(0x00000002 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000004*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000005*4);
  iowrite32(0x00000078 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000e*4);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000b*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000d*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000016*4);
  iowrite32(0x000001f4 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000f*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000011*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000013*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000017*4);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000019*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000014*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000015*4);
  iowrite32(0x00efefef , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000001c*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x000001c0*4);
  */
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000000*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000001*4);
  iowrite32(0x00000100 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000006*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000002*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000003*4);
  iowrite32(0x00000002 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000004*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000005*4);
  iowrite32(0x00000080 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000A*4);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000B*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000C*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000D*4);
  iowrite32(0x0000000C , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000E*4);
  iowrite32(0x00000032 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000000F*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000010*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000011*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000012*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000013*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000014*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000015*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000016*4);
  iowrite32(0x00000007 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000017*4);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000018*4);
  iowrite32(0x00000010 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000019*4);
  iowrite32(0x00000001 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000001A*4);
  iowrite32(0x0000000A , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x0000001B*4);
  iowrite32(0x00010000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x00000021*4);
  iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+0x000001c0*4);
#else
    for(i = 0; i < data_size; i = i + 2)
    { 
      //rumboot_printf("[%d] : %x : %x\n", i, data_ptr[i+1], NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+data_ptr[i]*4);
      data_ptr[i] = data_ptr[i]+0x800;
    }
#endif
/////////////////////////////////////////////////////////////////////////////////////////
    
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
    //nu_mpe_run(MY_MPE_REGS_BASE, iteration_desc.cfg_mpe);

/////////////////////////////////////////////////////////////////////////////////////////
    iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000008);
    iowrite32(0x00030001 , NPE_BASE+NA_MPE_BASE+0x00000000);
    iowrite32(0x00000000 , NPE_BASE+NA_MPE_BASE+0x00000408);
    iowrite32(0x00030001 , NPE_BASE+NA_MPE_BASE+0x00000400);
#ifndef MACRO
    nu_npe_cmd_dma_setup(NPE_BASE, (uint32_t)data_ptr, data_size*4);
    nu_npe_cmd_dma_run(NPE_BASE);
    nu_npe_cmd_dma_wait_page_complete(NPE_BASE);
#endif
    iowrite32(0, NPE_BASE + NA_CU_REGS_BASE + NA_UNITS_MODE);

    //iteration_desc.cfg_vpe->mark = 1;
/////////////////////////////////////////////////////////////////////////////////////////
    
    
      // Wait For The Corresponding DMA Channels To Complete
    if(iteration_desc.PPE_ENABLED==Enable_En)
      nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    else
      nu_vpe_wait_custom(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
    
    rumboot_printf("Comparing..\n");

////////////////////////////////////////////////////////////////////////////////////////////
//nu_calc_mpe_cube_out_metrics(iteration_desc.cfg_mpe, &iteration_desc.mpe_out_metrics);

unsigned int size = (iteration_desc.cfg_mpe->W - iteration_desc.cfg_mpe->S) / iteration_desc.cfg_mpe->Sw + 1;
unsigned int batch = iteration_desc.cfg_mpe->batch_size + 1;
unsigned int bstep = 128 / batch;
unsigned int bstride = bstep * batch;
unsigned int bnum = size / bstep + ((size % bstep) != 0);
unsigned int knum = iteration_desc.cfg_mpe->K;
unsigned int istride = 0;
unsigned int ib = 0;
unsigned int ik16 = 0;
unsigned int ik = 0;
unsigned int bcap = 0;

rumboot_printf("dimensions W: %d, S: %d, Sw: %d\n",iteration_desc.cfg_mpe->W ,iteration_desc.cfg_mpe->S,iteration_desc.cfg_mpe->Sw);
rumboot_printf("dimensions out size: %d, batch: %d\n",size,batch);


unsigned int element_step = bstep;

unsigned int cube_parts = ((iteration_desc.cfg_mpe->W - iteration_desc.cfg_mpe->S) / iteration_desc.cfg_mpe->Sw + 1) / element_step;
unsigned int cube_last  = ((iteration_desc.cfg_mpe->W - iteration_desc.cfg_mpe->S) / iteration_desc.cfg_mpe->Sw + 1) % element_step;

unsigned int result_size_x  = ((iteration_desc.cfg_mpe->W - iteration_desc.cfg_mpe->S) / iteration_desc.cfg_mpe->Sw + 1);
unsigned int result_size_y  = ((iteration_desc.cfg_mpe->H - iteration_desc.cfg_mpe->R) / iteration_desc.cfg_mpe->Sh + 1);

unsigned int iKs128   = 0;
unsigned int iKs    = 0;
unsigned int iwidth   = 0;
unsigned int ik_temp    = 0;
unsigned int iwidth_temp  = 0;
unsigned int istridex   = 0;
unsigned int istridey   = 0;
unsigned int ik16_temp  = 0;
unsigned int iKs16    = 0;
unsigned int ibatch   = 0;

unsigned int *buf = rumboot_malloc_from_heap_aligned(heap_id, knum * batch * result_size_x * result_size_y * sizeof(short int), 8);
  
unsigned int data_value = 0;

unsigned int iter = 0;

#ifdef CONVERT_NEW_FEATURE
rumboot_printf("step 1 converting\n");

for(istride = 0; istride < knum * batch * result_size_x * result_size_y; istride++)
{
  buf[istride] = 0;
}

iter = 0;

unsigned int istep = 0;
unsigned int ibstep = 0;

//WARNING!!! untested feature
for(iKs128 = 0; iKs128 < ((iteration_desc.cfg_mpe->K / 128) + ((iteration_desc.cfg_mpe->K % 128) != 0)); iKs128++)
{
  ik_temp = (((iteration_desc.cfg_mpe->K % 128) != 0) && (iKs128 == ((iteration_desc.cfg_mpe->K / 128) + ((iteration_desc.cfg_mpe->K % 128) != 0) - 1))) ? (iteration_desc.cfg_mpe->K % 128) : 128;
  for(istridey = 0; istridey < (iteration_desc.cfg_mpe->H - iteration_desc.cfg_mpe->R) / iteration_desc.cfg_mpe->Sh + 1; istridey++)
  {
    for(iKs = 0; iKs < ((ik_temp / 16) + ((ik_temp % 16) != 0)); iKs++)
    {
      for(istep = 0; istep < batch; istep++)
      {
        for(ibstep = 0; ibstep < result_size_x; ibstep++)
        {
          for(iKs16 = 0; iKs16 < 16; iKs16++)
          {
            buf[iKs16 + (ibstep*batch+istep + (iKs + ik_temp * (istridey + ((iteration_desc.cfg_mpe->H - iteration_desc.cfg_mpe->R) / iteration_desc.cfg_mpe->Sh + 1) * (iKs128))) * (batch * result_size_x))*16] = ((unsigned short int *)(iteration_desc.res_data))[iter++];
          }
        }
      }
    }
  }
}

iter = 0;

rumboot_printf("step 2 converting\n");

//rumboot_printf("refill buf\n");
for(istride = 0; istride < knum * batch * result_size_x * result_size_y; istride++)
{
  //((unsigned short int *)(iteration_desc.res_data))[istride] = buf[istride];
  ((unsigned short int *)(iteration_desc.res_data))[istride] = buf[istride];
  //if((istride % knum) == 0) {rumboot_printf("etalon: %x\n", buf[istride]);} 
}

rumboot_printf("converting DONE\n");
#endif
unsigned int passed = 0;

for(ik = 0; ik < knum * batch * result_size_x * result_size_y; ik++)
{
  if(((unsigned short int *)(iteration_desc.etalon))[ik] != ((unsigned short int *)(iteration_desc.res_data))[ik])
  {
    if((ik % knum) == 0)
    {
      rumboot_printf("mismatch: %x : %x\n", ((unsigned short int *)(iteration_desc.etalon))[ik], ((unsigned short int *)(iteration_desc.res_data))[ik]);
    }
    passed = 1;
  }
  else
  {
    //if((ik % knum) == 0)
    {
      //rumboot_printf("   match: %x : %x\n", ((unsigned short int *)(iteration_desc.etalon))[ik], ((unsigned short int *)(iteration_desc.res_data))[ik]);
    }
  }
}

if(passed) return 1;
////////////////////////////////////////////////////////////////////////////////////////////
    /*
      // Result vs Etalon Comparision
    if(NU_COMPARE_FUNCTION(iteration_desc.res_data, iteration_desc.etalon, (iteration_desc.res_metrics->s*(iteration_desc.cfg_vpe->wdma_config.dma_bsize+1))) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      //nu_mpe_print_config(iteration_desc.cfg_mpe);
      //nu_vpe_print_config(iteration_desc.cfg_vpe);
      //if(iteration_desc.PPE_ENABLED==Enable_En)
      //  nu_ppe_print_config(iteration_desc.cfg_ppe);
      
      rumboot_printf("Test FAILED at invocation %d\n",i);

      return 1;
    }
*/
      // Point At The Next Iteration Data
    nu_npe_iterate_desc(&iteration_desc);
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}
