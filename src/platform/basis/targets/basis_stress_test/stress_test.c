#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>

#include <regs/regs_gp_timers.h>

#include <regs/regs_muart.h>
#include <devices/muart.h>

#include <regs/regs_mgeth.h>
#include <devices/mgeth.h>

#include <rumboot/rumboot.h>
#include <rumboot/timer.h>
#include <rumboot/irq.h>

#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
#include <rumboot/memfill.h>
#endif

// Must be <= 4!
#define UART_SEGMENT_NUM	4
#define ETH_SEGMENT_NUM		4
#define MDMA_SEGMENT_NUM	4

// Must be multiple of 8 bytes!
#define UART_SEGMENT_SZ		64
#define ETH_SEGMENT_SZ		512
#define MDMA_SEGMENT_SZ		4096

struct test_dev {
	void *base_addr;
	int irq;

	int global_status_offs;
	int local_status_offs;
	int local_mask_offs;

	int irq_core_indx;
	int irq_read_indx;
	int irq_write_indx;

	struct mdma_cfg cfg;
	int custom;

	struct mdma_chan *chan_rd;
	struct mdma_chan *chan_wr;

	void *src_addr[4];
	void *dst_addr[4];

	void *src_mirror[4];
	void *dst_mirror[4];

	int seg_num;
	int seg_size;

	volatile bool wr_done;
	volatile bool rd_done;

	int err_flags;	
};

#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) && \
((RUMBOOT_BASIS_DMA_MEM_ACCEL < 0) || (RUMBOOT_BASIS_DMA_MEM_ACCEL > 3))
#error	"Bad index of MDMA"
#endif

enum test_dev_indx {
	UART0_INDX = 0,
	UART1_INDX,
	ETH0_INDX,
	ETH1_INDX,
	ETH2_INDX,
	ETH3_INDX,

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 0)
	MDMA0_INDX,
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) ||  (RUMBOOT_BASIS_DMA_MEM_ACCEL != 1)
	MDMA1_INDX,
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 2)
	MDMA2_INDX,
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 3)
	MDMA3_INDX,
#endif

	TEST_DEV_NUM
};

static int test_setup_uart(struct test_dev *dev, struct muart_conf *cfg,
			struct rumboot_irq_entry *tbl, int heap_desc, int heap_data);
static void test_reset_uart(struct test_dev *dev);

static int test_setup_eth(struct test_dev *dev, struct mgeth_conf *cfg,
			struct rumboot_irq_entry *tbl, int heap_desc, int heap_data);
static void test_reset_eth(struct test_dev *dev);

static int test_setup_dma(struct test_dev *dev,
			struct rumboot_irq_entry *tbl, int heap_desc, int heap_data);
static void test_reset_dma(struct test_dev *dev);

static bool test_dev_check(struct test_dev *first_dev, struct test_dev *second_dev);

#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
static void test_dma_accel_init(void);
static void test_dma_memfill32(void *addr, int size, int val, int incr);
static void test_dma_memcheck32(void *first, void *second, int size);
#endif

#define TEST_DEV_CFG(width, indx, info)	{MDMA_CHAN_INTERRUPT, LONG_DESC, 0, width, 0, 0, indx, 0, 0, MDMA_SYNC_NONE, info, true}

static struct test_dev dev_to_test[TEST_DEV_NUM] = {
	{(void *)BASIS_VIRT(UART0_BASE), IRQ_VIRT(UART0_INTR), MUART_GEN_STATUS, MUART_STATUS, MUART_MASK, 0, 1, 2,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, UART_SEGMENT_NUM,
	UART_SEGMENT_SZ, false, false, 0x1FC},

	{(void *)UART1_BASE, UART1_INTR, MUART_GEN_STATUS, MUART_STATUS, MUART_MASK, 0, 1, 2,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, UART_SEGMENT_NUM,
	UART_SEGMENT_SZ, false, false, 0x1FC},

	{(void *)BASIS_VIRT(ETH0_BASE), IRQ_VIRT(MGETH0_IRQ), MGETH_GLOBAL_STATUS, MGETH_STATUS, MGETH_IRQ_MASK, 31, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, ETH_SEGMENT_NUM,
	ETH_SEGMENT_SZ, false, false, 0x11},

	{(void *)BASIS_VIRT(ETH1_BASE), IRQ_VIRT(MGETH1_IRQ), MGETH_GLOBAL_STATUS, MGETH_STATUS, MGETH_IRQ_MASK, 31, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, ETH_SEGMENT_NUM,
	ETH_SEGMENT_SZ, false, false, 0x11},

