#ifndef RUMBOOT_TESTSUITE_H
#define RUMBOOT_TESTSUITE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <rumboot/macros.h>

#define TEST_SUITE_BEGIN(var, subsystem) \
	static const struct rumboot_test_suite var = { \
	subsystem "-", ARRAY_SIZE(subsystem)+1, { \

#define TEST_ENTRY(name, func, base) \
	{ name, ARRAY_SIZE(name), func, (uint32_t) base, NULL }

#define TEST_ENTRY_COND(name, func, base, skipfunc) \
	{ name, ARRAY_SIZE(name), func, (uint32_t) base, skipfunc }

#define TEST_SUITE_END() \
	{ /*Sentinel */ } \
	} }

struct rumboot_test {
	const char *	name;
	/* strlen takes a lot of time when modelling.
	 *	we speed things up doing it compile-time
	 */
	int 		 namelen;
	bool		(*check_func)(uint32_t baseaddr);
	uint32_t	baseaddr;
	bool        (*should_skip)(uint32_t baseaddr);
};

struct rumboot_test_suite {
		const char *name;
		int namelen;
		struct rumboot_test tests[];
};


struct rumboot_test_result {
	const char *name;
	uint32_t result;
	uint32_t ticks;
};

#define TESTSUITE_RESULTSTRUCT_MAGIC 0xfead1eaf

struct rumboot_testsuite_results {
	uint32_t magic;
	uint32_t num_tests;
	struct rumboot_test_result results[];
};


void test_suite_init(struct rumboot_testsuite_results *out);
int test_suite_run(struct rumboot_testsuite_results *out, const struct rumboot_test_suite *suite);
int test_suite_run_single(struct rumboot_testsuite_results *out,
	const struct rumboot_test *test,
	const char *subsys,
	int subsyslen);

#endif /* end of include guard: RUMBOOT_TESTSUITE_H */
