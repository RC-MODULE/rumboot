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
	const uint32_t mdma_r_base = MDMA0_BASE;
	const uint32_t mdma_w_base = MDMA0_BASE;
	const uint32_t data_count = 4 * 1024;
	uint8_t *to_r_data = malloc(data_count);
	uint8_t *to_w_data = malloc(data_count);
	uint32_t desc_num = 1;                       //the same variable for rx and tx table
	uint32_t *rx_tbl_addr = malloc(NORMAL_DESC_SIZE * desc_num);
	uint32_t *tx_tbl_addr = malloc(NORMAL_DESC_SIZE * desc_num);

	memset(to_r_data, 0x55, data_count);
	memset(to_w_data, 0x0, data_count);

	struct settings set = { .ownership	= 0,.link = 0,
				.interrupt	= 1,.stop = 1, .increment = 0, .length = data_count };

	struct desc_cfg rx_desc = {
		.set	= &set,.data_addr = (uint32_t)to_r_data, .free_run_val = (int)NULL
	};

	struct desc_cfg tx_desc = {
		.set	= &set,.data_addr = (uint32_t)to_w_data, .free_run_val = (int)NULL
	};

	struct table_cfg rx_tbl_cfg = {
		.type	= NORMAL,.desc_gap = 0, .table_addr = (uint32_t)rx_tbl_addr
	};

	struct table_cfg tx_tbl_cfg = {
		.type	= NORMAL,.desc_gap = 0, .table_addr = (uint32_t)tx_tbl_addr
	};

	rumboot_printf("Set RX table.\n");
	mdma_set_rxtable(mdma_r_base, &rx_tbl_cfg);

	rumboot_printf("Set WX table.\n");
	mdma_set_wxtable(mdma_w_base, &tx_tbl_cfg);

	if (mdma_set_desc((uint32_t)rx_tbl_addr, &rx_desc) != OK)
		goto free_memory;

	if (mdma_set_desc((uint32_t)tx_tbl_addr, &tx_desc) != OK)
		goto free_memory;

	rumboot_printf("Init mdma1.\n");
	mdma_init(mdma_w_base);

	if (mdma_r_base != mdma_w_base) {
		rumboot_printf("Init mdma2.\n");
		mdma_init(mdma_r_base);
	}

	if (mdma_get_desc((uint32_t)tx_tbl_addr, &tx_desc) != OK)
		goto free_memory;

	rumboot_printf("Compare arrays.\n");
	if (bcmp(to_w_data, to_r_data, data_count) != 0)
		goto free_memory;

	return true;

free_memory:
	free(to_r_data);
	free(to_w_data);
	free(rx_tbl_addr);
	free(tx_tbl_addr);
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
	.mode			= RS_232,
	.rts_cts_en		= false,
	.is_loopback		= false,
	.baud_rate		= 921600,
	.dma_en			= false
};

#if 0
static bool muart_loopback_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.is_loopback = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}
#endif

bool muart_cts_rts_en_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.rts_cts_en = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

bool muart_mdma_test(uint32_t to_addrs)
{
	struct muart_conf muart = cfg;

	muart.dma_en = true;

	return test_cfg((struct base_addrs *)to_addrs, &muart);
}

// static const struct base_addrs loopback_addrs0 = { .base1 = UART0_BASE, .base2 = UART0_BASE };
// static const struct base_addrs loopback_addrs1 = { .base1 = UART1_BASE, .base2 = UART1_BASE };
static const struct base_addrs addrs01 = { .base1 = UART0_BASE, .base2 = UART1_BASE };
//static const struct base_addrs addrs10 = { .base1 = UART1_BASE, .base2 = UART0_BASE };

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "MUART test")
// TEST_ENTRY("cts/rts enable 01", muart_cts_rts_en_test, (uint32_t)&addrs01),
// TEST_ENTRY("cts/rts enable 10", muart_cts_rts_en_test, (uint32_t)&addrs10),
TEST_ENTRY("dma 01", muart_mdma_test, (uint32_t)&addrs01),
//TEST_ENTRY("dma 10", muart_mdma_test, (uint32_t)&addrs10),
// TEST_ENTRY("UART_0 loopback", muart_loopback_test, (uint32_t)&loopback_addrs0),
// TEST_ENTRY("UART_1 loopback", muart_loopback_test, (uint32_t)&loopback_addrs1),
TEST_SUITE_END();

uint32_t main()
{
	int ret = test_suite_run(NULL, &muart_test);

	rumboot_printf("%d tests from suite failed\n", ret);
	return ret;
}