	{(void *)ETH2_BASE, MGETH2_IRQ, MGETH_GLOBAL_STATUS, MGETH_STATUS, MGETH_IRQ_MASK, 31, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, ETH_SEGMENT_NUM,
	ETH_SEGMENT_SZ, false, false, 0x11},

	{(void *)ETH3_BASE, MGETH3_IRQ, MGETH_GLOBAL_STATUS, MGETH_STATUS, MGETH_IRQ_MASK, 31, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH4, -1, true), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, ETH_SEGMENT_NUM,
	ETH_SEGMENT_SZ, false, false, 0x11},

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 0)
	{(void *)BASIS_VIRT(MDMA0_BASE), IRQ_VIRT(MDMA0_IRQ), MDMA_GP_STATUS, ~0, ~0, -1, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH8, 4, false), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, MDMA_SEGMENT_NUM,
	MDMA_SEGMENT_SZ, false, false, 0},
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 1)
	{(void *)BASIS_VIRT(MDMA1_BASE), IRQ_VIRT(MDMA1_IRQ), MDMA_GP_STATUS, ~0, ~0, -1, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH8, 4, false), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, MDMA_SEGMENT_NUM,
	MDMA_SEGMENT_SZ, false, false, 0},
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 2)
	{(void *)MDMA2_BASE, MDMA2_IRQ, MDMA_GP_STATUS, ~0, ~0, -1, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH8, 4, false), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, MDMA_SEGMENT_NUM,
	MDMA_SEGMENT_SZ, false, false, 0},
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 3)
	{(void *)MDMA3_BASE, MDMA3_IRQ, MDMA_GP_STATUS, ~0, ~0, -1, 0, 16,
	TEST_DEV_CFG(MDMA_BURST_WIDTH8, 4, false), 0, NULL, NULL, {NULL}, {NULL}, {NULL}, {NULL}, MDMA_SEGMENT_NUM,
	MDMA_SEGMENT_SZ, false, false, 0},
#endif
};

static void test_dev_handler(int irq, void *arg)
{
	struct test_dev *dev = (struct test_dev *)arg;
	struct mdma_chan *chan;
	int global_flags;
	int ret;

	rumboot_printf("Device(0x%x): IRQ %d\n", dev->base_addr, dev->irq);

	if (dev->irq != irq)
		rumboot_platform_panic("IRQ %d mismatch\n", irq);

	global_flags = ioread32((unsigned long)dev->base_addr + dev->global_status_offs);

	if (!global_flags)
		rumboot_platform_panic("IRQ %d should not happen\n", irq);

	while (global_flags) {
		if (global_flags & (1 << dev->irq_read_indx)) {
			chan = dev->chan_rd;
			global_flags &= ~(1 << dev->irq_read_indx);
		}
		else if (global_flags & (1 << dev->irq_write_indx)) {
			chan = dev->chan_wr;
			global_flags &= ~(1 << dev->irq_write_indx);
		}
		else {
			int local_flags = 0, local_mask = 0;

			if ((dev->irq_core_indx != -1) && (global_flags & (1 << dev->irq_core_indx))) {
				local_flags = ioread32((unsigned long)dev->base_addr + dev->local_status_offs);
				local_mask = ioread32((unsigned long)dev->base_addr + dev->local_mask_offs);
			}

			local_flags &= local_mask;

			if (local_flags & dev->err_flags) {
				rumboot_platform_panic("IRQ %d: error flags - 0x%x\n", irq, local_flags);
			} else {
				rumboot_platform_panic("Unkhown IRQ %d\n", irq);
			}
		}

		ret = mdma_chan_irq_cb(chan);

		if (ret == IRQ_CHAN_NONE)
			continue;

		if (ret & IRQ_CHAN_ERROR)
			rumboot_platform_panic("Device(0x%x): channel error - 0x%x\n", dev->base_addr, chan->error);

		if (ret & IRQ_CHAN_DONE)
			(chan == dev->chan_rd) ? (dev->rd_done = true) : (dev->wr_done = true);
		else if ((ret & IRQ_CHAN_GROUP) || (ret & IRQ_CHAN_EVENT))
			rumboot_printf("Device(0x%x): test in progress\n", dev->base_addr);
		else
			rumboot_platform_panic("Unkhown reason of IRQ %d\n", irq);
	}

	return;
}

int main()
{
	struct muart_conf muart_cfg = {1, WL_8, STP1, 0, 0, RS_422, 0, 12500000, 1};
	struct mgeth_conf  mgeth_cfg = {true, SPEED_1000};

	struct rumboot_irq_entry *tbl;

	int heap_desc, heap_data, timeout = 1000;
	int i, ret = 0;

	rumboot_irq_cli();

	#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
        init_gpio_and_en_eth_phy(MGPIO0_BASE, MDIO0_BASE, AN_EN);
    #endif
	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE, AN_EN))
	{
		rumboot_printf("ERROR: SGMII initialization ERROR!\n");
		return 1;
	}

