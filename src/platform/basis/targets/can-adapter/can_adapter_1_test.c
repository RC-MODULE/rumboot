#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can_adapter.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h> 
#include <platform/interrupts.h>

/*struct adapter_instance{
      int adapter_index;
    };*/
    
/*static struct adapter_instance in[] = {
    {
        .adapter_index = 0
    },
    {
        .adapter_index = 1
    }
};*/

static void handler0(int irq, void *arg)
{
	volatile uint32_t *done = arg;
    rumboot_printf("IRQ 0 arrived, arg %x, count %d\n", arg, (*done));
	ioread32(CAN0_BASE + INTRP_FLAG_REG);
    *done = *done + 1;	
}

static void handler1(int irq, void *arg)
{
	volatile uint32_t *done = arg;
	rumboot_printf("IRQ 1 arrived, arg %x, count %d\n", arg, (*done));
    ioread32(CAN1_BASE + INTRP_FLAG_REG);	
    *done = *done + 1;	
}

int main()
{
    rumboot_printf("This is can_adapter_1_test! It checks interrupt connection.\n");
    volatile uint32_t done = 0;
    // Disable all interrupts
	rumboot_irq_cli(); 
    
    // Create an IRQ table
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

    // Set handler
    rumboot_irq_set_handler(tbl, CAN0_INT, 0, handler0, (void*)&done);
    rumboot_irq_set_handler(tbl, CAN1_INT, 0, handler1, (void*)&done);
    
    // Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(CAN0_INT);
	rumboot_irq_enable(CAN1_INT);
    
    // Allow interrupt handling
    rumboot_irq_sei(); 
    
    
    // TEST
    unsigned int read_data;
	//Interrupt enable reg init
    iowrite32(1 <<   GBIE | 1 <<   BIIE, CAN0_BASE + INTRP_EN_REG); // Bus idle interrupt enable for CAN0
    iowrite32(1 <<   GBIE | 1 <<   BIIE, CAN1_BASE + INTRP_EN_REG); // Bus idle interrupt enable for CAN1
    
    //Enable change TR Timing Register
    iowrite32(0 << TME |  1 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  1 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    
    // Write and check TR Timing Register value
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN0_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz) 
    read_data=ioread32(CAN0_BASE + TR_TIMING_REG);
    while (read_data ^ (1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV))
     {read_data=ioread32(CAN0_BASE + TR_TIMING_REG);}
    rumboot_printf("CAN0 TR timing is correct!\n");    
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN1_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz)
    read_data=ioread32(CAN1_BASE + TR_TIMING_REG);
    while (read_data ^ (1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV))
     {read_data=ioread32(CAN1_BASE + TR_TIMING_REG);}
    rumboot_printf("CAN1 TR timing is correct!\n");

    //Disable change TR Timing Register
    iowrite32(0 << TME |  0 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  0 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    
    // Init mode OFF
    iowrite32(2 << IMCR, CAN0_BASE + IMCR_REG); 
    //iowrite32(2 << IMCR, CAN1_BASE + IMCR_REG);
    
    //Wait Bus Idle
    read_data=ioread32(CAN0_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN0_BASE + STATUS_REG);}
    rumboot_printf("CAN0 Bus Idle!\n");
    
    iowrite32(2 << IMCR, CAN1_BASE + IMCR_REG);
    
    read_data=ioread32(CAN1_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN1_BASE + STATUS_REG);}
    rumboot_printf("CAN1 Bus Idle!\n");
    
    rumboot_printf("done = %d\n", done);    
    // End of TEST
    
    // Deinit
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);
    
    if (done==2)
    {
        rumboot_printf("TEST OK!\n");
	    return 0;
        }
    else{
        rumboot_printf("TEST FAIL!\n");
        return -1;
    }
     
}
