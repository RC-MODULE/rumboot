#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <rumboot/timer.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_mdma.h>

#include <devices/mdma_chan_api.h>

#define MEM_SEGMENT_NUM         3
#define MEM_SEGMENT_SZ          32
#define MEM_PITCH_WIDTH         16
#define MEM_STRING_LENGTH       8

struct mdma_gp_dev {
	void *base_addr;
	struct mdma_cfg cfg_rd;
	struct mdma_cfg cfg_wr;

	struct rumboot_irq_entry *tbl;
	int irq;

	struct mdma_chan *chan_rd;
	struct mdma_chan *chan_wr;

	void *dst_addr[2][MEM_SEGMENT_NUM];
	void *src_addr[2][MEM_SEGMENT_NUM];

	int cnt_rd;
	int cnt_wr;

	
};

static void mdma_gp_handler(int irq, void *arg)
{
	struct mdma_gp_dev *dev = (struct mdma_gp_dev *)arg;
	struct mdma_chan *chan;
	struct mdma_trans *trans;
	struct mdma_group *group;
	struct mdma_node *node;
	int flags, state, ret;

	flags = ioread32((unsigned long)dev->base_addr + MDMA_GP_STATUS);

	rumboot_printf("GP_STATUS: 0x%x\n", flags);

	while (flags) {
		void *addr;
		int *cnt;
		bool is_wr;

		if (flags & (1 << 16)) {
			chan = dev->chan_wr;
			addr = dev->dst_addr[1][dev->cnt_wr];
			cnt = &dev->cnt_wr;

			is_wr = true;

			flags &= ~(1 << 16);
		}
		else if (flags & (1 << 0)) {
			chan = dev->chan_rd;
			addr = dev->src_addr[1][dev->cnt_rd];
			cnt = &dev->cnt_rd;

			is_wr = false;

			flags &= ~(1 << 0);
		}
		else
			break;

		ret = mdma_chan_irq_cb(chan);
		state = chan->state;

		if (ret == IRQ_CHAN_NONE)
			continue;

		if (ret & IRQ_CHAN_ERROR)
			continue;

		if ((state == MDMA_CHAN_PENDING) || (ret & IRQ_CHAN_GROUP)) {
			if (!(*cnt < MEM_SEGMENT_NUM))
				continue;

			trans = mdma_chan_get_trans(chan);
			group = mdma_trans_get_group(trans);
			node = mdma_group_get_node(group);

			mdma_node_rebase(node, &chan->cfg, addr, MEM_SEGMENT_SZ);
			mdma_trans_rest_group(group);

			*cnt += 1;

			if (is_wr)
				rumboot_printf("CNT_WR: 0x%x\n", *cnt);
			else
				rumboot_printf("CNT_RD: 0x%x\n", *cnt);

			if (state == MDMA_CHAN_PENDING)
				mdma_chan_resume(chan);
		}
	}

	return;
}

