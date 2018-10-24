#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <rumboot/rumboot.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <devices/mdma_chan_api.h>

#undef MDMA_DEAD_LOOP

#ifdef MDMA_DEAD_LOOP
#define MDMA_CHAN_BUG(format, ...) \
	do { \
		volatile int _lock_ = 1;\
		rumboot_printf("BUG %s#%d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
		RUMBOOT_ATOMIC_BLOCK() { \
			while (_lock_) {}; \
		} \
	} while (0)
#else
#define MDMA_CHAN_BUG(format, ...) \
	do { \
		rumboot_platform_panic("BUG: " format "\n", ##__VA_ARGS__); \
	} while (0)
#endif

static int chan_trans_progress(struct mdma_chan *chan, int budget);

struct mdma_chan *mdma_chan_create(void *base_addr, struct mdma_cfg *cfg)
{
	struct mdma_chan *chan = NULL;

	chan_print_dbg(1, "start -->");

	if (!cfg || !base_addr)
		goto chan_create_exit;

	chan = rumboot_malloc(sizeof(struct mdma_chan));
	if (!chan) {
		rumboot_printf("\tChannel create: Unable allocate memory.\n");
		goto chan_create_exit;
	}

	chan->regs = (struct mdma_chan_regs *)base_addr;
	chan->state = MDMA_CHAN_DUMMY;
	chan->error = CHAN_NONE_ERR;
	
	chan->cfg = *cfg;
	chan->trans = NULL;

	chan->slave = NULL;
	chan->master = NULL;

	chan_print_dbg(0, "channel(0x%x) ---> base address(0x%x)", chan, chan->regs);

chan_create_exit:
	chan_print_dbg(1, "<-- end");

	return chan;
}

int mdma_chan_attach(struct mdma_chan *master, struct mdma_chan *slave)
{
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!master || !slave) {
		ret = -1;
		goto chan_attach_exit;
	}

	if ((master->slave != NULL) || (slave->master != NULL)) {
		ret = -2;
		goto chan_attach_exit;
	}

	if ((master->state != MDMA_CHAN_READY) || (slave->state != MDMA_CHAN_READY)) {
		ret = -3;
		goto chan_attach_exit;
	}

	master->slave = slave;
	master->master = NULL;

	slave->master = master;
	slave->slave = NULL;

	chan_print_dbg(0, "channel(0x%x) ---> channel(0x%x)", master, master->slave);

chan_attach_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

void mdma_chan_destroy(struct mdma_chan *chan)
{
	chan_print_dbg(1, "start -->");

	if (!chan)
		goto chan_destroy_exit;

	mdma_trans_destroy(chan);

	if (chan->master) {
		chan->master->slave = NULL;
		chan->master = NULL;
	}

	if (chan->slave) {
		chan->slave->master = NULL;
		chan->slave = NULL;
	}

	chan->regs = NULL;
	rumboot_free(chan);

	chan_print_dbg(0, "channel(0x%x)", chan);

chan_destroy_exit:
	chan_print_dbg(1, "<-- end");

	return;
}

int mdma_trans_create(struct mdma_chan *chan, enum mdma_trans_type trans_type)
{
	struct mdma_trans *trans;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans != NULL)) {
		ret = -1;
		goto trans_create_exit;
	}

	if (chan->state != MDMA_CHAN_DUMMY) {
		ret = -2;
		goto trans_create_exit;
	}

	trans = rumboot_malloc(sizeof(struct mdma_trans));
	if (!trans) {
		rumboot_printf("\tTransfer create: Unable allocate memory.\n");

		ret = -3;
		goto trans_create_exit;
	}

	trans->chan = chan;
	INIT_LIST_HEAD(&trans->group_list);
	trans->group_nmb = 0;

	trans->trans_type = trans_type;
	trans->ignore_event = false;

	trans->first_group = NULL;
	trans->cur_group = NULL;
	trans->next_group = NULL;
	trans->last_group = NULL;

	trans->group_wait = 0;
	
	chan->trans = trans;

	chan_print_dbg(0, "channel(0x%x) ---> transfer(0x%x)", chan, chan->trans);

trans_create_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

void mdma_trans_destroy(struct mdma_chan *chan)
{
	struct mdma_trans *trans;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans == NULL))
		goto trans_destroy_exit;

	trans = chan->trans;
	mdma_trans_free_groups(chan);

	trans->chan = NULL;
	
	rumboot_free(trans);
	chan->trans = NULL;

	chan_print_dbg(0, "channel(0x%x) ---x transfer(0x%x)", chan, trans);

trans_destroy_exit:
	chan_print_dbg(1, "<-- end");

	return;
}

