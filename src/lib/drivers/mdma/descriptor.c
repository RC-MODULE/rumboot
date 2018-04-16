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
}__attribute__((packed));

struct descriptor {
  volatile struct extra	ex;//DEFINE THIS FIELD AS NULL IF YOU WORK WITH NORMAL DSCRIPTORS!
  volatile uint32_t data_addr;
	volatile struct settings set;
} __attribute__((packed));

#define OWNERSHIP_i 31
#define LINK_i 30
#define INTERRUPT_i 29
#define STOP_i 28
#define INCREMENT_i 27
#define ERROR_i 26
#define LENGTH_i 0

void dump_desc(volatile struct descriptor *cfg)
{
	volatile struct settings set = cfg->set;
  volatile struct extra ex = cfg->ex;

	rumboot_printf("ownership: %d\n", set.ownership);
	rumboot_printf("link: %d\n", set.link);
	rumboot_printf("interrupt: %d\n", set.interrupt);
	rumboot_printf("stop: %d\n", set.stop);
	rumboot_printf("increment: %d\n", set.increment);
	rumboot_printf("error: %d\n", set.er);
  rumboot_printf("length: %d\n", set.length);

	rumboot_printf("data address: %x\n", cfg->data_addr);
	rumboot_printf("string length: %x\n", ex.string_length);
  rumboot_printf("pitch: %x\n\n", ex.pitch);
}

int mdma_set_desc(bool interrupt, bool stop, uint32_t data_len, volatile uint32_t data_addr, enum DESC_TYPE type,
   volatile uint32_t desc_addr)
{
	rumboot_printf("Set descriptor addr: %x, pointed to %x with length: %x.\n", desc_addr, data_addr, data_len);

	volatile uint32_t settings = 0;

  settings |= (interrupt) ? (1 << INTERRUPT_i) : 0;
  settings |= (stop) ? (1 << STOP_i): 0;
  settings |= (data_len << LENGTH_i);

	if (type != NORMAL) {
      //What should i write here?
    iowrite32(0, desc_addr);
    //*((volatile uint32_t*) (desc_addr)) = ex_reserve;
    desc_addr +=4;
    iowrite16(0, desc_addr);
    //*((volatile uint16_t*) (desc_addr)) = ex_reserve;
    desc_addr += 2;
    iowrite16(0, desc_addr);
    desc_addr += 2;
	}
	iowrite32(data_addr, desc_addr); //desc_addr
	desc_addr += 4;
	iowrite32(settings, desc_addr);

	return 0;
}

  //FixMe: HACK
struct descriptor mdma_get_desc(volatile uint32_t desc_addr, enum DESC_TYPE type)
{
	volatile struct descriptor desc;

	rumboot_printf("Get descriptor, addr: %x\n", desc_addr);

	if (type != NORMAL) {
    desc.ex = *(volatile struct extra *) (desc_addr);
		desc.ex.reserve = *(volatile uint64_t *) desc_addr;
    desc_addr += 4;
    desc.ex.string_length = *(volatile uint16_t*) desc_addr;
		desc_addr += 2;
    desc.ex.pitch = *(volatile uint16_t *) desc_addr;
    desc_addr += 2;
	}

  if(type != NORMAL) {
      if(desc.ex.reserve == 0 && desc.ex.string_length == 0 && desc.ex.pitch == 0) {
        rumboot_printf("Error! Extra part of descriptor equals 0!\n");
      }
      else
        rumboot_printf("Success.\n");
  }

	desc.data_addr = *(volatile uint32_t *) (desc_addr);
	desc_addr += 4;

  desc.set =  *(volatile struct settings *) (desc_addr);
  dump_desc(&desc);

	return desc;
}
