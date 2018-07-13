
//-----------------------------------------------------------------------------
//  This file contains MKIO (Gaisler gr1553b) related functions 
//  It includes next functions:
//    - mkio present
//    - report mkio bc status
//    - report mkio rt status
//    - report mkio bm status
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/devices.h>

#include <regs/regs_mkio.h>

#include <devices/mkio.h>

uint32_t mkio_present  (uint32_t base_address)
{
    if (
            (ioread32 (base_address + HWCFG) != 0x00000000) |
            ((ioread32 (base_address + BCSL) & 0x80000000) != 0x80000000) |
            ((ioread32 (base_address + RTS ) & 0x80000000) != 0x80000000) |
            ((ioread32 (base_address + BMS ) & 0x80000000) != 0x80000000)
        )
        return -1;
    rumboot_printf("    mkio_present.OK %x\n", base_address);
    return 0;
}

void mkio_report_bc_status (uint32_t base_address)
{
    uint32_t rdata;
    rdata = ioread32 (base_address + BCSL);
    rumboot_printf("MKIO_BC BCSL reg value: %x\n", rdata);
    rdata = ioread32 (base_address + IRQ);
    rumboot_printf("MKIO_BC IRQ reg value: %x\n", rdata);
}

void mkio_report_rt_status (uint32_t base_address)
{
    uint32_t rdata;
    rdata = ioread32 (base_address + RTS);
    rumboot_printf("MKIO_RT RTS reg value: %x\n", rdata);
    rdata = ioread32 (base_address + IRQ);
    rumboot_printf("MKIO_RT IRQ reg value: %x\n", rdata);
}