#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
	test_dma_accel_init();
#endif

	iowrite32 (1, GLOBAL_TIMERS + ENABLE);

	tbl = rumboot_irq_create(NULL);
	rumboot_irq_table_activate(tbl);

	heap_desc = rumboot_malloc_heap_by_name("IM1");
	heap_data = rumboot_malloc_heap_by_name("DDR0");

	if (test_setup_uart(&dev_to_test[UART0_INDX], &muart_cfg, tbl, heap_desc, heap_data)) {
		ret = -2;
		goto stress_test_exit_1;
	}

	if (test_setup_eth(&dev_to_test[ETH0_INDX], &mgeth_cfg, tbl, heap_desc, heap_data)) {
		ret = -3;
		goto stress_test_exit_2;
	}

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 0)
	if (test_setup_dma(&dev_to_test[MDMA0_INDX], tbl, heap_desc, heap_data)) {
		ret = -4;
		goto stress_test_exit_3;
	}
#endif

	heap_desc = rumboot_malloc_heap_by_name("IM1");
	heap_data = rumboot_malloc_heap_by_name("DDR1");

	if (test_setup_eth(&dev_to_test[ETH1_INDX], &mgeth_cfg, tbl, heap_desc, heap_data)) {
		ret = -5;
		goto stress_test_exit_4;
	}

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 1)
	if (test_setup_dma(&dev_to_test[MDMA1_INDX], tbl, heap_desc, heap_data)) {
		ret = -6;
		goto stress_test_exit_5;
	}
#endif

	heap_desc = rumboot_malloc_heap_by_name("IM1");
	heap_data = rumboot_malloc_heap_by_name("PCIE-DDR0");

	if (test_setup_uart(&dev_to_test[UART1_INDX], &muart_cfg, tbl, heap_desc, heap_data)) {
		ret = -7;
		goto stress_test_exit_6;
	}

	if (test_setup_eth(&dev_to_test[ETH2_INDX], &mgeth_cfg, tbl, heap_desc, heap_data)) {
		ret = -8;
		goto stress_test_exit_7;
	}

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 2)
	if (test_setup_dma(&dev_to_test[MDMA2_INDX], tbl, heap_desc, heap_data)) {
		ret = -9;
		goto stress_test_exit_8;
	}
#endif

	heap_desc = rumboot_malloc_heap_by_name("IM1");
	heap_data = rumboot_malloc_heap_by_name("PCIE-DDR1");

	if (test_setup_eth(&dev_to_test[ETH3_INDX], &mgeth_cfg, tbl, heap_desc, heap_data)) {
		ret = -10;
		goto stress_test_exit_9;
	}

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 3)
	if (test_setup_dma(&dev_to_test[MDMA3_INDX], tbl, heap_desc, heap_data)) {
		ret = -11;
		goto stress_test_exit_10;
	}
#endif

	for (i = 0; i < TEST_DEV_NUM; i++) {
		if (mdma_chan_start(dev_to_test[i].chan_wr)) {
			ret = -12;
			goto stress_test_exit;
		}
	}

	for (i = TEST_DEV_NUM; i > (ETH3_INDX + 1); i--) {
		if (mdma_chan_start(dev_to_test[i - 1].chan_rd)) {
			ret = -13;
			goto stress_test_exit;
		}
	}

	iowrite32(1, GLOBAL_TIMERS + TMR_0_LIMIT);
	iowrite32(0x10002, GLOBAL_TIMERS + TMR_0_STATE);

	for (i = (ETH3_INDX + 1); i > 0; i--) {
		if (mdma_chan_start(dev_to_test[i - 1].chan_rd)) {
			ret = -13;
			goto stress_test_exit;
		}
	}

	rumboot_irq_sei();

	do {
		udelay(1);

		rumboot_printf("test - wait completion\n");

		for (i = 0; i < TEST_DEV_NUM; i++) {
			if ((dev_to_test[i].rd_done == false) || (dev_to_test[i].wr_done == false))
				break;
		}

		if (i == TEST_DEV_NUM)
			break;

		timeout--;
	} while (timeout);

	if (!timeout) {
		ret = -14;
		goto stress_test_exit;
	}

	if (test_dev_check(&dev_to_test[UART0_INDX], &dev_to_test[UART1_INDX]) == false)
		ret = -15;
	else if (test_dev_check(&dev_to_test[ETH0_INDX], &dev_to_test[ETH1_INDX]) == false)
		ret = -16;
	else if (test_dev_check(&dev_to_test[ETH2_INDX], &dev_to_test[ETH3_INDX]) == false)
		ret = -17;

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 0)
	else if (test_dev_check(&dev_to_test[MDMA0_INDX], &dev_to_test[MDMA0_INDX]) == false)
		ret = -18;
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 1)
	else if (test_dev_check(&dev_to_test[MDMA1_INDX], &dev_to_test[MDMA1_INDX]) == false)
		ret = -19;
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 2)
	else if (test_dev_check(&dev_to_test[MDMA2_INDX], &dev_to_test[MDMA2_INDX]) == false)
		ret = -20;
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 3)
	else if (test_dev_check(&dev_to_test[MDMA3_INDX], &dev_to_test[MDMA3_INDX]) == false)
		ret = -21;
