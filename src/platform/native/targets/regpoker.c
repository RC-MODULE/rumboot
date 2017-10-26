#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/regpoker.h>


uint32_t virtualregs[] = {
	0xdeadc0de,
	0x0badb00b,
	0x00ffc0de,
};

struct regpoker_checker check_array[] = {
	{ "reg1",  REGPOKER_READ32, 0x88000, 0xdeadc0de, ~0 },
	{ "reg2",  REGPOKER_READ32, 0x88000, 0x0badb00b, ~0 },
	{ "reg3",  REGPOKER_READ32, 0x88000, 0xc0de, 0xffff },
	{ "reg3",  REGPOKER_WRITE32, 0x88000, 0, ~0 },
	{ /* Sentinel */ }
};

int main()
{
	/* Fill in real addresses */
	check_array[0].base = &virtualregs[0];
	check_array[1].base = &virtualregs[1];
	check_array[2].base = &virtualregs[2];
	check_array[3].base = &virtualregs[3];
	return rumboot_regpoker_check_array(check_array, 0);
}
