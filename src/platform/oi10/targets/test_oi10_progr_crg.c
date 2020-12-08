#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <arch/ppc_476fp_config.h>
#include <platform/devices.h>


#include <regs/regs_gpio_pl061.h>

int main(void)
{
    rumboot_printf("start test_oi10_progr_crg!\n");
    uint32_t test_result = 0;
      
    rumboot_printf( "Init PLL \n");    
    dcr_write(0x8008003c,0x1ACCE551);

    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));
  
    mdelay(500);
    dcr_write(0x80080004,0x3);
    
    dcr_write(0x80080018,0x4);
    mdelay(500);
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));
    
    dcr_write(0x80080004,0x00);
    
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080000,dcr_read(0x80080000));
    rumboot_printf( "dcr_read(0x%x) = 0x%x \n",0x80080018,dcr_read(0x80080018));

/*    
    uint32_t addr;
    rumboot_printf( "READ INIT VALUE CKDIVMODE\n");
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
    
    rumboot_printf( "WRITE CKDIVMODE \n");
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
*/    
    iowrite32 (0xff,GPIO_0_BASE+GPIO_DIR);
    for (int i=0; i<16; i++) {  
        mdelay(1000);
        rumboot_printf( "delay 0x%x \n",i);
        if (i%2 == 0)
            iowrite32 (0x00,GPIO_0_BASE+0x3fc);
        else
            iowrite32 (0xff,GPIO_0_BASE+0x3fc);
    }    

    return test_result;
}
