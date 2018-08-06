#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <rumboot/testsuite.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_global_timers.h>
#include <regs/regs_mgeth.h>
#include <devices/gp_timers.h>
#include <devices/mgeth.h>

uint32_t BASE[] = {ETH0_BASE, ETH1_BASE, ETH2_BASE, ETH3_BASE};
uint32_t INTR[] = {MGETH0_IRQ, MGETH1_IRQ, MGETH2_IRQ, MGETH3_IRQ};
struct rumboot_irq_entry *tbl;
volatile int int_f_send = 0, int_f_recv = 0;

static void isr_send(int irq, void *arg)
{
	rumboot_printf("isr_send start; irq = %d; arg = 0x%X.\n", irq, arg);

	rumboot_printf("GLOBAL_STATUS (ETH TX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_GLOBAL_STATUS));
	rumboot_printf("MGETH_STATUS (ETH TX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_STATUS));
	rumboot_printf("STATUS_R (ETH TX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_SEND_CH_0 + STATUS_x));

	int_f_send = 1;

	rumboot_printf("isr_send end.\n");
}

static void isr_recv(int irq, void *arg)
{
	rumboot_printf("isr_recv start; irq = %d; arg = 0x%X.\n", irq, arg);

	rumboot_printf("GLOBAL_STATUS (ETH RX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_GLOBAL_STATUS));
	rumboot_printf("MGETH_STATUS (ETH RX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_STATUS));
	rumboot_printf("STATUS_W (ETH RX) = 0x%X\n", ioread32((uint32_t)arg + MGETH_RECV_CH_0 + STATUS_x));

	int_f_recv = 1;

	rumboot_printf("isr_recv end.\n");
}

static bool test_mgeth_frame_xfer(uint32_t eth_send_num)
{
	uint32_t eth_recv_num;
	volatile uint8_t *frame_o, *frame_i;
	volatile uint32_t *desc_o, *desc_i;
	int i;

	rumboot_printf("--------------------------------------------------------------------------------\n");

	if (!(eth_send_num % 2))
		eth_recv_num = eth_send_num + 1;
	else
		eth_recv_num = eth_send_num - 1;

	rumboot_printf("eth_send_num = %u\n", eth_send_num);
	rumboot_printf("eth_recv_num = %u\n", eth_recv_num);

	int_f_send = 0;
	int_f_recv = 0;

	while (int_f_send || int_f_recv);

	rumboot_irq_set_handler(tbl, INTR[eth_send_num], 0x0, isr_send, (void *)BASE[eth_send_num]);
	rumboot_irq_set_handler(tbl, INTR[eth_recv_num], 0x0, isr_recv, (void *)BASE[eth_recv_num]);
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(INTR[eth_send_num]);
	rumboot_irq_enable(INTR[eth_recv_num]);
	rumboot_irq_sei();

	frame_o = rumboot_malloc_from_heap_aligned(0, 64, 4);
	if (!frame_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		return false;
	}

	frame_i = rumboot_malloc_from_heap_aligned(0, 64, 4);
	if (!frame_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		return false;
	}

	desc_o = rumboot_malloc_from_heap_aligned(0, 16, 4);
	if (!desc_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	desc_i = rumboot_malloc_from_heap_aligned(0, 16, 4);
	if (!desc_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		rumboot_free((void *)desc_o);
		return false;
	}

	for (i = 0; i < 12; i++)
		frame_o[i] = 0xFF;

	frame_o[12] = 0x08;
	frame_o[13] = 0x0;

	frame_o[14] = 0x45;
	frame_o[15] = 0x0;
	frame_o[16] = 0;
	frame_o[17] = 50;
	frame_o[18] = 0x0;
	frame_o[19] = 0x0;
	frame_o[20] = 0x0;
	frame_o[21] = 0x0;
	frame_o[22] = 64;
	frame_o[23] = 0x0;
	frame_o[24] = 0x0;
	frame_o[25] = 0x0;
	frame_o[26] = 0xFF;
	frame_o[27] = 0xFF;
	frame_o[28] = 0xFF;
	frame_o[29] = 0xFF;
	frame_o[30] = 0xFF;
	frame_o[31] = 0xFF;
	frame_o[32] = 0xFF;
	frame_o[33] = 0xFF;

	for (i = 34; i < 64; i++)
		frame_o[i] = i - 34;

	for (i = 0; i < 64; i++)
		frame_i[i] = 0x0;

	for (i = 0; i < 4; i++)
	{
		desc_o[i] = 0x0;
		desc_i[i] = 0x0;
	}

	desc_o[2] = (uint32_t)frame_o;
	desc_o[3] = 0x12000040;

	desc_i[2] = (uint32_t)frame_i;
	desc_i[3] = 0x10000040;

	iowrite32(0x00100012, BASE[eth_recv_num] + MGETH_RECV_CH_0 + SETTINGS_x); // desc_kind = 10b (1 - 0 bits); add_info = 1 (4 bit); desc_gap = 0x10 (31 - 16 bits)
	iowrite32(0x10, BASE[eth_recv_num] + MGETH_RECV_CH_0 + IRQ_MASK_x); // stop_desc = 1 (4 bit)
	iowrite32((uint32_t)desc_i, BASE[eth_recv_num] + MGETH_RECV_CH_0 + DESC_ADDR_x);

	iowrite32(0x00100012, BASE[eth_send_num] + MGETH_SEND_CH_0 + SETTINGS_x); // desc_kind = 10b (1 - 0 bits); add_info = 1 (4 bit); desc_gap = 0x10 (31 - 16 bits)
	iowrite32(0x10, BASE[eth_send_num] + MGETH_SEND_CH_0 + IRQ_MASK_x); // stop_desc = 1 (4 bit)
	iowrite32((uint32_t)desc_o, BASE[eth_send_num] + MGETH_SEND_CH_0 + DESC_ADDR_x);

	iowrite32(0x1, BASE[eth_recv_num] + MGETH_RECV_CH_0 + ENABLE_x); // enable = 1 (0 bit)
	iowrite32(0x1, BASE[eth_send_num] + MGETH_SEND_CH_0 + ENABLE_x); // enable = 1 (0 bit)

	rumboot_printf("Waiting of sending of output packet...\n");

	while (!int_f_send);

	rumboot_printf("Output packet sended.\n");
	rumboot_printf("Waiting of receiving of input packet...\n");

	while (!int_f_recv);

	rumboot_printf("Input packet received.\n");

	rumboot_printf("desc_o:\n");
	for (i = 0; i < 4; i++)
		rumboot_printf("%X ", desc_o[i]);
	rumboot_printf("\n");

	rumboot_printf("desc_i:\n");
	for (i = 0; i < 4; i++)
		rumboot_printf("%X ", desc_i[i]);
	rumboot_printf("\n");

	rumboot_printf("frame_o:\n");
	for (i = 0; i < 64; i++)
		rumboot_printf("%X ", frame_o[i]);
	rumboot_printf("\n");

	rumboot_printf("frame_i:\n");
	for (i = 0; i < 64; i++)
		rumboot_printf("%X ", frame_i[i]);
	rumboot_printf("\n");

	for (i = 0; i < 64; i++)
		if ((i != 24) && (i != 25) && (frame_i[i] != frame_o[i]))
		{
			rumboot_printf("ERROR: Wrong byte %d!\n", i);
			rumboot_free((void *)frame_o);
			rumboot_free((void *)frame_i);
			rumboot_free((void *)desc_o);
			rumboot_free((void *)desc_i);
			return false;
		}

	rumboot_free((void *)frame_o);
	rumboot_free((void *)frame_i);

	if ((!desc_o[0]) && (!desc_o[1]))
	{
		rumboot_printf("ERROR: No timestamps!\n");
		rumboot_free((void *)desc_o);
		rumboot_free((void *)desc_i);
		return false;
	}

	if (desc_i[1] < desc_o[1])
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		rumboot_free((void *)desc_o);
		rumboot_free((void *)desc_i);
		return false;
	}

	if ((desc_i[1] == desc_o[1]) && (desc_i[0] < desc_o[0]))
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		rumboot_free((void *)desc_o);
		rumboot_free((void *)desc_i);
		return false;
	}

	rumboot_free((void *)desc_o);
	rumboot_free((void *)desc_i);

	return true;
}

// Declare the testsuite structure
TEST_SUITE_BEGIN(mgeth_frame_xfer_independent_test, "Frame transfer")
TEST_ENTRY("From ETH0 to ETH1", test_mgeth_frame_xfer, 0),
TEST_ENTRY("From ETH1 to ETH0", test_mgeth_frame_xfer, 1),
TEST_ENTRY("From ETH2 to ETH3", test_mgeth_frame_xfer, 2),
TEST_ENTRY("From ETH3 to ETH2", test_mgeth_frame_xfer, 3),
TEST_SUITE_END();

int main()
{
	int res;

	rumboot_printf("================================================================================\n");

	rumboot_printf("Waiting SGMII initialization...\n");

	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE))
	{
		rumboot_printf("ERROR: SGMII initialization ERROR!\n");
		return 1;
	}

	rumboot_printf("SGMII initialized.\n");

	tbl = rumboot_irq_create(NULL);

	// Start timer
	gp_timer_turn_on();

	mgeth_reset(ETH0_BASE);
	mgeth_reset(ETH1_BASE);
	mgeth_reset(ETH2_BASE);
	mgeth_reset(ETH3_BASE);

	res = test_suite_run(NULL, &mgeth_frame_xfer_independent_test);

	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	if (res)
		rumboot_printf("%d tests from suite failed!\n", res);

	return res;
}
