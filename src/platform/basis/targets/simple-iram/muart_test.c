#include <string.h>
#include <stdlib.h>

#include <devices/muart.h>
#include <devices/mdma.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <regs/regs_muart.h>

struct base_addrs {
	uint32_t	base1;
	uint32_t	base2;
};

static bool test_cfg(struct base_addrs *addrs, const struct muart_conf *cfg)
{
	size_t data_size = 4;
	char *buf = malloc(data_size);

	memset(buf, 0x55, data_size);

	rumboot_printf("init\n");
	muart_init(addrs->base1, cfg);
	muart_init(addrs->base2, cfg);

	rumboot_printf("muart enable\n");
	muart_enable(addrs->base1);
	muart_enable(addrs->base2);

	if (!cfg->dma_en) {
		if (!muart_transmit_data_throught_apb(addrs->base1, addrs->base2, buf, data_size)) {
			free(buf);
			return false;
		}
	} else {
		char *output = malloc(data_size);
		memset(output, 0x0, data_size);

		if (!mdma_transmit_data(MDMA0_BASE, output, buf, data_size)) {
			free(buf);
			free(output);
		}
		free(output);
	}

	rumboot_printf("OK!\n");
	free(buf);
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


static const struct base_addrs addrs01 = { .base1 = UART0_BASE, .base2 = UART1_BASE };
static const struct base_addrs addrs10 = { .base1 = UART1_BASE, .base2 = UART0_BASE };

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "MUART test")
//TEST_ENTRY("rs232 01", muart_rs232_test, (uint32_t)&addrs01),
// TEST_ENTRY("rs232 10", muart_rs232_test, (uint32_t)&addrs10),
TEST_ENTRY("dma 01", muart_mdma_test, (uint32_t)&addrs01),
TEST_ENTRY("dma 10", muart_mdma_test, (uint32_t)&addrs10),
// TEST_ENTRY("rs485 01", muart_rs485_test, (uint32_t)&addrs01),
// TEST_ENTRY("rs485 10", muart_rs485_test, (uint32_t)&addrs10),
TEST_SUITE_END();

void handler_uart0(int irq, void *arg)
{
	uint32_t *done = arg;

	*done = ioread32(UART0_BASE + MUART_STATUS);
	rumboot_printf("IRQ arrived, arg %x\n", *((uint32_t *)arg));
	rumboot_printf("done\n");
}

uint32_t main()
{
	volatile uint32_t done = 0;
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

	rumboot_irq_set_handler(tbl, UART0_INTR, 0, handler_uart0, (void *)&done);
	rumboot_irq_enable(UART0_INTR);
	rumboot_irq_table_activate(tbl);

	int ret = test_suite_run(NULL, &muart_test);

	asm volatile ("swi #74");

	rumboot_printf("%d tests from suite failed\n", ret);

	rumboot_printf("muart status: %x\n", done);

	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	return ret;
}
