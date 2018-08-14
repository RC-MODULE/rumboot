#include "test_suite.h"

static struct mdma_gp mdma_gp_dev[];

static void *all_addr[4][2];

static int gp_timer_state[4] = {TMR_0_STATE, TMR_1_STATE, TMR_2_STATE, TMR_3_STATE};
static int gp_timer_limit[4] = {TMR_0_LIMIT, TMR_1_LIMIT, TMR_2_LIMIT, TMR_3_LIMIT};

static int gp_timer_event[4] = {4, 5, 6, 7};
static int mdma_gp_event[4] = {0, 1, 2, 3};

static int test_alloc_mem(const char *name, void *seg_addr[], int seg_size)
{
	int i, id;
	int ret = 0;

	id = rumboot_malloc_heap_by_name(name);
	if (id < 0) {
		ret = -1;
		goto alloc_mem_exit;
	}

	rumboot_printf("test -  allocate memory\n");

	for (i = 0; i < 2; i++) {
		seg_addr[i] = rumboot_malloc_from_heap_aligned(id, seg_size, MDMA_BURST_WIDTH8);

		if (!seg_addr[i]) {
			ret = -2;
			break;
		}
	}

	if (i != 2) {
		for (; i > 0; i--)
			rumboot_free(seg_addr[i - 1]);
	}

#if 0
	for (i = 0; i < (seg_size / 4); i++) {
		*((unsigned long *)seg_addr[1] + i) = 0;
		*((unsigned long *)seg_addr[0] + i) = i + 1;
	}
#endif

alloc_mem_exit:
	return ret;
}

static void test_free_mem(void *seg_addr[])
{
	int i;

	rumboot_printf("test -  free memory\n");

	for (i = 0; i < 2; i++) {
		if (seg_addr[i])
			rumboot_free(seg_addr[i]);
	}

	return;
}

static bool test_event(unsigned long arg)
{
	struct mdma_gp *dev;
	int i, j, err;
	int timeout = MDMA_GP_TIMEOUT;
	bool ret = true;

	iowrite32(1, GLOBAL_TIMERS + SW_RST);

	while (ioread32(GLOBAL_TIMERS + SW_RST) & 1) {
		if (!timeout) {
			ret = false;
			goto event_exit_1;
		}

		rumboot_printf("test - gp_timer reset\n");

		udelay(1);
		timeout--;
	}

	iowrite32(1, GLOBAL_TIMERS + ENABLE);

	for (i = 0; i < 4; i++) {
		dev = &mdma_gp_dev[i];

		if (i < arg)
			j =  4 - (arg - i);
		else
			j = i - arg;

		if (i == arg)
			err = mdma_gp_dev_init(dev, gp_timer_event[arg], i);
		else 
			err = mdma_gp_dev_init(dev, mdma_gp_event[(i < arg) ? j : (j - 1)], i);

		if (err) {
			ret = false;
			goto event_exit_2;
		}

		if (i == arg)
			err = mdma_gp_dev_config(dev, all_addr[j][0], all_addr[j][1], 2 * MDMA_GP_SEGMENT_IM0);
		else if (j == 1)
			err = mdma_gp_dev_config(dev, all_addr[j][0], all_addr[j][1], 2 * MDMA_GP_SEGMENT_IM1);
		else
			err = mdma_gp_dev_config(dev, all_addr[j][0], all_addr[j][1], 2 * MDMA_GP_SEGMENT_DDR);

		if (err) {
			ret = false;
			goto event_exit_3;
		}

#if 1
		for (j = 0; j < (dev->segment_size / 4); j++) {
			*((unsigned long *)dev->dst_mirror + j) = 0;
			*((unsigned long *)dev->src_mirror + j) = arg + i + j;
		}
#endif

		if (mdma_gp_dev_start(dev)) {
			ret = false;
			goto event_exit_3;
		}
	}

	rumboot_printf("test - wait event\n");

	iowrite32(1, GLOBAL_TIMERS + gp_timer_limit[arg]);
	iowrite32(0x10002, GLOBAL_TIMERS + gp_timer_state[arg]);

	timeout = MDMA_GP_TIMEOUT;

	do {
		udelay(1);

		rumboot_printf("test - wait completion\n");

		for (i = 0; i < 4; i++) {
			dev = &mdma_gp_dev[i];

			if (dev->done == false)
				break;
		}

		if (i == 4)
			break;

		timeout--;
	} while (timeout);

	if (!timeout) {
		ret = false;
		goto event_exit_3;
	}

#if 1
	for (i = 0; i < 4; i++) {
		dev = &mdma_gp_dev[i];

		ret = mdma_gp_dev_check(dev);
		if (ret == false)
			break;
	}
#endif

event_exit_3:
	for(i = 0; i < 4; i++)
		mdma_gp_dev_terminate(&mdma_gp_dev[i]);

event_exit_2:
	for(i = 0; i < 4; i++)
		mdma_gp_dev_destroy(&mdma_gp_dev[i]);

event_exit_1:
	return ret;
}

