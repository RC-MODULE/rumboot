#include <devices/mdma.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <stdlib.h>


/**
 * settings     : Structure contains descriptor settings
 * ownership   :
 * link        :
 * interrupt  :
 * stop       :
 * increment  :
 */
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

#define OWNERSHIP_i 31
#define LINK_i 30
#define INTERRUPT_i 29
#define STOP_i 28
#define INCREMENT_i 27
#define OER_i 26
#define BER_i 25
#define PER_i 24
#define FER_i 23
#define LENGTH_i 0
int mdma_set_desc(struct descriptor *desc, uint32_t desc_addr)
{
	rumboot_printf("Set descriptor, addr: %x.\n", desc_addr);

	if (desc->set->ownership == 1) return -1;

	struct settings *sett = desc->set;
	uint32_t settings = sett->ownership << OWNERSHIP_i | sett->link << LINK_i |
			    sett->interrupt << INTERRUPT_i | sett->stop << STOP_i |
			    sett->increment << INCREMENT_i | sett->length << LENGTH_i;
	uint32_t data_addr = (uint32_t)desc->data_addr;
	uint64_t free_run_val = desc->free_run_val;

	if (desc->free_run_val != (int)NULL) {
		iowrite64(free_run_val, desc_addr);
		desc_addr += 8;
	}
	iowrite32(data_addr, desc_addr);
	desc_addr += 4;
	iowrite32(settings, desc_addr);

	//dump_desc(cfg);
	return 0;
}

struct descriptor mdma_get_desc(uint32_t desc_addr)
{
  struct descriptor desc = {
    .set = NULL,
  };

	rumboot_printf("Get descriptor, addr: %x\n", desc_addr);

	desc.free_run_val = ioread64(desc_addr);
	desc_addr += 8;

	desc.data_addr = (void *)ioread32(desc_addr);
	desc_addr += 8;

	uint32_t settings = ioread32(desc_addr);
	struct settings *set = desc.set;

	set->ownership = settings | (1 << OWNERSHIP_i);
	set->link = settings | (1 << LINK_i);
	set->interrupt = settings | (1 << INTERRUPT_i);
	set->stop = settings | (1 << STOP_i);
	set->increment = settings | (1 << INCREMENT_i);
	set->oer = settings | (1 << OER_i);
	set->per = settings | (1 << PER_i);
	set->fer = settings | (1 << FER_i);

	return desc;
}

void dump_desc(struct descriptor *cfg)
{
	struct settings *set = cfg->set;

	rumboot_printf("Settings: %x\n", (uint32_t)set);
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
