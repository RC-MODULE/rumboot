#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/regs/regs_scrb.h>
#include <platform/devices/scrb.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>



void scrb_set_timer(uint32_t base_addr, uint32_t timer_value)
{
    iowrite32(timer_value, base_addr+SCRB_REG_TM);
}


void scrb_set_allcnt(uint32_t base_addr, uint32_t allcnt_value)
{
    iowrite32(allcnt_value, base_addr+SCRB_REG_ALLCNT);
}


void scrb_set_fref(uint32_t base_addr, uint32_t fref_value)
{
    iowrite32(fref_value, base_addr+SCRB_REG_FREF);
}


void scrb_set_imr(uint32_t base_addr, uint32_t imr_value)
{
    iowrite32(imr_value, base_addr+SCRB_REG_IMR);
}

void scrb_set_work_mode(uint32_t base_addr, uint32_t mode_value)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_CR);
    // Clean mode bit in old value
    old_value = old_value & (~(1<<2));
    uint32_t new_value = old_value|mode_value<<2;
    iowrite32(new_value, base_addr+SCRB_REG_CR);
}

void scrb_set_priority(uint32_t base_addr, uint32_t priority_value)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_CR);
    // Clean priority bit in old value
    old_value = old_value & (~(1<<3));
    uint32_t new_value = old_value|priority_value<<3;
    iowrite32(new_value, base_addr+SCRB_REG_CR);
}


void scrb_start_scrubber(uint32_t base_addr, uint32_t start_im2_im1_bits)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_CR);
    //priority_bit = priority_value<<3
    // Clean start bits in old value
    old_value = old_value & (~(3));
    uint32_t new_value = old_value|start_im2_im1_bits;
    iowrite32(new_value, base_addr+SCRB_REG_CR);
}


void scrb_stop_scrubbing_im1(uint32_t base_addr)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_CR);
    old_value = old_value & (~(1));
    iowrite32(old_value, base_addr+SCRB_REG_CR);
}

void scrb_stop_scrubbing_im2(uint32_t base_addr)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_CR);
    old_value = old_value & (~(2));
    iowrite32(old_value, base_addr+SCRB_REG_CR);
}

void scrb_clear_interrupt_0(uint32_t base_addr)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_IRR);
    old_value = old_value & 0x2;
    iowrite32(old_value, base_addr+SCRB_REG_IRR);
}


void scrb_clear_interrupt_1(uint32_t base_addr)
{
    uint32_t old_value = ioread32(base_addr+SCRB_REG_IRR);
    old_value = old_value & 0x1;
    iowrite32(old_value, base_addr+SCRB_REG_IRR);
}

int scrb_get_number_im1_errors(uint32_t base_addr)
{
    return (ioread32(base_addr+SCRB_REG_FCNT)<<16)>>16;
}


int scrb_get_number_im2_errors(uint32_t base_addr)
{
    return ioread32(base_addr+SCRB_REG_FCNT)>>16;

}


int scrb_get_number_total_errors_left_in_reg(uint32_t base_addr)
{
    // Value in register is inverted
    return ioread32(base_addr+SCRB_REG_ALLCNT);
}

uint32_t scrb_scrubbing_is_finished(uint32_t base_addr, int im1_enable, int im2_enable)
{
    uint32_t is_finished = 0x0;
    if(im1_enable>0){
        if (scrb_get_number_im1_errors(base_addr)>0){
            is_finished=is_finished|0x1;
        }
    }
    if(im2_enable>0){
        if (scrb_get_number_im2_errors(base_addr)>0){
            is_finished=is_finished|0x1;
        }
    }
    return is_finished;
}