int mdma_trans_add_group(struct mdma_chan *chan, void *mem_addr, size_t mem_size,
			int custom_flags, bool is_last)
{
	struct mdma_trans *trans;
	struct mdma_group *group;
	struct mdma_node *node;
	struct mdma_cfg *cfg;
	int desc_align, gap_size;
	int i, num_node;
	size_t size_limit;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans == NULL)) {
		ret = -1;
		goto trans_add_group_exit;
	}

	if (chan->state != MDMA_CHAN_DUMMY) {
		ret = -2;
		goto trans_add_group_exit;
	}

	trans = chan->trans;

	if (trans->last_group != NULL) {
		ret = -3;
		goto trans_add_group_exit;
	}

	if (!mem_addr || !mem_size) {
		ret = -4;
		goto trans_add_group_exit;
	}

	cfg = &chan->cfg;

	if ((cfg->burst_width != MDMA_BURST_WIDTH4) &&
		(cfg->burst_width != MDMA_BURST_WIDTH8)) {
		rumboot_printf("\tAdd group: Uncorrectable burst width.\n");

		ret = -5;
		goto trans_add_group_exit;
	}

	if (((size_t)mem_addr) & (cfg->burst_width - 1)) {
		ret = -7;
		goto trans_add_group_exit;
	}

	if (!cfg->addr_inc && (mem_size & (cfg->burst_width - 1))) {
		ret = -8;
		goto trans_add_group_exit;
	}

	chan_print_dbg(0, "transfer(0x%x) ---> memory address(0x%x)", trans, mem_addr);
	chan_print_dbg(0, "transfer(0x%x) ---> memory size = 0x%x", trans, mem_size);

	size_limit = 0;
		
	if (cfg->desc_kind == NORMAL_DESC) {
		gap_size = sizeof(struct __normal_desc);
		desc_align = __alignof__(struct __normal_desc);
	}
	else if (cfg->desc_kind == LONG_DESC) {
		gap_size = sizeof(struct __long_desc);
		desc_align = __alignof__(struct __long_desc);
	}
	else if (cfg->desc_kind == PITCH_DESC) {
		gap_size = sizeof(struct __pitch_desc);
		desc_align = __alignof__(struct __pitch_desc);

		if (!cfg->addr_inc) {
			ret = -9;
			goto trans_add_group_exit;
		}
		
		if (!cfg->str_length) {
			ret = -10;
			goto trans_add_group_exit;
		}
		else if (!(cfg->str_length < MDMA_STRLEN_LIMIT)) {
			ret = -11;
			goto trans_add_group_exit;
		}

		if(cfg->pitch_width) {
			if (cfg->pitch_width < cfg->str_length) {
				ret = -12;
				goto trans_add_group_exit;
			}
			
			if (!(cfg->pitch_width < MDMA_PITCH_LIMIT)) {
				ret = -13;
				goto trans_add_group_exit;
			}
			
			if (cfg->pitch_width & (cfg->burst_width - 1)) {
				ret = -14;
				goto trans_add_group_exit;
			}
		}

		chan_print_dbg(0, "transfer(0x%x) ---> string length = 0x%x", trans, cfg->str_length);
		chan_print_dbg(0, "transfer(0x%x) ---> pitch width = 0x%x", trans, cfg->pitch_width);

		num_node = mem_size / cfg->str_length;

		if (num_node < 0) {
			ret = -15;
			goto trans_add_group_exit;
		}
		
		size_limit = cfg->str_length * num_node;

		if (size_limit != mem_size) {
			ret = -16;
			goto trans_add_group_exit;
		}

		if (cfg->add_info) {
			if (!(num_node < MDMA_LENGTH_LIMIT(0)))
				size_limit = (MDMA_LENGTH_LIMIT(0) - 1) * cfg->str_length;
			else
				size_limit = mem_size;
		}
		else {
			if (!(num_node < MDMA_LENGTH_LIMIT(1)))
				size_limit = (MDMA_LENGTH_LIMIT(1) - 1) * cfg->str_length;
			else
				size_limit = mem_size;
		}
	}
	else {
		rumboot_printf("\tAdd group: Unknown descriptor kind.\n");

		ret = -18;
		goto trans_add_group_exit;
	}

	if (cfg->desc_kind != PITCH_DESC) {
		if (cfg->add_info) {
			if (!(mem_size < MDMA_LENGTH_LIMIT(0)))
				size_limit = (MDMA_LENGTH_LIMIT(0) - 1) & ~(cfg->burst_width - 1);
			else
				size_limit = mem_size;
		}
		else {
			if (!(mem_size < MDMA_LENGTH_LIMIT(1)))
				size_limit = (MDMA_LENGTH_LIMIT(1) - 1) & ~(cfg->burst_width - 1);
			else
				size_limit = mem_size;
		}
	}
	
	num_node = mem_size / size_limit;

	if ((num_node < 0) || ((num_node + 2) < 0)) {
		ret = -19;
		goto trans_add_group_exit;
	}
	
	if ((num_node * size_limit) != mem_size)
		num_node++;

	chan_print_dbg(0, "transfer(0x%x) ---> data size limit = 0x%x, number of data nodes = 0x%x",
			trans, size_limit, num_node);
	
	group = rumboot_malloc(sizeof(struct mdma_group));
	if (!group) {
		rumboot_printf("\tAdd group: Unable allocate memory.\n");

		ret = -20;
		goto trans_add_group_exit;
	}

	group->trans = trans;
	group->cur_indx = -1;
	group->gap_size = gap_size;
	group->group_done = false;

	if (!is_last)
		group->node_nmb = num_node + 1;
	else if (trans->trans_type == MDMA_TRANS_QUEUE)
		group->node_nmb = num_node;
	else if (trans->trans_type == MDMA_TRANS_RING)
		group->node_nmb = num_node + 1;
	else {
		rumboot_printf("\tAdd group: Unknown transfer type.\n");
		rumboot_free(group);

		ret = -21;
		goto trans_add_group_exit;
	}

	group->node_group = rumboot_malloc(group->node_nmb * sizeof(struct mdma_node));
	if (group->node_group == NULL) {
		rumboot_printf("\tAdd group: Unable allocate memory.\n");
		rumboot_free(group);

		ret = -22;
		goto trans_add_group_exit;
	}	

	group->desc_pool_size = group->node_nmb * group->gap_size;
	group->desc_pool_addr = rumboot_malloc_from_heap_aligned(cfg->heap_id,
								group->desc_pool_size,
								desc_align);
	if (group->desc_pool_addr == NULL) {
		rumboot_printf("\tAdd group: Unable allocate descriptors pool - heap(0x%x), size = 0x%x.\n",
				cfg->heap_id, group->desc_pool_size);
		rumboot_free(group->node_group);
		rumboot_free(group);

		ret = -23;
		goto trans_add_group_exit;
	}

	chan_print_dbg(0, "group(0x%x) ---> nodes pool address(0x%x), number of nodes = 0x%x",
			group, group->node_group, group->node_nmb);
	chan_print_dbg(0, "group(0x%x) ---> descriptors pool address(0x%x), size of descriptor = 0x%x",
			group, group->desc_pool_addr, group->gap_size);
	
	if (trans->first_group == NULL) {
		trans->first_group = group;
		chan_print_dbg(0, "transfer(0x%x) ---> first group address(0x%x)", trans, group);
	}
	
	if (is_last) {
		trans->last_group = group;
		chan_print_dbg(0, "transfer(0x%x) ---> last group address(0x%x)", trans, group);
	}

	node = NULL;

	for (i = 0; i < num_node; i++) {
		node = &group->node_group[i];

		node->group = group;
		node->desc_type	= DESC_DATA;
		node->desc_indx = i;

		chan_print_dbg(0, "group(0x%x) ---> data node address(0x%x), index = 0x%x",
				node->group, node, node->desc_indx);

		node->mem_addr = mem_addr;

		if (!(mem_size < size_limit))
			mem_size -= size_limit;
		else
			size_limit = mem_size;

		if (cfg->desc_kind == PITCH_DESC) {
			node->mem_size = size_limit / cfg->str_length;
			
			if (cfg->pitch_width)
				mem_addr += node->mem_size * cfg->pitch_width;
		}
		else {
			node->mem_size = size_limit;
			
			if (cfg->addr_inc)
				mem_addr += node->mem_size; 
		}
		
		chan_print_dbg(0, "node(0x%x) ---> data address(0x%x), data size = 0x%x",
				node, node->mem_addr, node->mem_size);

		node->custom_flags = custom_flags;
		node->is_stop = false;
		node->is_last = false;
	}
	
	if (i < group->node_nmb) {
		node = &group->node_group[i];

		node->group = group;
		node->desc_type = DESC_LINK;
		node->desc_indx = i;

		chan_print_dbg(0, "group(0x%x) ---> link node address(0x%x), index = 0x%x",
				group, node, node->desc_indx);

		node->mem_addr = NULL;
		node->mem_size = 0;

		node->custom_flags = 0;
		node->is_stop = false;
		node->is_last = true;
		
		if (is_last) {
			node->mem_addr = trans->first_group->desc_pool_addr;

			chan_print_dbg(0, "node(0x%x) is last node of ring, link at address(0x%x)",
					node, node->mem_addr);
		}
	}
	else {
		node->is_stop = true;
		node->is_last = true;

		chan_print_dbg(0, "node(0x%x) is last node of queue", node);
	}

	if (!list_empty(&trans->group_list)) {
		struct mdma_group *prev_group;
		struct mdma_node *_node;
		int node_indx;
		
		prev_group = list_last_entry(&trans->group_list, struct mdma_group, group_node);
		
		node_indx = prev_group->node_nmb - 1;
		_node = &prev_group->node_group[node_indx];

		_node->mem_addr = group->desc_pool_addr;

		chan_print_dbg(0, "group(0x%x) ---> node(0x%x) is link at address(0x%x)",
				prev_group, _node, _node->mem_addr);
	}

	list_add_tail(&group->group_node, &trans->group_list);
	trans->group_nmb++;

	chan_print_dbg(0, "transfer(0x%x) ---> group(0x%x)", trans, group);

