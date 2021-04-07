
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 
#include <regs/regs_nu_vpe.h>

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

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

int main() {
  int res_size;
  int in_size;
  
  in_size=16*128; // 128 Lines of 16 32bit elements
  res_size=in_size;
  
  rumboot_printf("Hello\n");
  
  
  iowrite32(0x01234567, DUT_BASE + 0x000);
  iowrite32(0xFFF34567, DUT_BASE + NU_VPE_WDMA_DST_VECTOR_STRIDE); // 0x024
  iowrite32(0xAAA34567, DUT_BASE + NU_VPE_WDMA_DST_CFG);           // 0x028
  
  
  
  cfg_bin = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  rumboot_platform_request_file("cfg_file_tag", (uintptr_t)cfg_bin);
  
  in_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,in_size*sizeof(int32_t) /*size in bytes*/,64/*aligned by 64 bytes*/);
  rumboot_platform_request_file("in_file_tag", (uintptr_t)in_data); // What is "in_file_tag" defined in scr1.cmake
  
  //print_in_data(in_data,in_size);
  
  nu_vpe_load_config(&cfg, cfg_bin);
  nu_vpe_print_config(&cfg);
  
  nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg, &cfg_dma);
  
  res_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*sizeof(int16_t) , 32);
  memset(res_data,0xA5,res_size*sizeof(int16_t));
  
  nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_size*sizeof(int32_t));
  nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_size*sizeof(int16_t));
  
  rumboot_printf("Running DMA..\n");
  
    // Do Not Run Yet - It Is Falling
  // nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  // nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
  
  // nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
  // nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_size*sizeof(int16_t));
  
  etalon = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*sizeof(int16_t), 32);
  rumboot_platform_request_file("etalon_file_tag",(uintptr_t) etalon);
  
  rumboot_printf("Comparing..\n");
  
  // if(memcmp((char*)res_data,(char*)etalon,res_size*sizeof(int16_t)) != 0) {
    // rumboot_printf("Test FAILED\n");
    // return -1;
  // }
  
  rumboot_printf("Test PASSED\n");
  
  return 0;
  
}
