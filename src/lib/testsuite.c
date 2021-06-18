#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>


void test_suite_init(struct rumboot_testsuite_results *out)
{
	if (!out)
		return;
	out->magic = TESTSUITE_RESULTSTRUCT_MAGIC;
	out->num_tests = 0;
}


int test_suite_run_single(struct rumboot_testsuite_results *out, const struct rumboot_test *test,
		const char *subsystem, int subsysnamelen)
{
	uint32_t tstart;
	uint32_t tend;

    rumboot_printf("--- Executing test: %s - %s --- \n", subsystem, test->name);

	if (test->should_skip && test->should_skip(test->baseaddr)) {
    	rumboot_printf("--- SKIPPED ---\n");
		return 1;
	}

	tstart = rumboot_platform_get_uptime();

	rumboot_malloc_update_heaps(true);

	bool result = test->check_func(test->baseaddr);

	rumboot_malloc_update_heaps(false);

	tend = rumboot_platform_get_uptime();

    rumboot_printf("--- %s. Took %d us to complete ---\n", 
		result ? "PASSED" : "FAILED", (tend - tstart));

	if (out) {
		/* Log results to our structure */
		struct rumboot_test_result *res = &out->results[out->num_tests];
		res->name = test->name;
		res->result = result;
		res->ticks = rumboot_platform_get_uptime() - tstart;
		out->num_tests++;
	}
	return result;
}


int test_suite_run(struct rumboot_testsuite_results *out, const struct rumboot_test_suite *suite)
{
	int total   = 0;
	int num_passed = 0;
	const struct rumboot_test *list = suite->tests;

	/* Cache strlen to save time */
	while (list->name) {
		total++;
		num_passed += test_suite_run_single(out, list, suite->name, suite->namelen);
		list++;
	}
	return total - num_passed;
}
