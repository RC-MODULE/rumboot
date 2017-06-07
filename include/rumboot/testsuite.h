#ifndef RUMBOOT_TESTSUITE_H
#define RUMBOOT_TESTSUITE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <rumboot/macros.h>


/**
 *
 * \defgroup testsuite TestSuite module
 *
 * Functions and macros in this group implement a simple infrastructure for
 * implementing selftest components.
 *
 * \addtogroup testsuite
 *  @{
 */


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

/**
 * Initialize the test result structure
 * @param  out [description]
 * @return     [description]
 */
void test_suite_init(struct rumboot_testsuite_results *out);

/**
 * Run the testlist
 * If out is specified, results will be logged to this structure
 *
 * @param  out  If not NULL, test results will be appended to this structure
 * @param  list [description]
 * @return      the number of failed tests.
 */
int test_suite_run(struct rumboot_testsuite_results *out, const struct rumboot_test_suite *suite);


/**
 * Run a single test
 * @param  out       If not NULL, test results will be appended to this structure
 * @param  test      The test structure containing result
 * @param  subsys    subsystem name, will be appended to the test name
 * @param  subsyslen [description]
 * @return           [description]
 */
int test_suite_run_single(struct rumboot_testsuite_results *out,
	const struct rumboot_test *test,
	const char *subsys,
	int subsyslen);



/** @}*/

#endif /* end of include guard: RUMBOOT_TESTSUITE_H */
