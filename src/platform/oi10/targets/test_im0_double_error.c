/*
 * test_l2c_im0_scrubber_apb.c
 *
 *  Created on: Mar 30, 2021
 *      Author: M.Chelyshev
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/interrupts.h>
#include <rumboot/memfill.h>
#include <rumboot/rumboot.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_itrpt_fields.h>

#include <platform/devices/l2c.h>


#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/regs/regs_scrubber.h>

#include <arch/regs/regs_p64.h>
#include <platform/regs/regs_srammc2plb4.h>
#include <regs/regs_gpio_pl061.h>

static uint32_t im0_ded_accepted = 0;

static void im0_ded_irq_handler() { 
    iowrite32(0xA0,GPIO_0_BASE + 0x3fc);
    iowrite32(0x20,GPIO_0_BASE + 0x3fc);
    rumboot_printf( "\n im0_ded_irq_handler!\n");
    im0_ded_accepted++;
    rumboot_printf( "  im0_ded_accepted = %x\n",im0_ded_accepted);
    ioread32(IM0_BASE + 0x10000);
    l2c_l2_write( DCR_L2C_BASE, L2C_L2CPUSTAT, ( 0b1 << IBM_BIT_INDEX( 32, 28 ) )
                                             | ( 0b1 << IBM_BIT_INDEX( 32, 29 ) )
                                             | ( 0b1 << IBM_BIT_INDEX( 32, 30 ) )
                                             | ( 0b1 << IBM_BIT_INDEX( 32, 31 ) ) );    
}

struct rumboot_irq_entry* create_im0_ded_interrupt_handler() {
    struct rumboot_irq_entry* tbl = NULL;
    
//    RUMBOOT_ATOMIC_BLOCK() {
    rumboot_irq_cli();
    tbl = rumboot_irq_table_get();
    if (tbl == NULL) {
        tbl = rumboot_irq_create( NULL );
    }
    
    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, im0_ded_irq_handler, NULL);

//     Activate the table 
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( L2C0_MCHKOUT);
    rumboot_irq_sei();
//    }
    
    return tbl;
}


int main() {
    rumboot_printf("\nStart of IM0 double errors handling test\n");
    int max_iterations = 1;
    int data_size = 1024;
    
    rumboot_printf("\nAllocate memory\n");
    int heap_id = rumboot_malloc_heap_by_name("IM0");
    uint32_t data_addr = rumboot_malloc_from_heap_aligned(heap_id, data_size, 8);
    rumboot_memfill32((void*)data_addr, data_size/4, 0x02030405, 0x04040404);
    
    // Add zero cells for reset error state at SRAM IM0 controller
    iowrite32(0xa5a5a5a5, IM0_BASE + 0x10000);
    iowrite32(0xa5a5a5a5, IM0_BASE + 0x10004);
    iowrite32(0xa5a5a5a5, IM0_BASE + 0x10008);
    iowrite32(0xa5a5a5a5, IM0_BASE + 0x1000c);
    
    
    // Registered Interrupt handler
    im0_ded_accepted = 0;
    struct rumboot_irq_entry *tbl;
    tbl = create_im0_ded_interrupt_handler();
    
    // Enable parity IM0 check
    rumboot_printf( "Enable parity IM0 check\n" );
    uint32_t tmp = dcr_read(DCR_SRAMMC2PLB4_1_BASE + SRAMMC2PLB4_DPC );
    tmp = tmp | 0x80000000;
    dcr_write( DCR_SRAMMC2PLB4_1_BASE + SRAMMC2PLB4_DPC, tmp);

    // Disable machinecheck for IM0 parity error
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~(0b1 << ITRPT_XSR_ME_i));   // disable machine check
    
    // Enable double errors insertion
    rumboot_printf( "Enable double errors insertion\n");
//    iowrite32(0x20,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to write
    iowrite32(0xff,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to write
    iowrite32(0x20,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 1
    
    int i;
    
    while (im0_ded_accepted < max_iterations){
        ioread32(data_addr+i);
        iowrite32(0x60,GPIO_0_BASE + 0x3fc);
        iowrite32(0x20,GPIO_0_BASE + 0x3fc);
        
//        rumboot_printf("%x: %x\n", data_addr+i, ioread32(data_addr+i));
        if (i == data_size/4) {
            rumboot_printf("\nIM0 double errors handling test FAILD!\n");
            rumboot_printf("    No one double event detected!\n");
            // Disable double errors insertion
            iowrite32(0x00,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 0
            iowrite32(0x00,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to read            
            return 1;
        }
    }
    
//    for (i=0; i<data_size/4; i+=4) {
////        ioread32(data_addr+i);
//        rumboot_printf("%x: %x\n", data_addr+i, ioread32(data_addr+i));
//    }
    
    // Disable double errors insertion
    iowrite32(0x00,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 0
    iowrite32(0x00,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to read


    
    rumboot_printf("\nIM0 double errors handling test OK!\n");
    
    return 0;
}
