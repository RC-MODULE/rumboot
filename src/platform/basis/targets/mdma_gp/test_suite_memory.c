#include "test_suite.h"

static struct mdma_gp mdma_gp_dev[];

#ifdef RUMBOOT_BASIS_ENABLE_MIRROR
#define MEM_SEGMENT_MULT	4
static char *test_heap[] = {"PCIE-IM0", "PCIE-IM1", "PCIE-DDR0", "PCIE-DDR1"};
#else
#define MEM_SEGMENT_MULT	8
static char *test_heap[] = {"IM0", "IM1", "DDR0", "DDR1"};
#endif

static void *cmp_addr[2] = {NULL};
static void *all_addr[4] = {NULL};

static int gp_timer_state[4] = {TMR_0_STATE, TMR_1_STATE, TMR_2_STATE, TMR_3_STATE};
static int gp_timer_limit[4] = {TMR_0_LIMIT, TMR_1_LIMIT, TMR_2_LIMIT, TMR_3_LIMIT};

static int gp_timer_event[4] = {4, 5, 6, 7};

static int test_alloc_mem(const char *name, void *seg_addr[], int indx, int seg_size)
{
	int id, ret = 0;

	id = rumboot_malloc_heap_by_name(name);
	if (id < 0) {
		ret = -1;
		goto alloc_mem_exit;
	}

	rumboot_printf("test -  allocate memory\n");

	seg_addr[indx] = rumboot_malloc_from_heap_aligned(id, seg_size, MDMA_BURST_WIDTH8);

	if (!seg_addr[indx])
		ret = -2;

alloc_mem_exit:
	return ret;
}

static void test_free_mem(void *seg_addr[], int indx)
{
	rumboot_printf("test -  free memory\n");

	if (seg_addr[indx])
		rumboot_free(seg_addr[indx]);

	return;
}

static bool test_memory(unsigned long arg)
{
	struct mdma_gp *dev;
	int i, err;
	int timeout = MDMA_GP_TIMEOUT;
	bool ret = true;

	iowrite32(1, GLOBAL_TIMERS + SW_RST);

	while (ioread32(GLOBAL_TIMERS + SW_RST) & 1) {
		if (!timeout) {
			ret = false;
			goto speed_exit_1;
		}

		rumboot_printf("test - gp_timer reset\n");

		udelay(1);
		timeout--;
	}

	dev = &mdma_gp_dev[arg];

	if (mdma_gp_dev_init(dev, gp_timer_event[arg], 0)) {
		ret = false;
		goto speed_exit_1;
	}

	mdma_gp_mem_fill(cmp_addr[0], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM, arg, 1);
	mdma_gp_mem_fill(cmp_addr[1], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM, 0, 0);

	iowrite32(1, GLOBAL_TIMERS + ENABLE);

	for (i = 0; i < 5; i++) {
		if (i == 0)
			err = mdma_gp_dev_config(dev, cmp_addr[0], all_addr[0], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);
		else if (i == 1)
			err = mdma_gp_dev_config(dev, all_addr[0], all_addr[1], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);
		else if (i == 2)
			err = mdma_gp_dev_config(dev, all_addr[1], all_addr[2], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);
		else if (i == 3)
			err = mdma_gp_dev_config(dev, all_addr[2], all_addr[3], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);
		else
			err = mdma_gp_dev_config(dev, all_addr[3], cmp_addr[1], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);

		if (err) {
			ret = false;
			goto speed_exit_2;
		}

		if (mdma_gp_dev_start(dev)) {
			ret = false;
			goto speed_exit_3;
		}

		rumboot_printf("test - wait event\n");

		iowrite32(1, GLOBAL_TIMERS + gp_timer_limit[arg]);
		iowrite32(0x10002, GLOBAL_TIMERS + gp_timer_state[arg]);

		timeout = MDMA_GP_TIMEOUT;

		while (dev->done == false) {
			if (!timeout) {
				ret = false;
				goto speed_exit_3;
			}

			rumboot_printf("test - wait completion\n");

			timeout--;
			udelay(1);
		}

		ret = mdma_gp_dev_check(dev);
		if (ret == false)
			goto speed_exit_3;

		mdma_gp_dev_terminate(dev);
	}

	ret = mdma_gp_mem_check(cmp_addr[0], cmp_addr[1], MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM);

speed_exit_3:
	mdma_gp_dev_terminate(dev);

speed_exit_2:
	mdma_gp_dev_destroy(dev);

speed_exit_1:
	return ret;
}

TEST_SUITE_BEGIN(mdma_gp_test_memory, "MDMA GP memory")
TEST_ENTRY("MDMA0", test_memory, 0),
TEST_ENTRY("MDMA1", test_memory, 1),
TEST_ENTRY("MDMA2", test_memory, 2),
TEST_ENTRY("MDMA3", test_memory, 3),
TEST_SUITE_END();

int main()
{
	struct mdma_gp *dev;
	struct rumboot_irq_entry *tbl;
	int i, timeout = MDMA_GP_TIMEOUT;
	int err, ret = 0;

	rumboot_irq_cli();

	rumboot_printf("Test MDMA GP: 2.1, 2.2, 2.3, 2.4\n");

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

	if (test_alloc_mem("IM1", cmp_addr, 0, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -2;
		goto test_exit_1;
	}

	if (test_alloc_mem("IM1", cmp_addr, 1, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -3;
		goto test_exit_2;
	}

	if (test_alloc_mem(test_heap[0], all_addr, 0, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -4;
		goto test_exit_3;
	}

	if (test_alloc_mem(test_heap[1], all_addr, 1, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -5;
		goto test_exit_4;
	}

	if (test_alloc_mem(test_heap[2], all_addr, 2, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -6;
		goto test_exit_5;
	}

	if (test_alloc_mem(test_heap[3], all_addr, 3, MEM_SEGMENT_MULT * MDMA_GP_SEGMENT_MEM)) {
		ret = -7;
		goto test_exit_6;
	}

	err = test_suite_run(NULL, &mdma_gp_test_memory);
	rumboot_printf("%d tests from suite failed\n", err);

	if (err)
		ret = 1;

	test_free_mem(all_addr, 3);

test_exit_6:
	test_free_mem(all_addr, 2);

test_exit_5:
	test_free_mem(all_addr, 1);

test_exit_4:
	test_free_mem(all_addr, 0);

test_exit_3:
	test_free_mem(cmp_addr, 1);

test_exit_2:
	test_free_mem(cmp_addr, 0);

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
