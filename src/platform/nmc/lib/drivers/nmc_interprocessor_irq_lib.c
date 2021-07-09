#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <regs/regs_interprocessor_irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <devices/interprocessor_irq.h>


void interprocessor_irq_clear_interrupt(uint32_t base_addr, uint32_t irq_id){
    uint32_t old_value = ioread32(base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
    uint32_t new_value;
    new_value = old_value | (1<<irq_id);
    iowrite32(new_value, base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
}

void interprocessor_irq_set_interrupt(uint32_t base_addr, uint32_t irq_id){
    uint32_t old_value = ioread32(base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
    uint32_t new_value;
    new_value = old_value | (1<<(irq_id+16));
    iowrite32(new_value, base_addr+INTERPROCESSOR_IRQ_REG_SYNC);
}


void interprocessor_irq_set_irq_confirmation(uint32_t base_addr, uint32_t irq_conf_value){
    iowrite32(irq_conf_value, base_addr+INTERPROCESSOR_IRQ_REG_GP);
}

uint32_t interprocessor_irq_get_irq_confirmation(uint32_t base_addr){
    uint32_t old_value = ioread32(base_addr+INTERPROCESSOR_IRQ_REG_GP);
    return old_value;

}

void interprocessor_irq_clear_irq_confirmation(uint32_t base_addr){
    iowrite32(0, base_addr+INTERPROCESSOR_IRQ_REG_GP);
}