trans_add_group_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

void mdma_trans_free_groups(struct mdma_chan *chan)
{
	struct mdma_trans *trans;
	struct mdma_group *group, *next;
	struct mdma_node *node;
	int i;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans == NULL))
		goto trans_free_groups_exit;

	trans = chan->trans;

	if (list_empty(&trans->group_list))
		goto trans_free_groups_exit;

	chan_print_dbg(0, "channel(0x%x) ---> transfer(0x%x)", chan, trans);

	mdma_chan_reset(chan);

	list_for_each_entry_safe(group, next, &trans->group_list, group_node) {
		chan_print_dbg(0, "transfer(0x%x) ---x group(0x%x)", trans, group);

		list_del(&group->group_node);
		group->trans = NULL;

		for (i = 0; i < group->node_nmb; i++) {
			node = &group->node_group[i];

			chan_print_dbg(0, "group(0x%x) ---x node(0x%x)", group, node);

			node->group = NULL;
			node->mem_addr = NULL;
		}

		rumboot_free(group->node_group);
		group->node_group = NULL;

		chan_print_dbg(0, "group(0x%x) ---x descriptor poll(0x%x)",
				group, group->desc_pool_addr);

		rumboot_free(group->desc_pool_addr); // allocate from specific heap
		group->desc_pool_addr = NULL;

		rumboot_free(group);
	}

	INIT_LIST_HEAD(&trans->group_list);
	trans->group_nmb = 0;

	trans->first_group = NULL;
	trans->cur_group = NULL;
	trans->next_group = NULL;
	trans->last_group = NULL;

	trans->group_wait = 0;

trans_free_groups_exit:
	chan_print_dbg(1, "<-- end");

	return;
}

static int chan_set_desc(struct mdma_node *node, struct mdma_cfg *cfg)
{
	void *desc_pool = node->group->desc_pool_addr;
	uint32_t val, addr;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	val = 0;

	if (node->desc_type == DESC_LINK)
		val |= MDMA_DESC_LINK;
	
	if (node->is_last)
		val |= MDMA_DESC_INTERRUPT;

	if (node->desc_type == DESC_DATA) {
		if (cfg->add_info) {
			val |= MDMA_DESC_CUSTOM_W(node->custom_flags);
			val |= MDMA_DESC_LENGTH(node->mem_size);
		}
		else
			val |= MDMA_DESC_LENGTH_EXT(node->mem_size);

		if (!cfg->addr_inc)
			val |= MDMA_DESC_INCREMENT;
	}
	
	if (node->is_stop)
		val |= MDMA_DESC_STOP;
	
	addr = (uint32_t)node->mem_addr;

	switch (cfg->desc_kind) {
	case NORMAL_DESC:
		do {
			struct __normal_desc *normal_desc,
				*normal_pool = (struct __normal_desc *)desc_pool;

			normal_desc = &normal_pool[node->desc_indx];

			chan_print_dbg(0, "node(0x%x) settings to normal descriptor(0x%x), index = 0x%x",
					node, normal_desc, node->desc_indx);
			
			mdma_chan_write32(addr, (size_t)(&normal_desc->memptr));
			mdma_chan_write32(val, (size_t)(&normal_desc->flags_length));
		} while (0);

		break;
	case LONG_DESC:
		do {
			struct __long_desc *long_desc,
				*long_pool = (struct __long_desc *)desc_pool;

			long_desc = &long_pool[node->desc_indx];

			chan_print_dbg(0, "node(0x%x) settings to long descriptor(0x%x), index = 0x%x",
					node, long_desc, node->desc_indx);

			iowrite32(0, (size_t)(&long_desc->usrdata_l));
			iowrite32(0, (size_t)(&long_desc->usrdata_h));
			mdma_chan_write32(addr, (size_t)(&long_desc->memptr));
			mdma_chan_write32(val, (size_t)(&long_desc->flags_length));
		} while (0);

		break;
	case PITCH_DESC:
		do {
			struct __pitch_desc *pitch_desc,
				*pitch_pool = (struct __pitch_desc *)desc_pool;
			uint32_t tmp;

			pitch_desc = &pitch_pool[node->desc_indx];

			chan_print_dbg(0, "node(0x%x) settings to pitch descriptor(0x%x), index = 0x%x",
					node, pitch_desc, node->desc_indx);

			iowrite32(0, (size_t)(&pitch_desc->usrdata));

			tmp = MDMA_DESC_PITCH(cfg->pitch_width);
			tmp |= MDMA_DESC_STRLEN(cfg->str_length);
			mdma_chan_write32(tmp, (size_t)(&pitch_desc->pitch_strlen));

			mdma_chan_write32(addr, (size_t)(&pitch_desc->memptr));
			mdma_chan_write32(val, (size_t)(&pitch_desc->flags_length));
		} while (0);

		break;
	default:
		ret = -1;
	}

	chan_print_dbg(1, "<-- end");
	
	return ret;
}

