
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_sp805.h>
#include <devices/sp805.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

void sp805_enable(uint32_t base_addr)
{
    int cntrl;
    int control_reg;
    control_reg = WD_REG_CONTROL;
    cntrl = dcr_read(base_addr + control_reg);
    cntrl |= WD_CTRL_INTEN;
    dcr_write(base_addr + control_reg, cntrl);
 }

void sp805_stop(uint32_t base_addr)
{
    int cntrl;
    int control_reg;
    control_reg = WD_REG_CONTROL;

    cntrl = dcr_read(base_addr + control_reg);
    cntrl = cntrl & (~( WD_CTRL_RESEN));
    dcr_write(base_addr + control_reg, cntrl);
}

int sp805_get_value(uint32_t base_addr)
{
    return dcr_read(base_addr + WD_REG_VALUE);
}

void sp805_clrint(uint32_t base_addr)
{
    dcr_write(base_addr + WD_REG_INTCLR, 1);
}

void sp805_config(uint32_t base_addr, const struct sp805_conf * config)
{
    int cntrl = 0;
    // INT EN
    if(config->interrupt_enable)
    {
        cntrl |= WD_CTRL_INTEN;
    }
    else
    {
        cntrl &= ~WD_CTRL_INTEN;
    }
    // enable/disable watch
    if(config->load)
    {
        dcr_write(base_addr + WD_REG_LOAD, config->load);
    }
}


uint32_t sp805_get_itcr(uint32_t base_addr)
{
    return dcr_read(base_addr + WD_REG_ITCR);
}

void sp805_write_to_itcr(uint32_t base_addr, uint32_t value)
{
    dcr_write(base_addr + WD_REG_ITCR, value);
}

void sp805_write_to_itop(uint32_t base_addr, uint32_t value)
{
    dcr_write(base_addr + WD_REG_ITOP, value);
}

void sp805_unlock_access(uint32_t base_addr)
{
    dcr_write(base_addr + WD_REG_LOCK, 0x1ACCE551);
}

void sp805_disable(uint32_t base_addr)
{
    dcr_write(base_addr + WD_REG_CONTROL, 0);
}

