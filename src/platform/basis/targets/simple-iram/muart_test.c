#include <string.h>
#include <stdlib.h>

#include <devices/muart.h>
#include <devices/mdma.h>

#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/io.h>

//#define TEST_LOOPBACK

struct base_addrs {
	uint32_t	base1;
	uint32_t	base2;
};

static bool transmit_data_throught_apb(struct base_addrs *addrs)
{
	char etalon_ch = 'x';
	int buf_size = 512;

	rumboot_printf("write char\n");
	int count = buf_size;
	while (count--)
		muart_write_char(addrs->base1, etalon_ch);

	rumboot_printf("read char\n");
	count = buf_size;
	char read_ch = '\0';
	while (count--) {
		read_ch = muart_read_char(addrs->base2);

		if (read_ch == etalon_ch) {
			continue;
		} else {
			rumboot_printf("failed\n");
			rumboot_printf("read char: %c\n", read_ch);
			return false;
		}
	}

	return true;
}

static bool transmit_data_throught_dma(struct base_addrs *addrs)
{
	//CONFIG DMA
	rumboot_printf("Config DMA.\n");
	uint32_t mdma_base = MDMA0_BASE;
	struct mdma_config cfg = { .desc_type = NORMAL, .desc_gap = 0, .num_descriptors = 1 };
	struct mdma_device *mdma = mdma_create(mdma_base, &cfg);
	mdma_set(mdma,&cfg);

	//CREATE DATA
	rumboot_printf("Create data.\n");
	size_t count = 32;
	void *dest = malloc(count);
	void *src = malloc(count);

	memset(src, 0x55, count);
	memset(dest, 0x0, count);

	rumboot_printf("Create transaction.\n");
	struct mdma_transaction *t = mmdma_transaction_create(mdma, dest, src, count);

	rumboot_printf("Queue transaction.\n");
	mdma_transaction_queue(t);

	rumboot_printf("Init MDMA.\n");
	mdma_init(mdma_base);

	rumboot_printf("Get transaction state.\n");
	while (!mdma_transaction_is_finished(t)) ;

	rumboot_printf("Compare arrays.\n");
	if (memcmp(src, dest, count) != 0)
		goto free_memory;
	else
		return true;

rumboot_printf("Free memory.\n");
free_memory:
	free(dest);
	free(src);
	return false;
}

static bool test_cfg(struct base_addrs *addrs, const struct muart_conf *cfg)
{
	rumboot_printf("init\n");
	muart_init(addrs->base1, cfg);
	if (addrs->base1 != addrs->base2) muart_init(addrs->base2, cfg);

	rumboot_printf("muart enable\n");
	muart_enable(addrs->base1);
	if (addrs->base1 != addrs->base2) muart_enable(addrs->base2);

	if (cfg->dma_en) {
		if (!transmit_data_throught_dma(addrs))
			return false;
	} else {
		if (!transmit_data_throught_apb(addrs))
			return false;
	}

	rumboot_printf("OK!\n");
	return true;
}

static const struct muart_conf cfg = {
	.wlen			= WL_8,
	.stp2			= STP2,
	.is_even		= true,
	.is_parity_available	= true,
	.mode			= RS_422,
	.is_loopback		= false,
	.baud_rate		= 921600,
	.dma_en			= false
};

#ifdef TEST_LOOPBACK
static bool muart_loopback_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.is_loopback = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}
#endif

bool muart_rs232_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.mode = RS_232;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

bool muart_rs485_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.mode = RS_485;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

bool muart_mdma_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.dma_en = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

#ifdef TEST_LOOPBACK
static const struct base_addrs loopback_addrs0 = { .base1 = UART0_BASE, .base2 = UART0_BASE };
static const struct base_addrs loopback_addrs1 = { .base1 = UART1_BASE, .base2 = UART1_BASE };
#endif

static const struct base_addrs addrs01 = { .base1 = UART0_BASE, .base2 = UART1_BASE };
static const struct base_addrs addrs10 = { .base1 = UART1_BASE, .base2 = UART0_BASE };

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "MUART test")
TEST_ENTRY("dma 01", muart_mdma_test, (uint32_t)&addrs01),
TEST_ENTRY("dma 10", muart_mdma_test, (uint32_t)&addrs10),
#ifdef TEST_LOOPBACK
TEST_ENTRY("rs485 01", muart_rs485_test, (uint32_t)&addrs01),
TEST_ENTRY("rs485 10", muart_rs485_test, (uint32_t)&addrs10),
TEST_ENTRY("rs232 01", muart_rs232_test, (uint32_t)&addrs01),
TEST_ENTRY("rs232 10", muart_rs232_test, (uint32_t)&addrs10),
TEST_ENTRY("UART_0 loopback", muart_loopback_test, (uint32_t)&loopback_addrs0),
TEST_ENTRY("UART_1 loopback", muart_loopback_test, (uint32_t)&loopback_addrs1),
#endif
TEST_SUITE_END();

uint32_t main()
{
	int ret = test_suite_run(NULL, &muart_test);

	rumboot_printf("%d tests from suite failed\n", ret);
	return ret;
}
