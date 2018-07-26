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
#include <regs/regs_global_timers.h>
#include <regs/regs_mgeth.h>
#include <devices/gp_timers.h>
#include <devices/mgeth.h>

uint32_t BASE[] = {ETH0_BASE, ETH1_BASE, ETH2_BASE, ETH3_BASE};

static bool test_mgeth_frame_xfer(uint32_t eth_send_num)
{
	uint32_t eth_recv_num;
	volatile uint8_t *frame_o, *frame_i;
	volatile uint32_t *desc_o, *desc_i;
	uint32_t tmp32;
	int i;

	rumboot_printf("--------------------------------------------------------------------------------\n");

	if (!(eth_send_num % 2))
		eth_recv_num = eth_send_num + 1;
	else
		eth_recv_num = eth_send_num - 1;

	rumboot_printf("eth_send_num = %u\n", eth_send_num);
	rumboot_printf("eth_recv_num = %u\n", eth_recv_num);

	frame_o = rumboot_malloc_from_heap_aligned(0, 64, 4);
	frame_i = rumboot_malloc_from_heap_aligned(0, 64, 4);
	desc_o = rumboot_malloc_from_heap_aligned(0, 16, 4);
	desc_i = rumboot_malloc_from_heap_aligned(0, 16, 4);

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
	iowrite32((uint32_t)desc_i, BASE[eth_recv_num] + MGETH_RECV_CH_0 + DESC_ADDR_x);

	iowrite32(0x00100012, BASE[eth_send_num] + MGETH_SEND_CH_0 + SETTINGS_x); // desc_kind = 10b (1 - 0 bits); add_info = 1 (4 bit); desc_gap = 0x10 (31 - 16 bits)
	iowrite32((uint32_t)desc_o, BASE[eth_send_num] + MGETH_SEND_CH_0 + DESC_ADDR_x);

	iowrite32(0x1, BASE[eth_recv_num] + MGETH_RECV_CH_0 + ENABLE_x); // enable = 1 (0 bit)
	iowrite32(0x1, BASE[eth_send_num] + MGETH_SEND_CH_0 + ENABLE_x); // enable = 1 (0 bit)

	rumboot_printf("Waiting of sending of output packet...\n");

	tmp32 = 0x1;
	while (tmp32 & 0x1)
	{
		tmp32 = ioread32(BASE[eth_send_num] + MGETH_SEND_CH_0 + ENABLE_x);

		rumboot_printf("ENABLE_R = 0x%X\n", tmp32);
	}

	rumboot_printf("Output packet sended.\n");
	rumboot_printf("Waiting of receiving of input packet...\n");

	tmp32 = 0x1;
	while (tmp32 & 0x1)
	{
		tmp32 = ioread32(BASE[eth_recv_num] + MGETH_RECV_CH_0 + ENABLE_x);

		rumboot_printf("ENABLE_W = 0x%X\n", tmp32);
	}

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

	mgeth_init_sgmii(SGMII_PHY, SCTL_BASE);

	// Start timer
	gp_timer_turn_on();

	mgeth_reset(ETH0_BASE);
	mgeth_reset(ETH1_BASE);
	mgeth_reset(ETH2_BASE);
	mgeth_reset(ETH3_BASE);

	res = test_suite_run(NULL, &mgeth_frame_xfer_test);

	if (res)
		rumboot_printf("%d tests from suite failed!\n", res);

	return res;
}
