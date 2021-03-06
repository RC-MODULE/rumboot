#ifndef __TEST_SUITE_H__
#define __TEST_SUITE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>

#include <rumboot/timer.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

#include <regs/regs_mdma.h>
#include <regs/regs_gp_timers.h>

#include <devices/mdma_chan_api.h>

#define MDMA_GP_TIMEOUT		1000

// Must be multiple of 8 bytes!
#define MDMA_GP_SEGMENT_MEM	128

struct mdma_gp {
	void *base_addr;
	struct mdma_cfg cfg;
	int irq;

	struct mdma_chan *chan_rd;
	struct mdma_chan *chan_wr;

	void *dst_addr;
	void *src_addr;
	int segment_size;

	void *dst_mirror;
	void *src_mirror;

	unsigned int start_l;
	unsigned int start_h;

	unsigned int end_l;
	unsigned int end_h;

	volatile bool done;
};

int mdma_gp_dev_init(struct mdma_gp *dev, int event_indx, int event_prior);
void mdma_gp_dev_destroy(struct mdma_gp *dev);
int mdma_gp_dev_config(struct mdma_gp *dev, void *src_addr, void *dst_addr, int segment_size);
void mdma_gp_dev_terminate(struct mdma_gp *dev);
int mdma_gp_dev_start(struct mdma_gp *dev);
bool mdma_gp_dev_check(struct mdma_gp *dev);
void mdma_gp_handler(int irq, void *arg);

void *mdma_gp_addr_trans(void *addr);

bool mdma_gp_mem_check(void *first, void *second, int size);
void mdma_gp_mem_fill(void *addr, int size, int val, int incr);

#endif // __TEST_SUITE_H__
