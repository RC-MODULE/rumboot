#include <stdbool.h>

#include <algo/shrink.h>

#include <rumboot/printf.h>


static int s1, s2, s3;

static void prandom_reset()
{
	s1 = 100;
	s2 = 200;
	s3 = 300;
}

static uint32_t prandom32()
{
#define TAUSWORTHE(s, a, b, c, d) ((s & c) << d) ^ (((s << a) ^ s) >> b)

	s1 = TAUSWORTHE(s1, 13, 19, 4294967294UL, 12);
	s2 = TAUSWORTHE(s2, 2, 25, 4294967288UL, 4);
	s3 = TAUSWORTHE(s3, 3, 11, 4294967280UL, 17);

	return s1 ^ s2 ^ s3;
}

uint32_t shrink(uint8_t *inbuf, const uint32_t input_size, uint8_t *compbuf, const uint32_t comp_sz)
{
	heatshrink_encoder encoder;

	heatshrink_encoder_reset(&encoder);

	uint32_t sunk = 0;
	uint32_t polled = 0;
	size_t count = 0;

	HSE_sink_res res_sink;
	HSE_poll_res res_poll;
	HSE_finish_res res_finish;

	while (sunk < input_size) {
		res_sink = heatshrink_encoder_sink(&encoder, &inbuf[sunk], input_size - sunk, &count);
		if (res_sink < 0)
			return polled;

		sunk += count;

		if (sunk == input_size)
			if (heatshrink_encoder_finish(&encoder) != HSER_FINISH_MORE)
				return polled;

		do {
			res_poll = heatshrink_encoder_poll(&encoder, &compbuf[polled], comp_sz - polled, &count);

			if (res_poll < 0)
				return polled;

			polled += count;

#ifdef DEBUG
			rumboot_printf("^^ polled %u\n", count);
#endif
		} while (res_poll == HSER_POLL_MORE);

		if (res_poll != HSER_POLL_EMPTY)
			return polled;


		if (polled >= comp_sz) {
#ifdef DEBUG
			rumboot_printf("compression should never expand that much\n");
#endif
		}

		if (sunk == input_size) {
			res_finish = heatshrink_encoder_finish(&encoder);
			if (res_finish != HSER_FINISH_DONE)
				return polled;
		}
	}

#ifdef DEBUG
	rumboot_printf("in: %u compressed: %u ", input_size, polled);
#endif

	return polled;
}

void unshrink(uint8_t *compbuf, const uint32_t compressed_size, uint8_t *decompbuf, const uint32_t decomp_sz,
	      const uint32_t etalon_sz)
{
	heatshrink_decoder decoder;

	heatshrink_decoder_reset(&decoder);

	uint32_t sunk = 0;
	uint32_t polled = 0;
	size_t count = 0;

	HSD_sink_res res_sink;
	HSD_poll_res res_poll;
	HSD_finish_res res_finish;

#ifdef DEBUG
	rumboot_printf("\nDECOMPRESSING\n");
#endif
	//    dump_buf("comp", compbuf, compressed_size);

	while (sunk < compressed_size) {
		res_sink = heatshrink_decoder_sink(&decoder, &compbuf[sunk], compressed_size - sunk, &count);

		if (res_sink < 0)
			return;

		sunk += count;

#ifdef DEBUG
		rumboot_printf("^^ sunk %u\n", count);
#endif

		if (sunk == compressed_size) {
			res_finish = heatshrink_decoder_finish(&decoder);
			if (res_finish != HSDR_FINISH_MORE)
				return;
		}

		do {
			res_poll = heatshrink_decoder_poll(&decoder, &decompbuf[polled], decomp_sz - polled, &count);

			if (res_poll < 0)
				return;

			polled += count;

#ifdef DEBUG
			rumboot_printf("^^ polled %u\n", count);
#endif
		} while (res_poll == HSDR_POLL_MORE);

		if (res_poll != HSDR_POLL_EMPTY)
			return;

		if (sunk == compressed_size) {
			res_finish = heatshrink_decoder_finish(&decoder);
			if (res_finish != HSDR_FINISH_DONE)
				return;
		}

#ifdef DEBUG
		if (polled > etalon_sz)
			rumboot_printf("Decompressed data is larger than original input");
#endif
	}

#ifdef DEBUG
	rumboot_printf("decompressed: %d\n", polled);
#endif

	if (polled != etalon_sz) {
#ifdef DEBUG
		rumboot_printf("Decompressed length does not match original input length");
#endif
	}
}

bool check_buf_with_random_seq(uint8_t *buf, uint32_t size)
{
	bool is_ok = true;

	prandom_reset();
	//prandom32();

	size_t i;
	uint8_t random;
	for (i = 0; i < size; i++) {
		random = prandom32() >> 24;

#ifdef DEBUG
		rumboot_printf("random value = %x buf[%d] = %x\n", random, i, buf[i]);
#endif

		if (buf[i] != random) {
			is_ok = false;
			break;
		}
	}

#ifdef DEBUG
	is_ok ? rumboot_printf("Validation is OK!\n") : rumboot_printf("Validation is failed!\n");
#endif

	return is_ok;
}

void fill_buff_with_random_seq(uint8_t *buf, uint32_t size)
{
	prandom_reset();

	size_t i;
	for (i = 0; i < size; i++)
		buf[i] = prandom32() >> 24;
}

void dump_buf(uint8_t *buf, uint32_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
		rumboot_printf("buf[%d]=%d ", i, buf[i]);

	rumboot_printf("\n");
}
