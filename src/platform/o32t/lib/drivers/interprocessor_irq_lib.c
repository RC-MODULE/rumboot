#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <regs/regs_interprocessor_irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <devices/interprocessor_irq.h>


void interprocessor_irq_clear_interrupt(uint32_t base_addr, uint32_t irq_id){
    uint32_t old_value = dcr_read(base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
    uint32_t new_value;
    new_value = old_value | (1<<irq_id);
    dcr_write(base_addr+INTERPROCESSOR_IRQ_REG_SYNC, new_value);
}

void interprocessor_irq_set_interrupt(uint32_t base_addr, uint32_t irq_id){
    uint32_t old_value = dcr_read(base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
    uint32_t new_value;
    new_value = old_value | (1<<(irq_id+16));
    dcr_write(base_addr+INTERPROCESSOR_IRQ_REG_SYNC, new_value);
}


void interprocessor_irq_set_irq_confirmation(uint32_t base_addr, uint32_t irq_conf_value){
    dcr_write(base_addr+INTERPROCESSOR_IRQ_REG_GP, irq_conf_value);
}

uint32_t interprocessor_irq_get_irq_confirmation(uint32_t base_addr){
    uint32_t old_value = dcr_read(base_addr+INTERPROCESSOR_IRQ_REG_GP);
    return old_value;
}

void interprocessor_irq_clear_irq_confirmation(uint32_t base_addr){
    dcr_write(base_addr+INTERPROCESSOR_IRQ_REG_GP, 0);
}