#endif

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 3)
stress_test_exit:
	test_reset_dma(&dev_to_test[MDMA3_INDX]);
#endif

#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) && (RUMBOOT_BASIS_DMA_MEM_ACCEL == 3)
stress_test_exit:
#else
stress_test_exit_10:
#endif
	test_reset_eth(&dev_to_test[ETH3_INDX]);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 2)
stress_test_exit_9:
	test_reset_dma(&dev_to_test[MDMA2_INDX]);
#endif

stress_test_exit_8:
	test_reset_eth(&dev_to_test[ETH2_INDX]);

stress_test_exit_7:
	test_reset_uart(&dev_to_test[UART1_INDX]);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 1)
stress_test_exit_6:
	test_reset_dma(&dev_to_test[MDMA1_INDX]);
#endif

stress_test_exit_5:
	test_reset_eth(&dev_to_test[ETH1_INDX]);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL) || (RUMBOOT_BASIS_DMA_MEM_ACCEL != 0)
stress_test_exit_4:
	test_reset_dma(&dev_to_test[MDMA0_INDX]);
#endif

stress_test_exit_3:
	test_reset_eth(&dev_to_test[ETH0_INDX]);

stress_test_exit_2:
	test_reset_uart(&dev_to_test[UART0_INDX]);

stress_test_exit_1:
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	rumboot_irq_cli();

	iowrite32(1, GLOBAL_TIMERS + SW_RST);
	udelay(1);

	return ret;	
}

static int test_dev_init(struct test_dev *dev, int rd_offs, int wr_offs, int heap_id);
static int test_dev_config(struct test_dev *dev, void *src_addr, void *dst_addr);
static void test_dev_terminate(struct test_dev *dev);
static void test_dev_destroy(struct test_dev *dev);
static void *test_addr_map(void *src_addr);

static int test_setup_uart(struct test_dev *dev, struct muart_conf *cfg,
		struct rumboot_irq_entry *tbl, int heap_desc, int heap_data)
{
	void *src_addr, *dst_addr;
	void *dst_mirror, *src_mirror;
	int buf_size;
	int ret = 0;

	muart_soft_reset((unsigned long)(dev->base_addr));
	udelay(1);

	muart_init((unsigned long)(dev->base_addr), cfg);
	iowrite32(0x1FC, (unsigned long)(dev->base_addr + MUART_MASK));
	muart_enable((unsigned long)(dev->base_addr));

	rumboot_irq_set_handler(tbl, dev->irq, 0, test_dev_handler, dev);
	rumboot_irq_enable(dev->irq);

	if (test_dev_init(dev, TX_CHANNEL_R, RX_CHANNEL_W, heap_desc)) {
		ret = -1;
		goto setup_uart_exit_1;
	}

	buf_size = dev->seg_num * dev->seg_size;

	src_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!src_addr) {
		ret = -2;
		goto setup_uart_exit_2;
	}

	dst_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!dst_addr) {
		rumboot_free(src_addr);

		ret = -3;
		goto setup_uart_exit_2;
	}

	src_mirror  = test_addr_map(src_addr);
	dst_mirror  = test_addr_map(dst_addr);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
	for (int i = 0; i < (buf_size / sizeof(unsigned long)); i++) {
		*((unsigned long *)dst_mirror + i) = 0;
		*((unsigned long *)src_mirror + i) = buf_size - i;
	}
#else
	test_dma_memfill32(dst_mirror, buf_size / sizeof(unsigned long), 0, 0);
	test_dma_memfill32(src_mirror, buf_size / sizeof(unsigned long), buf_size, -1);
#endif

	if (test_dev_config(dev, src_addr, dst_addr)) {
		ret = -4;
		goto setup_uart_exit_3;
	}

	return ret;

setup_uart_exit_3:
	rumboot_free(dst_addr);
	rumboot_free(src_addr);

setup_uart_exit_2:
	test_dev_destroy(dev);

