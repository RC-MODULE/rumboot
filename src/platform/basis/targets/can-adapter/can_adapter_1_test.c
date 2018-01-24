#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can_adapter.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h> 


int main()
{
    unsigned int read_data;
	rumboot_printf("This is can_adapter_1_test! It checks interrupt connection.\n");

    //Interrupt enable reg init
    iowrite32(1 <<   GBIE | 1 <<   BIIE, CAN0_BASE + INTRP_EN_REG); // Bus idle interrupt enable for CAN0
    iowrite32(1 <<   GBIE | 1 <<   BIIE, CAN1_BASE + INTRP_EN_REG); // Bus idle interrupt enable for CAN1
    
    //TR Timing Register
    iowrite32(0 << TME |  1 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  1 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN0_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz)    
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN1_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz)
    iowrite32(0 << TME |  0 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  0 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    
    // Init mode OFF
    iowrite32(2 << IMCR, CAN0_BASE + IMCR_REG); 
    iowrite32(2 << IMCR, CAN1_BASE + IMCR_REG);
    
    //Wait Bus Idle
    read_data=ioread32(CAN0_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN0_BASE + STATUS_REG);}
    rumboot_printf("CAN0 Bus Idle!\n");
    
    read_data=ioread32(CAN1_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN1_BASE + STATUS_REG);}
    rumboot_printf("CAN1 Bus Idle!\n");
    
    
    rumboot_printf("Goodbye from can_adapter_1_test!\n");
	return 0;
}
