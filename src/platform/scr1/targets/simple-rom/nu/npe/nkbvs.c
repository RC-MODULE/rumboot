#define FORCE_VPE_WDMA_EN

#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <devices/rcm_na/nu_lib.h>
#include <devices/rcm_na/nu_test_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>

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
   rumboot_printf("nu_vpe_decide_dma_config \n");

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

  cfg->src_rdma_config.dma_axi_len = 15;
  cfg->op0_rdma_config.dma_axi_len = 15;
  cfg->op1_rdma_config.dma_axi_len = 15;
  cfg->op2_rdma_config.dma_axi_len = 15;
  cfg->wdma_config.dma_axi_len     = 15;
}

void nu_run_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics, uintptr_t mpe_dma_r_baddr, uintptr_t mpe_dma_w_baddr) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;
  uint32_t result;
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8);
  rumboot_printf("Configuring and starting MPE..\n");
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;

    ptr++;
    if(offset == MPE_RDMA_D_BASE+BFCA_MSha) {data = nu_virt_to_dma(mpe_dma_r_baddr);}
    if(offset == MPE_RDMA_W_BASE+BFCA_MSha) {data = nu_virt_to_dma(mpe_dma_w_baddr);}
    rumboot_printf("Writing data=0x%x, offset = 0x%x\n", data, offset);
    iowrite32(data,base + offset);
  }
}

CubeMetrics* in_metrics;
void *in_data;

WarrMetrics* warr_metrics;
void *warr;

MPECmdMetrics* cmd_metrics;
void *cmd;

CubeMetrics* res_metrics;
uint16_t* res_data;
uint16_t* etalon;

int main()
{
  int heap_id;

  rumboot_printf("START NKBVS TEST\n");

  heap_id = nu_get_heap_id();
  
  ConfigVPE cfg_vpe;
  char* fn_cfg_base = "cfg_file_tag"   ;
  char  fn_cfg[32];

  if(nu_vpe_load_cfg_by_tag(heap_id, &cfg_vpe, fn_cfg_base) != 0) return -1;


  cmd_metrics = nu_mpe_load_cmd_metrics(heap_id);
  if(cmd_metrics == NULL) return -1;
  
  cmd = nu_mpe_load_cmd(heap_id,cmd_metrics);
  if(cmd == NULL) return -1;

  in_metrics = nu_mpe_load_in_metrics(heap_id);
  if(in_metrics == NULL) return -1;
  
  in_data = nu_mpe_load_in_data(heap_id,in_metrics);
  if(in_data == NULL) return -1;

  warr_metrics = nu_mpe_load_warr_metrics(heap_id);
  if(warr_metrics == NULL) return -1;
  
  warr = nu_mpe_load_warr(heap_id,warr_metrics);
  if(warr == NULL) return -1;

  res_metrics= nu_mpe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;

  res_data = nu_mpe_malloc_res(heap_id,res_metrics);
  if(res_data == NULL) return -1;

  etalon = nu_mpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;

  // MPE
  nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);
  
  // VPE
  DataTypeExt in_data_type = DataTypeExt_Int32;
  DataType out_data_type = DataType_Int16;

  CubeMetrics* vpe_in_metrics;
  uint32_t size;
  size = sizeof(CubeMetrics);

  vpe_in_metrics = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));

  vpe_in_metrics->C = vpe_in_c;
  vpe_in_metrics->W = vpe_in_w;
  vpe_in_metrics->H = vpe_in_h;
  vpe_in_metrics->s = vpe_in_metrics->C * vpe_in_metrics->W * vpe_in_metrics->H * 2;

  nu_vpe_decide_dma_config(&cfg_vpe,vpe_in_metrics,15,0,0,0,0,res_metrics,res_data);
  nu_vpe_setup(NPE_BASE+NA_VPE_BASE, &cfg_vpe);

  //nu_vpe_print_config(&cfg_vpe);
  
  nu_vpe_run(NPE_BASE+NA_VPE_BASE, &cfg_vpe);
  nu_vpe_wait(NPE_BASE+NA_VPE_BASE, &cfg_vpe);
  rumboot_printf("End VPE\n");

  rumboot_printf("Comparing...\n");

  // Compare arrays
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }

  return 0;
}
