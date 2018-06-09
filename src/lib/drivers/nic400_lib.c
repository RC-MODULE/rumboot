#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_nic400.h>
#include <devices/nic400.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>



int nic400_get_pidr4( )
{
    return ioread32(NIC400_BASE+NIC400_PIDR4);
}

int nic400_get_pidr5()
{
    return ioread32(NIC400_BASE+NIC400_PIDR5);
}

int nic400_get_pidr6()
{
    return ioread32(NIC400_BASE+NIC400_PIDR6);
}

int nic400_get_pidr7()
{
    return ioread32(NIC400_BASE+NIC400_PIDR7);
}

int nic400_get_pidr0()
{
    return ioread32(NIC400_BASE+NIC400_PIDR0);
}

int nic400_get_pidr1()
{
    return ioread32(NIC400_BASE+NIC400_PIDR1);
}

int nic400_get_pidr2()
{
    return ioread32(NIC400_BASE+NIC400_PIDR2);
}

int nic400_get_pidr3()
{
    return ioread32(NIC400_BASE+NIC400_PIDR3);
}

int nic400_get_cidr0()
{
    return ioread32(NIC400_BASE+NIC400_CIDR0);
}

int nic400_get_cidr1()
{
    return ioread32(NIC400_BASE+NIC400_CIDR1);
}

int nic400_get_cidr2()
{
    return ioread32(NIC400_BASE+NIC400_CIDR2);
}

int nic400_get_cidr3()
{
    return ioread32(NIC400_BASE+NIC400_CIDR3);
}





int nic400_read_reg(int offset)
{
    return ioread32(NIC400_BASE+offset);
}


void nic400_write_reg(int a, int offset)
{
    iowrite32( a, NIC400_BASE+offset);
}




int nic400_check_rqos(int interface_block )
{
    return ioread32(NIC400_BASE+interface_block+RQOS_OFFSET);
}

int nic400_check_wqos(int interface_block )
{
    return ioread32(NIC400_BASE+interface_block+WQOS_OFFSET);
}

int nic400_check_fn_mod(int interface_block )
{
    return ioread32(NIC400_BASE+interface_block+FN_MOD_OFFSET);
}


void nic400_set_rqos(int a, int interface_block )
{
    iowrite32(a, NIC400_BASE+interface_block+RQOS_OFFSET);
}

void nic400_set_wqos(int a, int interface_block )
{
    iowrite32(a, NIC400_BASE+interface_block+WQOS_OFFSET);
}

void nic400_set_fn_mod(int a, int interface_block )
{
    iowrite32(a, NIC400_BASE+interface_block+FN_MOD_OFFSET);
}

