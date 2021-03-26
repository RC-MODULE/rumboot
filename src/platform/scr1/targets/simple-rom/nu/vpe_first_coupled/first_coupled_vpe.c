
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

void nu_vpe_decide_dma_config (ConfigVPE* cfg, ConfigDMAVPE* cfg_dma) {
  nu_vpe_decide_dma_config_trivial(cfg,cfg_dma);
  cfg_dma->dma_dst_en = Enable_NotEn; // Волюнтари3м
}

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

int main() {
  int res_size;
  int in_size;
  int in_data_item_size;
  int out_data_item_size;
  
  in_size=16*16*16;//H=16;W=16;C=16; // CHECK
  res_size=in_size;
  
  rumboot_printf("Hello\n");
  
  cfg_bin = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  rumboot_platform_request_file("cfg_file_tag", (uintptr_t)cfg_bin);
  
  nu_vpe_load_config(&cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  
  if(cfg.in_data_type == DataTypeExt_Fp32 || cfg.in_data_type == DataTypeExt_Int32)
    in_data_item_size=4;
  else if(cfg.in_data_type == DataTypeExt_Int16 || cfg.in_data_type == DataTypeExt_Fp16)
    in_data_item_size=2;
  else if(cfg.in_data_type == DataTypeExt_Int8)
    in_data_item_size=1;
  else {
    rumboot_printf("ERROR: Unknown in_data_type 0x%x",(uint32_t)cfg.in_data_type);
    return -1;
  }
  
  if(cfg.out_data_type == DataType_Int16 || cfg.out_data_type == DataType_Fp16)
    out_data_item_size=2;
  else if(cfg.out_data_type == DataType_Int8)
    out_data_item_size=1;
  else {
    rumboot_printf("ERROR: Unknown out_data_type 0x%x",(uint32_t)cfg.out_data_type);
    return -1;
  }
  
  in_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,in_size*in_data_item_size /*size in bytes*/,64/*aligned by 64 bytes*/);
  rumboot_platform_request_file("in_file_tag", (uintptr_t)in_data); // What is "in_file_tag" defined in scr1.cmake
  
  res_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*out_data_item_size , 32);
  memset(res_data,0xA5,res_size*out_data_item_size);
  
  etalon = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*out_data_item_size, 32);
  rumboot_platform_request_file("etalon_file_tag",(uintptr_t) etalon);
  
  //print_in_data(in_data,in_size);
  
  nu_vpe_print_config(&cfg);
  nu_vpe_print_config_dma(&cfg_dma);
  
  nu_vpe_decide_dma_config(&cfg,&cfg_dma);
  nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg, &cfg_dma);
  
    // Setup Main Channel DMAs if Required
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_size*in_data_item_size);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_size*out_data_item_size);
  
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
  
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_size*out_data_item_size);
  
  rumboot_printf("Comparing..\n");
  
  if(memcmp((char*)res_data,(char*)etalon,res_size*out_data_item_size) != 0) {
    rumboot_printf("Test FAILED\n");
    return -1;
  }
  
  rumboot_printf("Test PASSED\n");
  
  return 0;
  
}