int mdma_chan_config(struct mdma_chan *chan)
{
	struct mdma_trans *trans;
	struct mdma_group *group;
	struct mdma_node *node;
	struct mdma_cfg *cfg;
	uint32_t val;
	int i, ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->state != MDMA_CHAN_DUMMY)) {
		ret = -1;
		goto chan_config_exit;
	}

	trans = chan->trans;

	if (!trans || (trans->last_group == NULL)) {
		ret = -2;
		goto chan_config_exit;
	}

	cfg = &chan->cfg;

	if ((cfg->mode != MDMA_CHAN_INTERRUPT) &&
			(cfg->mode != MDMA_CHAN_POLLING)) {
		rumboot_printf("\tChannel configure: Unknown channel mode.\n");

		ret = -3;
		goto chan_config_exit;
	}

	chan_print_dbg(0, "channel(0x%x) ---> transfer(0x%x), number of groups = 0x%x",
			chan, trans, trans->group_nmb);

	list_for_each_entry(group, &trans->group_list, group_node) {
		chan_print_dbg(0, "transfer(0x%x) ---> group(0x%x), number of nodes = 0x%x",
				group->trans, group, group->node_nmb);

		if ((trans->trans_type == MDMA_TRANS_RING) && (trans->group_nmb < 2)) {
			if (group->node_nmb < 4) {
				ret = -4;
				goto chan_config_exit;
			}
		}
	
		for (i = 0; i < group->node_nmb; i++) {
			node = &group->node_group[i];

			if (chan_set_desc(node, cfg)) {
				ret = -5;
				goto chan_config_exit;
			}
		}

		group->cur_indx = 0;
		group->group_done = false;
	}

	trans->cur_group = trans->first_group;
	trans->next_group = NULL;
	trans->group_wait = 0;
	trans->ignore_event = false;

	mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));

	val = MDMA_SETTINGS_DESC_KIND(cfg->desc_kind);
	val |= MDMA_SETTINGS_DESC_GAP(trans->first_group->gap_size);
	
	if (cfg->add_info)
		val |= MDMA_SETTINGS_ADD_INFO;

	mdma_chan_write32(val, (size_t)(&chan->regs->settings));
	
	val = (uint32_t)trans->first_group->desc_pool_addr;
	mdma_chan_write32(val, (size_t)(&chan->regs->desc_addr));

	if (!(cfg->event_indx < 0)) {
		uint32_t prior_l, prior_h;

		if (!(cfg->event_indx < MDMA_EVENT_MAX_NUM) ||
			!(cfg->event_prior < MDMA_PRIOR_MAX_NUM)) {
			ret = -6;
			goto chan_config_exit;
		}

		if (cfg->sync_mode == MDMA_SYNC_SLAVE)
			mdma_chan_write32(MDMA_SYNC_EVENT_SLAVE, (size_t)(&chan->regs->synch_events));
		else if (cfg->sync_mode == MDMA_SYNC_MASTER)
			mdma_chan_write32(MDMA_SYNC_EVENT_MASTER, (size_t)(&chan->regs->synch_events));
		else if (cfg->sync_mode == MDMA_SYNC_NONE)
			mdma_chan_write32(0, (size_t)(&chan->regs->synch_events));
		else {
			rumboot_printf("\tChannel configure: Unknown synchronization mode.\n");

			ret = -7;
			goto chan_config_exit;
		}

		mdma_chan_write32(0, (size_t)(&chan->regs->sense_list));
		mdma_chan_write32(0, (size_t)(&chan->regs->activ_events));
		mdma_chan_read32((size_t)(&chan->regs->ignore_events));

		if (cfg->signal_time > 0)
			mdma_chan_write32(cfg->signal_time, (size_t)(&chan->regs->signal_time));

		prior_h = mdma_chan_read32((size_t)(&chan->regs->events_prior_h));
		prior_l = mdma_chan_read32((size_t)(&chan->regs->events_prior_l));

		for (i = 0; i < MDMA_EVENT_MAX_NUM; i++) {
			if (i < 8)
				val = MDMA_GET_PRIOR(i, prior_l);
			else
				val = MDMA_GET_PRIOR(i - 8, prior_h);

			if (val == cfg->event_prior)
				break;
		}

		if (i == MDMA_EVENT_MAX_NUM)
			MDMA_CHAN_BUG("channel(0x%x) - unknown event prior", chan);

		if (i != cfg->event_indx) {
			if (cfg->event_indx < 8) {
				val = MDMA_GET_PRIOR(cfg->event_indx, prior_l);
				prior_l &= ~MDMA_SET_PRIOR(cfg->event_indx, 0xF);
				prior_l |= MDMA_SET_PRIOR(cfg->event_indx, cfg->event_prior);
			}
			else {
				val = MDMA_GET_PRIOR(cfg->event_indx - 8, prior_h);
				prior_h &= ~MDMA_SET_PRIOR(cfg->event_indx - 8, 0xF);
				prior_h |= MDMA_SET_PRIOR(cfg->event_indx - 8, cfg->event_prior);
			}

			if (i < 8) {
				prior_l &= ~MDMA_SET_PRIOR(i, 0xF);
				prior_l |= MDMA_SET_PRIOR(i, val);
			}
			else {
				prior_h &= ~MDMA_SET_PRIOR(i - 8, 0xF);
				prior_h |= MDMA_SET_PRIOR(i - 8, val);
			}

			mdma_chan_write32(prior_l, (size_t)(&chan->regs->events_prior_l));
			mdma_chan_write32(prior_h, (size_t)(&chan->regs->events_prior_h));
		}

		val = (uint32_t)trans->first_group->desc_pool_addr;
		mdma_chan_write32(val, (size_t)(&chan->regs->event_desc_addr[cfg->event_prior]));
	}

	mdma_chan_read32((size_t)(&chan->regs->status));

	chan_trans_progress(chan, trans->group_nmb);
	chan->state = MDMA_CHAN_READY;
	chan->error = CHAN_NONE_ERR;

	chan_print_dbg(0, "channel(0x%x) - state = 0x%x", chan, chan->state);

