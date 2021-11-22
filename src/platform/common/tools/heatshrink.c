#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>

#include "data.h"
#include <algo/heatshrink_encoder.h>
#include <algo/heatshrink_decoder.h>

int main(void)
{
    uint8_t *input = random_data_bin;
    size_t input_size = sizeof(random_data_bin);

    heatshrink_encoder hse;
    heatshrink_decoder hsd;
    heatshrink_encoder_reset(&hse);
    heatshrink_decoder_reset(&hsd);

    size_t comp_sz = input_size + (input_size/2) + 4;
    size_t decomp_sz = input_size + (input_size/2) + 4;
    uint8_t *comp = rumboot_malloc(comp_sz);
    uint8_t *decomp = rumboot_malloc(decomp_sz);

    memset(comp, 0, comp_sz);
    memset(decomp, 0, decomp_sz);
    size_t count = 0;

    uint32_t sunk = 0, polled = 0, time = rumboot_platform_get_uptime();
    while (sunk < input_size) {
        assert(heatshrink_encoder_sink(&hse, &input[sunk], input_size - sunk, &count) >= 0);
        sunk += count;
        if (sunk == input_size)
            assert(HSER_FINISH_MORE == heatshrink_encoder_finish(&hse));

        HSE_poll_res pres;
        do {                    /* "turn the crank" */
            pres = heatshrink_encoder_poll(&hse, &comp[polled], comp_sz - polled, &count);
            assert(pres >= 0);
            polled += count;
        } while (pres == HSER_POLL_MORE);

        assert(HSER_POLL_EMPTY == pres);

        if (sunk == input_size)
            assert(HSER_FINISH_DONE == heatshrink_encoder_finish(&hse));
    }

    uint32_t compressed_size = polled;
    sunk = 0;
    polled = 0;

    while (sunk < compressed_size) {
        assert(heatshrink_decoder_sink(&hsd, &comp[sunk],
                                       compressed_size - sunk, &count) >= 0);
        sunk += count;

        if (sunk == compressed_size)
            assert(HSDR_FINISH_MORE == heatshrink_decoder_finish(&hsd));

        HSD_poll_res pres;
        do {
            pres = heatshrink_decoder_poll(&hsd, &decomp[polled],
                decomp_sz - polled, &count);
            assert(pres >= 0);
            polled += count;
        } while (pres == HSDR_POLL_MORE);
        assert(HSDR_POLL_EMPTY == pres);
        if (sunk == compressed_size) {
            HSD_finish_res fres = heatshrink_decoder_finish(&hsd);
            assert(HSDR_FINISH_DONE == fres);
        }
    }

    assert(polled == input_size);

    for (size_t i = 0; i < input_size; i++)
        assert(input[i] == decomp[i]);

    rumboot_printf("TIME: %d usec\n", rumboot_platform_get_uptime() - time);

    return 0;
}
