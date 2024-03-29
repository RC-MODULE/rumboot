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

static void set_data_size(uint32_t const base, uint32_t data_size)
{
    uint32_t value = ioread32(base + PL022_CR0);
    value &= ~0b1111;
    data_size--;
    data_size &= 0b1111;
    value |= data_size;
    iowrite32(value, base + PL022_CR0);
}

static inline uint32_t spi_rate(uint32_t const rate, uint16_t const cpsdvsr, uint16_t const scr)
{
    return rate / (cpsdvsr * (1 + scr));
}

int pl022_set_speed(uint32_t const base, struct pl022_config const * const conf)
{
    /* Lets calculate the frequency parameters */
    uint16_t cpsdvsr = CPSDVR_MIN, scr = SCR_MIN;
    uint32_t rate, min_tclk, freq, best_freq = 0, best_cpsdvsr = 0,
        best_scr = 0, tmp, found = 0;

    freq = conf->spi_clk;
    rate = conf->ssp_clk;

    /* cpsdvsr = 254 & scr = 255 */
    min_tclk = spi_rate(rate, CPSDVR_MAX, SCR_MAX);

#ifdef DEBUG
    /* cpsdvscr = 2 & scr 0 */
    uint32_t max_tclk = spi_rate(rate, CPSDVR_MIN, SCR_MIN);

    if (freq > max_tclk)
        rumboot_printf("pl022: Max speed that can be programmed is %d Hz, you requested %d\n",
            max_tclk, freq);
#endif

    if (freq < min_tclk) {
#ifdef DEBUG
            rumboot_printf("pl022: Requested frequency: %d Hz is less than minimum possible %d Hz\n",
            freq, min_tclk);
#endif
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

#ifdef DEBUG
    if (!best_freq)
        rumboot_printf("pl022: Matching cpsdvsr and scr not found for %d Hz rate \n", freq);
#endif

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

static inline void pl022_enable(uint32_t const base)
{
    uint32_t cr1 = ioread32(base + PL022_CR1);

    cr1 |= (1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i);

    iowrite32(cr1, base + PL022_CR1);
}

static inline void pl022_disable(uint32_t const base)
{
    uint32_t cr1 = ioread32(base + PL022_CR1);

    cr1 = (cr1 | (1 << PL022_CR1__SOD_i)) & ~(1 << PL022_CR1__SSE_i);

    iowrite32(cr1, base + PL022_CR1);
}

void pl022_slave_output(uint32_t const base, int enable)
{
    uint32_t cr1 = ioread32(base + PL022_CR1);

    if (enable)
         cr1 &= ~(1 << PL022_CR1__SOD_i);
    else
         cr1 |= (1 << PL022_CR1__SOD_i);

    iowrite32(cr1, base + PL022_CR1);
}

void pl022_init(uint32_t const base, struct pl022_config const * const conf)
{
    pl022_disable(base);

    pl022_set_speed(base, conf);
    set_data_size(base, conf->data_size);

    if (conf->soft_cs && (conf->variant != PL022_VARIANT_GSPI)) {
        rumboot_platform_panic("PL022: No SoftCS control for this hardware");
    }

    if (conf->variant != PL022_VARIANT_GSPI) {
	pl022_enable(base);
        return; /* Nothing to do */
    }

    uint32_t soft_cs_ctl = ioread32(base + SSPSR_SOFTCS);
    if (conf->soft_cs) {
        iowrite32(soft_cs_ctl | (1<<1), base + SSPSR_SOFTCS);
    } else {
        iowrite32(soft_cs_ctl & (~(1<<1)), base + SSPSR_SOFTCS);
    }

    pl022_enable(base);
}

void pl022_internal_cs(uint32_t const base, int const select)
{
    uint32_t soft_cs_ctl = ioread32(base + SSPSR_SOFTCS);
    if (select) {
        iowrite32(soft_cs_ctl | (1<<0), base + SSPSR_SOFTCS);
    } else {
        iowrite32(soft_cs_ctl & (~(1<<0)), base + SSPSR_SOFTCS);
    }
}

bool pl022_tx_empty(uint32_t const base)
{
    return !(ioread32(base + PL022_SR) & (1 << PL022_SR__BSY_i));
}

bool pl022_tx_avail(uint32_t const base)
{
    return ioread32(base + PL022_SR) & (1 << PL022_SR__TNF_i);
}

bool pl022_rx_empty(uint32_t const base)
{
    return !(ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i));
}

bool pl022_rx_full(uint32_t const base)
{
   return (ioread32(base + PL022_SR) & (1 << PL022_SR__RFF_i));
}

void pl022_dump_fifo_state(uint32_t const base)
{
    rumboot_printf("pl022: FIFO: rx: %s tx %s\n",
        pl022_rx_empty(base) ? "empty" : "non-empty",
        pl022_tx_empty(base) ? "empty" : "non-empty");
}

int pl022_xfer_timeout(uint32_t const base, unsigned char const *wrbuf, unsigned char *rdbuf, size_t const len, uint32_t const timeout)
{
    int written = 0;
    int read = 0;
    uint32_t lastop = rumboot_platform_get_uptime();

    while ((read < len) || (written < len)) {
        if ((read < len) && (!pl022_rx_empty(base))) {
            unsigned char tmp = (unsigned char)(ioread32(base + PL022_DR) & 0xff);
            if (rdbuf) {
                *rdbuf++ = tmp;
            }
            read++;
            lastop = rumboot_platform_get_uptime();
        }

        if ((written < len) && (pl022_tx_avail(base))) {
            unsigned char tmp = 0xff;
            if (wrbuf) {
                tmp = *wrbuf++;
            }
            iowrite32(tmp, base + PL022_DR);
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

int pl022_xfer(uint32_t const base, unsigned char const *wrbuf, unsigned char *rdbuf, size_t const len)
{
    return pl022_xfer_timeout(base, wrbuf, rdbuf, len, 0);
}

void pl022_clear_rx_buf(uint32_t const base)
{
    while (!pl022_tx_empty(base)) {};

    while (!pl022_rx_empty(base))
        ioread32(base + PL022_DR);


   iowrite32(1 << PL022_ICR__ROR_i, base + PL022_ICR);
}



void pl022_dma_reset(uint32_t const base_addr)
{
    iowrite32(0x01, base_addr + GSPI_SOFTRST); //reset DMA

    while (ioread32(base_addr + GSPI_SOFTRST))
        ;
}

void pl022_dma_enable(uint32_t const base_addr, enum ssp_dma_enable const enabled)
{
    iowrite32(enabled, base_addr + GSPI_SSPDMACR);
}

void pl022_dma_set_irq_mask(uint32_t const base_addr, enum ssp_dma_interrupt const interrupt)
{
    iowrite32(interrupt, base_addr + GSPI_IRQMASKS); //set irq masks
}

uint32_t pl022_get_ris(uint32_t const base_address)
{
    return ioread32(base_address + GSPI_SSPRIS);
}

uint32_t pl022_get_mis(uint32_t const base_address)
{
    return ioread32(base_address + GSPI_SSPMIS);
}

uint32_t pl022_get_ssp_status(uint32_t const base_addr)
{
    return (ioread32(base_addr + GSPI_SSPSR));
}

uint32_t pl022_get_dma_status(uint32_t const base_addr)
{
    return (ioread32(base_addr + GSPI_STATUS));
}

void pl022_set_int_mask(uint32_t const base_address, uint16_t mask)
{
    iowrite32(mask, base_address + GSPI_SSPIMSC);
}

void pl022_send_word(uint32_t const base_address, uint32_t word)
{
    //check transmit FIFO full
    if(ioread32(base_address + GSPI_SSPSR) & TNF) // transmit FIFO is not full
    {
        iowrite32(word, base_address + GSPI_SSPDR);
    } else
    {
        rumboot_printf("Warning: writing to SSP TFIFO when TFIFO is full\n");
    }
}

uint32_t pl022_get_word(uint32_t const base_address)
{
    uint32_t  word = 0x00;
    //check receive FIFO empty
    if(ioread32(base_address + GSPI_SSPSR) & RNE) //receive FIFO is not empty
    {
        word = ioread32(base_address + GSPI_SSPDR);
    } else
    {
        rumboot_printf("Warning: reading from SSP RFIFO when RFIFO is empty\n");
    }
    return word;
}


static inline void pl022_set_clock_rate(uint32_t const base_address, uint32_t const cpsdvr)
{
    //NOTE: CR0 should be zero
    iowrite32(cpsdvr, base_address + GSPI_SSPCPSR); //set divider
}

void pl022_dma_set_read_addr(uint32_t const base_addr, uint32_t const * const r_addr, uint32_t const byte_n)
{
    iowrite32((uint32_t)r_addr, base_addr + GSPI_DMARSTART);
    iowrite32((uint32_t)((uint8_t*)(r_addr) + byte_n+3), base_addr + GSPI_DMAREND);
}

void pl022_dma_set_write_addr(uint32_t const base_addr, uint32_t const * const w_addr, uint32_t const byte_n)
{
    iowrite32((uint32_t)(w_addr), base_addr + GSPI_DMAWSTART);
    iowrite32((uint32_t)((uint8_t*)(w_addr) + byte_n+3), base_addr + GSPI_DMAWEND);
}

void pl022_dma_set_mode(uint32_t const base_addr, enum dma_mode const mode)
{
    iowrite32(mode, base_addr + GSPI_AXIR_BUFTYPE);
}

void pl022_dma_set_param(uint32_t const base_addr, struct dma_params const * const param)
{
    iowrite32((param->endian_read << 9) | ((param->arlen << 5) |
              (param->endian_write << 4) | param->awlen),
             base_addr + GSPI_AXI_PARAMS);
}

void pl022_set_param(uint32_t const base_address, struct ssp_params const * const params)
{
    uint32_t cr0, cr1;
    //disable ssp
    pl022_disable(base_address);
    //setup the parameters
    cr0 = ioread32(base_address + GSPI_SSPCR0);
    cr0 &= ~((0x1 << SPH_SHIFT) | (0x1 << SPO_SHIFT) | (0x3 << FRF_SHIFT));
    //cpol, cpha, format
    iowrite32((((params->cpha & 0x1) << SPH_SHIFT) |
                ((params->cpol & 0x1) << SPO_SHIFT) |
                ((params->fr_format & 0x3) << FRF_SHIFT) | cr0),
                base_address + GSPI_SSPCR0);
    //mode, loopback, enable core
    cr1 = ioread32(base_address + GSPI_SSPCR1);
    cr1 &= ~((0x1 << LBM_SHIFT) | (0x1 << MS_SHIFT));
    iowrite32((((params->loopback & 0x1) << LBM_SHIFT) | ((params->mode & 0x1) << MS_SHIFT) | cr1), base_address + GSPI_SSPCR1);
    pl022_enable(base_address);
}