chan_config_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_reset(struct mdma_chan *chan)
{
	struct mdma_cfg *cfg;
	uint32_t timeout = MDMA_CHAN_TIMEOUT;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan) {
		ret = -1;
		goto chan_reset_exit;
	}
	
	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}

	cfg = &chan->cfg;

	if (!(cfg->event_indx < 0)) {
		mdma_chan_write32(0, (size_t)(&chan->regs->sense_list));
		
		mdma_chan_write32(0, (size_t)(&chan->regs->activ_events));
		mdma_chan_read32((size_t)(&chan->regs->ignore_events));
	}

	mdma_chan_write32(MDMA_CANCEL, (size_t)(&chan->regs->cancel));

	if (chan->slave)
		mdma_chan_reset(chan->slave);

	if (mdma_chan_read32((size_t)(&chan->regs->suspend)) & MDMA_SUSPEND)
		mdma_chan_write32(0, (size_t)(&chan->regs->suspend));
	
	while (mdma_chan_read32((size_t)(&chan->regs->enable)) & MDMA_ENABLE) {
		if(!timeout)
			MDMA_CHAN_BUG("channel(0x%x) - reset timeout", chan);
		else
			timeout--;

		udelay(1);
	}
	
	if (mdma_chan_read32((size_t)(&chan->regs->cancel)) & MDMA_CANCEL)
		MDMA_CHAN_BUG("channel(0x%x) - reset error", chan);

	mdma_chan_read32((size_t)(&chan->regs->status));

	chan->state = MDMA_CHAN_DUMMY;

	chan_print_dbg(0, "channel(0x%x) - state = 0x%x", chan, chan->state);

chan_reset_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

static int chan_is_error(struct mdma_chan *chan, uint32_t status)
{
	uint32_t err = MDMA_STATUS_DISCARD_DESC | MDMA_STATUS_WAXI_ERR |
			MDMA_STATUS_AXI_ERR;
	int ret = 0;

	if (status & err) {
		err &= status;

		if (err & MDMA_STATUS_DISCARD_DESC)
			chan->error = CHAN_RDESC_ERR;
		else if (err & MDMA_STATUS_WAXI_ERR)
			chan->error = CHAN_WDESC_ERR;
		else
			chan->error = CHAN_DATA_ERR;

		chan->state = MDMA_CHAN_ERROR;
		ret = 1;
	}

	chan_print_dbg(1, "channel(0x%x) - state = 0x%x, error = 0x%x",
			chan, chan->state, chan->error);
	
	return ret;
}

static int chan_check_event(struct mdma_chan *chan, uint32_t status)
{
	uint32_t event_active = 0, event_ignore = 0;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (status & MDMA_STATUS_START_EVENT) {
		mdma_chan_write32(0, (size_t)(&chan->regs->sense_list));

		event_active = mdma_chan_read32((size_t)(&chan->regs->activ_events));
		mdma_chan_write32(0, (size_t)(&chan->regs->activ_events));
		chan->state = MDMA_CHAN_RUNNING;

		if (event_active & ~(1 << chan->cfg.event_prior))
			MDMA_CHAN_BUG("channel(0x%x) - unknown event", chan);

		ret |= 1;
	}
	
	if (status & MDMA_STATUS_IGNORE_EVENT) {
		event_ignore = mdma_chan_read32((size_t)(&chan->regs->ignore_events));
		chan->trans->ignore_event = true;

		if (event_ignore & ~(1 << chan->cfg.event_prior))
			MDMA_CHAN_BUG("channel(0x%x) - unknown event", chan);

		ret |= 2;
	}
	
	chan_print_dbg(0, "channel(0x%x) - state = 0x%x, event_active = 0x%x, event_ignore = 0x%x",
			chan, chan->state, event_active, event_ignore);

	chan_print_dbg(1, "<-- end");

	return ret;
}

static int chan_trans_progress(struct mdma_chan *chan, int budget)
{
	struct mdma_trans *trans;
	struct mdma_group *group;
	void *desc_flags;
	uint32_t val, flags;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (budget < 0) {
		ret = -1;
		goto trans_progress_exit;
	}
	
	if (!chan || (chan->trans == NULL)) {
		ret = -2;
		goto trans_progress_exit;
	}

	trans = chan->trans;

	if ((trans->first_group == NULL) || (trans->last_group == NULL)) {
		ret = -3;
		goto trans_progress_exit;
	}

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}

	if (trans->cur_group == NULL)
		trans->next_group = NULL;
	else if (trans->next_group == NULL)
		trans->next_group = trans->cur_group;

	chan_print_dbg(0, "channel(0x%x) ---> transfer(0x%x) [current group(0x%x), next group(0x%x)]",
			chan, trans, trans->cur_group, trans->next_group);
	
	if (!budget || (trans->next_group == NULL))
		goto progress_done;

	chan_print_dbg(0, "transfer(0x%x) - number of group to handling = 0x%x, budget = 0x%x",
			trans, trans->group_wait, budget);

	do {
		group = trans->next_group;

		if (group->group_done)
			break;

		desc_flags = group->desc_pool_addr;
		desc_flags += (group->desc_pool_size - sizeof(uint32_t));
			
		if (((size_t)desc_flags) & 0x3)
			MDMA_CHAN_BUG("channel(0x%x) - unaligned address = 0x%x", chan, desc_flags);

		val = mdma_chan_read32((size_t)desc_flags);

		if (val & MDMA_DESC_OWNERSHIP)
			group->group_done = true;
		else
			break;

		trans->group_wait++;
		budget--;

		if (group != trans->last_group)
			trans->next_group = head_to_mdma_group(group->group_node.next);
		else if (trans->trans_type == MDMA_TRANS_RING)
			trans->next_group = trans->first_group;

		chan_print_dbg(0, "transfer(0x%x) ---> next group(0x%x)", trans, trans->next_group);
	} while (budget);

	chan_print_dbg(0, "transfer(0x%x) - number of group to handling = 0x%x, budget = 0x%x",
			trans, trans->group_wait, budget);

