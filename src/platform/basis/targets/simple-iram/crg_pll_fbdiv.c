#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/gic.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <platform/basis/platform/devices.h>

#include <regs/regs_crg.h> 
	 
void read_APB_CLKDIV() 
{
	int a;
	rumboot_printf("Checking read PLL PRDIV");
	a = ioread32(CRG_SYS_BASE + CRG_PLL_PRDIV);
	//rumboot_printf("Checking read CLKDIV end");
	rumboot_printf("PRDIV= %x", a);
}

void change_pll(int fbdiv, int prdiv, int psdiv)
{
	rumboot_printf("Change settings PLL\n");
	iowrite32(fbdiv, CRG_SYS_BASE + CRG_PLL_FBDIV);
	iowrite32(prdiv, CRG_SYS_BASE + CRG_PLL_PRDIV);
	iowrite32(psdiv, CRG_SYS_BASE + CRG_PLL_PSDIV);
	rumboot_printf("Changes is done\n");
}

void change_mode_pll(int mode)
{
	rumboot_printf("Change mode of PLL\n");
	iowrite32(mode, CRG_SYS_BASE + CRG_PLL_CTRL);
	rumboot_printf("Mode of PLL was changed\n");
	
}

int main(void)
{
	read_APB_CLKDIV();
	rumboot_printf("Start CRG PLL reconfiguration test\n"); 
	int fbdiv_ch;
	//read_APB_CLKDIV();
	iowrite32(0x1ACCE551, CRG_SYS_BASE + CRG_WR_LOCK);
	rumboot_printf("Changing FBDIV");
	for(int i=0; i<10; i++){
		fbdiv_ch = 99 + 5*i;
		change_mode_pll(0x3);
		change_pll(fbdiv_ch, 0,0);	
		change_mode_pll(0x0);
		do{
			pll_r = ioread32(CRG_SYS_BASE + CRG_PLL_STAT);
			pll_ch = pll_r  & 0xFFFFFFEF;
			rumboot_printf("reg pll_r= %x\n", pll_ch);
			if(pll_ch == 0x0) 
				rumboot_printf("Wait ready from PLL\n");
			else 
			if(pll_ch == 0x1){
				break;
			}
		} while(1);
		rumboot_printf("PLL is ready, next iteration\n");
	}
	rumboot_printf("Stop changing FBDIV");
	
	return 0;
}
