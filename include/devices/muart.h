#include <stdbool.h>
#include <stdint.h>

enum word_length {
	WL_5 = 0,
	WL_6,
	WL_7,
	WL_8,
};

enum stop_bits_n {
	STP1 = 0,
	STP1_2,
	STP2,
};

enum muart_mode {
	RS_232 = 0,
	RS_422,
	RS_485
};

struct muart_conf {
	enum word_length	wlen;
	enum stop_bits_n	stp2;
	bool			is_even;
	bool			is_parity_available;
	enum muart_mode		mode;
	bool rts_en;
	bool cts_en;
	bool			is_loopback;
	uint32_t	baud_rate;
};

void muart_init(const uint32_t base, const struct muart_conf *conf);

void muart_enable(uint32_t base);

void muart_disable(uint32_t base);

void muart_write_char(uint32_t base, char ch);

char muart_read_char(uint32_t base);
