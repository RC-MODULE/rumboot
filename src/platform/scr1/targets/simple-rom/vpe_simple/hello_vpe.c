
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 


int32_t *in_data;
int16_t *etalon;
int16_t *res_data;

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

int main() {
  int res_size;
  int in_size;
  
  in_size=16*128; // 128 Lines of 16 32bit elements
  res_size=in_size;
  
  rumboot_printf("Hello\n");
  
  in_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,in_size*sizeof(int32_t) /*size in bytes*/,64/*aligned by 64 bytes*/);
  rumboot_platform_request_file("in_file_tag", (uintptr_t)in_data); // What is "in_file_tag" defined in scr1.cmake
  
  //print_in_data(in_data,in_size);
  
  res_data = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*sizeof(int16_t) , 32);
  
  nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_size*sizeof(int32_t));
  nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_size*sizeof(int16_t));
  
  nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
  
  nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
  nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_size*sizeof(int16_t));
  
  etalon = rumboot_malloc_from_heap_aligned(NU_VPE_HEAPID,res_size*sizeof(int16_t), 32);
  rumboot_platform_request_file("etalon_file_tag",(uintptr_t) etalon);
  
  if(memcmp((char*)res_data,(char*)etalon,res_size*sizeof(int16_t)) != 0) {
    rumboot_printf("Test FAILED\n");
    return -1;
  }
  
  return 0;
  
}
