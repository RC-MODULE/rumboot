
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <string.h>

int32_t *in_data;
int16_t *etalon;
int16_t *res_data;
uint32_t *cfg_bin;
void *op0;
void *op1;
void *op2;

#define NU_VPE_HEAPID 1

void print_in_data(int32_t * in_data, int in_size){
  int lines;
  int ptr;
  
  lines = in_size / 16;
  ptr=0;
  
  for(int i=0;i<lines;i++) {
    rumboot_printf("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                   in_data[ptr+0],in_data[ptr+1],in_data[ptr+2],in_data[ptr+3],
                   in_data[ptr+4],in_data[ptr+5],in_data[ptr+6],in_data[ptr+7],
                   in_data[ptr+8],in_data[ptr+9],in_data[ptr+10],in_data[ptr+11],
                   in_data[ptr+12],in_data[ptr+13],in_data[ptr+14],in_data[ptr+15]);
    ptr = ptr + 16;
  }
}

void nu_vpe_decide_dma_config (ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma) {
  nu_vpe_decide_dma_config_trivial(cfg,metrics,cfg_dma);
  cfg_dma->dma_dst_en = Enable_NotEn; // Волюнтари3м
}


void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP0 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(index?"metrics_op1_cube_tag":"metrics_op0_cube_tag",(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file(index?"op1_cube_file_tag":"op0_cube_file_tag",(uintptr_t)op);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(index?"metrics_op1_vec_tag":"metrics_op0_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file(index?"op1_vec_file_tag":"op0_vec_file_tag",(uintptr_t)op);
    return op;
  } 
  
  return NULL;
}
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP2 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file("metrics_op2_cube_tag",(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file("op2_cube_file_tag",(uintptr_t)op);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file("metrics_op2_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file("op2_vec_file_tag",(uintptr_t)op);
    return op;
  } 
  
  return NULL;
}

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

int main() {
  
  rumboot_printf("Hello\n");
  
  cfg_bin = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  rumboot_platform_request_file("cfg_file_tag", (uintptr_t)cfg_bin);
  
  nu_vpe_load_config(&cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  
  in_metrics = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,sizeof(CubeMetrics),sizeof(int32_t));
  rumboot_platform_request_file("metrics_in_tag",(uintptr_t)in_metrics);
  
  res_metrics= rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,sizeof(CubeMetrics),sizeof(int32_t));
  rumboot_platform_request_file("metrics_etalon_tag",(uintptr_t)res_metrics);
  
  in_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,in_metrics->s /*size in bytes*/,64/*aligned by 64 bytes*/);
  rumboot_platform_request_file("in_file_tag", (uintptr_t)in_data); // What is "in_file_tag" defined in scr1.cmake
  
  res_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_metrics->s , 32);
  memset(res_data,0xA5,res_metrics->s);
  
    // Load OP0-OP2 Operands If Needed
  if(cfg.op0_en==Enable_En) {
    op0 = nu_vpe_load_op01(NU_VPE_HEAPID,&cfg.op0_config,0);
  }
  if(cfg.op1_en==Enable_En) {
    op1 = nu_vpe_load_op01(NU_VPE_HEAPID,&cfg.op1_config,1);
  }
  if(cfg.op2_en==Enable_En) {
    op2 = nu_vpe_load_op2(NU_VPE_HEAPID,&cfg.op2_config);
  }
  
  etalon = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_metrics->s, 32);
  rumboot_platform_request_file("etalon_file_tag",(uintptr_t) etalon);
  
  //print_in_data(in_data,in_size);
  
  nu_vpe_print_config(&cfg);
  nu_vpe_print_config_dma(&cfg_dma);
  
  nu_vpe_decide_dma_config(&cfg,in_metrics,&cfg_dma);
  nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg, &cfg_dma);
  
    // Setup Main Channel DMAs if Required
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
  
  rumboot_printf("Running DMA..\n");
  
    // Invoke Required DMA Channels
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
  nu_vpe_run(NU_VPE_STANDALONE_BASE, &cfg_dma);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
  
    // Finalize Required DMA Channels
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg_dma);
  
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
  
  rumboot_printf("Comparing..\n");
  
  if(memcmp((char*)res_data,(char*)etalon,res_metrics->s) != 0) {
    rumboot_printf("Test FAILED\n");
    return -1;
  }
  
  rumboot_printf("Test PASSED\n");
  
  return 0;
  
}
