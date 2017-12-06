#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>

#include <devices/sdio.h>
#include <devices/spi.h>
#include <devices/eeprom.h>
#include <devices/gpio.h>

#include <platform/devices.h>

#if 1
static bool spi_init_gpio_mux() {

	/*TO DO!*/
	return true;
}

static void spi_deinit_gpio_mux() {

	/*TO DO!*/
}

static bool eeprom_init_gpio_mux() {

	/*TO DO*/
	return true;
}

static void eeprom_deinit_gpio_mux() {

	/*TO DO!*/
}
#endif

/*
  Order of array elements are very important!
 */
const struct rumboot_bootsource arr[] = {
	{
	    .name = "SDIO",
	    .namelen = 4,
	    .bdatalen = sizeof(struct rumboot_bootsource),
	    .init = sd_init,
	    .deinit = sd_deinit ,
	    .load_img = sd_read,
	    .should_i_try_again = sd_load_failed_should_i_try_again,
	},

	{
			.name = "SPI",
			.namelen = 3,
			.bdatalen = sizeof(struct rumboot_bootsource),
			.init = spi_init,
			.deinit = spi_deinit ,
			.load_img = spi_read,
			.init_gpio_mux = spi_init_gpio_mux,
			.deinit_gpio_mux = spi_deinit_gpio_mux,
			.should_i_try_again = spi_load_failed_should_i_try_again,
	},

	{
			.name = "EEPROM",
			.namelen = 6,
			.bdatalen = sizeof(struct rumboot_bootsource),
			.init = eeprom_init,
			.deinit = eeprom_deinit ,
			.load_img = eeprom_read,
			.init_gpio_mux = eeprom_init_gpio_mux,
			.deinit_gpio_mux = eeprom_deinit_gpio_mux,
			.should_i_try_again = eeprom_load_failed_should_i_try_again,
	},

	{/*Sentinel*/}
};

static void selftest() {
	/*TO DO!*/
};

static void host_mode() {
	/*TO DO!*/
	struct rumboot_bootheader *hdr = NULL;
	void*  dataptr = NULL;
	while(1) {

		hdr = (struct rumboot_bootheader*) &rumboot_platform_spl_start;
		dataptr = (void*) hdr->data;
		if(!rumboot_bootimage_check_header(hdr, &dataptr) && !rumboot_bootimage_check_data(hdr))
			if(rumboot_bootimage_exec(hdr) < 0)
				continue;

		udelay(1);
	}
}

static const  uint32_t read_bootm0() {

	return (ioread32(GPIO0_BASE + GPIO_READ_PADtoAPB) & (1 << 1));
}

static const uint32_t read_bootm1() {

	return (ioread32(GPIO0_BASE + GPIO_READ_PADtoAPB) & (1 << 2));
}

int main()
{
	rumboot_printf("Hello world from IRAM!: %d\n");

	const uint32_t BOOTM0 = read_bootm0();
	const uint32_t BOOTM1 = read_bootm1();

  if(BOOTM0 == 1)
    selftest();

  if(BOOTM1 == 1) {

		//waiting correct image in host mode
		host_mode();
	}
  else {

//		struct bdata b;

		if( !bootsource_try_chain(arr) ) host_mode();
	}

	return 0;
}
