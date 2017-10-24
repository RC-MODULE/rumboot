#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <devices/gic.h>
#include <rumboot/io.h>
#include <rumboot/testsuite.h>

static void handler(int irq, void *arg)
{
	volatile uint32_t *done = arg;
	(*done)++;
	rumboot_printf("IRQ arrived, arg %x, count %d\n", arg, (*done));
}


static void fire()
{
	iowrite32(GIC_GENSWINT0, (GIC_DIST_BASE + GICD_REG_SGIR));
}

static void delay(uint32_t deadline)
{
	while (deadline--) {
		asm volatile ("nop");
	}
}

static bool test_swirq_simple(uint32_t arg)
{
	volatile uint32_t done = 0;
	int i;

	rumboot_irq_sei();
	rumboot_irq_set_handler(NULL, 0, 0, handler, (void *) &done);

	for (i = 0; i < arg; i++) {
		rumboot_printf("Iteration #%d\n", i);
		int deadline = 10;

		fire();

		while (deadline-- && (done == 0)) {
			asm volatile ("nop");
		}

		if (deadline <= 0)
			return false;
	}
	return true;
}

static bool test_irq_cli_sei(uint32_t arg)
{
	uint32_t done = 0;
	rumboot_irq_set_handler(NULL, 0, 0, handler, (void *) &done);
	rumboot_irq_cli();
	fire();
	delay(arg);
	if (done) {
		rumboot_printf("rumboot_irq_cli() is broken\n");
		return false;
	}

	rumboot_irq_sei();
	delay(arg);
	if (!done) {
		rumboot_printf("rumboot_irq_sei() is broken\n");
		return false;
	}

	return true;
}

static bool test_irq_atomic(uint32_t arg)
{
	uint32_t done = 0;
	rumboot_irq_set_handler(NULL, 0, 0, handler, (void *) &done);
	rumboot_irq_sei();

	/* The same, but from an atomic block */
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_printf("Firing IRQ from atomic block, arg %x\n", &done);
		fire();
		delay(arg);
		if (done != 0) {
			rumboot_printf("Atomics didn't prevent an irq ;(\n");
			return false;
		}
	}

	delay(arg);
	if (done == 0) {
		rumboot_printf("Atomic didn't re-enable the irq ;(\n");
		fire();
		delay(arg);
		if (done) {
			rumboot_printf("The IRQ looks like an implulse one\n");
		}
		return false;
	}
	return true;
}

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(gic_irq_test, "GIC and IRQ Subsystem")
TEST_ENTRY("GIC_GENSWINT0", 				test_swirq_simple, 	  2),
TEST_ENTRY("ARM_CLI_SEI",   				test_irq_cli_sei,  	  4),
TEST_ENTRY("ARM_ATOMIC",    				test_irq_atomic,   	  4),
TEST_SUITE_END();

/* Finally, call the whole test suite */
int main()
{
	/* Disable all interrupts */
	rumboot_irq_cli();

	/* Create an IRQ table */
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

	/* Configure handler */
	/* Activate the table */
	rumboot_irq_table_activate(tbl);

	/* Enable IRQ */
	rumboot_irq_enable(0);

	/* Allow interrupt handling */
	rumboot_irq_sei();

	/* Run our irq test suite */
    int ret = test_suite_run(NULL, &gic_irq_test);
    rumboot_printf("%d tests from suite failed\n", ret);

	/* Deinit */
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

    return ret;
}
