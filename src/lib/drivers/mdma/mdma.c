#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <regs/regs_mdma.h>
#include <devices/mdma.h>

struct settings {
        uint32_t ownership : 1;
        uint32_t link : 1;
        uint32_t interrupt : 1;
        uint32_t stop : 1;
        uint32_t increment : 1;
        uint32_t oer : 1;
        uint32_t ber : 1;
        uint32_t per : 1;
        uint32_t fer : 1;
        uint32_t length : 14;

};

struct descriptor {
        struct settings* set;
        void* data_addr;
        uint64_t free_run_val;//DEFINE THIS FIELD AS NULL IF YOU WORK WITH NORMAL DSCRIPTORS!
};

int mdma_set_desc(struct descriptor *desc, uint32_t desc_addr);
struct descriptor mdma_get_desc(uint32_t desc_addr);
void dump_desc(struct descriptor *cfg);

void mdma_dump(uint32_t base)
{
	rumboot_printf("status r: %x\n", ioread32(base + MDMA_STATUS_R));
	rumboot_printf("status w: %x\n", ioread32(base + MDMA_STATUS_W));
	// rumboot_printf("state r: %x\n", ioread32(base + MDMA_STATE_R));
	// rumboot_printf("state w: %x\n", ioread32(base + MDMA_STATE_W));
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

	size_t num_descriptors = (cfg) ? cfg->num_descriptors : 32;
	uint32_t desc_gap = (cfg) ? cfg->desc_gap : 0;
	size_t desc_size = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;

	size_t dsc_tbl_size = num_descriptors * (desc_size + desc_gap);

	char *buf = malloc(sizeof(*mdma) + 2 * dsc_tbl_size);

	mdma = (struct mdma_device *)buf;
	mdma->rxtbl = (struct descriptor *)&mdma[1];
	mdma->txtbl = (struct descriptor *)&buf[sizeof(*mdma) + dsc_tbl_size];

	mdma->base = base;
	mdma->conf = *cfg;

	return mdma;
}

void mdma_remove(struct mdma_device *dev)
{
	if (dev) free(dev);
}

void mdma_init(uint32_t base)
{
	iowrite32(0x1, base + MDMA_ENABLE_W);
	iowrite32(0x1, base + MDMA_ENABLE_R);
	iowrite32(ioread32(base + MDMA_IRQ_MASK_W) | (1 << STOP_DESC_i), base + MDMA_IRQ_MASK_W);
	iowrite32(ioread32(base + MDMA_IRQ_MASK_R) | (1 << STOP_DESC_i), base + MDMA_IRQ_MASK_R);
}

void mdma_deinit(uint32_t base)
{
	iowrite32(0x0, base + MDMA_ENABLE_W);
	iowrite32(0x0, base + MDMA_ENABLE_R);
}

void mdma_set(struct mdma_device *dev, struct mdma_config *cfg)
{
	iowrite32(((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), dev->base + MDMA_SETTINGS_R);
	iowrite32((uint32_t)dev->rxtbl, dev->base + MDMA_DESC_ADDR_R);

	iowrite32(((int)cfg->desc_type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), dev->base + MDMA_SETTINGS_W);
	iowrite32((uint32_t) dev->txtbl, dev->base + MDMA_DESC_ADDR_W);
}

void mdma_write_descriptors(uint32_t base, void* dst, void* src, size_t len, bool is_last)
{
	struct settings def_set = {
		.ownership	= 0,
		.link		= 0,
		.interrupt	= 1,
		.increment	= 0
	};

	def_set.stop = (is_last) ? 1 : 0;
	def_set.length = len;

	struct descriptor rxdesc = { .set = &def_set, .data_addr = src, .free_run_val = (int)NULL };
	struct descriptor txdesc = { .set = &def_set, .data_addr = dst, .free_run_val = (int)NULL };

  struct descriptor* cur_desc_rxaddr = (struct descriptor*) ioread32(base + MDMA_CUR_DESC_ADDR_R);
  struct descriptor* cur_desc_txaddr = (struct descriptor*) ioread32(base + MDMA_CUR_DESC_ADDR_W);

	mdma_set_desc(&rxdesc, (uint32_t) ++cur_desc_rxaddr);
	mdma_set_desc(&txdesc, (uint32_t) ++cur_desc_txaddr);
}

bool mdma_is_finished(uint32_t base) {

  uint32_t cur_desc_txaddr = ioread32(base + MDMA_CUR_DESC_ADDR_W);

  struct descriptor desc = mdma_get_desc(cur_desc_txaddr);

  if(desc.set->ownership != 1)
    return false;

  return true;
}

bool mdma_transmit_data(uint32_t base, void* dest, void* src, size_t len)
{
	//CONFIG DMA
	rumboot_printf("Config DMA.\n");
	struct mdma_config cfg = { .desc_type = NORMAL, .desc_gap = 0, .num_descriptors = 1 };
	struct mdma_device *mdma = mdma_create(base, &cfg);
	mdma_set(mdma, &cfg);

	rumboot_printf("Create transaction.\n");
	struct mdma_transaction *t = mmdma_transaction_create(mdma, dest, src, len);

	rumboot_printf("Queue transaction.\n");
	mdma_transaction_queue(t);

	rumboot_printf("Init MDMA.\n");
	mdma_init(base);

	rumboot_printf("Get transaction state.\n");
	while (!mdma_transaction_is_finished(t));

	rumboot_printf("Compare arrays.\n");
	if (memcmp(src, dest, len) != 0)
		return false;
	else
		return true;
}

#if 0
enum EVENT_TYPE {
        DIS_DONE = 0,
        CANCEL_DONE,
        INT_DESC_DONE,
        BAD_DESC,
        STOP_DESC_DONE,
        DISCARD_DESC,
        RAXI_ERR,
        WAXI_ERR,
        AXI_ERR,
        IGNORE_EVENT
};

static bool mdma_wait_r(uint32_t base, enum EVENT_TYPE event)
{
	if (ioread32(base + MDMA_STATUS_R) & (1 << event))
		return true;
	else
		return false;
}

static bool mdma_wait_w(uint32_t base, enum EVENT_TYPE event)
{
	if (ioread32(base + MDMA_STATUS_W) & (1 << event))
		return true;
	else
		return false;
}
#endif
