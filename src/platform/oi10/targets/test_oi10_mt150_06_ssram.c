#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <arch/ppc_476fp_config.h>
#include <arch/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>

#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_emi.h>

int main(void)
{   
    rumboot_printf("start!\n");
    uint32_t test_result = 0;

    memset((uint8_t*)SRAM0_BASE, 0x00, 0x1000);

    emi_init(DCR_EM2_EMI_BASE);


    //Unmask EMI memory error interrupts
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0xFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND0, 0x00000000);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND1, 0x0100003F);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_MSK_AND, 0x3FFFFFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION0, 0x00000420);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION1, 0x00000000);

    uint32_t read_data;
    uint32_t read_addr;
    
    rumboot_printf("Read emi ss2 \n");
    read_addr = DCR_EM2_EMI_BASE + 0x10;
    rumboot_printf("read_data(0x%x) = 0x%x\n", read_addr,dcr_read(read_addr));
    rumboot_printf("------------------------------------------------------------------------ \n");   
    
    dcr_write(read_addr, 0x1282);       //SSRAM - FLOW-THROUGH
    rumboot_printf("read_data(0x%x) = 0x%x\n", read_addr,dcr_read(read_addr));
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test DATA line\n");
    rumboot_printf("WRITE SSRAM\n");
    iowrite32(0x55555555, SSRAM_BASE);
    iowrite32(0xaaaaaaaa, SSRAM_BASE + 4);
    rumboot_printf("READ SSRAM\n");
    read_data = ioread32(SSRAM_BASE);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE,read_data);
    if (read_data != 0x55555555) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE,read_data);
        rumboot_printf("wait data      = 0x%x \n",0x55555555);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    read_data = ioread32(SSRAM_BASE + 4);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE + 4,read_data);
    if (read_data != 0xaaaaaaaa) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE + 4,read_data);
        rumboot_printf("wait data      = 0x%x \n",0xaaaaaaaa);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test all mem\n");
    rumboot_printf("WRITE SSRAM\n");
    for (int i=0; i<(0x1<<16); i++) {
        iowrite32(i, SSRAM_BASE+(i*4));
    }        
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("READ SSRAM\n");
    for (int i=0; i<(0x1<<16); i++) {
        read_data = ioread32(SSRAM_BASE+(i*4));
//        rumboot_printf("read_data(0x%x) = 0x%x\n", SSRAM_BASE+(i*4),read_data);
        if (read_data != i) {
            rumboot_printf("--------------------------------\n");
            rumboot_printf("ERROR DATA !!!\n");
            rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE + i*4,read_data);
            rumboot_printf("wait data      = 0x%x \n",i);
            rumboot_printf("--------------------------------\n");
            test_result = 1;
            return 1;
        }
        else if ((i & 0x1fff) == 0x1fff) {
            rumboot_printf("ioread32(0x%x) = 0x%x \n",SSRAM_BASE + i*4,read_data);
        }
    }
     
    if (!test_result)
        rumboot_putstring("Test has been finished successfully.\n");

    return test_result;
}
