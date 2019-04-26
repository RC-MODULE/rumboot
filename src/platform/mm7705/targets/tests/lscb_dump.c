#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/io.h>


#define MK_WR 1
#define MK_RD 0
uint32_t mkio_xfer(uintptr_t base, uint16_t maddr, uint16_t mdata, int wr)
{
        iowrite32((maddr << 16) | (wr << 30) | mdata | 1 << 31, base);
        while (ioread32(base) & (1<<31));
        return ioread32(base);
}


#define GPIOAFSEL 0x420
int main()
{
        iowrite32(0xFF, LSIF1_MGPIO0_BASE + GPIOAFSEL);
        iowrite32(0xFF, LSIF1_MGPIO1_BASE + GPIOAFSEL);
        iowrite32(0xFF, LSIF1_MGPIO2_BASE + GPIOAFSEL);
        iowrite32(0x00, GPIO0_BASE + GPIOAFSEL);

        uintptr_t p = LSCB0_BASE;

        int i;
        mkio_xfer(LSCB0_BASE, 0, 0xffff, MK_WR);
        mkio_xfer(LSCB0_BASE, 1, 0xc0de, MK_WR);
        for (i=0; i<16; i++) {
                uint32_t v = mkio_xfer(LSCB0_BASE, i, 0, MK_RD);
                rumboot_printf("[0x%x] = 0x%x\n",
                               i,
                               v
                               );
                mdelay(500);
        }
        return 0;
}