setup_uart_exit_1:
	rumboot_irq_disable(dev->irq);

	muart_soft_reset((unsigned long)(dev->base_addr));
	udelay(1);

	return ret;
}

static void test_reset_uart(struct test_dev *dev)
{

	test_dev_terminate(dev);

	rumboot_free(dev->dst_addr[0]);
	rumboot_free(dev->src_addr[0]);

	test_dev_destroy(dev);

	rumboot_irq_disable(dev->irq);

	muart_soft_reset((unsigned long)(dev->base_addr));
	udelay(1);

	return;
}

static int test_setup_eth(struct test_dev *dev, struct mgeth_conf *cfg,
		struct rumboot_irq_entry *tbl, int heap_desc, int heap_data)
{
	void *src_addr, *dst_addr;
	void *src_mirror, *dst_mirror;
	int i, buf_size;
	int ret = 0;

	mgeth_reset((unsigned long)(dev->base_addr));
	udelay(1);

	mgeth_init((unsigned long)(dev->base_addr), cfg);
	iowrite32(0x11, (unsigned long)(dev->base_addr + MGETH_IRQ_MASK));

	rumboot_irq_set_handler(tbl, dev->irq, 0, test_dev_handler, dev);
	rumboot_irq_enable(dev->irq);

	if (test_dev_init(dev, MGETH_SEND_CH_0, MGETH_RECV_CH_0, heap_desc)) {
		ret = -1;
		goto setup_eth_exit_1;
	}

	buf_size = dev->seg_num * dev->seg_size;

	src_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!src_addr) {
		ret = -2;
		goto setup_eth_exit_2;
	}

	src_mirror  = test_addr_map(src_addr);

	for (i = 0; i < dev->seg_num; i++) {
		int tmp = dev->seg_size / sizeof(unsigned long);

		*((unsigned long *)src_mirror + i * tmp + 0) = 0xFFFFFFFF;
		*((unsigned long *)src_mirror + i * tmp + 1) = 0xFFFFFFFF;
		*((unsigned long *)src_mirror + i * tmp + 2) = 0xFFFFFFFF;
		*((unsigned long *)src_mirror + i * tmp + 3) = 0x00450008;
		*((unsigned long *)src_mirror + i * tmp + 4) = 0x0000F201;
		*((unsigned long *)src_mirror + i * tmp + 5) = 0x00400000;
		*((unsigned long *)src_mirror + i * tmp + 6) = 0xFFFF0D79;
		*((unsigned long *)src_mirror + i * tmp + 7) = 0xFFFFFFFF;
		*((unsigned long *)src_mirror + i * tmp + 8) = 0x0000FFFF;
		*((unsigned long *)src_mirror + i * tmp + 9) = 0x87654321;

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
		for (int j = 10; j < tmp; j++)
			*((unsigned long *)src_mirror + i * tmp + j) = buf_size - j;
#else
		test_dma_memfill32((void *)((unsigned long *)src_mirror + i * tmp + 10),
			tmp - 10, buf_size - 10, -1);
#endif
	}

	dst_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!dst_addr) {
		rumboot_free(src_addr);

		ret = -3;
		goto setup_eth_exit_2;
	}

	dst_mirror  = test_addr_map(dst_addr);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
	for (i = 0; i < (buf_size / sizeof(unsigned long)); i++)
		*((unsigned long *)dst_mirror + i) = 0;
#else
	test_dma_memfill32(dst_mirror, buf_size / sizeof(unsigned long), 0, 0);
#endif

	if (test_dev_config(dev, src_addr, dst_addr)) {
		ret = -4;
		goto setup_eth_exit_3;
	}

	return ret;

setup_eth_exit_3:
	rumboot_free(dst_addr);
	rumboot_free(src_addr);

setup_eth_exit_2:
	test_dev_destroy(dev);

setup_eth_exit_1:
	rumboot_irq_disable(dev->irq);

	mgeth_reset((unsigned long)(dev->base_addr));
	udelay(1);

	return ret;
}

static void test_reset_eth(struct test_dev *dev)
{
	test_dev_terminate(dev);

	rumboot_free(dev->dst_addr[0]);
	rumboot_free(dev->src_addr[0]);

	test_dev_destroy(dev);

	rumboot_irq_disable(dev->irq);

	mgeth_reset((unsigned long)(dev->base_addr));
	udelay(1);

	return;
}

static int test_setup_dma(struct test_dev *dev,
		struct rumboot_irq_entry *tbl, int heap_desc, int heap_data)
{
	void *src_addr, *dst_addr;
	void *src_mirror, *dst_mirror;
	int buf_size;
	int ret = 0;

