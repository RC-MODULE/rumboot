// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

/* #include <common.h> // DDDDD */

/* Memory test
 *
 * General observations:
 * o The recommended test sequence is to test the data lines: if they are
 *   broken, nothing else will work properly.  Then test the address
 *   lines.  Finally, test the cells in the memory now that the test
 *   program knows that the address and data lines work properly.
 *   This sequence also helps isolate and identify what is faulty.
 *
 * o For the address line test, it is a good idea to use the base
 *   address of the lowest memory location, which causes a '1' bit to
 *   walk through a field of zeros on the address lines and the highest
 *   memory location, which causes a '0' bit to walk through a field of
 *   '1's on the address line.
 *
 * o Floating buses can fool memory tests if the test routine writes
 *   a value and then reads it back immediately.  The problem is, the
 *   write will charge the residual capacitance on the data bus so the
 *   bus retains its state briefely.  When the test program reads the
 *   value back immediately, the capacitance of the bus can allow it
 *   to read back what was written, even though the memory circuitry
 *   is broken.  To avoid this, the test program should write a test
 *   pattern to the target location, write a different pattern elsewhere
 *   to charge the residual capacitance in a differnt manner, then read
 *   the target location back.
 *
 * o Always read the target location EXACTLY ONCE and save it in a local
 *   variable.  The problem with reading the target location more than
 *   once is that the second and subsequent reads may work properly,
 *   resulting in a failed test that tells the poor technician that
 *   "Memory error at 00000000, wrote aaaaaaaa, read aaaaaaaa" which
 *   doesn't help him one bit and causes puzzled phone calls.  Been there,
 *   done that.
 *
 * Data line test:
 * ---------------
 * This tests data lines for shorts and opens by forcing adjacent data
 * to opposite states. Because the data lines could be routed in an
 * arbitrary manner the must ensure test patterns ensure that every case
 * is tested. By using the following series of binary patterns every
 * combination of adjacent bits is test regardless of routing.
 *
 *     ...101010101010101010101010
 *     ...110011001100110011001100
 *     ...111100001111000011110000
 *     ...111111110000000011111111
 *
 * Carrying this out, gives us six hex patterns as follows:
 *
 *     0xaaaaaaaaaaaaaaaa
 *     0xcccccccccccccccc
 *     0xf0f0f0f0f0f0f0f0
 *     0xff00ff00ff00ff00
 *     0xffff0000ffff0000
 *     0xffffffff00000000
 *
 * To test for short and opens to other signals on our boards, we
 * simply test with the 1's complemnt of the paterns as well, resulting
 * in twelve patterns total.
 *
 * After writing a test pattern. a special pattern 0x0123456789ABCDEF is
 * written to a different address in case the data lines are floating.
 * Thus, if a byte lane fails, you will see part of the special
 * pattern in that byte lane when the test runs.  For example, if the
 * xx__xxxxxxxxxxxx byte line fails, you will see aa23aaaaaaaaaaaa
 * (for the 'a' test pattern).
 *
 * Address line test:
 * ------------------
 *  This function performs a test to verify that all the address lines
 *  hooked up to the RAM work properly.  If there is an address line
 *  fault, it usually shows up as two different locations in the address
 *  map (related by the faulty address line) mapping to one physical
 *  memory storage location.  The artifact that shows up is writing to
 *  the first location "changes" the second location.
 *
 * To test all address lines, we start with the given base address and
 * xor the address with a '1' bit to flip one address line.  For each
 * test, we shift the '1' bit left to test the next address line.
 *
 * In the actual code, we start with address sizeof(ulong) since our
 * test pattern we use is a ulong and thus, if we tried to test lower
 * order address bits, it wouldn't work because our pattern would
 * overwrite itself.
 *
 * Example for a 4 bit address space with the base at 0000:
 *   0000 <- base
 *   0001 <- test 1
 *   0010 <- test 2
 *   0100 <- test 3
 *   1000 <- test 4
 * Example for a 4 bit address space with the base at 0010:
 *   0010 <- base
 *   0011 <- test 1
 *   0000 <- (below the base address, skipped)
 *   0110 <- test 2
 *   1010 <- test 3
 *
 * The test locations are successively tested to make sure that they are
 * not "mirrored" onto the base address due to a faulty address line.
 * Note that the base and each test location are related by one address
 * line flipped.  Note that the base address need not be all zeros.
 *
 * Memory tests 1-4:
 * -----------------
 * These tests verify RAM using sequential writes and reads
 * to/from RAM. There are several test cases that use different patterns to
 * verify RAM. Each test case fills a region of RAM with one pattern and
 * then reads the region back and compares its contents with the pattern.
 * The following patterns are used:
 *
 *  1a) zero pattern (0x00000000)
 *  1b) negative pattern (0xffffffff)
 *  1c) checkerboard pattern (0x55555555)
 *  1d) checkerboard pattern (0xaaaaaaaa)
 *  2)  bit-flip pattern ((1 << (offset % 32))
 *  3)  address pattern (offset)
 *  4)  address pattern (~offset)
 *
 * Being run in normal mode, the test verifies only small 4Kb
 * regions of RAM around each 1Mb boundary. For example, for 64Mb
 * RAM the following areas are verified: 0x00000000-0x00000800,
 * 0x000ff800-0x00100800, 0x001ff800-0x00200800, ..., 0x03fff800-
 * 0x04000000. If the test is run in slow-test mode, it verifies
 * the whole RAM.
 */

