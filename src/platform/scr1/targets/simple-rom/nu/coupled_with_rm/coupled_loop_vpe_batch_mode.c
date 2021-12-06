
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <stdbool.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

#include "devices/rcm_na/nu_vpe_file_tags.h"

void *in_data;
// void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;
void *lut1;
void *lut2;

void** etalon; // Need To Store Many etalon Arrays

void nu_vpe_set_op_pointers (
  ConfigVPE* cfg,
  void*op0,
  void*op1,
  void*op2
){
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2;
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  CubeMetrics*res_metrics,
  void*res_data
) {
  rumboot_printf("nu_vpe_decide_dma_config \n");
  
  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
  cfg->dst_flying = Enable_NotEn;

#ifdef VPE_TraceMode_PPE
  cfg->trace_mode = TraceMode_PPE;
#else
  cfg->trace_mode = TraceMode_MPE;
#endif
  
  cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode; // Init Them
  cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode;
  cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;
  cfg->wdma_config.dma_data_mode     = Mode_Element; // Copypaste From src_rdma_config in nu_vpe_decide_dma_config_trivial
  cfg->wdma_config.dma_data_use      = DmaDUse_Off;
  
  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  cfg->wdma_config.dma_en = Enable_En;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) in_data;
  cfg->wdma_config.dma_baddr     = (uint32_t) res_data;

  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;
  
}

int nu_vpe_update_batch_parameters(ConfigVPE* cfg, bool second_turn, bool last_turn, void* in_prev, void* in_curr, void* res_prev, void* res_curr) {
  uint32_t in_prev_uint;
  uint32_t in_curr_uint;
  uint32_t in_diff;
  uint32_t res_prev_uint;
  uint32_t res_curr_uint;
  uint32_t res_diff;
  
  in_prev_uint = (uint32_t) in_prev;
  in_curr_uint = (uint32_t) in_curr;
  in_diff = in_curr_uint - in_prev_uint;
  
  res_prev_uint = (uint32_t) res_prev;
  res_curr_uint = (uint32_t) res_curr;
  res_diff = res_curr_uint - res_prev_uint;
  
  if(second_turn) {
    cfg->src_rdma_config.dma_bstride = in_diff;
    cfg->    wdma_config.dma_bstride = res_diff;
  }
  else {
    if(cfg->src_rdma_config.dma_bstride != in_diff) {
      rumboot_printf("Bad Allocation of Batch In items: dma_bstride=0x%x, curr=0x%x, prev=0x%x\n",
        cfg->src_rdma_config.dma_bstride,in_curr_uint,in_prev_uint
      );
      return -1;
    }
    if(cfg->    wdma_config.dma_bstride != res_diff) {
      rumboot_printf("Bad Allocation of Batch Res items: dma_bstride=0x%x, curr=0x%x, prev=0x%x\n",
        cfg->    wdma_config.dma_bstride,res_curr_uint,res_prev_uint
      );
      return -1;
    }
  }
  
  cfg->src_rdma_config.dma_bsize++;
  cfg->    wdma_config.dma_bsize++;
  
  if(last_turn) {
    cfg->cube_size = (cfg->cube_size+1) * (cfg->wdma_config.dma_bsize+1) - 1;
  }
  
  return 0;
}

ConfigVPE cfg;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

VPEStatusRegs status_regs_etalon;

