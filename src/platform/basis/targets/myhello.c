#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <basis/defs.h>

int main()
{
    uint32_t temp_data;  // All the variables should be declared in stack yet
  
    rumboot_printf("Hello World!\n");  // Built-in RumBoot Function which prints 
    
      // The same as rumboot_platform_raise_event(EVENT_PERF,0xDA)
      // Just for example how to use iowrite32
    iowrite32(0x000000DA, GPIO1_BASE + GPIO_DATA);
    iowrite32(0x00000000, GPIO0_BASE + GPIO_DATA);
    iowrite32(0x00000009, GPIO0_BASE + GPIO_DATA);
      //
    temp_data = ioread32(GPIO1_BASE + GPIO_DATA);
    
    rumboot_printf("GPIO1=0x%x\n",temp_data);
    
    if(temp_data == 0x000000DA)
      return 0;
    else
      return 1;
}