// #include "irq_macros.h"

/*
 * Define INJECT_*_ERRORS for testing error detection in the presence of
 * _good_ hardware.
 */
#undef  INJECT_DATA_ERRORS
#undef  INJECT_ADDRESS_ERRORS

#ifdef INJECT_DATA_ERRORS
#warning "Injecting data line errors for testing purposes"
#endif

#ifdef INJECT_ADDRESS_ERRORS
#warning "Injecting address line errors for testing purposes"
#endif

/*
 * This function performs a double word move from the data at
 * the source pointer to the location at the destination pointer.
 * This is helpful for testing memory on processors which have a 64 bit
 * wide data bus.
 *
 * On those PowerPC with FPU, use assembly and a floating point move:
 * this does a 64 bit move.
 *
 * For other processors, let the compiler generate the best code it can.
 */
static void move64(const unsigned long long *src, unsigned long long *dest)
{
	*dest = *src;
}

static int memory_post_dataline(unsigned long long * pmem, const unsigned long long * pattern,
				const unsigned long long * otherpattern, int * mem_log,
				int label)
{
	unsigned long long temp64 = 0;
	int i, err = 0;
	unsigned int hi, lo, pathi, patlo;
	int ret = 0;

	for ( i = 0; pattern[i] !=  0x0ULL; i++) {
		move64(&(pattern[i]), pmem++);
		/*
		 * Put a different pattern on the data lines: otherwise they
		 * may float long enough to read back what we wrote.
		 */
		move64(otherpattern, pmem--);
		move64(pmem, &temp64);

#ifdef INJECT_DATA_ERRORS
		temp64 ^= 0x00008000;
#endif

		if (temp64 != pattern[i]) {
			int mem_cnt, mem_size;

			ret = -1;

            rumboot_printf ("      ERROR_1:  mem_cnt=%d  mem_size=0x%08x\n", mem_cnt, mem_size);
            rumboot_printf ("        temp64=0x%08x    pattern=0x%08x\n", temp64, pattern[i]);
			// TEST_ATOMIC_BLOCK() {
				// *(mem_log + 0) += 1;

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			if (mem_cnt <= (mem_size - 6)) {
				pathi = (pattern[i]>>32) & 0xffffffff;
				patlo = pattern[i] & 0xffffffff;

				hi = (temp64>>32) & 0xffffffff;
				lo = temp64 & 0xffffffff;

				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + mem_cnt + 3) = label;
					// *(mem_log + mem_cnt + 4) = (int)pmem;
					// *(mem_log + mem_cnt + 5) = patlo;
					// *(mem_log + mem_cnt + 6) = pathi;
					// *(mem_log + mem_cnt + 7) = lo;
					// *(mem_log + mem_cnt + 8) = hi;

					// mem_cnt = mem_cnt + 6;
					// *(mem_log + 1) = mem_cnt;
				// }
			}
		}
	}
	return ret;
}

