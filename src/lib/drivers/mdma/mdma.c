#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <regs/regs_mdma.h>

#include <devices/mdma.h>

#include <platform/interrupts.h>


/********DESCRIPTOR ASSIGNMENTS********/
struct settings {
	uint32_t	ownership : 1;
	uint32_t	link : 1;
	uint32_t	interrupt : 1;
	uint32_t	stop : 1;
	uint32_t	increment : 1;
	uint32_t	er : 1;
	uint32_t	length : 26;
} __attribute__((packed));

struct extra {
	uint64_t	reserve : 32;
	uint64_t	string_length : 16;
	uint64_t	pitch : 16;
} __attribute__((packed));

struct descriptor {
	struct settings *	set;
	void *			data_addr;
	struct extra *		ex;
};

int mdma_set_desc(bool interrupt, bool stop, uint32_t data_len, uint32_t data_addr, struct extra* ex
  , uint32_t desc_addr);
struct descriptor mdma_get_desc(uint32_t desc_addr, enum DESC_TYPE type);
void dump_desc(struct descriptor *cfg);

/*******************MDMA DEFENITION************************/
static volatile uint32_t irqstat_r = 0;
static volatile uint32_t irqstat_w = 0;

void mdma_irq_handler(int irq, void *arg)
{
	struct mdma_device *mdma = (struct mdma_device *)arg;
	uint32_t gp_status = ioread32(mdma->base + MDMA_GP_STATUS);

	irqstat_r |= ioread32(mdma->base + MDMA_STATUS_R);
	irqstat_w |= ioread32(mdma->base + MDMA_STATUS_W);

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
struct mdma_device *mdma_create(uint32_t base, struct mdma_config *cfg)
{
	struct mdma_device *mdma;
	enum DESC_TYPE desc_type = (cfg) ? cfg->desc_type : NORMAL;
	size_t num_txdescriptors = (cfg) ? cfg->num_txdescriptors : 32;
	size_t num_rxdescriptors = (cfg) ? cfg->num_rxdescriptors : 32;
	size_t desc_size = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;
	uint32_t desc_gap = (cfg) ? cfg->desc_gap : desc_size;

	size_t dsc_txtbl_size = num_txdescriptors * (desc_gap);
	size_t dsc_rxtbl_size = num_rxdescriptors * (desc_gap);

	char *buf = malloc(sizeof(*mdma) + dsc_rxtbl_size + dsc_txtbl_size + 2*sizeof(size_t));

	mdma = (struct mdma_device *)buf;
	mdma->txtbl = (struct descriptor *)&mdma[1];
	mdma->rxtbl = (struct descriptor *)&buf[sizeof(*mdma) + dsc_txtbl_size];

	mdma->cur_txdesc_index = 0;
	mdma->cur_rxdesc_index = 0;
	mdma->base = base;
	mdma->conf = *cfg;

	return mdma;
}

void mdma_remove(struct mdma_device *dev)
{
	if (dev) {
		free(dev);
	}
}

void mdma_init(struct mdma_device *mdma)
{
	iowrite32(0x1, mdma->base + MDMA_ENABLE_W);
	iowrite32(0x1, mdma->base + MDMA_ENABLE_R);

	uint32_t irqmask = (1 << RAXI_ERR_i) | (1 << WAXI_ERR_i) | (1 << AXI_ERR_i)
				 | (1 << STOP_DESC_i);

	if (mdma->conf.irq_en) {
		iowrite32(irqmask, mdma->base + MDMA_IRQ_MASK_W);
	}

	if (mdma->conf.irq_en) {
		iowrite32(irqmask, mdma->base + MDMA_IRQ_MASK_R);
	}
}

void mdma_deinit(struct mdma_device *mdma)
{
	iowrite32(0x0, mdma->base + MDMA_ENABLE_W);
	iowrite32(0x0, mdma->base + MDMA_ENABLE_R);
}

void mdma_configure(struct mdma_device *dev, struct mdma_config *cfg)
{
	iowrite32(((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), dev->base + MDMA_SETTINGS_R);
	iowrite32((uint32_t)dev->txtbl, dev->base + MDMA_DESC_ADDR_R);
	iowrite32(((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), dev->base + MDMA_SETTINGS_W);
	iowrite32((uint32_t)dev->rxtbl, dev->base + MDMA_DESC_ADDR_W);

	// if (cfg->desc_type == NORMAL) {
	// 	iowrite32(NORMAL_DESC_SIZE, dev->base + MDMA_MAX_LEN_W);
	// } else {
	// 	iowrite32(PITCH_DESC_SIZE, dev->base + MDMA_MAX_LEN_W);
	// }
  //
	// iowrite32(cfg->num_descriptors, dev->base + MDMA_ARLEN);
	// iowrite32(cfg->num_descriptors, dev->base + MDMA_AWLEN);
}

void mdma_write_txdescriptor(struct mdma_device *mdma, void *mem, size_t len, bool is_last)
{
	uint32_t desc_txaddr = (uint32_t) mdma->txtbl +	(mdma->conf.desc_gap * mdma->cur_txdesc_index);
	bool interrupt = (mdma->conf.irq_en) ? 1 : 0;
	bool stop		= (is_last) ? 1 : 0;

	struct extra *to_extra = NULL;
	if (mdma->conf.desc_type != NORMAL) {
		to_extra = (struct extra*) desc_txaddr;
	}

	mdma_set_desc(interrupt, stop, len, (uint32_t) mem, to_extra, desc_txaddr);

	if (mdma->cur_txdesc_index != 0) {
		mdma->cur_txdesc_index++;
	}
}

void mdma_write_rxdescriptor(struct mdma_device *mdma, void *mem, size_t len, bool is_last)
{
	uint32_t desc_rxaddr = (uint32_t) mdma->rxtbl +	(mdma->conf.desc_gap * mdma->cur_rxdesc_index);
	bool interrupt = (mdma->conf.irq_en) ? 1 : 0;
	bool stop		= (is_last) ? 1 : 0;

	struct extra *to_extra = NULL;
	if (mdma->conf.desc_type != NORMAL) {
		to_extra = (struct extra*) desc_rxaddr;
	}

	mdma_set_desc(interrupt, stop, len, (uint32_t) mem, to_extra, desc_rxaddr);

	if (mdma->cur_rxdesc_index != 0) {
		mdma->cur_rxdesc_index++;
	}
}

bool mdma_is_finished(struct mdma_device *mdma)
{
	if(mdma->conf.irq_en) {

		if (!(irqstat_r & (1 << STOP_DESC_i))) {
			rumboot_printf("irqstat_r: %x\n", irqstat_r);
			rumboot_printf("irqstat_w: %x\n", irqstat_w);
			return false;
		}
	}

	uint32_t desc_rxaddr = (uint32_t)mdma->rxtbl + mdma->conf.desc_gap * mdma->cur_rxdesc_index;
	struct descriptor desc = mdma_get_desc(desc_rxaddr, mdma->conf.desc_type);

	if (desc.set->ownership != 1) {
		return false;
	}

	return true;
}

bool mdma_transmit_data(uint32_t base, void *dest, void *src, size_t len)
{
	//CONFIG DMA
	rumboot_printf("Config DMA.\n");
	uint8_t desc_type = NORMAL;
	size_t num_rxdescriptors = 1;
	size_t num_txdescriptors = 1;
	bool irq_en = false;
	uint16_t desc_gap = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;

	struct mdma_config cfg = { .desc_type = desc_type, .desc_gap = desc_gap, .irq_en = irq_en,
		 .num_rxdescriptors = num_rxdescriptors, .num_txdescriptors = num_txdescriptors };
	struct mdma_device *mdma = mdma_create(base, &cfg);
	mdma_configure(mdma, &cfg);

	//Create IRQ
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, MDMA0_IRQ, 0, mdma_irq_handler, (void *)mdma);
	rumboot_irq_enable(MDMA0_IRQ);
	rumboot_irq_table_activate(tbl);

	//Create transactions
	//We can fragment data in junks and transmit in several transactions!
	struct mdma_transaction * t = NULL;
	size_t count = num_rxdescriptors;
	while(count--) {
		rumboot_printf("Create transaction.\n");
		rumboot_printf("Destination: %x, source: %x, length: %x\n", dest, src, len);
		t = mdma_transaction_create(mdma, dest, src, len);

		//Push transaction to queue
		rumboot_printf("Queue transaction.\n");
		if (count == 0) {
			t->is_last = true; //!we have 1 transaction!
		}
		//mdma_transaction_dump(t);
		mdma_transaction_queue(t);
		t++;
	}

	//Init DMA
	rumboot_printf("Init MDMA, base addr: %x.\n", mdma->base);
	mdma_init(mdma);

	//mdma_dump(mdma->base);

	rumboot_printf("Check - if mdma is ready.\n");
	while (!mdma_is_finished(mdma)) udelay(1);

	//Remove transactions
	count = num_rxdescriptors;
	while(count--) {
		rumboot_printf("Remove transaction.\n");
		mdma_transaction_remove(t);
		t++;
	}

	mdma_remove(mdma);

	rumboot_printf("Compare arrays.\n");
	if (memcmp(src, dest, len) != 0) {
		return false;
	} else {
		return true;
	}
}