TEST_SUITE_BEGIN(mdma_gp_test_event, "MDMA GP event")
TEST_ENTRY("MDMA0", test_event, 0),
TEST_ENTRY("MDMA1", test_event, 1),
TEST_ENTRY("MDMA2", test_event, 2),
TEST_ENTRY("MDMA3", test_event, 3),
TEST_SUITE_END();

int main()
{
	struct mdma_gp *dev;
	struct rumboot_irq_entry *tbl;
	int i, timeout = MDMA_GP_TIMEOUT;
	int err, ret = 0;

	rumboot_irq_cli();

	rumboot_printf("Test MDMA GP: 1.1, 1.2, 1.3, 1.4, 1.5\n");

	tbl = rumboot_irq_create(NULL);
	rumboot_irq_table_activate(tbl);

	for (i = 0; i < 4; i++) {
		dev = &mdma_gp_dev[i];

		iowrite32(1, (unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST));

		while (ioread32((unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST)) & 1) {
			if (!timeout) {
				ret = -1;
				goto test_exit_1;
			}

			rumboot_printf("test - mdma_gp reset\n");

			udelay(1);
			timeout--;
		}

		rumboot_printf("MDMA(0x%x), init IRQ 0x%x\n", dev->base_addr, dev->irq);

		rumboot_irq_set_handler(tbl, dev->irq, 0, mdma_gp_handler, (void*)dev);
		rumboot_irq_enable(dev->irq);
	}

	rumboot_irq_sei();

	if (test_alloc_mem("IM0", all_addr[0], 2 * MDMA_GP_SEGMENT_IM0)) {
		ret = -2;
		goto test_exit_1;
	}

	if (test_alloc_mem("IM1", all_addr[1], 2 * MDMA_GP_SEGMENT_IM1)) {
		ret = -3;
		goto test_exit_2;
	}

	if (test_alloc_mem("DDR0", all_addr[2], 2 * MDMA_GP_SEGMENT_DDR)) {
		ret = -4;
		goto test_exit_3;
	}

	if (test_alloc_mem("DDR1", all_addr[3], 2 * MDMA_GP_SEGMENT_DDR)) {
		ret = -5;
		goto test_exit_4;
	}

	err = test_suite_run(NULL, &mdma_gp_test_event);
	rumboot_printf("%d tests from suite failed\n", err);

	if (err)
		ret = 1;

	test_free_mem(all_addr[3]);

test_exit_4:
	test_free_mem(all_addr[2]);

test_exit_3:
	test_free_mem(all_addr[1]);

test_exit_2:
	test_free_mem(all_addr[0]);

test_exit_1:
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	rumboot_irq_cli();

	return ret;
}

#define MDMA_GP_INIT(base, irq)	{(void *)(base), \
	{MDMA_CHAN_INTERRUPT, NORMAL_DESC, 1, MDMA_BURST_WIDTH8, 0, 0, -1, 0, 0, MDMA_SYNC_NONE, false, true}, \
	(irq), NULL, NULL, NULL, NULL, 0, NULL, NULL, 0, 0, 0, 0, false}

static struct mdma_gp mdma_gp_dev[4] = {
	MDMA_GP_INIT(MDMA0_BASE, MDMA0_IRQ),
	MDMA_GP_INIT(MDMA1_BASE, MDMA1_IRQ),
	MDMA_GP_INIT(MDMA2_BASE, MDMA2_IRQ),
	MDMA_GP_INIT(MDMA3_BASE, MDMA3_IRQ),
};
