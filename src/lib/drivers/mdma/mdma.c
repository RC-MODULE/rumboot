#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <regs/regs_mdma.h>

#include <devices/mdma.h>

#include <platform/interrupts.h>


/********DESCRIPTOR ASSIGNMENTS********/
struct settings {
	uint32_t	length : 26;
	uint32_t	er : 1;
	uint32_t	increment : 1;
	uint32_t	stop : 1;
	uint32_t	interrupt : 1;
	uint32_t	link : 1;
	uint32_t	ownership : 1;
} __attribute__((packed));

struct extra {
	uint64_t	reserve : 32;
	uint64_t	string_length : 16;
	uint64_t	pitch : 16;
} __attribute__((packed));

struct descriptor {
	volatile struct extra	ex;//DEFINE THIS FIELD AS NULL IF YOU WORK WITH NORMAL DSCRIPTORS!
  volatile uint32_t data_addr;
	volatile struct settings set;
} /*__attribute__((packed))*/;

int mdma_set_desc(bool interrupt, bool stop, uint32_t data_len, volatile uint32_t data_addr, enum DESC_TYPE type
		  , volatile uint32_t desc_addr);
struct descriptor mdma_get_desc(volatile uint32_t desc_addr, enum DESC_TYPE type);
void dump_desc(volatile struct descriptor *cfg);
bool is_extra_empty(volatile struct descriptor *cfg);

/*******************MDMA DEFENITION************************/
static void debug_err(uint32_t base)
{
	rumboot_printf("raxi err addr r: %x\n", ioread32(base + MDMA_RAXI_ERR_ADDR_R));
	rumboot_printf("raxi err addr w: %x\n", ioread32(base + MDMA_RAXI_ERR_ADDR_W));
	rumboot_printf("waxi err addr r: %x\n", ioread32(base + MDMA_WAXI_ERR_ADDR_R));
	rumboot_printf("waxi err addr w: %x\n", ioread32(base + MDMA_WAXI_ERR_ADDR_W));
	rumboot_printf("waxi err addr: %x\n", ioread32(base + MDMA_WAXI_ERR_ADDR));
	rumboot_printf("raxi err addr: %x\n", ioread32(base + MDMA_RAXI_ERR_ADDR));
}

static volatile uint32_t irqstat_r = 0;
static volatile uint32_t irqstat_w = 0;

void mdma_irq_handler(int irq, void *arg)
{
	uint32_t base = *((uint32_t *)arg);
	uint32_t gp_status = ioread32(base + MDMA_GP_STATUS);

	irqstat_r |= ioread32(base + MDMA_STATUS_R);
	irqstat_w |= ioread32(base + MDMA_STATUS_W);

	if (irqstat_w & (1 << AXI_ERR_i)) {
		debug_err(base);
	}

	if (gp_status & (1 << 0)) {
		rumboot_printf("IRQ arrived from read channel, irq status %x\n", irqstat_r);
	} else if (gp_status & (1 << 16)) {
		rumboot_printf("IRQ arrived from write channel, irq status %x\n", irqstat_w);
	}
}

void mdma_dump(uint32_t base)
{
	rumboot_printf("status r: %x\n", ioread32(base + MDMA_STATUS_R));
	rumboot_printf("status w: %x\n", ioread32(base + MDMA_STATUS_W));
	rumboot_printf("settings r: %x\n", ioread32(base + MDMA_SETTINGS_R));
	rumboot_printf("settings w: %x\n", ioread32(base + MDMA_SETTINGS_W));
	rumboot_printf("desc addr r: %x\n", ioread32(base + MDMA_DESC_ADDR_R));
	rumboot_printf("desc addr w: %x\n", ioread32(base + MDMA_DESC_ADDR_W));
}

#define NORMAL_DESC_SIZE 8
#define PITCH_DESC_SIZE 16
#define LONG_DESC_SIZE 16