static int memory_post_addrline(unsigned long *testaddr, unsigned long *base,
				unsigned long size, int * mem_log, int label)
{
	unsigned long *target;
	unsigned long *end;
	unsigned long readfirst;
	unsigned long readsecond;
	unsigned long readthird;
	unsigned long xor;
	int ret = 0;

	end = (unsigned long *)((unsigned long)base + size);
	xor = 0;

	for(xor = sizeof(unsigned long); xor > 0; xor <<= 1) {
		target = (unsigned long *)((unsigned long)testaddr ^ xor);

		if((target >= base) && (target < end)) {
			readfirst = *target;
			*testaddr = ~readfirst;
			readsecond  = *target;

			readthird = *testaddr;

#ifdef INJECT_ADDRESS_ERRORS
			if(xor == 0x00008000) {
				readsecond = readthird;
			}
#endif
			if(readfirst != readsecond) {
				int mem_cnt, mem_size;

				ret = -1;

                rumboot_printf ("      ERROR_2\n");
                rumboot_printf ("        readfirst=0x%08x    readsecond=0x%08x\n", readfirst, readsecond);
				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + 0) += 1;

					// mem_cnt = *(mem_log + 1);
					// mem_size = *(mem_log + 2);
				// }
				
				// if (mem_cnt <= (mem_size - 7)) {
					// TEST_ATOMIC_BLOCK() {
						// *(mem_log + mem_cnt + 3) = label;
						// *(mem_log + mem_cnt + 4) = (int)testaddr;
						// *(mem_log + mem_cnt + 5) = (int)target;
						// *(mem_log + mem_cnt + 6) = ~readfirst;
						// *(mem_log + mem_cnt + 7) = readthird;
						// *(mem_log + mem_cnt + 8) = readfirst;
						// *(mem_log + mem_cnt + 9) = readsecond;

						// mem_cnt = mem_cnt + 7;
						// *(mem_log + 1) = mem_cnt;
					// }
				// }
			}
		}
	}
	return ret;
}

static int memory_post_test1(unsigned long start, unsigned long size,
				unsigned long val, int * mem_log, int label)
{
	unsigned long i;
	unsigned long *mem = (unsigned long *) start;
	unsigned long readback;
	int ret = 0;

	for (i = 0; i < size / sizeof (unsigned long); i++)
		mem[i] = val;

	for (i = 0; i < size / sizeof (unsigned long); i++) {
		readback = mem[i];

		if (readback != val) {
			int mem_cnt, mem_size;

			ret = -1;

            rumboot_printf ("      ERROR_3\n");
            rumboot_printf ("        readback=0x%08x    val=0x%08x\n", readback, val);
			// TEST_ATOMIC_BLOCK() {
				// *(mem_log + 0) += 1;

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			// if (mem_cnt <= (mem_size - 4)) {
				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + mem_cnt + 3) = label;
					// *(mem_log + mem_cnt + 4) = (int)(mem + i);
					// *(mem_log + mem_cnt + 5) = val;
					// *(mem_log + mem_cnt + 6) = readback;

					// mem_cnt = mem_cnt + 4;
					// *(mem_log + 1) = mem_cnt;
				// }
			// }
		}
	}

	return ret;
}

static int memory_post_test2(unsigned long start, unsigned long size, int * mem_log, int label)
{
	unsigned long i;
	unsigned long *mem = (unsigned long *) start;
	unsigned long readback;
	int ret = 0;

	for (i = 0; i < size / sizeof (unsigned long); i++)
		mem[i] = 1 << (i % 32);

	for (i = 0; i < size / sizeof (unsigned long); i++) {
		readback = mem[i];

		if (readback != (1 << (i % 32))) {
			int mem_cnt, mem_size;

			ret = -1;

            rumboot_printf ("      ERROR_4\n");
            rumboot_printf ("        readback=0x%08x    (1 << (i \% 32))=0x%08x\n", readback, (1 << (i % 32)));
			// TEST_ATOMIC_BLOCK() {
				// *(mem_log + 0) += 1;

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			// if (mem_cnt <= (mem_size - 4)) {
				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + mem_cnt + 3) = label;
					// *(mem_log + mem_cnt + 4) = (int)(mem + i);
					// *(mem_log + mem_cnt + 5) = 1 << (i % 32);
					// *(mem_log + mem_cnt + 6) = readback;

					// mem_cnt = mem_cnt + 4;
					// *(mem_log + 1) = mem_cnt;
				// }
			// }
		}
	}

	return ret;
}

static int memory_post_test3(unsigned long start, unsigned long size, int * mem_log, int label)
{
	unsigned long i;
	unsigned long *mem = (unsigned long *) start;
	unsigned long readback;
	int ret = 0;

	for (i = 0; i < size / sizeof (unsigned long); i++)
		mem[i] = i;

	for (i = 0; i < size / sizeof (unsigned long); i++) {
		readback = mem[i];

		if (readback != i) {
			int mem_cnt, mem_size;

			ret = -1;

            rumboot_printf ("      ERROR_5\n");
            rumboot_printf ("        readback=0x%08x    i=0x%08x    addr=0x%08x\n", readback, i, &mem[i]);
			// TEST_ATOMIC_BLOCK() {
				// *(mem_log + 0) += 1;

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			// if (mem_cnt <= (mem_size - 4)) {
				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + mem_cnt + 3) = label;
					// *(mem_log + mem_cnt + 4) = (int)(mem + i);
					// *(mem_log + mem_cnt + 5) = i;
					// *(mem_log + mem_cnt + 6) = readback;

					// mem_cnt = mem_cnt + 4;
					// *(mem_log + 1) = mem_cnt;
				// }
			// }
		}
	}

	return ret;
}