int main() {
  int heap_id;
  int i;
  int iterations;
  bool last_turn;
  int turn_index;
  
  uint32_t misalign; // Misaligns Operands Only
  uint8_t  axi_len;
  
  void* in_data_prev;
  void* res_data_prev;
  
  void* in_data_first;
  void* res_data_first;
  
#ifdef MISALIGN_EN
  misalign = IntMisalign;
#else
  misalign = 0;
#endif

#ifdef AXI_LEN
  axi_len = AxiLen;
#else 
  axi_len = 15;
#endif

 
  rumboot_printf("Hello Batch Mode\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  etalon = rumboot_malloc_from_heap(0, sizeof(void*) * iterations); // Allocate The Array Of Pointers To Etalon Data
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif


  turn_index=0;
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
    
    if(turn_index==0) {
      if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, cfg_file_tag[i]) != 0) return -1;
    }
    
    in_metrics = nu_load_cube_metrics(heap_id,metrics_in_tag[i]);
    if(in_metrics == NULL) return -1;
    
    last_turn = in_metrics->role == CubeRole_LastInBatch;
    
    res_metrics= nu_load_cube_metrics(heap_id,metrics_etalon_tag[i]);
    if(res_metrics == NULL) return -1;

    in_data = nu_load_cube(heap_id,in_file_tag[i],in_metrics);
    if(in_data == NULL) return -1;
    
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
    etalon[i] = nu_load_cube(heap_id,etalon_file_tag[i],res_metrics);
    if(etalon[i] == NULL) return -1;
    
    if(turn_index==0) {
      nu_vpe_decide_dma_config(&cfg,in_metrics,axi_len,in_data,res_metrics,res_data);
      
        // Store The In And Out Batch Bases
      res_data_first = res_data;
      in_data_first = in_data;
      
    }
    else {  // turn_index != 0 // All Other Turns
      if(nu_vpe_update_batch_parameters(&cfg,turn_index==1,last_turn,in_data_prev,in_data,res_data_prev,res_data) != 0) {
        return -1;
      }
    }
    
    if(last_turn) {
      char* res_ptr;
      char* in_data_ptr;
      int k;
      
      k=i-turn_index; // Iteration Which If The First For This Turn
        // Load OP0-OP2 Operands If Needed
      if(cfg.op0_en==Enable_En) {
        op0 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op0_config,metrics_op0_cube_tag[k],metrics_op0_vec_tag[k],op0_cube_file_tag[k],op0_vec_file_tag[k],misalign);
      }
      else op0 = NULL;
      if(cfg.op1_en==Enable_En) {
        op1 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op1_config,metrics_op1_cube_tag[k],metrics_op1_vec_tag[k],op1_cube_file_tag[k],op1_vec_file_tag[k],misalign);
      }
      else op1 = NULL;
      if(cfg.op2_en==Enable_En) {
        op2 = nu_vpe_load_op2_misaligned_by_tags(heap_id,&cfg.op2_config,metrics_op2_cube_tag[k],metrics_op2_vec_tag[k],op2_cube_file_tag[k],op2_vec_file_tag[k],misalign);
      }
      else op2 = NULL;
    
        // Load LUTs If Needed
      if(cfg.op2_config.lut_en == Enable_En) {
        lut1_metrics = nu_load_vec_metrics(heap_id,metrics_lut1_file_tag[k]);
        lut2_metrics = nu_load_vec_metrics(heap_id,metrics_lut2_file_tag[k]);
        lut1 = nu_load_vec(heap_id,lut1_file_tag[k],lut1_metrics);
        lut2 = nu_load_vec(heap_id,lut2_file_tag[k],lut2_metrics);
        nu_vpe_load_lut(MY_VPE_REGS_BASE,lut1,lut2);
      }
      
      nu_vpe_set_op_pointers(&cfg,op0,op1,op2);
      
    //~ if(..) {
      //~ if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,status_regs_file_tag[i]) != 0) return -1;
    //~ }

      nu_vpe_print_config(&cfg);
      // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
      
      nu_vpe_setup(MY_VPE_REGS_BASE, &cfg);
      
        // Setup Main Channel DMAs if Required
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
      
      rumboot_printf("Running DMA..\n");
      
        // Invoke Required DMA Channels
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
      
      nu_vpe_run(MY_VPE_REGS_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
      
      nu_vpe_wait(MY_VPE_REGS_BASE, &cfg);
      
        // Finalize Required DMA Channels
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
      
      rumboot_printf("Comparing..\n");
      
      res_ptr = res_data_first;
      in_data_ptr = in_data_first;
      for(int j=k; j<=i; j++) { // Iterate From The Batch Start To The Current Iteration
        rumboot_printf("Comparing iteration %d: res=0x%x, etalon=0x%x, %d bytes\n",j,(uint32_t)res_ptr,(uint32_t)etalon[j], res_metrics->s);
        
        if(nu_bitwise_compare(res_ptr,etalon[j],res_metrics->s) == 0)
          rumboot_printf("Iteration %d PASSED\n",j);
        else {
          rumboot_printf("Test FAILED at iteration %d\n",j);
          nu_vpe_interpret_mismatch(&cfg,res_ptr,etalon[j],in_data_ptr,op0,op1,op2,res_metrics->s,res_metrics->C);
          return 1;
        }
        
        res_ptr     += cfg.    wdma_config.dma_bstride;
        in_data_ptr += cfg.src_rdma_config.dma_bstride;
      }
    }
    
    
      // Iteration Info Switch
    in_data_prev = in_data;
    res_data_prev = res_data;
    
    turn_index = last_turn ? 0 : turn_index+1;
  }
  
  return 0;
  
}
