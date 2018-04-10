#include <devices/mgeth.h>
#include <devices/mdma.h>

#include <regs/regs_mgeth.h>

#include <stdbool.h>
#include <stdint.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

void mgeth_init(const uint32_t base, const struct mgeth_conf *conf)
{
  uint8_t is_full_duplex = conf->is_full_duplex ? 1 : 0;

  uint32_t ctrl = (is_full_duplex << MGETH_FULL_DUPLEX_i) | (conf->speed << MGETH_SPEED_i);

  iowrite32(ctrl, base + MGETH_CONTROL);
}

#define NORMAL_DESC_SIZE 8
#define PITCH_DESC_SIZE 16
int mgeth_transmit_data_throught_mdma(uint32_t base1, uint32_t base2, volatile void *dest, volatile void *src, size_t len)
{
  //CONFIG DMA
	uint8_t desc_type = LONG;
	bool irq_en = false;
	uint16_t desc_gap = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;

	rumboot_printf("Config mgeth0.\n");
	size_t num_txdescriptors = 1;
	struct mdma_config cfg0 = { .desc_type		= desc_type, .desc_gap = desc_gap, .irq_en = irq_en,
				   .num_rxdescriptors	= 0,.num_txdescriptors = num_txdescriptors };
	volatile struct mdma_device *mgeth0 = mdma_create(base1 + 0x900, &cfg0);
  mgeth0->type = MDMA_MGETH;
	mdma_configure(mgeth0, &cfg0);

	rumboot_printf("Config mgeth1.\n");
	size_t num_rxdescriptors = 1;
	struct mdma_config cfg1 = { .desc_type		= desc_type,	   .desc_gap	      = desc_gap, .irq_en = irq_en,
				   .num_rxdescriptors	= num_rxdescriptors,.num_txdescriptors = 0 };
	volatile struct mdma_device *mgeth1 = mdma_create(base2 + 0x100, &cfg1);
  mgeth1->type = MDMA_MGETH;
  mdma_configure(mgeth1, &cfg1);

	//Create transactions
	//We can fragment data in junks and transmit in several transactions!
	volatile struct mdma_transaction *t0 = NULL;
	volatile struct mdma_transaction *t1 = NULL;
	size_t transaction_count = 1;
	while (transaction_count--) {
		rumboot_printf("Create transaction.\n");
		rumboot_printf("Destination: %x, source: %x, length: %x\n", dest, src, len);
		t0 = mdma_transaction_create(mgeth0, dest, src, len);
		t1 = mdma_transaction_create(mgeth1, dest, src, len);

		//Push transaction to queue
		rumboot_printf("Queue transaction.\n");
		//mdma_transaction_dump(t);
		if( mdma_transaction_queue((struct mdma_transaction*) t0) < 0) return -1;
		if( mdma_transaction_queue((struct mdma_transaction*) t1) < 0) return -1;
		t0++;
		t1++;
	}

	//Init DMA
	rumboot_printf("Init MDMA1, base addr: %x.\n", mgeth1->base);
	mdma_init(mgeth1);

	rumboot_printf("Init MDMA0, base addr: %x.\n", mgeth0->base);
	mdma_init(mgeth0);

	rumboot_printf("Check - if mdma is ready.\n");
	size_t timeout_us = 1000;
	while (!mdma_is_finished(mgeth1)) {
		if (!timeout_us) {
			return -1;
		}
		udelay(1);
		timeout_us--;
	}

	//Remove transactions
	transaction_count = 1;
	while (transaction_count--) {
		rumboot_printf("Remove transaction.\n");
		mdma_transaction_remove((struct mdma_transaction*) t0);
		mdma_transaction_remove((struct mdma_transaction*) t1);
		t0++;
		t1++;
	}

	mdma_remove(mgeth0);
	mdma_remove(mgeth1);

	return 0;
}
