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

#define NOR_SELFCHECKING_DISABLE
#include <platform/devices/nor_1636RR4.h>
#undef NOR_SELFCHECKING_DISABLE

#include <platform/regs/regs_mclfir.h>
#include <regs/regs_emi.h>

#include <regs/regs_gpio_pl061.h>

#define ADDR_SRAM0_SE                           SRAM0_BASE + 0x10000
#define ADDR_SRAM0_DE                           SRAM0_BASE + 0x10100

int main(void)
{
    rumboot_printf("start!\n");
    uint32_t test_result = 0;
    
/*  
    rumboot_printf( "Init PLL \n");    
    dcr_write(0x8008003c,0x1ACCE551);

//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));
//  
//    dcr_write(0x80080004,0x11);
//    
//    dcr_write(0x80080018,0x7);
//    mdelay(500);
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));
//    
//    dcr_write(0x80080004,0x00);
//    
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));

    
    uint32_t addr;
    rumboot_printf( "READ INIT VALUE \n");
    for (int i=0; i<16; i++) {  
        rumboot_printf( "--- 0x%x --- \n",i);
        addr = 0x80080100 + i*0x10;
        rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
        addr = 0x80080104 + i*0x10;
        rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    }

//    rumboot_printf( "WRITE CKDIVMODE 0\n");
//    addr = 0x80080100;
//    dcr_write(addr,0xf);
//    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    
    
    
    for (int i=0; i<16; i++) {  
        rumboot_printf( "--- 0x%x --- \n",i);
        addr = 0x80080100 + i*0x10;
        switch (i) {
            case 0:     dcr_write(addr,0xf); break;
            case 1:     dcr_write(addr,0xf); break;
            case 2:     dcr_write(addr,0x1); break;
            case 3:     dcr_write(addr,0xf); break;
            case 4:     dcr_write(addr,0x9); break;
            default :   dcr_write(addr,0xf);
        }
        rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    }
    
    rumboot_printf( "EN CKDIVMODE\n");
    addr = 0x80080060;
    dcr_write(addr,0x1);
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    
    iowrite32 (0xff,GPIO_0_BASE+GPIO_DIR);
    for (int i=0; i<16; i++) {  
        mdelay(62.5);
        rumboot_printf( "delay 0x%x \n",i);
        if (i%2 == 0)
            iowrite32 (0x00,GPIO_0_BASE+0x3fc);
        else
            iowrite32 (0xff,GPIO_0_BASE+0x3fc);
    }
        
    rumboot_printf( "WRITE CKDIVMODE 2\n");
    addr = 0x80080120;
    dcr_write(addr,0x1f);
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    rumboot_printf( "EN CKDIVMODE\n");
    addr = 0x80080060;
    dcr_write(addr,0x1);
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",addr,dcr_read(addr));
    iowrite32 (0xff,GPIO_0_BASE+GPIO_DIR);
    
    iowrite32 (0xff,GPIO_0_BASE+GPIO_DIR);
    for (int i=0; i<16; i++) {  
        mdelay(62.5);
        rumboot_printf( "delay 0x%x \n",i);
        if (i%2 == 0)
            iowrite32 (0x00,GPIO_0_BASE+0x3fc);
        else
            iowrite32 (0xff,GPIO_0_BASE+0x3fc);
    }    
*/        

    emi_init(DCR_EM2_EMI_BASE);
    //Unmask EMI memory error interrupts
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0xFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND0, 0x00000000);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND1, 0x0100003F);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_MSK_AND, 0x3FFFFFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION0, 0x00000420);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION1, 0x00000000);

    rumboot_printf("Read emi ss0 \n");
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE,dcr_read(DCR_EM2_EMI_BASE));
    dcr_write(DCR_EM2_EMI_BASE, 0x1c00);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE,dcr_read(DCR_EM2_EMI_BASE));
    
    rumboot_printf("SRAM0_BASE = 0x%x \n",SRAM0_BASE );
    
    uint32_t read_data;
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test DATA line\n");
    rumboot_printf("WRITE SSRAM\n");
    iowrite32(0x55555555, SRAM0_BASE);
    iowrite32(0xaaaaaaaa, SRAM0_BASE + 4);
    rumboot_printf("READ SSRAM\n");
    read_data = ioread32(SRAM0_BASE);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE,read_data);
    if (read_data != 0x55555555) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE,read_data);
        rumboot_printf("wait data      = 0x%x \n",0x55555555);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    read_data = ioread32(SRAM0_BASE + 4);
    rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE + 4,read_data);
    if (read_data != 0xaaaaaaaa) {
        rumboot_printf("--------------------------------\n");
        rumboot_printf("ERROR DATA !!!\n");
        rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE + 4,read_data);
        rumboot_printf("wait data      = 0x%x \n",0xaaaaaaaa);
        rumboot_printf("--------------------------------\n");
        test_result = 1;
        return 1;
    }
    
    rumboot_printf("------------------------------------------------------------------------ \n");
    rumboot_printf("Test all mem\n");
    rumboot_printf("WRITE SRAM0\n");
    for (int i=0; i<(0x1<<20); i++) {  //20
        iowrite32(i, SRAM0_BASE + i*4);
    }
    
    rumboot_printf("READ SRAM0\n");
    for (int i=0; i<(0x1<<20); i++) {  //20
        read_data = ioread32(SRAM0_BASE + i*4);
//        rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE + i*4,read_data);
        if (read_data != i) {
            rumboot_printf("--------------------------------\n");
            rumboot_printf("ERROR DATA !!!\n");
            rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE + i*4,read_data);
            rumboot_printf("wait data      = 0x%x \n",i);
            rumboot_printf("--------------------------------\n");
            test_result = 1;
            return 1;
        }
        else if ((i & 0xffff) == 0xffff) {
            rumboot_printf("ioread32(0x%x) = 0x%x \n",SRAM0_BASE + i*4,read_data);
        }
    }
       
    if (!test_result)
        rumboot_putstring("Test has been finished successfully.");

    return test_result;
}
