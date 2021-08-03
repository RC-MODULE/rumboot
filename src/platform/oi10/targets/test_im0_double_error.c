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

volatile static bool im0_ded_accepted;

static void im0_ded_irq_handler() { 
    rumboot_printf( "\n im0_ded_irq_handler!\n");
    im0_ded_accepted = 1;
    // Clear C470S_DBGMACHINECHECK
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
    // Clear L2C interrupt???
    l2c_l2_write( DCR_L2C_BASE, L2C_L2PLBSTAT1, 0xFFFFFFFF );
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

void wait_irq_or_pass_through(uint32_t timeout) {
    uint32_t i = 0;
    for (i=0; i<timeout; i++) {
        if (im0_ded_accepted) break;
    }
//    rumboot_printf("       end of wait_irq_or_pass_through\n");
}

int main() {
    rumboot_printf("\nStart of IM0 double errors handling test\n");
    int max_iterations = 16;
    int data_size = 4096;
    int i = 0;
    
    rumboot_printf("\nAllocate memory\n");
    uint32_t data_addr = IM0_BASE + 0x10000 - data_size;

    uint32_t tmp_data = 0x00010203;
    for (i=0; i<data_size/4; i+=4) {
        iowrite32(tmp_data, data_addr + i);
        tmp_data = tmp_data + 0x04040404;
    }

    // Registered Interrupt handler
    uint32_t im0_ded_cnt = 0;
    struct rumboot_irq_entry *tbl;
    tbl = create_im0_ded_interrupt_handler();

    // Disable machinecheck for IM0 parity error
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~(0b1 << ITRPT_XSR_ME_i));   // disable machine check
    
    // Enable parity IM0 check
    rumboot_printf( "Enable parity IM0 check\n" );
    uint32_t tmp = dcr_read(DCR_SRAMMC2PLB4_1_BASE + SRAMMC2PLB4_DPC );
    tmp = tmp | 0x80000000;
    dcr_write( DCR_SRAMMC2PLB4_1_BASE + SRAMMC2PLB4_DPC, tmp);

    // Enable double errors insertion
    rumboot_printf( "Enable double errors insertion\n");
    iowrite32(0x20,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to write
    iowrite32(0x20,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 1
    
    i = 0;
    im0_ded_accepted = 0;
    rumboot_printf("Start of cycle\n");
    while (im0_ded_cnt < max_iterations){
        tmp = ioread32(data_addr+i);
        wait_irq_or_pass_through(10);
//        rumboot_printf("  %x: %x\n",data_addr+i, tmp);
        if (im0_ded_accepted) {
            im0_ded_accepted = 0;
            im0_ded_cnt++;
            rumboot_printf("     im0_ded_cnt = %d\n", im0_ded_cnt);
        }
        
        if (i == data_size/4) {
            rumboot_printf("\nIM0 double errors handling test FAILD!\n");
            rumboot_printf("    No one double event detected!\n");
            // Disable double errors insertion
            iowrite32(0x00,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 0
            iowrite32(0x00,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to read            
//            delete_irq_handlers(tbl);
            return 1;
        }
        i+=4;
    }
    
    // Disable double errors insertion
    iowrite32(0x00,GPIO_0_BASE + 0x3fc);    // GPIO0_5 set to 0
    iowrite32(0x00,GPIO_0_BASE + GPIO_DIR); // GPIO0_5 switch to read
    
//    delete_irq_handlers(tbl);
    
    rumboot_printf("\nIM0 double errors handling test OK!\n");
    
    return 0;
}