progress_done:
	ret = trans->group_wait;

	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

trans_progress_exit:
	chan_print_dbg(1 , "<-- end");

	return ret;
}

int mdma_chan_get_state(struct mdma_chan *chan)
{
	uint32_t status;
	int ret;

	chan_print_dbg(1, "start -->");

	if (!chan) {
		chan_print_dbg(1, "<-- end");
		return -1;
	}

	if (chan->cfg.mode == MDMA_CHAN_POLLING) {
		status = mdma_chan_read32((size_t)(&chan->regs->status));

		if (!(chan->cfg.event_indx < 0))
			chan_check_event(chan, status);

		if (chan_is_error(chan, status)) {
			ret = MDMA_CHAN_ERROR;
			goto state_exit;
		}

		if (status & MDMA_STATUS_BAD_DESC) {
			chan->state = MDMA_CHAN_PENDING;

			ret = MDMA_CHAN_PENDING;
			goto state_exit;
		}

		if (status & MDMA_STATUS_STOP_DESC) {
			chan->state = MDMA_CHAN_DUMMY;

			ret = MDMA_CHAN_DUMMY;
			goto state_exit;
		}
		
		ret = chan->state;
	}
	else {
		RUMBOOT_ATOMIC_BLOCK() {
			ret = chan->state;
		}
	}

state_exit:
	chan_print_dbg(0, "channel(0x%x) - state = 0x%x", chan, ret);

#ifdef DEBUG_MDMA_CHAN
	mdma_chan_read32((size_t)(&chan->regs->dma_state));
	mdma_chan_read32((size_t)(&chan->regs->axi_state));
	mdma_chan_read32((size_t)(&chan->regs->available_space));
#endif

	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_irq_cb(struct mdma_chan *chan)
{
	uint32_t mask, status;
	int ret = IRQ_CHAN_NONE;

	chan_print_dbg(1, "start -->");

	mask = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
	if (!mask)
		goto cb_exit;
	
	status = mdma_chan_read32((size_t)(&chan->regs->status));

	status &= mask;
	if (!status)
		MDMA_CHAN_BUG("channel(0x%x) - unknown IRQ", chan);

	if (status & MDMA_STATUS_INT_DESC)
		ret |= IRQ_CHAN_GROUP;

	if (!(chan->cfg.event_indx < 0) && chan_check_event(chan, status))
		ret |= IRQ_CHAN_EVENT;

	if (chan_is_error(chan, status)) {
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));

		ret |= IRQ_CHAN_ERROR;
		goto cb_exit;
	}

	if (status & MDMA_STATUS_BAD_DESC) {
		chan->state = MDMA_CHAN_PENDING;

		ret |= IRQ_CHAN_DONE;
	}
	else if (status & MDMA_STATUS_STOP_DESC) {
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
		chan->state = MDMA_CHAN_DUMMY;

		ret |= IRQ_CHAN_DONE;
	}

cb_exit:
	chan_print_dbg(0, "channel(0x%x) - state = 0x%x, irq = 0x%x",
			chan, chan->state, ret);

#ifdef DEBUG_MDMA_CHAN
	mdma_chan_read32((size_t)(&chan->regs->dma_state));
	mdma_chan_read32((size_t)(&chan->regs->axi_state));
	mdma_chan_read32((size_t)(&chan->regs->available_space));
#endif

	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_start(struct mdma_chan *chan)
{
	struct mdma_cfg *cfg;
	uint32_t val;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->state != MDMA_CHAN_READY)) {
		ret = -1;
		goto chan_start_exit;
	}

	cfg = &chan->cfg;

	if (!(cfg->event_indx < 0)) {
		val = mdma_chan_read32((size_t)(&chan->regs->ignore_events));
		if (val)
			rumboot_printf("Channel(0x%x): discard ignore_events - 0x%x.\n", chan, val);

		val = mdma_chan_read32((size_t)(&chan->regs->activ_events));
		mdma_chan_write32(0, (size_t)(&chan->regs->activ_events));
		if (val)
			rumboot_printf("Channel(0x%x): discard active_events - 0x%x.\n", chan, val);
	}

	val = mdma_chan_read32((size_t)(&chan->regs->status));
	if (val)
		rumboot_printf("Channel(0x%x): discard IRQ, status - 0x%x.\n", chan, val);

	val = 0;

	if (cfg->mode == MDMA_CHAN_INTERRUPT) {
		val |= MDMA_IRQ_MASK_BAD_DESC | MDMA_IRQ_MASK_STOP_DESC;
		val |= MDMA_IRQ_MASK_DISCARD_DESC | MDMA_IRQ_MASK_WAXI_ERR;
		val |= MDMA_IRQ_MASK_AXI_ERR;
		val |= MDMA_IRQ_MASK_INT_DESC;

		if (!(cfg->event_indx < 0)) {
			val |= MDMA_IRQ_MASK_START_EVENT;
			val |= MDMA_IRQ_MASK_IGNORE_EVENT;
		}
	}

	if (!(cfg->event_indx < 0)) {
		mdma_chan_write32((1 << cfg->event_indx), (size_t)(&chan->regs->sense_list));
		chan->state = MDMA_CHAN_WAITING;
	}
	else {
		mdma_chan_write32(MDMA_ENABLE, (size_t)(&chan->regs->enable));
		chan->state = MDMA_CHAN_RUNNING;
	}

	chan_print_dbg(0, "channel(0x%x) - state = 0x%x", chan, chan->state);

	mdma_chan_write32(val, (size_t)(&chan->regs->irq_mask));

chan_start_exit:
	chan_print_dbg(1, "<-- end");
	
	return ret;
}

int mdma_chan_wait_event(struct mdma_chan *chan, int timeout)
{
	int state, ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan) {
		ret = -1;
		goto wait_event_exit;
	}

	chan_print_dbg(0, "channel(0x%x)", chan);

	state = mdma_chan_get_state(chan);

	while (state == MDMA_CHAN_WAITING) {
		if (!timeout) {
			ret = 1;
			break;
		}

		if (timeout > 0)
			timeout--;

		udelay(1);
		state = mdma_chan_get_state(chan);	
	}

