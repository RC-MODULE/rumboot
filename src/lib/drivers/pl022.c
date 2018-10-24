#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <devices/pl022.h>
#include <rumboot/platform.h>


#define CPSDVR_MIN 0x02
#define CPSDVR_MAX 0xFE
#define SCR_MIN    0x00
#define SCR_MAX    0xFF

//#define DEBUG

static void set_data_size(uint32_t base, uint32_t data_size)
{
	uint32_t value = ioread32(base + PL022_CR0);
	value &= ~0b1111;
	data_size--;
	data_size &= 0b1111;
	value |= data_size;
	iowrite32(value, base + PL022_CR0);
}

static inline uint32_t spi_rate(uint32_t rate, uint16_t cpsdvsr, uint16_t scr)
{
	return rate / (cpsdvsr * (1 + scr));
}

int pl022_set_speed(uint32_t base, struct pl022_config *conf)
{
	/* Lets calculate the frequency parameters */
	uint16_t cpsdvsr = CPSDVR_MIN, scr = SCR_MIN;
	uint32_t rate, max_tclk, min_tclk, freq, best_freq = 0, best_cpsdvsr = 0,
		best_scr = 0, tmp, found = 0;

	freq = conf->spi_clk;
	rate = conf->ssp_clk;

	/* cpsdvscr = 2 & scr 0 */
	max_tclk = spi_rate(rate, CPSDVR_MIN, SCR_MIN);
	/* cpsdvsr = 254 & scr = 255 */
	min_tclk = spi_rate(rate, CPSDVR_MAX, SCR_MAX);

	if (freq > max_tclk)
		rumboot_printf("pl022: Max speed that can be programmed is %d Hz, you requested %d\n",
			max_tclk, freq);

	if (freq < min_tclk) {
			rumboot_printf("pl022: Requested frequency: %d Hz is less than minimum possible %d Hz\n",
			freq, min_tclk);
		return -1;
	}

	/*
	 * best_freq will give closest possible available rate (<= requested
	 * freq) for all values of scr & cpsdvsr.
	 */
	while ((cpsdvsr <= CPSDVR_MAX) && !found) {
		while (scr <= SCR_MAX) {
			tmp = spi_rate(rate, cpsdvsr, scr);

			if (tmp > freq) {
				/* we need lower freq */
				scr++;
				continue;
			}

			/*
			 * If found exact value, mark found and break.
			 * If found more closer value, update and break.
			 */
			if (tmp > best_freq) {
				best_freq = tmp;
				best_cpsdvsr = cpsdvsr;
				best_scr = scr;

				if (tmp == freq)
					found = 1;
			}
			/*
			 * increased scr will give lower rates, which are not
			 * required
			 */
			break;
		}
		cpsdvsr += 2;
		scr = SCR_MIN;
	}

	if (!best_freq)
		rumboot_printf("pl022: Matching cpsdvsr and scr not found for %d Hz rate \n", freq);

	iowrite32((best_cpsdvsr & 0xFF), base + PL022_CPSR);

	uint32_t v = ioread32(base + PL022_CR0);
	v &= ~0xff00;
	iowrite32(v | ((best_scr & 0xFF) << 8), base + PL022_CR0);

#ifdef DEBUG
	rumboot_printf("pl022: SPI Target Frequency is: %u, Effective Frequency is %u\n",
		freq, best_freq);
	rumboot_printf("pl022: SSP cpsdvsr = %d, scr = %d\n",
		best_cpsdvsr, best_scr);

	rumboot_printf("pl022: CR0:  0x%x\n", ioread32(base + PL022_CR0));
	rumboot_printf("pl022: CPSR: 0x%x\n", ioread32(base + PL022_CPSR));
#endif

	return 0;
}

void pl022_enable(uint32_t base)
{
	iowrite32((1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i), base + PL022_CR1);
}

void pl022_disable(uint32_t base)
{
	uint32_t cr1 = ioread32(base + PL022_CR1);
	iowrite32(cr1 & ~((1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i)), base + PL022_CR1);
}

void pl022_init(uint32_t base, struct pl022_config *conf)
{
	pl022_set_speed(base, conf);
	set_data_size(base, conf->data_size);
	pl022_enable(base);
	uint32_t soft_cs_ctl = ioread32(base + SSPSR_SOFTCS);
	if (conf->soft_cs) {
		iowrite32(soft_cs_ctl | (1<<1), base + SSPSR_SOFTCS);
	} else {
		iowrite32(soft_cs_ctl & (~(1<<1)), base + SSPSR_SOFTCS);
	}
}

void pl022_internal_cs(uint32_t base, int select)
{
	uint32_t soft_cs_ctl = ioread32(base + SSPSR_SOFTCS);
	if (select) {
		iowrite32(soft_cs_ctl | (1<<0), base + SSPSR_SOFTCS);
	} else {
		iowrite32(soft_cs_ctl & (~(1<<0)), base + SSPSR_SOFTCS);
	}
}

bool pl022_tx_empty(uint32_t base)
{
	return ioread32(base + PL022_SR) & (1 << PL022_SR__TFE_i);
}

bool pl022_tx_avail(uint32_t base)
{
	return ioread32(base + PL022_SR) & (1 << PL022_SR__TNF_i);
}

bool pl022_rx_empty(uint32_t base)
{
	return !(ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i));
}

void pl022_dump_fifo_state(uint32_t base)
{
	rumboot_printf("pl022: FIFO: rx: %s tx %s\n",
		pl022_rx_empty(base) ? "empty" : "non-empty",
		pl022_tx_empty(base) ? "empty" : "non-empty");
}

int pl022_xfer_timeout(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len, uint32_t timeout)
{
	int written = 0;
	int read = 0;
	uint32_t lastop = rumboot_platform_get_uptime();

	while ((read < len) || (written < len)) {
		if ((read < len) && (!pl022_rx_empty(base))) {
			*rdbuf++ = (unsigned char)(ioread32(base + PL022_DR) & 0xff);
			read++;
			lastop = rumboot_platform_get_uptime();
		}

		if ((written < len) && (pl022_tx_avail(base))) {
			iowrite32(wrbuf ? *wrbuf++ : 0, base + PL022_DR);
			written++;
			lastop = rumboot_platform_get_uptime();
		}
		if (timeout && (rumboot_platform_get_uptime() - lastop > timeout)) {
			return -1;
		}
	}
	/* return count of bytes read */
	return read;
}

int pl022_xfer(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len)
{
	return pl022_xfer_timeout(base, wrbuf, rdbuf, len, 0);
}

void pl022_clear_rx_buf(uint32_t base)
{
	while (ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i))
		ioread32(base + PL022_DR);
}
