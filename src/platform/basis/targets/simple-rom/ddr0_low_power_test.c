
//-----------------------------------------------------------------------------
//  This program is for checking DDR0 low power features.
//    It also check interruptions.
//  
//  Test includes:
//    - DDR0 turning On function
//    
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/gic.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>

#define irq_cntr_ptr 0x00050000
#define irq_flag_ptr 0x00050008

static void DDR0_IRQ_handler (int irq, void *arg)
{
	rumboot_printf("DDR IRQ happened \n");
    iowrite32 (ioread32 (irq_cntr_ptr) + 1, irq_cntr_ptr);
    iowrite32 (1, irq_flag_ptr);
    iowrite32 (0xFFFFFFFF, DDR0_BASE + DENALI_CTL_95);
}

uint32_t low_power_test (uint32_t base_address)
{
    //    Active power-down
    rumboot_printf ("    Active power-down \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x02000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Pre-charge power-down
    rumboot_printf ("    Pre-charge power-down \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x06000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Self-refresh
    rumboot_printf ("    Self-refresh \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x0A000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Active power-down + Memory CG
    rumboot_printf ("    Active power-down + Memory CG \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x22000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Pre-charge power-down + Memory CG
    rumboot_printf ("    Pre-charge power-down + Memory CG \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x26000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Self-refresh + Memory CG
    rumboot_printf ("    Self-refresh + Memory CG \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x2A000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    //    Self-refresh + Memory CG
    rumboot_printf ("    Self-refresh + Memory CG + Controller CG \n");
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x6A000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    rgDENALI_CTL_22 = rgDENALI_CTL_22 | 0x01000000;
    while (ioread32 (irq_flag_ptr) != 1)
    {
    }
    iowrite32 (0, irq_flag_ptr) ;
    
    return 0;
}

int main ()
{
    iowrite64 (0, irq_cntr_ptr) ;
    iowrite64 (0, irq_flag_ptr) ;
    
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    
    //---------------------------------------------------------------------
    //  GIC configuration
    //---------------------------------------------------------------------
	rumboot_irq_cli();
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, (32 + 55), 0, DDR0_IRQ_handler, NULL);
	/* Activate the table */
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable((32 + 55));
	rumboot_irq_sei();
    //---------------------------------------------------------------------
    
    if (low_power_test (DDR0_BASE) != 0)
        return -2;
    
    return 0;
}

