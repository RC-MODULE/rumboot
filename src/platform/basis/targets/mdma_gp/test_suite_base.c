#include "test_suite.h"

int mdma_gp_dev_init(struct mdma_gp *dev, int event_indx, int event_prior)
{
	int ret = 0;

	dev->cfg.event_indx = event_indx;
	dev->cfg.event_prior = event_prior;

	dev->cfg.sync_mode = MDMA_SYNC_SLAVE;

	dev->chan_rd = mdma_chan_create(dev->base_addr + MDMA_CHANNEL_R, &dev->cfg);
	if (dev->chan_rd) {
		ret = mdma_trans_create(dev->chan_rd, MDMA_TRANS_QUEUE);
		if (ret) {
			ret = -1;
			goto dev_init_exit_1;
		}
	}

	dev->cfg.sync_mode = MDMA_SYNC_MASTER;

	dev->chan_wr = mdma_chan_create(dev->base_addr + MDMA_CHANNEL_W, &dev->cfg);
	if (dev->chan_wr) {
		ret = mdma_trans_create(dev->chan_wr, MDMA_TRANS_QUEUE);
		if (ret) {
			ret = -2;
			goto dev_init_exit_2;
		}
	}

	if (!dev->chan_rd || !dev->chan_wr) {
		ret = -3;
		goto dev_init_exit_2;
	}

	rumboot_printf("MDMA(0x%x), init device\n", dev->base_addr);

	return ret;

dev_init_exit_2:
	mdma_chan_destroy(dev->chan_wr);
	dev->chan_wr = NULL;

dev_init_exit_1:
	mdma_chan_destroy(dev->chan_rd);
	dev->chan_rd = NULL;

	dev->cfg.event_indx = -1;

	return ret;
}

void mdma_gp_dev_destroy(struct mdma_gp *dev)
{
	mdma_chan_destroy(dev->chan_wr);
	dev->chan_wr = NULL;

	mdma_chan_destroy(dev->chan_rd);
	dev->chan_rd = NULL;

	rumboot_printf("MDMA(0x%x), destroy device\n", dev->base_addr);

	dev->cfg.event_indx = -1;

	return;
}

int mdma_gp_dev_config(struct mdma_gp *dev, void *src_addr, void *dst_addr, int segment_size)
{
	int ret = 0;

	dev->done = false;
	dev->segment_size = segment_size;

	dev->dst_addr = dst_addr;
	if (mdma_trans_add_group(dev->chan_wr, dev->dst_addr, dev->segment_size, 0, true)) {
		ret = -1;
		goto dev_config_exit_1;
	}

	if ((dst_addr >= (void *)0x40000000) && (dst_addr <= (void *)0x7FFFFFFF)) {
		if (dst_addr >= (void*)0x60000000)
			dev->dst_mirror = dst_addr - 0x60000000 + 0xC0000000;
		else if (dst_addr >= (void*)0x50000000)
			dev->dst_mirror = dst_addr - 0x50000000 + 0x80000000;
		else
			dev->dst_mirror = BASIS_VIRT(dst_addr);
	}
	else {
		dev->dst_mirror = dst_addr;
	}

	dev->src_addr = src_addr;
	if (mdma_trans_add_group(dev->chan_rd, dev->src_addr, dev->segment_size, 0, true)) {
		ret = -2;
		goto dev_config_exit_2;
	}

	if ((src_addr >= (void *)0x40000000) && (src_addr <= (void *)0x7FFFFFFF)) {
		if (src_addr >= (void*)0x60000000)
			dev->src_mirror = src_addr - 0x60000000 + 0xC0000000;
		else if (src_addr >= (void*)0x50000000)
			dev->src_mirror = src_addr - 0x50000000 + 0x80000000;
		else
			dev->src_mirror = BASIS_VIRT(src_addr);
	}
	else {
		dev->src_mirror = src_addr;
	}

	if (mdma_chan_config(dev->chan_wr) || mdma_chan_config(dev->chan_rd)) {
		ret = -3;
		goto dev_config_exit_2;
	}

	mdma_chan_attach(dev->chan_wr, dev->chan_rd);

	rumboot_printf("MDMA(0x%x), config device\n", dev->base_addr);

	return ret;

dev_config_exit_2:
	mdma_trans_free_groups(dev->chan_rd);
	dev->src_addr = NULL;
	dev->src_mirror = NULL;

dev_config_exit_1:
	mdma_trans_free_groups(dev->chan_wr);
	dev->dst_addr = NULL;
	dev->dst_mirror = NULL;

	dev->segment_size = 0;

	return ret;
}

