#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_simple.h>

void simple_config_mdma(uintptr_t base) {

    //iowrite32(0x1, base+MDMA_AXLEN_R);
    //iowrite32(0x1, base+MDMA_AXLEN_W);

    //iowrite32(, base+MDMA_ARLEN); // 0xF
    //iowrite32(, base+MDMA_AWLEN); // 0xF

    //iowrite32(0xF, base+MDMA_R_MAX_TRANS);
    //iowrite32(0xF, base+MDMA_W_MAX_TRANS);
}

// transfer data size < 64MB (length[121:96])
void simple_descr_form (struct mdma_descr* p_mdma_descr, void* addr, uint32_t size) {

    p_mdma_descr->own           = 0;
    p_mdma_descr->link          = 0;
    p_mdma_descr->intr          = 0;
    p_mdma_descr->stop          = 1;
    p_mdma_descr->incr          = 0;
    p_mdma_descr->err           = 1;
    p_mdma_descr->cstm_lngth    = size >> 14;       // 121:110 12b msb
    p_mdma_descr->length        = size;             // 109:96 14b lsb
    p_mdma_descr->data_addr     = (uint32_t)addr;   // only for 32b address
    p_mdma_descr->pitch         = 0;
    p_mdma_descr->str_lnth      = 0;
}

int simple_mdma_end_wait(uintptr_t base) {
    char i, j, ret;

    rumboot_printf ("simple_mdma_wait_end\n");

    ret = 0;
    while (((ioread32(base+MDMA_ENABLE_R) & 0x1) != 0x0) ||
        ((ioread32(base+MDMA_ENABLE_W) & 0x1) != 0x0)) {

        if (ret == 7) {
            rumboot_printf ("ERROR: timeout\n");

            ret = 1;
            break;
        }

        for (i=0, j=0; i<63 && j<63; i++, ret++) j = j % i + ret;   // dummy wait
    }

    return ret;
}

int simple_mdma_exec (int heap_id, uintptr_t base, void* addr_src, void* addr_dst, uint32_t data_size) {
    int ret;

    struct mdma_descr* p_mdma_descr_r;
    struct mdma_descr* p_mdma_descr_w;

    p_mdma_descr_r = rumboot_malloc_from_heap_aligned (heap_id, sizeof (struct mdma_descr), 16);
    p_mdma_descr_w = rumboot_malloc_from_heap_aligned (heap_id, sizeof (struct mdma_descr), 16);

    //p_mdma_descr_r = rumboot_malloc_from_heap (heap_id, 16);
    //p_mdma_descr_w = rumboot_malloc_from_heap (heap_id, 16);

    if ((addr_src == NULL) || (addr_dst == NULL) ||
        (p_mdma_descr_r == NULL) || (p_mdma_descr_w == NULL)) {

        ret = 1;
        rumboot_printf ("ERROR: NULL pointer!\n");
    }
    else {
        simple_config_mdma(base);

        simple_descr_form(p_mdma_descr_r, addr_src, data_size);
        simple_descr_form(p_mdma_descr_w, addr_dst, data_size);

        iowrite32((uint32_t)p_mdma_descr_r, base+MDMA_DESC_ADDR_R);
        iowrite32((uint32_t)p_mdma_descr_w, base+MDMA_DESC_ADDR_W);

        iowrite32(0x1, base+MDMA_ENABLE_R);
        iowrite32(0x1, base+MDMA_ENABLE_W);

        ret = simple_mdma_end_wait(base);
    }

    return ret;
}