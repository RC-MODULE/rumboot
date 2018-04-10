#include <stdint.h>
#include <stdbool.h>

#include <devices/muart.h>
#include <regs/regs_muart.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>

static uint32_t calc_bdiv(uint32_t baud_rate, uint8_t n)
{
	return (400 * 1000000 / baud_rate) / 8;
}

static void set_reg(uint32_t base, uint32_t reg_offset, uint32_t value)
{
	iowrite32(value, base + reg_offset);
}

static void set_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask)
{
	uint32_t reg = ioread32(base + reg_offset);

	reg |= (mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}

static void reset_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask)
{
	uint32_t reg = ioread32(base + reg_offset);

	reg &= ~(mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}


void muart_init(const uint32_t base, const struct muart_conf *conf)
{
	uint32_t ctrl = ioread32(base + MUART_CTRL);

	ctrl |= (conf->mode << MUART_MDS_i);

	ctrl |= (conf->wlen << MUART_WLEN_i);
	ctrl |= (conf->stp2 << MUART_STP2_i);

	if (conf->is_even) ctrl |= (1 << MUART_EPS_i);
	if (conf->is_loopback) ctrl |= (1 << MUART_LBE_i);
	if (conf->is_parity_available) ctrl |= (1 << MUART_PEN_i);

	if( conf->mode == RS_232 ) {
		ctrl |= (1 << MUART_RTSen_i) | (1 << MUART_CTSen_i);
	}
	else if( conf->mode == RS_485) {
		ctrl |= (1 << MUART_RTSen_i) & ~(1 << MUART_CTSen_i);
	}

	if (conf->dma_en) ctrl &= ~(1 << MUART_APB_MD);
	else ctrl |= (1 << MUART_APB_MD);

	set_reg(base, MUART_CTRL, ctrl);

	set_field(base, MUART_BDIV, MUART_BAUD_DIV_i, calc_bdiv(conf->baud_rate, 10));
}

void muart_enable(uint32_t base)
{
	set_field(base, MUART_CTRL, MUART_MEN_i, 1);
}

void muart_disable(uint32_t base)
{
	reset_field(base, MUART_CTRL, MUART_MEN_i, 0);
}

#include <rumboot/printf.h>
void muart_write_char(uint32_t base, char ch)
{
	while ((ioread32(base + MUART_FIFO_STATE) & 0x7ff0000) == 0x400) rumboot_printf("fifo state: %x\n", ioread32(base + MUART_FIFO_STATE)); ;

	iowrite8((uint8_t)ch, base + MUART_DTRANS);
}

char muart_read_char(uint32_t base)
{
	char ch = '\0';

	while ((ioread32(base + MUART_FIFO_STATE) & 0xfff) == 0) rumboot_printf("fifo state: %x\n", ioread32(base + MUART_FIFO_STATE));

	ch = ioread8(base + MUART_DREC);

	return ch;
}

int muart_transmit_data_throught_apb(uint32_t base1, uint32_t base2, void* data, size_t size)
{
	rumboot_printf("write char\n");
	int count = size;
	while (count--)
		muart_write_char(base1, *((char*) data++) );

	rumboot_printf("read char\n");
	count = size;
	char read_ch = '\0';
	while (count--) {
		read_ch = muart_read_char(base2);

		if (read_ch == *((char*) data++)) {
			continue;
		} else {
			rumboot_printf("read char: %c\n", read_ch);
			return -1;
		}
	}

	return 0;
}

#include <devices/mdma.h>
#define NORMAL_DESC_SIZE 8
#define PITCH_DESC_SIZE 16
int muart_transmit_data_throught_mdma(uint32_t base1, uint32_t base2, volatile void *dest, volatile void *src, size_t len)
{
	//CONFIG DMA
	uint8_t desc_type = NORMAL;
	bool irq_en = false;
	uint16_t desc_gap = (desc_type == NORMAL) ? NORMAL_DESC_SIZE : PITCH_DESC_SIZE;

	rumboot_printf("Config MUART0.\n");
	size_t num_txdescriptors = 1;
	struct mdma_config cfg0 = { .desc_type		= desc_type, .desc_gap = desc_gap, .irq_en = irq_en,
				   .num_rxdescriptors	= 0,.num_txdescriptors = num_txdescriptors };
	volatile struct mdma_device *muart0 = mdma_create(base1, &cfg0);
	mdma_configure(muart0, &cfg0);

	rumboot_printf("Config MUART1.\n");
	size_t num_rxdescriptors = 1;
	struct mdma_config cfg1 = { .desc_type		= desc_type,	   .desc_gap	      = desc_gap, .irq_en = irq_en,
				   .num_rxdescriptors	= num_rxdescriptors,.num_txdescriptors = 0 };
	volatile struct mdma_device *muart1 = mdma_create(base2, &cfg1);
	mdma_configure(muart1, &cfg1);

	//Create transactions
	//We can fragment data in junks and transmit in several transactions!
	volatile struct mdma_transaction *t0 = NULL;
	volatile struct mdma_transaction *t1 = NULL;
	size_t transaction_count = 1;
	while (transaction_count--) {
		rumboot_printf("Create transaction.\n");
		rumboot_printf("Destination: %x, source: %x, length: %x\n", dest, src, len);
		t0 = mdma_transaction_create(muart0, dest, src, len);
		t1 = mdma_transaction_create(muart1, dest, src, len*2);

		//Push transaction to queue
		rumboot_printf("Queue transaction.\n");
		//mdma_transaction_dump(t);
		if( mdma_transaction_queue((struct mdma_transaction*) t0) < 0) return -1;
		if( mdma_transaction_queue((struct mdma_transaction*) t1) < 0) return -1;
		t0++;
		t1++;
	}

	//Init DMA
	rumboot_printf("Init MDMA1, base addr: %x.\n", muart1->base);
	mdma_init(muart1);

	rumboot_printf("Init MDMA0, base addr: %x.\n", muart0->base);
	mdma_init(muart0);

	rumboot_printf("Check - if mdma is ready.\n");
	size_t timeout_us = 1000;
	while (!mdma_is_finished(muart1)) {
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

	mdma_remove(muart0);
	mdma_remove(muart1);

	return 0;
}
