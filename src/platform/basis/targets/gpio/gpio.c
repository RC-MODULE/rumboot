#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <regs/regs_gpio.h>
#include <platform/devices.h>

#define GPIO_OK    0
#define GPIO_ERROR 1




int GPIO(uint8_t send_data, uint8_t direction, uint32_t base_gpio_addr)
{
	unsigned apb_to_pad;
	unsigned pad_to_apb;
	unsigned a;

  
  //init GPIO,write
	iowrite32(0xff, base_gpio_addr + GPIO_SOURCE);//1-pad exchange with gpio
	iowrite32(direction, base_gpio_addr + GPIO_DIRECTION);//1-output,0-input
	iowrite32(send_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);
	apb_to_pad = ioread32(base_gpio_addr + GPIO_WRITE_APBtoPAD);
   if ((apb_to_pad&direction)!=(send_data&direction))
   {rumboot_printf("ERROR_SEND!\n");
      return GPIO_ERROR;}
	pad_to_apb = ioread32(base_gpio_addr + GPIO_READ_PADtoAPB);
  rumboot_printf("pad_to_apb = x%x\n", pad_to_apb); 

	// ("0-3 pins = outputs\n");
  if (direction == 0x0f) 
  {
    a = pad_to_apb >> 4;
    if (apb_to_pad == a) 
    {
      rumboot_printf("true \n");
      return GPIO_OK;
    } else 
    {
      rumboot_printf("error \n");
      return GPIO_ERROR;
    }

  }
  else if (direction == 0xf0) 
  {
  // ("4-7 pins = outputs\n");
    a = pad_to_apb << 4;
    if (apb_to_pad == a) 
    {
        rumboot_printf("true \n");
        return GPIO_OK;
    }
    else 
    {
        rumboot_printf("error \n");
        return GPIO_ERROR;
    }
  }
  else 
    return GPIO_ERROR;
      
}

//
int data_dir(uint32_t base_gpio_addr)
{
	uint8_t data;
	uint8_t dir;

//for dir=0x0f  
    data = 0x00;
    dir = 0x0f;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
    data = 0x02;
    dir = 0x0f;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
    data = 0x04;
    dir = 0x0f;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
    data = 0x08;
    dir = 0x0f;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
    data = 0x0f;
    dir = 0x0f;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
  
   //for dir=0xf0
	 data = 0x00;
	 dir = 0xf0;
	  if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
	 data = 0x10;
	 dir = 0xf0;
	  if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
	 data = 0x20;
	 dir = 0xf0;
	  if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
   data = 0x40;
   dir = 0xf0;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
   data = 0x80;
   dir = 0xf0;
    if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
   data = 0xf0;
   dir = 0xf0;
     if  (GPIO(data, dir, base_gpio_addr)!= GPIO_OK)
    return GPIO_ERROR;
   return 0;
}

int main()
{
	//data_dir(GPIO0_BASE);

    if  (data_dir(GPIO1_BASE)!= GPIO_OK)
    return GPIO_ERROR;
    rumboot_printf("gpio1_ok!\n"); 
      if  (data_dir(GPIO2_BASE)!= GPIO_OK)
    return GPIO_ERROR;
    rumboot_printf("gpio2_ok!\n"); 
      if  (data_dir(GPIO3_BASE)!= GPIO_OK)
    return GPIO_ERROR;
    rumboot_printf("gpio3_ok!\n"); 
        if  (data_dir(GPIO4_BASE)!= GPIO_OK)
    return GPIO_ERROR;
    rumboot_printf("gpio4_ok!\n"); 
           if  (data_dir(GPIO5_BASE)!= GPIO_OK)
    return GPIO_ERROR;
    rumboot_printf("gpio5_ok!\n"); 
       if  (data_dir(GPIO6_BASE)!= GPIO_OK)
    return GPIO_ERROR;
rumboot_printf("TEST_OK!\n"); 
	return 0;
}

