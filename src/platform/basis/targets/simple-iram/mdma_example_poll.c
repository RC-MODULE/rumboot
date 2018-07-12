#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_mdma.h>

#include <devices/mdma_chan_api.h>

#define MEM_SEGMENT_SZ          15
#define MEM_RING_SIZE		5
#define MEM_SEGMENT_NUM		(2 * MEM_RING_SIZE)

int main()
{
	struct mdma_cfg cfg;

	struct mdma_chan *chan_rd;
	struct mdma_chan *chan_wr;

	struct mdma_trans *trans;
	struct mdma_group *group;
	struct mdma_node *node;

	void *dst[MEM_SEGMENT_NUM], *src[MEM_SEGMENT_NUM];
	void *dst_addr, *src_addr;
	int i, j, buf_size;
	int tmp, ret = 0;

	cfg.mode = MDMA_CHAN_POLLING;
	cfg.desc_kind = LONG_DESC;
	cfg.heap_id = 0;
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

	mdma_chan_attach(chan_wr, chan_rd);

	if (mdma_trans_create(chan_rd, MDMA_TRANS_RING) ||
		mdma_trans_create(chan_wr, MDMA_TRANS_QUEUE)) {
		ret = -3;
		goto test_exit_2;
	}

	buf_size = MEM_SEGMENT_NUM *
		((MEM_SEGMENT_SZ / MDMA_BURST_WIDTH8) * MDMA_BURST_WIDTH8 + MDMA_BURST_WIDTH8);

	dst_addr = rumboot_malloc_from_heap(0, buf_size);
	if (!dst_addr) {
		ret = -4;
		goto test_exit_2;
	}

	for (i = 0; i < buf_size; i++)
		*((char *)(dst_addr + i)) = 0;

	src_addr = rumboot_malloc_from_heap(1, buf_size);
	if (!src_addr) {
		ret = -5;
		goto test_exit_3;
	}

	for (i = 0; i < buf_size; i++)
		*((char *)(src_addr + i)) = i | 0x01;


	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		dst[i] = dst_addr + i * (buf_size / MEM_SEGMENT_NUM);
		src[i] = src_addr + i * (buf_size / MEM_SEGMENT_NUM);

		if (mdma_trans_add_group(chan_wr, dst[i], MEM_SEGMENT_SZ, 0, (i < (MEM_SEGMENT_NUM - 1)) ? false : true)) {
			ret = -6;
			goto test_exit_4;
		}
	}

	for (i = 0; i < MEM_RING_SIZE; i++) {
		if(mdma_trans_add_group(chan_rd, src[i], MEM_SEGMENT_SZ, 0, (i < (MEM_RING_SIZE - 1)) ? false : true)) {
			ret = -7;
			goto test_exit_4;
		}
	}

	if (mdma_chan_config(chan_wr) || mdma_chan_config(chan_rd)) {
		ret = -8;
		goto test_exit_4;
	}

	if (mdma_chan_start(chan_wr) || mdma_chan_start(chan_rd)) {
		ret = -9;
		goto test_exit_5;
	}

	for (i = MEM_RING_SIZE; i < MEM_SEGMENT_NUM; i++) {
		tmp = mdma_chan_get_state(chan_wr);
		if (tmp != MDMA_CHAN_RUNNING)
			break;

		if (mdma_chan_wait_group_end(chan_rd, 100)) {
			ret = -10;
			goto test_exit_5;
		}

		tmp = mdma_chan_get_state(chan_rd);
		if (tmp == MDMA_CHAN_ERROR) {
			ret = -11;
			goto test_exit_5;
		}

		trans = mdma_chan_get_trans(chan_rd);
		group = mdma_trans_get_group(trans);
		node = mdma_group_get_node(group);

		mdma_node_rebase(node, &chan_rd->cfg, src[i], MEM_SEGMENT_SZ);
		mdma_trans_rest_group(group);

		tmp = mdma_chan_get_state(chan_rd);
		if (tmp == MDMA_CHAN_PENDING)
			mdma_chan_resume(chan_rd);
	}

	mdma_chan_wait_trans_end(chan_rd, 100);

	tmp = mdma_chan_get_state(chan_rd);
	if (tmp != MDMA_CHAN_PENDING) {
		ret = -12;
		goto test_exit_5;
	}

	mdma_chan_wait_trans_end(chan_wr, 100);

	tmp = mdma_chan_get_state(chan_wr);
	if (tmp != MDMA_CHAN_DUMMY) {
		ret = -13;
		goto test_exit_5;
	}

	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		tmp = buf_size / MEM_SEGMENT_NUM;

		for (j = 0; j < tmp; j++) {
#if 1
			if ((j < MEM_SEGMENT_SZ) &&
				(*((char *)(dst_addr + j + i * tmp)) != *((char *)(src_addr + j + i * tmp)))) {
				ret = -14;
				break;
			}

			if (!(j <  MEM_SEGMENT_SZ) &&
				(*((char *)(dst_addr + j + i * tmp)) == *((char *)(src_addr + j + i * tmp)))) {
				ret = -15;
				break;
			}
#else
			rumboot_printf("src: 0x%x <-> dst: 0x%x\n",
					*((char *)(src_addr + j + i * tmp)), *((char *)(dst_addr + j + i * tmp)));
#endif
		}
	}

test_exit_5:
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
