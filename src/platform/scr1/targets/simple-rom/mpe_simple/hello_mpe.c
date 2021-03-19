
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 


int16_t *in_data;
int32_t *etalon;
int32_t *res_data;
uint32_t *cfg_bin;

#define NU_HEAPID 1

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

ConfigMPE cfg;

int main() {
  int res_size;
  int in_size;
  
  in_size=16*128; // 128 Lines of 16 elements
  res_size=in_size; // Not Equal for MPE ???
  
  rumboot_printf("Hello\n");
  
  cfg_bin = rumboot_malloc_from_heap_aligned(NU_HEAPID,NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  rumboot_platform_request_file("cfg_file_tag", (uintptr_t)cfg_bin);
  
  in_data = rumboot_malloc_from_heap_aligned(NU_HEAPID,in_size*sizeof(int16_t) /*size in bytes*/,64/*aligned by 64 bytes*/);
  rumboot_platform_request_file("in_file_tag", (uintptr_t)in_data); // What is "in_file_tag" defined in scr1.cmake
  
  //print_in_data(in_data,in_size);
  
  nu_mpe_load_config(&cfg, cfg_bin);
  nu_mpe_print_config(&cfg);
  
  nu_mpe_setup(NU_MPE_STANDALONE_BASE, &cfg);
  
  res_data = rumboot_malloc_from_heap_aligned(NU_HEAPID,res_size*sizeof(int32_t) , 64);
  memset(res_data,0xA5,res_size*sizeof(int32_t));
  
  nu_mpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_size*sizeof(int32_t));
  
  rumboot_printf("Running DMA..\n");
  nu_mpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
  
  // Uncomment This When DUT Will Actually Issue A Data
  // nu_mpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  //
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_size*sizeof(int32_t));
  
  etalon = rumboot_malloc_from_heap_aligned(NU_HEAPID,res_size*sizeof(int32_t), 32);
  rumboot_platform_request_file("etalon_file_tag",(uintptr_t) etalon);
  
  rumboot_printf("Comparing..\n");
  
  if(memcmp((char*)res_data,(char*)etalon,res_size*sizeof(int32_t)) != 0) {
    rumboot_printf("Test FAILED\n");
    return -1;
  }
  
  rumboot_printf("Test PASSED\n");
  
  return 0;
  
}
