
//-----------------------------------------------------------------------------
//  This program is for checking DDR1 low power features.
//    It also check interruptions.
//
//  Test includes:
//    - DDR1 turning On function
//    - configure GIC
//    - repeat few times
//      - turn On, turn Off Active power-down
//      - turn On, turn Off Pre-charge power-down
//      - turn On, turn Off Self-refresh
//      - turn On, turn Off Active power-down + Memory CG
//      - turn On, turn Off Pre-charge power-down + Memory CG
//      - turn On, turn Off Self-refresh + Memory CG
//      - turn On, turn Off Self-refresh + Memory CG + Controller CG
//
//    - Interruption occur every time power state change
//
//    Test duration (RTL): <
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/sp804.h>

#define irq_cntr_ptr 0x00050000
#define irq_flag_ptr 0x00050008

#define ddr1_int 88

#define repeat_number 5

static void DDR1_IRQ_handler (int irq, void *arg)
{
    rumboot_printf("DDR1 IRQ happened \n");
    iowrite32 (ioread32 (irq_cntr_ptr) + 1, irq_cntr_ptr);
    iowrite32 (1, irq_flag_ptr);
    iowrite32 (0xFFFFFFFF, DDR1_BASE + DENALI_CTL_95);
}

uint32_t low_power_test (uint32_t base_address)
{
    for (uint32_t i = 0; i < repeat_number; i++)
    {
        //    Active power-down
        rumboot_printf ("    Active power-down \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x02000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Pre-charge power-down
        rumboot_printf ("    Pre-charge power-down \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x06000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Self-refresh
        rumboot_printf ("    Self-refresh \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x0A000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Active power-down + Memory CG
        rumboot_printf ("    Active power-down + Memory CG \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x22000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Pre-charge power-down + Memory CG
        rumboot_printf ("    Pre-charge power-down + Memory CG \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x26000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Self-refresh + Memory CG
        rumboot_printf ("    Self-refresh + Memory CG \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x2A000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;

        //    Self-refresh + Memory CG
        rumboot_printf ("    Self-refresh + Memory CG + Controller CG \n");
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x6A000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        iowrite32 (ioread32 (base_address + DENALI_CTL_22) | 0x01000000, base_address + DENALI_CTL_22);
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
    }

    if (ioread32 (irq_cntr_ptr) != (repeat_number * 14))
    {
        return ioread32 (irq_cntr_ptr);
    }

    return 0;
}

int main ()
{
    iowrite64 (0, irq_cntr_ptr) ;
    iowrite64 (0, irq_flag_ptr) ;

    if (ddr_init (DDR1_BASE) != 0)
        return -1;

    //---------------------------------------------------------------------
    //  GIC configuration
    //---------------------------------------------------------------------
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, ddr1_int, 0, DDR1_IRQ_handler, NULL);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(ddr1_int);
    rumboot_irq_sei();
    //---------------------------------------------------------------------

    if (low_power_test (DDR1_BASE) != 0)
        return -2;

    return 0;
}
