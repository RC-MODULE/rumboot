#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <devices/mdma.h>


struct mdma_transaction *mmdma_transaction_create(struct mdma_device *dev, void *dest, void *src, size_t len)
{
	struct mdma_transaction *ret = malloc(sizeof(*ret));

	if (!ret)
		return NULL;

	ret->src = src;

	if (!dest)
		ret->dest = malloc(sizeof(*ret->dest));
	/* TODO: Err handling */
	else
		ret->dest = dest;

	ret->len = len;
	ret->owner = dev;
	ret->state = IDLE;

	return ret;
}

int mmdma_transaction_remove(struct mdma_transaction *t)
{
	if (!t) return -1;

	t->state = FINISHED;

	free(t);

	return 0;
}

int mdma_transaction_get_state(struct mdma_transaction *t)
{
	return t->state;
}

int mdma_transaction_queue(struct mdma_transaction *t)
{
	struct mdma_device *dev = t->owner;

	if (!t->src || !t->dest || !dev->rxtbl || !dev->txtbl)
		return -1;

	mdma_transaction(dev->base, t->src, t->dest, t->len, t->is_last);

	t->state = STARTED;

	return 0;
}

bool mdma_transaction_is_finished(struct mdma_transaction *t)
{
	if (!mdma_is_finished(t->owner->base))
		return false;

	t->state = FINISHED;
	return true;
}
