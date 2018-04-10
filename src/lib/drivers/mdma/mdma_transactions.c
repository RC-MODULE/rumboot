#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <devices/mdma.h>

#include <rumboot/printf.h>
#include <rumboot/rumboot.h>

struct mdma_transaction *mdma_transaction_create(volatile struct mdma_device *dev,
	volatile void *dest, volatile void *src, size_t len)
{
	struct mdma_transaction *ret = rumboot_malloc(sizeof(*ret));

	if (!ret)
		return NULL;

	ret->src = src;

	if (!dest)
		ret->dest = rumboot_malloc(sizeof(*ret->dest));
	/* TODO: Err handling */
	else
		ret->dest = dest;

	ret->len = len;
	ret->owner = dev;
	ret->state = IDLE;

	return ret;
}

int mdma_transaction_remove(struct mdma_transaction *t)
{
	if (!t) return -1;

	t->state = FINISHED;

	rumboot_free(t);

	return 0;
}

int mdma_transaction_get_state(struct mdma_transaction *t)
{
	return t->state;
}

int mdma_transaction_queue(struct mdma_transaction *t)
{
	volatile struct mdma_device *dev = t->owner;

	// if (!t->src || !t->dest || !dev->rxtbl || !dev->txtbl)
	// 	return -1;

	//WE can cut memory and write any number of descriptors!
	size_t tx_desc_numb = dev->conf.num_txdescriptors;
	while(tx_desc_numb) {
			mdma_write_txdescriptor(dev, t->src, t->len, dev->conf.num_txdescriptors - tx_desc_numb--);
	}

	size_t rx_desc_numb = dev->conf.num_rxdescriptors;
	while(rx_desc_numb) {
		mdma_write_rxdescriptor(dev, t->dest, t->len, dev->conf.num_rxdescriptors - rx_desc_numb--);
	}

	t->state = STARTED;

	return 0;
}

bool mdma_transaction_is_finished(struct mdma_transaction *t)
{
	if (!mdma_is_finished(t->owner)) {

		rumboot_printf("Transaction is not finished yet!\n");
		return false;
	}

	t->state = FINISHED;

	return true;
}

void mdma_transaction_dump(struct mdma_transaction *t)
{
	rumboot_printf("dest: %x\n", (uint32_t) t->dest);
	rumboot_printf("src: %x\n", (uint32_t) t->src);
	rumboot_printf("len: %d\n", (uint32_t) t->len);
	rumboot_printf("state: %d\n", t->state);
}
