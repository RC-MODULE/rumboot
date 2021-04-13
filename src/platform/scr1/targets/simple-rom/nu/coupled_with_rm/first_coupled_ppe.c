
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h>

#include <platform/devices/nu_lib.h>
#include <platform/devices/nu_test_lib.h>
#include <regs/regs_nu_ppe.h>

int nu_ppe_decide_dma_config(ConfigPPE* cfg, CubeMetrics* in_cube_metrics, CubeMetrics* out_cube_metrics, ConfigREGPPE* cfg_reg) {
  cfg_reg->rOpEn=0; // Волюнтари3м
  return nu_ppe_decide_dma_config_trivial(cfg, in_cube_metrics, out_cube_metrics, cfg_reg);
}

void *in_data;
void *etalon;
void *res_data;


ConfigPPE cfg;
ConfigREGPPE cfg_reg = {0};

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

uint32_t dma_loop = 1;

int main() {
  int heap_id;

  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();

  if(nu_ppe_load_cfg(heap_id, &cfg) != 0) return -1;

  in_metrics = nu_ppe_load_in_metrics(heap_id);
  if(in_metrics == NULL) return -1;

  res_metrics= nu_ppe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;

  in_data = nu_ppe_load_in_data(heap_id,in_metrics);
  if(in_data == NULL) return -1;
  cfg_reg.rBALs = (uintptr_t)in_data;

  res_data = nu_ppe_malloc_res(heap_id, res_metrics);
  if(res_data == NULL) return -1;
  cfg_reg.wBALd = (uintptr_t)res_data;


  etalon = nu_ppe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;

  //print_in_data(in_data,in_size);

  nu_ppe_print_config(&cfg);
  if(nu_ppe_decide_dma_config(&cfg,in_metrics,res_metrics,&cfg_reg) != 0) return -1;
  nu_ppe_print_config_reg(&cfg_reg);

  if(dma_loop == 1){
    cfg_reg.wESd  = cfg_reg.rESs;
    cfg_reg.wVSd  = cfg_reg.rVSs;
    cfg_reg.wLSd  = cfg_reg.rLSs;
    cfg_reg.wPCo  = cfg_reg.rPCi;
    cfg_reg.wPWo  = cfg_reg.rPWi;
    cfg_reg.wPHo  = cfg_reg.rPHi;
    cfg_reg.wBSWo = cfg_reg.rBSWi;
    cfg_reg.wBSHo = cfg_reg.rBSHi;
    cfg_reg.wBSCo = cfg_reg.rBSCi;
    cfg_reg.wStWo = cfg_reg.rStWi;
    cfg_reg.wOfWo = cfg_reg.rOfWi;
    nu_ppe_setup_reg(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_reg);

    cfg_reg.rOpEn  = 0X00000001; // Set start of PPE RDMA field to active value
    cfg_reg.wOpEn  = 0X00000001; // Set start of PPE+WDMA field to active value
    // Start RDMA then PPE+WDMA
    nu_ppe_rdma_run(NU_PPE_RDMA_BASE, &cfg_reg);
    nu_ppe_run(NU_PPE_STANDALONE_BASE, &cfg_reg);
    // Wait RDMA then PPE+WDMA
    nu_ppe_rdma_wait_complete(NU_PPE_RDMA_BASE);
    nu_ppe_wait_complete(NU_PPE_STANDALONE_BASE);
  }

  if(cfg_reg.rOpEn==0)
    nu_ppe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);

  nu_ppe_setup(NU_PPE_STANDALONE_BASE, &cfg, &cfg_reg);


  rumboot_printf("Running DMA..\n");

    // Invoke Required DMA Channels
  if(cfg_reg.rOpEn==0)
    nu_ppe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  else
    nu_ppe_rdma_run(NU_PPE_RDMA_BASE, &cfg_reg);
  nu_ppe_run(NU_PPE_STANDALONE_BASE, &cfg_reg);

    // Finalize Required DMA Channels
    // Uncomment This When Actual Proper Work Debugged
  /*
  if(cfg_reg.rOpEn==0)
    nu_ppe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
  else
    nu_ppe_rdma_wait_complete(NU_PPE_RDMA_BASE);
  nu_ppe_wait_complete(NU_PPE_STANDALONE_BASE);
  */


  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);

  rumboot_printf("Comparing..\n");

  if(dma_loop == 1){
    if(nu_bitwise_compare(res_data,in_data,in_metrics->s) == 0)
      rumboot_printf("Test PASSED\n");
    else {
      rumboot_printf("Test FAILED\n");
      return 1;
    }
  } else {
    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Test PASSED\n");
    else {
      rumboot_printf("Test FAILED\n");
      return 1;
    }
  }

  return 0;

}
