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
#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>

#include <platform/regs/regs_mclfir.h>
#include <regs/regs_emi.h>

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

    rumboot_putstring("WRITE SDRAM\n");
    
    uint32_t read_data;
    
    rumboot_printf("Read emi sd1 \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0xc);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0xc,read_data);
    rumboot_printf("Read emi rfc \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x30);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x30,read_data);
    rumboot_printf("Read EMI_HSTSR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x34);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x34,read_data);
    rumboot_printf("Read EMI_WECR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x58);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x58,read_data);
    rumboot_printf("------------------- \n");
    rumboot_printf("Write emi sd1 \n");
    dcr_write((DCR_EM2_EMI_BASE + 0xc), 0x9622);
    rumboot_printf("Read emi sd1 \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0xc);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0xc,read_data);
    
    rumboot_printf("Write EMI_HSTSR \n");
    dcr_write((DCR_EM2_EMI_BASE + 0x34), 0x2);
    rumboot_printf("Read EMI_HSTSR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x34);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x34,read_data);
    
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test DATA line\n");
    rumboot_printf("WRITE SSRAM\n");
    iowrite32(0x55555555, SDRAM_BASE);
    iowrite32(0xaaaaaaaa, SDRAM_BASE + 4);
    rumboot_printf("READ SSRAM\n");
    read_data = ioread32(SDRAM_BASE);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SDRAM_BASE,read_data);
    if (read_data != 0x55555555) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SDRAM_BASE,read_data);
        rumboot_printf("wait data      = 0x%x \n",0x55555555);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    read_data = ioread32(SDRAM_BASE + 4);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SDRAM_BASE + 4,read_data);
    if (read_data != 0xaaaaaaaa) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SDRAM_BASE + 4,read_data);
        rumboot_printf("wait data      = 0x%x \n",0xaaaaaaaa);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test all mem\n");
    rumboot_printf("Write all mem  \n");
    for (int i=0; i<8388608; i++) {
        iowrite32(i*4, SDRAM_BASE+(i*4));
    }
    
    rumboot_printf("Read all mem  \n");
    for (int i=0; i<8388608; i++) {
        read_data = ioread32(SDRAM_BASE+(i*4));
//        rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
        if (read_data != i*4) {
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
            rumboot_printf("error data! \n");
            test_result = 1;
            break;
        }
        if ((i&0x00fffff) == 0xfffff) {
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
        }
    }

    if (!test_result)
        rumboot_putstring("Test has been finished successfully. \n");

    return test_result;
}
