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

	iowrite32(0xff, base_gpio_addr + GPIO_SOURCE);
	iowrite32(direction, base_gpio_addr + GPIO_DIRECTION);
	iowrite32(send_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);
	apb_to_pad = ioread32(base_gpio_addr + GPIO_WRITE_APBtoPAD);
	pad_to_apb = ioread32(base_gpio_addr + GPIO_READ_PADtoAPB);
#ifdef PRINT
	rumboot_printf("pad_to_apb = x%x\n", pad_to_apb);
#endif
	// ("0-3 pins = outputs\n");

	if (direction == 0x0f) {
		a = pad_to_apb >> 4;
		if (apb_to_pad == a) {
			rumboot_printf("true \n");
			return GPIO_OK;
		} else {
			rumboot_printf("error \n");
			return GPIO_ERROR;
		}
	} else if (direction == 0xf0) {
		// ("4-7 pins = outputs\n");
		a = pad_to_apb << 4;
		if (apb_to_pad == a) {

      rumboot_printf("true \n");
      return GPIO_OK;
    }
    else
      return GPIO_ERROR;
	}

    rumboot_printf("error \n");
		return GPIO_ERROR;

}


int data_dir(uint32_t base_gpio_addr)
{
	//uint8_t data;
	//uint8_t dir;
#if 0
	data = 0x00;
	dir = 0x0f;
	GPIO(data, dir, base_gpio_addr);
	data = 0x03;
	dir = 0x0f;
	GPIO(data, dir, base_gpio_addr);
	data = 0x0f;
	dir = 0x0f;
	GPIO(data, dir, base_gpio_addr);
	data = 0x00;
	dir = 0xf0;
	GPIO(data, dir, base_gpio_addr);
	data = 0x30;
	dir = 0xf0;
	GPIO(data, dir, base_gpio_addr);
	data = 0xf0;
	dir = 0xf0;
	GPIO(data, dir, base_gpio_addr);
#endif
	return 0;
}

int main()
{
	data_dir(GPIO0_BASE);
	data_dir(GPIO1_BASE);
	data_dir(GPIO2_BASE);
	data_dir(GPIO3_BASE);
	data_dir(GPIO4_BASE);

	return 0;
}
