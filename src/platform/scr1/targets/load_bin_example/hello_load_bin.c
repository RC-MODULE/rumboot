
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>


uint32_t data;

volatile uint32_t* dump_ptr;
int dump_size;

int main()
{
  dump_size = 8;
  dump_ptr = rumboot_malloc_from_heap(1,dump_size*4 /*size in bytes*/);

  dump_ptr[0] = 0xAAAA5555;
  if(dump_ptr[0] != 0xAAAA5555)
	 return 1;

  rumboot_platform_request_file_ex("myfile", dump_ptr,dump_size*4);
  data = dump_ptr[0];
  rumboot_printf("My data is %x\n",data);
  rumboot_platform_dump_region("mydump", (uint32_t) dump_ptr, dump_size*4);
  return 0;
}
