#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_mdma.h>

#include <devices/mdma_chan_api.h>

#define MEM_SEGMENT_SZ		27
#define MEM_SEGMENT_NUM		7

struct mdma_gp_dev {
	void *base_addr;	

	struct mdma_cfg cfg_rd;
	struct mdma_chan *chan_rd;

	struct mdma_cfg cfg_wr;
	struct mdma_chan *chan_wr;

	void *src[MEM_SEGMENT_NUM];
	void *dst[MEM_SEGMENT_NUM];
};

static int mdma_gp_dev_init(struct mdma_gp_dev *dev, void *src_addr, void *dst_addr, int pool_size)
{
	int i, ret = 0;

	iowrite32(1, (unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST));
	while (ioread32((unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST)) & 1) {};

	dev->chan_rd = mdma_chan_create(dev->base_addr + MDMA_CHANNEL_R, &dev->cfg_rd);
	if (dev->chan_rd) {
		ret = mdma_trans_create(dev->chan_rd, MDMA_TRANS_QUEUE);

		if (ret) {
			ret = -1;
			goto dev_init_exit_1;
		}
	}
	else {
		ret = -2;
		goto dev_init_exit_1;
	}

	dev->chan_wr = mdma_chan_create(dev->base_addr + MDMA_CHANNEL_W, &dev->cfg_wr);
	if (dev->chan_wr) {
		ret = mdma_trans_create(dev->chan_wr, MDMA_TRANS_QUEUE);

		if (ret) {
			ret = -3;
			goto dev_init_exit_2;
		}
	}
	else {
		ret = -4;
		goto dev_init_exit_2;
	}

	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		dev->dst[i] = dst_addr + i * (pool_size / MEM_SEGMENT_NUM);
		dev->src[i] = src_addr + i * (pool_size / MEM_SEGMENT_NUM);

		if (mdma_trans_add_group(dev->chan_wr, dev->dst[i], MEM_SEGMENT_SZ,
				0, (i < (MEM_SEGMENT_NUM - 1)) ? false : true)) {
			ret = -5;
			goto dev_init_exit_2;
		}

		if (mdma_trans_add_group(dev->chan_rd, dev->src[i], MEM_SEGMENT_SZ,
				0, (i < (MEM_SEGMENT_NUM - 1)) ? false : true)) {
			ret = -6;
			goto dev_init_exit_2;
		}
	}

	if (mdma_chan_config(dev->chan_wr) || mdma_chan_config(dev->chan_rd)) {
		ret = -7;
		goto dev_init_exit_2;
	}

	mdma_chan_attach(dev->chan_wr, dev->chan_rd);

	return ret;

dev_init_exit_2:
	mdma_chan_destroy(dev->chan_wr);

dev_init_exit_1:
	mdma_chan_destroy(dev->chan_rd);

	return ret;	
}

static void mdma_gp_dev_destroy(struct mdma_gp_dev *dev)
{
	mdma_chan_destroy(dev->chan_wr);
	mdma_chan_destroy(dev->chan_rd);

	return;
}

static int mdma_gp_dev_start(struct mdma_gp_dev *dev)
{
	int ret = 0;

	if (mdma_chan_start(dev->chan_wr) || mdma_chan_start(dev->chan_rd))
		ret = -1;

	return ret;
}

static int mdma_gp_wait_end(struct mdma_gp_dev *dev)
{
	int tmp;

	mdma_chan_wait_trans_end(dev->chan_rd, 100);

	tmp = mdma_chan_get_state(dev->chan_rd);
        if (tmp != MDMA_CHAN_DUMMY)
                return -1;

        mdma_chan_wait_trans_end(dev->chan_wr, 100);

        tmp = mdma_chan_get_state(dev->chan_wr);
        if (tmp != MDMA_CHAN_DUMMY)
                return -2;

	return 0;
}

static int mdma_gp_wait_event(struct mdma_gp_dev *dev)
{
	int ret = 0;

	if (mdma_chan_wait_event(dev->chan_wr, 100) ||
			mdma_chan_wait_event(dev->chan_rd, 100))
		ret = -1;

	return ret;
}

