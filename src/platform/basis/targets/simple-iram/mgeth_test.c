#include <devices/mgeth.h>
#include <devices/mdma.h>

#include <rumboot/rumboot.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/testsuite.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

#include <string.h>

struct base_addrs {
	uint32_t	base1;
	uint32_t	base2;
};

static bool mgeth_mdma_test(uint32_t to_addrs)
{
  struct base_addrs * addrs = (struct base_addrs *) to_addrs;

  struct mgeth_conf cfg = {
    .is_full_duplex = true,
    .speed = SPEED_1000,
  };

  size_t byte_number = 64;
  char *in_buf = rumboot_malloc_from_heap_aligned(0, byte_number, 8);
  char *out_buf = rumboot_malloc_from_heap_aligned(0, byte_number, 8);

	volatile uint64_t * cur_ptr = (volatile uint64_t *) in_buf;
	volatile uint64_t * end_ptr = (volatile uint64_t *) in_buf + byte_number;
	while (cur_ptr < end_ptr) {
		*cur_ptr = 0x55555555aaaaaaaa;
		cur_ptr++;
	}

  memset(out_buf, 0x0, byte_number);

  mgeth_init_sgmii();

  mgeth_init( addrs->base1, &cfg);
  mgeth_init( addrs->base2, &cfg);

  if( mgeth_transmit_data_throught_mdma(addrs->base1,  addrs->base2, out_buf, in_buf, byte_number) < 0 ) {
    goto test_failed;
  }

  if (memcmp(in_buf, out_buf, byte_number) != 0) {
  	goto test_failed;
  }

  return true;

  test_failed:
    rumboot_free(in_buf);
    rumboot_free(out_buf);
    return false;
}

// void handler_eth()
// {
//
// }


static const struct base_addrs addrs01 = { .base1 = ETH0_BASE, .base2 = ETH1_BASE };
static const struct base_addrs addrs23 = { .base1 = ETH2_BASE, .base2 = ETH3_BASE };

TEST_SUITE_BEGIN(mgeth_test, "MGETH test")
TEST_ENTRY("mgeth", mgeth_mdma_test, (uint32_t)&addrs01),
//if you turn it! Please change IRQ vector in main. Change MGETH1_IRQ on MGETH3_IRQ!
TEST_ENTRY("mgeth", mgeth_mdma_test, (uint32_t)&addrs23),
TEST_SUITE_END();

uint32_t main()
{
	volatile uint32_t * addrs = rumboot_malloc_from_heap_aligned(0, 12, 8);
	void * start_ptr = (void *) addrs;
	*addrs = ETH1_BASE;
	addrs++;
	*addrs = 0x900;//read channel
	addrs++;
	*addrs = 0x100; //write channel
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

	rumboot_irq_set_handler(tbl, MGETH1_IRQ, 0, mdma_irq_handler, start_ptr);
	rumboot_irq_enable(MGETH1_IRQ);
	rumboot_irq_table_activate(tbl);

	iowrite32(0x1, GLOBAL_TIMERS);
	int ret = test_suite_run(NULL, &mgeth_test);

	asm volatile ("swi #52");

	rumboot_printf("%d tests from suite failed\n", ret);

	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	return ret;
}
