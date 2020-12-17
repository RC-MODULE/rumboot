/* -*- rumboot-test-labels: hwonly -*- */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <stdlib.h>
#include <platform/regs/regs_crg_sys.h>

static uint32_t exclk_get_freq(uint32_t base_freq_khz, int div)
{
    if (div < 0)
        return base_freq_khz;
    return (base_freq_khz / (div + 1));
}

void extclk_configure(uint32_t base_freq_khz, uint32_t target_freq_khz)
{
    int i;
    uint32_t best_delta = -1;
    int best_div = -1; 

    for (i=0; i<31; i++) {
        uint32_t frq = exclk_get_freq(base_freq_khz, i);
        uint32_t curdelta = abs((int32_t) frq - target_freq_khz);
        if (curdelta < best_delta) {
            best_delta = abs(frq - target_freq_khz);
            best_div = i;
        }
    }
    rumboot_printf("extclk: Actual freq: %d kHz, desired: %d kHz (div = %d) \n", 
        exclk_get_freq(base_freq_khz, best_div), target_freq_khz, best_div);
    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, 0x1ACCE551);
    dcr_write(DCR_CRG_BASE + CRG_SYS_CKDIVMODE3,best_div | (0 << 16));
    dcr_write(DCR_CRG_BASE + CRG_SYS_CKUPDATE, 1);
    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, 0xd00dc0de);
}

int main()
{
    rumboot_printf("Configuring EXTCLK pin\n");
    extclk_configure(200000, 112000);
    while(1);
    return 0;
}
