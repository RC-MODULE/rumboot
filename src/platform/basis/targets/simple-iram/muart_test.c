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
#include <rumboot/rumboot.h>

#include <regs/regs_muart.h>

struct base_addrs {
	uint32_t	base1;
	uint32_t	base2;
};

static int mem_cmp_odd_numbs(char * src, char * dest, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++) {
		if( *src != *dest )
		{
			rumboot_printf("src byte: %x, dest byte: %x\n", *src, *dest);
			return -1;
		}
		else {
			src ++;
			dest += 2;
		}
	}

	return 0;
}

static bool test_cfg(struct base_addrs *addrs, const struct muart_conf *cfg)
{
	size_t data_size = 8;
	volatile char *buf = rumboot_malloc_from_heap_aligned(0, data_size, 8);

	memset((char *) buf, 0x55, data_size);

	rumboot_printf("init\n");
	muart_init(addrs->base1, cfg);
	muart_init(addrs->base2, cfg);

	rumboot_printf("muart enable\n");
	muart_enable(addrs->base1);
	muart_enable(addrs->base2);

	if (!cfg->dma_en) {
		if (muart_transmit_data_throught_apb(addrs->base1, addrs->base2, (char *) buf, data_size) < 0) {
			rumboot_free((char *) buf);
			return false;
		}
	} else {
		volatile char *output = rumboot_malloc_from_heap_aligned(0, data_size, 8);

		memset((char *) &output[0], 0x0, data_size);

		if (muart_transmit_data_throught_mdma(addrs->base1, addrs->base2, &output[0], &buf[0], data_size) < 0) {
			//What should i do here? If a want to rumboot_free memory?
			return false;
		}

		rumboot_printf("Compare arrays.\n");
		if (mem_cmp_odd_numbs((char *) &buf[0], (char *) &output[0], data_size) != 0) {
			//the same
			size_t j;
			for(j=0; j<data_size; j++)
			{
				rumboot_printf("%x ", output[j]);
			}
			rumboot_printf("\n");
			rumboot_printf("failed!\n");
			return false;
		}

		rumboot_free((char *) output);
	}

	rumboot_printf("OK!\n");
	rumboot_free((char *) buf);
	return true;
}

static const struct muart_conf cfg = {
	.wlen			= WL_8,
	.stp2			= STP2,
	.is_even		= true,
	.is_parity_available	= true,
	.mode			= RS_422,
	.is_loopback		= false,
	.baud_rate		= 12500000,
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
//static const struct base_addrs addrs10 = { .base1 = UART1_BASE, .base2 = UART0_BASE };

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "MUART test")
//TEST_ENTRY("rs232 01", muart_rs232_test, (uint32_t)&addrs01),
// TEST_ENTRY("rs232 10", muart_rs232_test, (uint32_t)&addrs10),
TEST_ENTRY("dma 01", muart_mdma_test, (uint32_t)&addrs01),
//TEST_ENTRY("dma 10", muart_mdma_test, (uint32_t)&addrs10),
// TEST_ENTRY("rs485 01", muart_rs485_test, (uint32_t)&addrs01),
// TEST_ENTRY("rs485 10", muart_rs485_test, (uint32_t)&addrs10),
TEST_SUITE_END();

// static void handler_uart1(int irq, void *arg)
// {
// 	uint32_t *done = arg;
//
// 	*done = ioread32(UART1_BASE + MUART_STATUS);
// 	rumboot_printf("IRQ arrived, arg %x\n", *((uint32_t *)arg));
// 	rumboot_printf("done\n");
// }

uint32_t main()
{
	volatile uint32_t done = 0;
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

	uint32_t base = UART1_BASE;
	rumboot_irq_set_handler(tbl, UART1_INTR, 0, mdma_irq_handler, (void *)&base);
	rumboot_irq_enable(UART1_INTR);
	rumboot_irq_table_activate(tbl);

	int ret = test_suite_run(NULL, &muart_test);

	asm volatile ("swi #77");

	rumboot_printf("%d tests from suite failed\n", ret);

	rumboot_printf("muart status: %x\n", done);

	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	return ret;
}