#define IM0_HEAP_ID 0
volatile struct mdma_device *mdma_create(uint32_t base, struct mdma_config *cfg)
{
	volatile struct mdma_device *mdma;
	enum DESC_TYPE desc_type = (cfg) ? cfg->desc_type : NORMAL;
	size_t num_txdescriptors = (cfg) ? cfg->num_txdescriptors : 32;
	size_t num_rxdescriptors = (cfg) ? cfg->num_rxdescriptors : 32;
	size_t desc_size = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;
	uint32_t desc_gap = (cfg) ? cfg->desc_gap : desc_size;

	size_t dsc_txtbl_size = num_txdescriptors * (desc_gap);
	size_t dsc_rxtbl_size = num_rxdescriptors * (desc_gap);

	volatile char *buf = rumboot_malloc_from_heap_aligned(IM0_HEAP_ID, sizeof(*mdma) + dsc_rxtbl_size + dsc_txtbl_size, 8);

	mdma = (struct mdma_device *)buf;


	mdma->txtbl = (dsc_txtbl_size == 0) ? NULL : (struct descriptor *)&mdma[1];
	rumboot_printf("Address of TX Table: %x\n", mdma->txtbl);

	mdma->rxtbl = (dsc_rxtbl_size == 0) ? NULL : (struct descriptor *)&buf[sizeof(*mdma) + dsc_txtbl_size];
	rumboot_printf("Address of RX Table: %x\n", mdma->rxtbl);

	mdma->base = base;
	mdma->conf = *cfg;

	return mdma;
}

void mdma_remove(volatile struct mdma_device *dev)
{
	if (dev) {
		rumboot_free((struct mdma_dev *)dev);
	}
}

void mdma_init(volatile struct mdma_device *mdma)
{
	if (mdma->rxtbl != NULL) {
		iowrite32(0x1, mdma->base + MDMA_ENABLE_W);
	}
	if (mdma->txtbl != NULL) {
		iowrite32(0x1, mdma->base + MDMA_ENABLE_R);
	}
}

void mdma_deinit(volatile struct mdma_device *mdma)
{
	iowrite32(0x0, mdma->base + MDMA_ENABLE_W);
	iowrite32(0x0, mdma->base + MDMA_ENABLE_R);
}

