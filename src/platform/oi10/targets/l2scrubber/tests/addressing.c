#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <stdio.h>
#include <inttypes.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>

#include <rumboot/timer.h>
#include "../src/platform/oi10/targets/l2scrubber/glue.h"


int main()
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);

    int i;
    for (i = 0; i < layout.l2size_bytes / 8 ; i++)
    {
        int way, addr, subaddr;
        l2_data_address_decode(&layout, i, &way, &addr, &subaddr);
        int idx = l2_data_address_encode(&layout, way, addr, subaddr);
        rumboot_printf("index: %d(%d), way: %d addr: %d sub:%d\n", idx, i, way, addr, subaddr);

        if (i != idx) {
            rumboot_printf("FAIL: way: %d addr: %d sub:%d\n", way, addr, subaddr);
            rumboot_printf("FAIL: expected index %d got %d\n", i, idx);

            return 1;
        }
        
    }
    return 0;
}