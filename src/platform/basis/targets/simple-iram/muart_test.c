#include <regs/regs_muart.h>
#include <devices/muart.h>
#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <rumboot/io.h>

bool muart_loopback_test(uint32_t base)
{
	struct muart_conf cfg = {
		.wlen			= WL_8,
		.stp2			= STP2,
		.is_even		= true,
		.is_parity_available	= true,
		.mode			= RS_232,
    .cts_en = true,
    .rts_en = true,
		.is_loopback		= true,
		.baud_rate		= 921600
	};

	char etalon_ch = 'x';
  int buf_size = 1;

	rumboot_printf("init\n");
	muart_init(base, &cfg);
	rumboot_printf("muart enable\n");
	muart_enable(base);

	rumboot_printf("write char\n");
	int count = buf_size;
	while (count--)
		muart_write_char(base, etalon_ch);

	rumboot_printf("FIFO STATE: %x\n", ioread32(base + 0x10));

	rumboot_printf("read char\n");
	count = buf_size;
	char read_ch;
	while (count--) {
		read_ch = muart_read_char(base);
		if (read_ch == etalon_ch) {
			continue;
		} else {
			rumboot_printf("failed\n");
			rumboot_printf("read char: %x\n", read_ch);
			return false;
		}
	}

	rumboot_printf("OK!\n");
  return true;
}

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(muart_test, "Loopback ")
TEST_ENTRY("UART_0", muart_loopback_test, UART0_BASE),
TEST_ENTRY("UART_1", muart_loopback_test, UART1_BASE),
TEST_SUITE_END();


uint32_t main()
{
	int ret = test_suite_run(NULL, &muart_test);

	rumboot_printf("%d tests from suite failed\n", ret);
	return ret;
}
