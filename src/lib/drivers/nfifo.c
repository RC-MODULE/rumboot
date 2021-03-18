#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <devices/nfifo.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>

#ifndef __PPC__
inline uint32_t dcr_read(uintptr_t addr) {
        return 0;
}
inline void dcr_write(uintptr_t addr, uint32_t data) {
}

#else
#include <arch/ppc_476fp_lib_c.h>
#endif

#define NFIFO_DATA    0
#define NFIFO_CONTROL 1


void nfifo_instance_init(struct nfifo_instance *inst, uintptr_t base)
{
    inst->base = base;
    #ifdef __PPC__
        inst->dcr = 1;
    #else
        inst->dcr = 0;
    #endif
    #ifdef __NM__
        inst->spacing = 8;
    #else
        inst->spacing = 4;
    #endif
}


static uint32_t nfifo_reg_read(struct nfifo_instance *inst, uintptr_t offset)
{
        uint32_t v = inst->dcr ? dcr_read(inst->base + (offset * inst->spacing)) : ioread32(inst->base + offset * inst->spacing);
        return v;
}

static void nfifo_reg_write(struct nfifo_instance *inst, uintptr_t offset, uint32_t value)
{
    if (inst->dcr) {
        dcr_write(inst->base + (offset * inst->spacing), value);
     } else {
        iowrite32(value, inst->base + (offset * inst->spacing));
     }
}

void nfifo_enable(struct nfifo_instance *inst, int enable)
{
        uint32_t status = nfifo_reg_read(inst,  NFIFO_CONTROL);
        nfifo_reg_write(inst, NFIFO_CONTROL, enable ? (status | (1<<0)) : (status & ~(1<<0)));
}

int nfifo_can_read(struct nfifo_instance *inst)
{
    return nfifo_reg_read(inst, NFIFO_CONTROL) & (1<<2);
}

int nfifo_can_write(struct nfifo_instance *inst)
{
    return nfifo_reg_read(inst, NFIFO_CONTROL) & (1<<1);
}

uint32_t nfifo_read(struct nfifo_instance *inst, uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();

        do {
                if (nfifo_can_read(inst)) {
                        return nfifo_reg_read(inst, NFIFO_DATA);
                }
        } while (rumboot_platform_get_uptime() - start < timeout_us);
        return -1;
}

void nfifo_write(struct nfifo_instance *inst, uint32_t word)
{
    while (!nfifo_can_write(inst));;
    nfifo_reg_write(inst, NFIFO_DATA, word);
}

int nfifo_read_status(struct nfifo_instance *inst)
{
        uint32_t status = nfifo_reg_read(inst, NFIFO_CONTROL);
        rumboot_printf("ctrl = 0x%x\n", status);
        return (!(status & (1 << 3)));
}