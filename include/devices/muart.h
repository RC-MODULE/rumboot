#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup devices_muart MUART
 * \ingroup devices
 *
 *  MUART function libriary.
 *  Contains functions such as:
 *  - Init muart
 *  - Enable muart
 *  - Disable muart
 *  - Write char to muart
 *  - Read char from muart
 *

 *
 *  Usage example:
 *
 *
 * \addtogroup devices_muart
 *
 * @{
 */

enum WORD_LENGTH {
	WL_5 = 0,
	WL_6,
	WL_7,
	WL_8,
};

enum STOP_BITS_N {
	STP1 = 0,
	STP3_2,
	STP2,
};

enum MUART_MODE {
	RS_232 = 0,
	RS_422,
	RS_485
};

struct muart_conf {
	enum WORD_LENGTH	wlen;
	enum STOP_BITS_N	stp2;
	bool			is_even;
	bool			is_parity_available;
	enum MUART_MODE mode;
	bool rts_en;
	bool cts_en;
	bool			is_loopback;
	uint32_t	baud_rate;
	bool dma_en;
};

/**
 * brief Init MUART
 * @param base base address of
 * @param conf [description]
 */
void muart_init(const uint32_t base, const struct muart_conf *conf);

/**
 * brief Enable MUART
 * @param base base address of MUART
 */
void muart_enable(const uint32_t base);

/**
 * brief Disable MUART
 * @param base base address of MUART
 */
void muart_disable(const uint32_t base);

/**
 * brief Write char to MUART
 * @param base base address of MUART
 * @param ch   written symbol
 */
void muart_write_char(const uint32_t base, char ch);

/**
 * brief Read char from MUART
 * @param  base base address of MUART
 * @return     read symbol
 */
char muart_read_char(const uint32_t base);
