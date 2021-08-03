#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <regs/regs_gpio_pl061.h>
#include <rumboot/timer.h>

int main() {
	rumboot_printf("hscb_pad_test\n");
    
    uint32_t wait_gpio_1_6  = 0;
    uint32_t value_gpio_1_7 = 0;
    uint32_t past_state_gpio_1_7 = 0;
    
    rumboot_printf("wait GPIO1[6] = 1 \n");
    rumboot_printf("gpio_1_6 = 0x%x \n",wait_gpio_1_6);
    while (wait_gpio_1_6 == 0) {
        wait_gpio_1_6 = ioread32(GPIO_1_BASE+0x3fc) & 0x40;
    }
    iowrite32 (0x0,HSCB0_BASE + 0x0c);
    iowrite32 (0x0,HSCB1_BASE + 0x0c);
    iowrite32 (0x0,HSCB2_BASE + 0x0c);
    iowrite32 (0x0,HSCB3_BASE + 0x0c);
    
	rumboot_printf("en_hscb = 0\n");
    value_gpio_1_7 =  ioread32(GPIO_1_BASE+0x3fc) & 0x80;
	rumboot_printf("value_gpio_1_7 = 0x%x \n",value_gpio_1_7);
    past_state_gpio_1_7 = value_gpio_1_7;
    while (1) {        
        if (value_gpio_1_7) {
            iowrite32 (0x3,HSCB0_BASE + 0x60);
            iowrite32 (0x3,HSCB1_BASE + 0x60);
            iowrite32 (0x3,HSCB2_BASE + 0x60);
            iowrite32 (0x3,HSCB3_BASE + 0x60);
        }
        else {
            iowrite32 (0x0,HSCB0_BASE + 0x60);
            iowrite32 (0x0,HSCB1_BASE + 0x60);
            iowrite32 (0x0,HSCB2_BASE + 0x60);
            iowrite32 (0x0,HSCB3_BASE + 0x60);
        }
        value_gpio_1_7 =  ioread32(GPIO_1_BASE+0x3fc) & 0x80;
        if (value_gpio_1_7 != past_state_gpio_1_7){
            rumboot_printf("value_gpio_1_7 = 0x%x \n",value_gpio_1_7);
        }
        past_state_gpio_1_7 = value_gpio_1_7;
    }
	return 0;
}