wait_event_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_pause(struct mdma_chan *chan, bool suspend)
{
	uint32_t flags;
	uint32_t timeout = MDMA_CHAN_TIMEOUT;
	int state, mask;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan) {
		ret = -1;
		goto chan_pause_exit;
	}

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}

	state = mdma_chan_get_state(chan);

	if (suspend) {			
		mdma_chan_write32(MDMA_SUSPEND, (size_t)(&chan->regs->suspend));
		mask = MDMA_STATE_SUSPEND;
	}
	else {
		mdma_chan_write32(MDMA_CANCEL, (size_t)(&chan->regs->cancel));
		mask = MDMA_STATE_CANCEL;
	}

	if (chan->slave)
		mdma_chan_pause(chan->slave, suspend);

	while (mdma_chan_read32((size_t)(&chan->regs->dma_state)) & mask) {
		if (!timeout)
			MDMA_CHAN_BUG("channel(0x%x) - pause timeout", chan);
		else
			timeout--;

		udelay(1);

		if (suspend)
			break;
	}

	if (suspend) {
		if (mdma_chan_read32((size_t)(&chan->regs->enable)) & MDMA_ENABLE)
			chan->state = MDMA_CHAN_SUSPENDED;
		else
			mdma_chan_write32(0, (size_t)(&chan->regs->suspend));
	}
	else {
		if (state == MDMA_CHAN_RUNNING)
			chan->state = MDMA_CHAN_CANCELED;
	}

	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

	state = mdma_chan_get_state(chan);
	if ((state != MDMA_CHAN_SUSPENDED) && (state != MDMA_CHAN_CANCELED))
		ret = -2;

chan_pause_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_resume(struct mdma_chan *chan)
{
	uint32_t flags;
	int state, ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan) {
		ret = -1;
		goto chan_resume_exit;
	}

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}

	state = mdma_chan_get_state(chan);
	
	if (!(chan->cfg.event_indx < 0) && (state == MDMA_CHAN_PENDING)) {
		mdma_chan_write32((1 << chan->cfg.event_indx), (size_t)(&chan->regs->sense_list));
		chan->state = MDMA_CHAN_WAITING;

		goto resume_done;
	}
	
	if (state == MDMA_CHAN_SUSPENDED)
		mdma_chan_write32(0, (size_t)(&chan->regs->suspend));
	else if ((state == MDMA_CHAN_CANCELED) || (state == MDMA_CHAN_PENDING))
		mdma_chan_write32(MDMA_ENABLE, (size_t)(&chan->regs->enable));
	else {
		ret = -2;
		goto resume_done;
	}

	chan->state = MDMA_CHAN_RUNNING;
	
resume_done:
	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

chan_resume_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_wait_trans_end(struct mdma_chan *chan, int timeout)
{
	
	int state, budget;
	int wait_nmb, ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans == NULL)) {
		chan_print_dbg(1, "<-- end");
		return -1;
	}

	chan_print_dbg(0, "channel(0x%x)", chan);

	state = mdma_chan_get_state(chan);

	while (state == MDMA_CHAN_RUNNING) {
		if (!timeout) {
			ret = 1;
			break;
		}

		if (timeout > 0)
			timeout--;

		udelay(1);
		state = mdma_chan_get_state(chan);
	}

	if (timeout)
		budget = chan->trans->group_nmb;
	else
		budget = 1;

	wait_nmb = chan_trans_progress(chan, budget);

	if (wait_nmb < 0)
		ret = -2;

	chan_print_dbg(1, "<-- end");

	return ret;
}

int mdma_chan_wait_group_end(struct mdma_chan *chan, int timeout)
{
	int wait_nmb, ret = 0;

	chan_print_dbg(1, "start -->");

	if (!chan || (chan->trans == NULL)) {
		chan_print_dbg(1, "<-- end");
		return -1;
	}

	chan_print_dbg(0, "channel(0x%x)", chan);

	wait_nmb = chan_trans_progress(chan, 1);

	while(!wait_nmb) {
		if (!timeout) {
			ret = 1;
			break;
		}

		if (timeout > 0)
			timeout--;

		udelay(1);
		wait_nmb = chan_trans_progress(chan, 1);
	}

	if (wait_nmb < 0)
		ret = -2;

	chan_print_dbg(1, "<-- end");

	return ret;
}

struct mdma_trans *mdma_chan_get_trans(struct mdma_chan *chan)
{
	if (!chan)
		return NULL;

	chan_print_dbg(1, "channel(0x%x) ---> transfer(0x%x)", chan, chan->trans);

	return chan->trans;
}

int mdma_trans_ignore_event(struct mdma_trans *trans)
{
	struct mdma_chan *chan;
	uint32_t flags;
	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!trans || (trans->chan == NULL)) {
		chan_print_dbg(1, "<-- end");
		return -1;
	}

	chan = trans->chan;

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}
	
	if (trans->ignore_event) {
		trans->ignore_event = false;
		chan_print_dbg(0, "transfer(0x%x)- ignore event", trans);

		ret = 1;
	}

	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

	chan_print_dbg(1, "<-- end");

	return ret;
}

struct mdma_group *mdma_trans_get_group(struct mdma_trans *trans)
{
	struct mdma_chan *chan;
	struct mdma_group *group;
	uint32_t flags;
	int wait_nmb;

	chan_print_dbg(1, "start -->");
	
	if (!trans || (trans->chan == NULL)) {
		chan_print_dbg(1, "<-- end");
		return NULL;
	}

	chan = trans->chan;

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}
	
	wait_nmb = chan_trans_progress(chan, 1);
	if (!(wait_nmb > 0)) {
		group = NULL;
		goto get_group_exit;
	}
		
	group = trans->cur_group;
	if (!group)
		MDMA_CHAN_BUG("transfer(0x%x) - NULL pointer", trans);

	chan_print_dbg(0, "transfer(0x%x) ---> group(0x%x)", trans, group);

	if (group != trans->last_group)
		trans->cur_group = head_to_mdma_group(group->group_node.next);
	else if (trans->trans_type == MDMA_TRANS_RING)
		trans->cur_group = trans->first_group;
	else
		trans->cur_group = NULL;

	trans->group_wait--;

get_group_exit:
	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

	chan_print_dbg(1, "<-- end");

	return group;
}

