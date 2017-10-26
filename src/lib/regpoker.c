#include <stdint.h>
#include <stdlib.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>

#define READCHECK(tag, sz)  \
case tag: \
{ \
	uint ## sz ## _t tmp = ioread ## sz(array->base + offset); \
	if ((tmp & array->mask) != array->expected) { \
		fails++; \
		rumboot_printf("Read register %s (0x%x): got: 0x%x expected: %x mask: 0x%x\n", \
			       array->name, array->base + offset, tmp, array->expected, array->mask \
			       ); \
	} \
	break;\
}

#define WRITECHECK(tag, sz) \
case tag: \
{ \
	int i; \
	for (i = 0; i < sz; i++) { \
		uint ## sz ## _t v, readback; \
		if (! ((1<<i) & array->mask)) \
			continue; \
		v = (1 << i); \
		iowrite ## sz(v, array->base + offset); \
		readback = ioread32(array->base + offset); \
		if ((readback & array->mask) != v) { \
			fails++; \
			rumboot_printf("Write register %s (0x%x): wrote: 0x%x got: 0x%x mask: 0x%x\n", \
				       array->name, array->base + offset, v, readback, array->mask \
				       ); \
		} \
	} \
	break; \
}


int rumboot_regpoker_check_array(struct regpoker_checker *array, uint32_t offset)
{
	int fails = 0;

	while (array->tp != REGPOKER_EOL) {
		switch (array->tp) {
			READCHECK(REGPOKER_READ8, 8);
			READCHECK(REGPOKER_READ16, 16);
			READCHECK(REGPOKER_READ32, 32);
			READCHECK(REGPOKER_READ64, 64);
			WRITECHECK(REGPOKER_WRITE8, 8);
			WRITECHECK(REGPOKER_WRITE16, 16);
			WRITECHECK(REGPOKER_WRITE32, 32);
			WRITECHECK(REGPOKER_WRITE64, 64);
		default:
			break;
		}
		array++;
	}
	return fails;
}