	iowrite32(1, (unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST));
	udelay(1);

	rumboot_irq_set_handler(tbl, dev->irq, 0, test_dev_handler, dev);
	rumboot_irq_enable(dev->irq);

	if (test_dev_init(dev, MDMA_CHANNEL_R, MDMA_CHANNEL_W, heap_desc)) {
		ret = -1;
		goto setup_dma_exit_1;
	}

	buf_size = dev->seg_num * dev->seg_size;

	src_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!src_addr) {
		ret = -2;
		goto setup_dma_exit_2;
	}

	dst_addr = rumboot_malloc_from_heap_aligned(heap_data, buf_size, 8);
	if (!dst_addr) {
		rumboot_free(src_addr);

		ret = -3;
		goto setup_dma_exit_2;
	}

	src_mirror  = test_addr_map(src_addr);
	dst_mirror  = test_addr_map(dst_addr);

#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
	for (int i = 0; i < (buf_size / sizeof(unsigned long)); i++) {
		*((unsigned long *)src_mirror + i) = buf_size - i;
		*((unsigned long *)dst_mirror + i) = 0;
	}
#else
	test_dma_memfill32(dst_mirror, buf_size / sizeof(unsigned long), 0, 0);
	test_dma_memfill32(src_mirror, buf_size / sizeof(unsigned long), buf_size, -1);
#endif

	if (test_dev_config(dev, src_addr, dst_addr)) {
		ret = -4;
		goto setup_dma_exit_3;
	}

	mdma_chan_attach(dev->chan_wr, dev->chan_rd);

	return ret;

setup_dma_exit_3:
	rumboot_free(dst_addr);
	rumboot_free(src_addr);

setup_dma_exit_2:
	test_dev_destroy(dev);

setup_dma_exit_1:
	rumboot_irq_disable(dev->irq);

	iowrite32(1, (unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST));
	udelay(1);

	return ret;
}

static void test_reset_dma(struct test_dev *dev)
{
	test_dev_terminate(dev);

	rumboot_free(dev->dst_addr[0]);
	rumboot_free(dev->src_addr[0]);

	test_dev_destroy(dev);

	rumboot_irq_disable(dev->irq);

	iowrite32(1, (unsigned long)(dev->base_addr + MDMA_GP_SOFT_RST));
	udelay(1);

	return;
}

static int test_dev_init(struct test_dev *dev, int rd_offs, int wr_offs, int heap_id)
{
	int ret = 0;

	dev->cfg.heap_id = heap_id;

	dev->chan_rd = mdma_chan_create(dev->base_addr + rd_offs, &dev->cfg);
	if (dev->chan_rd) {
		ret = mdma_trans_create(dev->chan_rd, MDMA_TRANS_QUEUE);
		if (ret) {
			ret = -1;
			goto dev_init_exit_1;
		}
	}

	dev->chan_wr = mdma_chan_create(dev->base_addr + wr_offs, &dev->cfg);
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

	rumboot_printf("Device(0x%x) is init\n", dev->base_addr);
	return ret;

dev_init_exit_2:
	mdma_chan_destroy(dev->chan_wr);

dev_init_exit_1:
	mdma_chan_destroy(dev->chan_rd);

	return ret;
}

static void *test_addr_map(void *src_addr)
{
	void *dst_addr;

	if ((src_addr >= (void *)0x40000000) && (src_addr <= (void *)0x7FFFFFFF)) {
		if (src_addr >= (void*)0x60000000)
			dst_addr = src_addr - 0x60000000 + 0xC0000000;
		else if (src_addr >= (void*)0x50000000)
			dst_addr = src_addr - 0x50000000 + 0x80000000;
		else
			dst_addr = BASIS_VIRT(src_addr);
	}
	else {
		dst_addr = src_addr;
	}

	return dst_addr;
}

static int test_dev_config(struct test_dev *dev, void *src_addr, void *dst_addr)
{
	int i, ret = 0;

	dev->rd_done = false;
	dev->wr_done = false;

        for (i = 0; i < dev->seg_num; i++) {
                dev->src_addr[i] = src_addr + i * dev->seg_size;
		dev->src_mirror[i] = test_addr_map(dev->src_addr[i]);

                dev->dst_addr[i] = dst_addr + i * dev->seg_size;
		dev->dst_mirror[i] = test_addr_map(dev->dst_addr[i]);

                if (mdma_trans_add_group(dev->chan_wr, dev->dst_addr[i], dev->seg_size,
                                dev->custom, (i < (dev->seg_num - 1)) ? false : true)) {
                        ret = -1;
                        goto dev_config_exit_1;
                }

                if (mdma_trans_add_group(dev->chan_rd, dev->src_addr[i], dev->seg_size,
                                dev->custom, (i < (dev->seg_num - 1)) ? false : true)) {
                        ret = -2;
                        goto dev_config_exit_1;
                }
        }

	if (mdma_chan_config(dev->chan_wr) || mdma_chan_config(dev->chan_rd)) {
		ret = -3;
		goto dev_config_exit_1;
	}	

	rumboot_printf("Device(0x%x) is configure\n", dev->base_addr);
	return ret;

dev_config_exit_1:
	mdma_trans_free_groups(dev->chan_wr);
	mdma_trans_free_groups(dev->chan_rd);

	return ret;
}

