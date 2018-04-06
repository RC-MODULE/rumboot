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
	rumboot_printf("CLKDIV= %x", a);
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
	rumboot_printf("Start CRG PLL reconfiguration test\n"); 
	int pll_r, pll_ch;
	//int fbdiv_ch;
	//read_APB_CLKDIV();
	iowrite32(0x1ACCE551, CRG_SYS_BASE + CRG_WR_LOCK);
	//rumboot_printf("Changing FBDIV\n");
	
	
	rumboot_printf("Changing PRDIV and PSDIV\n");
	
	change_mode_pll(0x3);
	change_pll(1499, 9, 0);		//Change freq 16*(1500/10*1)=2400Mhz , PRDIV =10, PSDIV=1;
	//udelay(5);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	change_mode_pll(0x3);
	change_pll(1119, 7, 0);		//Change freq 16*(1120/8*1)=2240Mhz , PRDIV =8, PSDIV=1;
	//udelay(5);
	change_mode_pll(0x0);
	//udelay(150);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	change_mode_pll(0x3);
	change_pll(779, 5, 0);		//Change freq 16*(780/6*1)=2080Mhz , PRDIV =6, PSDIV=1;
	//udelay(5);
	change_mode_pll(0x0);
	//udelay(150);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	change_mode_pll(0x3);
	change_pll(499, 3, 0);		//Change freq 16*(500/4*1)=2000Mhz , PRDIV =4, PSDIV=1;
	//udelay(5);
	change_mode_pll(0x0);
	//udelay(200);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	change_mode_pll(0x3);
	change_pll(359, 2, 0);		//Change freq 16*(360/1*3)=1920Mhz , PRDIV =1, PSDIV=3;
	//udelay(5);
	change_mode_pll(0x0);
	//udelay(200);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	
	change_mode_pll(0x3);
	change_pll(219, 1, 0);		//Change freq 16*(220/2*1)=1760Mhz , PRDIV =2, PSDIV=1;
	//udelay(100);
	change_mode_pll(0x0);
	//udelay(400);
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
	//udelay(150);
	rumboot_printf("PLL is ready, next iteration\n");
	
	change_mode_pll(0x3);
	change_pll(99, 0, 0);
	//udelay(5);
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
	//udelay(150);
	rumboot_printf("PLL is ready, end of test\n");
	//udelay(200);
	return 0;
}
