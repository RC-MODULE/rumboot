#include <stdint.h>
#include <stdlib.h>
#include <rumboot/memtest.h>
#include <rumboot/macros.h>
#include <rumboot/printf.h>
#include <stdbool.h>

static const uint32_t patterns[] = {
	/* The first entry has to be 0 to leave memtest with zeroed memory */
	0,
	0xffffffffUL,
	0x55555555UL,
	0xaaaaaaaaUL,
	0x11111111UL,
	0x22222222UL,
	0x44444444UL,
	0x88888888UL,
	0x33333333UL,
	0x66666666UL,
	0x99999999UL,
	0xccccccccUL,
	0x77777777UL,
	0xbbbbbbbbUL,
	0xddddddddUL,
	0xeeeeeeeeUL,
	0x7a6c7258UL, /* yeah ;-) */
};

static uint32_t bit_test(uint32_t reg, uint32_t pattern)
{
	uint32_t i;
	uint32_t mask = reg^pattern, err_bit = 0;
	if(mask != 0)
	{
		for(i=0;i<32;i++)
			if(mask&(1<<i))
					err_bit++;
	}
	return err_bit;
}


uint64_t memtest_once(uint32_t pattern, uintptr_t start_phys, uintptr_t size)
{
    uint64_t errors = 0;
	uint32_t *p, *start, *end;
	uintptr_t start_phys_aligned;
	const size_t incr = sizeof(pattern);

	start_phys_aligned = start_phys;
	start = (uint32_t *) start_phys;
	end = start + (size - (start_phys_aligned - start_phys)) / incr;

	for (p = start; p < end; p++)
		*p = pattern;

	for (p = start; p < end; p++, start_phys_aligned += incr) {
        uint32_t tmp = *p;
		if (tmp == pattern)
			continue;
        errors += bit_test(tmp, pattern);
        *p = pattern;
	}
    return errors;
}

uint64_t memtest(uintptr_t start_phys, uintptr_t size)
{
    int i;
    uint64_t errors = 0;

    for(i=0; i<ARRAY_SIZE(patterns);i++) {
        rumboot_printf("Testing memory @ %x pattern %lx\n", start_phys, patterns[i]);
        errors += memtest_once(patterns[i], start_phys, size);
    }
    return errors;
}



uint64_t memtest_check_pattern(uint32_t pattern, bool refresh, uintptr_t mem_start, size_t length)
{
	uint64_t errors;
	uint32_t buf;
	uintptr_t i;
	errors = buf = i = 0;


	for(i = mem_start; i < (mem_start + length); i+=sizeof(pattern))
	{
		uint32_t *mem_addr = (uint32_t *) i;
		buf = bit_test(*mem_addr, pattern);
		if(buf)
		{
			errors+=buf;
			if(refresh)
			{
				*mem_addr=pattern;				
			}
		}
	}
	return errors;	
}

void memtest_fill_pattern(uintptr_t mem, size_t length, uint32_t pattern)
{
	uint32_t *ptr = (uint32_t *) mem;
	while (length)
	{
		*ptr++ = pattern;				
		length -= sizeof(*ptr);
	}	
}
