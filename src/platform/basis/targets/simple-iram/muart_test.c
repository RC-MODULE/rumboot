#include <string.h>
#include <stdlib.h>

#include <devices/muart.h>
#include <devices/mdma.h>

#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/io.h>


struct base_addrs {
	uint32_t	base1;
	uint32_t	base2;
};

static bool transmit_data_throught_apb(struct base_addrs *addrs)
{
	char etalon_ch = 'x';
	int buf_size = 1;

	rumboot_printf("write char\n");
	int count = buf_size;
	while (count--)
		muart_write_char(addrs->base1, etalon_ch);

	rumboot_printf("read char\n");
	count = buf_size;
	char read_ch = '\0';
	while (count--) {
		read_ch = muart_read_char(addrs->base2);
		rumboot_printf("read char: %c\n", read_ch);

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
	uint32_t mdma_base = MDMA0_BASE;
	uint32_t count = 16;
	int i;
	uint8_t rx_data[count];
	uint8_t tx_data[count];
	uint32_t *rx_tbl_addr = malloc(16); //For 1 descriptor!
	uint32_t *tx_tbl_addr = malloc(16);

	for (i = 0; i < count; i++)
		rx_data[i] = 0x55;

	for (i = 0; i < count; i++)
		tx_data[i] = 0x0;

	struct settings set = { .ownership	= 0,.link = 0,
				.interrupt	= 0,.stop = 1,.increment = 0 };

	struct desc_cfg rx_desc = {
		.set	= &set,.data_addr = (uint32_t)&rx_data,
	};

	struct desc_cfg tx_desc = {
		.set	= &set,.data_addr = (uint32_t)&tx_data,
	};

	struct table_cfg rx_tbl_cfg = {
		.type	= NORMAL,.desc_gap = 0,.table_addr = (uint32_t)rx_tbl_addr
	};

	struct table_cfg tx_tbl_cfg = {
		.type	= NORMAL,.desc_gap = 0,.table_addr = (uint32_t)tx_tbl_addr
	};

	mdma_init(mdma_base);
	mdma_set_rxtable(mdma_base, &rx_tbl_cfg);
	mdma_set_wxtable(mdma_base, &tx_tbl_cfg);

	if (mdma_set_desc((uint32_t)rx_tbl_addr, &rx_desc) != OK)
		return false;

	if (mdma_set_desc((uint32_t)tx_tbl_addr, &tx_desc) != OK)
		return false;

	//if(!mdma_wait_r(mdma_base, INT_DESC_DONE) || !mdma_wait_w(mdma_base, INT_DESC_DONE))
		//return false;

	if (mdma_get_desc((uint32_t)tx_tbl_addr, &tx_desc) != OK)
		return false;

	if (memcmp(tx_data, rx_data, count) != 0)
		return false;

	return true;
}

static bool test_cfg(struct base_addrs *addrs, const struct muart_conf *cfg)
{
	rumboot_printf("init\n");
	muart_init(addrs->base1, cfg);
	if (addrs->base1 != addrs->base2) muart_init(addrs->base2, cfg);

	rumboot_printf("muart enable\n");
	muart_enable(addrs->base1);
	if (addrs->base1 != addrs->base2) muart_enable(addrs->base2);

	if (cfg->dma_en)
		transmit_data_throught_dma(addrs);
	else
		transmit_data_throught_apb(addrs);

	rumboot_printf("OK!\n");
	return true;
}

static const struct muart_conf cfg = {
	.wlen			= WL_8,
	.stp2			= STP2,
	.is_even		= true,
	.is_parity_available	= true,
	.mode			= RS_232,
	.cts_en			= false,
	.rts_en			= false,
	.is_loopback		= false,
	.baud_rate		= 921600,
	.dma_en = false
};

static bool muart_loopback_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.is_loopback = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

bool muart_cts_rts_en_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.cts_en = true;
	muart.rts_en = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

bool muart_mdma_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.dma_en = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

static const struct base_addrs loopback_addrs0 = { .base1 = UART0_BASE, .base2 = UART0_BASE };
static const struct base_addrs loopback_addrs1 = { .base1 = UART1_BASE, .base2 = UART1_BASE };
static const struct base_addrs addrs01 = { .base1 = UART0_BASE, .base2 = UART1_BASE };
static const struct base_addrs addrs10 = { .base1 = UART1_BASE, .base2 = UART0_BASE };

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "MUART test")
TEST_ENTRY("cts/rts enable 01", muart_cts_rts_en_test, (uint32_t)&addrs01),
TEST_ENTRY("cts/rts enable 10", muart_cts_rts_en_test, (uint32_t)&addrs10),
TEST_ENTRY("dma 01", muart_mdma_test, (uint32_t)&addrs01),
TEST_ENTRY("dma 10", muart_mdma_test, (uint32_t)&addrs10),
TEST_ENTRY("UART_0 loopback", muart_loopback_test, (uint32_t)&loopback_addrs0),
TEST_ENTRY("UART_1 loopback", muart_loopback_test, (uint32_t)&loopback_addrs1),
TEST_SUITE_END();

uint32_t main()
{
	int ret = test_suite_run(NULL, &muart_test);

	rumboot_printf("%d tests from suite failed\n", ret);
	return ret;
}
