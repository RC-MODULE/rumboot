#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

static uint8_t res8 = 0xDD;
static uint16_t res16 = 0x5454;
static uint32_t res32 = 0x75757575;
static uint64_t res64 = 0x5555555522222222;

static int check_cpu_ppc_im0_0()
{
   register uint8_t temp8;  
   register uint16_t temp16;
   register uint32_t temp32, temp64_output1, temp64_output2;
   register uint64_t temp64;

   temp8 = ioread8((uint32_t)&res8);
   if(temp8 == 0xDD )
   {
      rumboot_printf("Result: 0x%x \n", temp8);
   } 
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
     
   temp16 = ioread16((uint32_t)&res16);
   
   if(temp16 == 0x5454 )
   {
      rumboot_printf("Result: 0x%x \n", temp16);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
  
   temp32 = ioread32((uint32_t)&res32);
   
   if(temp32 == 0x75757575 )
   {
      rumboot_printf("Result: 0x%x \n", temp32);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_0 is failed!\n");
      return 0;
   }
   
   temp64 = ioread64((uint32_t)&res64);
   
   if(temp64 == 0x5555555522222222)
   {
      temp64_output1 = (uint32_t)(temp64 >> 32);
      temp64_output2 = (uint32_t)temp64 & 0xFFFFFFFF;
      rumboot_printf("Result: 0x%x%x\n", temp64_output1, temp64_output2);
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
   register uint8_t temp8;  
   register uint16_t temp16;
   register uint32_t temp32, temp64_output1, temp64_output2;
   register uint64_t temp64;
   
   iowrite8(0xBB, (uint32_t)&res8);
   temp8 = ioread8((uint32_t)&res8);
   
   if(temp8 == 0xBB )
   {
      rumboot_printf("Result: 0x%x \n", temp8);
   } 
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
     
   iowrite16(0x6666, (uint32_t)&res16);
   temp16 = ioread16((uint32_t)&res16);
   
   if(temp16 == 0x6666 )
   {
     
      rumboot_printf("Result: 0x%x \n", temp16);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
   
   iowrite32(0x88888888, (uint32_t)&res32);
   temp32 = ioread32((uint32_t)&res32);
   
   if(temp32 == 0x88888888 )
   {
      rumboot_printf("Result: 0x%x \n", temp32);
   }
   else
   {
      rumboot_printf("check_cpu_ppc_im0_1 is failed!\n");
      return 0;
   }
   
   iowrite64(0x4444444411111111, (uint32_t)&res64);
   temp64 = ioread64((uint32_t)&res64);
   
   if(temp64 == 0x4444444411111111)
   {
     temp64_output1 =(uint32_t)(temp64 >> 32);
     temp64_output2 =(uint32_t)temp64 & 0xFFFFFFFF;
     rumboot_printf("Result: 0x%x%x\n", temp64_output1, temp64_output2);
  
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