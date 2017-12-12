#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>

#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/eeprom.h>
#include <devices/gpio.h>

#include <platform/devices.h>

#if 1
static bool spiflash_init_gpio_mux() {

	/*TO DO!*/
	return true;
}

static void spiflash_deinit_gpio_mux() {

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

#define SDIO_CLK_FREQ 100000
#define SPI_CLK_FREQ 100000
#define EEPROM_CLK_FREQ 100000
/*
  Order of array elements are very important!
 */
const struct rumboot_bootsource arr[] = {
	{
	    .name = "SDIO",
			.base = SDIO0_BASE,
			.freq_khz = SDIO_CLK_FREQ,
	    .privdatalen = 128,
	    .init = sd_init,
	    .deinit = sd_deinit ,
	    .load_img = sd_read,
			.init_gpio_mux = spiflash_init_gpio_mux,
			.deinit_gpio_mux = spiflash_deinit_gpio_mux,
	    .are_load_attempts = sd_are_load_attempts,
	},

	{
			.name = "SPI",
			.base = 0,
			.freq_khz = SPI_CLK_FREQ,
			.privdatalen = 128,
			.init = spiflash_init,
			.deinit = spiflash_deinit ,
			.load_img = spiflash_read,
			.init_gpio_mux = spiflash_init_gpio_mux,
			.deinit_gpio_mux = spiflash_deinit_gpio_mux,
			.are_load_attempts = spiflash_are_load_attempts,
	},

	{
			.name = "EEPROM",
			.base = 0,
			.freq_khz = EEPROM_CLK_FREQ,
			.privdatalen = 128,
			.init = eeprom_init,
			.deinit = eeprom_deinit ,
			.load_img = eeprom_read,
			.init_gpio_mux = eeprom_init_gpio_mux,
			.deinit_gpio_mux = eeprom_deinit_gpio_mux,
			.are_load_attempts = eeprom_are_load_attempts,
	},

	{/*Sentinel*/}
};

static void selftest() {
	/*TO DO!*/
}

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

		if( !bootsource_try_chain(arr) ) host_mode();
	}

	return 0;
}