static void test_dev_destroy(struct test_dev *dev)
{
	mdma_chan_destroy(dev->chan_wr);
	dev->chan_wr = NULL;

	mdma_chan_destroy(dev->chan_rd);
	dev->chan_rd = NULL;

	rumboot_printf("Device(0x%x) is destroy\n", dev->base_addr);

	return;
}

static void test_dev_terminate(struct test_dev *dev)
{
	mdma_trans_free_groups(dev->chan_wr);
	mdma_trans_free_groups(dev->chan_rd);

	dev->wr_done = false;
	dev->rd_done = false;

	rumboot_printf("Device(0x%x) is terminate\n", dev->base_addr);

	return;
}

static bool test_dev_check(struct test_dev *first_dev, struct test_dev *second_dev)
{
	int state_rd, state_wr;
	int i;
	bool ret = true;

	rumboot_printf("Device(0x%x) is check\n", first_dev->base_addr);

	state_rd = mdma_chan_get_state(first_dev->chan_rd);
	state_wr = mdma_chan_get_state(first_dev->chan_wr);

	if ((state_rd != MDMA_CHAN_DUMMY) || (state_wr != MDMA_CHAN_DUMMY)) {
		ret = false;
		goto dev_check_exit;
	}

	if (first_dev == second_dev)
		goto dev_check_skip;

	rumboot_printf("Device(0x%x) is check\n", second_dev->base_addr);

	state_rd = mdma_chan_get_state(second_dev->chan_rd);
	state_wr = mdma_chan_get_state(second_dev->chan_wr);

	if ((state_rd != MDMA_CHAN_DUMMY) || (state_wr != MDMA_CHAN_DUMMY)) {
		ret = false;
		goto dev_check_exit;
	}

dev_check_skip:

	for (i = 0; i < first_dev->seg_num; i++) {
#if !defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)
		for (int j = 0; j < (first_dev->seg_size / sizeof(unsigned long)); j++) {
#if 1
			if (*((unsigned long *)first_dev->dst_mirror[i] + j) != *((unsigned long *)second_dev->src_mirror[i] + j)) {
				ret = false;
				goto dev_check_exit;
			}
#else
			rumboot_printf("dst[0x%x] == 0x%x <-> src[0x%x] == 0x%x\n",
				(unsigned long *)first_dev->dst_mirror[i] + j, *((unsigned long *)first_dev->dst_mirror[i] + j),
				(unsigned long *)second_dev->src_mirror[i] + j, *((unsigned long *)second_dev->src_mirror[i] + j));
#endif
			if (first_dev == second_dev)
				continue;
#if 1
			if (*((unsigned long *)second_dev->dst_mirror[i] + j) != *((unsigned int *)first_dev->src_mirror[i] + j)) {
				ret = false;
				goto dev_check_exit;
			}
#else
			rumboot_printf("dst[0x%x] == 0x%x <-> src[0x%x] == 0x%x\n",
				(unsigned long *)second_dev->dst_mirror[i] + j, *((unsigned long *)second_dev->dst_mirror[i] + j),
				(unsigned long *)first_dev->src_mirror[i] + j, *((unsigned long *)first_dev->src_mirror[i] + j));
#endif
		}
#else

		test_dma_memcheck32(second_dev->src_mirror[i], first_dev->dst_mirror[i], first_dev->seg_size / sizeof(unsigned long));

		if (first_dev == second_dev)
			continue;

		test_dma_memcheck32(first_dev->src_mirror[i], second_dev->dst_mirror[i], first_dev->seg_size / sizeof(unsigned long));
#endif
	}

	rumboot_printf("Device(0x%x): src addr 0x%x, dst addr 0x%x; 0x%x bytes\n",
			first_dev->base_addr, first_dev->src_addr, first_dev->dst_addr,
			first_dev->seg_num * first_dev->seg_size);

	if (first_dev != second_dev) {
		rumboot_printf("Device(0x%x): src addr 0x%x, dst addr 0x%x; 0x%x bytes\n",
			second_dev->base_addr, second_dev->src_addr, second_dev->dst_addr,
			second_dev->seg_num * second_dev->seg_size);
	}

dev_check_exit:
	return ret;
}


