#include <stdint.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <regs/regs_mdma.h>
#include <devices/mdma.h>


void init_mdma(uint32_t base)
{
	iowrite32(0x1, base + RX_ENABLE_W);
	iowrite32(0x1, base + TX_ENABLE_R);
}

void set_rdma_table(uint32_t base, uint32_t table_addr)
{
	iowrite32(table_addr, base + RX_DESC_ADDR_W);
}

void set_wdma_table(uint32_t base, uint32_t table_addr)
{
	iowrite32(table_addr, base + TX_DESC_ADDR_R);
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
#define LENGTH_i 13

enum ERR_CODE set_desc(uint32_t desc_addr, struct desc_cfg *cfg)
{
	rumboot_printf("Set descriptor.\n");

	if (cfg->set->ownership == 1) return NOT_AVAILABLE;

	struct settings *sett = cfg->set;
	uint32_t settings = 1 << OWNERSHIP_i | sett->link << LINK_i |
			    sett->interrupt << INTERRUPT_i | sett->stop << STOP_i |
			    sett->increment << INCREMENT_i | sett->length << LENGTH_i;
	uint32_t data_addr = cfg->data_addr;
	uint64_t free_run_val = cfg->free_run_val;

	iowrite32(settings, desc_addr);
	desc_addr += 4;
	iowrite32(data_addr, desc_addr);
	desc_addr += 4;
	iowrite64(free_run_val, desc_addr);

	return OK;
}

enum ERR_CODE get_desc(uint32_t desc_addr, struct desc_cfg *cfg)
{
	rumboot_printf("Get descriptor\n");

	cfg->free_run_val = ioread64(desc_addr);
	desc_addr += 8;

  cfg->data_addr = ioread32(desc_addr);
	desc_addr += 8;

  uint32_t settings = ioread32(desc_addr);
  struct settings *set = cfg->set;

	set->ownership = settings | (1 << OWNERSHIP_i);

//  if (set->ownership == 1) return NOT_AVAILABLE;

  set->link = settings | (1 << LINK_i);
	set->interrupt = settings | (1 << INTERRUPT_i);
	set->stop = settings | (1 << STOP_i);
	set->increment = settings | (1 << INCREMENT_i);
	set->oer = settings | (1 << OER_i);
	set->per = settings | (1 << PER_i);
	set->fer = settings | (1 << FER_i);

	return OK;
}

void dump_desc(struct desc_cfg *cfg)
{
	struct settings *set = cfg->set;

	rumboot_printf("Settings. \n");
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
	rumboot_printf("free run value: %x\n", cfg->free_run_val);
}