int main()
{
	struct mdma_gp_dev dev_master;
	struct mdma_gp_dev dev_slave;

	void *first_addr, *second_addr, *third_addr;
	int i, j, buf_size;
	int tmp, ret = 0;

	rumboot_printf("MDMA1,2 - allocate memory.\n");

	buf_size = MEM_SEGMENT_NUM *
		((MEM_SEGMENT_SZ / MDMA_BURST_WIDTH8) * MDMA_BURST_WIDTH8 + MDMA_BURST_WIDTH8);

	first_addr = rumboot_malloc_from_heap_aligned(0, buf_size, MDMA_BURST_WIDTH8);
	if (!first_addr) {
		ret = -1;
		goto test_exit_1;
	}

	second_addr = rumboot_malloc_from_heap_aligned(1, buf_size, MDMA_BURST_WIDTH8);
	if (!second_addr) {
		ret = -2;
		goto test_exit_2;
	}

	third_addr = rumboot_malloc_from_heap_aligned(1, buf_size, MDMA_BURST_WIDTH8);
	if (!third_addr) {
		ret = -3;
		goto test_exit_3;
	}

	for (i = 0; i < buf_size; i++) {
		*((char *)(first_addr + i)) = i | 0x80;
		*((char *)(second_addr + i)) = 0;
		*((char *)(third_addr + i)) = 0;
	}

	dev_master.base_addr = (void *)MDMA1_BASE;

	dev_master.cfg_rd.mode = dev_master.cfg_wr.mode = MDMA_CHAN_POLLING;
	dev_master.cfg_rd.desc_kind = dev_master.cfg_wr.desc_kind = NORMAL_DESC;
	dev_master.cfg_rd.heap_id = dev_master.cfg_wr.heap_id = 0;
	dev_master.cfg_rd.burst_width = dev_master.cfg_wr.burst_width = MDMA_BURST_WIDTH8;
	dev_master.cfg_rd.event_indx = dev_master.cfg_wr.event_indx = -1;
	dev_master.cfg_rd.add_info = dev_master.cfg_wr.add_info = false;
	dev_master.cfg_rd.addr_inc = dev_master.cfg_wr.addr_inc = true;

	rumboot_printf("MDMA1 - channels configure.\n");

	if (mdma_gp_dev_init(&dev_master, first_addr, second_addr, buf_size)) {
		ret = -4;
		goto test_exit_4;
	}

	dev_slave.base_addr =(void *) MDMA2_BASE;

	dev_slave.cfg_rd.mode = dev_slave.cfg_wr.mode = MDMA_CHAN_POLLING;
	dev_slave.cfg_rd.desc_kind = dev_slave.cfg_wr.desc_kind = NORMAL_DESC;
	dev_slave.cfg_rd.heap_id = dev_slave.cfg_wr.heap_id = 0;
	dev_slave.cfg_rd.burst_width = dev_slave.cfg_wr.burst_width = MDMA_BURST_WIDTH8;
	dev_slave.cfg_rd.event_indx = dev_slave.cfg_wr.event_indx = 1;
	dev_slave.cfg_rd.event_prior = dev_slave.cfg_wr.event_prior = 7;
	dev_slave.cfg_rd.signal_time = dev_slave.cfg_wr.signal_time = 0;
	dev_slave.cfg_rd.add_info = dev_slave.cfg_wr.add_info = false;
	dev_slave.cfg_rd.addr_inc = dev_slave.cfg_wr.addr_inc = true;
	dev_slave.cfg_rd.sync_mode = MDMA_SYNC_SLAVE;
	dev_slave.cfg_wr.sync_mode = MDMA_SYNC_MASTER;

	rumboot_printf("MDMA2 - channels configure.\n");

	if (mdma_gp_dev_init(&dev_slave, second_addr, third_addr, buf_size)) {
		ret = -5;
		goto test_exit_5;
	}

	rumboot_printf("MDMA2 - channels wait.\n");

	if (mdma_gp_dev_start(&dev_slave)) {
		ret = -6;
		goto test_exit_6;
	}

	rumboot_printf("MDMA1 - channels start.\n");

	if (mdma_gp_dev_start(&dev_master)) {
		ret = -7;
		goto test_exit_6;
	}

	if (mdma_gp_wait_event(&dev_slave)) {
		ret = -8;
		goto test_exit_6;	
	}

	rumboot_printf("MDMA2 - channels start.\n");

	if (mdma_gp_wait_end(&dev_slave)) {
		ret = -9;
		goto test_exit_6;
	}

	rumboot_printf("MDMA2 - channels end.\n");
	rumboot_printf("MDMA1,2 - channels check.\n");
	
	for (i = 0; i < MEM_SEGMENT_NUM; i++) {
		tmp = buf_size / MEM_SEGMENT_NUM;

		for (j = 0; j < tmp; j++) {
			void *dst_addr, *src_addr;

			dst_addr = dev_slave.dst[i] + j;
			src_addr = dev_master.src[i] + j;
#if 1
			if ((j < MEM_SEGMENT_SZ) && (*((char *)(dst_addr)) != *((char *)(src_addr)))) {
				ret = -10;
				goto test_exit_5;
			}

			if (!(j <  MEM_SEGMENT_SZ) && (*((char *)(dst_addr)) == *((char *)(src_addr)))) {
				ret = -11;
				goto test_exit_5;
			}
#else
			rumboot_printf("src[0x%x] == 0x%x <-> dst[0x%x] == 0x%x\n",
					src_addr, *(char *)src_addr, dst_addr, *(char *)dst_addr);
#endif
		}
	}

test_exit_6:
	mdma_gp_dev_destroy(&dev_slave);

test_exit_5:
	mdma_gp_dev_destroy(&dev_master);

test_exit_4:
	rumboot_free(third_addr);

test_exit_3:
	rumboot_free(second_addr);

test_exit_2:
	rumboot_free(first_addr);

test_exit_1:	
	return ret;
}
