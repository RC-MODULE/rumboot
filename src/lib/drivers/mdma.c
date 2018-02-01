#include <stdint.h>
#include <stddef.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <regs/regs_mdma.h>
#include <devices/mdma.h>

#if 0
static void dump_desc(struct desc_cfg *cfg)
{
	struct settings *set = cfg->set;

	rumboot_printf("Settings: %x\n", (uint32_t) set);
	rumboot_printf("ownership: %d\n", set->ownership);
	rumboot_printf("link: %d\n", set->link);
	rumboot_printf("interrupt: %d\n", set->interrupt);
	rumboot_printf("stop: %d\n", set->stop);
	rumboot_printf("increment: %d\n", set->increment);
	rumboot_printf("oer: %d\n", set->oer);
	rumboot_printf("ber: %d\n", set->ber);
	rumboot_printf("per: %d\n", set->per);
	rumboot_printf("fer: %d\n", set->fer);

	rumboot_printf("data address: %x\n", cfg->data_addr);
	rumboot_printf("free run value: %x\n\n", cfg->free_run_val);
}
#endif

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

void mdma_init(uint32_t base)
{
  iowrite32(0x1, base + MDMA_ENABLE_W);
	iowrite32(0x1, base + MDMA_ENABLE_R);
  iowrite32(ioread32(base + MDMA_IRQ_MASK_W) | (1<<STOP_DESC_i), base + MDMA_IRQ_MASK_W);
  iowrite32(ioread32(base + MDMA_IRQ_MASK_R) | (1<<STOP_DESC_i), base + MDMA_IRQ_MASK_R);
}

void mdma_set_rxtable(uint32_t base, struct table_cfg *cfg)
{
	uint32_t settings = 0x0;

	iowrite32(settings | ((int)cfg->type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), base + MDMA_SETTINGS_R);
	iowrite32(cfg->table_addr, base + MDMA_DESC_ADDR_R);
}

void mdma_set_wxtable(uint32_t base, struct table_cfg *cfg)
{
	uint32_t settings = 0x0;

	iowrite32(settings | ((int)cfg->type << DESC_TYPE_i) | (cfg->desc_gap << DESC_GAP_i), base + MDMA_SETTINGS_W);
	iowrite32(cfg->table_addr, base + MDMA_DESC_ADDR_W);
}

#define OWNERSHIP_i 31
#define LINK_i 30
#define INTERRUPT_i 29
#define STOP_i 28
#define INCREMENT_i 27
#define OER_i 26
#define BER_i 25
#define PER_i 24
#define FER_i 23
#define LENGTH_i 13/*0*/

enum ERR_CODE mdma_set_desc(uint32_t desc_addr, struct desc_cfg *cfg)
{
	rumboot_printf("Set descriptor, addr: %x.\n", desc_addr);

	if (cfg->set->ownership == 1) return NOT_AVAILABLE;

	struct settings *sett = cfg->set;
	uint32_t settings = sett->ownership << OWNERSHIP_i | sett->link << LINK_i |
			    sett->interrupt << INTERRUPT_i | sett->stop << STOP_i |
			    sett->increment << INCREMENT_i | sett->length << LENGTH_i;
	uint32_t data_addr = cfg->data_addr;
	uint64_t free_run_val = cfg->free_run_val;

  if( cfg->free_run_val != (int) NULL) {

    iowrite64(free_run_val, desc_addr);
    desc_addr += 8;
  }
  iowrite32(data_addr, desc_addr);
  desc_addr += 4;
	iowrite32(settings, desc_addr);

  //dump_desc(cfg);

	return OK;
}

enum ERR_CODE mdma_get_desc(uint32_t desc_addr, struct desc_cfg *cfg)
{
	rumboot_printf("Get descriptor, addr: %x\n", desc_addr);

	cfg->free_run_val = ioread64(desc_addr);
	desc_addr += 8;

	cfg->data_addr = ioread32(desc_addr);
	desc_addr += 8;

	uint32_t settings = ioread32(desc_addr);
	struct settings *set = cfg->set;

	set->ownership = settings | (1 << OWNERSHIP_i);
	set->link = settings | (1 << LINK_i);
	set->interrupt = settings | (1 << INTERRUPT_i);
	set->stop = settings | (1 << STOP_i);
	set->increment = settings | (1 << INCREMENT_i);
	set->oer = settings | (1 << OER_i);
	set->per = settings | (1 << PER_i);
	set->fer = settings | (1 << FER_i);

	//dump_desc(cfg);

	return OK;
}

bool mdma_wait_r(uint32_t base, enum EVENT_TYPE event)
{
	if (ioread32(base + MDMA_STATUS_R) & (1 << event))
		return true;
	else
		return false;
}

bool mdma_wait_w(uint32_t base, enum EVENT_TYPE event)
{
	if (ioread32(base + MDMA_STATUS_W) & (1 << event))
		return true;
	else
		return false;
}