static int memory_post_test4(unsigned long start, unsigned long size, int *mem_log, int label)
{
	unsigned long i;
	unsigned long *mem = (unsigned long *)start;
	unsigned long readback;
	int ret = 0;

	for (i = 0; i < size / sizeof (unsigned long); i++)
		mem[i] = ~i;

	for (i = 0; i < size / sizeof (unsigned long); i++) {
		readback = mem[i];

		if (readback != ~i) {
			int mem_cnt, mem_size;

			ret = -1;

            rumboot_printf ("      ERROR_6\n");
            rumboot_printf ("        readback=0x%08x    ~i=0x%08x\n", readback, ~i);
			// TEST_ATOMIC_BLOCK() {
				// *(mem_log + 0) += 1;

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			// if (mem_cnt <= (mem_size - 4)) {
				// TEST_ATOMIC_BLOCK() {
					// *(mem_log + mem_cnt + 3) = label;
					// *(mem_log + mem_cnt + 4) = (int)(mem + i);
					// *(mem_log + mem_cnt + 5) = ~i;
					// *(mem_log + mem_cnt + 6) = readback;

					// mem_cnt = mem_cnt + 4;
					// *(mem_log + 1) = mem_cnt;
				// }
			// }
		}
	}

	return ret;
}

static int memory_post_test_lines(unsigned long start, unsigned long size, const unsigned long long * pattern,
					const unsigned long long * otherpattern, int * debug_log)
{
	int log_cnt;
	int ret = 0;

	ret = memory_post_dataline((unsigned long long *)start, pattern, otherpattern,
					(int *)(*(debug_log + 1)), 0xEEEEEE01);

	if (!ret) {
		ret = memory_post_addrline((unsigned long *)start, (unsigned long *)start, size,
					(int *)(*(debug_log + 1)), 0xEEEEEE02);
	}
	else {
        rumboot_printf ("    ERROR: 0xEEEEEE01\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE01;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }

		goto test_lines_exit;
	}

	if (!ret) {
		ret = memory_post_addrline((unsigned long *)(start+size-8), (unsigned long *)start, size,
					(int *)(*(debug_log + 1)), 0xEEEEEE03);
	}
	else {
        rumboot_printf ("    ERROR: 0xEEEEEE02\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE02;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }

		goto test_lines_exit;
	}

	if (ret) {
        rumboot_printf ("    ERROR: 0xEEEEEE03\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE03;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }
	}

test_lines_exit:
	return ret;
}

static int memory_post_test_patterns(unsigned long start, unsigned long size, int * debug_log)
{
	int log_cnt;
	int ret = 0;

	ret = memory_post_test1(start, size, 0x00000000,
				(int *)(*(debug_log + 1)), 0xEEEEEE04);

	if (!ret) {
		ret = memory_post_test1(start, size, 0xffffffff,
				(int *)(*(debug_log + 1)), 0xEEEEEE05);
	}
	else {
        rumboot_printf ("    ERROR: 0xEEEEEE04\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE04;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }

		goto test_patterns_exit;
	}

	if (!ret) {
		ret = memory_post_test1(start, size, 0x55555555,
				(int *)(*(debug_log + 1)), 0xEEEEEE06);
	}
	else {
        rumboot_printf ("    ERROR: 0xEEEEEE05\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE05;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }

		goto test_patterns_exit;
	}

	if (!ret) {
		ret = memory_post_test1(start, size, 0xaaaaaaaa,
				(int *)(*(debug_log + 1)), 0xEEEEEE07);
	}
	else {
        rumboot_printf ("    ERROR: 0xEEEEEE06\n");
		// TEST_ATOMIC_BLOCK() {
			// log_cnt = *(debug_log + 2);
			// *(debug_log + log_cnt + 4) = 0xEEEEEE06;
			// *(debug_log + log_cnt + 5) = start;
			// *(debug_log + log_cnt + 6) = size;
			// *(debug_log + 2) = log_cnt + 3;
		// }

		goto test_patterns_exit;
	}

	if (!ret) {
		ret = memory_post_test2(start, size, (int *)(*(debug_log + 1)),
				0xEEEEEE08);
	}
	else {
                rumboot_printf ("    ERROR: 0xEEEEEE07\n");
                // TEST_ATOMIC_BLOCK() {
                        // log_cnt = *(debug_log + 2);
                        // *(debug_log + log_cnt + 4) = 0xEEEEEE07;
                        // *(debug_log + log_cnt + 5) = start;
                        // *(debug_log + log_cnt + 6) = size;
                        // *(debug_log + 2) = log_cnt + 3;
                // }

		goto test_patterns_exit;
	}

	if (!ret) {
		ret = memory_post_test3(start, size, (int *)(*(debug_log + 1)),
				0xEEEEEE09);
	}
	else {
                rumboot_printf ("    ERROR: 0xEEEEEE08\n");
                // TEST_ATOMIC_BLOCK() {
                        // log_cnt = *(debug_log + 2);
                        // *(debug_log + log_cnt + 4) = 0xEEEEEE08;
                        // *(debug_log + log_cnt + 5) = start;
                        // *(debug_log + log_cnt + 6) = size;
                        // *(debug_log + 2) = log_cnt + 3;
                // }

		goto test_patterns_exit;
	}

	if (!ret) {
		ret = memory_post_test4(start, size, (int *)(*(debug_log + 1)),
				0xEEEEEE0A);
	}
	else {
                rumboot_printf ("    ERROR: 0xEEEEEE09\n");
                // TEST_ATOMIC_BLOCK() {
                        // log_cnt = *(debug_log + 2);
                        // *(debug_log + log_cnt + 4) = 0xEEEEEE09;
                        // *(debug_log + log_cnt + 5) = start;
                        // *(debug_log + log_cnt + 6) = size;
                        // *(debug_log + 2) = log_cnt + 3;
                // }

		goto test_patterns_exit;
	}

	if (ret) {
                rumboot_printf ("    ERROR: 0xEEEEEE0A\n");
                // TEST_ATOMIC_BLOCK() {
                        // log_cnt = *(debug_log + 2);
                        // *(debug_log + log_cnt + 4) = 0xEEEEEE0A;
                        // *(debug_log + log_cnt + 5) = start;
                        // *(debug_log + log_cnt + 6) = size;
                        // *(debug_log + 2) = log_cnt + 3;
                // }
	}

test_patterns_exit:
	return ret;
}

static int memory_post_test_regions(unsigned long start, unsigned long size, int * debug_log)
{
	unsigned long i;
	int *mem_log, mem_cnt, mem_size;
	int log_cnt, log_size, tmp;
	int ret = 0;

	for (i = 0; i < (size >> 20); i++) {
		tmp = memory_post_test_patterns(start + (i << 20), 0x800, debug_log);
		if (tmp) {
			ret = -1;

			// mem_log = (int *)(*(debug_log + 1));

			// TEST_ATOMIC_BLOCK() {
				// log_cnt = *(debug_log + 2);
				// log_size = *(debug_log + 3);

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			if ((log_cnt > (log_size - 4)) || (mem_cnt > (mem_size - 8)))
				break;
		}

		tmp = memory_post_test_patterns(start + (i << 20) + 0xff800, 0x800, debug_log);
		if (tmp) {
			ret = -1;

			// mem_log = (int *)(*(debug_log + 1));

			// TEST_ATOMIC_BLOCK() {
				// log_cnt = *(debug_log + 2);
				// log_size = *(debug_log + 3);

				// mem_cnt = *(mem_log + 1);
				// mem_size = *(mem_log + 2);
			// }

			if ((log_cnt > (log_size - 4)) || (mem_cnt > (mem_size - 8)))
				break;
		}
	}

	return ret;
}

static int memory_post_tests(unsigned long start, unsigned long size, const unsigned long long * pattern,
				const unsigned long long * otherpattern, int * debug_log)
{
	int ret = 0;

	ret = memory_post_test_lines(start, size, pattern, otherpattern, debug_log);
	if (!ret)
		ret = memory_post_test_patterns(start, size, debug_log);

	return ret;
}

int memory_regions_post_test(unsigned long vstart, unsigned long memsize, const unsigned long long * pattern,
			const unsigned long long * otherpattern, int * debug_log, int flags)
{
	int ret = 0;

	ret = memory_post_test_lines(vstart, memsize, pattern, otherpattern, debug_log);
	if (!ret)
		ret = memory_post_test_regions(vstart, memsize, debug_log);

	return ret;
}

#define POST_SLOWTEST	0x1
#define POST_NORMAL	0x0

int memory_post_test(unsigned long vstart, unsigned long memsize, const unsigned long long * pattern,
		const unsigned long long * otherpattern, int * debug_log, int flags)
{
	int ret = 0;

	if (flags & POST_SLOWTEST) {
		ret = memory_post_tests(vstart, memsize, pattern, otherpattern, debug_log);
	} else {
		ret = memory_post_test_regions(vstart, memsize, debug_log);
	}

	return ret;
}
