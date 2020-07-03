
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>


uint32_t data;
uint32_t addr=0x004A0000;

int main()
{
  iowrite64(0xAAAA5555AAAA5555l,addr);
  data = ioread32(addr);
  if(data != 0xAAAA5555)
	 return 1;
  rumboot_platform_request_file("myfile", addr);
  data = ioread32(addr);
  rumboot_printf("My data is %x\n",data);
  rumboot_platform_dump_region("mydump", addr, 32);
  return 0;
}