int mdma_trans_rest_group(struct mdma_group *group)
{
	struct mdma_chan *chan;
	struct mdma_cfg *cfg;
	struct mdma_node *node;
	uint32_t flags;
	int i;

	chan_print_dbg(1, "start -->");

	if (!group || (group->trans == NULL)) {
		chan_print_dbg(1, "<-- end");
		return -1;
	}

	chan = group->trans->chan;

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}

	cfg = &chan->cfg;
	
	for (i = group->node_nmb; i > 0; i--) {
		node = &group->node_group[i - 1];
		
		chan_set_desc(node, cfg);
	}

	chan_print_dbg(0, "transfer(0x%x) ---> group(0x%x)", group->trans, group);
	
	group->cur_indx = 0;
	group->group_done = false;

	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

	chan_print_dbg(1, "<-- end");

	return 0;
}

struct mdma_node *mdma_group_get_node(struct mdma_group *group)
{
	struct mdma_chan *chan;
	struct mdma_node *node;
	uint32_t flags;

	chan_print_dbg(1, "start -->");

	if (!group || (group->trans == NULL)) {
		chan_print_dbg(1, "<-- end");
		return NULL;
	}

	chan = group->trans->chan;

	chan_print_dbg(0, "channel(0x%x)", chan);

	RUMBOOT_ATOMIC_BLOCK() {
		flags = mdma_chan_read32((size_t)(&chan->regs->irq_mask));
		mdma_chan_write32(0, (size_t)(&chan->regs->irq_mask));
	}
	
	if (group->cur_indx < 0) {
		node = NULL;
		goto get_node_exit;
	}
	
	node = &group->node_group[group->cur_indx];

	chan_print_dbg(0, "group(0x%x) ---> node(0x%x), index = 0x%x", group, node, group->cur_indx);

	group->cur_indx++;
	if (!(group->cur_indx < group->node_nmb))
		group->cur_indx = -1;

get_node_exit:
	RUMBOOT_ATOMIC_BLOCK() {
		mdma_chan_write32(flags, (size_t)(&chan->regs->irq_mask));
	}

	chan_print_dbg(1, "<-- end");

	return node;
}

int mdma_node_rebase(struct mdma_node *node, struct mdma_cfg *cfg,
			void *mem_addr, size_t mem_size)
{

	int ret = 0;

	chan_print_dbg(1, "start -->");

	if (!node || (node->desc_type != DESC_DATA)) {
		ret = -1;
		goto node_rebase_exit;
	}

	if (!mem_addr || !mem_size) {
		ret = -2;
		goto node_rebase_exit;
	}

	if (((size_t)mem_addr) & (cfg->burst_width - 1)) {
		ret = -4;
		goto node_rebase_exit;
	}

	if (!cfg->addr_inc && (mem_size & (cfg->burst_width - 1))) {
		ret = -5;
		goto node_rebase_exit;
	}

	if (cfg->desc_kind == PITCH_DESC) {
		size_t size_tmp;
		int num_str;
		
		num_str = mem_size / cfg->str_length;
		if (num_str < 0) {
			ret = -6;
			goto node_rebase_exit;
		}

		size_tmp = cfg->str_length * num_str;
		if (size_tmp != mem_size) {
			ret = -7;
			goto node_rebase_exit;
		}

		if (cfg->add_info && !(num_str < MDMA_LENGTH_LIMIT(0))) {
			ret = -9;
			goto node_rebase_exit;
		}

		if (!cfg->add_info && !(num_str < MDMA_LENGTH_LIMIT(1))) {
			ret = -10;
			goto node_rebase_exit;
		}

		node->mem_size = num_str;
	}
	else {
		if (cfg->add_info && !(mem_size < MDMA_LENGTH_LIMIT(0))) {
			ret = -11;
			goto node_rebase_exit;
		}

		if (!cfg->add_info && !(mem_size < MDMA_LENGTH_LIMIT(1))) {
			ret = -12;
			goto node_rebase_exit;
		}

		node->mem_size = mem_size;
	}

	node->mem_addr = mem_addr;

	chan_print_dbg(0, "node(0x%x), index = 0x%x ---> memory address(0x%x), size = 0x%x",
				node, node->desc_indx, node->mem_addr, node->mem_size);

node_rebase_exit:
	chan_print_dbg(1, "<-- end");

	return ret;
}

void *mdma_node_get_desc(struct mdma_node *node, int *size)
{
	struct mdma_group *group;
	void *desc = NULL;

	chan_print_dbg(1, "start -->");

	if (!node || (node->group == NULL))
		goto get_desc_exit;

	if (!size)
		goto get_desc_exit;

	group = node->group;

	chan_print_dbg(0, "channel(0x%x)", group->trans->chan);
	
	desc = group->desc_pool_addr + group->gap_size * node->desc_indx;
	*size = group->gap_size;

	chan_print_dbg(0, "node(0x%x) ---> descriptor(0x%x), index = 0x%x", node, desc, node->desc_indx);

get_desc_exit:
	chan_print_dbg(1, "<-- end");

	return desc;
}

uint32_t mdma_desc_flags_length(void *desc, int size)
{
	void *desc_ptr;
	uint32_t val = 0;

	chan_print_dbg(1, "start -->");

	if (!desc)
		goto flags_length_exit;

	if ((size < 8) || (size > 16))
		goto flags_length_exit;

	chan_print_dbg(0, "descriptor(0x%x)", desc);

	desc_ptr = desc + (size - sizeof(uint32_t));
	if (((size_t)desc_ptr) & 0x3)
		goto flags_length_exit;

	val = mdma_chan_read32((size_t)desc_ptr);

flags_length_exit:
	chan_print_dbg(1, "<-- end");

	return val;
}

uint32_t mdma_desc_user_data(void *desc, bool low)
{
	uint32_t val = 0;

	chan_print_dbg(1, "start -->");

	if (!desc)
		goto user_data_exit;

	chan_print_dbg(0, "descriptor(0x%x)", desc);

	if (((size_t)desc) & 0x3)
		goto user_data_exit;

	if (!low)
		val = mdma_chan_read32((size_t)(desc + sizeof(uint32_t)));
	else
		val = mdma_chan_read32((size_t)desc);

user_data_exit:
	chan_print_dbg(1, "<-- end");

	return val;
}

//NOTE: reading from some registers has shadow effects.
