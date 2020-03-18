#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/regs_tvsens.h>
#include <platform/devices.h>

int main() {
    volatile uint32_t data_r;

    rumboot_printf("tvsens: start\n");

    while (1) {
        for (data_r = 0; data_r<800000;) { data_r++; }

        data_r = ioread32 (TVSENS_BASE + TAVG);
        rumboot_printf("TAVG: 0x%x 0x%x 0x%x\n", (data_r>>20)&0x3FF, (data_r>>10)&0x3FF, data_r&0x3FF);
    }

    return 0;
}