void mdma_gp_dev_terminate(struct mdma_gp *dev)
{
	mdma_trans_free_groups(dev->chan_wr);
	dev->dst_addr = NULL;
	dev->dst_mirror = NULL;

	mdma_trans_free_groups(dev->chan_rd);
	dev->src_addr = NULL;
	dev->src_mirror = NULL;

	rumboot_printf("MDMA(0x%x), terminate device\n", dev->base_addr);

	dev->segment_size = 0;
	dev->done = false;

	return;
}

int mdma_gp_dev_start(struct mdma_gp *dev)
{
	int ret = 0;

	dev->start_l = 0;
	dev->start_h = 0;

	dev->end_l = 0;
	dev->end_h = 0;

	if (mdma_chan_start(dev->chan_wr) || mdma_chan_start(dev->chan_rd))
		ret = -1;

	rumboot_printf("MDMA(0x%x), start device\n", dev->base_addr);

	return ret;
}

bool mdma_gp_dev_check(struct mdma_gp *dev)
{
	struct mdma_trans *trans_rd, *trans_wr;
	int state_rd, state_wr;
	unsigned long long delta, start, end;
	bool ret = true;

	rumboot_printf("MDMA(0x%x), check device\n", dev->base_addr);

	state_rd = mdma_chan_get_state(dev->chan_rd);
	state_wr = mdma_chan_get_state(dev->chan_wr);

	if ((state_rd != MDMA_CHAN_DUMMY) || (state_wr != MDMA_CHAN_DUMMY)) {
		ret = false;
		goto dev_check_exit;
	}


	if ((dev->chan_rd->error != CHAN_NONE_ERR) || (dev->chan_wr->error != CHAN_NONE_ERR)) {
		ret = false;
		goto dev_check_exit;
	}

	trans_rd = mdma_chan_get_trans(dev->chan_rd);
	trans_wr = mdma_chan_get_trans(dev->chan_wr);

	if (mdma_trans_ignore_event(trans_rd) || mdma_trans_ignore_event(trans_wr)) {
		ret = false;
		goto dev_check_exit;
	}

	for (int i = 0; i < (dev->segment_size / sizeof(unsigned long)); i++) {
		if (*((unsigned long *)dev->dst_mirror + i) != *((unsigned long *)dev->src_mirror + i)) {
			ret = false;
			break;
		}
	}

	start = dev->start_h;
	start = (start << 32) + dev->start_l;

	end = dev->end_h;
	end = (end << 32) + dev->end_l;

	delta = (end - start) / 100;

	rumboot_printf("MDMA(0x%x) copy 0x%x bytes from 0x%x to 0x%x, elapsed time 0x%x_%x us\n",
		dev->base_addr, dev->segment_size, dev->src_addr, dev->dst_addr,
		(unsigned int)(delta >> 32), (unsigned int)delta);

dev_check_exit:
	return ret;
}

void mdma_gp_handler(int irq, void *arg)
{
	struct mdma_gp *dev = (struct mdma_gp *)arg;
	struct mdma_chan *chan;
	int flags, ret;
	bool is_rd;

	iowrite32(0x1, GLOBAL_TIMERS + FIX_CMD);

	flags = ioread32((unsigned long)dev->base_addr + MDMA_GP_STATUS);

	rumboot_printf("MDMA(0x%x) - IRQ flags = 0x%x\n", dev->base_addr, flags);

	while (flags) {
		if (flags & (1 << 16)) {
			chan = dev->chan_wr;
			is_rd = false;

			flags &= ~(1 << 16);
		}
		else if (flags & (1 << 0)) {
			chan = dev->chan_rd;
			is_rd = true;

			flags &= ~(1 << 0);
		}
		else {
			mdma_chan_reset(dev->chan_wr);
			break;
		}

		ret = mdma_chan_irq_cb(chan);

		if (ret == IRQ_CHAN_NONE)
			continue;

		if (ret & IRQ_CHAN_EVENT) {
			if (chan->trans->ignore_event) {
				mdma_chan_reset(dev->chan_wr);
				dev->done = true;

				break;
			}

			if (!is_rd) {
				dev->start_l = ioread32(GLOBAL_TIMERS + FREE_RUN_L);
				dev->start_h = ioread32(GLOBAL_TIMERS + FREE_RUN_H);
			}

			rumboot_printf("MDMA channel(0x%x) - event fire\n",
				dev->base_addr + ((is_rd) ? MDMA_CHANNEL_R : MDMA_CHANNEL_W));
		}

		if (ret & IRQ_CHAN_ERROR) {
			mdma_chan_reset(dev->chan_wr);
			dev->done = true;

			break;
		}

		if (!is_rd && (ret & IRQ_CHAN_DONE)) {
			dev->end_l = ioread32(GLOBAL_TIMERS + FREE_RUN_L);
			dev->end_h = ioread32(GLOBAL_TIMERS + FREE_RUN_H);

			dev->done = true;
		}
	}

	return;
}
