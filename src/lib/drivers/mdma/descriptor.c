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
}__attribute__((packed));

struct descriptor {
	struct settings *	set;
	void *			data_addr;
	struct extra *		ex;//DEFINE THIS FIELD AS NULL IF YOU WORK WITH NORMAL DSCRIPTORS!
};

#define OWNERSHIP_i 31
#define LINK_i 30
#define INTERRUPT_i 29
#define STOP_i 28
#define INCREMENT_i 27
#define ERROR_i 26
#define LENGTH_i 0

void dump_desc(struct descriptor *cfg)
{
	struct settings *set = cfg->set;
  struct extra * ex = cfg->ex;

	rumboot_printf("Settings: %x\n", (uint32_t)set);
	rumboot_printf("ownership: %d\n", set->ownership);
	rumboot_printf("link: %d\n", set->link);
	rumboot_printf("interrupt: %d\n", set->interrupt);
	rumboot_printf("stop: %d\n", set->stop);
	rumboot_printf("increment: %d\n", set->increment);
	rumboot_printf("error: %d\n", set->er);
  rumboot_printf("length: %d\n", set->length);

	rumboot_printf("data address: %x\n", cfg->data_addr);
	rumboot_printf("string length: %x\n", ex->string_length);
  rumboot_printf("pitch: %x\n\n", ex->pitch);
}

int mdma_set_desc(bool interrupt, bool stop, uint32_t data_len, uint32_t data_addr, struct extra* ex
  , uint32_t desc_addr)
{
	rumboot_printf("Set descriptor, addr: %x.\n", desc_addr);

	uint32_t settings = 0 << OWNERSHIP_i | 0 << LINK_i |
			    interrupt << INTERRUPT_i | stop << STOP_i |
			    0 << INCREMENT_i | data_len << LENGTH_i | 0 << ERROR_i;

	if (ex != NULL) {
      //What should i write here?
    iowrite32(ex->reserve, desc_addr);
    desc_addr +=4;
    iowrite16(ex->string_length, desc_addr);
    desc_addr += 2;
    iowrite16(ex->pitch, desc_addr);
    desc_addr += 2;
	}
	iowrite32(data_addr, desc_addr);
	desc_addr += 4;
	iowrite32(settings, desc_addr);

	return 0;
}

struct descriptor mdma_get_desc(uint32_t desc_addr, enum DESC_TYPE type)
{
	struct descriptor desc;

	rumboot_printf("Get descriptor, addr: %x\n", desc_addr);

	if (type == LONG || type == PITCH) {
		desc.ex->reserve = ioread32(desc_addr);
    desc_addr += 4;
    desc.ex->string_length = ioread16(desc_addr);
		desc_addr += 2;
    desc.ex->pitch = ioread16(desc_addr);
    desc_addr += 2;
	}

	desc.data_addr = (void *)ioread32(desc_addr);
	desc_addr += 4;

	uint32_t settings = ioread32(desc_addr);

	desc.set = NULL;

	desc.set->ownership = (settings & (1 << OWNERSHIP_i)) >> OWNERSHIP_i;
	desc.set->link = (settings & (1 << LINK_i)) >> LINK_i;
	desc.set->interrupt = (settings | (1 << INTERRUPT_i)) >> INTERRUPT_i;
	desc.set->stop = (settings & (1 << STOP_i)) >> STOP_i;
	desc.set->increment = (settings | (1 << INCREMENT_i)) >> INCREMENT_i;
	desc.set->er = (settings & (1 << ERROR_i)) >> ERROR_i;
  desc.set->length = (settings & (1 << LENGTH_i)) >> LENGTH_i;

  //dump_desc(&desc);

	return desc;
}