int main()
{
	struct mdma_gp_dev dev;
	void *dst_addr, *src_addr;
	unsigned int timeout = 100;
	int i, j, ret = 0;

	rumboot_irq_cli();

	dev.base_addr = (void *)MDMA0_BASE;

	iowrite32(1, (unsigned long)dev.base_addr + MDMA_GP_SOFT_RST);
	while (ioread32((unsigned long)dev.base_addr + MDMA_GP_SOFT_RST) & 1) {};

	dev.cfg_wr.mode = MDMA_CHAN_INTERRUPT;
	dev.cfg_wr.desc_kind = PITCH_DESC;
	dev.cfg_wr.heap_id = 1;
	dev.cfg_wr.burst_width = MDMA_BURST_WIDTH8;
	dev.cfg_wr.pitch_width = MEM_PITCH_WIDTH;
	dev.cfg_wr.str_length = MEM_STRING_LENGTH;
	dev.cfg_wr.event_indx = -1;
	dev.cfg_wr.add_info = false;
	dev.cfg_wr.addr_inc = true;

	dev.cfg_rd.mode = MDMA_CHAN_INTERRUPT;
	dev.cfg_rd.desc_kind = PITCH_DESC;
	dev.cfg_rd.heap_id = 1;
	dev.cfg_rd.burst_width = MDMA_BURST_WIDTH8;
	dev.cfg_rd.pitch_width = MEM_PITCH_WIDTH;
	dev.cfg_rd.str_length = MEM_STRING_LENGTH;
	dev.cfg_rd.event_indx = -1;
	dev.cfg_rd.add_info = false;
	dev.cfg_rd.addr_inc = true;

	dev.cnt_rd = 0;
	dev.cnt_wr = 0;
	
	dev.chan_rd = mdma_chan_create(dev.base_addr + MDMA_CHANNEL_R, &dev.cfg_rd);
	if (dev.chan_rd) {
		ret = mdma_trans_create(dev.chan_rd, MDMA_TRANS_RING);

		if (ret) {
			ret = -1;
			goto test_exit_1;
		}
	}

	dev.chan_wr = mdma_chan_create(dev.base_addr + MDMA_CHANNEL_W, &dev.cfg_wr);
	if (dev.chan_wr) {
		ret = mdma_trans_create(dev.chan_wr, MDMA_TRANS_RING);

		if (ret) {
			ret = -2;
			goto test_exit_2;
		}
	}

	if (!dev.chan_rd || !dev.chan_wr) {
		ret = -3;
		goto test_exit_2;
	}

	mdma_chan_attach(dev.chan_wr, dev.chan_rd);

	dst_addr = rumboot_malloc(4 * MEM_SEGMENT_NUM * MEM_SEGMENT_SZ);
	if (!dst_addr) {
		ret = -4;
		goto test_exit_2;
	}

	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		dev.dst_addr[0][i] = dst_addr + 2 * i * MEM_SEGMENT_SZ;
		dev.dst_addr[1][i] = dst_addr + 2 * MEM_SEGMENT_NUM * MEM_SEGMENT_SZ +
				2 * i * MEM_SEGMENT_SZ;

		for (j = 0; j < (2 * MEM_SEGMENT_SZ); j++) {
			*((char *)(dev.dst_addr[0][i] + j)) = 0;
			*((char *)(dev.dst_addr[1][i] + j)) = 0;
		}

		if (mdma_trans_add_group(dev.chan_wr, dev.dst_addr[0][i], MEM_SEGMENT_SZ, 0,
				(i < (MEM_SEGMENT_NUM - 1)) ? false : true)) {
			ret = -5;
			goto test_exit_3;
		}
	}

	src_addr = rumboot_malloc(4 * MEM_SEGMENT_NUM * MEM_SEGMENT_SZ);
	if (!src_addr) {
		ret = -6;
		goto test_exit_3;
	}

	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		dev.src_addr[0][i] = src_addr + 2 * i * MEM_SEGMENT_SZ;
		dev.src_addr[1][i] = src_addr + 2 * MEM_SEGMENT_NUM * MEM_SEGMENT_SZ +
				2 * i * MEM_SEGMENT_SZ;

		for (j = 0; j < (2 * MEM_SEGMENT_SZ); j++) {
			*((char *)(dev.src_addr[0][i] + j)) = ((i + j) & 0xAA) | 0x2;
			*((char *)(dev.src_addr[1][i] + j)) = ((i + j) & 0x55) | 0x1;
		}

		if (mdma_trans_add_group(dev.chan_rd, dev.src_addr[0][i], MEM_SEGMENT_SZ, 0,
				(i < (MEM_SEGMENT_NUM - 1)) ? false : true)) {
			ret = -7;
			goto test_exit_4;
		}
	}

	if (mdma_chan_config(dev.chan_rd) || mdma_chan_config(dev.chan_wr)) {
		ret = -8;
		goto test_exit_5;
	}

	dev.tbl = rumboot_irq_create(NULL);
	dev.irq = MDMA0_IRQ;

	rumboot_irq_set_handler(dev.tbl, dev.irq, 0, mdma_gp_handler, (void*)&dev);
	rumboot_irq_table_activate(dev.tbl);
	rumboot_irq_enable(dev.irq);

	rumboot_irq_sei();

	if (mdma_chan_start(dev.chan_wr) || mdma_chan_start(dev.chan_rd)) {
		ret = -9;
		goto test_exit_6;
	}

	i = 0;
	j = 0;

	while ((dev.cnt_wr < MEM_SEGMENT_NUM) || (dev.cnt_rd < MEM_SEGMENT_NUM)) {
		if (!timeout) {
			ret = -10;
			goto test_exit_6;
		}

		if ((i != dev.cnt_wr) || (j != dev.cnt_rd)) {
			i = dev.cnt_wr;
			j = dev.cnt_rd;
		}
		else {
			timeout--;
			udelay(1);			
		}
	}

	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		for (j = 0; j < (2 * MEM_SEGMENT_SZ); j++) {
			if ((j < MEM_SEGMENT_SZ) &&
				(*((char *)(dev.dst_addr[0][i] + j)) !=  *((char *)(dev.src_addr[0][i] + j))))
				break;

			if (!(j < MEM_SEGMENT_SZ) &&
				(*((char *)(dev.dst_addr[0][i] + j)) ==  *((char *)(dev.src_addr[0][i] + j))))
				break;
		}

		for (j = 0; j < (2 * MEM_SEGMENT_SZ); j++) {
			if ((j < MEM_SEGMENT_SZ) &&
				(*((char *)(dev.dst_addr[1][i] + j)) !=  *((char *)(dev.src_addr[1][i] + j))))
				break;

			if (!(j < MEM_SEGMENT_SZ) &&
				(*((char *)(dev.dst_addr[1][i] + j)) ==  *((char *)(dev.src_addr[1][i] + j))))
				break;
		}
	}

	if (i != MEM_SEGMENT_NUM)
		ret = -11;

test_exit_6:
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(dev.tbl);
	rumboot_irq_cli();

test_exit_5:
	mdma_chan_reset(dev.chan_wr);

test_exit_4:
	rumboot_free(src_addr);

test_exit_3:
	rumboot_free(dst_addr);

test_exit_2:
	mdma_chan_destroy(dev.chan_wr);

test_exit_1:
	mdma_chan_destroy(dev.chan_rd);
	
	return ret;
}
