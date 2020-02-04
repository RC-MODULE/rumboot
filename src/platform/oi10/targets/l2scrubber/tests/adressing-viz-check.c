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


static void write_some_shit(struct l2c_mem_layout *layout, int way, int addr, int subaddr)
{
    int idx = l2_data_address_encode(layout, way, addr, subaddr);
    uint64_t some_shit = 0xdeadc0deb00bc0de;
    l2c_write_mem(DCR_L2C_BASE, layout, L2C_MEM_DATA_ECC, idx, some_shit); 
    l2c_write_mem(DCR_L2C_BASE, layout, L2C_MEM_DATA, idx, some_shit); 
}

int main()
{
    rumboot_printf("Writing some data into L2\n");
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    int w; 
    for (w=0; w<4; w++) {
        write_some_shit(&layout, w, 0, 0);
        write_some_shit(&layout, w, 8191, 7);
    }
    rumboot_printf("Please check timing diagrams\n");
    return 0;
}