#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <rumboot/testsuite.h>
#include <platform/devices.h>
#include <regs/regs_global_timers.h>
#include <regs/regs_mgeth.h>
#include <devices/gp_timers.h>
#include <devices/mdma_chan_api.h>
#include <devices/mgeth.h>

#define XFER_SIZE 64

uint32_t BASE[] = {ETH0_BASE, ETH1_BASE, ETH2_BASE, ETH3_BASE};

static bool test_mgeth_frame_xfer(uint32_t eth_send_num)
{
	uint32_t eth_recv_num, ts_o_low, ts_o_high, ts_i_low, ts_i_high;
	volatile uint8_t *frame_o, *frame_i;
	struct mdma_chan *chan_o, *chan_i;
	struct mdma_trans *trans_o, *trans_i;
	struct mdma_group *group_o, *group_i;
	struct mdma_node *node_o, *node_i;
	void *desc_o, *desc_i;
	int desc_size, i, ret;

	rumboot_printf("--------------------------------------------------------------------------------\n");

	if (!(eth_send_num % 2))
		eth_recv_num = eth_send_num + 1;
	else
		eth_recv_num = eth_send_num - 1;

	rumboot_printf("eth_send_num = %u\n", eth_send_num);
	rumboot_printf("eth_recv_num = %u\n", eth_recv_num);

	frame_o = rumboot_malloc_from_heap_aligned(0, XFER_SIZE, 4);
	if (!frame_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		return false;
	}

	frame_i = rumboot_malloc_from_heap_aligned(0, XFER_SIZE, 4);
	if (!frame_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
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

	for (i = 34; i < XFER_SIZE; i++)
		frame_o[i] = i - 34;

	for (i = 0; i < XFER_SIZE; i++)
		frame_i[i] = 0x0;

	chan_i = mgeth_receive(BASE[eth_recv_num], (void *)frame_i, XFER_SIZE, false);
	if (!chan_i)
	{
		rumboot_printf("ERROR: Failed receive frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	chan_o = mgeth_transmit(BASE[eth_send_num], (void *)frame_o, XFER_SIZE, false);
	if (!chan_o)
	{
		rumboot_printf("ERROR: Failed transmit frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	rumboot_printf("Waiting of sending of output packet...\n");

	ret = mgeth_wait_transfer_complete(chan_o, -1);
	if (ret)
	{
		rumboot_printf("ERROR: Failed transmit frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	rumboot_printf("Output packet sended.\n");

	trans_o = mdma_chan_get_trans(chan_o);
	group_o = mdma_trans_get_group(trans_o);
	node_o = mdma_group_get_node(group_o);
	desc_o = mdma_node_get_desc(node_o, &desc_size);

	if (desc_size != 16)
	{
		rumboot_printf("ERROR: Wrong output descriptor size (%d)!\n", desc_size);
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	ts_o_low = mdma_desc_user_data(desc_o, true);
	ts_o_high = mdma_desc_user_data(desc_o, false);

	ret = mgeth_finish_transfer(chan_o);
	if (ret)
	{
		rumboot_printf("ERROR: Failed finish transfer!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	rumboot_printf("Waiting of receiving of input packet...\n");

	ret = mgeth_wait_transfer_complete(chan_i, -1);
	if (ret)
	{
		rumboot_printf("ERROR: Failed receive frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	rumboot_printf("Input packet received.\n");

	trans_i = mdma_chan_get_trans(chan_i);
	group_i = mdma_trans_get_group(trans_i);
	node_i = mdma_group_get_node(group_i);
	desc_i = mdma_node_get_desc(node_i, &desc_size);

	if (desc_size != 16)
	{
		rumboot_printf("ERROR: Wrong input descriptor size (%d)!\n", desc_size);
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	ts_i_low = mdma_desc_user_data(desc_i, true);
	ts_i_high = mdma_desc_user_data(desc_i, false);

	ret = mgeth_finish_transfer(chan_i);
	if (ret)
	{
		rumboot_printf("ERROR: Failed finish transfer!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return false;
	}

	rumboot_printf("frame_o:\n");
	for (i = 0; i < XFER_SIZE; i++)
		rumboot_printf("%X ", frame_o[i]);
	rumboot_printf("\n");

	rumboot_printf("frame_i:\n");
	for (i = 0; i < XFER_SIZE; i++)
		rumboot_printf("%X ", frame_i[i]);
	rumboot_printf("\n");

	for (i = 0; i < XFER_SIZE; i++)
		if ((i != 24) && (i != 25) && (frame_i[i] != frame_o[i]))
		{
			rumboot_printf("ERROR: Wrong byte %d!\n", i);
			rumboot_free((void *)frame_o);
			rumboot_free((void *)frame_i);
			return false;
		}

	rumboot_free((void *)frame_o);
	rumboot_free((void *)frame_i);

	rumboot_printf("Output timestamp: 0x%X_%X\n", ts_o_high, ts_o_low);
	rumboot_printf("Input timestamp: 0x%X_%X\n", ts_i_high, ts_i_low);

	if ((!ts_o_low) && (!ts_o_high))
	{
		rumboot_printf("ERROR: No timestamps!\n");
		return false;
	}

	if (ts_i_high < ts_o_high)
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		return false;
	}

	if ((ts_i_high == ts_o_high) && (ts_i_low < ts_o_low))
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		return false;
	}

	return true;
}

// Declare the testsuite structure
TEST_SUITE_BEGIN(mgeth_frame_xfer_test, "Frame transfer")
TEST_ENTRY("From ETH0 to ETH1", test_mgeth_frame_xfer, 0),
TEST_ENTRY("From ETH1 to ETH0", test_mgeth_frame_xfer, 1),
TEST_ENTRY("From ETH2 to ETH3", test_mgeth_frame_xfer, 2),
TEST_ENTRY("From ETH3 to ETH2", test_mgeth_frame_xfer, 3),
TEST_SUITE_END();

int main()
{
	struct mgeth_conf cfg;
	int res;

	rumboot_printf("================================================================================\n");

	rumboot_printf("Waiting SGMII initialization...\n");

	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE))
	{
		rumboot_printf("ERROR: SGMII initialization ERROR!\n");
		return 1;
	}

	rumboot_printf("SGMII initialized.\n");

	// Start timer
	gp_timer_turn_on();

	mgeth_reset(ETH0_BASE);
	mgeth_reset(ETH1_BASE);
	mgeth_reset(ETH2_BASE);
	mgeth_reset(ETH3_BASE);

	cfg.is_full_duplex = true;
	cfg.speed = SPEED_1000;

	mgeth_init(ETH0_BASE, &cfg);
	mgeth_init(ETH1_BASE, &cfg);
	mgeth_init(ETH2_BASE, &cfg);
	mgeth_init(ETH3_BASE, &cfg);

	res = test_suite_run(NULL, &mgeth_frame_xfer_test);

	if (res)
		rumboot_printf("%d tests from suite failed!\n", res);

	return res;
}
