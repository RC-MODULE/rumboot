
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

extern char rumboot_im0_heap_start;
extern char rumboot_im0_heap_end;
//extern char rumboot_platform_stack_area_end;
extern char rumboot_im1_heap_start;
extern char rumboot_im1_heap_end;
extern char rumboot_im2_heap_start;
extern char rumboot_im2_heap_end;



int make_test(uint32_t min, uint32_t max)
{
  uint32_t ptr;
  
  rumboot_printf("Write min=%x, max=%x\n",min,max);
  for(ptr = min; ptr < max; ptr = ptr + 4)
  {
    iowrite32(ptr,ptr);
  }
  
  rumboot_printf("Read min=%x, max=%x\n",min,max);
  for(ptr = min; ptr < max; ptr = ptr + 4)
  {
    if(ioread32(ptr) != ptr)
	return -1;
  }
  
  rumboot_printf("Write INV min=%x, max=%x\n",min,max);
  for(ptr = min; ptr < max; ptr = ptr + 4)
  {
    iowrite32(~ptr,ptr);
  }
  
  rumboot_printf("Read INV min=%x, max=%x\n",min,max);
  for(ptr = min; ptr < max; ptr = ptr + 4)
  {
    if(ioread32(ptr) != ~ptr)
	return -1;
  }
  
  return 0;
}

#define CHUNK_SIZE 1024

int main()
{
    uint32_t min;
    uint32_t max;
    
    min = 
      (
	((uint32_t) &rumboot_im0_heap_start) & ~(CHUNK_SIZE-1)
      ) + 
      CHUNK_SIZE; // Align It By CHUNK_SIZE
    
    max = min + CHUNK_SIZE;
    
    if(make_test(min,max)!=0)
      return 1;
    
    max = 
      (
	((uint32_t) &rumboot_im0_heap_end) & ~(CHUNK_SIZE-1)
      ) -
      CHUNK_SIZE;
      
    min = max - CHUNK_SIZE;
    
    if(make_test(min,max)!=0)
      return 2;
    
//    rumboot_printf("rumboot_platform_stack_area_end=%x\n",(uint32_t) &rumboot_platform_stack_area_end);
    
//     max = 
//       (
// 	((uint32_t) &rumboot_platform_stack_area_end) & ~(CHUNK_SIZE-1)
//       ) -
//       1024 - // Allready Occupied Stack
//       CHUNK_SIZE;
//     
//     min = max - CHUNK_SIZE;
//     
//     if(make_test(min,max)!=0)
//       return 3;
    
    
    min = (uint32_t) &rumboot_im1_heap_start; // Hope It Is Aligned
    max = min + CHUNK_SIZE;
    if(make_test(min,max)!=0)
      return 4;
    
    max = (uint32_t) &rumboot_im1_heap_end;
    min = max - CHUNK_SIZE;
    if(make_test(min,max)!=0)
      return 5;
    
    min = (uint32_t) &rumboot_im2_heap_start; // Hope It Is Aligned
    max = min + CHUNK_SIZE;
    if(make_test(min,max)!=0)
      return 6;
    
    max = (uint32_t) &rumboot_im2_heap_end;
    min = max - CHUNK_SIZE;
    if(make_test(min,max)!=0)
      return 7;
    
    return 0;
}
