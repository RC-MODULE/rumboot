#include <stdint.h>
#include <stdbool.h>

#include <devices/muart.h>
#include <regs/regs_muart.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>

static uint32_t calc_bdiv(uint32_t baud_rate, uint8_t n) {

  return (400*1000000/baud_rate)/8;
}

static void set_reg(uint32_t base, uint32_t reg_offset, uint32_t value) {

  iowrite32(value, base + reg_offset);
}

static void set_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask) {
	uint32_t reg = ioread32(base + reg_offset);

	reg |= (mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}

static void reset_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask) {
	uint32_t reg = ioread32(base + reg_offset);

	reg &= ~(mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}


void muart_init(const uint32_t base, const struct muart_conf *conf)
{
	uint32_t ctrl = ioread32(base + MUART_CTRL);

	ctrl |= (conf->wlen << MUART_WLEN_i);
	ctrl |= (conf->stp2 << MUART_STP2_i);

	if (conf->is_even) ctrl |= (1 << MUART_EPS);
	if (conf->is_loopback) ctrl |= (1 << MUART_LBE_i);
	if (conf->is_parity_available) ctrl |= (1 << MUART_PEN_i);
  //if (conf->cts_en) ctrl |= (1<<MUART_CTSEn_i);
  //if (conf->rts_en) ctrl |= (1<<MUART_RTSEn_i);

  ctrl |= (1 << MUART_APB_ON_i);

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

void muart_write_char(uint32_t base, char ch) {

  while( ioread32(base + MUART_STATUS) & (1 << MUART_TXRIS_i) );;

  iowrite8((uint8_t) ch, base + MUART_DTRANS);
}

char muart_read_char(uint32_t base) {

  char ch = '\0';

  while( ioread32(base + MUART_STATUS) & (1 << MUART_RERIS_i) ) ;;

  ch = ioread8( base + MUART_DREC);

  return ch;
}