#include <platform/devices.h>
void mdma_configure(volatile struct mdma_device *dev, struct mdma_config *cfg)
{
	bool is_add_info = false;
	uint32_t settings_r = 0;
	uint32_t settings_w = 0;

	if(dev->type == MDMA_MUART || dev->type == MDMA_MGETH) {
		is_add_info = true;
		//iowrite32(0x1, GLOBAL_TIMERS);
	}

	if (dev->txtbl != NULL) {
		settings_r = ((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i);
	}
	if (dev->txtbl != NULL) {
		iowrite32((volatile uint32_t)dev->txtbl, dev->base + MDMA_DESC_ADDR_R);
	}

	if(is_add_info) settings_r |= (1 << ADD_INFO_i);
	iowrite32(settings_r, dev->base + MDMA_SETTINGS_R);

	if (dev->rxtbl != NULL) {
		settings_w = ((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i);
	}
	if (dev->rxtbl != NULL) {
		iowrite32((volatile uint32_t)dev->rxtbl, dev->base + MDMA_DESC_ADDR_W);
	}

	if(is_add_info) settings_w |= (1 << ADD_INFO_i);
	iowrite32(settings_w, dev->base + MDMA_SETTINGS_W);

	uint32_t irqmask = (1 << RAXI_ERR_i) | (1 << WAXI_ERR_i) | (1 << AXI_ERR_i)
			   | (1 << STOP_DESC_i);

	if (dev->conf.irq_en && dev->rxtbl != NULL) {

			rumboot_printf("Set IRQ mask for W channel.\n");
			iowrite32(irqmask, dev->base + MDMA_IRQ_MASK_W);
	}

	if (dev->conf.irq_en && dev->txtbl != NULL) {

			rumboot_printf("Set IRQ mask for R channel.\n");
			iowrite32(irqmask, dev->base + MDMA_IRQ_MASK_R);
	}
}

void mdma_write_txdescriptor(volatile struct mdma_device *mdma, volatile void *mem, size_t len, size_t index)
{
	volatile uint32_t desc_txaddr = (volatile uint32_t)mdma->txtbl + (mdma->conf.desc_gap * index);
	bool interrupt = (mdma->conf.irq_en) ? true : false;
	bool stop = (mdma->conf.num_txdescriptors == (index + 1)) ? true : false;

	mdma_set_desc(interrupt, stop, len, (volatile uint32_t)mem, mdma->conf.desc_type, desc_txaddr);
}

void mdma_write_rxdescriptor(volatile struct mdma_device *mdma, volatile void *mem, size_t len, size_t index)
{
	volatile uint32_t desc_rxaddr = (volatile uint32_t)mdma->rxtbl + (mdma->conf.desc_gap * index);
	bool interrupt = (mdma->conf.irq_en) ? true : false;
	bool stop = (mdma->conf.num_rxdescriptors == (index + 1)) ? true : false;

	mdma_set_desc(interrupt, stop, len, (volatile uint32_t)mem,  mdma->conf.desc_type, desc_rxaddr);
}

bool mdma_is_finished(volatile struct mdma_device *mdma)
{
	if (mdma->conf.irq_en) {
		if (!(irqstat_w & (1 << STOP_DESC_i))) {
			rumboot_printf("irqstat_r: %x\n", irqstat_r);
			rumboot_printf("irqstat_w: %x\n", irqstat_w);
			return false;
		}

		return true;
	}

	volatile uint32_t desc_rxaddr = (volatile uint32_t)mdma->rxtbl + mdma->conf.desc_gap * (mdma->conf.num_rxdescriptors - 1);
	volatile struct descriptor desc = mdma_get_desc(desc_rxaddr, mdma->conf.desc_type);

	bool is_valid_for_mdma = desc.set.ownership;
	rumboot_printf("ownership: %d\n", is_valid_for_mdma);

	if (!is_valid_for_mdma) {
		return false;
	}

	return true;
}

int mdma_transmit_data(uint32_t base, volatile void *dest, volatile void *src, size_t len)
{
	//CONFIG DMA
	rumboot_printf("Config DMA.\n");
	uint8_t desc_type = NORMAL;
	size_t num_rxdescriptors = 1;
	size_t num_txdescriptors = 1;
	bool irq_en = false;

	uint16_t desc_gap = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;
	struct mdma_config cfg = { .desc_type		= desc_type,	   .desc_gap	      = desc_gap, .irq_en = irq_en,
				   .num_rxdescriptors	= num_rxdescriptors,.num_txdescriptors = num_txdescriptors };
	volatile struct mdma_device *mdma = mdma_create(base, &cfg);
	mdma_configure(mdma, &cfg);

	//Create transactions
	//We can fragment data in junks and transmit in several transactions!
	volatile struct mdma_transaction *t = NULL;
	size_t transaction_count = 1;
	while (transaction_count--) {
		rumboot_printf("Create transaction.\n");
		rumboot_printf("Destination: %x, source: %x, length: %x\n", dest, src, len);
		t = mdma_transaction_create(mdma, dest, src, len);

		//Push transaction to queue
		rumboot_printf("Queue transaction.\n");
		//mdma_transaction_dump(t);
		mdma_transaction_queue((struct mdma_transaction *)t);
		t++;
	}

	//Init DMA
	rumboot_printf("Init MDMA, base addr: %x.\n", mdma->base);
	mdma_init(mdma);

	rumboot_printf("Check - if mdma is ready.\n");
	size_t timeout_us = 1000;
	while (!mdma_is_finished(mdma)) {
		if (!timeout_us) {
			return -1;
		}
		udelay(1);
		timeout_us--;
	}

	//Remove transactions
	transaction_count = 1;
	while (transaction_count--) {
		rumboot_printf("Remove transaction.\n");
		mdma_transaction_remove((struct mdma_transaction *)t);
		t++;
	}

	mdma_remove(mdma);

	return 0;
}
