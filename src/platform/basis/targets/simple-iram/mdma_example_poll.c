#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_mdma.h>

#include <devices/mdma_chan_api.h>

#define MEM_SEGMENT_SZ          16

int main()
{
	struct mdma_chan *chan_rd;
	struct mdma_chan *chan_wr;
	struct mdma_cfg cfg;
	void *dst_addr, *src_addr;
	int i, state, ret = 0;

	cfg.mode = MDMA_CHAN_POLLING;
	cfg.desc_kind = NORMAL_DESC;
	cfg.heap_id = 1;
	cfg.burst_width = MDMA_BURST_WIDTH8;
	cfg.event_indx = -1;
	cfg.add_info = false;
	cfg.addr_inc = true;

	iowrite32(1, MDMA0_BASE + MDMA_GP_SOFT_RST);

	while (ioread32(MDMA0_BASE + MDMA_GP_SOFT_RST) & 1) {};
	
	chan_rd = mdma_chan_create((void *)(MDMA0_BASE + MDMA_CHANNEL_R), &cfg);
	if (!chan_rd) {
		ret = -1;
		goto test_exit_1;
	}

	chan_wr = mdma_chan_create((void *)(MDMA0_BASE + MDMA_CHANNEL_W), &cfg);
	if (!chan_wr) {
		ret = -2;
		goto test_exit_2;
	}

	if (mdma_trans_create(chan_rd, MDMA_TRANS_QUEUE)) {
		ret = -3;
		goto test_exit_2;
	}

	if (mdma_trans_create(chan_wr, MDMA_TRANS_QUEUE)) {
		ret = -4;
		goto test_exit_2;
	}

	dst_addr = rumboot_malloc(MEM_SEGMENT_SZ);
	if (!dst_addr) {
		ret = -5;
		goto test_exit_2;
	}

	for (i = 0; i < MEM_SEGMENT_SZ; i++)
		*((char *)(dst_addr + i)) = 0;

	src_addr = rumboot_malloc(MEM_SEGMENT_SZ);
	if (!src_addr) {
		ret = -6;
		goto test_exit_3;
	}

	for (i = 0; i < MEM_SEGMENT_SZ; i++)
		*((char *)(src_addr + i)) = i;

	if (mdma_trans_add_group(chan_rd, src_addr, MEM_SEGMENT_SZ, 0, true) ||
		mdma_trans_add_group(chan_wr, dst_addr, MEM_SEGMENT_SZ, 0, true)) {
		ret = -7;
		goto test_exit_4;
	}

	if (mdma_chan_config(chan_rd) || mdma_chan_config(chan_wr)) {
		ret = -8;
		goto test_exit_4;
	}

	if (mdma_chan_start(chan_wr) || mdma_chan_start(chan_rd)) {
		ret = -9;
		goto test_exit_5;
	}

	if (mdma_chan_wait_trans_end(chan_wr, 1000)) {
		ret = -10;
		goto test_exit_5;
	}

	state = mdma_chan_get_state(chan_wr);
	if (state != MDMA_CHAN_DUMMY) {
		ret = -11;
		goto test_exit_5;
	}

	state = mdma_chan_get_state(chan_rd);
	if (state != MDMA_CHAN_DUMMY) {
		ret = -12;
		goto test_exit_5;
	}

	for (i = 0; i < 16; i++) {
		if (*((char *)(dst_addr + i)) != *((char *)(src_addr + i))) {
			ret = -13;
			break;
		}
	}

test_exit_5:
	mdma_chan_reset(chan_rd);
	mdma_chan_reset(chan_wr);

test_exit_4:
	rumboot_free(src_addr);

test_exit_3:
	rumboot_free(dst_addr);

test_exit_2:
	mdma_chan_destroy(chan_wr);

test_exit_1:
	mdma_chan_destroy(chan_rd);
	
	return ret;
}
