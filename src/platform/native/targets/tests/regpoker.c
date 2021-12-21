#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/regpoker.h>


static uint32_t virtualregs[] = {
	0xdeadc0de,
	0x0badb00b,
	0x00ffc0de,
};


static uint64_t cv;
static uint64_t custread(uintptr_t base)
{
	return cv;
}

void custwrite(uint64_t value, uintptr_t addr)
{
	cv = value;
}

static struct regpoker_checker check_array[] = {
	{ "reg1",  REGPOKER_READ32, 0x88000, 0xdeadc0de, ~0 },
	{ "reg2",  REGPOKER_READ32, 0x88000, 0x0badb00b, ~0 },
	{ "reg3",  REGPOKER_READ32, 0x88000, 0xc0de, 0xffff },
	{ "reg3",  REGPOKER_WRITE32, 0x88000, 0, ~0 },

	{ "reg_cust8",  REGPOKER_READ8, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust16",  REGPOKER_READ16, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust32",  REGPOKER_READ32, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust64",  REGPOKER_READ64, 0x88000, 0, ~0, custread, custwrite },

	{ "reg_cust8",  REGPOKER_WRITE8, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust16",  REGPOKER_WRITE16, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust32",  REGPOKER_WRITE32, 0x88000, 0, ~0, custread, custwrite },
	{ "reg_cust64",  REGPOKER_WRITE64, 0x88000, 0, ~0, custread, custwrite },

	{ /* Sentinel */ }
};

int main()
{
	/* Fill in real addresses */
	check_array[0].base = (uint32_t) &virtualregs[0];
	check_array[1].base = (uint32_t) &virtualregs[1];
	check_array[2].base = (uint32_t) &virtualregs[2];
	check_array[3].base = (uint32_t) &virtualregs[3];
	return rumboot_regpoker_check_array(check_array, 0);
}
