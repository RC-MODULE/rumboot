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

#include <regs/regs_mdio.h>

#include <rumboot/timer.h>

#define XFER_SIZE 64


uint32_t BASE[] = {ETH0_BASE, ETH1_BASE, ETH2_BASE, ETH3_BASE};

int mgeth_frame_xfer(uint32_t eth_send_num, uint32_t eth_recv_num, const char *heap_name)
{
	volatile uint8_t *frame_o, *frame_i;
	struct mdma_chan *chan_o, *chan_i;
	int heap_id, i, ret;

	rumboot_printf("Checking access to memory: '%s'.\n", heap_name);

	heap_id = rumboot_malloc_heap_by_name(heap_name);
	if (heap_id < 0)
	{
		rumboot_printf("ERROR: Memory not exist!\n");
		return 1;
	}

	rumboot_printf("heap_id = %d\n", heap_id);

	frame_o = rumboot_malloc_from_heap_aligned(heap_id, XFER_SIZE, 4);
	if (!frame_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		return 1;
	}

	frame_i = rumboot_malloc_from_heap_aligned(heap_id, XFER_SIZE, 4);
	if (!frame_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		return 1;
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
		return 1;
	}

	chan_o = mgeth_transmit(BASE[eth_send_num], (void *)frame_o, XFER_SIZE, false);
	if (!chan_o)
	{
		rumboot_printf("ERROR: Failed transmit frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	rumboot_printf("Waiting of sending of output packet...\n");

	ret = mgeth_wait_transfer_complete(chan_o, -1);
	if (ret)
	{
		rumboot_printf("ERROR: Failed transmit frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	rumboot_printf("Output packet sended.\n");

	ret = mgeth_finish_transfer(chan_o);
	if (ret)
	{
		rumboot_printf("ERROR: Failed finish transfer!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	rumboot_printf("Waiting of receiving of input packet...\n");

	ret = mgeth_wait_transfer_complete(chan_i, -1);
	if (ret)
	{
		rumboot_printf("ERROR: Failed receive frame!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	rumboot_printf("Input packet received.\n");

	ret = mgeth_finish_transfer(chan_i);
	if (ret)
	{
		rumboot_printf("ERROR: Failed finish transfer!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	rumboot_printf("Checking frame...\n");

	for (i = 0; i < XFER_SIZE; i++)
		if ((i != 24) && (i != 25) && (frame_i[i] != frame_o[i]))
		{
			rumboot_printf("ERROR: Wrong byte %d!\n", i);
			rumboot_free((void *)frame_o);
			rumboot_free((void *)frame_i);
			return 1;
		}

	rumboot_free((void *)frame_o);
	rumboot_free((void *)frame_i);

	rumboot_printf("Done.\n");

	return 0;
}

static bool test_mgeth_frame_xfer(uint32_t eth_send_num)
{
	uint32_t eth_recv_num;
	int ret;

	rumboot_printf("--------------------------------------------------------------------------------\n");

	if (!(eth_send_num % 2))
		eth_recv_num = eth_send_num + 1;
	else
		eth_recv_num = eth_send_num - 1;

	rumboot_printf("eth_send_num = %u\n", eth_send_num);
	rumboot_printf("eth_recv_num = %u\n", eth_recv_num);

//------------------------------------------------------------------------------

	ret = mgeth_frame_xfer(eth_send_num, eth_recv_num, "IM0");
	if (ret)
	{
		rumboot_printf("ERROR: Transfer frame ERROR!\n");
		return false;
	}

	ret = mgeth_frame_xfer(eth_send_num, eth_recv_num, "IM1");
	if (ret)
	{
		rumboot_printf("ERROR: Transfer frame ERROR!\n");
		return false;
	}

//------------------------------------------------------------------------------

	return true;
}

// Declare the testsuite structure
TEST_SUITE_BEGIN(mgeth_mem_access_test, "Frame transfer")
TEST_ENTRY("From ETH0 to ETH1", test_mgeth_frame_xfer, 0),
TEST_ENTRY("From ETH1 to ETH0", test_mgeth_frame_xfer, 1),
TEST_ENTRY("From ETH2 to ETH3", test_mgeth_frame_xfer, 2),
TEST_ENTRY("From ETH3 to ETH2", test_mgeth_frame_xfer, 3),
TEST_SUITE_END();

int main()
{
	struct mgeth_conf cfg;
	int res,i;

	rumboot_printf("================================================================================\n");

	rumboot_printf("Waiting SGMII initialization...\n");

    #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
        init_gpio_and_en_eth_phy(MGPIO0_BASE, MDIO0_BASE, AN_EN);
    #endif
	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE, AN_EN))
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
// 1000
	cfg.speed = SPEED_1000;

	mgeth_init(ETH0_BASE, &cfg);
	mgeth_init(ETH1_BASE, &cfg);
	mgeth_init(ETH2_BASE, &cfg);
	mgeth_init(ETH3_BASE, &cfg);

	res = test_suite_run(NULL, &mgeth_mem_access_test);

	if (res)
    {
		rumboot_printf("%d tests from suite failed!\n", res);
		return 1;
    }
    
// 100
	cfg.speed = SPEED_100;

    #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
        for (i = 0; i < 4; i++){            
            rumboot_printf("MDIO%x: setting SPEED_100 \n",i);
            mdio_write (MDIO0_BASE + 0x1000*i,  0x0, 0x2100);
            mdio_read_data (MDIO0_BASE + 0x1000*i,  0x0);
        }
    
        rumboot_printf("delay 1s\n");
        mdelay(1000);
    #endif    

	mgeth_init(ETH0_BASE, &cfg);
	mgeth_init(ETH1_BASE, &cfg);
	mgeth_init(ETH2_BASE, &cfg);
	mgeth_init(ETH3_BASE, &cfg);

	res = test_suite_run(NULL, &mgeth_mem_access_test);

	if (res)
    {
		rumboot_printf("%d tests from suite failed!\n", res);
		return 1;
    }
    
// 10
/*	cfg.speed = SPEED_10;

	mgeth_init(ETH0_BASE, &cfg);
	mgeth_init(ETH1_BASE, &cfg);
	mgeth_init(ETH2_BASE, &cfg);
	mgeth_init(ETH3_BASE, &cfg);

	res = test_suite_run(NULL, &mgeth_mem_access_test);

	if (res)
    {
		rumboot_printf("%d tests from suite failed!\n", res);
		return 1;
    }
*/    
	return res;
}
