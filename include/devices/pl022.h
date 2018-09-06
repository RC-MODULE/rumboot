#ifndef PL022_H
#define PL022_H

struct pl022_config {
	uint32_t ssp_clk;
	uint32_t spi_clk;
	uint32_t data_size;
};

void pl022_init(uint32_t base, struct pl022_config *conf);
int pl022_set_speed(uint32_t base, struct pl022_config *conf);
void pl022_internal_cs(uint32_t base, int select);
bool pl022_tx_empty(uint32_t base);
bool pl022_tx_avail(uint32_t base);
bool pl022_rx_empty(uint32_t base);
void pl022_dump_fifo_state(uint32_t base);
int pl022_xfer(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len);
int pl022_xfer_timeout(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len, uint32_t timeout);
void pl022_clear_rx_buf(uint32_t base);

#endif /* end of include guard: PL022_H */
