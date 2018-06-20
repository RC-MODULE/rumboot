#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

static uint8_t res8;
static uint16_t res16;
static uint32_t res32;
static uint64_t res64;

static int check_cpu_ppc_im0_0() 
{
   register uint8_t _res8;  
   register uint16_t _res16;
   register uint32_t _res32, r64_output1, r64_output2;
   register uint64_t _res64;

   iowrite8(0xDD, (uint32_t)&res8);
   _res8 = ioread8((uint32_t)&res8);
   if(_res8 == 0xDD )
   {
      rumboot_printf("Result: 0x%x \n", _res8);
   } 
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
     
   iowrite16(0x5454, (uint32_t)&res16);
   _res16 = ioread16((uint32_t)&res16);
   
   if(_res16 == 0x5454 )
   {
      rumboot_printf("Result: 0x%x \n", _res16);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
  
   iowrite32(0x75757575, (uint32_t)&res32);
   _res32 = ioread32((uint32_t)&res32);
   
   if(_res32 == 0x75757575 )
   {
      rumboot_printf("Result: 0x%x \n", _res32);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
   
   iowrite64(0x5555555522222222, (uint32_t)&res64);
   _res64 = ioread64((uint32_t)&res64);
   
   if(_res64 == 0x5555555522222222)
   {
      r64_output1 = (uint32_t)(_res64 >> 32);
      r64_output2 = (uint32_t)_res64 & 0xFFFFFFFF;
      rumboot_printf("Result: 0x%x%x\n", r64_output1, r64_output2);
   }
   else
   { 
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
   return 1;
}

static int check_cpu_ppc_im0_1() 
{
   register uint8_t _res8;  
   register uint16_t _res16;
   register uint32_t _res32, _r64_output1, _r64_output2;
   register uint64_t _res64;
   
   
   iowrite8(0xBB, (uint32_t)&res8);
   _res8 = ioread8((uint32_t)&res8);
   
   if(_res8 == 0xBB )
   {
      rumboot_printf("Result: 0x%x \n", _res8);
   } 
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
     
   iowrite16(0x6666, (uint32_t)&res16);
   _res16 = ioread16((uint32_t)&res16);
   
   if(_res16 == 0x6666 )
   {
     
      rumboot_printf("Result: 0x%x \n", _res16);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
   
   iowrite32(0x88888888, (uint32_t)&res32);
   _res32 = ioread32((uint32_t)&res32);
   
   if(_res32 == 0x88888888 )
   {
      rumboot_printf("Result: 0x%x \n", _res32);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
   
   iowrite64(0x4444444411111111, (uint32_t)&res64);
   _res64 = ioread64((uint32_t)&res64);
   
   if(_res64 == 0x4444444411111111)
   {
     _r64_output1 =(uint32_t)(_res64 >> 32);
     _r64_output2 =(uint32_t)_res64 & 0xFFFFFFFF;
     rumboot_printf("Result: 0x%x%x\n", _r64_output1, _r64_output2);
  
   }   
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
   
   return 1;
}

int main(void) 
{
    if( check_cpu_ppc_im0_0() && check_cpu_ppc_im0_1())
    {
      return 0;
    }

    return 1;
}