#if defined(RUMBOOT_BASIS_DMA_MEM_ACCEL)

static void *_src_desc, *_src_addr;
static void *_dst_desc, *_dst_addr;

#if RUMBOOT_BASIS_DMA_MEM_ACCEL == 0
static unsigned long _base_addr = BASIS_VIRT(MDMA0_BASE);
#elif RUMBOOT_BASIS_DMA_MEM_ACCEL == 1
static unsigned long _base_addr = BASIS_VIRT(MDMA1_BASE);
#elif RUMBOOT_BASIS_DMA_MEM_ACCEL == 2
static unsigned long _base_addr = BASIS_VIRT(MDMA2_BASE);
#elif RUMBOOT_BASIS_DMA_MEM_ACCEL == 3
static unsigned long _base_addr = BASIS_VIRT(MDMA3_BASE);
#endif

static void test_dma_accel_init(void)
{
	unsigned long val;
	int heap_id;

	iowrite32(1, _base_addr + MDMA_GP_SOFT_RST);

	while (ioread32(_base_addr + MDMA_GP_SOFT_RST) & 1) {};

	iowrite32(0, _base_addr + MDMA_IRQ_MASK_R);
	iowrite32(0, _base_addr + MDMA_IRQ_MASK_W);

	ioread32(_base_addr + MDMA_STATUS_R);
	ioread32(_base_addr + MDMA_STATUS_W);

	val = MDMA_SETTINGS_DESC_KIND(NORMAL_DESC);
	val |= MDMA_SETTINGS_DESC_GAP(8);

	iowrite32(val, _base_addr + MDMA_SETTINGS_R);
	iowrite32(val, _base_addr + MDMA_SETTINGS_W);

	heap_id = rumboot_malloc_heap_by_name("IM1");

	_src_desc = rumboot_malloc_from_heap_aligned(heap_id, 8, 8);
	_src_addr = rumboot_malloc_from_heap_aligned(heap_id, 1024, 8);

	_dst_desc = rumboot_malloc_from_heap_aligned(heap_id, 8, 8);
	_dst_addr = rumboot_malloc_from_heap_aligned(heap_id, 1024, 8);

	return;
}

static void test_dma_copy(void *src, void *dst, int bytes)
{
	struct __normal_desc *normal_desc;
	unsigned long val = 0;

	val |= MDMA_DESC_LENGTH_EXT(bytes);
	val |= MDMA_DESC_STOP;

	normal_desc = (struct __normal_desc *)_src_desc;

	iowrite32((unsigned long)src, (unsigned long)(&normal_desc->memptr));
	iowrite32(val, (unsigned long)(&normal_desc->flags_length));
	iowrite32((unsigned long)_src_desc, _base_addr + MDMA_DESC_ADDR_R);

	normal_desc = (struct __normal_desc *)_dst_desc;

	iowrite32((unsigned long)dst, (unsigned long)(&normal_desc->memptr));
	iowrite32(val, (unsigned long)(&normal_desc->flags_length));
	iowrite32((unsigned long)_dst_desc, _base_addr + MDMA_DESC_ADDR_W);

	iowrite32(MDMA_ENABLE, _base_addr + MDMA_ENABLE_W);
	iowrite32(MDMA_ENABLE, _base_addr + MDMA_ENABLE_R);

	while (ioread32(_base_addr + MDMA_ENABLE_W) & MDMA_ENABLE) {};

	return;
}

static void test_dma_memfill32(void *addr, int size, int val, int incr)
{
	int _size;

	while (size > 0) {
		if (size > 256) {
			_size = 256;
			size -= 256;
		}
		else {
			_size = size;
			size = 0;
		}

		rumboot_memfill32(_src_addr, _size, val, incr);
		test_dma_copy(_src_addr, addr, _size * sizeof(unsigned long));

		if (size) {
			addr += _size * sizeof(unsigned long);
			val += _size * incr;
		}
	}

	return;
}

static void test_dma_memcheck32(void *first, void *second, int size)
{
	int _size;

	while (size > 0) {
		if (size > 256) {
			_size = 256;
			size -= 256;
		}
		else {
			_size = size;
			size = 0;
		}

		test_dma_copy(first, _src_addr, _size * sizeof(unsigned long));
		test_dma_copy(second, _dst_addr, _size * sizeof(unsigned long));
		rumboot_memcheck32(_src_addr, _dst_addr, _size);

		if (size) {
			first += _size * sizeof(unsigned long);
			second += _size *sizeof(unsigned long);
		}
	}

	return;
}
#endif
